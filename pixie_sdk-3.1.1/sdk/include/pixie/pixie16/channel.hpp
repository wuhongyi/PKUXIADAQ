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

/** @file channel.hpp
 * @brief Defines functions and data structures related to a Pixie-16 channel.
 */

#ifndef PIXIE_CHANNEL_H
#define PIXIE_CHANNEL_H

#include <functional>
#include <vector>

#include <pixie/error.hpp>
#include <pixie/param.hpp>

#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
namespace module {
class module;
}
/**
 * @brief Contains channel related functions and classes.
 */
namespace channel {
/**
 * @brief Defines Channel errors to throw.
 */
struct error : public pixie::error::error {
    typedef pixie::error::code code;
    explicit error(const int num, const int slot, const size_t channel, const code type,
                   const std::ostringstream& what);
    explicit error(const int num, const int slot, const size_t channel, const code type,
                   const std::string& what);
    explicit error(const int num, const int slot, const size_t channel, const code type,
                   const char* what);
    virtual void output(std::ostream& out);

private:
    std::string make_what(const int num, const int slot, const size_t channel, const char* what);
};

struct channel;

/**
 * @brief Defines a range as a list of channels for an operation.
 */
typedef std::vector<size_t> range;

/**
 * @brief Sets the range of all channels to [first, first + size).
 */
void range_set(range& range_, size_t first = 0);

/**
 * @brief A data structure holding the baselines obtained from the module.
 */
struct baseline {
    static const size_t max_num = 3640;

    typedef std::pair<double, double> value;
    typedef std::array<value, max_num> values;
    typedef std::vector<values> channels_values;

    module::module& module;
    range& channels;
    channels_values bl_values;

    /*
     * last found cut.
     */
    param::values cuts;

    /**
     * @brief Default constructor
     * @param module The module object to obtain baselines from.
     * @param channels The channels range is a selection channels in any order.
     */
    baseline(module::module& module, range& channels);

    void find_cut(size_t num = max_num);
    void compute_cut(size_t num);

    /**
     * @brief Get the channel values for the range.
     *
     * The indexes of the value's track the channels at the same index in the
     * channel range. For example:
     *
     * | Index | Channels | Values |
     * |---|---|---|
     * | 0 | 7 | `7 * chan_block_size + timebase` |
     * | 1 | 2 | `2 * chan_block_size + timebase` |
     * | 2 | 5 | `5 * chan_block_size + timebase` |
     * | 3 | 7 | `7 * chan_block_size + timebase` |
     *
     * Notes:
     *  - the Index column is the index for the channel range and channel
     *    values
     *  - the Values column is calculation used to find the channel's
     *    values in the baseline data read from the DSP
     *  - `chan_block_size` is the number of words of baseline per channel
     */
    void get(channels_values& chan_values, bool run = true);

    double time(hw::word time_word0, hw::word time_word1);
};

/**
 * @brief A Channel corresponds to a single input on the module's front panel.
 */
struct channel {

    /**
     * Number of this channel.
     */
    size_t number;

    /**
     * The hardware configuration for the channel.
     */
    hw::config config;

    /**
     * The module this channel is part of.
     */
    std::reference_wrapper<module::module> module;

    /**
     * Channel variables.
     */
    param::channel_variables vars;

    /**
     * @brief Default constructor
     * @param[in] module A channel must be part of a module.
     */
    channel(module::module& module);
    ~channel() = default;

    channel& operator=(const channel& c);

    /**
     * @brief Reads an ADC trace from the channel.
     * @param[out] buffer A data buffer with a length given by size.
     * @param[in] size The length of the data buffer for holding the trace.
     */
    void read_adc(hw::adc_word* buffer, size_t size);

    /**
     * @brief Reads a histogram from the channel.
     * @param[out] values Pointer to a data block to store the data.
     * @param[in] size The number of histogram values to read.
     */
    void read_histogram(hw::word_ptr values, const size_t size);
    /**
     * @brief Overloaded function used to pass in a word vector instead of a pointer.
     * @param[in,out] values A word vector whose length determines the number of histogram words.
     */
    void read_histogram(hw::words& values);

    /**
     * @brief Updated fifo settings based on the trace delay.
     * @param trace_delay The trace delay in units of samples.
     */
    void update_fifo(param::value_type trace_delay);

    double trigger_risetime();
    void trigger_risetime(double value);

    double trigger_flattop();
    void trigger_flattop(double value);

    double trigger_threshold();
    void trigger_threshold(double value);

    double energy_risetime();
    double energy_flattop();
    void energy_risetime_flattop(param::channel_param par, double value);

    double tau();
    void tau(double value);

    double trace_length();
    void trace_length(double value);

    double trace_delay();
    void trace_delay(double value);

    double voffset();
    void voffset(double value);

    double xdt();
    void xdt(double value);

    double baseline_percent();
    void baseline_percent(double value);

    double emin();
    void emin(double value);

    double binfactor();
    void binfactor(double value);

    double baseline_average();
    void baseline_average(double value);

    double csra();
    void csra(double value);

    double csrb();
    void csrb(double value);

    double bl_cut();
    void bl_cut(double value);

    double integrator();
    void integrator(double value);

    double fast_trig_backlen();
    void fast_trig_backlen(double value);

    double cfd_delay();
    void cfd_delay(double value);

    double cfd_scale();
    void cfd_scale(double value);

    double cfd_thresh();
    void cfd_thresh(double value);

    double qdc_len(param::channel_param par);
    void qdc_len(param::channel_param par, double value);

    double ext_trig_stretch();
    void ext_trig_stretch(double value);

    double veto_stretch();
    void veto_stretch(double value);

    double multiplicity_mask_l();
    void multiplicity_mask_l(double value);

    double multiplicity_mask_h();
    void multiplicity_mask_h(double value);

    double extern_delay_len();
    void extern_delay_len(double value);

    double ftrig_out_delay();
    void ftrig_out_delay(double value);

    double chan_trig_stretch();
    void chan_trig_stretch(double value);
};

/**
 * @brief Defines a type for a vector of channel objects
 */
typedef std::vector<channel> channels;
}  // namespace channel
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_CHANNEL_H
