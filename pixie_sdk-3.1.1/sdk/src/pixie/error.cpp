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

/** @file error.cpp
 * @brief Implements custom error codes used throughout the SDK.
 */

#include <iomanip>
#include <iostream>
#include <map>

#include <pixie/error.hpp>
#include <pixie/log.hpp>
#include <pixie/util.hpp>

namespace xia {
namespace pixie {
namespace error {
struct result_code {
    const int result;
    const char* text;

    result_code() : result(99), text("error code not set") {}
    result_code(const int result_, const char* text_) : result(result_), text(text_) {}
};

static const std::map<code, result_code> result_codes = {
    {code::success, {0, "success"}},
    /*
     * Crate
     */
    {code::crate_already_open, {100, "crate already open"}},
    {code::crate_not_ready, {101, "crate not ready"}},
    {code::crate_invalid_param, {102, "invalid system parameter"}},
    /*
     * Module
     */
    {code::module_number_invalid, {200, "invalid module number"}},
    {code::module_total_invalid, {201, "invalid module count"}},
    {code::module_already_open, {202, "module already open"}},
    {code::module_close_failure, {203, "module failed to close"}},
    {code::module_offline, {204, "module offline"}},
    {code::module_info_failure, {205, "module information failure"}},
    {code::module_invalid_operation, {206, "invalid module operation"}},
    {code::module_invalid_firmware, {207, "invalid module firmware"}},
    {code::module_initialize_failure, {208, "module initialization failure"}},
    {code::module_invalid_param, {209, "invalid module parameter"}},
    {code::module_invalid_var, {210, "invalid module variable"}},
    {code::module_param_disabled, {211, "module parameter disabled"}},
    {code::module_param_readonly, {212, "module parameter is readonly"}},
    {code::module_param_writeonly, {213, "module parameter is writeonly"}},
    {code::module_task_timeout, {214, "module task timeout"}},
    {code::module_invalid_slot, {215, "invalid module slot number"}},
    {code::module_not_found, {216, "module not found"}},
    {code::module_test_invalid, {217, "module test invalid"}},
    /*
     * Channel
     */
    {code::channel_number_invalid, {300, "invalid channel number"}},
    {code::channel_invalid_param, {301, "invalid channel parameter"}},
    {code::channel_invalid_var, {302, "invalid channel variable"}},
    {code::channel_invalid_index, {303, "invalid channel variable"}},
    {code::channel_param_disabled, {304, "invalid channel index"}},
    {code::channel_param_readonly, {305, "channel parameter is readonly"}},
    {code::channel_param_writeonly, {306, "channel parameter is writeonly"}},
    /*
     * Device
     */
    {code::device_load_failure, {500, "device failed to load"}},
    {code::device_boot_failure, {501, "device failed to boot"}},
    {code::device_initialize_failure, {502, "device failed to initialize"}},
    {code::device_copy_failure, {503, "device variable copy failed"}},
    {code::device_image_failure, {504, "device image failure"}},
    {code::device_hw_failure, {505, "device hardware failure"}},
    {code::device_dma_failure, {506, "device dma failure"}},
    {code::device_dma_busy, {507, "device dma busy"}},
    {code::device_fifo_failure, {508, "device fifo failure"}},
    {code::device_eeprom_failure, {509, "device eeprom failure"}},
    {code::device_eeprom_bad_type, {510, "device eeprom bad type"}},
    {code::device_eeprom_not_found, {511, "device eeprom tag not found"}},
    /*
     * Configuration
     */
    {code::config_invalid_param, {600, "invalid config parameter"}},
    {code::config_param_not_found, {601, "config parameter not found"}},
    {code::config_json_error, {602, "config json error"}},
    /*
     * File handling
     */
    {code::file_not_found, {700, "file not found"}},
    {code::file_open_failure, {701, "file open failure"}},
    {code::file_read_failure, {702, "file read failure"}},
    {code::file_size_invalid, {703, "invalid file size"}},
    {code::file_create_failure, {704, "file create failure"}},
    /*
     * System
     */
    {code::no_memory, {800, "no memory"}},
    {code::slot_map_invalid, {801, "invalid slot map"}},
    {code::invalid_value, {802, "invalid number"}},
    {code::not_supported, {803, "not supported"}},
    {code::buffer_pool_empty, {804, "buffer pool empty"}},
    {code::buffer_pool_not_empty, {805, "buffer pool not empty"}},
    {code::buffer_pool_busy, {806, "buffer pool bust"}},
    {code::buffer_pool_not_enough, {807, "buffer pool not enough"}},
    /*
     * Catch all
     */
    {code::unknown_error, {900, "unknown error"}},
    {code::internal_failure, {901, "internal failure"}},
    {code::bad_allocation, {902, "bad allocation"}},
    {code::bad_error_code, {990, "bad error code"}},
};

bool check_code_match() {
    return result_codes.size() == size_t(code::last);
}

error::error(const code type_, const std::ostringstream& what)
    : runtime_error(what.str()), type(type_) {
    log(log::error) << "error(except): " << what.str();
}

error::error(const code type_, const std::string& what) : runtime_error(what), type(type_) {
    log(log::error) << "error(except): " << what;
}

error::error(const code type_, const char* what) : runtime_error(what), type(type_) {
    log(log::error) << "error(except): " << what;
}

void error::output(std::ostream& out) {
    util::ostream_guard flags(out);
    out << std::setfill(' ') << "error: code:" << std::setw(3) << result() << " : " << what();
}

int error::result() const {
    return api_result(type);
}

std::string error::result_text() const {
    return api_result_text(type);
}

int error::return_code() const {
    return 0 - result();
}

int api_result(enum code type) {
    auto search = result_codes.find(type);
    int result;
    if (search == result_codes.end()) {
        result = result_codes.at(code::bad_error_code).result;
    } else {
        result = search->second.result;
    }
    return result;
}

std::string api_result_text(enum code type) {
    auto search = result_codes.find(type);
    std::string text;
    if (search == result_codes.end()) {
        text = result_codes.at(code::bad_error_code).text;
    } else {
        text = search->second.text;
    }
    return text;
}

int return_code(int result) {
    return 0 - result;
}

int api_result_bad_alloc_error() {
    return api_result(code::bad_allocation);
}

int api_result_unknown_error() {
    return api_result(code::unknown_error);
}

int api_result_not_supported() {
    return api_result(code::not_supported);
}

}  // namespace error
}  // namespace pixie
}  // namespace xia

std::ostringstream& operator<<(std::ostringstream& out, xia::pixie::error::error& error) {
    out << "result=" << error.result() << ", " << error.what();
    return out;
}

std::ostream& operator<<(std::ostream& out, xia::pixie::error::error& error) {
    error.output(out);
    return out;
}
