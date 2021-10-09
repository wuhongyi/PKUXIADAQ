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

/** @file stats.cpp
 * @brief Implements functions and data structures related to hardware statistics.
 */

#include <algorithm>

#include <pixie/stats.hpp>

#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
namespace stats {
static uint64_t make_u64(const param::value_type high, const param::value_type low) {
    return (uint64_t(high) << 32) | uint64_t(low);
}

static double make_u64_double(const param::value_type high, const param::value_type low) {
    return double(make_u64(high, low));
}

channel::channel(const hw::config& config)
    : fast_peaks_a(0), fast_peaks_b(0), live_time_a(0), live_time_b(0), chan_events_a(0),
      chan_events_b(0), runtime_a(0), runtime_b(0), config(config) {}

channel::channel()
    : fast_peaks_a(0), fast_peaks_b(0), live_time_a(0), live_time_b(0), chan_events_a(0),
      chan_events_b(0), runtime_a(0), runtime_b(0) {}

double channel::input_count_rate() const {
    const double fast_peaks = make_u64_double(fast_peaks_a, fast_peaks_b);
    const double live_time = make_u64_double(live_time_a, live_time_b);
    if (live_time == 0.0) {
        return 0.0;
    }
    return fast_peaks / (live_time * config.adc_clk_div * (1.0e-6 / config.adc_msps));
}

double channel::output_count_rate() const {
    const double chan_events = make_u64_double(chan_events_a, chan_events_b);
    const double real_time = make_u64_double(runtime_a, runtime_b);
    if (real_time == 0.0) {
        return 0.0;
    }
    return chan_events / (real_time * (1.0e-6 / hw::system_clock_mhz));
}

double channel::live_time() const {
    const double live_time = make_u64_double(live_time_a, live_time_b);
    return live_time * config.adc_clk_div * (1.0e-6 / config.adc_msps);
}

module::module() : num_events_a(0), num_events_b(0) {}

uint64_t module::processed_events() const {
    return make_u64(num_events_a, num_events_b);
}

double module::real_time() const {
    return make_u64_double(runtime_a, runtime_b) * (1.0e-6 / hw::system_clock_mhz);
}

stats::stats(const hw::configs& configs) {
    chans.resize(configs.size());
    for (size_t channel = 0; channel < configs.size(); ++channel) {
        chans[channel].config = configs[channel];
    }
}

void read(pixie::module::module& module_, stats& stats_) {
    const param::module_var_descs& mod_descs = module_.module_var_descriptors;
    const param::channel_var_descs& chan_descs = module_.channel_var_descriptors;

    std::vector<hw::address> addrs = {
        param::get_descriptor(mod_descs, param::module_var::NumEventsA).address,
        param::get_descriptor(mod_descs, param::module_var::NumEventsB).address,
        param::get_descriptor(mod_descs, param::module_var::RunTimeA).address,
        param::get_descriptor(mod_descs, param::module_var::RunTimeB).address,
        param::get_descriptor(chan_descs, param::channel_var::FastPeaksA).address,
        param::get_descriptor(chan_descs, param::channel_var::FastPeaksB).address,
        param::get_descriptor(chan_descs, param::channel_var::LiveTimeA).address,
        param::get_descriptor(chan_descs, param::channel_var::LiveTimeB).address,
        param::get_descriptor(chan_descs, param::channel_var::ChanEventsA).address,
        param::get_descriptor(chan_descs, param::channel_var::ChanEventsB).address,
    };

    const hw::address addr_low = *std::min_element(addrs.begin(), addrs.end(),
                                                   [](auto& a, auto& b) { return a < b; });
    const hw::address addr_high = *std::max_element(addrs.begin(), addrs.end(),
                                                    [](auto& a, auto& b) { return a < b; });
    const size_t words = addr_high - addr_low + module_.num_channels + 1;

    hw::memory::dsp dsp(module_);
    hw::words vars(words);

    dsp.read(addr_low, vars);

    stats_.mod.num_events_a = vars[addrs[0] - addr_low];
    stats_.mod.num_events_b = vars[addrs[1] - addr_low];
    stats_.mod.runtime_a = vars[addrs[2] - addr_low];
    stats_.mod.runtime_b = vars[addrs[3] - addr_low];

    for (auto& channel : stats_.chans) {
        const auto index = channel.config.index;
        channel.fast_peaks_a = vars[addrs[4] + index - addr_low];
        channel.fast_peaks_b = vars[addrs[5] + index - addr_low];
        channel.live_time_a = vars[addrs[6] + index - addr_low];
        channel.live_time_b = vars[addrs[7] + index - addr_low];
        channel.chan_events_a = vars[addrs[8] + index - addr_low];
        channel.chan_events_b = vars[addrs[9] + index - addr_low];
        channel.runtime_a = stats_.mod.runtime_a;
        channel.runtime_b = stats_.mod.runtime_b;
    }
}
}  // namespace stats
}  // namespace pixie
}  // namespace xia
