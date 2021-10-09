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

/** @file dsp.cpp
 * @brief Implements functionality to use the DSP on the Pixie-16 modules.
 */

#include <iomanip>
#include <iostream>

#include <pixie/log.hpp>

#include <pixie/pixie16/csr.hpp>
#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/dsp.hpp>
#include <pixie/pixie16/module.hpp>

/*
 * ADSP-21160 registers defined from
 * https://www.analog.com/media/en/dsp-documentation/processor-manuals/ADSP-21160_hwr_rev4.1.pdf
 * last accessed on 2021-06-24
 */
/*
 * System configuration register
 */
#define SYSCON 0x00
/*
 * EP DMA10 control register
 */
#define DMAC10 0x1c
/*
 * External port DMA buffer 0
 */
#define EPB0 0x04

/*
 * DSP code tag types
 */
#define FINAL_INIT 0X0000
#define ZERO_DM16 0X0001
#define ZERO_DM32 0X0002
#define ZERO_DM40 0X0003
#define INIT_DM16 0X0004
#define INIT_DM32 0X0005
#define INIT_DM40 0X0006
#define ZERO_PM16 0X0007
#define ZERO_PM32 0X0008
#define ZERO_PM40 0X0009
#define ZERO_PM48 0X000A
#define INIT_PM16 0X000B
#define INIT_PM32 0X000C
#define INIT_PM40 0X000D
#define INIT_PM48 0X000E
#define ZERO_DM64 0X000F
#define INIT_DM64 0X0010
#define ZERO_PM64 0X0011
#define INIT_PM64 0X0012

namespace xia {
namespace pixie {
namespace hw {
namespace dsp {
dsp::dsp(module::module& module_, bool trace_) : module(module_), online(false), trace(trace_) {}

dsp& dsp::operator=(dsp&& d) {
    trace = d.trace;
    online = d.online;
    d.trace = false;
    d.online = false;
    return *this;
}

void dsp::boot(const firmware::image& image, int retries) {
    log(log::info) << "dsp [slot " << module.slot << "] load: length=" << image.size()
                   << " retries=" << retries;

    bool running = false;

    module::module::bus_guard guard(module);

    /*
     * Guard the download bit, so it is cleared when we exit.
     */
    csr::set_clear csr(module, 1 << hw::bit::DSPDOWNLOAD);

    while (!running) {
        --retries;

        try {
            /*
             * The DSP is loaded using 32 bit PCI writes to MMA of 16 bit
             * values. We use the reader with a 16 bit word size.
             */
            firmware::reader reader(image, sizeof(load_value_type));

            bool ok;

            /*
             * Reset the DSP and wait 1msec. The reset bit self clears
             */
            bus_write(hw::device::CSR, bus_read(hw::device::CSR) | (1 << hw::bit::DSPRESET));
            wait(1000);

            /*
             * HBR done for reset (not sure what this means!)
             */
            bus_write(hw::device::HBR_DONE, 0);
            wait(1000);

            /*
             * SYSCON address
             */
            bus_write(hw::device::EXT_MEM_TEST, SYSCON);

            /*
             * HBR request
             */
            ok =
                checked_write(hw::device::REQUEST_HBR, SYSCON, hw::device::WRT_DSP_MMA, 0x10, 1000);
            if (!ok) {
                throw error(error::code::device_load_failure, make_what("DSP SYSCON set failure"));
            }

            /*
             * Check DSP DMAC10
             */
            ok = checked_write(hw::device::EXT_MEM_TEST, DMAC10, hw::device::WRT_DSP_MMA, 0xa1);
            if (!ok) {
                throw error(error::code::device_load_failure, make_what("DSP DMAC10 set failure"));
            }

            /*
             * Download the boot kernel. Blocks of 3 bus words are written
             * as a time.
             */
            bus_write(hw::device::EXT_MEM_TEST, EPB0);
            for (size_t i = 0; i < 256; ++i) {
                bus_write(hw::device::WRT_DSP_MMA, reader.get());
                bus_write(hw::device::WRT_DSP_MMA, reader.get());
                bus_write(hw::device::WRT_DSP_MMA, reader.get());
            }

            /*
             * De-assert HBR
             */
            bus_write(hw::device::HBR_DONE, 0);

            /*
             * Load the image
             */
            while (reader.offset < image.size()) {
                uint32_t tag = reader.peek();
                switch (tag) {
                    case ZERO_DM16:
                    case ZERO_PM16:
                    case ZERO_DM32:
                    case ZERO_PM32:
                    case ZERO_DM40:
                    case ZERO_PM40:
                    case ZERO_PM48:
                    case ZERO_DM64:
                    case ZERO_PM64:
                        section_load(reader, 0);
                        wait(2000);
                        break;
                    case INIT_DM16:
                    case INIT_PM16:
                    case INIT_DM32:
                    case INIT_PM32:
                    case INIT_PM48:
                        section_load(reader, sizeof(load_value_type));
                        break;
                    case INIT_DM64:
                    case INIT_PM64:
                        section_load(reader, 2 * sizeof(load_value_type));
                        break;
                    case FINAL_INIT:
                        bus_write(hw::device::REQUEST_HBR, 0);
                        for (size_t i = 0; i < 258; ++i) {
                            bus_write(hw::device::WRT_DSP_MMA, reader.get());
                            bus_write(hw::device::WRT_DSP_MMA, reader.get());
                            bus_write(hw::device::WRT_DSP_MMA, reader.get());
                        }
                        break;
                    default: {
                        std::ostringstream oss;
                        oss << "DSP image tag invalid: offset=" << reader.offset
                            << " tag=" << std::hex << tag;
                        throw error(error::code::device_image_failure,
                                    make_what(oss.str().c_str()));
                    }
                }
            }

            /*
             * Wait until DSP variable PowerUpInitDone is set to 1 by the
             * DSP after it completes its initialization tasks on hard
             * RESET. This should prevent premature downloading of DSP
             * parameters by the host, which would cause corruption of DSP
             * internal memory.
             */
            guard.unlock();
            size_t retry = 10;
            while (--retry > 0) {
                if (init_done()) {
                    running = true;
                    break;
                }
                wait(1000);
            }
            guard.lock();
            if (retry == 0) {
                throw error(error::code::device_boot_failure, make_what("DSP failed to start"));
            }
        } catch (error& e) {
            if (retries == 0) {
                throw;
            }
            log(log::error) << "dsp [slot " << module.slot << "] retries: " << retries << ": "
                            << e.what();
        }
    }

    log(log::info) << "dsp [slot " << module.slot << "] running";

    online = true;
}

bool dsp::init_done() {
    const auto& power_up_init_done =
        module.module_var_descriptors[int(param::module_var::PowerUpInitDone)];
    module::module::bus_guard guard(module);
    bus_write(hw::device::REQUEST_HBR, 0);
    bus_write(hw::device::EXT_MEM_TEST, power_up_init_done.address);
    word value = bus_read(hw::device::WRT_DSP_MMA);
    bus_write(hw::device::HBR_DONE, 0);
    if (value == 1) {
        csr::clear(module, 1 << hw::bit::DSPDOWNLOAD);
    }
    return value == 1;
}

void dsp::section_load(firmware::reader& reader, const size_t wordsize) {
    bus_write(hw::device::REQUEST_HBR, 0);
    bus_write(hw::device::WRT_DSP_MMA, reader.get());
    bus_write(hw::device::WRT_DSP_MMA, reader.get());
    bus_write(hw::device::WRT_DSP_MMA, reader.get());
    bus_write(hw::device::HBR_DONE, 0);
    size_t wordcount = reader.peek();
    bus_write(hw::device::REQUEST_HBR, 0);
    bus_write(hw::device::WRT_DSP_MMA, reader.get());
    bus_write(hw::device::WRT_DSP_MMA, reader.get());
    bus_write(hw::device::WRT_DSP_MMA, reader.get());
    bus_write(hw::device::HBR_DONE, 0);
    if (wordsize != 0) {
        if ((reader.remaining()) < (wordcount * 3 * wordsize)) {
            throw error(error::code::device_image_failure, make_what("image section too small"));
        }
        wordcount *= wordsize / sizeof(load_value_type);
        for (size_t i = 0; i < wordcount; ++i) {
            bus_write(hw::device::REQUEST_HBR, 0);
            bus_write(hw::device::WRT_DSP_MMA, reader.get());
            bus_write(hw::device::WRT_DSP_MMA, reader.get());
            bus_write(hw::device::WRT_DSP_MMA, reader.get());
            bus_write(hw::device::HBR_DONE, 0);
        }
    }
}

bool dsp::checked_write(const uint32_t out, const uint32_t value, const uint32_t in,
                        const uint32_t result, const int out_wait, const int in_wait) {
    int retry = 5;
    bus_write(out, value);
    if (out_wait > 0) {
        wait(out_wait);
    }
    while (--retry > 0) {
        uint32_t data = bus_read(in);
        if (data == result) {
            return true;
        }
        wait(in_wait);
    }
    return false;
}

void dsp::bus_write(int reg, uint32_t data) {
    module.write_word(reg, data);
}

uint32_t dsp::bus_read(int reg) {
    return module.read_word(reg);
}

std::string dsp::make_what(const char* msg) {
    std::string what;
    what = "dsp: ";
    what += msg;
    return what;
}
};  // namespace dsp
};  // namespace hw
};  // namespace pixie
};  // namespace xia
