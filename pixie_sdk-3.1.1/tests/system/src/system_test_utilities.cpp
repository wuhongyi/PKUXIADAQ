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

/** @file system_test_utilities.cpp
 * @brief Implements functions that are useful across several system test codes.
 */

#include <iostream>

#include "system_test_utilities.hpp"

std::vector<unsigned int> prepare_data_to_write(const DATA_PATTERN& data_pattern,
                                                const unsigned int& size) {
    if (!verify_power_of_two(size))
        throw std::invalid_argument("Test data must have a size that's a power of 2!!");

    std::vector<unsigned int> data{};
    for (unsigned int entry = 0; entry < size; entry += 2) {
        switch (data_pattern) {
            case DATA_PATTERN::HI_LO:
                data.emplace_back(0xAAAA5555);
                data.emplace_back(0x5555AAAA);
                break;
            case DATA_PATTERN::FLIP_FLOP:
                data.emplace_back(0xA0500A05);
                data.emplace_back(0x50A0050A);
                break;
            case DATA_PATTERN::RAMP_UP:
                data.emplace_back(entry);
                data.emplace_back(entry + 1);
                break;
            case DATA_PATTERN::RAMP_DOWN:
                data.emplace_back(size - entry);
                data.emplace_back(size - entry - 1);
                break;
            case DATA_PATTERN::CONSTANT:
                data.emplace_back(0x50f750fa);
                data.emplace_back(0x50f750fa);
                break;
            case DATA_PATTERN::EVEN_BITS:
                data.emplace_back(0xA5A5A5A5);
                data.emplace_back(0xA5A5A5A5);
                break;
            case DATA_PATTERN::ODD_BITS:
                data.emplace_back(0x5A5A5A5A);
                data.emplace_back(0x5A5A5A5A);
                break;
            case DATA_PATTERN::ZERO:
                data.emplace_back(0);
                data.emplace_back(0);
                break;
            default:
                break;
        }
    }
    return data;
}

bool verify_api_return_value(const int& val, const std::string& func_name,
                             const bool& print_success_msg /*=true*/) {
    if (val < 0) {
        std::cout << LOG("ERROR") << func_name << " failed with Error Code " << val;
        return false;
    }
    if (print_success_msg)
        std::cout << LOG("INFO") << func_name << " finished successfully.";
    return true;
}

int verify_data_read(const unsigned int* expected, const unsigned int* returned,
                     const int& module_number, const unsigned int& size) {
    unsigned int error_count = 0;
    for (unsigned int idx = 0; idx < size; idx++) {
        if (expected[idx] != returned[idx]) {
            std::cout << LOG("ERROR") << " Data mismatch in module " << module_number
                      << ", rd_data=" << std::hex << returned[idx] << ", wr_data=" << expected[idx]
                      << ", position=" << std::dec << idx;
            error_count++;
        }
    }
    return error_count;
}

bool verify_power_of_two(const unsigned int& value) {
    if ((value != 0) && ((value & (value - 1)) == 0))
        return true;
    return false;
}
