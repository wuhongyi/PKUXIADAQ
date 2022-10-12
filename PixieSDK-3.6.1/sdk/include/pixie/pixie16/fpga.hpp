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

/** @file fpga.hpp
 * @brief Defines common data structures for the FPGAs on the Pixie-16 modules.
 */

#ifndef PIXIE_HW_FPGA_H
#define PIXIE_HW_FPGA_H

#include <cstdint>

#include <pixie/fw.hpp>

#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
namespace module {
class module;
}
namespace hw {
/**
 * @brief General namespace for working with Pixie-16 FPGAs
 */
namespace fpga {
/**
 * @brief Defines an object for working with the on-board FPGAs.
 *
 * This class contains most of the common control tasks that we use
 * to read and write data to the FPGAs.
 */
struct control {
    /**
     * @brief The actual control commands.
     */
    struct controls {
        const uint32_t clear;
        const uint32_t set;
        const uint32_t done;

        controls(const uint32_t clear, const uint32_t set, const uint32_t done);
    };

    /**
     * @brief The memory registers that we'll be interacting with.
     */
    struct regs {
        const int DATACS;
        const int CTRLCS;
        const int RDCS;

        regs(const int DATACS, const int CTRLCS, const int RDCS);
    };

    module::module& module;
    const std::string name;
    const controls load_ctrl;
    const controls clear_ctrl;
    const regs reg;

    /**
     * Diagnostic Trace of the load operation
     */
    const bool trace;

    control(module::module& module, const std::string name, const controls& load_ctrl,
            const controls& clear_ctrl, const regs& reg, bool trace = false);

    /**
     * @brief Loads the firmware image onto the FPGA chip.
     * @param image The image data that we'd like to load to the hardware.
     * @param retries The number of times to retry if we encounter an issue.
     */
    void load(const firmware::image& image, int retries = 10);

    /**
     * @brief Checks if the FPGA has firmware and is ready for operation.
     * @return True if the FPGA reports that it's ready.
     */
    bool done();

    /**
     * @brief Provides low-level register write access to the FPGA
     * @param[in] reg The register that you'd like to access
     * @param[in] data The data that you'd like to write to the register
     */
    void bus_write(int reg, uint32_t data);
    /**
     * @brief Provides low-level register read access to the FPGA
     * @param reg The register that you'd like to access on the FPGA.
     * @return The value that was contained within the requested register.
     */
    uint32_t bus_read(int reg);

private:
    std::string make_what(const char* msg);
};
}  // namespace fpga
}  // namespace hw
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_HW_FPGA_H
