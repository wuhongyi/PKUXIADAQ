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

/** @file hbr.cpp
 * @brief Implements how we handle the Pixie-16 HBR.
 */

#include <pixie/pixie16/hbr.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
namespace hw {
namespace hbr {
/*
 * HBR request, DSP access (HBR and CS bits 0)
 */
static const hw::word REQUEST = (1 << hw::bit::HBR_READ_AGAIN_READ) | (1 << hw::bit::HBR_AUTO_INC);
/*
 * FPGA HBR request, DSP access (HBR and CS bits 0)
 */
static const hw::word FPGA_REQUEST =
    (1 << hw::bit::HBR_READ_AGAIN_READ) | (1 << hw::bit::HBR_AUTO_INC) | (1 << hw::bit::HBR_DSP_nCS);
/*
 * HBR release (default, bus released)
 */
static const hw::word RELEASE =
    (1 << hw::bit::HBR_READ_AGAIN_READ) | (1 << hw::bit::HBR_AUTO_INC) | (1 << hw::bit::HDR_HBR);

const host_bus_access dsp_access(REQUEST, RELEASE);;
const host_bus_access fpga_access(FPGA_REQUEST, RELEASE);

host_bus_request::host_bus_request(module::module& module_, bool hold,
                                   host_bus_access access_)
    : module(module_), access(access_), holding(false) {
    if (hold) {
        request();
    }
}

host_bus_request::host_bus_request(module::module& module_,
                                   host_bus_access access_)
    : module(module_), access(access_), holding(false) {
    request();
}

host_bus_request::~host_bus_request() {
    release();
}

void host_bus_request::request(bool force) {
    if (force || !holding) {
        module.write_word(hw::device::REQUEST_HBR, access.request);
        holding = true;
    }
}

void host_bus_request::release(bool force) {
    if (force || holding) {
        module.write_word(hw::device::HBR_DONE, access.release);
        holding = false;
    }
}
}  // namespace hbr
}  // namespace hw
}  // namespace pixie
}  // namespace xia
