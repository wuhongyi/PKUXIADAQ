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

/** @file config.cpp
 * @brief Implements data structures and functions for working with SDK configuration files.
 */
#include <algorithm>
#include <exception>
#include <fstream>

#include <pixie/config.hpp>

#include <nolhmann/json.hpp>

namespace xia {
namespace pixie {
namespace config {
using json = nlohmann::json;

static void throw_json_error(json::exception& e, const std::string& what) {
    throw error(error::code::config_json_error, what + ": " + e.what());
}

//@todo Need to make this more dynamic to take into account changes to the DSP vars.
static const json default_config = {
    {"channel",
     {{"input",
       {{"BLcut", {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}},
        {"BaselinePercent", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}},
        {"CFDDelay", {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8}},
        {"CFDScale", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"CFDThresh",
         {120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120}},
        {"ChanCSRa", {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4}},
        {"ChanCSRb", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"ChanTrigStretch", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"DigGain", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"EnergyLow", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"ExtTrigStretch",
         {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150}},
        {"ExternDelayLen", {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}},
        {"FastGap", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}},
        {"FastLength", {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}},
        {"FastThresh",
         {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
          1000}},
        {"FastTrigBackLen", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}},
        {"FtrigoutDelay", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"GainDAC", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"Integrator", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"Log2Bweight",
         {4294967294, 4294967294, 4294967294, 4294967294, 4294967294, 4294967294, 4294967294,
          4294967294, 4294967294, 4294967294, 4294967294, 4294967294, 4294967294, 4294967294,
          4294967294, 4294967294}},
        {"Log2Ebin",
         {4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 4294967295,
          4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 4294967295,
          4294967295, 4294967295}},
        {"MultiplicityMaskH", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"MultiplicityMaskL", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"OffsetDAC",
         {25056, 25312, 24864, 24928, 24608, 24160, 24608, 24608, 24672, 24608, 24608, 24608, 23392,
          23840, 24608, 24608}},
        {"PSAlength", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"PSAoffset", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"PeakSample", {42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42}},
        {"PreampTau",
         {1112014848, 1112014848, 1112014848, 1112014848, 1112014848, 1112014848, 1112014848,
          1112014848, 1112014848, 1112014848, 1112014848, 1112014848, 1112014848, 1112014848,
          1112014848, 1112014848}},
        {"QDCLen0", {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}},
        {"QDCLen1", {63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63}},
        {"QDCLen2", {88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88}},
        {"QDCLen3",
         {113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113}},
        {"QDCLen4",
         {138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138}},
        {"QDCLen5",
         {163, 163, 163, 163, 163, 163, 163, 163, 163, 163, 163, 163, 163, 163, 163, 163}},
        {"QDCLen6",
         {188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188}},
        {"QDCLen7",
         {213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213}},
        {"ResetDelay", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"SlowGap", {19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19}},
        {"SlowLength", {25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25}},
        {"ThreshWidth", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"TraceLength",
         {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500}},
        {"TrigOutLen", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"TriggerDelay",
         {344, 344, 344, 344, 344, 344, 344, 344, 344, 344, 344, 344, 344, 344, 344, 344}},
        {"VetoStretch", {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}},
        {"Xavg", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"Xwait", {6, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8}}}}}},
    {"metadata",
     {{"hardware_revision", "F"},
      {"slot", 99}}},
    {"module",
     {{"input",
       {{"CoincPattern", 0},
        {"CoincWait", 0},
        {"ControlTask", 4},
        {"CrateID", 0},
        {"FastFilterRange", 0},
        {"FastTrigBackplaneEna", 0},
        {"HostIO", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"HostRunTimePreset", 1092616192},
        {"InSynch", 1},
        {"MaxEvents", 0},
        {"ModCSRA", 0},
        {"ModCSRB", 1},
        {"ModFormat", 0},
        {"ModID", 0},
        {"ModNum", 0},
        {"Resume", 1},
        {"RunTask", 0},
        {"SlotID", 5},
        {"SlowFilterRange", 3},
        {"SynchWait", 1},
        {"TrigConfig", {0, 0, 0, 0}},
        {"U00", {0, 0, 0, 0, 0, 0, 0}},
        {"UserIn", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}}}}}};

void import_json(const std::string& filename, crate::crate& crate, module::number_slots& loaded) {
    std::ifstream input_json(filename);
    if (!input_json) {
        throw error(pixie::error::code::file_open_failure,
                    "opening json config: " + filename + ": " + std::strerror(errno));
    }

    json config;

    try {
        config = json::parse(input_json);
    } catch (json::exception& e) {
        throw_json_error(e, "parse config");
    }

    if (config.size() > crate.num_modules) {
        log(log::warning) << "too many module configs (" << config.size() << "), crate only has "
                          << crate.num_modules << " modules ";
    }

    if (config.size() < crate.num_modules) {
        log(log::warning) << "too few module configs (" << config.size() << "), crate has "
                          << crate.num_modules
                          << " modules. Using default config for missing modules";
        while (config.size() < crate.num_modules) {
            config.push_back(default_config);
        }
    }

    size_t mod = 0;
    auto ci = config.begin();

    while (mod < crate.num_modules) {
        auto& module = crate[mod];

        if (!module.online()) {
            log(log::warning) << "module " << mod << " not online, skipping";
        } else {
            auto& settings = *ci;

            if (!settings.contains("metadata")) {
                throw error(error::code::config_json_error, "'metadata' not found");
            }

            if (!settings.contains("module")) {
                throw error(error::code::config_json_error, "'module' not found");
            }

            if (!settings.contains("channel")) {
                throw error(error::code::config_json_error, "'channel' not found");
            }

            auto metadata = settings["metadata"];
            auto moddata = settings["module"];
            auto chandata = settings["channel"];

            if (!moddata.contains("input")) {
                throw error(error::code::config_json_error, "module 'input' not found");
            }

            if (!chandata.contains("input")) {
                throw error(error::code::config_json_error, "channel 'input' not found");
            }

            try {
                auto rev = metadata["hardware_revision"].get<std::string>();
                if (rev[0] != module.revision_label()) {
                    log(log::warning) << "config module " << mod << " (rev " << rev
                                      << ") loading on to " << module.revision_label();
                }
            } catch (json::exception& e) {
                throw_json_error(e, "config rev");
            }

            try {
                auto slot = metadata["slot"];
                if (slot != module.slot) {
                    log(log::warning) << "config module " << mod << " (slot " << slot
                                      << ") has moved to slot " << module.slot;
                }
            } catch (json::exception& e) {
                throw_json_error(e, "config slot-id");
            }

            /*
             * Write the config to the module variables
             */
            for (auto& el : moddata["input"].items()) {
                /*
                 * Load variables first and if not a variable check if it is a
                 * parameter and if not a parameter log a warning. This puts
                 * variables before parameters and ignores parameters if
                 * present.
                 */
                if (param::is_module_var(el.key())) {
                    auto var = param::lookup_module_var(el.key());
                    auto& desc = module.module_var_descriptors[int(var)];
                    if (desc.writeable()) {
                        if (desc.size != el.value().size()) {
                            log(log::warning) << module::module_label(module)
                                              << "size does not match: " << el.key();
                        } else {
                            log(log::debug)
                                << module::module_label(module) << "module var set: " << el.key();
                            if (desc.size > 1) {
                                for (size_t v = 0; v < desc.size; ++v) {
                                    try {
                                        module.write_var(var, el.value()[v], v, false);
                                    } catch (json::exception& e) {
                                        auto s = el.key() + ": " + std::string(el.value());
                                        throw_json_error(e, s);
                                    }
                                }
                            } else {
                                try {
                                    if (desc.par == xia::pixie::param::module_var::SlotID) {
                                        module.write_var(var, module.slot, 0, false);
                                    } else if (desc.par == xia::pixie::param::module_var::ModNum) {
                                        module.write_var(var, module.number, 0, false);
                                    } else {
                                        module.write_var(var, el.value(), 0, false);
                                    }
                                } catch (json::exception& e) {
                                    auto s = el.key() + ": " + std::string(el.value());
                                    throw_json_error(e, s);
                                }
                            }
                        }
                    }
                } else if (!param::is_module_param(el.key())) {
                    /*
                     * If not a parameter (ignore those) log a message
                     */
                    log(log::warning) << "config module " << mod << " (slot " << module.slot
                                      << "): invalid variable: " << el.key();
                }
            }

            /*
             * Write the config to the channel variables
             */
            for (auto& el : chandata["input"].items()) {
                /*
                 * Load variables first and if not a variable check if it is a
                 * parameter and if not a parameter log a warning. This puts
                 * variables before parameters and ignores parameters if
                 * present.
                 */
                if (param::is_channel_var(el.key())) {
                    auto var = param::lookup_channel_var(el.key());
                    auto& desc = module.channel_var_descriptors[int(var)];
                    if (desc.writeable()) {
                        if ((el.value().size() % desc.size) != 0) {
                            log(log::warning) << module::module_label(module)
                                              << "size does not match config: " << el.key();
                        } else {
                            log(log::debug)
                                << module::module_label(module) << "channel var set: " << el.key()
                                << ": " << el.value();
                            size_t vchannels = el.value().size() / desc.size;
                            for (size_t channel = 0;
                                 channel < module.num_channels && channel < vchannels &&
                                 channel * desc.size < el.value().size();
                                 ++channel) {
                                size_t vbase = channel * desc.size;
                                for (size_t v = 0; v < desc.size; ++v) {
                                    try {
                                        module.write_var(var, el.value()[vbase + v], channel, v,
                                                         false);
                                    } catch (json::exception& e) {
                                        auto s =
                                            el.key() + ": " + std::string(el.value()[vbase + v]);
                                        throw_json_error(e, s);
                                    }
                                }
                            }
                        }
                    }
                } else if (!param::is_channel_param(el.key())) {
                    /*
                     * If not a parameter (ignore those) log a message
                     */
                    log(log::warning) << "config module " << mod << " (slot " << module.slot
                                      << "): invalid variable: " << el.key();
                }
            }

            /*
             * Record the module had been loaded.
             */
            loaded.push_back(module::number_slot(module.number, module.slot));
        }

        ++ci;
        if (ci == config.end()) {
            break;
        }

        ++mod;
    }
}

static json json_firmware(const firmware::firmware_ref fw) {
    json jfw;
    jfw["tag"] = fw->tag;
    jfw["file"] = fw->basename();
    jfw["version"] = fw->version;
    jfw["rev"] = fw->mod_revision;
    jfw["adc_msps"] = fw->mod_adc_msps;
    jfw["adc_bits"] = fw->mod_adc_bits;
    return jfw;
}

void export_json(const std::string& filename, crate::crate& crate) {
    json config;

    for (auto m : crate.modules) {
        module::module& mod = *m;

        json metadata;
        char rv[2] = {mod.revision_label(), '\0'};
        metadata["number"] = mod.number;
        metadata["slot"] = mod.slot;
        metadata["serial-num"] = mod.serial_num;
        metadata["hardware_revision"] = rv;
        metadata["num-channels"] = mod.num_channels;
        metadata["sys"] = json_firmware(mod.get("sys"));
        metadata["fippi"] = json_firmware(mod.get("fippi"));
        metadata["dsp"] = json_firmware(mod.get("dsp"));
        metadata["var"] = json_firmware(mod.get("var"));
        metadata["fifo"]["buffers"] = mod.fifo_buffers;
        metadata["fifo"]["run-wait"] = mod.fifo_run_wait_usecs.load();
        metadata["fifo"]["idle-wait"] = mod.fifo_idle_wait_usecs.load();
        metadata["fifo"]["hold"] = mod.fifo_hold_usecs.load();
        metadata["config"] = json::array();
        for (auto& chan : mod.channels) {
            json cfg;
            cfg["adc_bits"] = chan.config.adc_bits;
            cfg["adc_msps"] = chan.config.adc_msps;
            cfg["adc_clk_div"] = chan.config.adc_clk_div;
            cfg["fpga_clk_mhz"] = chan.config.fpga_clk_mhz;
            metadata["config"].push_back(cfg);
        }

        json module;
        for (auto& var : mod.module_vars) {
            auto& desc = var.var;
            if (desc.mode != param::ro) {
                if (desc.size == 1) {
                    module[desc.name] = var.value[0].value;
                } else {
                    json value;
                    for (auto v : var.value) {
                        value.push_back(v.value);
                    }
                    module[desc.name] = value;
                }
            }
        }

        json channel;
        for (auto& desc : mod.channel_var_descriptors) {
            if (desc.mode != param::ro) {
                json values;
                for (auto& chan : mod.channels) {
                    for (auto& v : chan.vars[int(desc.par)].value) {
                        values.push_back(v.value);
                    }
                }
                channel[desc.name] = values;
            }
        }

        json mod_config;
        mod_config["metadata"] = metadata;
        mod_config["module"] = {{"input", module}};
        mod_config["channel"] = {{"input", channel}};

        config.push_back(mod_config);
    }

    std::ofstream output_json(filename);
    if (!output_json) {
        throw error(pixie::error::code::file_open_failure,
                    "opening json config: " + filename + ": " + std::strerror(errno));
    }

    output_json << std::setw(4) << config << std::endl;
}

}  // namespace config
}  // namespace pixie
}  // namespace xia
