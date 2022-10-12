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

/** @file hbr.hpp
 * @brief Host Bus Request Control
 */

#ifndef PIXIE_HBR_H
#define PIXIE_HBR_H

#include <pixie/error.hpp>

#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
namespace module {
class module;
}
namespace hw {
/**
 * @brief Host Bus Request
 */
namespace hbr {
/**
 * @brief HBR bus access.
 *
 * The HBR request value can effect the type of access to the host bus.
 */
struct host_bus_access {
    hw::word request;
    hw::word release;
    host_bus_access(hw::word req, hw::word rel) : request(req), release(rel) {}
    host_bus_access(const host_bus_access& a) : request(a.request), release(a.release) {}
    host_bus_access& operator=(const host_bus_access& a) {
        request = a.request;  release = a.release;
        return *this;
    }
};
/**
 * @brief DSP HBR Access
 *
 * Use to access the DSP.
 */
extern const host_bus_access dsp_access;
/**
 * @brief FPGA HBR Access
 *
 * Use to access the DSP.
 */
extern const host_bus_access fpga_access;
/**
 * @brief Host Bus Request
 *
 * Control the host bus request to take control of the bus. This object
 * requests the bus when constructed and releases the bus when it desctructs.
 *
 * You can optionally construct an HBR without requesting the bus. The DSP
 * booting needs this.
 *
 * Requesting and hold bus effects a module's performance.
 */
struct host_bus_request {
    module::module& module;
    const host_bus_access access;
    bool holding;
    /**
     * @brief Construct the HBR and optionally request the bus.
     * @param module_ The module's HBR to control
     * @param hold Request and hold the bus when constructing, the default is true
     * @param access_ The HBR access, the default is dsp_hbr
     */
    host_bus_request(module::module& module_, bool hold = true,
                     const host_bus_access access_ = dsp_access);
    /**
     * @brief Construct the HBR and optionally request the bus.
     * @param module_ The module's HBR to control
     * @param access_ The HBR access, the default is dsp_hbr
     */
    host_bus_request(module::module& module_,
                     const host_bus_access access_ = dsp_access);
    /**
     * @brief Release the bus if held.
     */
    ~host_bus_request();
    /**
     * @brief Request the bus if not held. Optionally force the hold.
     * @param force If true the held state is ignored and write to hardware
     *              happens.
     */
    void request(bool force = false);
    /**
     * @brief Release the bus if held. Optionally force the release.
     * @param force If true the held state is ignored and write to hardware
     *              happens.
     */
    void release(bool force = false);
};
}  // namespace hbr
}  // namespace hw
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_HBR_H
