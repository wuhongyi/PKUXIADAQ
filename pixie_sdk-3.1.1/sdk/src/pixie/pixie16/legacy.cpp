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

/** @file legacy.cpp
 * @brief Namespace and functions to interface with legacy settings files
 * @note This support will be deprecated July 31, 2023
 */

#include <algorithm>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include <pixie/param.hpp>
#include <pixie/util.hpp>

#include <pixie/pixie16/hw.hpp>
#include <pixie/pixie16/legacy.hpp>
#include <pixie/pixie16/memory.hpp>

namespace xia {
namespace pixie {
namespace legacy {
settings::settings(firmware::firmware_ref vars) {
    module_var_descriptors = param::module_var_descs(param::get_module_var_descriptors());
    channel_var_descriptors = param::channel_var_descs(param::get_channel_var_descriptors());
    param::load(vars, module_var_descriptors, channel_var_descriptors);
    addresses.set(MAX_CHANNELS, module_var_descriptors, channel_var_descriptors);
}

settings::settings(module::module& module) {
    if (module.num_channels != MAX_CHANNELS) {
        throw error(pixie::error::code::channel_number_invalid,
                    "invalid module channel count for legacy settings file");
    }
    module_var_descriptors = param::module_var_descs(module.module_var_descriptors);
    channel_var_descriptors = param::channel_var_descs(module.channel_var_descriptors);
    addresses = module.param_addresses;
}

int settings::num_modules() const {
    return dsp_mem.size() / N_DSP_PAR;
}

void settings::load(const std::string& parfile) {
    std::ifstream input(parfile, std::ifstream::in | std::ifstream::binary);
    if (!input) {
        throw error(pixie::error::code::file_open_failure,
                    "opening legacy settings config: " + parfile + ": " + std::strerror(errno));
    }

    input.unsetf(std::ios::skipws);
    input.seekg(0, std::ios::end);
    auto size = input.tellg();
    input.seekg(0, std::ios::beg);

    if ((size % N_DSP_PAR) != 0) {
        throw error(pixie::error::code::module_total_invalid,
                    "settings file not a multiple of N_DSP_PAR");
    }

    dsp_mem.clear();
    dsp_mem.resize(static_cast<unsigned int>(size) / sizeof(hw::word));
    input.read(reinterpret_cast<char*>(dsp_mem.data()), size);
}

void settings::import(module::module& module) {
    if (module.number >= num_modules()) {
        throw error(pixie::error::code::module_number_invalid,
                    "invalid module number for legacy settings file: "
                    "file's module num: " +
                        std::to_string(num_modules()));
    }
    if (module.num_channels != MAX_CHANNELS) {
        throw error(pixie::error::code::channel_number_invalid,
                    "invalid module channel count for legacy settings file");
    }
    if (dsp_mem.empty()) {
        throw error(pixie::error::code::channel_number_invalid, "no legacy settings file loaded");
    }
    for (auto& desc : module_var_descriptors) {
        if (desc.writeable()) {
            auto value = read_var(desc.par, module.number);
            if (desc.par == xia::pixie::param::module_var::SlotID) {
                module.write_var(desc.par, module.slot, 0, false);
            } else if (desc.par == xia::pixie::param::module_var::ModNum) {
                module.write_var(desc.par, module.number, 0, false);
            } else {
                module.write_var(desc.par, value, 0, false);
            }
        }
    }
    for (auto& desc : channel_var_descriptors) {
        if (desc.writeable()) {
            for (size_t channel = 0; channel < MAX_CHANNELS; ++channel) {
                for (size_t offset = 0; offset < desc.size; ++offset) {
                    auto value = read_var(desc.par, module.number, channel, offset);
                    module.write_var(desc.par, value, channel, offset, false);
                }
            }
        }
    }
}

void settings::write(module::module& module) {
    if (module.number >= num_modules()) {
        throw error(pixie::error::code::module_number_invalid,
                    "invalid module number for legacy settings file: "
                    "file's module num: " +
                        std::to_string(num_modules()));
    }
    if (module.num_channels != MAX_CHANNELS) {
        throw error(pixie::error::code::channel_number_invalid,
                    "invalid module channel count for legacy settings file");
    }
    if (dsp_mem.empty()) {
        throw error(pixie::error::code::channel_number_invalid, "no legacy settings file loaded");
    }
    hw::memory::dsp dsp(module);
    hw::words regs(dsp_mem);
    regs.resize(DSP_IO_BORDER);
    dsp.write(addresses.full.start, regs);
}

param::value_type settings::read_var(param::module_var var, int module, size_t offset) const {
    if (module >= num_modules()) {
        throw error(error::code::module_number_invalid,
                    "invalid module: " + std::to_string(module));
    }

    auto& desc = module_var_descriptors[int(var)];

    if (offset >= desc.size) {
        throw error(error::code::module_number_invalid,
                    "invalid module var offset: " + desc.name + ": " + std::to_string(offset));
    }

    auto word = index(desc, module, offset);
    return dsp_mem[word];
}

param::value_type settings::read_var(param::channel_var var, int module, int channel,
                                     size_t offset) const {
    if (module >= num_modules()) {
        throw error(error::code::module_number_invalid,
                    "invalid module: " + std::to_string(module));
    }

    auto& desc = channel_var_descriptors[int(var)];

    if (offset >= desc.size) {
        throw error(error::code::module_number_invalid,
                    "invalid channel var offset: " + desc.name + ": " + std::to_string(offset));
    }

    auto word = index(desc, module, channel, offset);
    return dsp_mem[word];
}

size_t settings::index(const param::module_var_desc& desc, int module, size_t offset) const {
    auto base = module * N_DSP_PAR;
    return base + address(desc, offset) - addresses.full.start;
}

size_t settings::index(const param::channel_var_desc& desc, int module, int channel,
                       size_t offset) const {
    auto base = module * N_DSP_PAR;
    return base + address(desc, channel, offset) - addresses.full.start;
}

size_t settings::address(const param::module_var_desc& desc, size_t offset) const {
    return desc.address + offset;
}

size_t settings::address(const param::channel_var_desc& desc, int channel, size_t offset) const {
    return desc.address + (channel * desc.size) + offset;
}

void settings::output(std::ostream& out) const {
    util::ostream_guard guard(out);
    out << "num modules: " << num_modules() << " (" << dsp_mem.size() << ')' << std::left
        << std::endl;
    for (auto module = 0; module < num_modules(); ++module) {
        for (auto& desc : module_var_descriptors) {
            out << ' ' << std::setw(2) << module << ": word=" << std::setw(6) << index(desc, module)
                << " 0x" << std::hex << address(desc) << ' ' << desc.name << " = ";
            for (size_t offset = 0; offset < desc.size; ++offset) {
                auto value = read_var(desc.par, module, offset);
                if (offset > 0) {
                    out << ", ";
                }
                out << std::dec << value << std::hex << " (0x" << value << ')';
            }
            out << std::endl;
        }
        for (auto& desc : channel_var_descriptors) {
            for (size_t channel = 0; channel < MAX_CHANNELS; ++channel) {
                out << ' ' << std::setw(2) << module << ": word=" << std::setw(6)
                    << index(desc, module, channel) << " 0x" << std::hex << address(desc, channel)
                    << ' ' << desc.name << '[' << std::setw(2) << channel << "] = ";
                for (size_t offset = 0; offset < desc.size; ++offset) {
                    auto value = read_var(desc.par, module, channel, offset);
                    if (offset > 0) {
                        out << ", ";
                    }
                    out << std::dec << value << std::hex << " (0x" << value << ')' << std::dec;
                }
                out << std::endl;
            }
        }
    }
}

}  // namespace legacy
}  // namespace pixie
}  // namespace xia

std::ostream& operator<<(std::ostream& out, const xia::pixie::legacy::settings& settings) {
    settings.output(out);
    return out;
}
