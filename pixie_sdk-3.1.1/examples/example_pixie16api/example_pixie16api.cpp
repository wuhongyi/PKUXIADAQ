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

/** @file example_pixie16api.cpp
 * @brief Demonstrates how to use the Pixie16Api functions to communicate with Pixie-16 modules.
 *
 * We demonstrate both the PixieSDK implementation using `Pixie16Api.so` and the Legacy C
 * implementation using `Pixie16App.so`. The only difference between using one library over the
 * other is in the header definitions.
 *
 * The `Pixie16Api.so` includes all the goodness that comes with using the C++ implementation
 * just with a convenient C wrapper. Users don't have as much control over the nitty-gritty details
 * of the PixieSDK. We've intentionally limited this to prevent this backward compatible API from
 * growing out of hand. We **really** recommend that you link your code directly with `PixieSDK.a`.
 */

#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

#include <args/args.hxx>
#include <nolhmann/json.hpp>

#ifndef LEGACY_EXAMPLE
#include <pixie16/pixie16.h>
#else
#include <pixie16app_defs.h>
#include <pixie16app_export.h>
#include <pixie16sys_defs.h>
#include <pixie16sys_export.h>
#endif

#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

struct LOG {
    explicit LOG(const std::string& type) {
        type_ = type;

        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::chrono::milliseconds now2 = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch());
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

struct module_config {
    unsigned short slot;
    unsigned short number;
    std::string com_fpga_config;
    std::string sp_fpga_config;
    std::string dsp_code;
    std::string dsp_par;
    std::string dsp_var;
};

typedef std::vector<module_config> module_configs;

struct configuration {
    module_configs modules;
    std::vector<unsigned short> slot_def;
    unsigned short num_modules() const {
        return static_cast<unsigned short>(modules.size());
    }
};

std::string generate_filename(const unsigned int& module_number, const std::string& type,
                              const std::string& ext) {
#ifndef LEGACY_EXAMPLE
    static const std::string file_prefix = "pixie16api-module";
#else
    static const std::string file_prefix = "pixie16app-module";
#endif
    return file_prefix + std::to_string(module_number) + "-" + type + "." + ext;
}

void verify_json_module(const nlohmann::json& mod) {
    if (!mod.contains("slot")) {
        throw std::invalid_argument("Missing slot definition in configuration element.");
    }

    if (!mod.contains("dsp")) {
        throw std::invalid_argument("Missing dsp object in configuration element.");
    }

    if (!mod["dsp"].contains("ldr") || !mod["dsp"].contains("var") || !mod["dsp"].contains("par")) {
        throw std::invalid_argument(
            "Missing dsp object in configuration element: ldr, dsp, or par.");
    }

    if (!mod.contains("fpga")) {
        throw std::invalid_argument("Missing fpga object in configuration element.");
    }

    if (!mod["fpga"].contains("fippi") || !mod["fpga"].contains("sys")) {
        throw std::invalid_argument("Missing fpga firmware definition (fippi or sys).");
    }
}

void read_config(const std::string& config_file_name, configuration& cfg) {
    std::ifstream input(config_file_name, std::ios::in);
    if (input.fail()) {
        throw std::ios_base::failure("open: " + config_file_name + ": " + std::strerror(errno));
    }

    nlohmann::json jf = nlohmann::json::parse(input);
    input.close();

    if (jf.empty() || jf.size() > SYS_MAX_NUM_MODULES) {
        throw std::invalid_argument("invalid number of modules");
    }

    cfg.slot_def.clear();
    for (const auto& module : jf) {
        verify_json_module(module);

        cfg.slot_def.push_back(module["slot"]);

        module_config mod_cfg;
        mod_cfg.slot = module["slot"];
        mod_cfg.number = static_cast<unsigned short>(cfg.slot_def.size() - 1);
        mod_cfg.com_fpga_config = module["fpga"]["sys"];
        mod_cfg.sp_fpga_config = module["fpga"]["fippi"];
        mod_cfg.dsp_code = module["dsp"]["ldr"];
        mod_cfg.dsp_par = module["dsp"]["par"];
        mod_cfg.dsp_var = module["dsp"]["var"];
        cfg.modules.push_back(mod_cfg);
    }
}

bool verify_api_return_value(const int& val, const std::string& func_name,
                             const bool& print_success = true) {
    if (val < 0) {
        std::cout << LOG("ERROR") << func_name << " failed with Error Code " << val << std::endl;
        return false;
    }
    if (print_success)
        std::cout << LOG("INFO") << func_name << " finished successfully." << std::endl;
    return true;
}

bool output_statistics_data(const unsigned short& mod_num, const std::string& type) {
    std::vector<unsigned int> stats(448, 0);
    if (!verify_api_return_value(Pixie16ReadStatisticsFromModule(stats.data(), mod_num),
                                 "Pixie16ReadStatisticsFromModule", false))
        return false;

    std::ofstream bin_output(generate_filename(mod_num, type, "bin"),
                             std::ios::binary | std::ios::out);
    bin_output.write(reinterpret_cast<char*>(stats.data()), sizeof(unsigned int) * stats.size());
    bin_output.close();

    std::ofstream csv_output(generate_filename(mod_num, type, "csv"), std::ios::out);
    csv_output << "channel,real_time,live_time,input_count_rate,output_count_rate" << std::endl;

    auto real_time = Pixie16ComputeRealTime(stats.data(), mod_num);

    std::cout << LOG("INFO") << "Begin Statistics for Module " << mod_num << std::endl;
    std::cout << LOG("INFO") << "Real Time: " << real_time << std::endl;
    for (unsigned int chan = 0; chan < NUMBER_OF_CHANNELS; chan++) {
        auto live_time = Pixie16ComputeLiveTime(stats.data(), mod_num, chan);
        auto icr = Pixie16ComputeInputCountRate(stats.data(), mod_num, chan);
        auto ocr = Pixie16ComputeOutputCountRate(stats.data(), mod_num, chan);

        std::cout << LOG("INFO") << "Channel " << chan << " LiveTime: " << live_time << std::endl;
        std::cout << LOG("INFO") << "Channel " << chan << " Input Count Rate: " << icr << std::endl;
        std::cout << LOG("INFO") << "Channel " << chan << " Output Count Rate: " << ocr << std::endl;

        csv_output << chan << "," << real_time << "," << live_time << "," << icr << "," << ocr
                   << std::endl;
    }
    std::cout << LOG("INFO") << "End Statistics for Module " << mod_num << std::endl;
    csv_output.close();
    return true;
}


bool save_dsp_pars(const std::string& filename) {
    std::cout << LOG("INFO") << "Saving DSP Parameters to " << filename << "." << std::endl;
    if (!verify_api_return_value(Pixie16SaveDSPParametersToFile(filename.c_str()),
                                 "Pixie16SaveDSPParametersToFile"))
        return false;
    return true;
}

bool execute_adjust_offsets(const module_config& module) {
    std::cout << LOG("INFO") << "Adjusting baseline offset for Module " << module.number << "." << std::endl;
    if (!verify_api_return_value(Pixie16AdjustOffsets(module.number),
                                 "Pixie16AdjustOffsets for Module " +
                                     std::to_string(module.number)))
        return false;
    if (!save_dsp_pars(module.dsp_par))
        return false;
    return true;
}

bool execute_baseline_capture(const unsigned short& module_number) {
    std::cout << LOG("INFO") << "Starting baseline capture for Module " << module_number << std::endl;
    if (!verify_api_return_value(Pixie16AcquireBaselines(module_number), "Pixie16AcquireBaselines"))
        return false;

    double baselines[NUMBER_OF_CHANNELS][MAX_NUM_BASELINES];
    double timestamps[MAX_NUM_BASELINES];
    for (unsigned int i = 0; i < NUMBER_OF_CHANNELS; i++) {
        std::cout << LOG("INFO") << "Acquiring baselines for Channel " << i << std::endl;
        if (!verify_api_return_value(Pixie16ReadSglChanBaselines(baselines[i], timestamps,
                                                                 MAX_NUM_BASELINES, module_number,
                                                                 i),
                                     "Pixie16ReadsglChanBaselines"))
            return false;
    }

    std::ofstream ofstream1(generate_filename(module_number, "baselines", "csv"));
    ofstream1 << "bin,timestamp,";
    for (unsigned int i = 0; i < NUMBER_OF_CHANNELS; i++)
        ofstream1 << "Chan" << i << ",";
    ofstream1 << std::endl;
    for (unsigned int i = 0; i < MAX_NUM_BASELINES; i++) {
        ofstream1 << i << "," << timestamps[i] << ",";
        for (unsigned int k = 0; k < NUMBER_OF_CHANNELS; k++) {
            if (k != NUMBER_OF_CHANNELS - 1)
                ofstream1 << baselines[k][i] << ",";
            else
                ofstream1 << baselines[k][i];
        }
        ofstream1 << std::endl;
    }
    return true;
}

bool execute_list_mode_run(const configuration& cfg, const double& runtime_in_seconds) {
    std::cout << LOG("INFO") << "Starting list mode data run for " << runtime_in_seconds << " s." << std::endl;

    std::cout << LOG("INFO") << "Calling Pixie16WriteSglModPar to write SYNCH_WAIT = 1 in Module 0." << std::endl;
    if (!verify_api_return_value(Pixie16WriteSglModPar("SYNCH_WAIT", 1, 0),
                                 "Pixie16WriteSglModPar - SYNC_WAIT"))
        return false;

    std::cout << LOG("INFO") << "Calling Pixie16WriteSglModPar to write IN_SYNCH  = 0 in Module 0." << std::endl;
    if (!verify_api_return_value(Pixie16WriteSglModPar("IN_SYNCH", 0, 0),
                                 "Pixie16WriteSglModPar - IN_SYNC"))
        return false;

    std::cout << LOG("INFO") << "Calling Pixie16StartListModeRun." << std::endl;
    if (!verify_api_return_value(Pixie16StartListModeRun(cfg.num_modules(), LIST_MODE_RUN, NEW_RUN),
                                 "Pixie16StartListModeRun"))
        return false;

    std::vector<std::ofstream*> output_streams(cfg.num_modules());
    for (unsigned short i = 0; i < cfg.num_modules(); i++) {
        output_streams[i] = new std::ofstream(generate_filename(i, "list-mode", "bin"),
                                          std::ios::out | std::ios::binary);
    }

    std::vector<uint32_t> data(EXTERNAL_FIFO_LENGTH, 0);
    unsigned int num_fifo_words = 0;

    std::cout << LOG("INFO") << "Collecting data for " << runtime_in_seconds << " s." << std::endl;
    std::chrono::steady_clock::time_point run_start_time = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::duration<double>>(
               std::chrono::steady_clock::now() - run_start_time)
               .count() < runtime_in_seconds) {
        for (unsigned short mod_num = 0; mod_num < cfg.num_modules(); mod_num++) {
            if (Pixie16CheckRunStatus(mod_num) == 1) {
                if (!verify_api_return_value(
                        Pixie16CheckExternalFIFOStatus(&num_fifo_words, mod_num),
                        "Pixie16CheckExternalFIFOStatus", false))
                    return false;

                if (double(num_fifo_words) / EXTERNAL_FIFO_LENGTH > 0.2) {
                    std::cout << LOG("INFO") << "External FIFO has " << num_fifo_words << " words." << std::endl;
                    if (!verify_api_return_value(
                            Pixie16ReadDataFromExternalFIFO(data.data(), num_fifo_words, mod_num),
                            "Pixie16ReadDataFromExternalFIFO", false))
                        return false;
                    output_streams[mod_num]->write(reinterpret_cast<char*>(data.data()),
                                                  num_fifo_words * sizeof(uint32_t));
                } else {
                    continue;
                }
            } else {
                std::cout << LOG("INFO") << "Module " << mod_num << " has no active run!" << std::endl;
            }
        }

        /*
         Check the run status of the Director module (module #0) to see if the run has been stopped.
         This is possible in a multi-chassis system where modules in one chassis can stop the run
         in all chassis.
         */
        if (Pixie16CheckRunStatus(0) == 0) {
            std::cout << LOG("INFO") << "Run was stopped by the director module. Stopping data collection." << std::endl;
            break;
        }
    }

    /*
     Stop run in the Director module (module #0) - a SYNC interrupt should be generated
     to stop run in all modules simultaneously
     */
    std::cout << LOG("INFO") << "Stopping List Mode Run." << std::endl;
    if (!verify_api_return_value(Pixie16EndRun(0), "Pixie16EndRun"))
        return false;

    std::cout << LOG("INFO") << "Checking that the run is finalized in all the modules." << std::endl;
    bool all_modules_finished = false;
    const unsigned int max_finalize_attempts = 50;
    for (unsigned int counter = 0; counter < max_finalize_attempts; counter++) {
        for (unsigned short k = 0; k < cfg.num_modules(); k++) {
            if (Pixie16CheckRunStatus(k) == 1) {
                all_modules_finished = false;
            } else {
                all_modules_finished = true;
            }
        }
        if (all_modules_finished) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!all_modules_finished) {
        std::cout << LOG("ERROR") << "All modules did not stop their runs properly!" << std::endl;
        return false;
    }

    std::cout << LOG("INFO") << "List-mode run finished in "
              << std::chrono::duration_cast<std::chrono::duration<double>>(
                     std::chrono::steady_clock::now() - run_start_time)
                     .count()
              << " s" << std::endl;

    std::cout << LOG("INFO") << "Reading the final words from the External FIFO and the run statistics." << std::endl;
    for (unsigned short mod_num = 0; mod_num < cfg.num_modules(); mod_num++) {
        if (!verify_api_return_value(Pixie16CheckExternalFIFOStatus(&num_fifo_words, mod_num),
                                     "Pixie16CheckExternalFIFOStatus", false))
            return false;

        if (num_fifo_words > 0) {
            std::cout << LOG("INFO") << "External FIFO has " << num_fifo_words << " words." << std::endl;
            if (!verify_api_return_value(
                    Pixie16ReadDataFromExternalFIFO(data.data(), num_fifo_words, mod_num),
                    "Pixie16ReadDataFromExternalFIFO", false))
                return false;
            output_streams[mod_num]->write(reinterpret_cast<char*>(data.data()),
                                          num_fifo_words * sizeof(uint32_t));
        }
        if (!output_statistics_data(mod_num, "list-mode-stats")) {
            return false;
        }
    }

    return true;
}

bool execute_mca_run(const unsigned int& mod, const double& runtime_in_seconds) {
    std::cout << LOG("INFO") << "Calling Pixie16WriteSglModPar to write HOST_RT_PRESET to " << runtime_in_seconds << std::endl;
    if (!verify_api_return_value(
            Pixie16WriteSglModPar("HOST_RT_PRESET", Decimal2IEEEFloating(runtime_in_seconds), mod),
            "Pixie16WriteSglModPar - HOST_RT_PRESET"))
        return false;

    std::cout << LOG("INFO") << "Calling Pixie16WriteSglModPar to write SYNCH_WAIT = 0 in Module 0." << std::endl;
    if (!verify_api_return_value(Pixie16WriteSglModPar("SYNCH_WAIT", 0, mod),
                                 "Pixie16WriteSglModPar - SYNC_WAIT"))
        return false;

    std::cout << LOG("INFO") << "Calling Pixie16WriteSglModPar to write IN_SYNCH  = 1 in Module 0." << std::endl;
    if (!verify_api_return_value(Pixie16WriteSglModPar("IN_SYNCH", 1, mod),
                                 "Pixie16WriteSglModPar - IN_SYNC"))
        return false;

    std::cout << LOG("INFO") << "Starting MCA data run for " << runtime_in_seconds << " s." << std::endl;
    if (!verify_api_return_value(Pixie16StartHistogramRun(mod, NEW_RUN),
                                 "Pixie16StartHistogramRun"))
        return false;

    std::chrono::steady_clock::time_point run_start_time = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::duration<double>>(
               std::chrono::steady_clock::now() - run_start_time)
               .count() < runtime_in_seconds) {
        //do nothing
    }
    std::cout << LOG("INFO") << "Stopping MCA data run after "
              << std::chrono::duration_cast<std::chrono::duration<double>>(
                     std::chrono::steady_clock::now() - run_start_time)
                     .count()
              << " s." << std::endl;
    if (!verify_api_return_value(Pixie16EndRun(mod), "Pixie16EndRun"))
        return false;

    std::string name = generate_filename(mod, "mca", "csv");
    std::ofstream out(name);
    out << "bin,";

    std::vector<std::vector<uint32_t>> hists;
    for (unsigned int i = 0; i < NUMBER_OF_CHANNELS; i++) {
        std::vector<uint32_t> hist(MAX_HISTOGRAM_LENGTH, 0);
        Pixie16ReadHistogramFromModule(hist.data(), MAX_HISTOGRAM_LENGTH, mod, i);
        hists.push_back(hist);
        out << "Chan" << i << ",";
    }
    out << std::endl;

    for (unsigned int bin = 0; bin < MAX_HISTOGRAM_LENGTH; bin++) {
        out << bin << ",";
        for (auto hist : hists) {
            out << hist[bin] << ",";
        }
        out << std::endl;
    }

    if (!output_statistics_data(mod, "mca-stats")) {
        return false;
    }

    return true;
}

bool execute_parameter_read(args::ValueFlag<std::string>& parameter,
                            args::ValueFlag<unsigned int>& crate,
                            args::ValueFlag<unsigned int>& module,
                            args::ValueFlag<unsigned int>& channel) {
    if (channel) {
        double result;
        std::cout << LOG("INFO") << "Pixie16ReadSglChanPar"
                  << " reading " << parameter.Get() << " from Crate " << crate.Get() << " Module "
                  << module.Get() << " Channel " << channel.Get() << "." << std::endl;
        if (!verify_api_return_value(Pixie16ReadSglChanPar(parameter.Get().c_str(), &result,
                                                           module.Get(), channel.Get()),
                                     "Pixie16ReadSglChanPar", false))
            return false;
        std::cout << LOG("INFO") << result << std::endl;
    } else {
        unsigned int result;
        std::cout << LOG("INFO") << "Pixie16ReadSglModPar reading " << parameter.Get() << " from Crate "
                  << crate.Get() << " Module " << module.Get() << "." << std::endl;
        if (!verify_api_return_value(
                Pixie16ReadSglModPar(parameter.Get().c_str(), &result, module.Get()),
                "Pixie16ReadSglModPar", false))
            return false;
        std::cout << LOG("INFO") << result << std::endl;
    }
    return true;
}

bool execute_parameter_write(args::ValueFlag<std::string>& parameter,
                             args::ValueFlag<double>& value, args::ValueFlag<unsigned int>& crate,
                             const module_config& module, args::ValueFlag<unsigned int>& channel) {
    if (channel) {
        std::cout << LOG("INFO") << "Pixie16WriteSglChanPar setting " << parameter.Get() << " to " << value.Get()
                  << " for Crate " << crate.Get() << " Module " << module.number << " Channel "
                  << channel.Get() << "." << std::endl;
        if (!verify_api_return_value(Pixie16WriteSglChanPar(parameter.Get().c_str(), value.Get(),
                                                            module.number, channel.Get()),
                                     "Pixie16WriteSglChanPar"))
            return false;
    } else {
        std::cout << LOG("INFO") << "Pixie16WriteSglModPar"
                  << " setting " << parameter.Get() << " to " << value.Get() << " for  Crate "
                  << crate.Get() << " Module " << module.number << "." << std::endl;
        if (!verify_api_return_value(
                Pixie16WriteSglModPar(parameter.Get().c_str(), value, module.number),
                "Pixie16WriteSglModPar"))
            return false;
    }

    if (!save_dsp_pars(module.dsp_par))
        return false;
    return true;
}

bool execute_trace_capture(const unsigned short& module_number) {
    std::cout << LOG("INFO") << "Pixie16AcquireADCTrace acquiring traces for Module " << module_number << "." << std::endl;
    if (!verify_api_return_value(Pixie16AcquireADCTrace(module_number), "Pixie16AcquireADCTrace"))
        return false;


    unsigned short trace[NUMBER_OF_CHANNELS][MAX_ADC_TRACE_LEN];
    for (unsigned int i = 0; i < NUMBER_OF_CHANNELS; i++) {
        if (!verify_api_return_value(
                Pixie16ReadSglChanADCTrace(trace[i], MAX_ADC_TRACE_LEN, module_number, i),
                "Pixie16AcquireADCTrace", false))
            return false;
    }

    std::ofstream ofstream1(generate_filename(module_number, "adc", "csv"));
    ofstream1 << "bin,";
    for (unsigned int i = 0; i < NUMBER_OF_CHANNELS; i++)
        ofstream1 << "Chan" << i << ",";
    ofstream1 << std::endl;
    for (unsigned int i = 0; i < MAX_ADC_TRACE_LEN; i++) {
        ofstream1 << i << ",";
        for (unsigned int k = 0; k < NUMBER_OF_CHANNELS; k++) {
            if (k != NUMBER_OF_CHANNELS - 1)
                ofstream1 << trace[k][i] << ",";
            else
                ofstream1 << trace[k][i];
        }
        ofstream1 << std::endl;
    }
    return true;
}

bool execute_blcut(args::ValueFlag<unsigned int>& module, args::ValueFlag<unsigned int>& channel) {
    if (!module)
        return false;

    std::cout << LOG("INFO") << "Executing Pixie16BLcutFinder for Module" << module.Get() << "." << std::endl;
    unsigned int blcut = 0;
    if (!verify_api_return_value(Pixie16BLcutFinder(module.Get(), channel.Get(), &blcut),
                                 "Pixie16BLcutFinder", false))
        return false;
    std::cout << LOG("INFO") << "BLCut for Module " << module.Get() << " Channel " << channel.Get() << " is "
              << blcut << std::endl;
    return true;
}

bool execute_set_dacs(args::ValueFlag<unsigned int>& module) {
    if (!module)
        return false;

    std::cout << LOG("INFO") << "Executing Pixie16SetDACs for Module" << module.Get() << "." << std::endl;
    if (!verify_api_return_value(Pixie16SetDACs(module.Get()), "Pixie16SetDACs", false))
        return false;
    return true;
}

bool execute_close_module_connection(const int& numModules) {
    for (int i = 0; i < numModules; i++) {
        std::cout << LOG("INFO") << "Closing out connection to Module " << i << "." << std::endl;
        verify_api_return_value(Pixie16ExitSystem(i),
                                "Pixie16ExitSystem for Module " + std::to_string(i));
    }
    return true;
}

double calculate_duration_in_seconds(const std::chrono::system_clock::time_point& start,
                                     const std::chrono::system_clock::time_point& end) {
    return std::chrono::duration<double>(end - start).count();
}

void output_module_info(const configuration& cfg) {
    unsigned short rev;
    unsigned int serial_number;
    unsigned short adc_bits;
    unsigned short adc_msps;
    for (const auto& mod : cfg.modules) {
        if (!verify_api_return_value(
                Pixie16ReadModuleInfo(mod.number, &rev, &serial_number, &adc_bits, &adc_msps),
                "Pixie16ReadModuleInfo", false))
            throw std::runtime_error("Could not get module information for Module " +
                                     std::to_string(mod.number));
        std::cout << LOG("INFO") << "Begin module information for Module " << mod.number << std::endl;
        std::cout << LOG("INFO") << "Serial Number: " << serial_number << std::endl;
        std::cout << LOG("INFO") << "Revision: " << rev << std::endl;
        std::cout << LOG("INFO") << "ADC Bits: " << adc_bits << std::endl;
        std::cout << LOG("INFO") << "ADC MSPS: " << adc_msps << std::endl;
        std::cout << LOG("INFO") << "End module information for Module " << mod.number << std::endl;
    }
}


int main(int argc, char** argv) {
    auto start = std::chrono::system_clock::now();
    args::ArgumentParser parser(
        "Sample code that interfaces with a Pixie system through the User API.");
    parser.LongSeparator("=");


    args::Group commands(parser, "commands");
    args::Command boot(commands, "boot", "Boots the crate of modules.");
    args::Command export_settings(
        commands, "export-settings",
        "Boots the system and dumps the settings to the file defined in the config.");
    args::Command histogram(commands, "histogram", "Save histograms from the module.");
    args::Command init(commands, "init", "Initializes the system without going any farther.");
    args::Command list_mode(commands, "list-mode", "Starts a list mode data run");
    args::Command read(commands, "read", "Read a parameter from the module.");
    args::Command write(commands, "write", "Write a parameter to the module.");
    args::Command trace(commands, "trace", "Captures traces from the modules.");
    args::Command adjust_offsets(commands, "adjust_offsets",
                                 "Adjusts the DC offsets for all modules in the config file.");
    args::Command baseline(commands, "baseline", "Acquire and print baselines from the module");
    args::Command mca(commands, "mca", "Starts an MCA data run.");
    args::Command blcut(commands, "blcut",
                        "Starts a control task to find the BLCut for a channel.");
    args::Command dacs(commands, "dacs", "Starts a control task to set the module's DACs");

    args::Group arguments(parser, "arguments", args::Group::Validators::AtLeastOne,
                          args::Options::Global);
    args::Flag api_pixie(arguments, "api", "Select the Pixie API (default: pixie16)", {'P', "api"});

    args::ValueFlag<std::string> conf_flag(arguments, "cfg", "The configuration file to load.",
                                           {'c', "config"}, args::Options::Required);
    args::HelpFlag help_flag(arguments, "help", "Displays this message", {'h', "help"});
    args::Flag is_fast_boot(boot, "fast-boot", "Performs a partial boot of the system.",
                            {'f', "fast-boot"});
    args::Flag is_offline(arguments, "Offline Mode",
                          "Tells the API to use Offline mode when running.", {'o', "offline"});
    args::ValueFlag<std::string> boot_pattern_flag(arguments, "boot_pattern",
                                                   "The boot pattern used for booting.",
                                                   {'b', "boot_pattern"}, "0x7F");
    args::ValueFlag<double> run_time(
        list_mode, "time", "The amount of time that a list mode run will take in seconds.",
        {'t', "run-time"}, 10.);
    args::ValueFlag<std::string> parameter(
        read, "parameter", "The parameter we want to read from the system.", {'n', "name"});
    args::ValueFlag<unsigned int> crate(read, "crate", "The crate", {"crate"}, 0);
    args::ValueFlag<unsigned int> module(read, "module", "The module", {"mod"});
    args::ValueFlag<unsigned int> channel(read, "channel", "The channel", {"chan"});
    args::ValueFlag<double> parameter_value(
        write, "parameter_value", "The value of the parameter we want to write.", {'v', "value"});
    adjust_offsets.Add(conf_flag);
    adjust_offsets.Add(boot_pattern_flag);
    adjust_offsets.Add(module);
    baseline.Add(is_fast_boot);
    baseline.Add(boot_pattern_flag);
    baseline.Add(module);
    blcut.Add(module);
    blcut.Add(channel);
    boot.Add(conf_flag);
    boot.Add(boot_pattern_flag);
    dacs.Add(module);
    mca.Add(module);
    mca.Add(boot_pattern_flag);
    read.Add(conf_flag);
    trace.Add(conf_flag);
    trace.Add(module);
    trace.Add(boot_pattern_flag);
    write.Add(conf_flag);
    write.Add(parameter);
    write.Add(crate);
    write.Add(module);
    write.Add(channel);

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

    configuration cfg;
    try {
        read_config(conf_flag.Get(), cfg);
    } catch (std::exception& e) {
        std::cout << LOG("ERROR") << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << LOG("INFO") << "Finished reading config in "
              << calculate_duration_in_seconds(start, std::chrono::system_clock::now()) << " s." << std::endl;

    int offline_mode = 0;
    if (is_offline)
        offline_mode = 1;

    start = std::chrono::system_clock::now();
    std::cout << LOG("INFO") << "Calling Pixie16InitSystem." << std::endl;
    if (!verify_api_return_value(
            Pixie16InitSystem(cfg.num_modules(), cfg.slot_def.data(), offline_mode),
            "Pixie16InitSystem", false))
        return EXIT_FAILURE;

    std::cout << LOG("INFO") << "Finished Pixie16InitSystem in "
              << calculate_duration_in_seconds(start, std::chrono::system_clock::now()) << " s." << std::endl;

    try {
        output_module_info(cfg);
    } catch (std::runtime_error& error) {
        std::cout << LOG("ERROR") << error.what() << std::endl;
        return EXIT_FAILURE;
    }

    if (init) {
        execute_close_module_connection(cfg.num_modules());
        return EXIT_SUCCESS;
    }

    unsigned int boot_pattern = stoul(args::get(boot_pattern_flag), nullptr, 0);
    if (is_fast_boot)
        boot_pattern = 0x70;

    for (const auto& mod : cfg.modules) {
        start = std::chrono::system_clock::now();
        std::cout << LOG("INFO") << "Calling Pixie16BootModule for Module " << mod.number
                  << " with boot pattern: " << std::showbase << std::hex << boot_pattern
                  << std::dec << std::endl;

        if (!verify_api_return_value(
                Pixie16BootModule(mod.com_fpga_config.c_str(), mod.sp_fpga_config.c_str(), nullptr,
                                  mod.dsp_code.c_str(), mod.dsp_par.c_str(), mod.dsp_var.c_str(),
                                  mod.number, boot_pattern),
                "Pixie16BootModule", "Finished booting!"))
            return EXIT_FAILURE;
        std::cout << LOG("INFO") << "Finished Pixie16BootModule for Module " << mod.number << " in "
                  << calculate_duration_in_seconds(start, std::chrono::system_clock::now())
                  << " s." << std::endl;
    }

    if (boot) {
        execute_close_module_connection(cfg.num_modules());
        return EXIT_SUCCESS;
    }

    if (read) {
        if (!execute_parameter_read(parameter, crate, module, channel))
            return EXIT_FAILURE;
    }

    if (write) {
        if (!execute_parameter_write(parameter, parameter_value, crate, cfg.modules[module.Get()],
                                     channel))
            return EXIT_FAILURE;
    }

    if (adjust_offsets) {
        if (!execute_adjust_offsets(cfg.modules[module.Get()]))
            return EXIT_FAILURE;
    }

    if (trace) {
        if (!execute_trace_capture(module.Get()))
            return EXIT_FAILURE;
    }

    if (list_mode) {
        if (!execute_list_mode_run(cfg, run_time.Get()))
            return EXIT_FAILURE;
    }

    if (export_settings) {
        if (!save_dsp_pars(cfg.modules.front().dsp_par))
            return EXIT_FAILURE;
    }

    if (baseline) {
        if (!execute_baseline_capture(module.Get()))
            return EXIT_FAILURE;
    }

    if (mca) {
        if (!execute_mca_run(module.Get(), run_time.Get()))
            return EXIT_FAILURE;
    }

    if (blcut) {
        if (!execute_blcut(module, channel))
            return EXIT_FAILURE;
    }

    if (dacs) {
        if (!execute_set_dacs(module))
            return EXIT_FAILURE;
    }

    execute_close_module_connection(cfg.num_modules());
    return EXIT_SUCCESS;
}
