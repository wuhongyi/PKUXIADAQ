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

/** @file dsp.hpp
 * @brief Defines functionality to use the DSP on the Pixie-16 modules.
 */

#ifndef PIXIE_HW_DSP_H
#define PIXIE_HW_DSP_H

#include <array>

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
 * @brief Defines functionality for working with Pixie-16's DSP chip.
 */
namespace dsp {
/**
 * @brief Defines an object for working with the DSP on-board the modules.
 */
struct dsp {
    /**
     * The word loaded into the DSP.
     */
    typedef uint16_t load_value_type;

    module::module& module;

    /**
     * Online?
     */
    bool online;

    /**
     * Diagnostic trace of the load operation
     */
    bool trace;

    dsp(module::module& module, bool trace = false);
    dsp& operator=(dsp&& d);

    /**
     * @brief Boots the DSP chip
     * @param image The firmware image to load to the chip
     * @param retries The number of times to try loading if we encounter an issue.
     */
    void boot(const firmware::image& image, int retries = 10);

    /**
     * @brief Checks if the DSP loaded and running
     * @return True if the PowerUpInitDone variable is set.
     */
    bool init_done();

private:
    /*
     * Checked write, writes and value then checks for a reply.
     */
    bool checked_write(const word out, const word value, const word in, const word result,
                       const int out_wait = 0, const int in_wait = 1000);

    /*
     * Image section loader.
     */
    void section_load(firmware::reader& reader, const size_t wordsize);

    /*
     * Low level access.
     */
    void bus_write(int reg, uint32_t data);
    uint32_t bus_read(int reg);

    std::string make_what(const char* msg);
};
}  // namespace dsp
}  // namespace hw
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_HW_DSP_H
