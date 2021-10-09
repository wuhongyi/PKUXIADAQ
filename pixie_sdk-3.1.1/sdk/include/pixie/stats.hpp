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

/** @file stats.hpp
 * @brief Defines functions and data structures related to hardware statistics.
 */

#ifndef PIXIE_STATS_H
#define PIXIE_STATS_H

#include <vector>

#include <pixie/param.hpp>

#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
namespace module {
class module;
}
/**
 * @brief Tools to handle statistics data
 */
namespace stats {
/**
 * @brief A data structure holding channel statistics read from the module.
 *
 * The channel and module stats cannot contain any containers while they
 * are used in the legacy API.
 */
struct channel {
    param::value_type fast_peaks_a;
    param::value_type fast_peaks_b;
    param::value_type live_time_a;
    param::value_type live_time_b;
    param::value_type chan_events_a;
    param::value_type chan_events_b;
    param::value_type runtime_a;
    param::value_type runtime_b;

    hw::config config;

    channel(const hw::config& config);
    channel();

    double input_count_rate() const;
    double output_count_rate() const;
    double live_time() const;
};

typedef std::vector<channel> channels;

/**
 * @brief A data structure holding module statistics read from the module.
 *
 * The channel and module stats cannot contain any containers while they
 * are used in the legacy API.
 */
struct module {
    param::value_type num_events_a;
    param::value_type num_events_b;
    param::value_type runtime_a;
    param::value_type runtime_b;

    module();

    uint64_t processed_events() const;
    double real_time() const;
};

typedef std::vector<module> modules;

/**
 * @brief A data structure aggregating module and channel statistics from a single module.
 */
struct stats {
    module mod;
    channels chans;

    stats(const hw::configs& configs);
};

void read(pixie::module::module& module_, stats& stats_);
}  // namespace stats
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_STATS_H
