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

/** @file pixie16sys_defs.h
 * @brief Declares internal constants for use by the API functions.
 * @note This file will be deprecated July 31, 2023
*/

#ifndef XIA_PIXIE16SYS_DEFS_H
#define XIA_PIXIE16SYS_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup LEGACY Legacy
 * Functions and definitions that are part of the Legacy C implementation
 *
 * @defgroup PRIVATE Private
 * @ingroup LEGACY
 * A series of functions that provide direct access to modules. These shouldn't be used in end-user
 *   code.
 *
 * @defgroup SYS System
 * @ingroup PRIVATE
 * Functions related to system procedures like initialization, booting, etc.
 *
 * @defgroup COMMUNICATION Communication
 * @ingroup PRIVATE
 * Functions related to communicating with the modules in some way.
 *
 * @defgroup TOOLS Tools
 * @ingroup PRIVATE
 * Functions that help us perform tasks within the code. Ex. Writing messages to the log file.
 *
 * @defgroup I2CM24C64 I2CM24C64
 * @ingroup PRIVATE
 * Functions related to communication with the I2CM24C64 chip on the module.
 *
 * @defgroup PCF8574 PCF8574
 * @ingroup PRIVATE
 * Functions related to communicating with the PCF8574 chip on the module.
 */

#if defined(_WIN64) || defined(_WIN32)
#define PIXIE16SYS_EXPORT __declspec(dllexport)
#define PIXIE16SYS_API _stdcall
#else
#define PIXIE16SYS_EXPORT
#define PIXIE16SYS_API
#endif

#ifndef PCI_CODE
#define PCI_CODE
#endif

#ifndef LITTLE_ENDIAN  // LITTLE_ENDIAN: least significant byte stored first
#define LITTLE_ENDIAN  // BIG_ENDIAN:    most significant byte stored first
#endif

/**
 * @ingroup SYS
 * Preset maximum number of modules for defining data structures
*/
#define SYS_MAX_NUM_MODULES 32

/**
 * @ingroup SYS
 * Maximum number of PXI slots for one PXI chassis (currently 14-slot is the biggest chassis)
*/
#define SYS_MAX_PXI_SLOTS 14

/*
 * ADSP-21160 registers defined from
 * @link https://www.analog.com/media/en/dsp-documentation/processor-manuals/ADSP-21160_hwr_rev4.1.pdf
 * last accessed on 2021-06-24
 */
/**
 * @ingroup SYS
 * System configuration register
 */
#define SYSCON 0x00
/**
 * @ingroup SYS
 * External port DMA buffer 0
 */
#define EPB0 0x04
/**
 * @ingroup SYS
 * EP DMA10 control register
 */
#define DMAC10 0x1c
/**
 * @ingroup SYS
 * DMA channel status register
 */
#define DMASTAT 0x37

/**
 * @ingroup SYS
 * Multiplication factor for the ns_per_cycle
 */
#define NSMULTIPLIER 1.0

/**
 * @ingroup SYS
 * Option for printing out debugging messages
 */
#define PRINT_DEBUG_MSG 1

/*------------------------------------- 
	DSP code tag types
  -------------------------------------*/

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


/*-------------------------------------
	I/O Mode
  -------------------------------------*/

/**
 * @ingroup SYS
 * Host read from modules
*/
#define SYS_MOD_READ 1
/**
 * @ingroup SYS
 * Host write to modules
*/
#define SYS_MOD_WRITE 0

/**
 * @ingroup SYS
 * Maximum length of error message
*/
#define MAX_ERRMSG_LENGTH 1024


/*-------------------------------------
	Host PCI CSR bits definitions
 --------------------------------------*/
/**
 * @ingroup SYS
 * Enable run
 */
#define RUNENA 0
/**
 * @ingroup SYS
 * Enable DSP code download
 */
#define DSPDOWNLOAD 1
/**
 * @ingroup SYS
 * Indicate PCI I/O is active
 */
#define PCIACTIVE 2
/**
 * @ingroup SYS
 * Control pull-up for the SYNC lines
 */
#define PULLUP_CTRL 3
/**
 * @ingroup SYS
 * Reset DSP
 */
#define DSPRESET 4
/**
 * @ingroup SYS
 * External FIFO watermark level indicator
*/
#define EXTFIFO_WML 6
/**
 * @ingroup SYS
 * Run active indicator
*/
#define RUNACTIVE 13
/**
 * @ingroup SYS
 * Clearing external memory active indicator
*/
#define CLREXTMEM_ACTIVE 15


/*-------------------------------------
	PCI address decoding and control registers
-------------------------------------*/
#define CFG_DATACS 0x00
#define CFG_CTRLCS 0x04
#define CFG_RDCS 0x08
#define I2CM24C64_ADDR 0x10
#define PCF8574_ADDR 0x10
#define CFG_DCMRST 0x14
#define CSR_ADDR 0x48
#define REQUEST_HBR 0x80
#define HBR_DONE 0x84
#define WRT_EXT_MEM 0xC0
#define EXT_MEM_TEST 0xC4
#define WRT_DSP_MMA 0xC8
#define SET_EXT_FIFO 0xCC

#define WRT_DSP_DMAC11 0xD0
#define WRT_DSP_II11 0xD4
#define WRT_CLR_EXTMEM 0xD8
#define WRT_DSP_C11 0xDC
#define WRT_DSP_IM11 0xE0
#define WRT_DSP_EC11 0xE4
#define RD_WRT_FIFO_WML 0xE8
#define SET_EXMEM_FIFO 0xEC
#define SET_INT_FIFO 0xF0
#define WRT_RESET_RFCNT 0xF4

/**
 * @ingroup SYS
 * Address for DSP variable PowerUpInitDone
*/
#define POWERUPINITDONE_ADDRESS 0x4A03F

/**
 * @ingroup SYS
 * PLX DMA transfer timeout limit, in ms
*/
#define DMATRANSFER_TIMEOUT (5 * 1000)

/*-------------------------------------
	I2C control bits as OR-able hex patterns
-------------------------------------*/
#define SDA 0x9
#define SCL 0x2
#define CTRL 0x4


#ifdef __cplusplus
}
#endif  // End of notice for C++ compilers

#endif  // End of pixie16sys_defs.h
