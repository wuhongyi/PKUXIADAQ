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

/** @file csr.hpp
 * @brief Defines how we handle the Pixie-16 CSR.
 */

#ifndef PIXIE_HW_CSR_H
#define PIXIE_HW_CSR_H

#include <cstdint>

#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
namespace module {
class module;
}
namespace hw {
/**
 * @brief PCI Host Control Register
 *
 * A Pixie-16 module has several control registers that are used to activate
 * runs, download DSP code or reset DSP, control pull-up resistor for the
 * trigger lines on the backplane, and/or indicate module status to the host
 * computer. Software on the host computer can set and/or read bits in these
 * registers to control the operation of the Pixie-16 module or monitor its
 * internal status.
 *
 * | Bit name | Bit # | Direction | Description |
 * |---|---|---|---|
 * | RUNENABLE | 0 | Read/Write | Enable run:  <br>=1: start a run <br>=0: stop a run |
 * | Download DSP code | 1 | Read/Write | Enable DSP code download:   <br>=1: Download DSP code <br> =0: DSP code is running |
 * | PCI Active read or write | 2 | Read/Write | Indicate PCI I/O is active:   <br>=1: PCI is reading or writing memory <br> =0: no PCI read or write |
 * | Pull-up resistor control | 3 | Read/Write | Control pull-up for the SYNC lines:   <br>=1: wired-OR trigger lines on the backplane   connect to a pullup resistor <br>=0: not connected |
 * | Reset DSP | 4 | Read/Write | Generate a pulse to reset DSP:   calling function Pixie_ReadCSR to read the value of this control register, and then set this bit to 1, and finally write the control register back to the System FPGA by calling function Pixie_WrtCSR |
 * | Reserved | 5 |  |  |
 * | EXTFIFO_WML | 6 | Read only | External FIFO watermark level indicator: <br>=1: number of data words in the external FIFO exceeds the watermark level, so the external FIFO is ready to be read out <br>=0: number of data words in the external FIFO is still below the watermark level, so the external   FIFO is not yet ready to be read out |
 * | Reserved | [12:7] |  |  |
 * | RUNACTIVE | 13 | Read only | Run active indicator:   <br>=1: run is active <br>=0: run has ended |
 * | Reserved | 14 |  |  |
 * | CLREXTMEM_ACTIVE | 15 | Read only | Clearing external memory active indicator:   <br>=1: clearing external memory is still ongoing <br>=0: clearing external memory has completed |
 */
namespace csr {
/**
 * @brief Reset the CSR to a default state.
 * @param[in] module The module whose CSR we'll reset.
 */
void reset(module::module& module);

/*
 * Low level read/write and bit set/clear support.
 */
word read(module::module& module);
void write(module::module& module, word value);
void set(module::module& module, word mask);
void clear(module::module& module, word mask);

/**
 * @brief Set the bit mask into the CSR when constructed and clear
 *          in the destructor.
 */
struct set_clear {
    module::module& module;
    const uint32_t mask;
    set_clear(module::module& module, uint32_t mask);
    ~set_clear();
};

/**
 * @brief Wait for external FIFO to get ready
 * @param module The module that we'll wait for.
 * @param polls The number of time's we'll check for a ready FIFO.
 */
void fifo_ready_wait(module::module& module, const size_t polls = 1000);
}  // namespace csr
}  // namespace hw
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_HW_CSR_H
