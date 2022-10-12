/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2022 XIA LLC, All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file list_mode_file_stats.cpp
 * @brief Ingests a list-mode data file and validates its contents. Part of P16-502.
 */

#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include <args/args.hxx>
#include <nolhmann/json.hpp>

#include "pixie/data/list_mode.hpp"

struct LOG {
    explicit LOG(const std::string& type) {
        type_ = type;

        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::chrono::milliseconds now2 =
            std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        char timeBuffer[80];
        std::strftime(timeBuffer, 80, "%FT%T", gmtime(&currentTime));

        std::stringstream tmp;
        tmp << timeBuffer << "." << std::setfill('0') << std::setw(3) << now2.count() % 1000 << "Z";

        datetime_ = tmp.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const LOG& log) {
        os << log.datetime_ << " - " << log.type_ << " - ";
        return os;
    }

    std::string type_;
    std::string datetime_;
};

struct config {
    ///TODO: Add a set of expected data that can be configured on a per-channel basis. E.g. esums.
    size_t frequency;
    size_t revision;
    size_t slot;
    config() : frequency(0), revision(0), slot(0) {}
};

using configs = std::vector<config>;

struct channel_info {
    size_t id;
    size_t count;
    double cps;
    double energy_ave;
    double energy_min;
    double energy_max;
    double event_length_ave;
    double header_length_ave;
    double time_min;
    double time_max;
    double trace_length_ave;
};

void to_json(nlohmann::json& j, const channel_info& ch) {
    j = nlohmann::json{
        {"id", ch.id},
        {"count", ch.count},
        {"count_per_second", ch.cps},
        {"energy", {{"max", ch.energy_max}, {"min", ch.energy_min}, {"ave", ch.energy_ave}}},
        {"time", {{"max", ch.time_max}, {"min", ch.time_min}}},
        {"event_length", ch.event_length_ave},
        {"trace_length", ch.trace_length_ave},
        {"header_length", ch.header_length_ave},
    };
}

using channel_id = size_t;
using channel_stats = std::map<channel_id, channel_info>;

void verify_json_slot(const nlohmann::json& node) {
    if (!node.contains("slot")) {
        throw std::invalid_argument("Missing slot definition in configuration element.");
    }

    if (!node.contains("revision")) {
        throw std::invalid_argument("Missing revision object in configuration element.");
    }

    if (!node.contains("frequency")) {
        throw std::invalid_argument("Missing frequency object in configuration element.");
    }
}

void read_config(const std::string& file, configs& cfgs) {
    std::ifstream input(file, std::ios::in);
    if (input.fail()) {
        throw std::ios_base::failure("open: " + file + ": " + std::strerror(errno));
    }

    nlohmann::json jf = nlohmann::json::parse(input);
    input.close();

    for (const auto& element : jf) {
        verify_json_slot(element);

        config mod_cfg;
        mod_cfg.slot = element["slot"];
        mod_cfg.revision = element["revision"];
        mod_cfg.frequency = element["frequency"];
        cfgs.push_back(mod_cfg);
    }
}

double calculate_duration_in_seconds(const std::chrono::system_clock::time_point& start,
                                     const std::chrono::system_clock::time_point& end) {
    return std::chrono::duration<double>(end - start).count();
}

int main(int argc, char** argv) {
    auto first_start = std::chrono::system_clock::now();
    auto start = first_start;
    args::ArgumentParser parser("Validates list-mode data files produced by the example software.");
    parser.LongSeparator("=");

    args::Group arguments(parser, "arguments", args::Group::Validators::AtLeastOne,
                          args::Options::Global);
    args::ValueFlag<std::string> conf_flag(
        arguments, "cfg", "The configuration file telling us what to expect in the list-mode data.",
        {'c', "config"}, args::Options::Required);
    args::HelpFlag help_flag(arguments, "help", "Displays this message", {'h', "help"});
    args::ValueFlag<std::string> input_flag(arguments, "input_file",
                                            "The input file we'll attempt to decode.",
                                            {'i', "input-file"}, args::Options::Required);

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help& help) {
        std::cout << LOG("INFO") << help.what() << std::endl;
        std::cout << parser;
        return EXIT_SUCCESS;
    } catch (args::Error& e) {
        std::cout << LOG("ERROR") << e.what() << std::endl;
        std::cout << parser;
        return EXIT_FAILURE;
    }

    configs cfgs;
    try {
        read_config(conf_flag.Get(), cfgs);
    } catch (std::exception& e) {
        std::cout << LOG("ERROR") << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << LOG("INFO") << "Finished reading config in "
              << calculate_duration_in_seconds(start, std::chrono::system_clock::now()) << " s."
              << std::endl;

    start = std::chrono::system_clock::now();
    std::cout << LOG("INFO") << "Starting to parse " << input_flag.Get() << std::endl;

    std::ifstream input(input_flag.Get(), std::ios::in | std::ios::binary | std::ios::ate);
    xia::pixie::data::list_mode::records records;
    xia::pixie::data::list_mode::buffer remainder;
    try {
        if (input.fail()) {
            throw std::ios_base::failure("open: " + input_flag.Get() + ": " + std::strerror(errno));
        }

        auto size = input.tellg();
        /*
         * We divide by 4 in the vector size as we're wanting to read 32-bit ints, which are read
         * using a char data type.
         */
        xia::pixie::data::list_mode::buffer data(size / 4, 0);
        input.seekg(0);
        input.read(reinterpret_cast<char*>(&data.data()[0]), size);
        input.close();

        xia::pixie::data::list_mode::decode_data_block(data, cfgs[0].revision, cfgs[0].frequency,
                                                       records, remainder);
    } catch (std::ios_base::failure& failure) {
        std::cout << LOG("ERROR") << failure.what() << std::endl;
    } catch (xia::pixie::error::error& sdkerr) {
        std::cout << LOG("ERROR") << sdkerr.what() << std::endl;
    }

    std::cout << LOG("INFO") << "Total records processed : " << records.size() << std::endl;

    channel_stats stats;
    for (const auto& record : records) {
        auto stat_rec = stats.find(record.channel_number);
        if (stat_rec == stats.end()) {
            stats[record.channel_number] = {record.channel_number,
                                            1,
                                            0,
                                            record.energy,
                                            record.energy,
                                            record.energy,
                                            static_cast<double>(record.event_length),
                                            static_cast<double>(record.header_length),
                                            record.time.count(),
                                            record.time.count(),
                                            static_cast<double>(record.trace_length)};
        } else {
            auto ch = &stat_rec->second;
            ch->count++;
            ch->energy_ave += record.energy;
            ch->trace_length_ave += record.trace_length;
            ch->header_length_ave += record.header_length;
            ch->event_length_ave += record.event_length;

            if (record.energy > ch->energy_max) {
                ch->energy_max = record.energy;
            }
            if (record.energy < ch->energy_min) {
                ch->energy_min = record.energy;
            }
            if (record.time.count() > ch->time_max) {
                ch->time_max = record.time.count();
            }
            if (record.time.count() < ch->time_min) {
                ch->time_min = record.time.count();
            }
        }
    }

    for (auto& stat : stats) {
        auto ch = &stat.second;
        ch->energy_ave /= ch->count;
        ch->cps = ch->count / (ch->time_max - ch->time_min);
        ch->header_length_ave /= ch->count;
        ch->trace_length_ave /= ch->count;
        ch->event_length_ave /= ch->count;
        nlohmann::json j = *ch;
        std::cout << LOG("INFO") << j.dump() << std::endl;
    }

    if (!remainder.empty()) {
        std::cout << LOG("WARN") << "Leftover Words: " << remainder.size() << std::endl;
    }

    std::cout << LOG("INFO") << "Finished execution in "
              << calculate_duration_in_seconds(first_start, std::chrono::system_clock::now())
              << " s." << std::endl;
    return EXIT_SUCCESS;
}