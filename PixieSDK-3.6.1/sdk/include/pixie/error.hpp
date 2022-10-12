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

/** @file error.hpp
 * @brief Defines custom error codes used throughout the SDK.
 */

#ifndef PIXIE_ERROR_H
#define PIXIE_ERROR_H

#include <iostream>
#include <sstream>
#include <stdexcept>

/**
 * @brief The top level XIA namespace
 */
namespace xia {
/**
 * @brief Namespace for the Pixie product lines.
 */
namespace pixie {
/**
 * @brief Namespace for error handling in the PixieSDK.
 */
namespace error {
/**
 * @brief Defines the error codes that we use throughout the software.
 *
 * Each code maps to an API code and a description text. The codes and
 * descriptions may shift over time. Users should access the associated
 * information using the associated lookup functions.
 *
 * @note Order of entries in this enum must match with the definitions in
 * `module_params`.
 *
 * @see error::api_result
 * @see error::api_result_text
 */
enum struct code {
    /**
     * @brief 0
     */
    success = 0,
    /*
     * Crate
     */
    /**
     * @brief 100
     */
    crate_already_open,
    /**
     * @brief 101
     */
    crate_not_ready,
    /**
     * @brief 102
     */
    crate_invalid_param,
    /*
     * Module
     */
    /**
     * @brief 200
     */
    module_number_invalid,
    /**
     * @brief 201
     */
    module_total_invalid,
    /**
     * @brief 202
     */
    module_already_open,
    /**
     * @brief 203
     */
    module_close_failure,
    /**
     * @brief 204
     */
    module_offline,
    /**
     * @brief 205
     */
    module_info_failure,
    /**
     * @brief 206
     */
    module_invalid_operation,
    /**
     * @brief 207
     */
    module_invalid_firmware,
    /**
     * @brief 208
     */
    module_initialize_failure,
    /**
     * @brief 209
     */
    module_invalid_param,
    /**
     * @brief 210
     */
    module_invalid_var,
    /**
     * @brief 211
     */
    module_param_disabled,
    /**
     * @brief 212
     */
    module_param_readonly,
    /**
     * @brief 213
     */
    module_param_writeonly,
    /**
     * @brief 214
     */
    module_task_timeout,
    /**
     * @brief 215
     */
    module_invalid_slot,
    /**
     * @brief 216
     */
    module_not_found,
    /**
     * @brief 217
     */
    module_test_invalid,
    /*
     * Channel
     */
    /**
     * @brief 300
     */
    channel_number_invalid,
    /**
     * @brief 301
     */
    channel_invalid_param,
    /**
     * @brief 302
     */
    channel_invalid_var,
    /**
     * @brief 303
     */
    channel_invalid_index,
    /**
     * @brief 304
     */
    channel_param_disabled,
    /**
     * @brief 305
     */
    channel_param_readonly,
    /**
     * @brief 306
     */
    channel_param_writeonly,
    /*
     * Data
     */
    /**
     * @brief 400
     */
    invalid_header_length,
    /**
     * @brief 401
     */
    invalid_revision,
    /**
     * @brief 402
     */
    invalid_frequency,
    /**
     * @brief 403
     */
    invalid_element,
    /**
     * @brief 404
     */
    invalid_buffer_length,
    /**
     * @brief 405
     */
    invalid_event_length,
    /**
     * @brief 406
     */
    invalid_buffer,
    /**
     * @brief 407
     */
    invalid_cfd_time,
    /**
     * @brief 408
     */
    invalid_slot_id,
    /*
     * Device
     */
    /**
     * @brief 500
     */
    device_load_failure,
    /**
     * @brief 501
     */
    device_boot_failure,
    /**
     * @brief 502
     */
    device_initialize_failure,
    /**
     * @brief 503
     */
    device_copy_failure,
    /**
     * @brief 504
     */
    device_image_failure,
    /**
     * @brief 505
     */
    device_hw_failure,
    /**
     * @brief 506
     */
    device_dma_failure,
    /**
     * @brief 507
     */
    device_dma_busy,
    /**
     * @brief 508
     */
    device_fifo_failure,
    /**
     * @brief 509
     */
    device_eeprom_failure,
    /**
     * @brief 510
     */
    device_eeprom_bad_type,
    /**
     * @brief 511
     */
    device_eeprom_not_found,
    /*
     * Configuration
     */
    /**
     * @brief 600
     */
    config_invalid_param,
    /**
     * @brief 601
     */
    config_param_not_found,
    /**
     * @brief 602
     */
    config_json_error,
    /*
     * File handling
     */
    /**
     * @brief 700
     */
    file_not_found,
    /**
     * @brief 701
     */
    file_open_failure,
    /**
     * @brief 702
     */
    file_read_failure,
    /**
     * @brief 703
     */
    file_size_invalid,
    /**
     * @brief 704
     */
    file_create_failure,
    /*
     * System
     */
    /**
     * @brief 800
     */
    no_memory,
    /**
     * @brief 801
     */
    slot_map_invalid,
    /**
     * @brief 802
     */
    invalid_value,
    /**
     * @brief 803
     */
    not_supported,
    /**
     * @brief 804
     */
    buffer_pool_empty,
    /**
     * @brief 805
     */
    buffer_pool_not_empty,
    /**
     * @brief 806
     */
    buffer_pool_busy,
    /**
     * @brief 807
     */
    buffer_pool_not_enough,
    /*
     * Catch all
     */
    /**
     * @brief 900
     */
    unknown_error,
    /**
     * @brief 901
     */
    internal_failure,
    /**
     * @brief 902
     */
    bad_allocation,
    /**
     * @brief 990
     */
    bad_error_code,
    /**
     * @brief Defined for use in internal checks.
     */
    last
};

/**
 * @brief All PixieSDK exceptions need to be based on this error.
 */
struct error : public std::runtime_error {
    typedef pixie::error::code code;
    const code type;

    explicit error(const code type, const std::ostringstream& what);
    explicit error(const code type, const std::string& what);
    explicit error(const code type, const char* what);
    virtual ~error() = default;

    virtual void output(std::ostream& out);

    int result() const;
    std::string result_text() const;

    int return_code() const;
};

/**
 * @brief Internal test: check that error code definition are consistent.
 */
bool check_code_match();

/**
 * @brief Get the API code for an error code.
 * @param type The error code to lookup.
 * @return The API code associated with the error code.
 */
int api_result(enum code type);
/**
 * @brief Get the description text for an error code.
 * @param type The error code to lookup.
 * @return The description text string associated with the error code.
 */
std::string api_result_text(enum code type);
/**
 * @brief Get the description text for a returned error code.
 * @param error_code The error code to lookup. Can be negative or positive.
 * @return The description text string associated with the error code.
 */
std::string api_result_text(int error_code);

/**
 * @brief Turn the API result into a C style result code.
 * @param[in] result The API result to convert.
 * @return The API code as a negative number.
 */
int return_code(int result);

/**
 * @brief Bad allocation error helper.
 */
int api_result_bad_alloc_error();

/**
 * @brief Unknown error helper.
 */
int api_result_unknown_error();

/**
 * @brief Not supported helper.
 */
int api_result_not_supported();

/**
 * @brief Bad allocation error helper.
 */
int return_code_bad_alloc_error();

/**
 * @brief Unknown error helper.
 */
int return_code_unknown_error();

/**
 * @brief Not supported helper.
 */
int return_code_not_supported();
}  // namespace error
}  // namespace pixie
}  // namespace xia

std::ostringstream& operator<<(std::ostringstream& out, xia::pixie::error::error& error);
std::ostream& operator<<(std::ostream& out, xia::pixie::error::error& error);

#endif  // PIXIE_ERROR_H
