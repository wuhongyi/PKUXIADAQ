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

/** @file i2c_bitbash.cpp
 * @brief Implements I2C bit bash data structures and functions
 */

#include <iomanip>
#include <iostream>

#include <pixie/log.hpp>

#include <pixie/pixie16/i2c_bitbash.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
namespace hw {
namespace i2c {
bitbash::bitbash(module::module& module_, int reg_, size_t bus_freq_, uint32_t SDA_, uint32_t SCL_,
                 uint32_t CTRL_, bool trace_)
    : module(module_), reg(reg_), bus_freq(bus_freq_), SDA(SDA_), SCL(SCL_), CTRL(CTRL_),
      trace(trace_), access_backoff(0), access_multiplier(1), access_multiplier_limit(4) {
    /*
     * Maximum clock freq is 200KHz
     */
    access_backoff = static_cast<size_t>(((1.0 / bus_freq) / module.bus_cycle_period)) + 2;
}

bitbash::~bitbash() {
    if (module.present())
        stop();
}

void bitbash::start() {
    if (trace) {
        xia_log(log::debug) << "i2c-bb: start";
    }

    /*
     * Set SCL and SDA to 1, then set SDA to 0 while keeping SCL at 1
     *
     * The CTRL is a tristate enable control bit for SDA
     */
    bus_write(SDA | SCL | CTRL);
    bus_write(SCL | CTRL);
}

void bitbash::stop() {
    uint32_t pins = bus_read() & (SDA | SCL | CTRL);

    if (trace) {
        xia_log(log::debug) << "i2c-bb: stop " << std::hex << pins;
    }

    /*
     * Set SCL and SDA to 1, then set SDA to 0 while keep SCL at 1
     */
    if (pins != (SDA | SCL | CTRL)) {
        bus_write(SCL | CTRL);
        bus_write(SDA | SCL | CTRL);
    }
}

void bitbash::write_ack(uint8_t data, const char* what) {
    if (trace) {
        xia_log(log::debug) << "i2c-bb: write_ack";
    }

    bool acked = false;
    access_multiplier = 1;

    while (access_multiplier < access_multiplier_limit) {
        write(data);
        if (get_ack()) {
            acked = true;
            break;
        }
        ++access_multiplier;
        if (trace) {
            xia_log(log::debug) << "i2c-bb: write_ack: access-multiplier="
                                << access_multiplier;
        }
    }

    if (!acked) {
        stop();
        throw error(error::code::device_hw_failure, what);
    }
}

uint8_t bitbash::read_ack(bool ack) {
    if (trace) {
        xia_log(log::debug) << "i2c-bb: read_ack " << ack;
    }

    uint8_t data = read();
    if (ack) {
        send_ack();
    } else {
        send_nack();
    }
    return data;
}

void bitbash::write(uint8_t data) {
    if (trace) {
        xia_log(log::info) << "i2c-bb: write " << std::hex << (int) data;
    }

    uint32_t data_bit = 0;

    for (int bit = 7; bit >= 0; bit--) {
        /*
         * SDA = 0; SCL = 0; CTRL = 1
         */
        bus_write(CTRL | data_bit);

        /*
         * Get the bit to send, MSB to LSB.
         */
        if ((data & (1 << bit)) != 0) {
            data_bit = SDA;
        } else {
            data_bit = 0;
        }

        /*
         * SDA = data_bit; SCL = 0; CTRL = 1
         */
        bus_write(CTRL | data_bit);

        /*
         * SDA = data_bit; SCL = 1; CTRL = 1
         */
        bus_write(CTRL | SCL | data_bit);
    }

    /*
     * SDA = data_bit; SCL = 0; CTRL = 0
     */
    bus_write(data_bit);
}

uint8_t bitbash::read() {
    if (trace) {
        xia_log(log::debug) << "i2c-bb: read";
    }

    uint32_t data_bit = 0;
    uint8_t data = 0;

    for (int bit = 7; bit >= 0; bit--) {
        /*
         * SDA = 0; SCL = 1; CTRL = 0
         */
        bus_write(SCL);

        /*
         * Read the bit, MSB to LSB.
         */
        data_bit = bus_read() & SDA;
        if (data_bit != 0) {
            data |= 1 << bit;
        }

        /*
         * SDA = data_bit; SCL = 0; CTRL = 0
         */
        bus_write(data_bit);
    }

    if (trace) {
        xia_log(log::debug) << "i2c-bb: read " << std::hex << (int) data;
    }

    return data;
}

bool bitbash::get_ack() {
    if (trace) {
        xia_log(log::debug) << "i2c-bb: get_ack";
    }

    uint32_t data;

    /*
     * SDA = 0; SCL = 1; CTRL = 0
     */
    bus_write(SCL);

    /*
     * Read SDA
     */
    data = bus_read();

    /*
    * SDA = 0; SCL = 0; CTRL = 0
    */
    bus_write(0);

    if (trace) {
        xia_log(log::debug) << std::boolalpha << "i2c-bb: get_ack " << ((data & SDA) == 0);
    }

    return (data & SDA) == 0;
}

void bitbash::send_ack() {
    if (trace) {
        xia_log(log::debug) << "i2c-bb: send_ack";
    }

    /*
     * SDA = 0; SCL = 0; CTRL = 1
     */
    bus_write(CTRL);

    /*
     * SDA = 0; SCL = 1; CTRL = 1
     */
    bus_write(SCL | CTRL);

    /*
     * SDA = 0; SCL = 0; CTRL = 0
     */
    bus_write(0);
}

void bitbash::send_nack() {
    if (trace) {
        xia_log(log::debug) << "i2c-bb: send_nack";
    }

    /*
     * SDA = 1; SCL = 0; CTRL = 1
     */
    bus_write(CTRL);
    bus_write(SDA | CTRL);

    /*
     * SDA = 0; SCL = 1; CTRL = 1
     */
    bus_write(SDA | SCL | CTRL);

    /*
     * SDA = 0; SCL = 0; CTRL = 0
     */
    bus_write(0);
}

void bitbash::bus_write(uint8_t data) {
    module.write_word(reg, static_cast<word>(data));
    bus_wait();
}

uint8_t bitbash::bus_read() {
    return static_cast<uint8_t>(module.read_word(reg));
}

void bitbash::bus_wait() {
    size_t polls = access_backoff * access_multiplier;
    while (polls-- > 0) {
        volatile uint8_t tmp = bus_read();
        (void) tmp;
    }
}
};  // namespace i2c
};  // namespace hw
};  // namespace pixie
};  // namespace xia
