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

/** @file legacy.hpp
 * @brief Namespace and functions to interface with legacy settings files
 * @note This file will be deprecated July 31, 2023
 */

#ifndef PIXIE_PIXIE16_LEGACY_HPP
#define PIXIE_PIXIE16_LEGACY_HPP

#include <string>

#include <pixie/error.hpp>
#include <pixie/os_compat.hpp>

#include <pixie/pixie16/crate.hpp>

namespace xia {
namespace pixie {
/**
 * @brief Used to provide some basic legacy functionality.
 */
namespace legacy {

typedef pixie::error::error error;

/**
 * @brief Defines a data structure related to binary settings files.
 */
struct settings {
    /**
     * Hardcoded constant from the legacy code.
     */
    static const size_t N_DSP_PAR = 1280;

    /**
     * Legacy settings files are limited to 16 channels only
     */
    static const size_t MAX_CHANNELS = 16;

    /**
     * The number of DSP words written when pushing the variables as a block.
     */
    static const size_t DSP_IO_BORDER = 832;

    hw::words dsp_mem;
    param::module_var_descs module_var_descriptors;
    param::channel_var_descs channel_var_descriptors;
    param::address_map addresses;

    /*
     * Construct the parameters and memory map from a var file or any existing
     * module. Must be a 16 channel module.
     */
    settings(firmware::firmware_ref vars);
    settings(module::module& module);

    int num_modules() const;

    /**
     * @brief Load the settings file into the DSP memory buffer
     * @param parfile The path to the binary settings file to load.
     */
    void load(const std::string& parfile);

    /**
     * @brief Import the DSP memory contents into the module
     * @param module The module that we'd like to associate with the settings.
     */
    void import(module::module& module);

    /**
     * @brief Actually write the DSP memory contents to the DSP hardware
     * @param module The module that we'd like to write to.
     */
    void write(module::module& module);

    /*
     * Read a var value from the internal DSP memory buffer
     */
    param::value_type read_var(param::module_var var, int module, size_t offset = 0) const;
    param::value_type read_var(param::channel_var var, int module, int channel,
                               size_t offset = 0) const;

    /*
     * Values of interest when looking at par files.
     */
    size_t index(const param::module_var_desc& desc, int module, size_t offset = 0) const;
    size_t index(const param::channel_var_desc& desc, int module, int channel,
                 size_t offset = 0) const;
    size_t address(const param::module_var_desc& desc, size_t offset = 0) const;
    size_t address(const param::channel_var_desc& desc, int channel, size_t offset = 0) const;

    /*
     * Output the settings file contents in a readable format
     */
    void output(std::ostream& out) const;
};

}  // namespace legacy
}  // namespace pixie
}  // namespace xia

/**
 * @brief Output stream operator for a legacy settings object
 */
std::ostream& operator<<(std::ostream& out, const xia::pixie::legacy::settings& settings);

#endif  // PIXIE_PIXIE16_LEGACY_HPP
