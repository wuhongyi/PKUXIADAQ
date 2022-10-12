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

/** @file run.cpp
 * @brief Implements run control enumerators and functions for the Pixie-16 hardware.
 */

#include <pixie/log.hpp>
#include <pixie/util.hpp>

#include <pixie/pixie16/csr.hpp>
#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/module.hpp>
#include <pixie/pixie16/run.hpp>

namespace xia {
namespace pixie {
namespace hw {
namespace run {

/**
 * The default module run configuration
 */
static module_config run_config_default = {
    true,  /* dsp_sets_dacs */
    true,  /* dsp_get_traces */
    true,  /* dsp_ramp_offsetdac */
    true,  /* dsp_adjust_offsetdacs */
    true   /* dsp_tau_finder */
};

/**
 * The AFE DB module run configuration
 */
static module_config run_config_afe_dbs = {
    false,  /* dsp_sets_dacs */
    false,  /* dsp_get_traces */
    false,  /* dsp_ramp_offsetdac */
    false,  /* dsp_adjust_offsetdacs */
    true    /* dsp_tau_finder */
};

module_config make(module::module& module) {
    switch (module.get_rev_tag()) {
    case hw::rev_H:
        return run_config_afe_dbs;
    default:
        break;
    }
    return run_config_default;
}

static const char* control_task_labels(control_task control_tsk) {
    switch (control_tsk) {
        case control_task::set_dacs:
            return "set_dacs";
        case control_task::enable_input:
            return "enable_input";
        case control_task::ramp_offsetdacs:
            return "ramp_offsetdacs";
        case control_task::get_traces:
            return "get_traces";
        case control_task::program_fippi:
            return "program_fippi";
        case control_task::get_baselines:
            return "get_baselines";
        case control_task::adjust_offsets:
            return "adjust_offsets";
        case control_task::tau_finder:
            return "tau_finder";
        case control_task::reset_adc:
            return "reset_adc";
        case control_task::fill_ext_fifo:
            return "fill_ext_fifo";
        default:
            break;
    }
    return "nop";
};

static const char* run_mode_labels[] = {"resume", "new_run"};

static const char* run_task_labels(run_task run_tsk) {
    switch (run_tsk) {
        case run_task::list_mode:
            return "list_mode";
        case run_task::histogram:
            return "histogram";
        default:
            break;
    }
    return "nop";
}

static bool run_ended(module::module& module) {
    /*
     * End the run if:
     *  1. The run task is histogram or list mode, and
     *  2. The module is not the leader
     */
    bool valid_task =
        module.run_task == run_task::histogram || module.run_task == run_task::list_mode;
    return valid_task && module.backplane.run.not_leader(module);
}

void start(module::module& module, run_mode mode, run_task run_tsk, control_task control_tsk) {
    xia_log(log::debug) << module::module_label(module, "run") << "start: run-mode=" << int(mode)
                        << " run-tsk=" << std::hex << int(run_tsk) << std::dec
                        << " control-tsk=" << int(control_tsk);

    end(module);

    if (mode == run_mode::new_run && run_tsk != run_task::nop) {
        static const size_t block_size = hw::large_histogram_length * 4;
        static const size_t mca_end = hw::large_histogram_length * module.num_channels;
        words zero(block_size);
        memory::mca mca(module);
        for (address addr = 0; addr < mca_end; addr += block_size) {
            mca.write(addr, zero);
        }
        module.run_task = run_tsk;
    } else {
        module.control_task = control_tsk;
    }

    module.write_var(param::module_var::RunTask, param::value_type(run_tsk));
    module.write_var(param::module_var::ControlTask, param::value_type(control_tsk));
    module.write_var(param::module_var::Resume, param::value_type(mode));

    module::module::bus_guard guard(module);
    csr::set(module, 1 << hw::bit::RUNENA);
}

void end(module::module& module) {
    if (active(module)) {
        xia_log(log::debug) << module::module_label(module, "run") << "ending";
        util::timepoint tp;
        int wait_msecs = 1000;
        int msecs = 0;
        module.run_task = run_task::run_stopping;
        tp.start();
        while (msecs < wait_msecs) {
            {
                module::module::bus_guard guard(module);
                csr::clear(module, 1 << hw::bit::RUNENA);
            }
            ++msecs;
            if (!hw::run::active(module) || run_ended(module)) {
                tp.end();
                xia_log(log::debug) << module::module_label(module, "run") << "ended, duration=" << tp;
                break;
            }
            hw::wait(1000);
        }
        if (msecs >= wait_msecs) {
            module.run_task = run_task::nop;
            module.control_task = control_task::nop;
            xia_log(log::error) << module::module_label(module, "run")
                                << "failed to end task; module reboot required";
            throw error(error::code::module_task_timeout,
                        "failed to end active run task; module reboot required");
        }
    }
    module.run_task = run_task::nop;
    module.control_task = control_task::nop;
}

bool active(module::module& module) {
    module::module::bus_guard guard(module);
    return (csr::read(module) & ((1 << hw::bit::RUNENA) | (1 << hw::bit::RUNACTIVE))) != 0;
}

static bool control_task_prerun(module::module& module, control_task control_tsk, int ) {
    switch (control_tsk) {
    case control_task::set_dacs:
        if (!module.run_config.dsp_sets_dacs) {
            module.fixtures->set_dacs();
            return false;
        }
        break;
    case control_task::ramp_offsetdacs:
        if (!module.run_config.dsp_ramp_offsetdacs) {
            throw error(error::code::module_invalid_operation, "not supported");
        }
        break;
    case control_task::get_traces:
        if (!module.run_config.dsp_get_traces) {
            module.fixtures->get_traces();
            return false;
        }
        break;
    case control_task::adjust_offsets:
        if (!module.run_config.dsp_adjust_offsetdacs) {
            module.fixtures->adjust_offsets();
            return false;
        }
        break;
    case control_task::tau_finder:
        if (!module.run_config.dsp_tau_finder) {
            module.fixtures->tau_finder();
            return false;
        }
    default:
        break;
    }
    return true;
}

static void control_task_postrun(module::module& module, control_task control_tsk, int ) {
    switch (control_tsk) {
    case control_task::tau_finder:
        /*
         * TODO:
         * Need to get the found TAUs (one for each channel) from DSP Memory address 0x4A43F,
         * store them somewhere, and then get that information back to the API.
         */
        break;
    default:
        break;
    }
}

void control_run_on_dsp(module::module& module, control_task control_tsk, int wait_msecs) {
    xia_log(log::debug) << module::module_label(module, "run on dsp")
                        << "control=" << control_task_labels(control_tsk) << " wait=" << wait_msecs;
    bool finished = false;
    start(module, run_mode::new_run, run_task::nop, control_tsk);
    for (int msecs = 0; !finished && msecs < wait_msecs; ++msecs) {
        if (active(module)) {
            hw::wait(1000);
        } else {
            finished = true;
        }
    }
    if (!finished) {
        std::ostringstream oss;
        oss << "control task failed to end: " << int(control_tsk);
        throw error(error::code::module_task_timeout, oss.str());
    }
}

void control(module::module& module, control_task control_tsk, int wait_msecs) {
    xia_log(log::debug) << module::module_label(module, "run")
                        << "control=" << control_task_labels(control_tsk) << " wait=" << wait_msecs;
    util::timepoint tp;
    tp.start();
    if (control_task_prerun(module, control_tsk, wait_msecs)) {
        control_run_on_dsp(module, control_tsk, wait_msecs);
    }
    control_task_postrun(module, control_tsk, wait_msecs);
    tp.end();
    xia_log(log::debug) << module::module_label(module, "control")
                        << "control=" << control_task_labels(control_tsk) << " duration=" << tp;
}

void run(module::module& module, run_mode mode, run_task run_tsk) {
    xia_log(log::debug) << module::module_label(module, "run") << "mode=" << run_mode_labels[int(mode)]
                        << " run=" << run_task_labels(run_tsk);
    start(module, mode, run_tsk, control_task::nop);
}
}  // namespace run
}  // namespace hw
}  // namespace pixie
}  // namespace xia
