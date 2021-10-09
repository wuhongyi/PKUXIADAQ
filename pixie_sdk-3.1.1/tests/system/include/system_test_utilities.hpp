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

/** @file system_test_utilities.hpp
 * @brief Defines commonly used functions for system tests.
 */

#ifndef PIXIE_SDK_SYSTEM_TEST_UTILITIES_HPP
#define PIXIE_SDK_SYSTEM_TEST_UTILITIES_HPP

#include <chrono>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

enum class DATA_PATTERN {
    HI_LO,
    FLIP_FLOP,
    RAMP_UP,
    RAMP_DOWN,
    CONSTANT,
    EVEN_BITS,
    ODD_BITS,
    ZERO
};

enum class DATA_IO { WRITE = 0, READ = 1 };

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

std::vector<unsigned int> prepare_data_to_write(const DATA_PATTERN& data_pattern,
                                                const unsigned int& size);

bool verify_api_return_value(const int& val, const std::string& func_name,
                             const bool& print_success_msg = true);

int verify_data_read(const unsigned int* expected, const unsigned int* returned,
                     const int& module_number, const unsigned int& size);

bool verify_power_of_two(const unsigned int& value);

#endif  //PIXIE_SDK_SYSTEM_TEST_UTILITIES_HPP
