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

/** @file pcf8574.hpp
 * @brief Defines data structures to read the Pixie-16's PCF 8574 chip.
 */

#ifndef PIXIE_HW_PCF8574_H
#define PIXIE_HW_PCF8574_H

#include <pixie/pixie16/i2c_bitbash.hpp>

namespace xia {
namespace pixie {
namespace hw {
namespace i2c {
/**
 * @brief Defines communication with the PCF8574 chip.
 *
 * This chip contains information about the physical slot the module occupies.
 */
struct pcf8574 : public bitbash {
    pcf8574(module::module& module, int reg, uint32_t SDA, uint32_t SCL, uint32_t CTRL,
            bool trace = false);

    /**
     * @brief Read a byte.
     * @returns the byte that was read.
     */
    uint8_t read_a_byte();
};
}  // namespace i2c
}  // namespace hw
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_HW_PCF8574_H
