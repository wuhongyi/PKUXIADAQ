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

/** @file param.cpp
 * @brief Implements functions and data structures related to handling parameter sets
 */

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include <pixie/error.hpp>
#include <pixie/fw.hpp>
#include <pixie/log.hpp>
#include <pixie/param.hpp>
#include <pixie/util.hpp>

#include <pixie/pixie16/channel.hpp>
#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
namespace param {
/*
 * Param errors
 */
typedef pixie::error::error error;

static const system_param_map system_params = {
    {"NUMBER_MODULES", {system_param::number_modules}},
    {"OFFLINE_ANALYSIS", {system_param::offline_analysis}},
    {"PXI_SLOT_MAP", {system_param::pxi_slot_map}},
};

static const module_param_map module_params = {
    {"CrateID", {module_param::crateid}},
    {"FAST_FILTER_RANGE", {module_param::fast_filter_range}},
    {"FastTrigBackplaneEna", {module_param::fasttrigbackplaneena}},
    {"HOST_RT_PRESET", {module_param::host_rt_preset}},
    {"IN_SYNCH", {module_param::in_synch}},
    {"MODULE_CSRA", {module_param::module_csra}},
    {"MODULE_CSRB", {module_param::module_csrb}},
    {"MODULE_FORMAT", {module_param::module_format}},
    {"ModID", {module_param::module_id}},
    {"MODULE_NUMBER", {module_param::module_number}},
    {"MAX_EVENTS", {module_param::max_events}},
    {"SlotID", {module_param::slotid}},
    {"SLOW_FILTER_RANGE", {module_param::slow_filter_range}},
    {"SYNCH_WAIT", {module_param::synch_wait}},
    {"TrigConfig0", {module_param::trigconfig0}},
    {"TrigConfig1", {module_param::trigconfig1}},
    {"TrigConfig2", {module_param::trigconfig2}},
    {"TrigConfig3", {module_param::trigconfig3}},
};

static const channel_param_map channel_params = {
    {"BASELINE_AVERAGE", {channel_param::baseline_average}},
    {"BASELINE_PERCENT", {channel_param::baseline_percent}},
    {"BINFACTOR", {channel_param::binfactor}},
    {"BLCUT", {channel_param::blcut}},
    {"CFDDelay", {channel_param::cfddelay}},
    {"CFDScale", {channel_param::cfdscale}},
    {"CFDThresh", {channel_param::cfdthresh}},
    {"CHANNEL_CSRA", {channel_param::channel_csra}},
    {"CHANNEL_CSRB", {channel_param::channel_csrb}},
    {"ChanTrigStretch", {channel_param::chantrigstretch}},
    {"EMIN", {channel_param::emin}},
    {"ENERGY_FLATTOP", {channel_param::energy_flattop}},
    {"ENERGY_RISETIME", {channel_param::energy_risetime}},
    {"ExternDelayLen", {channel_param::externdelaylen}},
    {"ExtTrigStretch", {channel_param::exttrigstretch}},
    {"FASTTRIGBACKLEN", {channel_param::fasttrigbacklen}},
    {"FtrigoutDelay", {channel_param::ftrigoutdelay}},
    {"INTEGRATOR", {channel_param::integrator}},
    {"MultiplicityMaskH", {channel_param::multiplicitymaskh}},
    {"MultiplicityMaskL", {channel_param::multiplicitymaskl}},
    {"QDCLen0", {channel_param::qdclen0}},
    {"QDCLen1", {channel_param::qdclen1}},
    {"QDCLen2", {channel_param::qdclen2}},
    {"QDCLen3", {channel_param::qdclen3}},
    {"QDCLen4", {channel_param::qdclen4}},
    {"QDCLen5", {channel_param::qdclen5}},
    {"QDCLen6", {channel_param::qdclen6}},
    {"QDCLen7", {channel_param::qdclen7}},
    {"TAU", {channel_param::tau}},
    {"TRACE_DELAY", {channel_param::trace_delay}},
    {"TRACE_LENGTH", {channel_param::trace_length}},
    {"TRIGGER_FLATTOP", {channel_param::trigger_flattop}},
    {"TRIGGER_RISETIME", {channel_param::trigger_risetime}},
    {"TRIGGER_THRESHOLD", {channel_param::trigger_threshold}},
    {"VetoStretch", {channel_param::vetostretch}},
    {"VOFFSET", {channel_param::voffset}},
    {"XDT", {channel_param::xdt}},
};

static const module_var_map module_vars = {
    {"AECorr", module_var::AECorr},
    {"AOutBuffer", module_var::AOutBuffer},
    {"BufHeadLen", module_var::BufHeadLen},
    {"ChanHeadLen", module_var::ChanHeadLen},
    {"ChanNum", module_var::ChanNum},
    {"CoincPattern", module_var::CoincPattern},
    {"CoincWait", module_var::CoincWait},
    {"ControlTask", module_var::ControlTask},
    {"CrateID", module_var::CrateID},
    {"DSPbuild", module_var::DSPbuild},
    {"DSPerror", module_var::DSPerror},
    {"DSPrelease", module_var::DSPrelease},
    {"DSPVariant", module_var::DSPVariant},
    {"EventHeadLen", module_var::EventHeadLen},
    {"FastFilterRange", module_var::FastFilterRange},
    {"FastTrigBackplaneEna", module_var::FastTrigBackplaneEna},
    {"FIFOLength", module_var::FIFOLength},
    {"FippiID", module_var::FippiID},
    {"FippiVariant", module_var::FippiVariant},
    {"GSLTtime", module_var::GSLTtime},
    {"HardVariant", module_var::HardVariant},
    {"HardwareID", module_var::HardwareID},
    {"HostIO", module_var::HostIO},
    {"HostRunTimePreset", module_var::HostRunTimePreset},
    {"InSynch", module_var::InSynch},
    {"LECorr", module_var::LECorr},
    {"LOutBuffer", module_var::LOutBuffer},
    {"MaxEvents", module_var::MaxEvents},
    {"ModCSRA", module_var::ModCSRA},
    {"ModCSRB", module_var::ModCSRB},
    {"ModFormat", module_var::ModFormat},
    {"ModID", module_var::ModID},
    {"ModNum", module_var::ModNum},
    {"NumEventsA", module_var::NumEventsA},
    {"NumEventsB", module_var::NumEventsB},
    {"PowerUpInitDone", module_var::PowerUpInitDone},
    {"RealTimeA", module_var::RealTimeA},
    {"RealTimeB", module_var::RealTimeB},
    {"Resume", module_var::Resume},
    {"RunTask", module_var::RunTask},
    {"RunTimeA", module_var::RunTimeA},
    {"RunTimeB", module_var::RunTimeB},
    {"SlotID", module_var::SlotID},
    {"SlowFilterRange", module_var::SlowFilterRange},
    {"SynchDone", module_var::SynchDone},
    {"SynchWait", module_var::SynchWait},
    {"TrigConfig", module_var::TrigConfig},
    {"U00", module_var::U00},
    {"U20", module_var::U20},
    {"UserIn", module_var::UserIn},
    {"UserOut", module_var::UserOut},
};

static const channel_var_map channel_vars = {
    {"AutoTau", channel_var::AutoTau},
    {"BaselinePercent", channel_var::BaselinePercent},
    {"BLcut", channel_var::BLcut},
    {"CFDDelay", channel_var::CFDDelay},
    {"CFDScale", channel_var::CFDScale},
    {"CFDThresh", channel_var::CFDThresh},
    {"ChanCSRa", channel_var::ChanCSRa},
    {"ChanCSRb", channel_var::ChanCSRb},
    {"ChanEventsA", channel_var::ChanEventsA},
    {"ChanEventsB", channel_var::ChanEventsB},
    {"ChanTrigStretch", channel_var::ChanTrigStretch},
    {"DigGain", channel_var::DigGain},
    {"EnergyLow", channel_var::EnergyLow},
    {"ExternDelayLen", channel_var::ExternDelayLen},
    {"ExtTrigStretch", channel_var::ExtTrigStretch},
    {"FastGap", channel_var::FastGap},
    {"FastLength", channel_var::FastLength},
    {"FastPeaksA", channel_var::FastPeaksA},
    {"FastPeaksB", channel_var::FastPeaksB},
    {"FastThresh", channel_var::FastThresh},
    {"FastTrigBackLen", channel_var::FastTrigBackLen},
    {"FtrigoutDelay", channel_var::FtrigoutDelay},
    {"GainDAC", channel_var::GainDAC},
    {"InSpecA", channel_var::InSpecA},
    {"InSpecB", channel_var::InSpecB},
    {"Integrator", channel_var::Integrator},
    {"LiveTimeA", channel_var::LiveTimeA},
    {"LiveTimeB", channel_var::LiveTimeB},
    {"Log2Bweight", channel_var::Log2Bweight},
    {"Log2Ebin", channel_var::Log2Ebin},
    {"MultiplicityMaskH", channel_var::MultiplicityMaskH},
    {"MultiplicityMaskL", channel_var::MultiplicityMaskL},
    {"OffsetDAC", channel_var::OffsetDAC},
    {"OverflowA", channel_var::OverflowA},
    {"OverflowB", channel_var::OverflowB},
    {"PAFlength", channel_var::PAFlength},
    {"PeakSample", channel_var::PeakSample},
    {"PeakSep", channel_var::PeakSep},
    {"PreampTau", channel_var::PreampTau},
    {"PSAlength", channel_var::PSAlength},
    {"PSAoffset", channel_var::PSAoffset},
    {"QDCLen0", channel_var::QDCLen0},
    {"QDCLen1", channel_var::QDCLen1},
    {"QDCLen2", channel_var::QDCLen2},
    {"QDCLen3", channel_var::QDCLen3},
    {"QDCLen4", channel_var::QDCLen4},
    {"QDCLen5", channel_var::QDCLen5},
    {"QDCLen6", channel_var::QDCLen6},
    {"QDCLen7", channel_var::QDCLen7},
    {"ResetDelay", channel_var::ResetDelay},
    {"SlowGap", channel_var::SlowGap},
    {"SlowLength", channel_var::SlowLength},
    {"ThreshWidth", channel_var::ThreshWidth},
    {"TraceLength", channel_var::TraceLength},
    {"TriggerDelay", channel_var::TriggerDelay},
    {"TrigOutLen", channel_var::TrigOutLen},
    {"U30", channel_var::U30},
    {"UnderflowA", channel_var::UnderflowA},
    {"UnderflowB", channel_var::UnderflowB},
    {"VetoStretch", channel_var::VetoStretch},
    {"Xavg", channel_var::Xavg},
    {"Xwait", channel_var::Xwait},
};

static const module_var_descs module_var_descriptors_default = {
    {module_var::AECorr, enable, ro, 1, "AECorr"},
    {module_var::AOutBuffer, enable, ro, 1, "AOutBuffer"},
    {module_var::BufHeadLen, disable, ro, 1, "BufHeadLen"},
    {module_var::ChanHeadLen, disable, ro, 1, "ChanHeadLen"},
    {module_var::ChanNum, enable, rw, 1, "ChanNum"},
    {module_var::CoincPattern, disable, rw, 1, "CoincPattern"},
    {module_var::CoincWait, disable, rw, 1, "CoincWait"},
    {module_var::ControlTask, enable, rw, 1, "ControlTask"},
    {module_var::CrateID, enable, rw, 1, "CrateID"},
    {module_var::DSPbuild, enable, ro, 1, "DSPbuild"},
    {module_var::DSPerror, enable, ro, 1, "DSPerror"},
    {module_var::DSPrelease, enable, ro, 1, "DSPrelease"},
    {module_var::DSPVariant, disable, ro, 1, "DSPVariant"},
    {module_var::EventHeadLen, disable, ro, 1, "EventHeadLen"},
    {module_var::FastFilterRange, enable, rw, 1, "FastFilterRange"},
    {module_var::FastTrigBackplaneEna, enable, rw, 1, "FastTrigBackplaneEna"},
    //@TODO This is here temporarily to facilitate tests until P16-263 is done.
    {module_var::FIFOLength, enable, rw, 1, "FIFOLength"},
    {module_var::FippiID, disable, ro, 1, "FippiID"},
    {module_var::FippiVariant, disable, ro, 1, "FippiVariant"},
    {module_var::GSLTtime, enable, ro, 1, "GSLTtime"},
    {module_var::HardVariant, enable, ro, 1, "HardVariant"},
    {module_var::HardwareID, enable, ro, 1, "HardwareID"},
    {module_var::HostIO, enable, rw, 16, "HostIO"},
    {module_var::HostRunTimePreset, enable, rw, 1, "HostRunTimePreset"},
    {module_var::InSynch, enable, rw, 1, "InSynch"},
    {module_var::LECorr, enable, ro, 1, "LECorr"},
    {module_var::LOutBuffer, disable, ro, 1, "LOutBuffer"},
    {module_var::MaxEvents, disable, rw, 1, "MaxEvents"},
    {module_var::ModCSRA, disable, rw, 1, "ModCSRA"},
    {module_var::ModCSRB, enable, rw, 1, "ModCSRB"},
    {module_var::ModFormat, disable, rw, 1, "ModFormat"},
    {module_var::ModID, disable, rw, 1, "ModID"},
    {module_var::ModNum, enable, rw, 1, "ModNum"},
    {module_var::NumEventsA, disable, ro, 1, "NumEventsA"},
    {module_var::NumEventsB, disable, ro, 1, "NumEventsB"},
    {module_var::PowerUpInitDone, enable, ro, 1, "PowerUpInitDone"},
    {module_var::RealTimeA, enable, ro, 1, "RealTimeA"},
    {module_var::RealTimeB, enable, ro, 1, "RealTimeB"},
    {module_var::Resume, enable, rw, 1, "Resume"},
    {module_var::RunTask, enable, rw, 1, "RunTask"},
    {module_var::RunTimeA, enable, ro, 1, "RunTimeA"},
    {module_var::RunTimeB, enable, ro, 1, "RunTimeB"},
    {module_var::SlotID, enable, rw, 1, "SlotID"},
    {module_var::SlowFilterRange, enable, rw, 1, "SlowFilterRange"},
    {module_var::SynchDone, enable, ro, 1, "SynchDone"},
    {module_var::SynchWait, enable, rw, 1, "SynchWait"},
    {module_var::TrigConfig, enable, rw, 4, "TrigConfig"},
    {module_var::U00, disable, rw, 7, "U00"},
    {module_var::U20, enable, ro, 1, "U20"},
    {module_var::UserIn, enable, rw, 16, "UserIn"},
    {module_var::UserOut, enable, ro, 16, "UserOut"},
};

static const channel_var_descs channel_var_descriptors_default = {
    {channel_var::AutoTau, enable, ro, 1, "AutoTau"},
    {channel_var::BaselinePercent, enable, rw, 1, "BaselinePercent"},
    {channel_var::BLcut, enable, rw, 1, "BLcut"},
    {channel_var::CFDDelay, enable, rw, 1, "CFDDelay"},
    {channel_var::CFDScale, enable, rw, 1, "CFDScale"},
    {channel_var::CFDThresh, enable, rw, 1, "CFDThresh"},
    {channel_var::ChanCSRa, enable, rw, 1, "ChanCSRa"},
    {channel_var::ChanCSRb, disable, rw, 1, "ChanCSRb"},
    {channel_var::ChanEventsA, enable, ro, 1, "ChanEventsA"},
    {channel_var::ChanEventsB, enable, ro, 1, "ChanEventsB"},
    {channel_var::ChanTrigStretch, enable, rw, 1, "ChanTrigStretch"},
    {channel_var::DigGain, disable, rw, 1, "DigGain"},
    {channel_var::EnergyLow, enable, rw, 1, "EnergyLow"},
    {channel_var::ExternDelayLen, enable, rw, 1, "ExternDelayLen"},
    {channel_var::ExtTrigStretch, enable, rw, 1, "ExtTrigStretch"},
    {channel_var::FastGap, enable, rw, 1, "FastGap"},
    {channel_var::FastLength, enable, rw, 1, "FastLength"},
    {channel_var::FastPeaksA, enable, ro, 1, "FastPeaksA"},
    {channel_var::FastPeaksB, enable, ro, 1, "FastPeaksB"},
    {channel_var::FastThresh, enable, rw, 1, "FastThresh"},
    {channel_var::FastTrigBackLen, enable, rw, 1, "FastTrigBackLen"},
    {channel_var::FtrigoutDelay, enable, rw, 1, "FtrigoutDelay"},
    {channel_var::GainDAC, disable, rw, 1, "GainDAC"},
    {channel_var::InSpecA, enable, ro, 1, "InSpecA"},
    {channel_var::InSpecB, enable, ro, 1, "InSpecB"},
    {channel_var::Integrator, disable, rw, 1, "Integrator"},
    {channel_var::Log2Bweight, enable, rw, 1, "Log2Bweight"},
    {channel_var::Log2Ebin, enable, rw, 1, "Log2Ebin"},
    {channel_var::LiveTimeA, enable, ro, 1, "LiveTimeA"},
    {channel_var::LiveTimeB, enable, ro, 1, "LiveTimeB"},
    {channel_var::MultiplicityMaskH, enable, rw, 1, "MultiplicityMaskH"},
    {channel_var::MultiplicityMaskL, enable, rw, 1, "MultiplicityMaskL"},
    {channel_var::OffsetDAC, enable, rw, 1, "OffsetDAC"},
    {channel_var::OverflowA, enable, ro, 1, "OverflowA"},
    {channel_var::OverflowB, enable, ro, 1, "OverflowB"},
    {channel_var::PAFlength, enable, rw, 1, "PAFlength"},
    {channel_var::PeakSample, enable, rw, 1, "PeakSample"},
    {channel_var::PeakSep, enable, rw, 1, "PeakSep"},
    {channel_var::PreampTau, enable, rw, 1, "PreampTau"},
    {channel_var::PSAlength, disable, rw, 1, "PSAlength"},
    {channel_var::PSAoffset, disable, rw, 1, "PSAoffset"},
    {channel_var::QDCLen0, enable, rw, 1, "QDCLen0"},
    {channel_var::QDCLen1, enable, rw, 1, "QDCLen1"},
    {channel_var::QDCLen2, enable, rw, 1, "QDCLen2"},
    {channel_var::QDCLen3, enable, rw, 1, "QDCLen3"},
    {channel_var::QDCLen4, enable, rw, 1, "QDCLen4"},
    {channel_var::QDCLen5, enable, rw, 1, "QDCLen5"},
    {channel_var::QDCLen6, enable, rw, 1, "QDCLen6"},
    {channel_var::QDCLen7, enable, rw, 1, "QDCLen7"},
    {channel_var::ResetDelay, disable, rw, 1, "ResetDelay"},
    {channel_var::SlowGap, enable, rw, 1, "SlowGap"},
    {channel_var::SlowLength, enable, rw, 1, "SlowLength"},
    {channel_var::ThreshWidth, disable, rw, 1, "ThreshWidth"},
    {channel_var::TraceLength, enable, rw, 1, "TraceLength"},
    {channel_var::TriggerDelay, enable, rw, 1, "TriggerDelay"},
    {channel_var::TrigOutLen, disable, rw, 1, "TrigOutLen"},
    {channel_var::U30, disable, ro, 11, "U30"},
    {channel_var::UnderflowA, enable, ro, 1, "UnderflowA"},
    {channel_var::UnderflowB, enable, ro, 1, "UnderflowB"},
    {channel_var::Xavg, disable, rw, 1, "Xavg"},
    {channel_var::Xwait, enable, rw, 1, "Xwait"},
    {channel_var::VetoStretch, enable, rw, 1, "VetoStretch"},
};

static const module_var module_param_var_map[] = {
    module_var::CrateID,
    module_var::FastFilterRange,
    module_var::FastTrigBackplaneEna,
    module_var::InSynch,
    module_var::HostRunTimePreset,
    module_var::ModNum,
    module_var::ModCSRA,
    module_var::ModCSRB,
    module_var::ModID,
    module_var::ModFormat,
    module_var::MaxEvents,
    module_var::SlotID,
    module_var::SlowFilterRange,
    module_var::SynchWait,
    module_var::TrigConfig,
    module_var::TrigConfig,
    module_var::TrigConfig,
    module_var::TrigConfig,
};

const copy_filter energy_filter = {
    channel_var::BLcut,        channel_var::PAFlength,  channel_var::PeakSample,
    channel_var::PeakSep,      channel_var::SlowLength, channel_var::SlowGap,
    channel_var::TriggerDelay,
    /* channel_var::CoincWait,      MODULE var?? */
    /* channel_var::SlowFilterRange MODULE var?? */
};

const copy_filter trigger_filter = {
    channel_var::FastGap, channel_var::FastLength, channel_var::FastThresh,
    /* channel_var::FastFilterRange MODULE var?? */
};

const copy_filter analog_signal_cond_filter = {
    {channel_var::ChanCSRa, 1 << hw::bit::CCSRA_POLARITY | 1 << hw::bit::CCSRA_ENARELAY},
    channel_var::OffsetDAC};

const copy_filter histogram_control_filter = {channel_var::EnergyLow, channel_var::Log2Ebin};

const copy_filter decay_time_filter = {channel_var::PreampTau};

const copy_filter pulse_shape_analysis_filter = {channel_var::TraceLength,
                                                 channel_var::TriggerDelay, channel_var::PAFlength};

const copy_filter baseline_control_filter = {channel_var::BLcut, channel_var::BaselinePercent,
                                             channel_var::Log2Bweight};

const copy_filter channel_csra_filter = {channel_var::ChanCSRa};

const copy_filter cfd_trigger_filter = {channel_var::CFDDelay, channel_var::CFDScale,
                                        channel_var::CFDThresh};

const copy_filter trigger_stretch_len_filter = {
    channel_var::ExtTrigStretch, channel_var::ChanTrigStretch, channel_var::VetoStretch,
    channel_var::FastTrigBackLen};

const copy_filter fifo_delays_filter = {channel_var::ExternDelayLen, channel_var::FtrigoutDelay};

const copy_filter multiplicity_filter = {channel_var::MultiplicityMaskL,
                                         channel_var::MultiplicityMaskH};

const copy_filter qdc_filter = {channel_var::QDCLen0, channel_var::QDCLen1, channel_var::QDCLen2,
                                channel_var::QDCLen3, channel_var::QDCLen4, channel_var::QDCLen5,
                                channel_var::QDCLen6, channel_var::QDCLen7};

address_map::range::range() : start(0), end(0), size(0) {}

void address_map::range::set_size() {
    size = end - start;
}

address_map::address_map() : vars(0), module_vars(0), channel_vars(0), vars_per_channel(0) {}

void address_map::range::output(std::ostream& out) const {
    util::ostream_guard guard(out);
    out << std::showbase << std::hex << '[' << start << ',' << end << std::dec << ")," << size;
}

void address_map::set(const size_t max_channels, const module_var_descs& module_descs,
                      const channel_var_descs& channel_descs) {
    vars_per_channel = channel_descs.size();
    module_vars = module_descs.size();
    channel_vars = max_channels * vars_per_channel;
    vars = module_vars + channel_vars;

    desc_addresses da;

    get(module_descs, da, rwrowr::rw);
    module_in.start = min(da);
    module_in.end = max(da);
    module_in.set_size();

    get(module_descs, da, rwrowr::ro);
    module_out.start = min(da);
    module_out.end = max(da);
    module_out.set_size();

    module.start = std::min(module_in.start, module_out.start);
    module.end = std::max(module_in.end, module_out.end);
    module.set_size();

    get(channel_descs, da, rwrowr::rw);
    check_channel_gap(max_channels, channel_descs, da);
    channels_in.start = min(da);
    channels_in.end = max(da);
    channels_in.set_size();

    get(channel_descs, da, rwrowr::ro);
    check_channel_gap(max_channels, channel_descs, da);
    channels_out.start = min(da);
    channels_out.end = max(da);
    channels_out.set_size();

    channels.start = std::min(channels_in.start, channels_out.start);
    channels.end = std::max(channels_in.end, channels_out.end);
    channels.set_size();

    full.start = std::min(module.start, channels.start);
    full.end = std::max(module.end, channels.end);
    full.set_size();
}

void address_map::output(std::ostream& out) const {
    out << "full=";
    full.output(out);
    out << " mod=";
    module.output(out);
    out << " mod-in=";
    module_in.output(out);
    out << " mod-out=";
    module_out.output(out);
    out << " chans=";
    channels.output(out);
    out << " chans-in=";
    channels_in.output(out);
    out << " chans-out=";
    channels_out.output(out);
}

void address_map::check_channel_gap(const size_t max_channels,
                                    const channel_var_descs& channel_descs,
                                    const desc_addresses& addresses) {
    /*
     * The var file of addresses are not in the same order as the logical
     * descriptor table. Sort by address then check the gaps.
     */
    for (size_t a = 0; a < addresses.size() - 1; ++a) {
        size_t desc = std::get<0>(addresses[a]);
        hw::address addr = std::get<1>(addresses[a]);
        size_t gap = std::get<1>(addresses[a + 1]) - addr;
        size_t gap_num_channels = gap / channel_descs[desc].size;
        if (gap_num_channels != max_channels) {
            throw error(error::code::channel_invalid_var,
                        "invalid channel var address gap: " + channel_descs[desc].name);
        }
    }
}

template<typename V>
void address_map::get(const V& vars, desc_addresses& addresses, const rwrowr mode) {
    addresses.clear();
    for (size_t d = 0; d < vars.size(); ++d) {
        if (vars[d].mode == mode) {
            auto da = desc_address(d, vars[d].address);
            addresses.push_back(da);
        }
    }
    std::sort(addresses.begin(), addresses.end(),
              [](auto& a, auto& b) { return std::get<1>(a) < std::get<1>(b); });
}

hw::address address_map::min(const desc_addresses& addresses) {
    auto min = std::min_element(addresses.begin(), addresses.end(),
                                [](auto& a, auto& b) { return std::get<1>(a) < std::get<1>(b); });
    return std::get<1>((*min));
}

hw::address address_map::max(const desc_addresses& addresses) {
    auto max = std::max_element(addresses.begin(), addresses.end(),
                                [](auto& a, auto& b) { return std::get<1>(a) < std::get<1>(b); });
    return std::get<1>((*max));
}

const module_var_descs& get_module_var_descriptors() {
    return module_var_descriptors_default;
}

const channel_var_descs& get_channel_var_descriptors() {
    return channel_var_descriptors_default;
}

const system_param_map get_system_param_map() {
    return system_params;
}

const module_param_map get_module_param_map() {
    return module_params;
}

const channel_param_map get_channel_param_map() {
    return channel_params;
}

bool is_system_param(const std::string& label) {
    auto search = system_params.find(label);
    return search != system_params.end();
}

bool is_module_param(const std::string& label) {
    auto search = module_params.find(label);
    return search != module_params.end();
}

bool is_channel_param(const std::string& label) {
    auto search = channel_params.find(label);
    return search != channel_params.end();
}

bool is_module_var(const std::string& label) {
    auto search = module_vars.find(label);
    return search != module_vars.end();
}

bool is_channel_var(const std::string& label) {
    auto search = channel_vars.find(label);
    return search != channel_vars.end();
}

system_param lookup_system_param(const std::string& label) {
    auto search = system_params.find(label);
    if (search == system_params.end()) {
        throw error(error::code::crate_invalid_param, "invalid system param: " + label);
    }
    return search->second;
}

module_param lookup_module_param(const std::string& label) {
    auto search = module_params.find(label);
    if (search == module_params.end()) {
        throw error(error::code::module_invalid_param, "invalid module param: " + label);
    }
    return search->second;
}

channel_param lookup_channel_param(const std::string& label) {
    auto search = channel_params.find(label);
    if (search == channel_params.end()) {
        throw error(error::code::channel_invalid_param, "invalid channel param: " + label);
    }
    return search->second;
}

module_var lookup_module_var(const std::string& label) {
    auto search = module_vars.find(label);
    if (search == module_vars.end()) {
        throw error(error::code::module_invalid_var, "invalid module variable: " + label);
    }
    return search->second;
}

channel_var lookup_channel_var(const std::string& label) {
    auto search = channel_vars.find(label);
    if (search == channel_vars.end()) {
        throw error(error::code::channel_invalid_var, "invalid channel variable: " + label);
    }
    return search->second;
}

void load(const std::string& dspvarfile, module_var_descs& module_var_descriptors,
          channel_var_descs& channel_var_descriptors) {
    std::ifstream input(dspvarfile, std::ios::in | std::ios::binary);
    if (!input) {
        throw error(error::code::file_read_failure, std::string("channel_var file open: ") +
                                                        dspvarfile + ": " + std::strerror(errno));
    }
    try {
        load(input, module_var_descriptors, channel_var_descriptors);
    } catch (...) {
        input.close();
        throw;
    }
    input.close();
}

module_var map_module_param(module_param par) {
    return module_param_var_map[int(par)];
}

void load(firmware::firmware_ref& firmware, module_var_descs& module_var_descriptors,
          channel_var_descs& channel_var_descriptors) {
    /*
     * Sanity check for the enums and the tables, so they match. If the enum
     * does not reference the correct variable descriptor things break.
     */
    for (size_t d = 0; d < module_var_descriptors_default.size(); ++d) {
        if (d != size_t(module_var_descriptors_default[d].par)) {
            log(log::error) << "module desc: index=" << d
                            << " enum=" << int(module_var_descriptors_default[d].par);
            throw error(error::code::internal_failure,
                        "module descriptor table does not match enum ordering");
        }
    }
    for (size_t d = 0; d < channel_var_descriptors_default.size(); ++d) {
        if (d != size_t(channel_var_descriptors_default[d].par)) {
            log(log::error) << "channel desc: index=" << d
                            << " enum=" << int(channel_var_descriptors_default[d].par);
            throw error(error::code::internal_failure,
                        "channel descriptor table does not match enum ordering");
        }
    }

    log(log::info) << "firmware: load vars: " << *firmware;

    if (firmware->device != "var") {
        throw error(error::code::device_image_failure, "invalid image type");
    }
    if (firmware->data.empty()) {
        throw error(error::code::device_image_failure, "no image loaded");
    }

    struct membuf : std::streambuf {
        membuf(char* base, std::ptrdiff_t size) {
            this->setg(base, base, base + size);
        }
    };
    char* data = static_cast<char*>(static_cast<void*>(firmware->data.data()));
    membuf sbuf(data, firmware->data.size());
    std::istream input(&sbuf);
    load(input, module_var_descriptors, channel_var_descriptors);
}

void load(std::istream& input, module_var_descs& module_var_descriptors,
          channel_var_descs& channel_var_descriptors) {
    for (std::string line; std::getline(input, line);) {
        if (!line.empty()) {
            std::istringstream iss(line);
            uint32_t address;
            std::string name;
            iss >> std::hex >> address >> name;
            auto mvi = std::find_if(module_var_descriptors.begin(), module_var_descriptors.end(),
                                    [name](module_var_desc desc) { return desc.name == name; });
            if (mvi != module_var_descriptors.end()) {
                (*mvi).address = address;
            } else {
                auto cvi =
                    std::find_if(channel_var_descriptors.begin(), channel_var_descriptors.end(),
                                 [name](channel_var_desc desc) { return desc.name == name; });
                if (cvi != channel_var_descriptors.end()) {
                    (*cvi).address = address;
                } else {
                    std::string what = "DSP variable not found: ";
                    what += name;
                    throw error(error::code::device_image_failure, what);
                }
            }
        }
    }

    log(log::info) << "firmware: var descriptions loaded: module=" << module_var_descriptors.size()
                   << " channel=" << channel_var_descriptors.size();
}

void copy_parameters(const copy_filter& filter, const channel_variables& source,
                     channel_variables& dest) {
    if (source.size() != dest.size())
        throw error(error::code::device_copy_failure, "copy source and dest size do not match");

    for (auto f : filter) {
        int v = static_cast<int>(f.var);
        for (size_t i = 0; i < dest.size(); ++i) {
            dest[v].value[i].value =
                (dest[v].value[i].value & ~f.mask) | (source[v].value[i].value & f.mask);
            dest[v].value[i].dirty = true;
        }
    }
}

void copy_parameters(const unsigned int filter_mask, const channel_variables& source,
                     channel_variables& dest) {
    if ((filter_mask & energy_mask) != 0) {
        copy_parameters(energy_filter, source, dest);
    }
    if ((filter_mask & trigger_mask) != 0) {
        copy_parameters(trigger_filter, source, dest);
    }
    if ((filter_mask & analog_signal_cond_mask) != 0) {
        copy_parameters(analog_signal_cond_filter, source, dest);
    }
    if ((filter_mask & histogram_control_mask) != 0) {
        copy_parameters(histogram_control_filter, source, dest);
    }
    if ((filter_mask & decay_time_mask) != 0) {
        copy_parameters(decay_time_filter, source, dest);
    }
    if ((filter_mask & pulse_shape_analysis_mask) != 0) {
        copy_parameters(pulse_shape_analysis_filter, source, dest);
    }
    if ((filter_mask & baseline_control_mask) != 0) {
        copy_parameters(baseline_control_filter, source, dest);
    }
    if ((filter_mask & channel_csra_mask) != 0) {
        copy_parameters(channel_csra_filter, source, dest);
    }
    if ((filter_mask & cfd_trigger_mask) != 0) {
        copy_parameters(cfd_trigger_filter, source, dest);
    }
    if ((filter_mask & trigger_stretch_len_mask) != 0) {
        copy_parameters(trigger_stretch_len_filter, source, dest);
    }
    if ((filter_mask & fifo_delays_mask) != 0) {
        copy_parameters(fifo_delays_filter, source, dest);
    }
    if ((filter_mask & multiplicity_mask) != 0) {
        copy_parameters(multiplicity_filter, source, dest);
    }
    if ((filter_mask & qdc_mask) != 0) {
        copy_parameters(qdc_filter, source, dest);
    }
}

};  // namespace param
};  // namespace pixie
};  // namespace xia

std::ostream& operator<<(std::ostream& out, const xia::pixie::param::address_map& config) {
    config.output(out);
    return out;
}
