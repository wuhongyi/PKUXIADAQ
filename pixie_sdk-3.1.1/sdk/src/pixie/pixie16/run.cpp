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
void start(module::module& module, run_mode mode, run_task run_tsk, control_task control_tsk) {
    log(log::debug) << module::module_label(module, "run") << "start: run-mode=" << int(mode)
                    << " run-tsk=" << std::hex << int(run_tsk) << std::dec
                    << " control-tsk=" << int(control_tsk);

    end(module);

    if (mode == run_mode::new_run && run_tsk != run_task::nop) {
        static const size_t block_size = max_histogram_length * 4;
        static const size_t mca_end = max_histogram_length * module.num_channels;
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
    module.run_task = run_task::nop;
    module.control_task = control_task::nop;
    if (active(module)) {
        log(log::debug) << module::module_label(module, "run") << "end";
        util::timepoint tp;
        tp.start();
        {
            module::module::bus_guard guard(module);
            csr::clear(module, 1 << hw::bit::RUNENA);
        }
        int wait_msecs = 1000;
        for (int msecs = 0; msecs < wait_msecs; ++msecs) {
            if (!hw::run::active(module)) {
                tp.end();
                log(log::debug) << module::module_label(module, "run") << "end duration=" << tp;
                return;
            }
            hw::wait(1000);
        }
        throw error(error::code::module_task_timeout, "failed to stop task");
    }
}

bool active(module::module& module) {
    module::module::bus_guard guard(module);
    return (csr::read(module) & ((1 << hw::bit::RUNENA) | (1 << hw::bit::RUNACTIVE))) != 0;
}

void control(module::module& module, control_task control_tsk, int wait_msecs) {
    log(log::debug) << module::module_label(module, "run")
                    << "control=" << control_task_labels(control_tsk) << " wait=" << wait_msecs;
    util::timepoint tp;
    tp.start();
    start(module, run_mode::new_run, run_task::nop, control_tsk);
    for (int msecs = 0; msecs < wait_msecs; ++msecs) {
        if (!active(module)) {
            tp.end();
            log(log::debug) << module::module_label(module, "control")
                            << "control=" << control_task_labels(control_tsk) << " duration=" << tp;
            return;
        }
        hw::wait(1000);
    }
    std::ostringstream oss;
    oss << "control task failed to start: " << int(control_tsk);
    throw error(error::code::module_task_timeout, oss.str());
}

void run(module::module& module, run_mode mode, run_task run_tsk) {
    log(log::debug) << module::module_label(module, "run") << "mode=" << run_mode_labels[int(mode)]
                    << " run=" << run_task_labels(run_tsk);
    start(module, mode, run_tsk, control_task::nop);
}
}  // namespace run
}  // namespace hw
}  // namespace pixie
}  // namespace xia
