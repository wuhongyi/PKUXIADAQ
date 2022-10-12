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

/** @file pixie16.cpp
 * @brief C wrappers for the C++ API that expose the same signature as the legacy code
 */

#include <bitset>
#include <cstring>

#include <pixie16/pixie16.h>

#include <pixie/config.hpp>
#include <pixie/error.hpp>
#include <pixie/log.hpp>
#include <pixie/os_compat.hpp>
#include <pixie/stats.hpp>
#include <pixie/util.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/legacy.hpp>
#include <pixie/pixie16/run.hpp>

/*
 * Local types for convenience.
 */
using xia_error = xia::pixie::error::error;

/*
 * Boot patterns
 */
#define BOOTPATTERN_COMFPGA_BIT 1
#define BOOTPATTERN_SPFPGA_BIT 2
#define BOOTPATTERN_DSPCODE_BIT 3
#define BOOTPATTERN_DSPPAR_BIT 4
#define BOOTPATTERN_PROGFIPPI_BIT 5
#define BOOTPATTERN_SETDACS_BIT 6

/*
 * Legacy API stats data exported to users as opaque data
 */
struct stats_legacy {
    static const unsigned int mark_1 = 0x20010928;
    static const unsigned int mark_2 = 0x19650829;

    unsigned int marker_1;
    size_t num_channels;
    xia::pixie::stats::module module;
    xia::pixie::stats::channel channels[xia::pixie::hw::max_channels];
    unsigned int marker_2;

    explicit stats_legacy(const xia::pixie::hw::configs& configs);

    void validate() const;
};

typedef stats_legacy* stats_legacy_ptr;

/*
 * The crate. We only handle a single crate with the legacy API.
 */
static xia::pixie::crate::crate crate;

stats_legacy::stats_legacy(const xia::pixie::hw::configs& configs)
    : marker_1(mark_1), marker_2(mark_2) {
    for (size_t channel = 0; channel < configs.size(); ++channel) {
        channels[channel].config = configs[channel];
    }
}

void stats_legacy::validate() const {
    if (marker_1 != mark_1 || marker_2 != mark_2) {
        throw xia_error(xia_error::code::invalid_value, "statistics data corrupt");
    }
}

static int not_supported() {
    return xia::pixie::error::return_code_not_supported();
}

static void load_settings_file(xia::pixie::module::module& module, const std::string& filename) {
    try {
        xia::pixie::legacy::settings settings(module);
        settings.load(filename);
        settings.import(module);
        settings.write(module);
        module.sync_vars();
    } catch (xia::pixie::error::error& err) {
        if (err.type == xia::pixie::error::code::module_total_invalid ||
            err.type == xia::pixie::error::code::channel_number_invalid) {
            xia_log(xia::log::info) << "Settings file binary format not recognized. Will try JSON fallback.";
            xia::pixie::module::number_slots modules;
            ///TODO: Not super efficient if we're calling module-by-module.
            crate.import_config(filename, modules);
        } else {
            throw;
        }
    }
}

template<class T>
T set_bit(const std::string& name, unsigned short& bit, T value, const bool& bit_status) {
    T local_val = value;
    try {
        auto value_bits = std::bitset<std::numeric_limits<T>::digits>(value);
        value_bits.set(bit, bit_status);
        return (T) value_bits.to_ulong();
    } catch (std::out_of_range& out_of_range) {
        xia_log(xia::log::warning) << name << ": bit out of range - " << out_of_range.what();
    }
    return local_val;
}

template<class T>
T test_bit(const std::string& name, unsigned short& bit, T value) {
    try {
        return std::bitset<std::numeric_limits<T>::digits>(value).test(bit);
    } catch (std::out_of_range& out_of_range) {
        xia_log(xia::log::warning) << name << ": bit out of range - " << out_of_range.what();
    }
    return false;
}

PIXIE_EXPORT unsigned short PIXIE_API APP16_TstBit(unsigned short bit, unsigned short value) {
    return test_bit("APP16_TstBit", bit, value);
}

PIXIE_EXPORT unsigned short PIXIE_API APP16_SetBit(unsigned short bit, unsigned short value) {
    return set_bit("APP16_SetBit", bit, value, true);
}

PIXIE_EXPORT unsigned short PIXIE_API APP16_ClrBit(unsigned short bit, unsigned short value) {
    return set_bit("APP16_ClrBit", bit, value, false);
}

PIXIE_EXPORT unsigned int PIXIE_API APP32_SetBit(unsigned short bit, unsigned int value) {
    return set_bit("APP32_SetBit", bit, value, true);
}

PIXIE_EXPORT unsigned int PIXIE_API APP32_ClrBit(unsigned short bit, unsigned int value) {
    return set_bit("APP32_ClrBit", bit, value, false);
}

PIXIE_EXPORT unsigned int PIXIE_API APP32_TstBit(unsigned short bit, unsigned int value) {
    return test_bit("APP32_TstBit", bit, value);
}

PIXIE_EXPORT double PIXIE_API IEEEFloating2Decimal(unsigned int IEEEFloatingNumber) {
    return static_cast<double>(xia::util::ieee_float(IEEEFloatingNumber));
}

PIXIE_EXPORT unsigned int PIXIE_API Decimal2IEEEFloating(double DecimalNumber) {
    return static_cast<unsigned int>(xia::util::ieee_float(DecimalNumber));
}

PIXIE_EXPORT unsigned int PIXIE_API Pixie16GetStatisticsSize(void) {
    return sizeof(stats_legacy);
}

PIXIE_EXPORT int PIXIE_API PixieGetHistogramLength(const unsigned short mod_num,
                                                   const unsigned short chan_num,
                                                   unsigned int* hist_length) {
    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, mod_num);
        module->channel_check(chan_num);
        *hist_length = module->channels[chan_num].fixture->config.max_histogram_length;
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }
    return 0;
}

PIXIE_EXPORT int PIXIE_API PixieGetTraceLength(const unsigned short mod_num,
                                               const unsigned short chan_num,
                                               unsigned int* trace_length) {
    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, mod_num);
        module->channel_check(chan_num);
        *trace_length = module->channels[chan_num].fixture->config.max_adc_trace_length;
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }
    return 0;
}

PIXIE_EXPORT int PIXIE_API PixieGetMaxNumBaselines(const unsigned short mod_num,
                                                   const unsigned short chan_num,
                                                   unsigned int* max_num_baselines) {
    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, mod_num);
        module->channel_check(chan_num);
        *max_num_baselines = module->channels[chan_num].fixture->config.max_num_baselines;
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }
    return 0;
}

PIXIE_EXPORT int PIXIE_API PixieGetReturnCodeText(int return_code, char* buf,
                                                  unsigned int buf_size) {
    std::string msg = xia::pixie::error::api_result_text(return_code);
    if (buf_size < msg.size()) {
        xia::pixie::error::error error(xia::pixie::error::code::invalid_value,
                                       "buffer size not large enough");
        return error.return_code();
    }
    strncpy(buf, msg.c_str(), msg.size());
    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16AcquireADCTrace(unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16AcquireADCTrace: ModNum=" << ModNum;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        module->get_traces();
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16AcquireBaselines(unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16AcquireBaselines: ModNum=" << ModNum;

    try {
        crate.ready();
        if (ModNum == crate.num_modules) {
            for (size_t mod_num = 0; mod_num < crate.num_modules; mod_num++) {
                xia::pixie::crate::module_handle module(crate, mod_num);
                if (*module == xia::pixie::hw::rev_H) {
                    return not_supported();
                }
                module->acquire_baselines();
            }
        } else {
            xia::pixie::crate::module_handle module(crate, ModNum);
            if (*module == xia::pixie::hw::rev_H) {
                return not_supported();
            }
            module->acquire_baselines();
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16AdjustOffsets(unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16AdjustOffsets: ModNum=" << ModNum;

    try {
        crate.ready();
        if (ModNum == crate.num_modules) {
            for (size_t mod_num = 0; mod_num < crate.num_modules; mod_num++) {
                xia::pixie::crate::module_handle module(crate, mod_num);
                module->adjust_offsets();
            }
        } else {
            xia::pixie::crate::module_handle module(crate, ModNum);
            module->adjust_offsets();
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16BLcutFinder(unsigned short ModNum, unsigned short ChanNum,
                                              unsigned int* BLcut) {
    xia_log(xia::log::debug) << "Pixie16BLcutFinder: ModNum=" << ModNum << " ChanNum=" << ChanNum;

    try {
        if (BLcut == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "BLcut is NULL");
        }
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        module->channel_check(ChanNum);

        xia::pixie::channel::range channels = {size_t(ChanNum)};
        xia::pixie::param::values values;
        module->bl_find_cut(channels, values);
        *BLcut = values[0];
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

static void PixieBootModule(xia::pixie::module::module& module, const char* ComFPGAConfigFile,
                            const char* SPFPGAConfigFile, const char* DSPCodeFile,
                            const char* DSPParFile, const char* DSPVarFile,
                            unsigned short BootPattern) {
    using firmware = xia::pixie::firmware::firmware;
    using hw_config = xia::pixie::hw::config;

    hw_config& config = module.eeprom.configs[0];

    firmware comm_fw("n/a", module.revision, config.adc_msps, config.adc_bits, "sys");
    firmware fippi_fw("n/a", module.revision, config.adc_msps, config.adc_bits, "fippi");
    firmware dsp_fw("n/a", module.revision, config.adc_msps, config.adc_bits, "dsp");
    firmware dsp_var("n/a", module.revision, config.adc_msps, config.adc_bits, "var");

    comm_fw.filename = ComFPGAConfigFile;
    comm_fw.slot.push_back(module.slot);
    xia::pixie::firmware::add(crate.firmware, comm_fw);

    fippi_fw.filename = SPFPGAConfigFile;
    fippi_fw.slot.push_back(module.slot);
    xia::pixie::firmware::add(crate.firmware, fippi_fw);

    dsp_fw.filename = DSPCodeFile;
    dsp_fw.slot.push_back(module.slot);
    xia::pixie::firmware::add(crate.firmware, dsp_fw);

    dsp_var.filename = DSPVarFile;
    dsp_var.slot.push_back(module.slot);
    xia::pixie::firmware::add(crate.firmware, dsp_var);

    crate.set_firmware();
    xia::pixie::firmware::load(crate.firmware);

    const auto num_bits = std::numeric_limits<unsigned short>::digits;
    auto pattern = std::bitset<num_bits>(BootPattern);

    module.probe();
    module.boot(pattern.test(BOOTPATTERN_COMFPGA_BIT), pattern.test(BOOTPATTERN_SPFPGA_BIT),
                pattern.test(BOOTPATTERN_DSPCODE_BIT));

    if (pattern.test(BOOTPATTERN_DSPPAR_BIT)) {
        load_settings_file(module, DSPParFile);
    }

    module.sync_hw(pattern.test(BOOTPATTERN_PROGFIPPI_BIT), pattern.test(BOOTPATTERN_SETDACS_BIT));
}

PIXIE_EXPORT int PIXIE_API Pixie16BootModule(const char* ComFPGAConfigFile,
                                             const char* SPFPGAConfigFile, const char*,
                                             const char* DSPCodeFile, const char* DSPParFile,
                                             const char* DSPVarFile, unsigned short ModNum,
                                             unsigned short BootPattern) {
    xia_log(xia::log::info) << "Pixie16BootModule: ModNum=" << ModNum << std::hex
                           << " BootPattern=0x" << BootPattern;
    xia_log(xia::log::info) << "Pixie16BootModule: ModNum=" << ModNum
                           << " ComFPGAConfigFile=" << ComFPGAConfigFile;
    xia_log(xia::log::info) << "Pixie16BootModule: ModNum=" << ModNum
                           << " SPFPGAConfigFile=" << SPFPGAConfigFile;
    xia_log(xia::log::info) << "Pixie16BootModule: ModNum=" << ModNum
                           << " DSPCodeFile=" << DSPCodeFile;
    xia_log(xia::log::info) << "Pixie16BootModule: ModNum=" << ModNum
                           << " DSPParFile=" << DSPParFile;
    xia_log(xia::log::info) << "Pixie16BootModule: ModNum=" << ModNum
                           << " DSPVarFile=" << DSPVarFile;

    const unsigned short device_boot_mask = (1 << BOOTPATTERN_COMFPGA_BIT) |
                                            (1 << BOOTPATTERN_SPFPGA_BIT) |
                                            (1 << BOOTPATTERN_DSPCODE_BIT);
    const unsigned short devices_boot = BootPattern & device_boot_mask;

    if (devices_boot != 0 && devices_boot != device_boot_mask) {
        xia_log(xia::log::error) << "invalid value: must boot COMM, FPGA and DSP together";
        return xia::pixie::error::return_code(
            xia::pixie::error::api_result(xia_error::code::invalid_value));
    }

    ///todo: This needs to use the handle, but the handle won't work until this is complete!
    try {
        if (ModNum == crate.num_modules) {
            xia::pixie::crate::crate::user user(crate);
            for (auto& module : crate.modules) {
                PixieBootModule(*module, ComFPGAConfigFile, SPFPGAConfigFile, DSPCodeFile,
                                DSPParFile, DSPVarFile, BootPattern);
            }
        } else {
            PixieBootModule(*crate.modules[ModNum], ComFPGAConfigFile, SPFPGAConfigFile,
                            DSPCodeFile, DSPParFile, DSPVarFile, BootPattern);
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16CheckExternalFIFOStatus(unsigned int* nFIFOWords,
                                                          unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16CheckExternalFIFOStatus: ModNum=" << ModNum;

    int result = 0;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        *nFIFOWords = static_cast<unsigned int>(module->read_list_mode_level());
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return result;
}

PIXIE_EXPORT int PIXIE_API Pixie16CheckRunStatus(unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16CheckRunStatus: ModNum=" << ModNum;

    int result = 0;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        if (module->run_active()) {
            result = 1;
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return result;
}

PIXIE_EXPORT double PIXIE_API Pixie16ComputeInputCountRate(unsigned int* Statistics,
                                                           unsigned short ModNum,
                                                           unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ComputeInputCountRate: ModNum=" << ModNum
                            << " ChanNum=" << ChanNum;

    double result = 0;

    try {
        if (Statistics == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "statistics pointer is NULL");
        }
        stats_legacy_ptr stats = reinterpret_cast<stats_legacy_ptr>(Statistics);
        stats->validate();
        if (ChanNum >= stats->num_channels) {
            throw xia_error(xia_error::code::channel_number_invalid, "invalid channel number");
        }
        result = stats->channels[ChanNum].input_count_rate();
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
    }

    return result;
}

PIXIE_EXPORT double PIXIE_API Pixie16ComputeLiveTime(unsigned int* Statistics,
                                                     unsigned short ModNum,
                                                     unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ComputeLiveTime: ModNum=" << ModNum
                            << " ChanNum=" << ChanNum;

    double result = 0;

    try {
        if (Statistics == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "statistics pointer is NULL");
        }
        auto stats = reinterpret_cast<stats_legacy_ptr>(Statistics);
        stats->validate();
        if (ChanNum >= stats->num_channels) {
            throw xia_error(xia_error::code::channel_number_invalid, "invalid channel number");
        }
        result = stats->channels[ChanNum].live_time();
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
    }

    return result;
}

PIXIE_EXPORT double PIXIE_API Pixie16ComputeOutputCountRate(unsigned int* Statistics,
                                                            unsigned short ModNum,
                                                            unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ComputeOutputCountRate: ModNum=" << ModNum
                            << " ChanNum=" << ChanNum;

    double result = 0;

    try {
        if (Statistics == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "statistics pointer is NULL");
        }
        stats_legacy_ptr stats = reinterpret_cast<stats_legacy_ptr>(Statistics);
        stats->validate();
        if (ChanNum >= stats->num_channels) {
            throw xia_error(xia_error::code::channel_number_invalid, "invalid channel number");
        }
        result = stats->channels[ChanNum].output_count_rate();
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
    }

    return result;
}

PIXIE_EXPORT double PIXIE_API Pixie16ComputeProcessedEvents(unsigned int* Statistics,
                                                            unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16ComputeProcessedEvents: ModNum=" << ModNum;
    (void) Statistics;
    (void) ModNum;
    return not_supported();
}

PIXIE_EXPORT double PIXIE_API Pixie16ComputeRawInputCount(unsigned int* Statistics,
                                                          unsigned short ModNum,
                                                          unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ComputeRawInputCount: ModNum=" << ModNum
                            << " ChanNum=" << ChanNum;

    double result = 0;

    try {
        if (Statistics == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "statistics pointer is NULL");
        }
        auto stats = reinterpret_cast<stats_legacy_ptr>(Statistics);
        stats->validate();
        if (ChanNum >= stats->num_channels) {
            throw xia_error(xia_error::code::channel_number_invalid, "invalid channel number");
        }
        result = stats->channels[ChanNum].input_counts();
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
    }

    return result;
}

PIXIE_EXPORT double PIXIE_API Pixie16ComputeRawOutputCount(unsigned int* Statistics,
                                                           unsigned short ModNum,
                                                           unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ComputeRawOutputCount: ModNum=" << ModNum
                            << " ChanNum=" << ChanNum;

    double result = 0;

    try {
        if (Statistics == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "statistics pointer is NULL");
        }
        auto stats = reinterpret_cast<stats_legacy_ptr>(Statistics);
        stats->validate();
        if (ChanNum >= stats->num_channels) {
            throw xia_error(xia_error::code::channel_number_invalid, "invalid channel number");
        }
        result = stats->channels[ChanNum].output_counts();
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
    }

    return result;
}

PIXIE_EXPORT double PIXIE_API Pixie16ComputeRealTime(unsigned int* Statistics,
                                                     unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16ComputeRealTime: ModNum=" << ModNum;

    double result = 0;

    try {
        if (Statistics == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "statistics pointer is NULL");
        }
        auto stats = reinterpret_cast<stats_legacy_ptr>(Statistics);
        stats->validate();
        result = double(stats->module.real_time());
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
    }

    return result;
}

PIXIE_EXPORT int PIXIE_API Pixie16CopyDSPParameters(unsigned short BitMask,
                                                    unsigned short SourceModule,
                                                    unsigned short SourceChannel,
                                                    unsigned short* DestinationMask) {
    xia_log(xia::log::debug) << "Pixie16CopyDSPParameters: Source Module=" << SourceModule
                            << " Source Channel=" << SourceChannel
                            << "  Destination Mask=" << DestinationMask << " Bit Mask=" << BitMask;

    try {
        crate.ready();
        xia::pixie::crate::module_handle source(crate, SourceModule);

        for (size_t dest_mod = 0; dest_mod < crate.num_modules; dest_mod++) {
            xia::pixie::crate::module_handle dest_handle(crate, dest_mod);

            for (size_t dest_chan = 0; dest_chan < dest_handle->num_channels; dest_chan++) {
                if (DestinationMask[dest_mod * dest_handle->num_channels + dest_chan] == 0) {
                    continue;
                }
                xia::pixie::param::copy_parameters(BitMask, source->channels[dest_chan].vars,
                                                   dest_handle->channels[dest_chan].vars);
            }
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16LoadDSPParametersFromFile(const char* FileName) {
    xia_log(xia::log::debug) << "Pixie16LoadDSPParametersFromFile: FileName=" << FileName;

    try {
        crate.ready();
        xia::pixie::crate::crate::user user(crate);
        for (auto& module : crate.modules) {
            load_settings_file(*module, FileName);
            xia::pixie::hw::run::control(*module, xia::pixie::hw::run::control_task::program_fippi);
            module->set_dacs();
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16EndRun(unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16EndRun: ModNum=" << ModNum;

    try {
        crate.ready();
        if (ModNum == crate.num_modules) {
            for (size_t mod_num = 0; mod_num < crate.num_modules; mod_num++) {
                xia::pixie::crate::module_handle module(crate, mod_num);
                module->run_end();
            }
        } else {
            xia::pixie::crate::module_handle module(crate, ModNum);
            module->run_end();
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16ExitSystem(unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16ExitSystem: ModNum=" << ModNum;

    try {
        crate.ready();
        if (ModNum == crate.num_modules) {
            for (size_t mod_num = 0; mod_num < crate.num_modules; mod_num++) {
                xia::pixie::crate::module_handle module(crate, mod_num,
                                                        xia::pixie::crate::module_handle::present);
                module->close();
            }
        } else {
            xia::pixie::crate::module_handle module(crate, ModNum,
                                                    xia::pixie::crate::module_handle::present);
            module->close();
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16InitSystem(unsigned short NumModules, unsigned short* PXISlotMap,
                                             unsigned short OfflineMode) {
    /*
     * Create a log file. The environment can change the level of logging.
     */
    xia::log::level log_level = xia::log::info;
    const char* env_log_level = std::getenv("PIXIE16_LOG_LEVEL");
    if (env_log_level != nullptr) {
        std::string ell = env_log_level;
        if (ell == "DEBUG" || ell == "debug") {
            log_level = xia::log::debug;
        } else if (ell == "INFO" || ell == "info") {
            log_level = xia::log::info;
        } else if (ell == "WARNING" || ell == "warning" || ell == "WARN" || ell == "warn") {
            log_level = xia::log::warning;
        } else if (ell == "ERROR" || ell == "error") {
            log_level = xia::log::error;
        }
    }
    xia::logging::start("log", "Pixie16Msg.log", true);
    xia::logging::set_level(log_level);

    xia_log(xia::log::info) << "Pixie16InitSystem: NumModules=" << NumModules
                           << " PXISlotMap=" << PXISlotMap << " OfflineMode=" << OfflineMode;

    /*
     * Not supported. We now have tools that can access the DSP variables.
     */
    if (OfflineMode != 0) {
        return not_supported();
    }

    try {
        xia::pixie::module::number_slots numbers;
        for (int i = 0; i < static_cast<int>(NumModules); ++i) {
            using number_slot = xia::pixie::module::number_slot;
            xia_log(xia::log::info)
                << "Pixie16InitSystem: slot map: " << PXISlotMap[i] << " => " << i;
            numbers.push_back(number_slot(i, PXISlotMap[i]));
        }

        crate.initialize();

        if (crate.modules.size() == 0) {
            crate.shutdown();
            throw xia_error(xia::pixie::error::code::module_total_invalid,
                            "Crate did not initialize with any modules.");
        }

        /*
         * Only handle the index to slot assignment if the user supplied the
         * number of modules (ie the length of the array) and the array.
         */
        if (NumModules > 0 && PXISlotMap != nullptr) {
            /*
             * If the number of modules requested is greater than the number
             * of modules in the crate, then it is an error.
             */
            if (NumModules > crate.num_modules) {
                crate.shutdown();
                throw xia_error(xia_error::code::module_total_invalid,
                                "module count does not match user supplied "
                                "number of modules");
            }
            crate.assign(numbers);
        }
    } catch (xia_error& e) {
        try {
            crate.shutdown();
        } catch (...) {
        }
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        try {
            crate.shutdown();
        } catch (...) {
        }
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        try {
            crate.shutdown();
        } catch (...) {
        }
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        try {
            crate.shutdown();
        } catch (...) {
        }
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16ReadDataFromExternalFIFO(unsigned int* ExtFIFO_Data,
                                                           unsigned int nFIFOWords,
                                                           unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16ReadDataFromExternalFIFO: ModNum=" << ModNum
                            << " nFIFOWords=" << nFIFOWords;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);

        xia::pixie::hw::words data(nFIFOWords);
        auto copied = module->read_list_mode(data);
        if (copied != nFIFOWords) {
            xia_log(xia::log::error)
                << "Failed to read FIFO words, requested nFIFOWords (" << nFIFOWords
                << "), copied " << copied << " for Module " << ModNum
                << ". Remaining values filled with zero.";
        }
        for (size_t i = 0; i < copied; i++) {
            ExtFIFO_Data[i] = data[i];
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16ReadHistogramFromModule(unsigned int* Histogram,
                                                          unsigned int NumWords,
                                                          unsigned short ModNum,
                                                          unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ReadHistogramFromModule: ModNum=" << ModNum
                            << " ChanNum=" << ChanNum << " NumWords=" << NumWords;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        module->channel_check(ChanNum);
        auto chan = module->channels[ChanNum];
        auto read_words = NumWords;
        if (read_words > chan.fixture->config.max_histogram_length) {
            xia_log(xia::log::warning)
                << "NumWords (" << NumWords << ") greater than the max_histogram_length ("
                << chan.fixture->config.max_histogram_length << ") for Module " << ModNum
                << " Channel " << ChanNum
                << ". Truncating to the maximum length and filling with max bin values.";
            read_words = chan.fixture->config.max_histogram_length;

            for (unsigned int i = read_words; i < NumWords; i++)
                Histogram[i] = std::numeric_limits<unsigned int>::max();
        }
        if (read_words < chan.fixture->config.max_histogram_length) {
            xia_log(xia::log::warning)
                << "NumWords (" << NumWords << ") less than the max_histogram_length ("
                << chan.fixture->config.max_histogram_length << ") for Module " << ModNum
                << " Channel " << ChanNum << ". You may not be capturing all the data.";
        }
        module->read_histogram(ChanNum, Histogram, read_words);
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16ReadModuleInfo(unsigned short ModNum, unsigned short* ModRev,
                                                 unsigned int* ModSerNum,
                                                 unsigned short* ModADCBits,
                                                 unsigned short* ModADCMSPS) {
    xia_log(xia::log::debug) << "Pixie16ReadModuleInfo: ModNum=" << ModNum;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum,
                                                xia::pixie::crate::module_handle::present);
        if (ModRev)
            *ModRev = module->revision;
        if (ModSerNum)
            *ModSerNum = module->serial_num;
        if (ModADCBits)
            *ModADCBits = module->eeprom.configs[0].adc_bits;
        if (ModADCMSPS)
            *ModADCMSPS = module->eeprom.configs[0].adc_msps;
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API PixieGetModuleInfo(unsigned short mod_num, module_config* cfg) {
    xia_log(xia::log::debug) << "PixieReadModuleInfo: ModNum=" << mod_num;

    try {
        if (!cfg) {
            throw xia::pixie::error::error(
                xia::pixie::error::code::module_info_failure,
                "Module configuration object was null. Please provide a structure to populate.");
        }

        crate.ready();
        xia::pixie::crate::module_handle module(crate, mod_num,
                                                xia::pixie::crate::module_handle::present);

        cfg->adc_bit_resolution = module->eeprom.configs[0].adc_bits;
        cfg->adc_sampling_frequency = module->eeprom.configs[0].adc_msps;
        cfg->number = module->number;
        cfg->number_of_channels = static_cast<unsigned short>(module->num_channels);
        cfg->revision = module->revision;
        cfg->serial_number = module->serial_num;
        cfg->slot = module->slot;
        for (const auto& fw: module->firmware) {
            if (fw->device == "sys") {
                std::memset(cfg->sys_fpga, 0, sizeof(cfg->sys_fpga));
                std::strncpy(cfg->sys_fpga, fw->filename.c_str(), sizeof(cfg->sys_fpga) - 1);
            }
            if (fw->device == "fippi") {
                std::memset(cfg->sp_fpga, 0, sizeof(cfg->sp_fpga));
                std::strncpy(cfg->sp_fpga, fw->filename.c_str(), sizeof(cfg->sp_fpga) - 1);
            }
            if (fw->device == "dsp") {
                std::memset(cfg->dsp_code, 0, sizeof(cfg->dsp_code));
                std::strncpy(cfg->dsp_code, fw->filename.c_str(), sizeof(cfg->dsp_code) - 1);
            }
            if (fw->device == "var") {
                std::memset(cfg->dsp_var, 0, sizeof(cfg->dsp_var));
                std::strncpy(cfg->dsp_var, fw->filename.c_str(), sizeof(cfg->dsp_var) - 1);
            }
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16ReadSglChanADCTrace(unsigned short* Trace_Buffer,
                                                      unsigned int Trace_Length,
                                                      unsigned short ModNum,
                                                      unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ReadSglChanADCTrace: ModNum=" << ModNum
                             << " ChanNum=" << ChanNum << " Trace_Length=" << Trace_Length;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        module->channel_check(ChanNum);
        module->read_adc(ChanNum, Trace_Buffer, Trace_Length, false);
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16ReadSglChanBaselines(double* Baselines, double* TimeStamps,
                                                       unsigned short NumBases,
                                                       unsigned short ModNum,
                                                       unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ReadSglChanBaselines: ModNum=" << ModNum
                            << " ChanNum=" << ChanNum << " NumBases=" << NumBases;

    try {
        if (Baselines == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "Baselines is NULL");
        }
        if (TimeStamps == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "TimeStamps is NULL");
        }

        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        module->channel_check(ChanNum);

        xia::pixie::channel::range channels = {size_t(ChanNum)};
        xia::pixie::channel::baseline::channels_values values(1);
        module->bl_get(channels, values, false);
        xia::pixie::channel::baseline::values& cv = values[0];
        for (size_t v = 0; v < size_t(NumBases); ++v) {
            TimeStamps[v] = std::get<0>(cv[v]);
            Baselines[v] = std::get<1>(cv[v]);
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16ReadSglChanPar(const char* ChanParName, double* ChanParData,
                                                 unsigned short ModNum, unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16ReadSglChanPar: ModNum=" << ModNum << " ChanNum=" << ChanNum
                            << " ChanParName=" << ChanParName;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        module->channel_check(ChanNum);

        *ChanParData = module->read(ChanParName, ChanNum);
        xia_log(xia::log::debug) << "Pixie16ReadSglChanPar: ModNum=" << ModNum
                                << " ChanNum=" << ChanNum << " ChanParName=" << ChanParName
                                << " ChanParData=" << *ChanParData;
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16ReadSglModPar(const char* ModParName, unsigned int* ModParData,
                                                unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16ReadSglModPar: ModNum=" << ModNum
                            << " ModParName=" << ModParName;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        *ModParData = module->read(ModParName);
        xia_log(xia::log::debug) << "Pixie16ReadSglModPar: ModNum=" << ModNum
                                << " ModParName=" << ModParName << " ModParData=" << *ModParData;
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16ReadStatisticsFromModule(unsigned int* Statistics,
                                                           unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16ReadStatisticsFromModule: ModNum=" << ModNum;

    try {
        if (Statistics == nullptr) {
            throw xia_error(xia_error::code::invalid_value, "statistics pointer is NULL");
        }
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        stats_legacy_ptr legacy_stats = new (Statistics) stats_legacy(module->eeprom.configs);
        legacy_stats->validate();
        xia::pixie::stats::stats stats(*module);
        module->read_stats(stats);
        legacy_stats->num_channels = module->num_channels;
        legacy_stats->module = stats.mod;
        for (size_t channel = 0; channel < module->num_channels; ++channel) {
            legacy_stats->channels[channel] = stats.chans[channel];
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16SaveDSPParametersToFile(const char* FileName) {
    xia_log(xia::log::debug) << "Pixie16SaveDSPParametersToFile: FileName=" << FileName;

    try {
        xia::pixie::config::export_json(FileName, crate);
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16SaveHistogramToFile(const char* FileName, unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16SaveHistogramToFile: ModNum=" << ModNum
                            << " FileName=" << FileName;

    return not_supported();
}

PIXIE_EXPORT int PIXIE_API Pixie16SetDACs(unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16SetDACs: ModNum=" << ModNum;

    try {
        crate.ready();
        if (ModNum == crate.num_modules) {
            for (size_t mod_num = 0; mod_num < crate.num_modules; mod_num++) {
                xia::pixie::crate::module_handle module(crate, mod_num);
                module->set_dacs();
            }
        } else {
            xia::pixie::crate::module_handle module(crate, ModNum);
            module->set_dacs();
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16StartHistogramRun(unsigned short ModNum, unsigned short mode) {
    xia_log(xia::log::debug) << "Pixie16StartHistogramRun: ModNum=" << ModNum << " mode=" << mode;

    try {
        xia::pixie::hw::run::run_mode run_mode;
        switch (mode) {
            case 0:
                run_mode = xia::pixie::hw::run::run_mode::resume;
                break;
            case 1:
                run_mode = xia::pixie::hw::run::run_mode::new_run;
                break;
            default:
                throw xia_error(xia_error::code::invalid_value, "invalid histogram start run mode");
        }
        crate.ready();
        if (ModNum == crate.num_modules) {
            for (size_t mod_num = 0; mod_num < crate.num_modules; mod_num++) {
                xia::pixie::crate::module_handle module(crate, mod_num);
                module->start_histograms(run_mode);
            }
        } else {
            xia::pixie::crate::module_handle module(crate, ModNum);
            module->start_histograms(run_mode);
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16StartListModeRun(unsigned short ModNum, unsigned short RunType,
                                                   unsigned short mode) {
    xia_log(xia::log::debug) << "Pixie16StartListModeRun: ModNum=" << ModNum
                            << " RunType=" << RunType << " mode=" << mode;

    try {
        if (RunType != static_cast<unsigned short>(xia::pixie::hw::run::run_task::list_mode)) {
            throw xia_error(xia_error::code::invalid_value,
                            "invalid list-mode start run type (must be 0x100)");
        }
        xia::pixie::hw::run::run_mode run_mode;
        switch (mode) {
            case 0:
                run_mode = xia::pixie::hw::run::run_mode::resume;
                break;
            case 1:
                run_mode = xia::pixie::hw::run::run_mode::new_run;
                break;
            default:
                throw xia_error(xia_error::code::invalid_value, "invalid list-mode start run mode");
        }

        crate.ready();
        if (ModNum == crate.num_modules) {
            for (size_t mod_num = 0; mod_num < crate.num_modules; mod_num++) {
                xia::pixie::crate::module_handle module(crate, mod_num);
                module->start_listmode(run_mode);
            }
        } else {
            xia::pixie::crate::module_handle module(crate, ModNum);
            module->start_listmode(run_mode);
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16TauFinder(unsigned short ModNum, double* Tau) {
    xia_log(xia::log::debug) << "Pixie16TauFinder: ModNum=" << ModNum;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        if (*module == xia::pixie::hw::rev_H) {
            return not_supported();
        }
        module->tau_finder();
        xia::pixie::hw::doubles taus;
        module->read_autotau(taus);
        std::copy(taus.begin(), taus.end(), Tau);
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }
    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16WriteSglChanPar(const char* ChanParName, double ChanParData,
                                                  unsigned short ModNum, unsigned short ChanNum) {
    xia_log(xia::log::debug) << "Pixie16WriteSglChanPar: ModNum=" << ModNum << " ChanNum=" << ChanNum
                            << " ChanParName=" << ChanParName << " ChanParData=" << ChanParData;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, ModNum);
        module->channel_check(ChanNum);
        module->write(ChanParName, ChanNum, ChanParData);
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API Pixie16WriteSglModPar(const char* ModParName, unsigned int ModParData,
                                                 unsigned short ModNum) {
    xia_log(xia::log::debug) << "Pixie16WriteSglModPar: ModNum=" << ModNum
                            << " ModParName=" << ModParName << " ModParData=" << ModParData;

    try {
        crate.ready();
        bool bcast;
        if (ModNum == crate.num_modules) {
            bcast = true;
        } else {
            xia::pixie::crate::module_handle module(crate, ModNum);
            bcast = module->write(ModParName, ModParData);
        }
        if (bcast) {
            xia::pixie::crate::crate::user user(crate);
            for (auto& module : crate.modules) {
                if (ModNum != module->number && module->online()) {
                    module->write(ModParName, ModParData);
                }
            }
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API PixieBootCrate(const char* settings_file,
                                          const PIXIE_BOOT_MODE boot_mode) {
    xia_log(xia::log::debug) << "PixieBootCrate: settings_file=" << settings_file
                            << " boot-mode=" << boot_mode;

    try {
        crate.ready();
        crate.set_firmware();

        bool import_settings;
        bool boot;
        bool force;

        switch (boot_mode) {
            case PIXIE_BOOT_SETTINGS_LOAD:
                if (settings_file == nullptr) {
                    throw xia_error(xia_error::code::invalid_value,
                                    "settings file pointer is NULL");
                }
                import_settings = true;
                boot = false;
                force = false;
                if (!crate.probe()) {
                    throw xia_error(xia_error::code::module_offline,
                                    "fast boot not available with offline modules");
                }
                break;
            case PIXIE_BOOT_RESET_LOAD:
                import_settings = settings_file != nullptr;
                boot = true;
                force = true;
                break;
            default:
                import_settings = false;
                boot = !crate.probe();
                force = false;
                break;
        }

        if (boot) {
            crate.boot(force);
        }

        if (import_settings) {
            xia::pixie::module::number_slots loaded_slots;
            crate.import_config(settings_file, loaded_slots);
            crate.initialize_afe();
        }
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API PixieGetWorkerConfiguration(const unsigned short mod_num,
                                                       fifo_worker_config* worker_config) {
    xia_log(xia::log::debug) << "PixieGetWorkerConfiguration: Module=" << mod_num;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, mod_num,
                                                xia::pixie::crate::module_handle::present);
        worker_config->bandwidth_mb_per_sec = module->fifo_bandwidth;
        worker_config->buffers = module->fifo_buffers;
        worker_config->dma_trigger_level_bytes = module->fifo_dma_trigger_level;
        worker_config->hold_usecs = module->fifo_hold_usecs;
        worker_config->idle_wait_usecs = module->fifo_idle_wait_usecs;
        worker_config->run_wait_usecs = module->fifo_run_wait_usecs;
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }
    return 0;
}


PIXIE_EXPORT int PIXIE_API PixieRegisterFirmware(const unsigned int version, const int revision,
                                                 const int adc_msps, const int adc_bits,
                                                 const char* device, const char* path,
                                                 unsigned short ModNum) {
    xia_log(xia::log::debug) << "PixieRegisterFirmware: version=" << version
                             << " revision=" << revision << " adc_msps=" << adc_msps
                             << " adc_bits=" << adc_bits << " device=" << device << " path=" << path
                             << " ModNum=" << ModNum;

    using firmware = xia::pixie::firmware::firmware;

    try {
        int slot = -1;
        if (ModNum != 0xACE) {
            xia::pixie::crate::module_handle module(crate, ModNum,
                                                    xia::pixie::crate::module_handle::present);
            slot = module->slot;
        }
        std::string ver_s = std::to_string(version);
        std::string dev_s = device;
        firmware fw(ver_s, revision, adc_msps, adc_bits, dev_s);
        fw.filename = path;
        if (slot > 0) {
            fw.slot.push_back(slot);
        }
        xia::pixie::firmware::add(crate.firmware, fw);
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << "unknown error: " << e.what();
        return xia::pixie::error::return_code_unknown_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }

    return 0;
}

PIXIE_EXPORT int PIXIE_API PixieSetWorkerConfiguration(const unsigned short mod_num,
                                                       fifo_worker_config* worker_config) {
    xia_log(xia::log::debug) << "PixieGetWorkerConfiguration: Module=" << mod_num;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, mod_num,
                                                xia::pixie::crate::module_handle::present);
        module->set_fifo_bandwidth(worker_config->bandwidth_mb_per_sec);
        module->set_fifo_buffers(worker_config->buffers);
        module->set_fifo_dma_trigger_level(worker_config->dma_trigger_level_bytes);
        module->set_fifo_hold(worker_config->hold_usecs);
        module->set_fifo_idle_wait(worker_config->idle_wait_usecs);
        module->set_fifo_run_wait(worker_config->run_wait_usecs);
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }
    return 0;
}

PIXIE_EXPORT int PIXIE_API PixieReadRunFifoStats(unsigned short mod_num,
                                                 struct module_fifo_stats* fifo_stats) {
    xia_log(xia::log::debug) << "PixieReadModuleRunFifoStats: Module=" << mod_num;

    try {
        crate.ready();
        xia::pixie::crate::module_handle module(crate, mod_num,
                                                xia::pixie::crate::module_handle::present);
        xia::pixie::module::module::fifo_stats snapshot;
        snapshot = module->run_stats;
        fifo_stats->in = snapshot.get_in_bytes();
        fifo_stats->out = snapshot.get_out_bytes();
        fifo_stats->dma_in = snapshot.get_dma_in_bytes();
        fifo_stats->overflows = snapshot.overflows;
        fifo_stats->dropped = snapshot.dropped;
        fifo_stats->hw_overflows = snapshot.hw_overflows;
    } catch (xia_error& e) {
        xia_log(xia::log::error) << e;
        return e.return_code();
    } catch (std::bad_alloc& e) {
        xia_log(xia::log::error) << "bad allocation: " << e.what();
        return xia::pixie::error::return_code_bad_alloc_error();
    } catch (...) {
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        return xia::pixie::error::return_code_unknown_error();
    }
    return 0;
}
