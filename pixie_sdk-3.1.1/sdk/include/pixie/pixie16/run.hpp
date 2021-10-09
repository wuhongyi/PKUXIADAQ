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

/** @file run.hpp
 * @brief Defines run control enumerators and functions for the Pixie-16 hardware.
 */

#ifndef PIXIE_HW_RUN_H
#define PIXIE_HW_RUN_H

#include <cstdint>

#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
namespace module {
class module;
}
namespace hw {
/**
 * @brief Everything you need to handle run control on a Pixie-16 module.
 */
namespace run {
/**
 * @brief Run modes used to start data runs.
 */
enum struct run_mode {
    new_run = 1, /*!< Starts a new data run, which clears hw memory. */
    resume = 0 /*!< Resumes a stopped data run leaving hw memory untouched. */
};

/**
 * @brief Run tasks used to collect data from the hardware.
 */
enum struct run_task {
    /**
     * @brief Control task data run.
     *
     * RunTask 0 is used to request slow control tasks. These include
     * programming the trigger/filter FPGAs, setting the DACs in the system,
     * transfers to/from the external memory, and calibration tasks.
     */
    nop = 0,
    /**
     * @brief List-mode data run.
     *
     * In this run type triggered waveforms together with time of arrival
     * (trigger time), event energy, and other event information are written
     * into the External FIFO for each channel and module. The raw data stream
     * is always sent to the intermediate buffer in the signal processing FPGAs.
     * The data-gathering routine in the DSP reads the raw data from the FPGAs,
     * computes the event energy, and then writes those data to the external
     * FIFO. If the intermediate buffer in the FPGA is full, newly arrived
     * events will be ignored until there is room again in the buffer.
     */
    list_mode = 0x100,
    /**
     * @brief Histogram data run.
     *
     * Similar to RunTask 0x100, the event raw data stream is always sent to the
     * intermediate buffer in the signal processing FPGAs. The data-gathering
     * routine in the DSP reads the raw data from the FPGAs, computes the event
     * energy, and then writes the energy value to the External Histogram
     * Memory. If the intermediate buffer in the FPGA is full, newly arrived
     * events will be ignored until there is room again in the buffer. This run
     * type does not write data to the External FIFO.
     */
    histogram = 0x301
};

/**
 * @brief The control tasks that we can request from the DSP.
 *
 * The DSP can execute a number of control tasks, which are necessary to control
 * hardware blocks that are not directly accessible from the host computer. The
 * most prominent tasks are those to set the DACs and program the trigger/filter
 * FPGAs. The following is a list of control tasks that will be of interest to
 * the programmer.

 * To start a control task, set RUNTASK=0 and choose a CONTROLTASK value from
 * the list below. Then start a run by setting bit 0 in the control and status
 * register (CSR).
 *
 * Control tasks respond within a few hundred nanoseconds by setting the
 * RUNACTIVE bit (#13) in the CSR. The host can poll the CSR and watch for the
 * RUNACTIVE bit to be deasserted. All control tasks indicate task completion
 * by clearing this bit.
 *
 * Execution times vary considerably from task to task, ranging from under a
 * microsecond to 10 seconds. Hence, polling the CSR is the most effective way
 * to check for completion of a control task.
 */
enum struct control_task {
    /**
     * @brief Write the OFFSETDAC values of all channels into the respective DACs.
     *
     * Reprogramming the DACs is required to make effective changes in the
     * values of the variables OFFSETDAC{0…15}.
     */
    set_dacs = 0,
    /**
     * @brief This control task is not implemented in the DSP code.
     */
    enable_input = 1,
    /**
     * @brief Ramp offset DACs of a module from 0 to 65535 at a step size of 64.
     *
     * Baseline values are captured at each offset DAC level, and a plot of
     * baseline values versus offset DAC levels can then be used to find the
     * appropriate offset DAC value for a given baseline level. The number of
     * baseline values for each channel that are available after calling this
     * control task is 1024. So for 16 channels, there will be a total of 16384
     * baseline values to read out from the DSP internal memory.
     */
    ramp_offsetdacs = 3,
    /**
     * @brief This task provides ADC values measured on all 16 channels.
     *
     * @see xia::pixie::module::read_adc
     *
     * It gives the user an idea of what the noise and the DC-levels in the
     * system are. This function samples 8192 ADC words (16-bit) for each of
     * the 16 channels. The XWAIT variable determines the time between
     * successive ADC samples (samples are XWAIT * 10ns apart). The results are
     * written to the 65536 words (32-bit) long I/O buffer. Use this function
     * to check if the offset adjustment was successful.
     */
    get_traces = 4,
    /**
     * @brief This task writes all relevant data to the FiPPI registers in the FPGA.
     *
     * For example, the fast filter rise time and flat top, etc. These data
     * have already been converted to FPGA recognizable values by the SDK
     * before they are downloaded to the DSP, e.g. slow filter rise time has
     * been converted from units of microseconds to units of FPGA clock cycles.
     */
    program_fippi = 5,
    /**
     * @brief This routine is used to collect baseline values.
     *
     * Currently, DSP collects three words, time stamp (low 32-bit word),
     * time stamp (high 16-bit word), and baseline value, for each baseline.
     * 3640 baselines are collected until the 65536-word (32-bit) I/O buffer is
     * almost completely filled. The host computer can then read the I/O buffer
     * and retrieve the baseline values. Please note, the baseline values
     * retrieved from the DSP are 32-bit IEEE floating point numbers. The SDK
     * automatically converts to decimal values when presenting the data to the
     * user.
     *
     * Baseline values can then be statistically analyzed to determine the
     * standard deviation associated with the averaged baseline value and to
     * set the BLCUT. BLCUT should be about 3 times the standard deviation.
     * Baseline values can also be plotted against time stamp to explore the
     * detector performance. BLCUT should be set to zero while running Control
     * Task 6.
     */
    get_baselines = 6,
    /**
     * @brief This task automatically adjusts the DC offsets of each of the 16
     * channels of a Pixie-16 module.
     *
     * It sets Offset DACs to appropriate values. The signal baseline levels
     * after the adjustment will be determined by DSP parameter BaselinePercent.
     * For instance, if the BaselinePercent is set to 10% on a Pixie-16 module
     * installed with 14-bit ADCs, the DC offset will be about 1640 after
     * successful DC offset adjustment.
     */
    adjust_offsets = 7,
    /**
     * @brief This task tries to find the single exponential decay time constant
     *          of the input analog signals of each channel.
     *
     * It uses the un-trigged ADC traces to find and fit the input pulses, so
     * reasonably high count rate for the input signals is required to find the
     * decay time successfully.
     */
    tau_finder = 8,
    /**
     * @brief Fills the external (list-mode) FIFO with data.
     */
    fill_ext_fifo = 11,
    /**
     * @brief This task resets the ADCs of a Pixie-16 module. Only used while booting.
     * @note Revision F only!
     */
    reset_adc = 23,
    nop = 100
};

/*
 * Run and control task management.
 */
void start(module::module& module, run_mode mode, run_task run_tsk, control_task control_tsk);
void end(module::module& module);
bool active(module::module& module);

/**
 * @brief Execute a control task in a specific module
 * @param module The module to work with
 * @param control_tsk The control task that we'll execute.
 * @param wait_msecs how long we'll wait for the task to finish. This is generally empirically
 *                    determined
 */
void control(module::module& module, control_task control_tsk, int wait_msecs = 10000);

/**
 * @brief Executes a run task on the requested module
 * @param module The module that we'll execute against.
 * @param mode Whether we're starting a new run or resuming one.
 * @param run_tsk The run task that we'd like to execute.
 */
void run(module::module& module, run_mode mode, run_task run_tsk);
}  // namespace run
}  // namespace hw
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_HW_RUN_H
