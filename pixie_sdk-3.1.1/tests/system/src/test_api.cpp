/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
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

/** @file test_api.cpp
 * @brief A C++ test interface to the PixieSDK.
 */

#include <algorithm>
#include <cstring>
#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <sstream>

#include <pixie/config.hpp>
#include <pixie/log.hpp>
#include <pixie/util.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/legacy.hpp>
#include <pixie/pixie16/sim.hpp>

#include <args/args.hxx>

/*
 * Localize the log and error
 */
using xia_log = xia::log;
using error = xia::pixie::error::error;

/*
 * Module threading worker base
 */
struct module_thread_worker {
    int number;
    int slot;
    int pci_bus;
    int pci_slot;

    std::atomic_bool running;
    bool has_error;
    xia::util::timepoint period;
    size_t total;
    size_t last_total;

    module_thread_worker();

    virtual void worker(xia::pixie::module::module& module) = 0;
};

/*
 * Command processor.
 */
using options = std::vector<std::string>;
using commands = std::vector<options>;

struct command_def {
    std::vector<size_t> counts;
    std::string help;
};

struct cmd_handler {
    std::string cmd;
    void (*func)(xia::pixie::crate::crate& crate, options& cmd);
};

static void help(xia::pixie::crate::crate& crate, options& cmd);
static void boot(xia::pixie::crate::crate& crate, options& cmd);
static void crate_report(xia::pixie::crate::crate& crate, options& cmd);
static void adj_off(xia::pixie::crate::crate& crate, options& cmd);
static void set_dacs(xia::pixie::crate::crate& crate, options& cmd);
static void adc_acq(xia::pixie::crate::crate& crate, options& cmd);
static void adc_save(xia::pixie::crate::crate& crate, options& cmd);
static void bl_acq(xia::pixie::crate::crate& crate, options& cmd);
static void bl_save(xia::pixie::crate::crate& crate, options& cmd);
static void hist_start(xia::pixie::crate::crate& crate, options& cmd);
static void hist_resume(xia::pixie::crate::crate& crate, options& cmd);
static void hist_save(xia::pixie::crate::crate& crate, options& cmd);
static void list_start(xia::pixie::crate::crate& crate, options& cmd);
static void list_resume(xia::pixie::crate::crate& crate, options& cmd);
static void list_save(xia::pixie::crate::crate& crate, options& cmd);
static void run_active(xia::pixie::crate::crate& crate, options& cmd);
static void run_end(xia::pixie::crate::crate& crate, options& cmd);
static void par_write(xia::pixie::crate::crate& crate, options& cmd);
static void par_read(xia::pixie::crate::crate& crate, options& cmd);
static void var_write(xia::pixie::crate::crate& crate, options& cmd);
static void var_read(xia::pixie::crate::crate& crate, options& cmd);
static void lset_report(xia::pixie::crate::crate& crate, options& cmd);
static void lset_import(xia::pixie::crate::crate& crate, options& cmd);
static void lset_load(xia::pixie::crate::crate& crate, options& cmd);
static void stats(xia::pixie::crate::crate& crate, options& cmd);
static void import(xia::pixie::crate::crate& crate, options& cmd);
static void export_(xia::pixie::crate::crate& crate, options& cmd);
static void test(xia::pixie::crate::crate& crate, options& cmd);
static void wait(xia::pixie::crate::crate& crate, options& cmd);

static const std::map<std::string, command_def> command_defs = {
    {"help", {{0}, "command help"}},
    {"crate", {{0}, "report the crate"}},
    {"boot", {{0}, "boots the module(s)"}},
    {"adj-off", {{1}, "adjust the module's offsets"}},
    {"set-dacs", {{1}, "set the module's DACs"}},
    {"adc-acq", {{1}, "acquire a module's ADC trace"}},
    {"adc-save", {{1, 2, 3}, "save a module's ADC trace to a file"}},
    {"bl-acq", {{1}, "acquire the module's baselines"}},
    {"bl-save", {{1, 2}, "save the module's baselines"}},
    {"run-active", {{1}, "does the module have an active run?"}},
    {"run-end", {{1}, "end the module's run"}},
    {"hist-start", {{1}, "start module histograms"}},
    {"hist-resume", {{1}, "resume module histograms"}},
    {"hist-save", {{1, 2, 3}, "save a module's histogram to a file"}},
    {"list-start", {{1}, "start module list mode"}},
    {"list-resume", {{1}, "resume module list mode"}},
    {"list-save", {{3}, "save a module's histogram to a file"}},
    {"par-read", {{2, 3}, "read module/channel parameter"}},
    {"par-write", {{3, 4}, "write module/channel parameter"}},
    {"var-read", {{2, 3, 4}, "read module/channel variable"}},
    {"var-write", {{3, 4, 5}, "write module/channel variable"}},
    {"lset-report", {{2}, "output a legacy settings file in a readable format"}},
    {"lset-import", {{2, 3}, "import a legacy settings file to a module"}},
    {"lset-load", {{2, 3}, "load a legacy settings file to a module's DSP memory"}},
    {"stats", {{2, 3}, "module/channel stats"}},
    {"import", {{1}, "import a JSON configuration file"}},
    {"export", {{1}, "export a configuration to a JSON file"}},
    {"test", {{2}, "start a test"}},
    {"wait", {{1}, "wait a number of msecs"}}};

static const std::vector<cmd_handler> cmd_handlers = {
    {"help", help},
    {"crate", crate_report},
    {"boot", boot},
    {"adj-off", adj_off},
    {"set-dacs", set_dacs},
    {"adc-acq", adc_acq},
    {"adc-save", adc_save},
    {"bl-acq", bl_acq},
    {"bl-save", bl_save},
    {"hist-start", hist_start},
    {"hist-resume", hist_resume},
    {"hist-save", hist_save},
    {"list-start", list_start},
    {"list-resume", list_resume},
    {"list-save", list_save},
    {"run-active", run_active},
    {"run-end", run_end},
    {"par-write", par_write},
    {"par-read", par_read},
    {"var-write", var_write},
    {"var-read", var_read},
    {"lset-report", lset_report},
    {"lset-import", lset_import},
    {"lset-load", lset_load},
    {"stats", stats},
    {"import", import},
    {"export", export_},
    {"test", test},
    {"wait", wait},
};

static std::string adc_prefix = "adc-trace";
static std::string histogram_prefix = "histo";
static std::string baseline_prefix = "baselines";

static bool check_number(const std::string& opt) {
    return std::regex_match(opt, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
}

template<typename T>
static T get_value(const std::string& opt) {
    if (!check_number(opt)) {
        throw std::runtime_error("invalid number: " + opt);
    }
    std::istringstream iss(opt);
    T value;
    iss >> value;
    return value;
}

template<typename T>
static std::vector<T> get_values(const std::string& opt, const size_t max_count) {
    std::vector<T> values;
    if (opt == "all") {
        values.resize(max_count);
        std::iota(values.begin(), values.end(), 0);
    } else {
        xia::util::strings sc;
        xia::util::split(sc, opt, ',');
        for (auto& slots : sc) {
            xia::util::strings sd;
            xia::util::split(sd, slots, '-');
            if (sd.size() == 1) {
                values.push_back(get_value<T>(sd[0]));
            } else if (sd.size() == 2) {
                auto start = get_value<T>(sd[0]);
                auto end = get_value<T>(sd[1]);
                if (start > end) {
                    throw std::runtime_error("invalid range, start before end: " + opt);
                }
                for (T s = start; s <= end; ++s) {
                    values.push_back(s);
                }
            } else {
                throw std::runtime_error("invalid range: " + opt);
            }
        }
    }
    return values;
}

static void module_check(xia::pixie::crate::crate& crate, std::vector<size_t> mod_nums) {
    for (auto mod_num : mod_nums) {
        if (mod_num > crate.num_modules) {
            throw std::runtime_error(
                std::string("invalid module number: " + std::to_string(mod_num)));
        }
        if (!crate.modules[mod_num]->online()) {
            throw std::runtime_error(std::string("module offline: " + std::to_string(mod_num)));
        }
    }
}

template<typename V>
static void output_value(const std::string& name, V value) {
    xia::util::ostream_guard oguard(std::cout);
    std::cout << name << " = " << value;
    if (!std::is_same<V, double>::value) {
        std::cout << std::hex << " (0x" << value << ')';
    }
    std::cout << std::endl;
}

static bool make_command_sets(const std::vector<std::string>& cmd_strings, commands& cmds) {
    options option;
    for (auto opt : cmd_strings) {
        if (option.empty()) {
            auto search = command_defs.find(opt);
            if (search == command_defs.end()) {
                std::cerr << "error: invalid command: " << opt << std::endl;
                return false;
            }
        }
        option.push_back(opt);
        auto search = command_defs.find(option[0]);
        auto counts = search->second.counts;
        auto min = std::min_element(counts.begin(), counts.end());
        auto max = std::max_element(counts.begin(), counts.end());
        if (*min != *max && option.size() > *min) {
            search = command_defs.find(opt);
            if (search != command_defs.end()) {
                if (option.size() > 1) {
                    option.pop_back();
                }
                cmds.push_back(option);
                option.clear();
                option.push_back(opt);
            }
        }
        if (option.size() > *max) {
            cmds.push_back(option);
            option.clear();
        }
    }
    if (!option.empty()) {
        auto search = command_defs.find(option[0]);
        auto counts = search->second.counts;
        auto min = std::min_element(counts.begin(), counts.end());
        if (option.size() <= *min) {
            std::cerr << "error: malformed command:";
            for (auto& opt : option) {
                std::cerr << ' ' << opt;
            }
            std::cerr << std::endl;
            return false;
        }
        cmds.push_back(option);
    }
    return true;
}

static bool make_command_sets(args::PositionalList<std::string>& cmd, commands& cmds) {
    std::vector<std::string> cmd_strings;
    for (auto opt : args::get(cmd)) {
        cmd_strings.push_back(opt);
    }
    return make_command_sets(cmd_strings, cmds);
}

static bool make_command_sets(std::string cmd_file, commands& cmds) {
    std::ifstream file(cmd_file, std::ios::binary);
    if (!file) {
        std::cerr << "error: cannot open command file: " << cmd_file << std::endl;
        return false;
    }
    std::vector<std::string> cmd_strings;
    for (std::string line; std::getline(file, line);) {
        xia::util::ltrim(line);
        if (!line.empty() && line[0] != '#') {
            xia::util::strings ss;
            xia::util::split(ss, line);
            for (auto& s : ss) {
                cmd_strings.push_back(s);
            }
        }
    }
    file.close();
    return make_command_sets(cmd_strings, cmds);
}

template<typename W>
void module_threads(xia::pixie::crate::crate& crate, std::vector<size_t>& mod_nums,
                    std::vector<W>& workers, std::string error_message,
                    bool show_performance = true) {
    if (workers.size() != mod_nums.size()) {
        throw std::runtime_error("workers and modules counts mismatch");
    }
    using promise_error = std::promise<error::code>;
    using future_error = std::future<error::code>;
    std::vector<promise_error> promises(mod_nums.size());
    std::vector<future_error> futures;
    std::vector<std::thread> threads;
    for (size_t m = 0; m < mod_nums.size(); ++m) {
        auto module = crate.modules[mod_nums[m]];
        auto& worker = workers[m];
        futures.push_back(future_error(promises[m].get_future()));
        threads.push_back(std::thread([m, &promises, module, &worker] {
            try {
                worker.running = true;
                worker.worker(*module);
                promises[m].set_value(error::code::success);
            } catch (xia::pixie::error::error& e) {
                promises[m].set_value(e.type);
            } catch (...) {
                try {
                    promises[m].set_exception(std::current_exception());
                } catch (...) {
                }
            }
            worker.running = false;
        }));
    }
    error::code first_error = error::code::success;
    size_t finished = 0;
    size_t show_secs = 5;
    xia::util::timepoint duration(true);
    xia::util::timepoint interval(true);
    while (finished != threads.size()) {
        finished = threads.size();
        for (size_t t = 0; t < threads.size(); ++t) {
            auto& future = futures[t];
            if (future.valid()) {
                auto zero = std::chrono::seconds(0);
                if (future.wait_for(zero) == std::future_status::ready) {
                    workers[t].period.stop();
                    error::code e = future.get();
                    if (e != error::code::success) {
                        std::cout << "module " << t
                                  << ": error: " << xia::pixie::error::api_result_text(e)
                                  << std::endl;
                    }
                    if (first_error == error::code::success) {
                        first_error = e;
                    }
                    threads[t].join();
                } else {
                    --finished;
                }
            }
        }
        xia::pixie::hw::wait(20 * 1000);
        if (show_performance && interval.secs() > show_secs) {
            auto secs = interval.secs();
            interval.restart();
            std::cout << "running: " << threads.size() - finished << std::endl;
            size_t all_total = 0;
            for (auto& w : workers) {
                if (w.period.secs() > 0) {
                    auto total = w.total;
                    auto bytes = (total - w.last_total) * sizeof(xia::pixie::hw::word);
                    auto rate = double(bytes) / secs;
                    all_total += total;
                    bytes = total * sizeof(xia::pixie::hw::word);
                    char active = w.running.load() ? '>' : ' ';
                    w.last_total = total;
                    std::ostringstream oss;
                    oss << ' ' << active << std::setw(2) << w.number << ": total: " << std::setw(8)
                        << xia::util::humanize(bytes) << " rate: " << std::setw(8)
                        << xia::util::humanize(rate) << " bytes/sec pci: bus=" << w.pci_bus
                        << " slot=" << w.pci_slot;
                    std::cout << oss.str() << std::endl;
                    xia_log(xia_log::info) << oss.str();
                } else {
                    std::cout << ' ' << std::setw(2) << w.number << ": not running" << std::endl;
                }
            }
            all_total *= sizeof(xia::pixie::hw::word);
            std::ostringstream oss;
            oss << " all: total: " << std::setw(8) << xia::util::humanize(all_total)
                << " rate: " << std::setw(8)
                << xia::util::humanize(double(all_total) / duration.secs()) << " bytes/sec";
            std::cout << oss.str() << std::endl;
            xia_log(xia_log::info) << oss.str();
        }
    }
    if (first_error != error::code::success) {
        throw error(first_error, error_message);
    }
}

module_thread_worker::module_thread_worker()
    : number(-1), slot(-1), pci_bus(-1), pci_slot(-1), running(false), has_error(false), total(0),
      last_total(0) {}

template<typename W>
void set_num_slot(xia::pixie::crate::crate& crate, std::vector<size_t>& mod_nums,
                  std::vector<W>& workers) {
    for (size_t m = 0; m < mod_nums.size(); ++m) {
        auto module = crate.modules[mod_nums[m]];
        auto& worker = workers[m];
        worker.number = module->number;
        worker.slot = module->slot;
        worker.pci_bus = module->pci_bus();
        worker.pci_slot = module->pci_slot();
    }
}

template<typename W>
void performance_stats(std::vector<W>& workers, bool show_workers = false) {
    size_t total = 0;
    size_t secs = 0;
    for (auto& w : workers) {
        if (w.period.secs() > secs) {
            secs = size_t(w.period.secs());
        }
        total += w.total;
        if (show_workers) {
            if (w.has_error) {
                std::stringstream he_oss;
                he_oss << "module: num:" << std::setw(2) << w.number << " slot:" << std::setw(2)
                       << w.slot << ": has an error; check the log";
                std::cout << he_oss.str() << std::endl;
                xia_log(xia_log::info) << he_oss.str();
            }
            std::stringstream dr_oss;
            auto bytes = w.total * sizeof(xia::pixie::hw::word);
            auto rate = double(bytes) / w.period.secs();
            dr_oss << "module: num:" << std::setw(2) << w.number << " slot:" << std::setw(2)
                   << w.slot << ": data received: " << std::setw(8) << xia::util::humanize(bytes)
                   << " bytes (" << std::setw(9) << bytes << "), rate: " << std::setw(8)
                   << xia::util::humanize(rate) << " bytes/sec pci: bus=" << w.pci_bus
                   << " slot=" << w.pci_slot;
            std::cout << dr_oss.str() << std::endl;
            xia_log(xia_log::info) << dr_oss.str();
        }
    }
    total *= sizeof(xia::pixie::hw::word);
    std::stringstream oss;
    oss << "data received: " << xia::util::humanize(total) << " bytes (" << total
        << "), rate: " << xia::util::humanize(double(total) / secs, " bytes/sec");
    std::cout << oss.str() << std::endl;
    xia_log(xia_log::info) << oss.str();
}

static void help(xia::pixie::crate::crate&, options&) {
    std::cout << "Command help:" << std::endl;
    auto mi = std::max_element(command_defs.begin(), command_defs.end(), [](auto& a, auto& b) {
        return (std::get<0>(a).size() < std::get<0>(b).size());
    });
    auto max = std::get<0>(*mi).size();
    for (auto& cmd_def : command_defs) {
        std::cout << std::setw(max + 1) << std::get<0>(cmd_def) << " " << std::get<1>(cmd_def).help
                  << std::endl;
    }
}

static void boot(xia::pixie::crate::crate& crate, options&) {
    xia::util::timepoint tp;
    std::cout << "booting crate" << std::endl;
    tp.start();
    crate.boot();
    tp.end();
    std::cout << "boot time=" << tp << std::endl;
}

static void crate_report(xia::pixie::crate::crate& crate, options&) {
    std::cout << crate << std::endl;
}

static void adj_off(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_num = get_value<size_t>(cmd[1]);
    crate[mod_num].adjust_offsets();
}

static void set_dacs(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_num = get_value<size_t>(cmd[1]);
    crate[mod_num].set_dacs();
}

static void adc_acq(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_num = get_value<size_t>(cmd[1]);
    crate[mod_num].get_traces();
}

static void adc_save(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_num = get_value<size_t>(cmd[1]);
    xia::pixie::channel::range channels;
    size_t length = xia::pixie::hw::max_adc_trace_length;
    if (cmd.size() == 3) {
        auto value = get_value<size_t>(cmd[2]);
        if (value > crate[mod_num].num_channels) {
            length = value;
        } else {
            channels.resize(1);
            channels[0] = value;
        }
    } else if (cmd.size() == 4) {
        channels.resize(1);
        channels[0] = get_value<size_t>(cmd[2]);
        length = get_value<size_t>(cmd[3]);
    }
    if (channels.empty()) {
        channels.resize(crate[mod_num].num_channels);
        xia::pixie::channel::range_set(channels);
    }

    std::vector<xia::pixie::hw::adc_trace> traces;
    std::ostringstream name;
    name << std::setfill('0') << adc_prefix << '-' << std::setw(2) << mod_num << ".csv";
    std::ofstream out(name.str());
    out << "bin,";

    for (auto channel : channels) {
        xia::pixie::hw::adc_trace adc_trace(length);
        crate[mod_num].read_adc(channel, adc_trace, false);
        traces.push_back(adc_trace);
        out << "Chan" << channel << ",";
    }

    out << std::endl;

    for (unsigned int bin = 0; bin < length; bin++) {
        out << bin << ",";
        for (auto trc : traces) {
            out << trc[bin] << ",";
        }
        out << std::endl;
    }
}

static void bl_acq(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        crate[mod_num].acquire_baselines();
    }
}

static void bl_save(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        xia::pixie::channel::range channels;
        if (cmd.size() == 3) {
            channels = get_values<size_t>(cmd[2], crate[mod_num].num_channels);
        }
        if (channels.empty()) {
            channels.resize(crate[mod_num].num_channels);
            xia::pixie::channel::range_set(channels);
        }
        xia::pixie::channel::baseline::channels_values baselines(crate[mod_num].num_channels);
        crate[mod_num].bl_get(channels, baselines, false);

        std::ostringstream name;
        name << std::setfill('0') << baseline_prefix << '-' << std::setw(2) << mod_num << ".csv";
        std::ofstream out(name.str());
        out << "sample, time,";

        for (auto channel : channels) {
            out << "Chan" << channel << ",";
        }
        out << std::endl;

        for (unsigned int sample = 0; sample < baselines.front().size(); sample++) {
            out << sample << "," << std::get<0>(baselines.front()[sample]) << ",";
            for (auto chan : channels) {
                out << std::get<1>(baselines[chan][sample]) << ",";
            }
            out << std::endl;
        }
    }
}

static void run_active(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        std::cout << "module=" << mod_num << " run-active=" << std::boolalpha
                  << crate[mod_num].run_active() << std::endl;
    }
}

static void hist_start(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_num = get_value<size_t>(cmd[1]);
    using namespace xia::pixie::hw::run;
    crate[mod_num].start_histograms(run_mode::new_run);
}

static void hist_resume(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_num = get_value<size_t>(cmd[1]);
    using namespace xia::pixie::hw::run;
    crate[mod_num].start_histograms(run_mode::resume);
}

static void hist_save(xia::pixie::crate::crate& crate, options& cmd) {
    /*
     * hist-save <mod> [channels [length]]
     */
    auto mod_num = get_value<size_t>(cmd[1]);
    using namespace xia::pixie::hw::run;
    xia::pixie::channel::range channels;
    size_t length = xia::pixie::hw::max_histogram_length;
    if (cmd.size() == 3) {
        auto chans = get_values<size_t>(cmd[2], crate[mod_num].num_channels);
        if (chans.size() == 1 && chans[0] > crate[mod_num].num_channels) {
            length = chans[0];
        } else {
            for (auto c : chans) {
                channels.push_back(c);
            }
        }
    } else if (cmd.size() == 4) {
        auto chans = get_values<size_t>(cmd[2], crate[mod_num].num_channels);
        for (auto c : chans) {
            channels.push_back(c);
        }
        length = get_value<size_t>(cmd[3]);
    }
    if (channels.empty()) {
        channels.resize(crate[mod_num].num_channels);
        xia::pixie::channel::range_set(channels);
    }

    std::ostringstream name;
    name << std::setfill('0') << histogram_prefix << '-' << std::setw(2) << mod_num << ".csv";
    std::ofstream out(name.str());
    out << "bin,";

    std::vector<xia::pixie::hw::words> histos;
    for (auto channel : channels) {
        xia::pixie::hw::words histogram(length);
        crate[mod_num].read_histogram(channel, histogram);
        histos.push_back(histogram);
        out << "Chan" << channel << ",";
    }
    out << std::endl;

    for (unsigned int bin = 0; bin < length; bin++) {
        out << bin << ",";
        for (auto hist : histos) {
            out << hist[bin] << ",";
        }
        out << std::endl;
    }
}

static void list_start(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        using namespace xia::pixie::hw::run;
        crate[mod_num].start_listmode(run_mode::new_run);
    }
}

static void list_resume(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        using namespace xia::pixie::hw::run;
        crate[mod_num].start_listmode(run_mode::resume);
    }
}

struct list_save_worker : public module_thread_worker {
    std::string name;
    size_t seconds;

    list_save_worker();
    void worker(xia::pixie::module::module& module);
};

list_save_worker::list_save_worker() : seconds(0) {}

void list_save_worker::worker(xia::pixie::module::module& module) {
    name += '-' + std::to_string(module.number) + ".lmd";
    std::ofstream out(name, std::ios::binary);
    if (!out) {
        throw std::runtime_error(std::string("list mode file open: ") + name + ": " +
                                 std::strerror(errno));
    }
    const size_t poll_period_usecs = 100 * 1000;
    total = 0;
    period.start();
    while (period.secs() < seconds) {
        size_t data_available = module.read_list_mode_level();
        if (data_available > 0) {
            xia::pixie::hw::words lm;
            module.read_list_mode(lm);
            total += lm.size();
            out.write(reinterpret_cast<char*>(lm.data()), lm.size() * sizeof(xia::pixie::hw::word));
        }
        if (data_available == 0) {
            xia::pixie::hw::wait(poll_period_usecs);
        }
    }
    period.end();
}

static void list_save(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    auto secs = get_value<size_t>(cmd[2]);
    auto name = cmd[3];
    module_check(crate, mod_nums);
    if (secs == 0) {
        throw std::runtime_error(std::string("list mode save period is 0"));
    }
    auto saves = std::vector<list_save_worker>(mod_nums.size());
    set_num_slot(crate, mod_nums, saves);
    for (auto& s : saves) {
        s.name = name;
        s.seconds = secs;
    };
    module_threads(crate, mod_nums, saves, "list mode save error; see log");
    performance_stats(saves);
}

static void run_end(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        crate[mod_num].run_end();
    }
}

static void par_write(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        if (cmd.size() == 4) {
            auto value = get_value<xia::pixie::param::value_type>(cmd[3]);
            crate[mod_num].write(cmd[2], value);
        } else {
            auto channel_num = get_value<size_t>(cmd[2]);
            auto value = get_value<double>(cmd[4]);
            crate[mod_num].write(cmd[3], channel_num, value);
        }
    }
}

static void par_read(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        int reg_name;
        if (cmd.size() == 3) {
            reg_name = 2;
            std::cout << "# module param read: " << mod_num << ": " << cmd[reg_name] << std::endl;
            if (cmd[reg_name] == "all") {
                for (auto& par : xia::pixie::param::get_module_param_map()) {
                    try {
                        output_value(par.first, crate[mod_num].read(par.second));
                    } catch (error& e) {
                        if (e.type != error::code::module_param_disabled &&
                            e.type != error::code::module_param_writeonly) {
                            throw;
                        }
                    }
                }
            } else {
                output_value(cmd[reg_name], crate[mod_num].read(cmd[reg_name]));
            }
        } else {
            reg_name = 3;
            auto channel_num = get_value<size_t>(cmd[2]);
            std::cout << "# channel param read: " << mod_num << ':' << channel_num << ": "
                      << cmd[reg_name] << std::endl;
            if (cmd[reg_name] == "all") {
                for (auto& par : xia::pixie::param::get_channel_param_map()) {
                    try {
                        output_value(par.first, crate[mod_num].read(par.second, channel_num));
                    } catch (error& e) {
                        if (e.type != error::code::channel_param_disabled &&
                            e.type != error::code::channel_param_writeonly) {
                            throw;
                        }
                    }
                }
            } else {
                output_value(cmd[reg_name], crate[mod_num].read(cmd[reg_name], channel_num));
            }
        }
    }
}

static void var_write(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    int reg_name;
    size_t channel = 0;
    size_t offset = 0;
    int val;
    switch (cmd.size()) {
        case 4:
            reg_name = 2;
            val = 3;
            break;
        case 5:
            channel = get_value<size_t>(cmd[2]);
            reg_name = 3;
            val = 4;
            break;
        case 6:
        default:
            channel = get_value<size_t>(cmd[2]);
            reg_name = 3;
            val = 4;
            offset = get_value<size_t>(cmd[5]);
            break;
    }
    for (auto mod_num : mod_nums) {
        auto value = get_value<xia::pixie::param::value_type>(cmd[val]);
        crate[mod_num].write_var(cmd[reg_name], value, channel, offset);
    }
}

static void var_read(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    bool is_module = false;
    int reg_name;
    size_t channel = 0;
    size_t offset = 0;
    switch (cmd.size()) {
        case 3:
            is_module = true;
            reg_name = 2;
            break;
        case 4:
            channel = get_value<size_t>(cmd[2]);
            reg_name = 3;
            break;
        case 5:
        default:
            channel = get_value<size_t>(cmd[2]);
            reg_name = 3;
            offset = get_value<size_t>(cmd[4]);
            break;
    }
    for (auto mod_num : mod_nums) {
        if (cmd[reg_name] == "all") {
            if (is_module) {
                std::cout << "# module var read: " << mod_num << ": " << cmd[reg_name] << std::endl;
                for (auto& var : crate[mod_num].module_var_descriptors) {
                    try {
                        output_value(var.name, crate[mod_num].read_var(var.par));
                    } catch (error& e) {
                        if (e.type != error::code::module_param_disabled &&
                            e.type != error::code::module_param_writeonly) {
                            throw;
                        }
                    }
                }
            } else {
                std::cout << "# channel var read: " << mod_num << ':' << channel << ": "
                          << cmd[reg_name] << std::endl;
                for (auto& var : crate[mod_num].channel_var_descriptors) {
                    try {
                        output_value(var.name, crate[mod_num].read_var(var.par, channel, offset));
                    } catch (error& e) {
                        if (e.type != error::code::channel_param_disabled &&
                            e.type != error::code::channel_param_writeonly) {
                            throw;
                        }
                    }
                }
            }
        } else {
            output_value(cmd[reg_name], crate[mod_num].read_var(cmd[reg_name], channel, offset));
        }
    }
}

static void lset_report(xia::pixie::crate::crate& crate, options& cmd) {
    /*
     * lset-report <module> <settings file>
     */
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        xia::pixie::legacy::settings settings(crate[mod_num]);
        settings.load(cmd[2]);
        std::cout << settings;
    }
}

static void lset_import(xia::pixie::crate::crate& crate, options& cmd) {
    /*
     * lset-import <module> <settings file> [flush/sync]
     */
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        xia::pixie::module::module& module = crate[mod_num];
        xia::pixie::legacy::settings settings(module);
        settings.load(cmd[2]);
        settings.import(module);
        if (module.online() && cmd.size() == 4) {
            if (cmd[3] == "flush") {
                module.sync_vars();
            } else if (cmd[3] == "sync") {
                module.sync_vars();
                module.sync_hw();
            } else {
                throw std::runtime_error(
                    std::string("invalid post settingsimport operation: " + cmd[3]));
            }
        }
    }
}

static void lset_load(xia::pixie::crate::crate& crate, options& cmd) {
    /*
     * lset-import <module> <settings file> [flush/sync]
     */
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        xia::pixie::module::module& module = crate[mod_num];
        xia::pixie::legacy::settings settings(module);
        settings.load(cmd[2]);
        settings.write(module);
        if (module.online() && cmd.size() == 4) {
            if (cmd[3] == "flush") {
                module.sync_vars();
            } else if (cmd[3] == "sync") {
                module.sync_vars();
                module.sync_hw();
            } else {
                throw std::runtime_error(
                    std::string("invalid post settings load operation: " + cmd[3]));
            }
        }
    }
}

static void stats(xia::pixie::crate::crate& crate, options& cmd) {
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        std::string stat;
        xia::pixie::channel::range channels;
        if (cmd.size() == 3) {
            stat = cmd[2];
            channels.resize(crate[mod_num].num_channels);
            xia::pixie::channel::range_set(channels);
        } else {
            channels.resize(1);
            channels[0] = get_value<size_t>(cmd[2]);
            stat = cmd[3];
        }
        xia::pixie::stats::stats stats(crate[mod_num].configs);
        crate[mod_num].read_stats(stats);
        if (stat == "pe") {
            std::cout << "module " << mod_num
                      << ": processed-events=" << stats.mod.processed_events() << std::endl;
        } else if (stat == "icr") {
            for (auto channel : channels) {
                std::cout << "module " << mod_num << " chan " << channel
                          << ": input-count-rate=" << stats.chans[channel].input_count_rate()
                          << std::endl;
            }
        } else if (stat == "ocr") {
            for (auto channel : channels) {
                std::cout << "module " << mod_num << " chan " << channel
                          << ": output-count-rate=" << stats.chans[channel].output_count_rate()
                          << std::endl;
            }
        } else if (stat == "rt") {
            std::cout << "module " << mod_num << ": real-time=" << stats.mod.real_time()
                      << std::endl;
        } else if (stat == "lt") {
            for (auto channel : channels) {
                std::cout << "module " << mod_num << " chan " << channel
                          << ": live-time=" << stats.chans[channel].live_time() << std::endl;
            }
        }
    }
}

static void import(xia::pixie::crate::crate& crate, options& cmd) {
    xia::util::timepoint tp;
    xia::pixie::module::number_slots modules;
    tp.start();
    crate.import_config(cmd[1], modules);
    tp.end();
    std::cout << "Modules imported: " << modules.size() << " time=" << tp << std::endl;
}

static void export_(xia::pixie::crate::crate& crate, options& cmd) {
    xia::util::timepoint tp;
    tp.start();
    crate.export_config(cmd[1]);
    tp.end();
    std::cout << "Modules export time=" << tp << std::endl;
}

struct test_fifo_worker : public module_thread_worker {
    size_t length;

    test_fifo_worker();
    void worker(xia::pixie::module::module& module);
};

test_fifo_worker::test_fifo_worker() : length(0) {}

void test_fifo_worker::worker(xia::pixie::module::module& module) {
    try {
        module.start_test(xia::pixie::module::module::test::lm_fifo);
        const size_t poll_period_usecs = 10 * 1000;
        period.start();
        while (total < length) {
            size_t data_available = module.read_list_mode_level();
            if (data_available > 0) {
                xia::pixie::hw::words lm;
                module.read_list_mode(lm);
                total += lm.size();
            }
            if (data_available == 0) {
                xia::pixie::hw::wait(poll_period_usecs);
            }
        }
        period.end();
        module.end_test();
    } catch (...) {
        period.end();
        has_error = true;
        module.end_test();
        throw;
    }
}

static void test(xia::pixie::crate::crate& crate, options& cmd) {
    xia::pixie::module::module::test mode = xia::pixie::module::module::test::off;
    if (cmd[1] == "lmfifo") {
        mode = xia::pixie::module::module::test::lm_fifo;
    }
    if (mode == xia::pixie::module::module::test::off) {
        throw std::runtime_error(std::string("invalid test mode: " + cmd[1]));
    }
    size_t bytes = 500 * 1024 * 1000;
    auto mod_nums = get_values<size_t>(cmd[2], crate.num_modules);
    auto tests = std::vector<test_fifo_worker>(mod_nums.size());
    set_num_slot(crate, mod_nums, tests);
    for (auto& t : tests) {
        t.length = (bytes) / sizeof(xia::pixie::hw::word);
        ;
    };
    std::cout << "Test: " << cmd[1] << " length=" << xia::util::humanize(bytes) << std::endl;
    module_threads(crate, mod_nums, tests, "fifo test error; see log");
    performance_stats(tests, true);
}

static void wait(xia::pixie::crate::crate&, options& cmd) {
    auto msecs = get_value<size_t>(cmd[1]);
    std::cout << "waiting " << msecs << " msecs" << std::endl;
    xia::pixie::hw::wait(msecs * 1000);
}

static void initialize(xia::pixie::crate::crate& crate, bool reg_trace) {
    xia::util::timepoint tp;
    std::cout << "crate: initialize" << std::endl;
    tp.start();
    crate.initialize(reg_trace);
    tp.end();
    std::cout << "modules: detected=" << crate.modules.size() << " time=" << tp << std::endl;
}

static bool process_command_sets(xia::pixie::crate::crate& crate, commands& cmds) {
    for (auto& cmd : cmds) {
        xia_log(xia_log::info) << "test: cmd: " << xia::util::join<options>(cmd);
        for (const auto& handler : cmd_handlers) {
            if (handler.cmd == cmd[0]) {
                handler.func(crate, cmd);
                break;
            }
        }
    }
    return true;
}

void load_crate_firmware(const std::string& file, xia::pixie::firmware::crate& firmwares) {
    std::ifstream input(file, std::ios::in | std::ios::binary);
    if (!input) {
        throw std::runtime_error(std::string("crate firmware file open: ") + file + ": " +
                                 std::strerror(errno));
    }
    for (std::string line; std::getline(input, line);) {
        if (!line.empty()) {
            auto fw = xia::pixie::firmware::parse(line, ',');
            if (xia::pixie::firmware::check(firmwares, fw)) {
                std::string what("duplicate firmware option: ");
                what += line;
                throw std::runtime_error(what);
            }
            xia::pixie::firmware::add(firmwares, fw);
        }
    }
}

int main(int argc, char* argv[]) {
    args::ArgumentParser parser("Tests C++ API");

    parser.helpParams.addDefault = true;
    parser.helpParams.addChoices = true;

    args::Group option_group(parser, "Options");
    args::HelpFlag help(option_group, "help", "Display this help menu", {'h', "help"});
    args::Flag debug_flag(option_group, "debug_flag", "Enable debug log level", {'d', "debug"},
                          false);
    args::Flag throw_unhandled_flag(option_group, "throw_unhandled_flag",
                                    "Throw an unhandled exception, it will detail the exception",
                                    {'t', "throw-unhandled"}, false);
    args::Flag reg_trace(option_group, "reg_trace", "Registers debugging traces in the API.",
                         {'R', "reg-trace"});
    args::Flag simulate(option_group, "simulate", "Simulate the crate and modules",
                        {'S', "simulate"}, false);
    args::ValueFlag<size_t> num_modules_flag(
        option_group, "num_modules_flag", "Number of modules to report", {'n', "num-modules"}, 0);
    args::ValueFlagList<std::string> fw_file_flag(
        option_group, "fw_file_flag",
        "Firmware file(s) to load. Can be repeated. "
        "Takes the form rev:mod-rev-num:type:name"
        "Ex. r33339:15:sys:syspixie16_revfgeneral_adc250mhz_r33339.bin",
        {'F', "firmware"});
    args::ValueFlag<std::string> module_defs(option_group, "module_file_flag",
                                             "Crate simulation module definition file to load. "
                                             "The file contains the module to simulate.",
                                             {'M', "modules"});
    args::ValueFlagList<std::string> crate_file_flag(option_group, "crate_file_flag",
                                                     "Crate firmware file to load. "
                                                     "The file contain the list of firmware files.",
                                                     {'C', "crate"});
    args::ValueFlag<std::string> log_file_flag(
        option_group, "log_file_flag", "Log file. Use `stdout` for the console.", {'l', "log"});
    args::ValueFlagList<std::string> slot_map_flag(
        option_group, "slot_map_flag", "A list of slots used to define the slot to index mapping.",
        {'s', "slot_map"});
    args::ValueFlag<std::string> cmd_file_flag(option_group, "cmd_file_flag",
                                               "Command file to execue.", {'c', "cmd"});

    args::Group command_group(parser, "Commands");
    args::PositionalList<std::string> cmd_flag(command_group, "commands",
                                               "Commands to be performed in order. "
                                               "The command `help` lists available command.");

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help& help) {
        std::cout << help.what() << std::endl;
        std::cout << parser;
        return EXIT_SUCCESS;
    } catch (args::Error& e) {
        std::cout << e.what() << std::endl;
        std::cout << parser;
        return EXIT_FAILURE;
    }

    try {
        xia::util::timepoint run;
        run.start();

        std::string log;
        if (log_file_flag) {
            log = args::get(log_file_flag);
        } else {
            log = "test-api-log.txt";
        }

        auto log_level = xia_log::info;
        if (args::get(debug_flag)) {
            log_level = xia_log::debug;
        }
        xia::logging::start("log", log, log_level, false);

        size_t num_modules = args::get(num_modules_flag);

        xia::pixie::crate::crate crate_hw;
        xia::pixie::sim::crate crate_sim;

        xia::pixie::crate::crate* crate_selection = &crate_hw;

        if (simulate) {
            if (!module_defs) {
                throw std::runtime_error("simulation requires a module definition");
            }
            xia_log(xia_log::info) << "simulation: " << args::get(module_defs);
            xia::pixie::sim::load_module_defs(args::get(module_defs));
            crate_selection = &crate_sim;
        }

        xia::pixie::crate::crate& crate = *crate_selection;

        if (fw_file_flag) {
            for (const auto& firmware : args::get(fw_file_flag)) {
                auto fw = xia::pixie::firmware::parse(firmware, ':');
                if (xia::pixie::firmware::check(crate.firmware, fw)) {
                    std::string what("duplicate firmware: ");
                    what += firmware;
                    throw std::runtime_error(what);
                }
                xia::pixie::firmware::add(crate.firmware, fw);
            }
        }

        if (crate_file_flag) {
            for (const auto& firmware : args::get(crate_file_flag)) {
                load_crate_firmware(firmware, crate.firmware);
            }
        }

        initialize(crate, reg_trace);

        if (num_modules != 0 && crate.num_modules != num_modules) {
            throw std::runtime_error("invalid number of modules detected: "
                                     "found " +
                                     std::to_string(crate.num_modules));
        }

        if (slot_map_flag) {
            xia::pixie::module::number_slots slot_map;
            for (const auto& slot : args::get(slot_map_flag)) {
                std::vector<int> slots = get_values<int>(slot, crate.num_modules);
                for (auto s : slots) {
                    slot_map.emplace_back(std::make_pair(int(slot_map.size()), s));
                }
            }
            crate.assign(slot_map);
        }

        std::cout << "modules: online=" << crate.modules.size()
                  << " offline=" << crate.offline.size() << std::endl;

        crate.set_firmware();
        crate.probe();

        commands cmds;

        if (cmd_file_flag) {
            if (!make_command_sets(args::get(cmd_file_flag), cmds)) {
                return EXIT_FAILURE;
            }
        }
        if (cmd_flag) {
            if (!make_command_sets(cmd_flag, cmds)) {
                return EXIT_FAILURE;
            }
        }

        if (!process_command_sets(crate, cmds)) {
            return EXIT_FAILURE;
        }

        run.end();
        std::cout << "run time=" << run << std::endl;
    } catch (xia::pixie::error::error& e) {
        xia_log(xia_log::error) << e;
        std::cerr << e << std::endl;
        return e.return_code();
    } catch (std::exception& e) {
        xia_log(xia_log::error) << "unknown error: " << e.what();
        std::cerr << "error: unknown error: " << e.what() << std::endl;
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (args::get(throw_unhandled_flag)) {
            throw;
        }
        xia_log(xia_log::error) << "unknown error: unhandled exception";
        std::cerr << "error: unknown error: unhandled exception" << std::endl;
        return xia::pixie::error::api_result_unknown_error();
    }

    return EXIT_SUCCESS;
}
