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

/** @file sim.hpp
 * @brief Defines a Pixie-16 simulation framework to facilitate testing
 */

#ifndef PIXIE_SDK_SYSTEM_SIMULATION_HPP
#define PIXIE_SDK_SYSTEM_SIMULATION_HPP

#include <iostream>

#include <pixie/error.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
/**
 * @brief Collects tools for simulating the Pixie-16 hardware.
 */
namespace sim {
typedef xia::pixie::error::error error;

/**
 * @brief A Simulated a module derived from the module class.
 */
class module : public xia::pixie::module::module {
public:
    static const size_t pci_addr_space_size = 8 * 1024 * 1024;

    typedef xia::pixie::module::error error;

    ~module() override;

    void open(size_t device_number) override;
    void close() override;
    void probe() override;
    void boot(bool boot_comms = true, bool boot_fippi = true, bool boot_dsp = true) override;
    void initialize() override;
    void init_values() override;

    void load_var_defaults(const std::string& file);
    void load_var_defaults(std::istream& input);

    std::unique_ptr<uint8_t[]> pci_memory;
    std::string var_defaults;
};

/**
 * @brief Simulated a crate. Creates the simulated module.
 */
class crate : public xia::pixie::crate::crate {
public:
    typedef xia::pixie::crate::error error;

    void add_module() override;
};

/**
 * @brief Module definition used to set up the simulation.
 */
struct module_def {
    size_t device_number;
    int slot;
    int revision;
    int eeprom_format;
    int serial_num;
    int num_channels;
    int adc_bits;
    int adc_msps;
    int adc_clk_div;
    std::string var_defaults;

    module_def();
};

typedef std::vector<module_def> module_defs;

extern module_defs mod_defs;

void load_module_defs(const std::string mod_def_file);
void load_module_defs(std::istream& input);
void add_module_def(const std::string mod_desc, const char delimiter = ' ');
}  // namespace sim
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_SDK_SYSTEM_SIMULATION_HPP
