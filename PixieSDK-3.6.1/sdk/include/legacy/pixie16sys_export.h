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

/** @file pixie16sys_export.h
 * @brief Declares internal function prototypes
 * @note This file will be deprecated July 31, 2023
 */

#ifndef XIA_PIXIE16SYS_EXPORT_H
#define XIA_PIXIE16SYS_EXPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pixie16sys_defs.h"

/**
 * @ingroup SYS
 * @brief Initialize Pixie16 system.
 *
 * The initialization process includes mapping PXI slots to Pixie16 modules and assigning virtual
 * addresses to the PLX9054 chips of those modules.
 *
 * @param[in] NumModules:
 * @param[in] PXISlotMap:
 * @param[in] OfflineMode:
 * @returns A status code indicating the result of the operation
 * @retval  0 - Successful
 * @retval -1 - Failed to measure host computer speed (ns per cycle)
 * @retval -2 - Can't find any PLX device in the system
 * @retval -3 - Can't find all Pixie-16 modules that were specified
 * @retval -4 - Could not open a PCI Device when trying find all devices
 * @retval -5 - Unable to map a PCI BAR into user virtual space
 * @retval -6 - Could not read chassis slot number in which a module is installed
 * @retval -7 - Failed to find PCI device when trying to read its chassis slot number
 * @retval -8 - Unable to unmap the PCI BAR while closing those all opened PCI devices
 * @retval -9 - Unable to close the PLX9054 device while closing those all opened PCI devices
 * @retval -10 - Can't match a module with one found by the PLX driver
 * @retval -11 - Could not open PCI Device after finishing mapping modules
 * @retval -12 - Could not close PCI device for all opened modules
 * @retval -13 - Could not find PCI device after finishing mapping modules
 * @retval -20 - Found an unsupported revision during initialization
 */
PIXIE16SYS_EXPORT int Pixie_InitSystem(unsigned short NumModules, unsigned short* PXISlotMap,
                                       unsigned short OfflineMode);

/**
 * @ingroup SYS
 * @brief Unmap PCI BAR and close PLX PCI devices.
 * @param[in] ModNum: The module that we want to unmap.
 * @returns A status code indicating the result of the operation
 * @retval  0 - successful
 * @retval -1 - Unable to unmap the PCI BAR
 * @retval -2 - Unable to close the PCI device
 */
PIXIE16SYS_EXPORT int Pixie_ClosePCIDevices(unsigned short ModNum);

/**
 * @ingroup SYS
 * @brief Download system chip FPGA configuration.
 * @param[in] ModNum: The module that we'll work with
 * @param[in] Com_FPGA_conf: Pointer to the data block holding the firmware data
 * @param[in] NumComFPGAconf: The size of the memory block holding the firmware data.
 * @returns A status code indicating the result of the operation
 * @retval  0 - boot successful
 * @retval -2 - Clearing system FPGA MM timed out
 * @retval -3 - Downloading system FPGA timed out
 */
PIXIE16SYS_EXPORT int Pixie_Boot_CompFPGA(unsigned short ModNum, unsigned int* Com_FPGA_conf,
                                          unsigned int NumComFPGAconf);

/**
 * @ingroup SYS
 * @brief Downloads the trigger FPGA configuration.
 * @note This function is deprecated and is no longer implemented!
 * @param[in] ModNum: The module number to work with
 * @param[in] Trig_FPGA_conf: Pointer to a memory block holding the Trigger FPGA data.
 * @param[in] NumTrigFPGAconf: The size of the memory block holding the Trigger FPGA data.
 * @returns Nothing as it's not implemented.
 */
PIXIE16SYS_EXPORT int Pixie_Boot_TrigFPGA(unsigned short ModNum, unsigned int* Trig_FPGA_conf,
                                          unsigned int NumTrigFPGAconf);

/**
 * @ingroup SYS
 * @brief Boots the signal processing (FIPPI) FPGAs.
 * @param[in] ModNum: The module that we'll operate against.
 * @param[in] SP_FPGA_conf: Pointer to a data block holding the firmware
 * @param[in] NumSPFPGAconf: Size of the data block holding the firmware.
 * @returns A status code indicating the result of the operation
 * @retval  0 - boot successful
 * @retval -2 - Clearing FIPPI configuration timed out
 * @retval -3 - Downloading FIP0&1 FPGA timed out
 */
PIXIE16SYS_EXPORT int Pixie_Boot_FIPPI(unsigned short ModNum, unsigned int* SP_FPGA_conf,
                                       unsigned int NumSPFPGAconf);

/**
 * @ingroup SYS
 * @brief Uploads the DSP code to the module.
 * @param[in] ModNum: The module that we want to work with.
 * @param[in] DSP_code: Pointer to a memory block holding the DSP code.
 * @param[in] NumDSPCodeWords: The size of the memory block holding the DSP code.
 * @returns A status code indicating the result of the operation
 * @retval  0 - boot DSP successful
 * @retval -1 - Can not set timer resolution to 1 ms
 * @retval -2 - Set DSP SYSCON in module failed
 * @retval -3 - Set DSP DMAC10 in module failed
 * @retval -4 - tagnum is invalid
 * @retval -5 - DSP can't finish initialization on reset
 */
PIXIE16SYS_EXPORT int Pixie_Boot_DSP(unsigned short ModNum, unsigned int* DSP_code,
                                     unsigned int NumDSPCodeWords);

/**
 * @ingroup COMMUNICATION
 * @brief Write/read data to/from the DSP memory.
 *
 * The function will always write one 32-bit word at a time. Reads depend on nWords. If nWords is
 * greater than 1 then the entire block will be read at once using a burst read.
 *
 * @param[in,out] dsp_data: The data that we'll be writing to or reading from the DSP
 * @param[in] dsp_address: The DSP memory address to perform the operation on
 * @param[in] nWords: The data size that we'll read/write
 * @param[in] direction: Whether this operation is a read or write. Uses MOD_READ or MOD_WRITE
 * @param[in] ModNum: The module number that we'll be performing the operation against.
 * @returns A status code indicating the result of the operation
 * @retval  0 - if successful
 * @retval -1 - if reading the DSP memory blocks failed
 * @retval -2 - if reading DSP memory remaining words failed
*/
PIXIE16SYS_EXPORT int Pixie_DSP_Memory_IO(unsigned int* dsp_data, unsigned int dsp_address,
                                          unsigned int nWords, unsigned short direction,
                                          unsigned short ModNum);

/**
 * @ingroup COMMUNICATION
 * @brief Check the external FIFO status and return the number of words that exist in the FIFO.
 * @param[out] nFIFOWords : A pointer to a 32-bit unsigned integer whose value will be the number of
 *     32-bit words in the External FIFO.
 * @param[in] ModNum: The Pixie module being addressed
 * @retval A status code indicating the result of the operation
 * @retval 0 - I/O successful
 */
PIXIE16SYS_EXPORT int Pixie_Read_ExtFIFOStatus(unsigned int* nFIFOWords, unsigned short ModNum);

/**
 * @ingroup COMMUNICATION
 * @brief Read data from the external FIFO using PCI burst mode.
 * @note This function depends on the PLX major version number!
 * @param extfifo_data : Pointer to a memory block large enough to hold all of the external
 *     FIFO data.
 * @param nWords : The number of external FIFO words to be read.
 * @param ModNum : The module number to read the external FIFO data from.
 * @returns A status code indicating the result of the operation
 * @retval  0 - I/O successful
 * @retval -1 - Failed to open PLX DMA channel
 * @retval -2 - Failed to read FIFO watermark in System FPGA
 * @retval -3 - PlxPci_DmaTransferUserBuffer failed while reading from external FIFO
 * @retval -4 - PlxPci_DmaChannelClose failed after reading from DSP memory
 */
PIXIE16SYS_EXPORT int Pixie_ExtFIFO_Read(unsigned int* extfifo_data, unsigned int nWords,
                                         unsigned short ModNum);

/**
 * @ingroup COMMUNICATION
 * @brief Pixie external memory I/O communication (burst Read/Write)
 * @note This function depends on the PLX major revision number.
 * @param[in,out] memory_data : Pointer to a data block that will contain the data.
 * @param[in] memory_address : The main memory address that we'd like to put or receive data.
 * @param[in] nWords : The number of 32-bit unsigned integers to transfer
 * @param[in] direction : The direction of data flow. Either SYS_MOD_WRITE or SYS_MOD_READ).
 * @param[in] ModNum : The module index that we'll be working with. Counting from 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Failed to open PLX DMA channel
 * @retval -2 - PlxPci_DmaTransferUserBuffer failed while reading from external memory
 * @retval -3 - PlxPci_DmaChannelClose failed after reading from external memory
 */
PIXIE16SYS_EXPORT int Pixie_Main_Memory_IO(unsigned int* memory_data, unsigned int memory_address,
                                           unsigned int nWords, unsigned short direction,
                                           unsigned short ModNum);

/**
 * @ingroup COMMUNICATION
 * @brief Clears the histogram data from the module's external memory.
 *
 * Clear Pixie external memory by the System FPGA (However, this feature is not
 * available in Rev-A modules, which still uses single writes to clear external
 * memory). This method of clearing  external memory is ~20 times faster than
 * clearing by host PCI through single writes to the external memory.
 *
 * @param[in] memory_address: External memory address to clear.
 * @param[in] nWords: Number of external memory words to clear, starting from memory address.
 * @param[in] ModNum: The module number to operate on.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Operation timed out.
 */
PIXIE16SYS_EXPORT int Pixie_Clear_Main_Memory(unsigned int memory_address, unsigned int nWords,
                                              unsigned short ModNum);

/**
 * @ingroup COMMUNICATION
 * @brief Used for single-word I/O communications between Host and Pixie registers.
 * @param[in] ModNum: the Pixie module to communicate to
 * @param[in] address: register address
 * @param[in] direction: either SYS_MOD_READ or SYS_MOD_WRITE
 * @param[in,out] value: holds or receives the data
 * @returns A status code indicating the result of the operation
 * @retval 0 - Success
 */
PIXIE16SYS_EXPORT int Pixie_Register_IO(unsigned short ModNum, unsigned int address,
                                        unsigned short direction, unsigned int* value);

/**
 * @ingroup COMMUNICATION
 * @brief Read the module's control status register (CSR).
 * @param[in] ModNum: The module to read from
 * @param[out] CSR: The value of the CSR.
 */
PIXIE16SYS_EXPORT void Pixie_ReadCSR(unsigned short ModNum, unsigned int* CSR);

/**
 * @ingroup COMMUNICATION
 * @brief Write to the module's control status register (CSR).
 * @param[in] ModNum: The module to write to
 * @param[in] CSR: The value to write
 */
PIXIE16SYS_EXPORT void Pixie_WrtCSR(unsigned short ModNum, unsigned int CSR);

#if __GNUC__
#define PIXIE16_PRINTF(_s, _f) __attribute__((format(printf, _s, _f)))
#else
#define PIXIE16_PRINTF(_s, _f)
#endif
#define PIXIE_FUNC __func__

/**
 * @ingroup TOOLS
 * @brief Prints a message to the log file.
 * @param[in] format: The message formatting.
 * @param[in] ...: The message arguments.
 */
PIXIE16SYS_EXPORT void Pixie_Print_MSG(const char* format, ...) PIXIE16_PRINTF(1, 2);

/**
 * @ingroup TOOLS
 * @brief Prints an error message to the log file.
 * @param[in] func: The function doing the printing.
 * @param[in] format: The message formatting.
 * @param[in] ...: The message arguments.
 */
PIXIE16SYS_EXPORT void Pixie_Print_Error(const char* func, char* format, ...) PIXIE16_PRINTF(2, 3);

/**
 * @ingroup TOOLS
 * @brief Prints a warning message to the log file.
 * @param[in] func: The function doing the printing.
 * @param[in] format: The message formatting.
 * @param[in] ...: The message arguments.
 */
PIXIE16SYS_EXPORT void Pixie_Print_Warning(const char* func, char* format, ...)
    PIXIE16_PRINTF(2, 3);

/**
 * @ingroup TOOLS
 * @brief Prints an informational message to the log file.
 * @param[in] func: The function doing the printing.
 * @param[in] format: The message formatting.
 * @param[in] ...: The message arguments.
 */
PIXIE16SYS_EXPORT void Pixie_Print_Info(const char* func, char* format, ...) PIXIE16_PRINTF(2, 3);

/**
 * @ingroup TOOLS
 * @brief Prints a debugging message to the log file.
 *
 * Depends on the preprocessor definition PRINT_DEBUG_MSG.
 *
 * @param[in] func: The function doing the printing.
 * @param[in] format: The message formatting.
 * @param[in] ...: The message arguments.
 */
PIXIE16SYS_EXPORT void Pixie_Print_Debug(const char* func, char* format, ...) PIXIE16_PRINTF(2, 3);

/**
 * @ingroup I2CM24C64
 * @brief Bus master writes one byte to I2C serial EEPROM M24C64.
 * @param[in] ModNum: Pixie module module
 * @param[in] Address: The address to write this byte
 * @param[in] ByteValue: The byte value that will be written
 * @returns A status code indicating the result of the operation
 * @retval  0 - Successful
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to get Acknowledge after sending DevSel byte
 * @retval -3 - Failed to get Acknowledge after sending address byte (MSB)
 * @retval -4 - Failed to get Acknowledge after sending address byte (LSB)
 * @retval -5 - Failed to get Acknowledge after sending byte value
 */
PIXIE16SYS_EXPORT int I2CM24C64_Write_One_Byte(unsigned short ModNum, unsigned short Address,
                                               char* ByteValue);

/**
 * @ingroup I2CM24C64
 * @brief Bus master reads one byte from I2C serial EEPROM M24C64.
 * @param[in] ModNum: Pixie module number
 * @param[in] Address: The address to read this byte
 * @param[out] ByteValue: The byte value read from the address
 * @returns A status code indicating the result of the operation
 * @retval  0 - Successful
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to get Acknowledge after sending DevSel byte
 * @retval -3 - Failed to get Acknowledge after sending address byte (MSB)
 * @retval -4 - Failed to get Acknowledge after sending address byte (LSB)
 * @retval -5 - Failed to get Acknowledge after sending DevSel byte
 */
PIXIE16SYS_EXPORT int I2CM24C64_Read_One_Byte(unsigned short ModNum, unsigned short Address,
                                              char* ByteValue);

/**
 * @ingroup I2CM24C64
 * @brief Bus master writes one page to I2C serial EEPROM M24C64.
 * @param[in] ModNum: Pixie module module
 * @param[in] Address: The starting address to write this page
 * @param[in] NumBytesToWrite: The number of bytes for this page to write
 * @param[in] ByteValue: The byte values
 * @returns A status code indicating the result of the operation
 * @retval  0 - Successful
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to get Acknowledge after sending DevSel byte
 * @retval -3 - Failed to get Acknowledge after sending address byte (MSB)
 * @retval -4 - Failed to get Acknowledge after sending address byte (LSB)
 * @retval -5 - Failed to get Acknowledge after writing a byte
 */
PIXIE16SYS_EXPORT int I2CM24C64_Page_Write(unsigned short ModNum, unsigned short Address,
                                           unsigned short NumBytesToWrite, char* ByteValue);

/**
 * @ingroup I2CM24C64
 * @brief Bus master reads multiple bytes from I2C serial EEPROM M24C64.
 * @param[in] ModNum: Pixie module number
 * @param[in] Address: The starting address to read
 * @param[in] NumBytesToRead: Number of bytes to read (maximum 32 bytes)
 * @param[out] ByteValue: The byte values
 * @returns A status code indicating the result of the operation
 * @retval  0 - Successful
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to get Acknowledge after sending DevSel byte
 * @retval -3 - Failed to get Acknowledge after sending address byte (MSB)
 * @retval -4 - Failed to get Acknowledge after sending address byte (LSB)
 * @retval -5 - Failed to get Acknowledge after writing a byte
 */
PIXIE16SYS_EXPORT int I2CM24C64_Sequential_Read(unsigned short ModNum, unsigned short Address,
                                                unsigned short NumBytesToRead, char* ByteValue);

/**
 * @ingroup PCF8574
 * @brief Bus master reads one byte from PCF8574.
 * @param[in] ModNum: The module to read the byte from
 * @param[out] ByteValue: A variable to store the read byte value.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Successful
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to get Acknowledge after sending DevSel byte
 * @retval -3 - Failed to get Acknowledge after sending DevSel byte
 */
PIXIE16SYS_EXPORT int PCF8574_Read_One_Byte(unsigned short ModNum, char* ByteValue);

#ifdef __cplusplus
}
#endif  // End of notice for C++ compilers
#endif  // End of pixie16sys_export.h
