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

#include <pixie/error.hpp>
#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
namespace hw {
struct module_fixture_rec {
    const std::string label;
    module_fixture fixture_id;
};

static const std::vector<module_fixture_rec> fixtures = {
    {"mainboard", module_fixture::mainboard}, {"DB01", module_fixture::DB01},
    {"DB02", module_fixture::DB02},           {"DB04", module_fixture::DB04},
    {"DB06", module_fixture::DB06},           {"DB07", module_fixture::DB07}};

config::config(size_t histogram_length, size_t adc_trace_length, size_t num_baselines,
               int adc_bits_, int adc_msps_, int adc_clk_div_, int fpga_clk_mhz_)
    : index(-1), fixture(module_fixture::mainboard), adc_bits(adc_bits_), adc_msps(adc_msps_),
      adc_clk_div(adc_clk_div_), fpga_clk_mhz(fpga_clk_mhz_),
      max_histogram_length(histogram_length), max_adc_trace_length(adc_trace_length),
      max_num_baselines(num_baselines) {}

config::config() {
    clear();
}

bool config::operator==(const config& cfg) {
    return adc_bits == cfg.adc_bits && adc_msps == cfg.adc_msps && adc_clk_div == cfg.adc_clk_div &&
           fpga_clk_mhz == cfg.fpga_clk_mhz;
}

bool config::operator!=(const config& cfg) {
    return !(*this == cfg);
}

void config::clear() {
    fixture = module_fixture::mainboard;
    max_histogram_length = hw::large_histogram_length;
    max_adc_trace_length = hw::max_adc_trace_length;
    max_num_baselines = hw::max_large_num_baselines;
    adc_bits = 0;
    adc_msps = 0;
    adc_clk_div = 0;
    fpga_clk_mhz = 0;
}

void config::report(std::ostream& out) const {
    out << "DSP var offset : " << index << std::endl
        << "ADC Bits       : " << adc_bits << std::endl
        << "ADC Clock      : " << adc_msps << " MHz" << std::endl
        << "FPGA Clock     : " << fpga_clk_mhz << " MHz" << std::endl
        << "MCA max length : " << max_histogram_length << " words" << std::endl
        << "ADC max trace  : " << max_adc_trace_length << " words" << std::endl;
}

module_fixture get_module_fixture(const std::string label) {
    for (auto& fixture : fixtures) {
        if (fixture.label == label) {
            return fixture.fixture_id;
        }
    }
    throw error(error::code::module_invalid_operation, "invalid fixture label: " + label);
}

std::string get_module_fixture_label(const module_fixture fixture_id) {
    for (auto& fixture : fixtures) {
        if (fixture.fixture_id == fixture_id) {
            return fixture.label;
        }
    }
    throw error(error::code::module_invalid_operation,
                "invalid fixture id: " + std::to_string(static_cast<int>(fixture_id)));
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
