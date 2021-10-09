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

/** @file channel.cpp
 * @brief Implements functions and data structures related to a Pixie-16 channel.
 */

#include <cmath>
#include <numeric>

#include <pixie/os_compat.hpp>

#include <pixie/log.hpp>
#include <pixie/util.hpp>

#include <pixie/pixie16/channel.hpp>
#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/module.hpp>
#include <pixie/pixie16/run.hpp>

namespace xia {
namespace pixie {
namespace channel {
static std::string channel_label(const int num, const int slot, const size_t chan) {
    std::ostringstream oss;
    oss << "channel: num=" << num << ",slot=" << slot << ",chan=" << chan << ": ";
    return oss.str();
}

static std::string channel_label(const channel& chan) {
    module::module& mod = chan.module.get();
    return channel_label(mod.number, mod.slot, chan.number);
}

error::error(const int num, const int slot, const size_t channel, const code type,
             const std::ostringstream& what)
    : pixie::error::error(type, make_what(num, slot, channel, what.str().c_str())) {}

error::error(const int num, const int slot, const size_t channel, const code type,
             const std::string& what)
    : pixie::error::error(type, make_what(num, slot, channel, what.c_str())) {}

error::error(const int num, const int slot, const size_t channel, const code type, const char* what)
    : pixie::error::error(type, make_what(num, slot, channel, what)) {}

void error::output(std::ostream& out) {
    util::ostream_guard flags(out);
    out << std::setfill(' ') << "error: code=" << std::setw(2) << result() << ' ' << what();
}

std::string error::make_what(const int num, const int slot, const size_t channel,
                             const char* what_) {
    std::ostringstream oss;
    oss << channel_label(num, slot, channel) << what_;
    return oss.str();
}

void range_set(range& range_, size_t first) {
    std::iota(range_.begin(), range_.end(), first);
}

baseline::baseline(module::module& module, range& channels_)
    : module(module), channels(channels_), cuts(channels.size()) {
    if (channels.size() > module.num_channels) {
        throw module::error(module.number, module.slot, error::code::invalid_value,
                            "more channels in range than module has");
    }
}

void baseline::find_cut(size_t num) {
    log(log::info) << module::module_label(module) << "find bl cut: num=" << num;

    if (num > max_num) {
        throw module::error(module.number, module.slot, error::code::invalid_value,
                            "baseline values size exceeds max");
    }

    util::timepoint tp(true);

    param::values log2_bweight(channels.size());
    param::values current_bl_cut(channels.size());

    bl_values.resize(channels.size());

    for (auto chan : channels) {
        log2_bweight[chan] = module.read_var(param::channel_var::Log2Bweight, chan, 0, false);
        current_bl_cut[chan] = module.read_var(param::channel_var::BLcut, chan, 0, false);
        module.write_var(param::channel_var::Log2Bweight, 0, chan);
        module.write_var(param::channel_var::BLcut, 0, chan);
    }

    try {
        compute_cut(num);
        for (auto chan : channels) {
            module.write_var(param::channel_var::BLcut, cuts[chan], chan);
        }
        compute_cut(num);
        for (auto chan : channels) {
            module.write_var(param::channel_var::BLcut, cuts[chan], chan);
            log(log::info) << module::module_label(module) << "channel=" << chan
                           << " bl cut=" << cuts[chan];
        }
    } catch (...) {
        try {
            for (auto chan : channels) {
                module.write_var(param::channel_var::Log2Bweight, log2_bweight[chan], chan);
                module.write_var(param::channel_var::Log2Bweight, current_bl_cut[chan], chan);
            }
        } catch (...) {
            /* ignore nesting exceptions, keep the first */
        }
        throw;
    }

    for (auto chan : channels) {
        module.write_var(param::channel_var::Log2Bweight, log2_bweight[chan], chan);
        log(log::info) << module::module_label(module) << "channel=" << chan
                       << "find bl cut: cut=" << cuts[chan];
    }

    tp.end();

    log(log::info) << module::module_label(module) << "find bl cut: duration=" << tp;
}

double baseline::time(hw::word time_word0, hw::word time_word1) {
    double time;
    time = double(static_cast<uint64_t>(time_word0) << 32);
    time += double(time_word1);
    time *= 1.0e-6 / double(hw::system_clock_mhz);
    return time;
}

void baseline::get(baseline::channels_values& chan_values, bool run) {
    hw::memory::dsp dsp(module);
    hw::io_buffer buffer;

    log(log::debug) << module::module_label(module) << "baseline get: channels=" << channels.size()
                    << " chan-values=" << chan_values.size();

    /*
     * A baseline is returned in words as:
     *
     * [timestamp msw][timestamp lsw][baseline 0][baseline 2] ... [baseline N]
     *
     * where N is the maximum number of channels a module can have.
     */
    const size_t bl_block_len = 2 + module.num_channels;

    if (channels.size() == 0) {
        throw module::error(module.number, module.slot, error::code::invalid_value,
                            "no channels in the channel range");
    }

    if (chan_values[0].size() > (buffer.size() / bl_block_len)) {
        throw module::error(module.number, module.slot, error::code::invalid_value,
                            "channels values more than available baselines");
    }

    if (run) {
        hw::run::control(module, hw::run::control_task::get_baselines);
    }

    dsp.read(hw::memory::IO_BUFFER_ADDR, buffer);

    double starttime = time(buffer[0], buffer[1]);
    size_t offset = 2;

    for (size_t bl = 0; bl < max_num; ++bl, offset += bl_block_len) {
        double timestamp = time(buffer[offset], buffer[offset + 1]) - starttime;
        for (size_t c = 0;
             c < channels.size() && c < chan_values.size();
             ++c) {
            auto chan = channels[c];
            values& chan_vals = chan_values[c];
            if (bl < chan_vals.size()) {
                double baseline = util::ieee_float(buffer[offset + 2 + chan]);
                chan_vals[bl] = value(timestamp, baseline);
            }
        }
    }
}

void baseline::compute_cut(size_t num) {
    for (size_t count = 0; count < 10; ++count) {
        get(bl_values);
        for (auto chan : channels) {
            double sdev = 0.0;
            size_t sdev_count = 0;
            cuts[chan] = 0;
            for (size_t bl = 0; bl < (num - 1); ++bl) {
                double val =
                    (std::fabs(bl_values[chan][bl].second - bl_values[chan][bl + 1].second));
                if (val != 0) {
                    if (val < (10.0 * bl_values[chan][bl].second) &&
                        val < (10.0 * bl_values[chan][bl + 1].second)) {
                        /*
                         * @todo This peice of logic does not make sense because
                         * `cut` is set to 0 and not touched. It is a form of the
                         * code from before but it needs to be checked.
                         */
                        if (cuts[chan] == 0 || val < cuts[chan]) {
                            sdev += val;
                            ++sdev_count;
                        }
                    }
                }
            }
            if (sdev_count > 0) {
                const double sqrpi = std::sqrt(M_PI_2);
                double bl_sigma = sdev * sqrpi / sdev_count;
                cuts[chan] = static_cast<param::value_type>(std::floor(8.0 * bl_sigma));
            } else {
                cuts[chan] = 0;
            }

            log(log::debug) << module::module_label(module) << " channel=" << chan
                            << "computed cut=" << cuts[chan];
        }
    }
}

channel::channel(module::module& module_) : number(-1), module(module_) {}

channel& channel::operator=(const channel& c) {
    number = c.number;
    module = c.module;
    std::copy(c.vars.begin(), c.vars.end(), std::back_inserter(vars));
    return *this;
}

void channel::read_adc(hw::adc_word* buffer, size_t size) {
    const hw::address addr =
        (hw::memory::IO_BUFFER_ADDR + (int(number) * (hw::max_adc_trace_length / 2)));

    hw::memory::dsp dsp(module);
    hw::adc_trace_buffer adc_trace;

    dsp.read(addr, adc_trace, size / 2);

    for (size_t w = 0; w < size / 2; ++w) {
        buffer[w * 2] = hw::adc_word(adc_trace[w] & 0xffff);
        buffer[w * 2 + 1] = hw::adc_word((adc_trace[w] >> 16) & 0xffff);
    }
}

void channel::read_histogram(hw::word_ptr values, const size_t size) {
    if (size != 0) {
        const hw::address addr =
            (hw::memory::HISTOGRAM_MEMORY + (int(number) * hw::max_histogram_length));
        hw::memory::mca mca(module);
        mca.read(addr, values, size);
    }
}

void channel::read_histogram(hw::words& values) {
    read_histogram(values.data(), values.size());
}

void channel::update_fifo(param::value_type trace_delay) {
    log(log::debug) << channel_label(*this) << "fifo update: trace-delay=" << trace_delay;

    module::module& mod = module.get();

    const param::value_type sfr_mask =
        1 << mod.read_var(param::module_var::SlowFilterRange, 0, false);
    const param::value_type ffr_mask =
        1 << mod.read_var(param::module_var::FastFilterRange, 0, false);
    const param::value_type fifo_length = mod.read_var(param::module_var::FIFOLength, 0, false);
    const param::value_type peak_sep = mod.read_var(param::channel_var::PeakSep, number, 0, false);

    param::value_type trigger_delay;
    param::value_type paf_length;

    trigger_delay = (peak_sep - 1) * sfr_mask;
    paf_length = (trigger_delay / ffr_mask) + trace_delay;

    if (paf_length > fifo_length) {
        paf_length = fifo_length - 1;
        trigger_delay = (paf_length - trace_delay) * ffr_mask;
    }

    mod.write_var(param::channel_var::TriggerDelay, trigger_delay, number);
    mod.write_var(param::channel_var::PAFlength, paf_length, number);
}

double channel::trigger_risetime() {
    module::module& mod = module.get();

    param::value_type ffr_mask = 1 << mod.read_var(param::module_var::FastFilterRange);
    double fast_length = mod.read_var(param::channel_var::FastLength, number);

    double result = (fast_length * ffr_mask) / config.fpga_clk_mhz;

    return result;
}

void channel::trigger_risetime(double value) {
    module::module& mod = module.get();

    param::value_type ffr_mask = 1 << mod.read_var(param::module_var::FastFilterRange, 0, false);
    param::value_type fast_gap = mod.read_var(param::channel_var::FastGap, number, 0, false);

    param::value_type fast_length =
        param::value_type(std::round((value * config.fpga_clk_mhz) / ffr_mask));

    if ((fast_length + fast_gap) > hw::limit::FASTFILTER_MAX_LEN) {
        fast_length = hw::limit::FASTFILTER_MAX_LEN - fast_gap;
    }
    if (fast_length < hw::limit::FASTLENGTH_MIN_LEN) {
        fast_length = hw::limit::FASTLENGTH_MIN_LEN;
        if ((fast_length + fast_gap) > hw::limit::FASTFILTER_MAX_LEN) {
            fast_gap = hw::limit::FASTFILTER_MAX_LEN - hw::limit::FASTLENGTH_MIN_LEN;
        }
    }

    mod.write_var(param::channel_var::FastLength, fast_length, number);
    mod.write_var(param::channel_var::FastGap, fast_gap, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::trigger_flattop() {
    module::module& mod = module.get();

    param::value_type ffr_mask = 1 << mod.read_var(param::module_var::FastFilterRange);
    double fast_gap = mod.read_var(param::channel_var::FastGap, number);

    double result = (fast_gap * ffr_mask) / config.fpga_clk_mhz;

    return result;
}

void channel::trigger_flattop(double value) {
    module::module& mod = module.get();

    param::value_type ffr_mask = 1 << mod.read_var(param::module_var::FastFilterRange, 0, false);
    param::value_type fast_length = mod.read_var(param::channel_var::FastLength, number, 0, false);

    param::value_type fast_gap =
        param::value_type(std::round((value * config.fpga_clk_mhz) / ffr_mask));

    if ((fast_length + fast_gap) > hw::limit::FASTFILTER_MAX_LEN) {
        fast_gap = hw::limit::FASTFILTER_MAX_LEN - fast_length;
    }

    mod.write_var(param::channel_var::FastGap, fast_gap, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::trigger_threshold() {
    module::module& mod = module.get();

    double fast_thresh = mod.read_var(param::channel_var::FastThresh, number);
    double fast_length = mod.read_var(param::channel_var::FastLength, number);

    double result = fast_thresh / (fast_length * double(config.adc_clk_div));

    return result;
}

void channel::trigger_threshold(double value) {
    module::module& mod = module.get();

    double fast_length = mod.read_var(param::channel_var::FastLength, number);

    param::value_type fast_thresh =
        static_cast<param::value_type>(value * fast_length * config.adc_clk_div);

    if (fast_thresh >= hw::limit::FAST_THRESHOLD_MAX) {
        double dbl_fast_thresh(fast_thresh);
        value = (double(hw::limit::FAST_THRESHOLD_MAX) / (dbl_fast_thresh - 0.5)) * dbl_fast_thresh;
        fast_thresh = static_cast<param::value_type>(value);
    }

    mod.write_var(param::channel_var::FastThresh, fast_thresh, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::energy_risetime() {
    module::module& mod = module.get();

    param::value_type sfr_mask = 1 << mod.read_var(param::module_var::SlowFilterRange);
    double slow_length = mod.read_var(param::channel_var::SlowLength, number);

    double result = (slow_length * sfr_mask) / config.fpga_clk_mhz;

    return result;
}

double channel::energy_flattop() {
    module::module& mod = module.get();

    param::value_type sfr_mask = 1 << mod.read_var(param::module_var::SlowFilterRange);
    double slow_gap = mod.read_var(param::channel_var::SlowGap, number);

    double result = (slow_gap * sfr_mask) / config.fpga_clk_mhz;

    return result;
}

void channel::energy_risetime_flattop(param::channel_param par, double value) {
    ///@todo Need range checking to prevent negative values.

    module::module& mod = module.get();

    param::value_type ffr_mask = 1 << mod.read_var(param::module_var::FastFilterRange, 0, false);
    param::value_type sfr = mod.read_var(param::module_var::SlowFilterRange, 0, false);
    param::value_type sfr_mask = 1 << sfr;
    param::value_type paf_length = mod.read_var(param::channel_var::PAFlength, number, 0, false);
    param::value_type trigger_delay =
        mod.read_var(param::channel_var::TriggerDelay, number, 0, false);

    param::value_type trace_delay = paf_length - (trigger_delay / ffr_mask);

    param::value_type slow_length;
    param::value_type slow_gap;

    if (par == param::channel_param::energy_risetime) {
        slow_length = param::value_type(std::round((value * config.fpga_clk_mhz) / sfr_mask));
        slow_gap = mod.read_var(param::channel_var::SlowGap, number, 0, false);
        if ((slow_length + slow_gap) > hw::limit::SLOWFILTER_MAX_LEN) {
            slow_length = hw::limit::SLOWFILTER_MAX_LEN - slow_gap;
        }
        if (slow_length < hw::limit::SLOWLENGTH_MIN_LEN) {
            slow_length = hw::limit::SLOWLENGTH_MIN_LEN;
            if ((slow_length + slow_gap) > hw::limit::SLOWFILTER_MAX_LEN) {
                slow_gap = hw::limit::SLOWFILTER_MAX_LEN - hw::limit::SLOWLENGTH_MIN_LEN;
            }
        }
    } else if (par == param::channel_param::energy_flattop) {
        slow_gap = param::value_type(std::round((value * config.fpga_clk_mhz) / sfr_mask));
        slow_length = mod.read_var(param::channel_var::SlowLength, number, 0, false);
        if ((slow_length + slow_gap) > hw::limit::SLOWFILTER_MAX_LEN) {
            slow_gap = hw::limit::SLOWFILTER_MAX_LEN - slow_length;
        }
        if (slow_gap < hw::limit::SLOWGAP_MIN_LEN) {
            slow_gap = hw::limit::SLOWGAP_MIN_LEN;
            if ((slow_length + slow_gap) > hw::limit::SLOWFILTER_MAX_LEN) {
                slow_length = hw::limit::SLOWFILTER_MAX_LEN - hw::limit::SLOWGAP_MIN_LEN;
            }
        }
    } else {
        throw error(mod.number, mod.slot, number, error::code::channel_invalid_param,
                    "param not energy risetime or flattop");
    }

    mod.write_var(param::channel_var::SlowLength, slow_length, number);
    mod.write_var(param::channel_var::SlowGap, slow_gap, number);

    param::value_type peak_sep = slow_length + slow_gap;
    param::value_type peak_sample;

    switch (sfr) {
        case 1:
            peak_sample = peak_sep - 3;
            break;
        case 5:
            peak_sample = peak_sep;
            break;
        case 4:
        case 6:
            peak_sample = peak_sep - 1;
            break;
        case 2:
        case 3:
        default:
            peak_sample = peak_sep - 2;
            break;
    }

    mod.write_var(param::channel_var::PeakSample, peak_sample, number);
    mod.write_var(param::channel_var::PeakSep, peak_sep, number);

    update_fifo(trace_delay);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::tau() {
    module::module& mod = module.get();
    util::ieee_float preamp_tau = mod.read_var(param::channel_var::PreampTau, number);
    return preamp_tau;
}

void channel::tau(double value) {
    module::module& mod = module.get();

    param::value_type preamp_tau = util::ieee_float(value);

    mod.write_var(param::channel_var::PreampTau, preamp_tau, number);

    range chans = {number};
    baseline bl(mod, chans);
    bl.find_cut();
}

double channel::trace_length() {
    module::module& mod = module.get();

    double trace_len = mod.read_var(param::channel_var::TraceLength, number);
    param::value_type ffr_mask = 1 << mod.read_var(param::module_var::FastFilterRange);

    double result = trace_len / (double(config.adc_msps) * ffr_mask);

    return result;
}

void channel::trace_length(double value) {
    ///@todo Need range checking to prevent negative values.

    module::module& mod = module.get();

    param::value_type ffr_mask = 1 << mod.read_var(param::module_var::FastFilterRange);
    param::value_type fifo_length = mod.read_var(param::module_var::FIFOLength);

    param::value_type trace_length = param::value_type(value * config.adc_msps / ffr_mask);

    /*
     * Adjust the length to suit the FPGA requirements
     */
    switch (config.adc_msps) {
        case 500:
            trace_length = (trace_length / 10) * 10;
            break;
        case 250:
        case 100:
            trace_length = (trace_length / 2) * 2;
            break;
        default:
            break;
    }

    if (trace_length > fifo_length) {
        trace_length = fifo_length;
    }

    mod.write_var(param::channel_var::TraceLength, trace_length, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::trace_delay() {
    module::module& mod = module.get();

    double paf_length = mod.read_var(param::channel_var::PAFlength, number);
    double trigger_delay = mod.read_var(param::channel_var::TriggerDelay, number);
    param::value_type fast_filter_range = mod.read_var(param::module_var::FastFilterRange);
    param::value_type ffr_mask = 1 << fast_filter_range;

    double result = (paf_length - (trigger_delay / ffr_mask)) / config.fpga_clk_mhz * ffr_mask;

    return result;
}

void channel::trace_delay(double value) {
    ///@todo Need range checking to prevent negative values.

    module::module& mod = module.get();

    param::value_type ffr_mask = 1 << mod.read_var(param::module_var::FastFilterRange);
    param::value_type trace_length = mod.read_var(param::channel_var::TraceLength, number);

    param::value_type trace_delay = param::value_type(value * config.fpga_clk_mhz / ffr_mask);

    if (trace_delay > trace_length) {
        trace_delay = trace_length / 2;
    }

    if (trace_delay > hw::limit::TRACEDELAY_MAX) {
        trace_delay = hw::limit::TRACEDELAY_MAX;
    }

    update_fifo(trace_delay);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::voffset() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::OffsetDAC, number);

    return hw::limit::DAC_VOLTAGE_RANGE * (value / 65536.0 - 0.5);
}

void channel::voffset(double value) {
    module::module& mod = module.get();

    auto offset_dac = param::value_type(65536 * (value / hw::limit::DAC_VOLTAGE_RANGE + 0.5));

    /*
     * 16-bit DAC, range check.
     */
    if (offset_dac > 65535) {
        offset_dac = 65535;
    }

    mod.write_var(param::channel_var::OffsetDAC, offset_dac, number);

    hw::run::control(mod, hw::run::control_task::set_dacs);
}

double channel::xdt() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::Xwait, number);

    double result = double(value) / hw::limit::DSP_CLOCK_MHZ;

    return result;
}

void channel::xdt(double value) {
    ///@todo Need range checking to prevent negative values.

    module::module& mod = module.get();

    param::value_type current_xwait = mod.read_var(param::channel_var::Xwait, number);

    param::value_type xwait = param::value_type(std::round(value * hw::limit::DSP_CLOCK_MHZ));

    double multiple;

    switch (config.adc_msps) {
        case 100:
        case 500:
            /*
         * For 100 MSPS RevB/C/D or RevF, or 500 MSPS RevF, xwait should be
         * multiples of 6
         */
            multiple = 6;
            break;
        default:
            /*
         * For 250 RevF, xwait should be multiples of 8
         */
            multiple = 8;
            break;
    }

    if (xwait < multiple) {
        xwait = param::value_type(multiple);
    }
    if (xwait > current_xwait) {
        xwait = param::value_type(std::ceil(double(xwait) / multiple) * multiple);
    } else {
        xwait = param::value_type(std::floor(double(xwait) / multiple) * multiple);
    }

    mod.write_var(param::channel_var::Xwait, xwait, number);
}

double channel::baseline_percent() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::BaselinePercent, number);

    double result = double(value);

    return result;
}

void channel::baseline_percent(double value) {
    ///@todo Need range checking to prevent negative values.

    module::module& mod = module.get();

    param::value_type bl_percent = param::value_type(value);

    if (bl_percent < 1) {
        bl_percent = 1;
    } else if (bl_percent > 99) {
        bl_percent = 99;
    }

    mod.write_var(param::channel_var::BaselinePercent, bl_percent, number);
}

double channel::emin() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::EnergyLow, number);

    double result = double(value);

    return result;
}

void channel::emin(double value) {
    ///@todo Need range checking to prevent negative values.

    module::module& mod = module.get();

    mod.write_var(param::channel_var::EnergyLow, param::value_type(value), number);
}

double channel::binfactor() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::Log2Ebin, number);

    double result = double(1ULL << 32) - double(value);

    return result;
}

void channel::binfactor(double value) {
    ///@todo Need range checking to prevent negative values.

    module::module& mod = module.get();

    param::value_type log2ebin = param::value_type(value);

    if (log2ebin < 1) {
        log2ebin = 1;
    } else if (log2ebin > 6) {
        log2ebin = 6;
    }

    log2ebin = param::value_type((1ULL << 32) - uint64_t(log2ebin));

    mod.write_var(param::channel_var::Log2Ebin, log2ebin, number);
}

double channel::baseline_average() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::Log2Bweight, number);

    double result;

    if (value == 0) {
        result = 0.0;
    } else {
        result = double(1ULL << 32) - double(value);
    }

    return result;
}

void channel::baseline_average(double value) {
    ///@todo Need range checking to prevent negative values.

    module::module& mod = module.get();

    param::value_type bl_avg = param::value_type(value);

    if (bl_avg > 16) {
        bl_avg = 16;
    }

    bl_avg = param::value_type((1ULL << 32) - uint64_t(bl_avg));

    mod.write_var(param::channel_var::Log2Bweight, bl_avg, number);
}

double channel::csra() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::ChanCSRa, number);

    double result = double(value);

    return result;
}

void channel::csra(double value) {
    ///@todo Need range checking to prevent negative values.

    module::module& mod = module.get();

    param::value_type current_csra = mod.read_var(param::channel_var::ChanCSRa, number);

    param::value_type csra = param::value_type(value);

    mod.write_var(param::channel_var::ChanCSRa, csra, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
    hw::run::control(mod, hw::run::control_task::set_dacs);

    if ((csra & (1 << hw::bit::CCSRA_ENARELAY)) !=
        (current_csra & (1 << hw::bit::CCSRA_ENARELAY))) {
        range chans = {number};
        baseline bl(mod, chans);
        bl.find_cut();
    }
}

double channel::csrb() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::ChanCSRb, number);

    double result = double(value);

    return result;
}

void channel::csrb(double value) {
    ///@todo Need range checking to prevent negative values.

    module::module& mod = module.get();

    mod.write_var(param::channel_var::ChanCSRb, param::value_type(value), number);
}

double channel::bl_cut() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::BLcut, number);

    double result = double(value);

    return result;
}

void channel::bl_cut(double value) {
    ///@todo Need range checking to prevent negative values.

    module::module& mod = module.get();

    mod.write_var(param::channel_var::BLcut, param::value_type(value), number);
}

double channel::integrator() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::Integrator, number);

    double result = double(value);

    return result;
}

void channel::integrator(double value) {
    ///@todo Need range checking to prevent negative values.
    ///@note This variable is disabled. Do we still need this functionality??
    module::module& mod = module.get();

    if (value > 7) {
        value = 7;
    }

    mod.write_var(param::channel_var::Integrator, param::value_type(value), number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::fast_trig_backlen() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::FastTrigBackLen, number);

    double result = double(value) / config.fpga_clk_mhz;

    return result;
}

void channel::fast_trig_backlen(double value) {
    ///@todo Need range checking to prevent negative values.

    module::module& mod = module.get();

    param::value_type fast_trig_blen = param::value_type(std::round(value * config.fpga_clk_mhz));

    param::value_type fast_trig_blen_min;
    switch (config.adc_msps) {
        case 100:
        case 500:
            fast_trig_blen_min = hw::limit::FASTTRIGBACKLEN_MIN_100MHZFIPCLK;
            break;
        default:
            fast_trig_blen_min = hw::limit::FASTTRIGBACKLEN_MIN_125MHZFIPCLK;
            break;
    }

    if (fast_trig_blen < fast_trig_blen_min) {
        fast_trig_blen = fast_trig_blen_min;
    } else if (fast_trig_blen > hw::limit::FASTTRIGBACKLEN_MAX) {
        fast_trig_blen = hw::limit::FASTTRIGBACKLEN_MAX;
    }

    mod.write_var(param::channel_var::FastTrigBackLen, fast_trig_blen, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::cfd_delay() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::CFDDelay, number);

    double result = double(value) / config.fpga_clk_mhz;

    return result;
}

void channel::cfd_delay(double value) {
    ///@todo Need range checking to prevent negative values.

    module::module& mod = module.get();

    param::value_type cfddelay = param::value_type(std::round(value * config.fpga_clk_mhz));

    if (cfddelay < hw::limit::CFDDELAY_MIN) {
        cfddelay = hw::limit::CFDDELAY_MIN;
    }
    if (cfddelay > hw::limit::CFDDELAY_MAX) {
        cfddelay = hw::limit::CFDDELAY_MAX;
    }

    mod.write_var(param::channel_var::CFDDelay, cfddelay, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::cfd_scale() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::CFDScale, number);

    auto result = double(value);

    return result;
}

void channel::cfd_scale(double value) {
    ///@todo Need range checking to prevent negative values.
    module::module& mod = module.get();

    param::value_type cfdscale = param::value_type(value);

    if (cfdscale > hw::limit::CFDSCALE_MAX) {
        cfdscale = hw::limit::CFDSCALE_MAX;
    }

    mod.write_var(param::channel_var::CFDScale, cfdscale, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::cfd_thresh() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::CFDThresh, number);

    double result = double(value);

    return result;
}

void channel::cfd_thresh(double value) {
    module::module& mod = module.get();

    param::value_type cfdthresh = param::value_type(value);

    if (cfdthresh < hw::limit::CFDTHRESH_MIN || cfdthresh > hw::limit::CFDTHRESH_MAX) {
        std::ostringstream oss;
        oss << ": out of range CFDThresh: " << cfdthresh;
        throw error(mod.number, mod.slot, number, error::code::channel_invalid_param, oss.str());
    }

    mod.write_var(param::channel_var::CFDThresh, cfdthresh, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::qdc_len(param::channel_param par) {
    module::module& mod = module.get();

    param::channel_var var;

    switch (par) {
        case param::channel_param::qdclen0:
            var = param::channel_var::QDCLen0;
            break;
        case param::channel_param::qdclen1:
            var = param::channel_var::QDCLen1;
            break;
        case param::channel_param::qdclen2:
            var = param::channel_var::QDCLen2;
            break;
        case param::channel_param::qdclen3:
            var = param::channel_var::QDCLen3;
            break;
        case param::channel_param::qdclen4:
            var = param::channel_var::QDCLen4;
            break;
        case param::channel_param::qdclen5:
            var = param::channel_var::QDCLen5;
            break;
        case param::channel_param::qdclen6:
            var = param::channel_var::QDCLen6;
            break;
        case param::channel_param::qdclen7:
            var = param::channel_var::QDCLen7;
            break;
        default:
            throw error(mod.number, mod.slot, number, error::code::channel_invalid_param,
                        "invalid QDCLen param offset");
    }

    double divider = config.adc_msps;
    if (config.adc_msps == 500) {
        divider /= 5;
    }

    param::value_type value = mod.read_var(var, number);

    double result = double(value) / divider;

    return result;
}

void channel::qdc_len(param::channel_param par, double value) {
    ///@todo Need range checking to prevent negative values.

    module::module& mod = module.get();

    param::channel_var var;

    switch (par) {
        case param::channel_param::qdclen0:
            var = param::channel_var::QDCLen0;
            break;
        case param::channel_param::qdclen1:
            var = param::channel_var::QDCLen1;
            break;
        case param::channel_param::qdclen2:
            var = param::channel_var::QDCLen2;
            break;
        case param::channel_param::qdclen3:
            var = param::channel_var::QDCLen3;
            break;
        case param::channel_param::qdclen4:
            var = param::channel_var::QDCLen4;
            break;
        case param::channel_param::qdclen5:
            var = param::channel_var::QDCLen5;
            break;
        case param::channel_param::qdclen6:
            var = param::channel_var::QDCLen6;
            break;
        case param::channel_param::qdclen7:
            var = param::channel_var::QDCLen7;
            break;
        default:
            throw error(mod.number, mod.slot, number, error::code::channel_invalid_param,
                        "invalid QDCLen param offset");
    }

    double multiplier = config.adc_msps;
    if (config.adc_msps == 500) {
        multiplier /= 5;
    }

    param::value_type qdclen = param::value_type(std::round(value * multiplier));

    if (qdclen < hw::limit::QDCLEN_MIN) {
        qdclen = hw::limit::QDCLEN_MIN;
    }
    if (qdclen > hw::limit::QDCLEN_MAX) {
        qdclen = hw::limit::QDCLEN_MAX;
    }

    mod.write_var(var, qdclen, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::ext_trig_stretch() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::ExtTrigStretch, number);

    double result = double(value) / config.fpga_clk_mhz;

    return result;
}

void channel::ext_trig_stretch(double value) {
    ///@todo Need range checking to prevent negative values.
    ///@note this logic is identical to channel::chan_trig_stretch and channel::veto_stretch.

    module::module& mod = module.get();

    param::value_type exttrigstretch = param::value_type(std::round(value * config.fpga_clk_mhz));

    if (exttrigstretch < hw::limit::EXTTRIGSTRETCH_MIN) {
        exttrigstretch = hw::limit::EXTTRIGSTRETCH_MIN;
    }
    if (exttrigstretch > hw::limit::EXTTRIGSTRETCH_MAX) {
        exttrigstretch = hw::limit::EXTTRIGSTRETCH_MAX;
    }

    mod.write_var(param::channel_var::ExtTrigStretch, exttrigstretch, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::veto_stretch() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::VetoStretch, number);

    double result = double(value) / config.fpga_clk_mhz;

    return result;
}

void channel::veto_stretch(double value) {
    ///@todo Need range checking to prevent negative values.
    ///@note this logic is identical to channel::chan_trig_stretch and channel::ext_trig_stretch.
    module::module& mod = module.get();

    param::value_type vetostretch = param::value_type(std::round(value * config.fpga_clk_mhz));

    if (vetostretch < hw::limit::VETOSTRETCH_MIN) {
        vetostretch = hw::limit::VETOSTRETCH_MIN;
    }
    if (vetostretch > hw::limit::VETOSTRETCH_MAX) {
        vetostretch = hw::limit::VETOSTRETCH_MAX;
    }

    mod.write_var(param::channel_var::VetoStretch, vetostretch, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::multiplicity_mask_l() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::MultiplicityMaskL, number);

    double result = double(value);

    return result;
}

void channel::multiplicity_mask_l(double value) {
    ///@todo Need range checking to prevent negative values.
    module::module& mod = module.get();

    param::value_type multiplicitymaskl = param::value_type(value);

    mod.write_var(param::channel_var::MultiplicityMaskL, multiplicitymaskl, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::multiplicity_mask_h() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::MultiplicityMaskH, number);

    double result = double(value);

    return result;
}

void channel::multiplicity_mask_h(double value) {
    ///@todo Need range checking to prevent negative values.
    module::module& mod = module.get();

    param::value_type multiplicitymaskh = param::value_type(value);

    mod.write_var(param::channel_var::MultiplicityMaskH, multiplicitymaskh, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::extern_delay_len() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::ExternDelayLen, number);

    double result = double(value) / config.fpga_clk_mhz;

    return result;
}

void channel::extern_delay_len(double value) {
    ///@todo Need range checking to prevent negative values. For now negative values get set to max.
    ///@note This function's logic is identical to channel::ftrig_out_delay, maybe we can simplify?
    module::module& mod = module.get();

    param::value_type externdelaylen = param::value_type(std::round(value * config.fpga_clk_mhz));
    param::value_type externdelaylen_max;

    switch (mod.revision) {
        case hw::rev_B:
        case hw::rev_C:
        case hw::rev_D:
            externdelaylen_max = hw::limit::EXTDELAYLEN_MAX_REVBCD;
            break;
        case hw::rev_F:
        case hw::rev_H:
            externdelaylen_max = hw::limit::EXTDELAYLEN_MAX_REVF;
            break;
        default:
            throw error(mod.number, mod.slot, number, error::code::not_supported,
                        "ExternDelayLen not supported on this revision");
    }

#if EXTDELAYLEN_MIN > 0
    if (externdelaylen < hw::limit::EXTDELAYLEN_MIN) {
        externdelaylen = hw::limit::EXTDELAYLEN_MIN;
    }
#endif
    if (externdelaylen > externdelaylen_max) {
        externdelaylen = externdelaylen_max;
    }

    mod.write_var(param::channel_var::ExternDelayLen, externdelaylen, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::ftrig_out_delay() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::FtrigoutDelay, number);

    double result = double(value) / config.fpga_clk_mhz;

    return result;
}

void channel::ftrig_out_delay(double value) {
    ///@todo Need range checking to prevent negative values. For now negative values get set to max.
    module::module& mod = module.get();

    param::value_type ftrigoutdelay = param::value_type(std::round(value * config.fpga_clk_mhz));
    param::value_type ftrigoutdelay_max;

    switch (mod.revision) {
        case hw::rev_B:
        case hw::rev_C:
        case hw::rev_D:
            ftrigoutdelay_max = hw::limit::EXTDELAYLEN_MAX_REVBCD;
            break;
        case hw::rev_F:
        case hw::rev_H:
            ftrigoutdelay_max = hw::limit::EXTDELAYLEN_MAX_REVF;
            break;
        default:
            throw error(mod.number, mod.slot, number, error::code::not_supported,
                        "FtrigoutDelay not supported on this revision");
    }

    if (ftrigoutdelay > ftrigoutdelay_max) {
        ftrigoutdelay = ftrigoutdelay_max;
    }

    mod.write_var(param::channel_var::FtrigoutDelay, ftrigoutdelay, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

double channel::chan_trig_stretch() {
    module::module& mod = module.get();

    param::value_type value = mod.read_var(param::channel_var::ChanTrigStretch, number);

    double result = double(value) / config.fpga_clk_mhz;

    return result;
}

void channel::chan_trig_stretch(double value) {
    ///@todo Need range checking here. Negative values get set to max.
    ///@note this logic is identical to channel::veto_stretch and channel::ext_trig_stretch.

    module::module& mod = module.get();

    param::value_type chantrigstretch = param::value_type(std::round(value * config.fpga_clk_mhz));

    if (chantrigstretch < hw::limit::CHANTRIGSTRETCH_MIN) {
        chantrigstretch = hw::limit::CHANTRIGSTRETCH_MIN;
    }
    if (chantrigstretch > hw::limit::CHANTRIGSTRETCH_MAX) {
        chantrigstretch = hw::limit::CHANTRIGSTRETCH_MAX;
    }

    mod.write_var(param::channel_var::ChanTrigStretch, chantrigstretch, number);

    hw::run::control(mod, hw::run::control_task::program_fippi);
}

}  // namespace channel
}  // namespace pixie
}  // namespace xia
