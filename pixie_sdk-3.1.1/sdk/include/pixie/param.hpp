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

/** @file param.hpp
 * @brief Defines functions and data structures related to handling parameter sets
 */

#ifndef PIXIE_PARAM_H
#define PIXIE_PARAM_H

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <pixie/os_compat.hpp>

#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
namespace firmware {
struct firmware;
typedef std::shared_ptr<firmware> firmware_ref;
}  // namespace firmware
/**
 * @brief Tools for working with parameters and variables in the SDK.
 */
namespace param {
/**
 * @brief Defines System parameters that we use in various locations.
 *
 * @note Order of entries in this enum must match with the definitions in
 * `system_params`.
 */
enum struct system_param {
    /**
     * @brief The number of modules that are online.
     */
    number_modules,
    /**
     * @brief **Not implemented.**
     */
    offline_analysis,
    /**
     * @brief **Not implemented.**
     */
    pxi_slot_map,
    /**
     * @brief Size marker
     */
    END
};

/**
 * @brief Defines user facing module parameters.
 *
 * Please see the linked DSP Variables for more information.
 *
 * @remark All ranges are integer steps unless otherwise noted.
 * @note Order of entries in this enum must match with the definitions in
 * `module_params`.
 */
enum struct module_param {
    /**
     * @ref xia::pixie::param::module_var::CrateID <br>
     * **Values**: [0, number of crates)
     */
    crateid,
    /**
     * @ref xia::pixie::param::module_var::FastFilterRange <br>
     * **Values**: 0
     */
    fast_filter_range,
    /**
     * @ref xia::pixie::param::module_var::FastTrigBackplaneEna <br>
     * **Values**: [0, 1] 
     */
    fasttrigbackplaneena,
    /**
     * @ref xia::pixie::param::module_var::HostRunTimePreset
     * **Values**: All real numbers in [0, 2<sup>32</sup>-1]
     */
    host_rt_preset,
    /**
     * @ref xia::pixie::param::module_var::InSynch <br>
     * **Values**: [0, 1] 
     */
    in_synch,
    /**
     * @ref xia::pixie::param::module_var::MaxEvents
     */
    max_events,
    /**
     * @ref xia::pixie::param::module_var::ModCSRA
     *
     * **Values**: [0, 2<sup>32</sup>-1]
     */
    module_csra,
    /**
     * @ref xia::pixie::param::module_var::ModCSRB
     *
     * **Values**: [0, 2<sup>32</sup>-1]
     */
    module_csrb,
    /**
     * @ref xia::pixie::param::module_var::ModFormat
     */
    module_format,
    /**
     * @ref xia::pixie::param::module_var::ModID <br>
     * **Values**: [0, number of modules)
     */
    module_id,
    /**
     * @ref xia::pixie::param::module_var::ModNum
     *
     * **Range**: [0, # active modules)
     */
    module_number,
    /**
     * @ref xia::pixie::param::module_var::SlotID <br>
     * **Values**: [2, 14]
     */
    slotid,
    /**
     * @ref xia::pixie::param::module_var::SlowFilterRange <br>
     * **Values**: [1, 6] 
     */
    slow_filter_range,
    /**
     * @ref xia::pixie::param::module_var::SynchWait <br>
     * **Values**: [0, 1] 
     */
    synch_wait,
    /**
     * @ref xia::pixie::param::module_var::TrigConfig
     */
    trigconfig0,
    /**
     * @ref xia::pixie::param::module_var::TrigConfig
     */
    trigconfig1,
    /**
     * @ref xia::pixie::param::module_var::TrigConfig
     */
    trigconfig2,
    /**
     * @ref xia::pixie::param::module_var::TrigConfig
     */
    trigconfig3,
    /**
     * @brief Size marker
     */
    END
};

/**
 * @brief Define user-facing Channel parameters.
 *
 * Custom interfaces can set and read these variables in whatever form is most
 * suitable. For all parameters, also see the corresponding entry in DSP
 * parameter section for details.
 *
 * In most cases, the PixieSDK adjusts the input parameter to the closest value
 * that corresponds to a valid DSP parameter value, for example entering an
 * ENERGY_FLATTOP of 22ns will be adjusted to 20ns.
 *
 * For compatibility, the SDK uses a lookup table to match the enumeration with
 * the original string values. For example, `CHANNEL_CSRA` maps to the
 * enumerator `channel_csra`.
 *
 * @note Order of entries in this enum must match with the definitions in
 * `channel_params`.
 */
enum struct channel_param {
    /**
     * @ref xia::pixie::param::channel_var::Log2Bweight <br>
     * **Unit**: None
     */
    baseline_average,
    /**
     * @ref xia::pixie::param::channel_var::BaselinePercent <br>
     * **Unit**: %
     */
    baseline_percent,
    /**
     * @ref xia::pixie::param::channel_var::Log2Ebin <br>
     * **Value**: [1, 16] in integer steps.
     */
    binfactor,
    /**
     * @ref xia::pixie::param::channel_var::BLcut <br>
     * **Unit**: ADC Units
     */
    blcut,
    /**
     * @ref xia::pixie::param::channel_var::CFDDelay <br>
     * **Unit**: &mu;s
     */
    cfddelay,
    /**
     * @ref xia::pixie::param::channel_var::CFDScale <br>
     * **Unit**: None
     */
    cfdscale,
    /**
     * @ref xia::pixie::param::channel_var::CFDThresh <br>
     * **Unit**: ADC units
     */
    cfdthresh,
    /**
     * @ref xia::pixie::param::channel_var::ChanCSRa <br>
     * **Value**: [0, 2<sup>32</sup>-1] <br>
     * **Unit**: None
     */
    channel_csra,
    /**
     * @ref xia::pixie::param::channel_var::ChanCSRb <br>
     * **Value**: [0, 2<sup>32</sup>-1] <br>
     * **Unit**: None
     */
    channel_csrb,
    /**
     * @ref xia::pixie::param::channel_var::ChanTrigStretch <br>
     * **Unit**: &mu;s
     */
    chantrigstretch,
    /**
     * @ref xia::pixie::param::channel_var::EnergyLow <br>
     * **Value**: [0, 65535] in integer steps.
     */
    emin,
    /**
     * @ref xia::pixie::param::channel_var::SlowGap <br>
     * **Value:** [3, 125] * 2<sup>SlowFilterRange</sup> * multiplier
     * | ADC Freq. (MHz) | Multiplier (&mu;s) |
     * |---|---|
     * | 100 or 500 | 0.01  |
     * | 250 | 0.008 |
     * **Unit**: &mu;s
     */
    energy_flattop,
    /**
     * @ref xia::pixie::param::channel_var::SlowLength <br>
     * **Value:** [2, 124] * 2<sup>SlowFilterRange</sup> * multiplier
     * | ADC Freq. (MHz) | Multiplier (&mu;s) |
     * |---|---|
     * | 100 or 500 | 0.01  |
     * | 250 | 0.008 |
     * **Unit**: &mu;s
     */
    energy_risetime,
    /**
     * @ref xia::pixie::param::channel_var::ExternDelayLen <br>
     * **Unit**: &mu;s
     */
    externdelaylen,
    /**
     * @ref xia::pixie::param::channel_var::ExtTrigStretch <br>
     * **Unit**: &mu;s
     */
    exttrigstretch,
    /**
     * @ref xia::pixie::param::channel_var::FastTrigBackLen <br>
     * **Unit**: &mu;s
     */
    fasttrigbacklen,
    /**
     * @ref xia::pixie::param::channel_var::FtrigoutDelay <br>
     * **Unit**: &mu;s
     */
    ftrigoutdelay,
    /**
     * @ref xia::pixie::param::channel_var::Integrator
     */
    integrator,
    /**
     * @ref xia::pixie::param::channel_var::MultiplicityMaskL <br>
     * **Unit**: None
     */
    multiplicitymaskl,
    /**
     * @ref xia::pixie::param::channel_var::MultiplicityMaskH <br>
     * **Unit**: None
     */
    multiplicitymaskh,
    /**
     * @ref xia::pixie::param::channel_var::QDCLen0 <br>
     * **Unit**: &mu;s
     */
    qdclen0,
    /**
     * @ref xia::pixie::param::channel_var::QDCLen1 <br>
     * **Unit**: &mu;s
     */
    qdclen1,
    /**
     * @ref xia::pixie::param::channel_var::QDCLen2 <br>
     * **Unit**: &mu;s
     */
    qdclen2,
    /**
     * @ref xia::pixie::param::channel_var::QDCLen3 <br>
     * **Unit**: &mu;s
     */
    qdclen3,
    /**
     * @ref xia::pixie::param::channel_var::QDCLen4 <br>
     * **Unit**: &mu;s
     */
    qdclen4,
    /**
     * @ref xia::pixie::param::channel_var::QDCLen5 <br>
     * **Unit**: &mu;s
     */
    qdclen5,
    /**
     * @ref xia::pixie::param::channel_var::QDCLen6 <br>
     * **Unit**: &mu;s
     */
    qdclen6,
    /**
     * @ref xia::pixie::param::channel_var::QDCLen7 <br>
     * **Unit**: &mu;s
     */
    qdclen7,
    /**
     * @ref xia::pixie::param::channel_var::PreampTau <br>
     * **Unit**: &mu;s
     */
    tau,
    /**
     * @ref xia::pixie::param::channel_var::TriggerDelay <br>
     * @ref xia::pixie::param::channel_var::PAFlength <br>
     * **Value**:
     * | Revision | ADC Freq. (MHz) | Low (&mu;s) | High (&mu;s)  | Step Size (ns) |
     * |---|---|---|---|---|
     * | B/C/D/F | 100 or 500 | 0 | 10.24 | 10 |
     * | F | 250 | 0 | 8.192 | 8 |
     * **Unit**: &mu;s
     */
    trace_delay,
    /**
     * @ref xia::pixie::param::channel_var::TraceLength <br>
     * **Value**:
     * | Revision | ADC Freq. (MHz) | Low (&mu;s) | High (&mu;s)  | Step Size (ns) |
     * |---|---|---|---|---|
     * | B/C/D | 100 | 0 | 81.92 | 10 |
     * | F | 100 | 0 | 163.8 | 10 |
     * | F | 250 | 0 | 131 | 8 |
     * | F | 500 | 0 | 40.94 | 2 |
     * **Unit**: &mu;s
     */
    trace_length,
    /**
     * @ref xia::pixie::param::channel_var::FastGap <br>
     * **Value**: [0, 125] * multiplier
     * | ADC Freq. (MHz) | Multiplier (&mu;s) |
     * |---|---|
     * | 100 or 500 | 0.01 |
     * | 250 | 0.008 |
     * **Unit**: &mu;s
     */
    trigger_flattop,
    /**
     * @ref xia::pixie::param::channel_var::FastLength <br>
     * **Value:** [2, 127] * multiplier
     * | ADC Freq. (MHz) | Multiplier (&mu;s) |
     * |---|---|
     * | 100 or 500 | 0.01 |
     * | 250 | 0.008 |
     * **Unit**: &mu;s
     */
    trigger_risetime,
    /**
     * @ref xia::pixie::param::channel_var::FastThresh <br>
     * **Value**: [1, 65535]
     * **Unit**: ADC units
     */
    trigger_threshold,
    /**
     * @ref xia::pixie::param::channel_var::VetoStretch <br>
     * **Unit**: &mu;s
     */
    vetostretch,
    /**
     * @ref xia::pixie::param::channel_var::OffsetDAC <br>
     * **Value**: [-1.5, 1.5] <br>
     * **Unit**: Volts
     */
    voffset,
    /**
     * @ref xia::pixie::param::channel_var::Xwait <br>
     * **Unit**: &mu;s
     */
    xdt,
    /**
     * @brief Size marker
     */
    END
};

/**
 * @brief Module variables that are defined within the DSP VAR file
 *
 * @note Order of entries in this enum must match with the definitions in
 * `module_vars` and `module_var_descriptors_default`.
 */
enum struct module_var {
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    AECorr,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    AOutBuffer,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    BufHeadLen,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    ChanHeadLen,
    /**
     * @brief [INPUT] The chosen channel number of a Pixie module. Mainly used
     * by the host to set the designated channel.
     */
    ChanNum,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    CoincPattern,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    CoincWait,
    /**
     * @brief [INPUT] Use this variable to select a control task. The control task will
     * be launched when a run start command is issued with RUNTASK=0. <br>
     * See @ref xia::pixie::hw::run::control_task for more information.
     */
    ControlTask,
    /**
     * @brief [INPUT] ID number for chassis. Reported in list mode data for
     * purposes of event building.
     *
     * **Value:** [0, 15) in integer steps.
     */
    CrateID,
    /**
     * @brief [OUTPUT] DSP software build number.
     */
    DSPbuild,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    DSPerror,
    /**
     * @brief [OUTPUT] DSP software release number.
     */
    DSPrelease,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    DSPVariant,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    EventHeadLen,
    /**
     * @brief [INPUT] Sets the number of ADC samples
     * (2<sup>FastFilterRange</sup>) to be averaged before entering the trigger
     * filtering logic. The currently supported filer range in the signal
     * processing FPGA is only 0.
     */
    FastFilterRange,
    /**
     * @brief [INPUT] Bit mask pattern to specify whether a channel should send
     * its fast trigger to its nearest neighboring module: bits [15:0] of
     * FastTrigBackplaneEna indicating whether sending (1) or not sending (0)
     * each of the 16 channels’ fast trigger to the nearest neighboring module
     * on its left; bits [31:16] of FastTrigBackplaneEna indicating whether
     * sending (1) or not sending (0) each of the 16 channels’ fast trigger
     * to the nearest neighboring module on its right.
     */
    FastTrigBackplaneEna,
    /**
     * @brief [OUTPUT] Length of the trace buffer in the signal processing FPGAs.
     */
    FIFOLength,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    FippiID,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    FippiVariant,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    GSLTtime,
    /**
     * @brief [OUTPUT] Variant of the hardware.
     */
    HardVariant,
    /**
     * @brief [OUTPUT] ID of the hardware version.
     */
    HardwareID,
    /**
     * @brief [INPUT] A 16 word data block that is used to specify command
     * options.
     */
    HostIO,
    /**
     * @brief [INPUT] Used to set exact run time in MCA mode. A 32-bit IEEE
     * 754 standard floating point number in seconds.
     * @note Only used by MCA histogram mode run.
     */
    HostRunTimePreset,
    /**
     * @brief [INPUT] Controls clock behavior at the start of data runs.
     *
     * It is used in multi-Pixie systems in which the modules are driven by a
     * common clock. When InSynch is 1, the module assumes it is in synch with
     * the other modules and no particular action is taken at run start. If this
     * variable is 0, then all system timers are cleared at the beginning of
     * the next data acquisition run (RunTask>0). The timers are reset when
     * the entire system actually starts the run. After run start, InSynch is
     * automatically set to 1 by the DSP.
     */
    InSynch,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    LECorr,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    LOutBuffer,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    MaxEvents,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    ModCSRA,
    /**
     * @brief [INPUT] The Module Control and Status Register B.
     *
     * | Bit | Description |
     * |---|---|
     * | 0 | If set, wired-OR trigger lines on the backplane connect to a pullup resistor. This bit should be set for only one module in the backplane segment. |
     * | 1-3 | Reserved |
     * | 4 | Set this module as the Director module (1) or non-Director module (0). |
     * | 5 | Reserved |
     * | 6 | Control chassis master module: 1: chassis master module; 0: chassis non-master module |
     * | 7 | Select global fast trigger source |
     * | 8 | Select external trigger source |
     * | 9 | Reserved |
     * | 10 | Control external INHIBIT signal: use INHIBIT (1) or don't use INHIBIT (0) |
     * | 11 | Distribute clock and triggers in multiple crates: multiple crates (1) or only single crate (0) |
     * | 12 | Sort (1) or don't sort events based on their timestamps |
     * | 13 | Enable connection of fast triggers to backplane |
     * | 14-31 | Reserved |
     */
    ModCSRB,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    ModFormat,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    ModID,
    /**
     * @brief [INPUT] Logical number of the module. This number can be used to be part
     * of the list-mode data file name to aid offline event reconstruction. It
     * is assigned by the SDK at boot. <br>
     * Valid range: 0 to max crate slots - 2.
     */
    ModNum,
    /**
     * @brief [OUTPUT] **Not implemented.
     */
    NumEventsA,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    NumEventsB,
    /**
     * @brief [OUTPUT] Indicates whether routine to initialize DSP on RESET
     * is done (1) or not (0).
     */
    PowerUpInitDone,
    /**
     * @brief [OUTPUT] Upper 32-bits of the time spent on data acquisition processes.
     *
     * The clock is zeroed on power up, and also in response to a synchronous
     * data acquisition start when InSynch was set to 0 prior to the run start.
     */
    RealTimeA,
    /**
     * @brief [OUTPUT] Lower 32-bits of the time spent on data acquisition processes.
     *
     * The clock is zeroed on power up, and also in response to a synchronous
     * data acquisition start when InSynch was set to 0 prior to the run start.
     *
     * RealTime = (RealTimeA * 2<sup>32</sup> + RealTimeB) * 10ns
     */
    RealTimeB,
    /**
     * @brief [INPUT] Set this variable to 1 to resume a data run; otherwise,
     * set it to 0.
     */
    Resume,
    /**
     * @brief [INPUT] What kind of run to start in response to a run start
     * request. <br>
     * See @ref xia::pixie::hw::run::run_task for more information.
     */
    RunTask,
    /**
     * @brief [OUTPUT] Upper 32-bits of the time spent acquiring data.
     *
     * This time counter is active only while a data acquisition run is in
     * progress and thus counts the elapsed run time.
     */
    RunTimeA,
    /**
     * @brief [OUTPUT] Lower 32-bits of the time spent acquiring data.
     *
     * This time counter is active only while a data acquisition run is in
     * progress and thus counts the elapsed run time.
     *
     * RunTime = (RunTimeA * 2<sup>32</sup> + RunTimeB) * 10ns
     */
    RunTimeB,
    /**
     * @brief [INPUT] ID number for physical slot in chassis. Reported in
     * list-mode data for purposes of event building. Limited to 0..15.
     */
    SlotID,
    /**
     * @brief [INPUT] Sets the number of ADC samples
     * (2<sup>SLOWFILTERRANGE</sup>) to be averaged before entering the energy
     * filtering logic.
     */
    SlowFilterRange,
    /**
     * @brief [OUTPUT] This variable is used to indicate whether the
     * synchronization interrupt has successfully occurred in the DSP when a
     * run start request was issued with SYNCHWAIT=1. If the DSP is stuck in
     * an infinite loop caused by a malfunctioning Busy-Synch loop, indicated
     * by the value of SynchDone being always 0, the module should be rebooted
     * and then the reason which caused the malfunctioning Busy-Synch loop
     * should be investigated.
     */
    SynchDone,
    /**
     * @brief [INPUT] Controls run start behavior.
     *
     * When set to 0 the module simply starts or resumes a run in response to the
     * corresponding host request. When set to 1, modules will run synchronously
     * through the backplane. This will ensure that the last module ready to actually
     * begin data taking will start the run in all modules and the first module to
     * end the run will stop the run in all modules. This way it never happens
     * that a multi-Pixie system is only partially active.
     */
    SynchWait,
    /**
     * @brief [INPUT] Four bit pattern words used to configure various trigger
     * options. See user manual for details.
     */
    TrigConfig,
    /**
     * @brief [INPUT] **Not implemented. Reserved.**
     */
    U00,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    U20,
    /**
     * @brief [INPUT] A block of 16 input variables used by user-written DSP
     * code.
     */
    UserIn,
    /**
     * @brief [OUTPUT] 16 words of user output data, which may be used by the
     * user written DSP code.
     */
    UserOut,
    /**
     * @brief Size marker
     */
    END
};

/**
 * @brief Defines Channel variables that are defined within the DSP VAR file.
 *
 * @note Order of entries in this enum must match with the definitions in
 * `channel_vars` and `channel_var_descriptors_default`.
 */
enum struct channel_var {
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    AutoTau,
    /**
     * @brief [INPUT] This variable sets the DC-offset level in terms of the
     * percentage of the ADC range. The DSP uses this variable to set the
     * DC-offset level when it is executing the ADJUST OFFSET control task.
     */
    BaselinePercent,
    /**
     * @brief [INPUT] This variable sets the cutoff value for baselines in
     * baseline measurements. If BLCUT is not set to zero, the DSP checks
     * continuously each baseline value to see if it is outside the limit
     * set by BLCUT. If the baseline value is within the limit, it will be
     * used to calculate the average baseline value. Otherwise, it will be
     * discarded. Set BLCUT to zero to not check baselines, therefore reduce
     * processing time.
     *
     * Control Task @ref xia::pixie::hw::run::control_task::get_baselines can
     * be used to measure baselines. Host computer can then histogram these
     * baseline values and determine the appropriate value for BLCUT for each
     * channel according to the standard deviation SIGMA for the averaged
     * baseline value. BLCUT could be set to be three times SIGMA.
     */
    BLcut,
    /**
     * @brief [INPUT] The CFD algorithm builds the difference of original and
     * delayed/scaled ADC signal. This variable defines the delay. See user
     * manual for details.
     */
    CFDDelay,
    /**
     * @brief [INPUT] The CFD algorithm builds the difference of original and
     * delayed/scaled ADC signal. This variable defines the scale. See user
     * manual for details.
     */
    CFDScale,
    /**
     * @brief [INPUT] This sets the threshold of the constant fraction
     * discriminator (CFD) trigger that is implemented in the trigger/filter
     * FPGA.
     */
    CFDThresh,
    /**
     * @brief [INPUT] The control and status register bits switch on/off
     * various aspects of the PIXIE-16 operation.
     *
     * | Bit | Description |
     * |---|---|
     * | 0 | Fast trigger selection<br>1: select external fast trigger<br>0: select local fast trigger |
     * | 1 | Module validation signal selection<br>1: select module gate signal<br>0: select global validation signal |
     * | 2 | Good channel. Only channels marked as good will contribute to spectra and list mode data. |
     * | 3 | Channel validation signal selection<br>1: select channel gate signal<br>0: select channel validation signal |
     * | 4 | Block data acquisition if trace or header DPMs are full<br>1: enable<br>0: disable. |
     * | 5 | Trigger positive. Set this bit to trigger on a positive slope; clear it for triggering on a negative slope. The trigger/filter FPGA can only handle positive signals. The PIXIE-16 handles negative signals by inverting them immediately after entering the FPGA. |
     * | 6 | Veto channel trigger<br>1: enable<br>0: disable |
     * | 7 | **Not implemented.** |
     * | 8 | Enable trace capture. <br>Set to 1 to enable trace capture for this channel. <br>Set to 0 to disable trace capture. |
     * | 9 | Enable QDC sums capture. <br>Set to 1 to enable QDC sums capture for this channel. <br>Set to 0 to disable QDC sums capture. |
     * | 10 | Enable CFD trigger mode. <br>Set to 1 to enable CFD trigger mode for this channel. <br>Set to 0 to disable CFD trigger. |
     * | 11 | Enable the requirement for module validation trigger. <br>Set to 1 to require module validation trigger for events validation for this channel. <br>Set to 0 to disable the requirement for module validation trigger. |
     * | 12 | Enable capture raw energy sums and baselines. <br>Set to 1 to store raw energy sums and baselines for events captured in this channel. <br>Set to 0 to not capture raw energy sums and baselines. |
     * | 13 | Enable the requirement for channel validation trigger. <br>Set to 1 to require channel validation trigger for events validation for this channel. <br>Set to 0 to disable the requirement for channel validation trigger. |
     * | 14 | Enable input relay. This bit controls the ON or OFF position switching of the input relay of each channel of the Pixie-16, resulting in two discrete fixed gains for the input signal: one high and one low. The actual gain value depends on the input design of each particular Pixie-16 hardware variant. |
     * | 15,16 | Pileup rejection control. <table><tr><th>Bit 15</th><th>Bit 16</th><th>Description</th></tr><tr><td>0</td><td>0</td><td>record all events (trace, timestamps, etc., but no energy for piled-up events)</td></tr><tr><td>0</td><td>1</td><td>only record single events (trace, energy, timestamps, etc.) (i.e., reject piled-up events)</td></tr><tr><td>1</td><td>0</td><td>record trace, timestamps, etc. for piled-up events but do not record trace for single events</td></tr><tr><td>1</td><td>1</td><td>only record trace, timestamps, etc., for piled-up events (i.e., reject single events)</td></tr></table> |
     * | 17 | Enable "no trace for large pulses" feature<br>1: enable<br>0: disable |
     * | 18 | Group trigger selection<br>1: external group trigger<br>0: local fast trigger. |
     * | 19 | Channel veto selection<br>1: channel validation trigger<br>0: front panel channel veto. |
     * | 20 | Module veto selection<br>1: module validation trigger<br>0: front panel module veto. |
     * | 21 | External timestamps in event heade<br>1: enable<br>0: disable. |
     */
    ChanCSRa,
    /**
     * @brief [INPUT] **Not implemented. Reserved.**
     */
    ChanCSRb,
    /**
     * @brief [OUTPUT] The upper 32-bits of the number of events processed by
     * the DSP.
     */
    ChanEventsA,
    /**
     * @brief [OUTPUT] The lower 32-bits of the number of events processed by
     * the DSP. Calculated with
     *
     * TotalEvents = ChanEventsA * 2<sup>32</sup> + ChanEventsB
     */
    ChanEventsB,
    /**
     * @brief [INPUT] The “channel validation trigger” from the system FPGA is
     * extended by this value (in clock cycles). See section 3.3.10 in the
     * user manual.
     */
    ChanTrigStretch,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    DigGain,
    /**
     * @brief [INPUT] An energy cutoff value which is used to determine
     * whether to record trace in list mode data runs, i.e. the "no trace for
     * large pulses" feature: 1) if the computed energy is above EnergyLow (or
     * EMIN as described in section 4.2.4), trace will not be recorded; 2) if
     * the computed energy is equal to or smaller than EnergyLow, trace will
     * be recorded. This feature is only effective if bit 17 of ChanCSRA is
     * set to 1. In other words, if bit 17 of ChanCSRA is set to 0, the value
     * of EnergyLow has no effect on trace recording.
     */
    EnergyLow,
    /**
     * @brief [INPUT] This parameter is used to delay the incoming ADC waveform
     * and the local fast trigger in order to compensate for the delayed
     * arrival of the external trigger pulses, e.g., module validation trigger,
     * channel validation trigger, etc.
     */
    ExternDelayLen,
    /**
     * @brief [INPUT] This parameter is used to stretch the module validation
     * trigger pulse. Only relevant when module validation is required by
     * setting ChanCSRA bit 11 CCSRA_GLOBTRIG.
     */
    ExtTrigStretch,
    /**
     * @brief [INPUT] Used to calculate the flat-top of the trigger filter.
     * @remark FastLength + Fast Gap < 127
     *
     * | ADC Freq. (MHz) | Rise Time calculation |
     * |---|---|
     * | 100/500 | FastGap * 2<sup>FastFilterRange</sup> * 10 ns |
     * | 250 | FastGap * 2<sup>FastFilterRange</sup> * 8 ns |
     */
    FastGap,
    /**
     * @brief [INPUT] Used to calculate the rise-time of the trigger filter.
     * @remark Must be greater than 2.
     * @remark FastLength + Fast Gap < 127
     *
     * | ADC Freq. (MHz) | Rise Time calculation |
     * |---|---|
     * | 100/500 | FastLength * 2<sup>FastFilterRange</sup> * 10 ns |
     * | 250 | FastLength * 2<sup>FastFilterRange</sup> * 8 ns |
     */
    FastLength,
    /**
     * @brief [OUTPUT] The upper 32-bits of the number of fast filter triggers.
     */
    FastPeaksA,
    /**
     * @brief [OUTPUT] The lower 32-bits of the number of fast filter triggers.
     * Calculated with
     *
     * NumEvents = FastPeaksA * 2<sup>32</sup> + FastPeaksB
     */
    FastPeaksB,
    /**
     * @brief [INPUT] This is the trigger threshold used by the trigger/filter
     * FPGA. The value relates to a trigger threshold through the formula:
     *
     * FASTTHRESH = TriggerThreshold * FASTLENGTH
     *
     * @remark FASTTHRESH < 65535
     */
    FastThresh,
    /**
     * @brief [INPUT] Length of trigger for coincidence logic in system FPGA.
     * See user manual section 3.3.10 for details.
     */
    FastTrigBackLen,
    /**
     * @brief [INPUT] Delay for sending trigger to system FPGA for coincidence
     * logic. See user manual section 3.3.10 for details.
     */
    FtrigoutDelay,
    /**
     * @brief [INPUT] **Not implemented. Reserved.**
     */
    GainDAC,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    InSpecA,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    InSpecB,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    Integrator,
    /**
     * @brief [INPUT] The PIXIE measures baselines continuously and effectively
     * extracts DC-offsets from these measurements. The DC-offset value is
     * needed to apply a correction to the computed energies. To reduce the
     * noise contribution from this correction baseline samples are averaged
     * in a geometric weight scheme. The averaging depends on LOG2BWEIGHT:
     *
     * DC<sub>avg</sub> = DC<sub>avg</sub> + (DC - DC<sub>avg</sub>) * 2<sup>LOG2BWEIGHT</sup>
     *
     * DC is the latest measurement and DC<sub>avg</sub> is the average that is
     * continuously being updated. At the beginning, and at the resuming, of a
     * run, DC<sub>avg</sub> is seeded with the first available DC measurement. As
     * before, the DSP ensures that LOG2BWEIGHT will be negative. The noise
     * contribution from the DC-offset correction falls with increased
     * averaging. The standard deviation of DC<sub>avg</sub> falls in proportion to
     * &radic;(2<sup>LOG2BWEIGHT</sup>).
     *
     * When using a BLCUT value from a noise measurement
     * (@ref xia::pixie::hw::run::control_task::get_baselines) the PIXIE will
     * internally adjust the effective LOG2BWEIGHT for best energy resolution,
     * up to the maximum value given by LOG2BWEIGHT. Hence, the Log2Bweight
     * setting should be chosen at low count rates (dead time < 10%). Best
     * energy resolutions are typically obtained at values of -3 to -4, and
     * this parameter does not need to be adjusted afterwards.
     */
    Log2Bweight,
    /**
     * @brief [INPUT] Controls the binning of the histogram. Energy values are
     * calculated to 16 bits precision. The LSB corresponds to 1/16th of a
     * 12-bit ADC. The PIXIEs, however, do not have enough histogram memory
     * available to record 64K spectra, nor would this always be desirable.
     * The user is therefore free to control the binning. Observe the
     * following formula to find to which MCA bin a value of Energy will
     * contribute:
     *
     * MCAbin = Energy * 2Log2Ebin
     *
     * As can be seen, Log2Ebin should be a negative number to achieve the
     * correct behaviour. At run start the DSP program ensures that Log2Ebin
     * is indeed negative by replacing the stored value by -abs(Log2Ebin).
     */
    Log2Ebin,
    /**
     * @brief [OUTPUT] Upper 32-bits of the channel's live time as measured by
     * the trigger/filter FPGA of that channel. It excludes times during which
     * the input signal is out of the ADC’s voltage range, or when the run
     * was stopped.
     */
    LiveTimeA,
    /**
     * @brief [OUTPUT] Lower 32-bits of the channel's live time as measured by
     * the trigger/filter FPGA of that channel. It excludes times during which
     * the input signal is out of the ADC’s voltage range, or when the run
     * was stopped.
     * | ADC Freq. (MHz) | Live Time calculation |
     * |---|---|
     * | 100/500 | (LiveTimeA * 2<sup>FastFilterRange</sup> + LiveTimeB) * 10 ns |
     * | 250 | (LiveTimeA * 2<sup>FastFilterRange</sup> + LiveTimeB) * 8 ns |
     */
    LiveTimeB,
    /**
     * @brief [INPUT] Upper 32-bits of the bit patterns controlling the
     * coincidence logic. See user manual for details.
     */
    MultiplicityMaskH,
    /**
     * @brief [INPUT] Lower 32-bits of the bit patterns controlling the
     * coincidence logic. See user manual for details.
     */
    MultiplicityMaskL,
    /**
     * @brief [INPUT] This DAC determines the DC-offset voltage. The offset can
     * be calculated using the following formula: <br>
     * Offset [V] = 1.5 * ((OffsetDAC – 32768) / 32768)
     */
    OffsetDAC,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    OverflowA,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    OverflowB,
    /**
     * @brief [INPUT] Legacy parameter used by obsolete designs.
     * @note Do not set manually, it is computed by the DSP and/or the SDK
     * from the filter settings.
     */
    PAFlength,
    /**
     * @brief [OUTPUT] This variable determines at what time the value from the
     * energy filter will be sampled. Its value is set according to the value
     * of SlowFilterRange. If the sampling point is chosen poorly, the
     * resulting spectrum will show energy resolutions of 10% and wider rather
     * than the expected fraction of a percent. For some parameter
     * combinations PeakSample needs to be varied by one or two units in
     * either direction, due to the pipelined architecture of the
     * trigger/filter FPGA.
     *
     * | SlowFilterRange | PeakSample |
     * |---|---|
     * | 1 | SlowLength + Slow Gap - 3 |
     * | 2 | SlowLength + Slow Gap - 2 |
     * | 3 | SlowLength + Slow Gap - 2 |
     * | 4 | SlowLength + Slow Gap - 1 |
     * | 5 | SlowLength + Slow Gap |
     * | 6 | SlowLength + Slow Gap + 1 |
     *
     * @note Do not set manually, it is computed by the DSP and/or the SDK
     * from the filter settings.
     */
    PeakSample,
    /**
     * @brief [INPUT] This value governs the minimum time separation between
     * two pulses. Two pulses that arrive within a time span shorter than
     * determined by PeakSep will be rejected as piled up. The SDK sets this
     * value to be SlowLength + SlowGap.
     *
     * @note Do not set manually, it is computed by the DSP and/or the SDK
     * from the filter settings.
     */
    PeakSep,
    /**
     * @brief [INPUT] Preamplifier exponential decay time. This variable is
     * used to store the preamplifier decay time. It is stored in the DSP as a
     * 32-bit floating point number (IEEE standard floating point format). The
     * DSP uses this variable to compute coefficients for the event energy
     * calculations.
     */
    PreampTau,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    PSAlength,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    PSAoffset,
    /**
     * @brief [INPUT] Length of the QDC sums.
     */
    QDCLen0,
    /**
     * @brief [INPUT] Length of the QDC sums.
     */
    QDCLen1,
    /**
     * @brief [INPUT] Length of the QDC sums.
     */
    QDCLen2,
    /**
     * @brief [INPUT] Length of the QDC sums.
     */
    QDCLen3,
    /**
     * @brief [INPUT] Length of the QDC sums.
     */
    QDCLen4,
    /**
     * @brief [INPUT] Length of the QDC sums.
     */
    QDCLen5,
    /**
     * @brief [INPUT] Length of the QDC sums.
     */
    QDCLen6,
    /**
     * @brief [INPUT] Length of the QDC sums.
     */
    QDCLen7,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    ResetDelay,
    /**
     * @brief [INPUT] Used to calculate the flat-top of the energy filter.
     * @remark Must be greater than 2.
     * @remark SlowLength + Slow Gap < 127
     *
     * | ADC Freq. (MHz) | Rise Time calculation |
     * |---|---|
     * | 100/500 | SlowGap * 2<sup>SlowFilterRange</sup> * 10 ns |
     * | 250 | SlowGap * 2<sup>SlowFilterRange</sup> * 8 ns |
     */
    SlowGap,
    /**
     * @brief [INPUT] Used to calculate the energy filter rise-time.
     * @remark Must be greater than 2.
     * @remark SlowLength + Slow Gap < 127
     *
     * | ADC Freq. (MHz) | Rise Time calculation |
     * |---|---|
     * | 100/500 | SlowLength * 2<sup>SlowFilterRange</sup> * 10 ns |
     * | 250 | SlowLength * 2<sup>SlowFilterRange</sup> * 8 ns |
     */
    SlowLength,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    ThreshWidth,
    /**
     * @brief [INPUT] This tells the DSP how many words of trace data to read
     * for each event. The action taken depends on FIFOlength, whose value
     * depends on hardware variants and specific firmware implementations.
     * If TraceLength < FIFOlength, the DSP will read from the FIFO. In that
     * case individual samples are either 10 ns (100 MHz or 500 MHz modules)
     * or 8 ns (250 MHz) apart. If FIFOlength <= TraceLength, the SDK
     * will force the TraceLength to be equal to FIFOlength.
     */
    TraceLength,
    /**
     * @brief [INPUT] Legacy parameter used by obsolete designs.
     * @note Do not set manually, it is computed by the DSP and/or the SDK
     * from the filter settings.     */
    TriggerDelay,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    TrigOutLen,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    U30,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    UnderflowA,
    /**
     * @brief [OUTPUT] **Not implemented.**
     */
    UnderflowB,
    /**
     * @brief [INPUT] **Not implemented.**
     */
    Xavg,
    /**
     * @brief [INPUT] This parameter controls the number of clock cycles
     * between untriggered ADC traces in control run with
     * @ref xia::pixie::hw::run::control_task::get_traces.
     * The time between recorded samples is &Delta;T = XWAIT*10ns.
     */
    Xwait,
    /**
     * @brief [INPUT] This parameter is used to stretch the veto pulse for
     * this channel.
     */
    VetoStretch,
    /**
     * @brief Size marker
     */
    END
};

/**
 * @brief defines the Variable's input/output mode.
 */
enum rwrowr { rw, ro, wr };

/**
 * @brief Variable enabled or disabled
 */
enum enabledisable { enable, disable };

/**
 * @brief Variable addressing defines what chip this variable is associated with.
 */
enum addressing { dsp_reg, fpga_reg, composite };

/*
 * Value type.
 */
typedef uint32_t value_type;

/*
 * Values.
 */
typedef std::vector<value_type> values;

/**
 * @brief A data structure describing information about Parameters.
 */
template<typename P>
struct parameter_desc {
    const P par; /*!< Parameter (index) */
    const rwrowr mode; /*!< In/out of the variable */
    const size_t size; /*!< Number of DSP words it covers */
    enabledisable state; /*!< Variable's state */
    const std::string name; /*!< Name of the variable */
    parameter_desc(const P par_, enabledisable state_, const rwrowr mode_, const size_t size_,
                   const std::string name_)
        : par(par_), mode(mode_), size(size_), state(state_), name(name_) {}
    bool writeable() const {
        return state == enable && mode != ro;
    }
};

/**
 * @brief A data structure describing information about a Variable.
 */
template<typename V>
struct variable_desc : public parameter_desc<V> {
    hw::address address; /*!< DSP memory address */
    variable_desc(const V var_, enabledisable state_, const rwrowr mode_, const size_t size_,
                  const std::string name_)
        : parameter_desc<V>(var_, state_, mode_, size_, name_), address(0) {}
};

/*
 * Parameter descriptor sets for the system, modules and channels
 */
typedef parameter_desc<system_param> system_param_desc;
typedef std::vector<system_param_desc> system_param_descs;
typedef parameter_desc<module_param> module_param_desc;
typedef std::vector<module_param_desc> module_param_descs;
typedef parameter_desc<channel_param> channel_param_desc;
typedef std::vector<channel_param_desc> channel_param_descs;

/*
 * Variable descriptor sets for modules and channels
 */
typedef variable_desc<module_var> module_var_desc;
typedef std::vector<module_var_desc> module_var_descs;
typedef variable_desc<channel_var> channel_var_desc;
typedef std::vector<channel_var_desc> channel_var_descs;

/**
 * @brief A variable is an object that combines descriptors with values.
 */
template<typename Vdesc>
struct variable {
    /**
     * @brief Structure to describe the data associated with a descriptor.
     */
    struct data {
        bool dirty; /*!< Written to hardware? */
        value_type value;
        data() : dirty(false), value(0) {}
    };
    const Vdesc& var; /*!< The variable descriptor */
    std::vector<data> value; /*!< The value(s) */

    variable(const Vdesc& var_) : var(var_) {
        value.resize(var.size);
    }
};

/*
 * Module variables
 */
typedef variable<module_var_desc> module_variable;
typedef std::vector<module_variable> module_variables;

/*
 * Channel variables
 */
typedef variable<channel_var_desc> channel_variable;
typedef std::vector<channel_variable> channel_variables;
typedef std::vector<channel_variables> channels_variables;

/**
 * @brief Copies filter variables from one channel to another. Only used with channel objects.
 */
template<typename V>
struct copy_filter_var {
    V var;
    uint32_t mask;

    copy_filter_var(V var_, uint32_t mask_ = UINT32_MAX) : var(var_), mask(mask_) {}
};

typedef std::vector<copy_filter_var<channel_var>> copy_filter;

/*
 * Copy filter masks. The masks select the filters used in a copy.
 */
const unsigned int energy_mask = 1 << 0;
const unsigned int trigger_mask = 1 << 1;
const unsigned int analog_signal_cond_mask = 1 << 2;
const unsigned int histogram_control_mask = 1 << 3;
const unsigned int decay_time_mask = 1 << 4;
const unsigned int pulse_shape_analysis_mask = 1 << 5;
const unsigned int baseline_control_mask = 1 << 6;
const unsigned int channel_csra_mask = 1 << 7;
const unsigned int cfd_trigger_mask = 1 << 8;
const unsigned int trigger_stretch_len_mask = 1 << 9;
const unsigned int fifo_delays_mask = 1 << 10;
const unsigned int multiplicity_mask = 1 << 11;
const unsigned int qdc_mask = 1 << 12;
const unsigned int all_mask = (1 << 12) - 1;

/*
 * Look up maps. A fast way to map a name to a parameter or variable.
 */
typedef std::map<std::string, system_param> system_param_map;
typedef std::map<std::string, module_param> module_param_map;
typedef std::map<std::string, channel_param> channel_param_map;
typedef std::map<std::string, module_var> module_var_map;
typedef std::map<std::string, channel_var> channel_var_map;

/**
 * @brief Defines an Address map that can be used to parse binary data blobs.
 *
 * This structure is typically used to parse Statistics or configuration data from the DSP.
 */
struct address_map {
    typedef std::pair<size_t, hw::address> desc_address;
    typedef std::vector<desc_address> desc_addresses;
    /**
     * @brief Data structure for working with address ranges
     */
    struct range {
        hw::address start;
        hw::address end;
        size_t size;
        range();
        void set_size();
        void output(std::ostream& out) const;
    };

    range full;

    range module;
    range module_in;
    range module_out;

    range channels;
    range channels_in;
    range channels_out;

    size_t vars;
    size_t module_vars;
    size_t channel_vars;
    size_t vars_per_channel;

    address_map();

    void set(const size_t num_channels, const module_var_descs& module_descs,
             const channel_var_descs& channel_descs);
    hw::address channel_base(const size_t channel);

    void output(std::ostream& out) const;

private:
    void check_channel_gap(const size_t max_channels, const channel_var_descs& channel_descs,
                           const desc_addresses& addresses);

    template<typename V>
    void get(const V& vars, desc_addresses& addresses, const rwrowr mode);
    hw::address min(const desc_addresses& addresses);
    hw::address max(const desc_addresses& addresses);
};

/**
 * @brief Get a descriptor from the descriptors by its variable name.
 */
template<typename D, typename V>
const typename D::value_type& get_descriptor(const D& descs, const V var) {
    return descs[size_t(var)];
}

/*
 * Get a copy of the defaults.
 */
const module_var_descs& get_module_var_descriptors();
const channel_var_descs& get_channel_var_descriptors();

/*
 * Get the maps
 */
const system_param_map get_system_param_map();
const module_param_map get_module_param_map();
const channel_param_map get_channel_param_map();

/*
 * Check is the parameter or variable is valid?
 */
bool is_system_param(const std::string& label);
bool is_module_param(const std::string& label);
bool is_channel_param(const std::string& label);
bool is_module_var(const std::string& label);
bool is_channel_var(const std::string& label);

/*
 * Look up parameters and variables.
 */
system_param lookup_system_param(const std::string& label);
module_param lookup_module_param(const std::string& label);
channel_param lookup_channel_param(const std::string& label);
module_var lookup_module_var(const std::string& label);
channel_var lookup_channel_var(const std::string& label);

/**
 * @brief Maps a Module parameter to module variable.
 */
module_var map_module_param(module_param par);

/*
 * Load the variables from the DSP variable file into the
 * descriptors.
 */
void load(const std::string& dspvarfile, module_var_descs& module_var_descriptors,
          channel_var_descs& channel_var_descriptors);
void load(firmware::firmware_ref& dspvarfw, module_var_descs& module_var_descriptors,
          channel_var_descs& channel_var_descriptors);
void load(std::istream& input, module_var_descs& module_var_descriptors,
          channel_var_descs& channel_var_descriptors);

/**
 * @brief Copy the variables based on the filter.
 */
void copy_parameters(const copy_filter& filter, const channel_variables& source,
                     channel_variables& dest);

/**
 * @brief Copy the variables based on the filter mask.
 */
void copy_parameters(const unsigned int filter_mask, const channel_variables& source,
                     channel_variables& dest);

};  // namespace param
};  // namespace pixie
};  // namespace xia

std::ostream& operator<<(std::ostream& out, const xia::pixie::param::address_map& config);

#endif  // PIXIE_PARAM_H
