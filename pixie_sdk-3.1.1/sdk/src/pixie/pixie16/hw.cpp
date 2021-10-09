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

/** @file hw.cpp
 * @brief Implements hardware specific data for the Pixie-16 modules.
 */

#include <chrono>
#include <thread>

#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
namespace hw {
config::config(int adc_bits_, int adc_msps_, int adc_clk_div_, int fpga_clk_mhz_)
    : index(-1), adc_bits(adc_bits_), adc_msps(adc_msps_), adc_clk_div(adc_clk_div_),
      fpga_clk_mhz(fpga_clk_mhz_) {}

config::config() {
    clear();
}

bool config::operator==(const config& cfg) {
    return adc_bits == cfg.adc_bits || adc_msps == cfg.adc_msps || adc_clk_div == cfg.adc_clk_div ||
           fpga_clk_mhz == cfg.fpga_clk_mhz;
}

bool config::operator!=(const config& cfg) {
    return !(*this == cfg);
}

void config::clear() {
    adc_bits = 0;
    adc_msps = 0;
    adc_clk_div = 0;
    fpga_clk_mhz = 0;
}

void wait(size_t microseconds) {
    if (microseconds < 100) {
        volatile size_t count = 10 * microseconds;
        while (count > 0) {
            --count;
        }
    } else {
        std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
    }
}
};  // namespace hw
};  // namespace pixie
};  // namespace xia
