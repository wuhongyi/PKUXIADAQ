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

/** @file pixie16app_export.h
 * @brief Defines user-facing function prototypes
 * @note This file will be deprecated July 31, 2023
 */

#ifndef XIA_PIXIE16APP_EXPORT_H
#define XIA_PIXIE16APP_EXPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pixie16app_defs.h"

/**
 * @ingroup APP
 * @brief Initializes the system by mapping PXI slots to modules
 *
 * Use this function to configure the Pixie-16 modules in the PXI chassis.
 *
 * NumModules is the total number of Pixie-16 modules installed in the system. PXISlotMap is the
 * pointer to an array that must have at least as many entries as there are Pixie-16 modules in the
 * chassis.
 *
 * PXISlotMap serves as a simple mapping of the logical module number and the physical slot number
 * that the modules reside in. The logical module number runs from 0. For instance, in a system
 * with 5 Pixie-16 modules, these 5 modules may occupy slots 3 through 7. The user must fill
 * PXISlotMap as follows: PXISlotMap = {3, 4, 5, 6, 7 ...} since module number 0 resides in slot
 * number 3, etc. To find out in which slot a module is located, any piece of subsequent code can
 * use the expression PXISlotMap[ModNum], where ModNum is the logic module number.
 * OfflineMode is used to indicate to the API whether the system is running in OFFLINE mode (1) or
 * ONLINE mode (0). OFFLINE mode is useful for situations where no Pixie-16 modules are present but
 * users can still test their calls to the API functions in their application software.
 * This function must be called as the first step in the boot process. It makes the modules known
 * to the system and “opens” each module for communication.
 *
 * @param[in] NumModules: The total number of Pixie16 modules in the system provided by the user.
 * @param[in] PXISlotMap: An array containing the PXI slot number for each module. The array index
 *     indicates which module number the slot number takes. The first slot number is Module 0.
 * @param[in] OfflineMode: Used to tell the API that there are no modules connected to the system.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid total number of Pixie16 modules
 * @retval -2 - Null pointer *PXISlotMap.
 * @retval -3 - Failed to initialize system
 * @retval -4 - Failed to read the module's information
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16InitSystem(unsigned short NumModules,
                                                       unsigned short* PXISlotMap,
                                                       unsigned short OfflineMode);

/**
 * @ingroup APP
 * @brief Release resources used by PCI devices before exiting the application.
 * @param[in] ModNum: The module number that we'll be closing.
 * @returns A status code indicating the result of the operation
 * @retval:  0 - Success
 * @retval: -1 - Invalid Pixie module number
 * @retval: -2 - Failed to close Pixie module
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ExitSystem(unsigned short ModNum);

/**
 * @ingroup APP
 * @brief Read information stored on each module, including its revision, serial number, and ADC.
 *
 * Use this function to read information stored on each module, including its revision, serial
 * number, ADC bits and sampling rate. This should be done after initializing the PCI communication.
 * Information from the module can be used to select the appropriate firmware, DSP, and
 * configuration parameters files before booting the module.
 * 
 * EEPROM Format
 * 
 * Read the first 128 bytes from the beginning of the I2C serial EEPROM.
 * 
 *  Version 1:
 *   First three words of EEPROM stores serial number and revision number
 * 
 *  Version 2:
 *   If the CRC in the header validates it is a version 2 or higher format.
 * 
 *   Header: [CRC32 (little endian, 4 bytes)] [CTRL (1 byte)] [RESERVED (1 byte)]
 *   Data: [[ID (1 byte)] [DATA (variable length)]
 * 
 *   Supported Ids:
 *    SERIAL_NUM = 11, little endian number, size = 4
 *    REVISION = 12, little endian number, size = 2
 *    END = 255
 *
 * @param[in] ModNum: The module number (counts from 0) that we'll read information
 * @param[out] ModRev: The revision read from the on-board EEPROM
 * @param[out] ModSerNum: The serial number read from the on-board EEPROM
 * @param[out] ModADCBits: The ADC bit resolution read from the on-board EEPROM
 * @param[out] ModADCMSPS: The ADC sampling frequency read from the on-board EEPROM
 * @returns A status code indicating the result of the operation
 * @retval:  0 - Success
 * @retval: -1 - Invalid Pixie module number
 * @retval: -2 - Failed to read the serial number from I2C serial EEPROM
 * @retval: -3 - Failed to read the ADC information from I2C serial EEPROM
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadModuleInfo(unsigned short ModNum,
                                                           unsigned short* ModRev,
                                                           unsigned int* ModSerNum,
                                                           unsigned short* ModADCBits,
                                                           unsigned short* ModADCMSPS);

/**
 * @ingroup APP
 * @brief Boot one or more modules in the system.
 *
 * Use this function to boot Pixie-16 modules so that they can be set up for data taking. The
 * function downloads to the Pixie-16 modules the communication (or system) FPGA configurations,
 * signal processing FPGA configurations, trigger FPGA configurations (Revision A modules only),
 * executable code for the digital signal processor (DSP), and DSP parameters.
 *
 * The FPGA configurations consist of a fixed number of words depending on the hardware mounted on
 * the modules; the DSP codes have a length which depends on the actual compiled code; and the set
 * of DSP parameters always consists of 1280 32-bit words for each module. The host software has to
 * make the names of those boot data files on the hard disk available to the boot function.
 *
 * ModNum is the module number which starts counting at 0. If ModNum is set to be less than the
 * total number of modules in the system, only the module specified by ModNum will be booted. But
 * if ModNum is equal to the total number of modules in the system, e.g. there are 5 modules in
 * the chassis and ModNum = 5, then all modules in the system will be booted.
 *
 * #### Boot Pattern Bits
 * The boot pattern is a bit mask (shown below) indicating which on-board chip will be booted.
 * Under normal circumstances, all on-board chips should be booted, i.e. the boot pattern would
 * be 0x7F. For Rev-B, C, D, F modules, bit 1, i.e., “Boot trigger FPGA”, will be ignored even if
 * that bit is set to 1.
 * | Bit | Description | Applicable Hardware |
 * |---|---|---|
 * | 0 | Boot communication FPGA  | All Pixie-16 Revisions  |
 * | 1 | Boot trigger FPGA  | Revision A - Ignored  |
 * | 2 | Boot signal processing FPGA  | All Pixie-16 Revisions  |
 * | 3 | Boot digital signal processor (DSP)  | All Pixie-16 Revisions  |
 * | 4 | Download DSP I/O parameters  | All Pixie-16 Revisions  |
 * | 5 | Program on-board FPGAs  | All Pixie-16 Revisions  |
 * | 6 | Set on-board DACs  | All Pixie-16 Revisions  |
 *
 * @param[in] ComFPGAConfigFile: name of communications FPGA configuration file
 * @param[in] SPFPGAConfigFile: name of signal processing FPGA configuration file
 * @param[in] TrigFPGAConfigFile: name of trigger FPGA configuration file (!!! IGNORED)
 * @param[in] DSPCodeFile: name of executable code file for digital signal processor (DSP)
 * @param[in] DSPParFile: name of DSP parameter file
 * @param[in] DSPVarFile: name of DSP variable names file
 * @param[in] ModNum: The module number. If set to Number_Modules we'll boot all modules.
 * @param[in] BootPattern: boot pattern bit mask
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid module number
 * @retval -2 - Size of ComFPGAConfigFile is invalid
 * @retval -3 - Could not read the ComFPGAConfig file
 * @retval -4 - Failed to allocate memory to store ComFPGAConfig
 * @retval -5 - Failed to open ComFPGAConfigFile
 * @retval -10 - Size of SPFPGAConfigFile is invalid
 * @retval -11 - Failed to read SPFPGAConfigFile
 * @retval -12 - Failed to allocate memory to store SPFPGAConfigFile
 * @retval -13 - Failed to open SPFPGAConfigFile
 * @retval -14 - Failed to read DSPCodeFile
 * @retval -15 - Failed to allocate memory to store DSP executable code
 * @retval -16 - Failed to open DSPCodeFile
 * @retval -17 - Size of DSPParFile is invalid
 * @retval -18 - Failed to open DSPParFile
 * @retval -19 - Can't initialize DSP variable indices
 * @retval -20 - Can't copy DSP variable indices
 * @retval -21 - Failed to program Fippi in a module
 * @retval -22 - Failed to set DACs in a module
 * @retval -23 - Failed to start RESET_ADC run in a module
 * @retval -24 - RESET_ADC run timed out in a module
 * @retval -25 - Failed to boot the Communication FPGA
 * @retval -26 - Failed to boot signal processing FPGA
 * @retval -27 - Failed to boot DSP
 * @retval -28 - Failed to read DSPParFile
 */
PIXIE16APP_EXPORT int PIXIE16APP_API
Pixie16BootModule(const char* ComFPGAConfigFile, const char* SPFPGAConfigFile,
                  const char* TrigFPGAConfigFile, const char* DSPCodeFile, const char* DSPParFile,
                  const char* DSPVarFile, unsigned short ModNum, unsigned short BootPattern);

/**
 * @ingroup APP
 * @brief Acquire ADC traces from a Pixie module.
 *
 * Use this function to acquire ADC traces from Pixie-16 modules. Specify the module using ModNum
 * which starts counting at 0. If ModNum is set to be less than the total number of modules in the
 * system, only the module specified by ModNum will have its ADC traces acquired. But if ModNum is
 * equal to the total number of modules in the system, then all modules in the system will have
 * their ADC traces acquired.
 *
 * After the successful return of this function, the DSP’s internal memory will be filled with ADC
 * trace data. A user’s application software should then call the function
 * Pixie16ReadSglChanADCTrace to read the ADC trace data out to the host computer,
 * channel by channel.
 *
 * @see Pixie16ReadSglChanADCTrace
 *
 * @param[in] ModNum: The module number that we'll be working with
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to start run
 * @retval -3 - Acquiring ADC traces timed out
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AcquireADCTrace(unsigned short ModNum);

/**
 * @ingroup APP
 * @brief Acquire ADC traces from a Pixie module.
 *
 * Use this function to read ADC trace data from a Pixie-16 module. Before calling this function,
 * another function Pixie16AcquireADCTrace should be called to fill the DSP internal memory first.
 * Also, the host code should allocate appropriate amount of memory to store the trace data. The
 * ADC trace data length for each channel is 8192. Since the trace data are 16-bit unsigned
 * integers (for hardware variants with less than 16-bit ADCs only the lower 12-bit or 14-bit
 * contain real data), two consecutive 16-bit words are packed into one 32-bit word in the DSP
 * internal memory. So for each channel, 4096 32-bit words are read out first from the DSP, and
 * then each 32-bit word is unpacked to form two 16-bit words.
 *
 * Specify the module using ModNum and the channel on the module using ChanNum. Note that both the
 * modules and channels are counted starting at 0.
 *
 * @see Pixie16AcquireADCTrace
 *
 * @param[out] Trace_Buffer: Pointer to the data buffer that will hold the data.
 * @param[in] Trace_Length: Length of the traces for the module.
 * @param[in] ModNum: The module that we want to read
 * @param[in] ChanNum: The channel that we want to read
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid Pixie channel number
 * @retval -3 - Invalid trace length
 * @retval -4 - Failed to allocate memory to store ADC traces
 * @retval -5 - Failed to read ADC traces
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanADCTrace(unsigned short* Trace_Buffer,
                                                                unsigned int Trace_Length,
                                                                unsigned short ModNum,
                                                                unsigned short ChanNum);

/**
 * @ingroup APP
 * @brief Read or write data from/to the DSP internal memory of a Pixie module.
 *
 * Use this function to directly transfer data between the host and the DSP internal memory of a
 * Pixie-16 module.
 *
 * The DSP internal memory is split into two blocks with address range 0x40000 to 0x4FFFF for the
 * first block and address range 0x50000 to 0x5FFFF for the second block. Within the first block,
 * address range 0x40000 to 0x49FFF is reserved for program memory and shouldn’t be accessed
 * directly by the host computer. Address range 0x4A000 to 0x4A4FF is used by the DSP I/O parameters
 * which are stored in the configuration files (.set files) in the host. Within this range, 0x4A000
 * to 0x4A33F can be both read and written, but 0x4A340 to 0x4A4FF can only be read but not written.
 * The remaining address range (0x4A500 to 4FFFF) in the first block and the entire second block
 * (0x50000 to 0x5FFFF) should only be read but not written by the host.
 *
 * @param[in,out] Buffer: Memory block used to hold the data for read or write
 * @param[in] NumWords: Number of words that we're going to read or write.
 * @param[in] Address: The address that we'd like to perform the operation against
 * @param[in] Direction: 0 for writes and 1 for reads.
 * @param[in] ModNum: The module number that we'd like to work against. Starts counting at 0.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - success
 * @retval -1 - null pointer for buffer data
 * @retval -2 - number of buffer words exceeds the limit
 * @retval -3 - invalid DSP internal memory address
 * @retval -4 - invalid I/O direction
 * @retval -5 - invalid Pixie module number
 * @retval -6 - I/O Failure
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16IMbufferIO(unsigned int* Buffer, unsigned int NumWords,
                                                       unsigned int Address,
                                                       unsigned short Direction,
                                                       unsigned short ModNum);

/**
 * @ingroup APP
 * @brief Read or write data from/to the external memory of a Pixie module.
 *
 * Use this function to directly read data from or write data to the on-board external memory of a
 * Pixie-16 module. The valid memory address is from 0x0 to 0x7FFFF (32-bit wide).
 *
 * The external memory is used to store the histogram data accumulated for each of the 16 channels
 * of a Pixie-16 module. Each channel has a fixed histogram length of 32768 words (32-bit wide),
 * and the placement of the histogram data in the memory is in the same order of the channel
 * number, i.e. channel 0 occupies memory address 0x0 to 0x7FFF, channel 1 occupies 0x8000 to
 * 0xFFFF, and so on.
 *
 * NOTE: another function Pixie16ReadHistogramFromModule can also be used to read out the
 * histograms except that it needs to be called channel by channel.
 *
 * @param[in,out] Buffer: Memory block used to hold the data for read or write
 * @param[in] NumWords: Number of words that we're going to read or write.
 * @param[in] Address: The address that we'd like to perform the operation against
 * @param[in] Direction: 0 for writes and 1 for reads.
 * @param[in] ModNum: The module number that we'd like to work against. Starts counting at 0.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - success
 * @retval -1 - null pointer for buffer data
 * @retval -2 - number of buffer words exceeds the limit
 * @retval -3 - invalid System FPGA internal memory address
 * @retval -4 - invalid I/O direction
 * @retval -5 - invalid Pixie module number
 * @retval -6 - I/O Failure
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16EMbufferIO(unsigned int* Buffer, unsigned int NumWords,
                                                       unsigned int Address,
                                                       unsigned short Direction,
                                                       unsigned short ModNum);

/**
 * @ingroup APP
 * @brief Start or resume a list-mode run.
 *
 * Use this function to start a list-mode data acquisition run in Pixie-16 modules. list-mode runs
 * are used to collect data on an event-by-event basis, gathering energies, timestamps, pulse shape
 * analysis values, and waveforms for each event. Runs will continue until the user terminates the
 * run by calling function Pixie16EndRun. To start the data acquisition this function has to be
 * called for every Pixie-16 module in the system. If all modules are to run synchronously, the
 * last module addressed will release all others and the acquisition starts then. The first module
 * to end the run will immediately stop the run in all other modules if run synchronization has
 * been set up among these modules.
 *
 * There is only one list-mode run type supported, that is, 0x100. However, different output data
 * options can be chosen by enabling or disabling different CHANCSRA bits.
 *
 * ModNum is the module number which starts counting at 0. If ModNum is set to be less than the
 * total number of modules in the system, only the module specified by ModNum will have its list
 * mode run started. But if ModNum is set to equal to the total number of modules in the system,
 * then all modules in the system will have their runs started together.
 *
 * @param[in] ModNum: The module number that we'd like to work against. Starts counting at 0.
 * @param[in] RunType: The type of run that we're going to be executing. There's only one 0x100.
 * @param[in] mode: How we'll handle existing data when starting up the run. Use mode=NEW_RUN (=1)
 *     to erase histograms and statistics information before launching the new
 *     run. Note that this will cause a startup delay of up to 1 millisecond.
 *     Use mode=RESUME_RUN (=0) to resume an earlier run. This mode has a startup delay of only a
 *     few microseconds.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid mode
 * @retval -3 - Failed to start list-mode run
 * @retval -4 - Invalid run type
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16StartListModeRun(unsigned short ModNum,
                                                             unsigned short RunType,
                                                             unsigned short mode);

/**
 * @ingroup APP
 * @brief Start or resume a histogram run.
 *
 * Use this function to begin a data acquisition run that accumulates energy histograms, one for
 * each channel. It launches a data acquisition run in which only energy information is preserved
 * and histogrammed locally to each channel.
 *
 * Call this function for each Pixie-16 module in the system to initialize the run in each module.
 * Actual data acquisition will start synchronously in all modules when the last module finished
 * the initialization (requires the synchronization parameter to be set). Histogram runs can be
 * self-terminating when the elapsed run time exceeds the preset run time, or the user can
 * prematurely terminate the run by calling Pixie16EndRun. On completion, final histogram and
 * statistics data will be available.
 *
 * ModNum is the module number which starts counting at 0. If ModNum is set to be less than the
 * total number of modules in the system, only the module specified by ModNum will have its
 * histogram run started. But if ModNum is set to be equal to the total number of modules in the
 * system, then all modules in the system will have their runs started together.
 *
 * @param[in] ModNum: The module number that we'd like to work against. Starts counting at 0.
 * @param[in] mode: How we'll handle existing data when starting up the run. Use mode=NEW_RUN (=1)
 *     to erase histograms and statistics information before launching the new
 *     run. Use mode=RESUME_RUN (=0) to resume an earlier run.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid mode
 * @retval -3 - Failed to start histogram run
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16StartHistogramRun(unsigned short ModNum,
                                                              unsigned short mode);

/**
 * @ingroup APP
 * @brief Poll the run status of a Pixie module.
 *
 * Use this function to check the run status of a Pixie-16 module while a list-mode data
 * acquisition run is in progress. If the run is still in progress continue polling.
 *
 * If the return code of this function indicates the run has finished, there might still be some
 * data in the external FIFO (Rev-B, C, D, F modules) that need to be read out to the host
 * computer. In addition, final run statistics and histogram data are available for reading out too.
 *
 * In MCA histogram run mode, this function can also be called to check if the run is still in
 * progress even though it is normally self-terminating.
 *
 * @param[in] ModNum: The module number to interrogate and counting starts at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - No run is in progress
 * @retval  1 - Run is still in progress
 * @retval -1 - Invalid Pixie module number
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CheckRunStatus(unsigned short ModNum);

/**
 * @ingroup APP
 * @brief Stop the run in a Pixie module.
 *
 * Use this function to end a histogram run, or to force the end of a list-mode run. In a
 * multi-module system, if all modules are running synchronously, only one module needs to be
 * addressed this way. It will immediately stop the run in all other module in the system.
 *
 * When stopping the run in the Director module (module #0) - a SYNC interrupt should be generated
 * to stop run in all modules simultaneously, if DSP parameter SynchWait is set to 1.
 * If SynchWait is set to 0, then CSR bit 0 is cleared to stop the run.
 *
 * @param[in] ModNum: The module number to interrogate and counting starts at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - No run is in progress
 * @retval -1 - Invalid Pixie module number
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16EndRun(unsigned short ModNum);

/**
 * @ingroup APP
 * @brief Compute input count rate of a channel
 *
 * Use this function to calculate the input count rate on one channel of a Pixie-16 module. This
 * function does not communicate with Pixie-16 modules. Before calling this function, another
 * function, Pixie16ReadStatisticsFromModule, should be called to read statistics data from the
 * module.
 *
 * The *Statistics array is filled with data from a Pixie-16 module after calling function
 * Pixie16ReadStatisticsFromModule.
 *
 * @note The units on the return value are triggers per second.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics: A pointer to the statistics array whose size is exactly 448 unsigned
 *    integer words (32-bit).
 * @param[in] ModNum: ModNum is the module number which starts counting at 0.
 * @param[in] ChanNum: ChanNum is the channel number which starts counting at 0.
 * @return 0 if the live time was 0. The number of triggers divided by the live time in seconds
 *     otherwise.
 */
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeInputCountRate(unsigned int* Statistics,
                                                                     unsigned short ModNum,
                                                                     unsigned short ChanNum);

/**
 * @ingroup APP
 * @brief Compute output count rate of a channel
 *
 * Use this function to calculate the output count rate on one channel of a Pixie-16 module. This
 * function does not communicate with Pixie-16 modules. Before calling this function, another
 * function, Pixie16ReadStatisticsFromModule, should be called to read statistics data from the
 * module.
 *
 * The *Statistics array is filled with data from a Pixie-16 module after calling function
 * Pixie16ReadStatisticsFromModule.
 *
 * @note The return value's units are channel events per second.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics: A pointer to the statistics array whose size is exactly 448 unsigned
 *    integer words (32-bit).
 * @param[in] ModNum: ModNum is the module number which starts counting at 0.
 * @param[in] ChanNum: ChanNum is the channel number which starts counting at 0.
 * @return 0 if the live time was 0. Otherwise, the number of channel events divided by the
 *     live time in seconds.
 */
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeOutputCountRate(unsigned int* Statistics,
                                                                      unsigned short ModNum,
                                                                      unsigned short ChanNum);

/**
 * @ingroup APP
 * @brief Compute live time that a channel accumulated in a run
 *
 * Use this function to calculate the live time that one channel of a Pixie-16 module has spent on
 * data acquisition. This function does not communicate with Pixie-16 modules. Before calling this
 * function, another function, Pixie16ReadStatisticsFromModule, should be called to read
 * statistics data from the module.
 *
 * The *Statistics array is filled with data from a Pixie-16 module after calling function
 * Pixie16ReadStatisticsFromModule.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics: A pointer to the statistics array whose size is exactly 448 unsigned
 *    integer words (32-bit).
 * @param[in] ModNum: ModNum is the module number, which starts counting at 0.
 * @param[in] ChanNum: ChanNum is the channel number, which starts counting at 0.
 * @return The live time of the module in seconds.
 */
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeLiveTime(unsigned int* Statistics,
                                                               unsigned short ModNum,
                                                               unsigned short ChanNum);

/**
 * @ingroup APP
 * @brief Compute number of events processed by a module
 *
 * @note This function is only used by Rev-A modules.
 *
 * Use this function to calculate the number of events that have been processed by a Pixie-16
 * module during a data acquisition run. This function does not communicate with Pixie-16 modules.
 * Before calling this function, another function, Pixie16ReadStatisticsFromModule, should be
 * called to read statistics data from the module first.
 *
 * The *Statistics array is filled with data from a Pixie-16 module after calling function
 * Pixie16ReadStatisticsFromModule.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics: A pointer to the statistics array whose size is exactly 448 unsigned
 *    integer words (32-bit).
 * @param[in] ModNum: ModNum is the module number, which starts counting at 0.
 * @return The number of events processed by the module.
 */
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeProcessedEvents(unsigned int* Statistics,
                                                                      unsigned short ModNum);

/**
 * @ingroup APP
 * @brief Compute real time that a module accumulated in a run
 *
 * Use this function to calculate the real time that a Pixie-16 module has spent on data
 * acquisition.This function does not communicate with Pixie-16 modules.
 * Before calling this function, another function, Pixie16ReadStatisticsFromModule, should be
 * called to read statistics data from the module first.
 *
 * The *Statistics array is filled with data from a Pixie-16 module after calling function
 * Pixie16ReadStatisticsFromModule.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics: A pointer to the statistics array whose size is exactly 448 unsigned
 *    integer words (32-bit).
 * @param[in] ModNum: The module number, which starts counting at 0.
 * @return The number of seconds that the module spent on data acquisition.
 */
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeRealTime(unsigned int* Statistics,
                                                               unsigned short ModNum);

/**
 * @ingroup APP
 * @brief Computes the Complex FFT of a trace using Cooley-Turkey algorithm
 *
 * Implementation based on Numerical Recipes in C.
 *
 * @see https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm
 *
 * @param[in,out] data: A pointer to an array containing the data we'll transform with the
 *    complex FFT. This array is overwritten with the results of the transformation.
 * @param[in] length: The length of the data array, which **must** be a power of 2. Ex. 8192, 16384
 * @returns Zero, always.
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16complexFFT(double* data, unsigned int length);

/**
 * @ingroup APP
 * @brief Test a bit in a 16-bit unsigned integer.
 * @param[in] bit: The bit position to test, which starts counting at 0.
 * @param[in] value: A 16-bit number that we'll test to see if the requested bit is true.
 * @return 0 if the bit was set, 1 otherwise.
 */
PIXIE16APP_EXPORT unsigned short PIXIE16APP_API APP16_TstBit(unsigned short bit,
                                                             unsigned short value);

/**
 * @ingroup APP
 * @brief Set a bit in a 16-bit unsigned integer.
 * @param[in] bit: The bit position to test, which starts counting at 0.
 * @param[in] value: A 16-bit number that will have its bit set to true.
 * @return The new integer after the bit has been toggled.
 */
PIXIE16APP_EXPORT unsigned short PIXIE16APP_API APP16_SetBit(unsigned short bit,
                                                             unsigned short value);

/**
 * @ingroup APP
 * @brief Clear a bit in a 16-bit unsigned integer.
 * @param[in] bit: The bit position to test, which starts counting at 0.
 * @param[in] value: A 16-bit number that will have its bit set to false.
 * @return The new integer after the bit has been toggled.
 */
PIXIE16APP_EXPORT unsigned short PIXIE16APP_API APP16_ClrBit(unsigned short bit,
                                                             unsigned short value);

/**
 * @ingroup APP
 * @brief Set a bit in a 32-bit unsigned integer.
 * @param[in] bit: The bit position to test, which starts counting at 0.
 * @param[in] value: A 32-bit number that will have its bit set to true.
 * @return The new integer after the bit has been toggled.
 */
PIXIE16APP_EXPORT unsigned int PIXIE16APP_API APP32_SetBit(unsigned short bit, unsigned int value);

/**
 * @ingroup APP
 * @brief Clear a bit in a 32-bit unsigned integer.
 * @param[in] bit: The bit position to test, which starts counting at 0.
 * @param[in] value: A 32-bit number that will have its bit set to false.
 * @return The new integer after the bit has been toggled.
 */
PIXIE16APP_EXPORT unsigned int PIXIE16APP_API APP32_ClrBit(unsigned short bit, unsigned int value);

/**
 * @ingroup APP
 * @brief Test a bit in a 32-bit unsigned integer.
 * @param[in] bit: The bit position to test, which starts counting at 0.
 * @param[in] value: A 32-bit number that we'll test to see if the requested bit is true.
 * @return 0 if the bit was set, 1 otherwise.
 */
PIXIE16APP_EXPORT unsigned int PIXIE16APP_API APP32_TstBit(unsigned short bit, unsigned int value);

/**
 * @ingroup APP
 * @brief Set all DACs.
 *
 * Use this function to reprogram the on-board digital to analog converters (DAC) of the Pixie-16
 * modules. In this operation the DSP uses data from the DSP parameters that were previously
 * downloaded.
 *
 * @param[in] ModNum: The module number, which starts counting at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to start the SET_DACs run
 * @retval -3 - SET_DACs run timed out after 10 seconds of trying.
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SetDACs(unsigned short ModNum);

/**
 * @ingroup APP
 * @brief Program Fippi
 *
 * Use this function to program the on-board signal processing FPGAs of the Pixie-16 modules.
 * After the host computer has written the DSP parameters to the DSP memory, the DSP needs to
 * write some of these parameters to the FPGAs. This function makes the DSP perform that action.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to start the PROGRAM_FIPPI run
 * @retval -3 - PROGRAM_FIPPI run timed out after 10 seconds of trying.
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ProgramFippi(unsigned short ModNum);

/**
 * @ingroup APP
 * @brief Adjust DC-Offsets
 *
 * Use this function to adjust the DC-offsets of Pixie-16 modules. Specify the module using ModNum
 * which starts counting at 0. If ModNum is set to be less than the total number of modules in the
 * system, only the module specified by ModNum will have its DC-offsets adjusted. But if ModNum is
 * set to be equal to the total number of modules in the system, then all modules in the system
 * will have their DC-offsets adjusted.
 *
 * After the DC-offset levels have been adjusted, the baseline level of the digitized input
 * signals will be determined by the DSP parameter BaselinePercent. For instance, if
 * BaselinePercent is set to 10(%), the baseline level of the input signals will be ~ 409 on the
 * 12-bit ADC scale (minimum: 0; maximum: 4095).
 *
 * The main purpose of this function is to ensure the input signals fall within the voltage range
 * of the ADCs so that all input signals can be digitized by the ADCs properly.
 *
 * @param[in] ModNum: The module number to adjust baselines for
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to start the ADJUST_OFFSETS run
 * @retval -3 - ADJUST_OFFSETS run timed out after 10 seconds of attempts
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AdjustOffsets(unsigned short ModNum);

/**
 * @ingroup APP
 * @brief Acquire baselines
 *
 * Use this function to acquire baselines from Pixie-16 modules. Specify the module using ModNum
 * which starts counting at 0. If ModNum is set to be less than the total number of modules in the
 * system, only the module specified by ModNum will have its baselines acquired. But if ModNum is
 * set to be equal to the total number of modules in the system, then all modules in the system
 * will have their baselines acquired.
 *
 * After the successful return of this function, the DSP’s internal memory will be filled with
 * baselines data. Users should then call the function Pixie16ReadSglChanBaselines to read the
 * baselines data out to the host computer, channel by channel.
 *
 * @see Pixie16ReadSglChanBaselines
 *
 * @param[in] ModNum: The module number to get baselines from
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to start the GET_BASELINES run
 * @retval -3 - GET_BASELINES run timed out
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AcquireBaselines(unsigned short ModNum);

/**
 * @ingroup APP
 * @brief Read baselines from a channel in a module
 *
 * Use this function to read baseline data from a Pixie-16 module. Before calling this function,
 * another function Pixie16AcquireBaselines should be called to fill the DSP internal memory first.
 * Also, the host code should allocate appropriate amount of memory to store the baseline data.
 * The number of baselines for each channel is 3640. In the DSP internal memory, each baseline is
 * a 32-bit IEEE floating point number. After being read out to the host, this function will
 * convert each baseline data to a decimal number. In addition to baseline values, timestamps
 * corresponding to each baseline are also returned after this function call.
 *
 * @see Pixie16AcquireBaselines
 *
 * @param[out] Baselines: Buffer to hold the baseline values
 * @param[in] TimeStamps: The timestamp that the baselines were obtained at
 * @param[in] NumBases: The number of baselines values read
 * @param[in] ModNum: The module number to read the baselines from, counting from 0.
 * @param[in] ChanNum: The channel number to read the baselines from, counting from 0.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Requested number of baselines exceeded the limit
 * @retval -3 - Failed to allocate memory to store baselines
 * @retval -4 - Failed to read baselines
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanBaselines(double* Baselines,
                                                                 double* TimeStamps,
                                                                 unsigned short NumBases,
                                                                 unsigned short ModNum,
                                                                 unsigned short ChanNum);

/**
 * @ingroup APP
 * @brief Ramp Offset DACs of a module and record the baselines
 *
 * @note However, this function is no longer needed due to the introduction of function
 *     Pixie16AdjustOffsets.
 *
 * Use this function to execute the RAMP_OFFSETDACS control task run. Each Offset DAC has 65536
 * steps, and the RAMP_OFFSETDACS control task ramps the DAC from 0 to 65335 with a step size of
 * 64, i.e., a total of 1024 steps. At each DAC step, the control task computes the baseline value
 * as the representation of the signal baseline and stores it in the DSP memory. After the control
 * task is finished, the stored baseline values are read out to the host computer and saved to a
 * binary file called “rampdacs.bin” in the form of IEEE 32-bit floating point numbers. Users can
 * then plot the baseline values vs. DAC steps to determine the appropriate DAC value to be set in
 * the DSP in order to bring the input signals into the voltage range of the ADCs. However, this
 * function is no longer needed due to the introduction of function Pixie16AdjustOffsets.
 *
 * If ModNum is set to less than the total number of modules in the system, only the module
 * specified by ModNum will start the RAMP_OFFSETDACS control task run. But if ModNum is equal to
 * the total number of modules in the system, e.g. there are 5 modules in the chassis and
 * ModNum = 5, then all modules in the system will start the RAMP_OFFSETDACS control task run.
 *
 * @see Pixie16AdjustOffsets
 *
 * @param[out] DCValues: The final values for the DC offsets
 * @param[in] NumDCVals: The number of DC values to read
 * @param[in] ModNum: The module number we'll query for the DACs, counting starts at 0.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Null pointer *DCValues
 * @retval -3 - requested number of DC values exceeded the limit
 * @retval -4 - Failed to start the RAMP_OFFSETDACS run
 * @retval -5 - RAMP_OFFSETDACS run timed out
 * @retval -6 - Failed to read DC values
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16RampOffsetDACs(double* DCValues,
                                                           unsigned short NumDCVals,
                                                           unsigned short ModNum);

/**
 * @ingroup APP
 * @brief  Execute special control tasks
 *
 * Use this function to execute special control tasks. This may include programming the Fippi or
 * setting the DACs after downloading DSP parameters.
 *
 * @see Control Tasks
 *
 * @param[in] ModNum: The module number to execute the control task against. Starts counting with 0.
 * @param[in] ControlTask : The control task number that we're going to execute.
 * @param[in] Max_Poll: The time in milliseconds that we should wait before considering the task
 *   timed out.
 * @returns A status code indicating the result of the operation
 * @retval   0 - successful
 * @retval  -1 - Invalid Pixie module number
 * @retval  -2 - control task run failed
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ControlTaskRun(unsigned short ModNum,
                                                           unsigned short ControlTask,
                                                           unsigned int Max_Poll);

/**
 * @ingroup APP
 * @brief Find the BLcut value for the selected channel and return it using pointer  *BLcut.
 *
 * Use this function to find the Baseline Cut value for one channel of a Pixie-16 module. The
 * baseline cut value is then downloaded to the DSP, where baselines are captured and averaged
 * over time. The cut value would prevent a bad baseline value from being used in the averaging
 * process, i.e., if a baseline value is outside the baseline cut range, it will not be used for
 * computing the baseline average. Averaging baselines over time improves energy resolution
 * measurement.
 *
 * @param[in] ModNum: The module number to work with, starts counting at 0.
 * @param[in] ChanNum: The channel number to work with, starts counting at 0.
 * @param[out] BLcut: The Baseline cut as determined by the calculation.
 * @returns A status code indicating the result of the operation
 * @retval  0 - successful
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - failed to collect baselines
 * @retval -3 - failed to read baselines from the data memory
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16BLcutFinder(unsigned short ModNum,
                                                        unsigned short ChanNum,
                                                        unsigned int* BLcut);

/**
 * @ingroup APP
 * @brief Find the exponential decay constant of the detector/preamplifier signals.
 *
 * Use this function to find the exponential decay time constants (Tau value) of the detector or
 * preamplifier signal that is connected to each of the 16 channels of a Pixie-16 module. Tau
 * is used as part of the on-board energy calculations.
 *
 * @see [High Rate Pulse Processing Algorithms for Microcalorimeters](https://xia.com/wp-content/uploads/2021/03/High-Rate-Pulse-Processing-Algorithms-for-Microcalorimeters-Tan-2008-.pdf)
 *
 * @param[in] ModNum: The module number to work with. Starts counting at 0.
 * @param[out] Tau: Pointer to the array containing the calculated tau values. A value of -1.0
 *     indicates a failed calculation for that channel. Units are microseconds.
 * @returns A status code indicating the result of the operation
 * @retval  0 - success
 * @retval -1 - invalid Pixie module number
 * @retval -2 - failed to start TAU_FINDER run
 * @retval -3 - TAU_FINDER run timed out
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16TauFinder(unsigned short ModNum, double* Tau);

/**
 * @ingroup APP
 * @brief Write a module parameter to a Pixie module
 *
 * #### List of module parameters
 * | Name | Unit | Type | DSP Name |
 * |-|-|-|-|
 * | MODULE_CSRA | bit pattern | user set | MODCSRA |
 * | MODULE_CSRB | bit pattern | user set | MODCSRB |
 * | MODULE_FORMAT | None | auto DSP | MODFORMAT |
 * | MAX_EVENTS | None | user set/auto DSP | MAXEVENTS |
 * | SYNCH_WAIT | logic (0, 1) | user set | SYNCHWAIT |
 * | IN_SYNCH | logic (0, 1) | user/DSP set | INSYNCH |
 * | SLOW_FILTER_RANGE | None | user set | SLOWFILTERRANGE |
 * | FAST_FILTER_RANGE | None | user set | FASTFILTERRANGE |
 * | FastTrigBackplaneEna | bit pattern | user set | FASTTRIGBACKPLANEENA |
 * | CrateID | None | user set | CRATEID |
 * | SlotID | None | user set | SLOTID |
 * | ModID | None | user set | MODID |
 * | TrigConfig0 | None | user set | TRIGCONFIG[0] |
 * | TrigConfig1 | None | user set | TRIGCONFIG[1] |
 * | TrigConfig2 | None | user set | TRIGCONFIG[2] |
 * | TrigConfig3 | None | user set | TRIGCONFIG[3] |
 * | HOST_RT_PRESET | seconds | user set | HOSTRUNTIMEPRESET |
 *
 * @param[in] ModParName: The name of the module parameter that we'll write
 * @param[in] ModParData: The value of the module parameter that we'll write
 * @param[in] ModNum: The module number we'll write to. Numbering starts counting at 0.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid module parameter name
 * @retval -3 - Failed to program Fippi after downloading module parameter
 * @retval -4 - Failed to find BLcut after downloading module parameter
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteSglModPar(const char* ModParName,
                                                           unsigned int ModParData,
                                                           unsigned short ModNum);


/**
 * @ingroup APP
 * @brief Read a module parameter from a Pixie module
 *
 * #### List of module parameters
 * | Name | Unit | Type | DSP Name |
 * |-|-|-|-|
 * | MODULE_CSRA | bit pattern | user set | MODCSRA |
 * | MODULE_CSRB | bit pattern | user set | MODCSRB |
 * | MODULE_FORMAT | None | auto DSP | MODFORMAT |
 * | MAX_EVENTS | None | user set/auto DSP | MAXEVENTS |
 * | SYNCH_WAIT | logic (0, 1) | user set | SYNCHWAIT |
 * | IN_SYNCH | logic (0, 1) | user/DSP set | INSYNCH |
 * | SLOW_FILTER_RANGE | None | user set | SLOWFILTERRANGE |
 * | FAST_FILTER_RANGE | None | user set | FASTFILTERRANGE |
 * | FastTrigBackplaneEna | bit pattern | user set | FASTTRIGBACKPLANEENA |
 * | CrateID | None | user set | CRATEID |
 * | SlotID | None | user set | SLOTID |
 * | ModID | None | user set | MODID |
 * | TrigConfig0 | None | user set | TRIGCONFIG[0] |
 * | TrigConfig1 | None | user set | TRIGCONFIG[1] |
 * | TrigConfig2 | None | user set | TRIGCONFIG[2] |
 * | TrigConfig3 | None | user set | TRIGCONFIG[3] |
 * | HOST_RT_PRESET | seconds | user set | HOSTRUNTIMEPRESET |
 *
 * @param[in] ModParName: The name of the module parameter that we'll read
 * @param[out] ModParData: Contains the value of the requested parameters.
 * @param[in] ModNum: The module number we'll write to. Numbering starts counting at 0.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid module parameter name
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglModPar(const char* ModParName,
                                                          unsigned int* ModParData,
                                                          unsigned short ModNum);

/**
 * @ingroup APP
 * @brief Write a channel parameter to a Pixie module.
 *
 * #### List of Channel Parameters
 * | Name | Unit | Type | DSP Name |
 * |-|-|-|-|
 * | TRIGGER_RISETIME | &mu;s | user set | FASTLENGTH |
 * | TRIGGER_FLATTOP | &mu;s | user set | FASTGAP |
 * | TRIGGER_THRESHOLD | ADC units | user set | FASTTHRESH |
 * | ENERGY_RISETIME | &mu;s | user set | SLOWLENGTH |
 * | ENERGY_FLATTOP | &mu;s | user set | SLOWGAP |
 * | TAU | &mu;s | user set | PREAMPTAU |
 * | TRACE_LENGTH | &mu;s | user set | TRACELENGTH |
 * | TRACE_DELAY | &mu;s | user set | TRIGGERDELAY, PAFLENGTH |
 * | VOFFSET | V | user set | OFFSETDAC |
 * | XDT | &mu;s | user set | XWAIT |
 * | BASELINE_PERCENT | % | user set | BASELINEPERCENT |
 * | EMIN | None | user set | ENERGYLOW |
 * | BINFACTOR | None | user set | LOG2EBIN |
 * | BASELINE_AVERAGE | None | user set | LOG2BWEIGHT |
 * | CHANNEL_CSRA | bit pattern | user set | CHANCSRA |
 * | CHANNEL_CSRB | bit pattern | user set | CHANCSRB |
 * | BLCUT | None | user set/auto API | BLCUT |
 * | INTEGRATOR | None | user set | INTEGRATOR |
 * | FASTTRIGBACKLEN | &mu;s | user set | FASTTRIGBACKLEN |
 * | CFDDelay | &mu;s | user set | CFDDELAY |
 * | CFDScale | None | user set | CFDSCALE |
 * | CFDThresh | ADC units | user set | CFDTHRESH |
 * | QDCLen0 | &mu;s | user set | QDCLEN0 |
 * | QDCLen1 | &mu;s | user set | QDCLEN1 |
 * | QDCLen2 | &mu;s | user set | QDCLEN2 |
 * | QDCLen3 | &mu;s | user set | QDCLEN3 |
 * | QDCLen4 | &mu;s | user set | QDCLEN4 |
 * | QDCLen5 | &mu;s | user set | QDCLEN5 |
 * | QDCLen6 | &mu;s | user set | QDCLEN6 |
 * | QDCLen7 | &mu;s | user set | QDCLEN7 |
 * | ExtTrigStretch | &mu;s | user set | EXTTRIGSTRETCH |
 * | VetoStretch | &mu;s | user set | VETOSTRETCH |
 * | MultiplicityMaskL | bit pattern | user set | MULTIPLICITYMASKL |
 * | MultiplicityMaskH | bit pattern | user set | MULTIPLICITYMASKH |
 * | ExternDelayLen | &mu;s | user set | EXTERNDELAYLEN |
 * | FtrigoutDelay | &mu;s | user set | FTRIGOUTDELAY |
 * | ChanTrigStretch | &mu;s | user set | CHANTRIGSTRETCH |
 *
 * @param[in] ChanParName: The name of the channel parameter that we'll write
 * @param[in] ChanParData: The value that we'll write to the provided channel
 * @param[in] ModNum: The module number we'll write to. Counting starts at 0.
 * @param[in] ChanNum: The channel number we'll write to. Counting starts at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid Pixie channel number
 * @retval -3 - Invalid channel parameter name
 * @retval -4 - Programing Fippi failed downloading channel parameter
 * @retval -5 - BLcutFinder failed downloading channel parameter
 * @retval -6 - SetDACs failed downloading channel parameter
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteSglChanPar(const char* ChanParName,
                                                            double ChanParData,
                                                            unsigned short ModNum,
                                                            unsigned short ChanNum);


/**
 * @ingroup APP
 * @brief Read a channel parameter from a module.
 *
 * #### List of Channel Parameters
 * | Name | Unit | Type | DSP Name |
 * |-|-|-|-|
 * | TRIGGER_RISETIME | &mu;s | user set | FASTLENGTH |
 * | TRIGGER_FLATTOP | &mu;s | user set | FASTGAP |
 * | TRIGGER_THRESHOLD | ADC units | user set | FASTTHRESH |
 * | ENERGY_RISETIME | &mu;s | user set | SLOWLENGTH |
 * | ENERGY_FLATTOP | &mu;s | user set | SLOWGAP |
 * | TAU | &mu;s | user set | PREAMPTAU |
 * | TRACE_LENGTH | &mu;s | user set | TRACELENGTH |
 * | TRACE_DELAY | &mu;s | user set | TRIGGERDELAY, PAFLENGTH |
 * | VOFFSET | V | user set | OFFSETDAC |
 * | XDT | &mu;s | user set | XWAIT |
 * | BASELINE_PERCENT | % | user set | BASELINEPERCENT |
 * | EMIN | None | user set | ENERGYLOW |
 * | BINFACTOR | None | user set | LOG2EBIN |
 * | BASELINE_AVERAGE | None | user set | LOG2BWEIGHT |
 * | CHANNEL_CSRA | bit pattern | user set | CHANCSRA |
 * | CHANNEL_CSRB | bit pattern | user set | CHANCSRB |
 * | BLCUT | None | user set/auto API | BLCUT |
 * | INTEGRATOR | None | user set | INTEGRATOR |
 * | FASTTRIGBACKLEN | &mu;s | user set | FASTTRIGBACKLEN |
 * | CFDDelay | &mu;s | user set | CFDDELAY |
 * | CFDScale | None | user set | CFDSCALE |
 * | CFDThresh | ADC units | user set | CFDTHRESH |
 * | QDCLen0 | &mu;s | user set | QDCLEN0 |
 * | QDCLen1 | &mu;s | user set | QDCLEN1 |
 * | QDCLen2 | &mu;s | user set | QDCLEN2 |
 * | QDCLen3 | &mu;s | user set | QDCLEN3 |
 * | QDCLen4 | &mu;s | user set | QDCLEN4 |
 * | QDCLen5 | &mu;s | user set | QDCLEN5 |
 * | QDCLen6 | &mu;s | user set | QDCLEN6 |
 * | QDCLen7 | &mu;s | user set | QDCLEN7 |
 * | ExtTrigStretch | &mu;s | user set | EXTTRIGSTRETCH |
 * | VetoStretch | &mu;s | user set | VETOSTRETCH |
 * | MultiplicityMaskL | bit pattern | user set | MULTIPLICITYMASKL |
 * | MultiplicityMaskH | bit pattern | user set | MULTIPLICITYMASKH |
 * | ExternDelayLen | &mu;s | user set | EXTERNDELAYLEN |
 * | FtrigoutDelay | &mu;s | user set | FTRIGOUTDELAY |
 * | ChanTrigStretch | &mu;s | user set | CHANTRIGSTRETCH |
 *
 * @param[in] ChanParName: The name of the channel parameter that we'll read
 * @param[out] ChanParData: Contains the value we read from the channel.
 * @param[in] ModNum: The module number we'll read from. Counting starts at 0.
 * @param[in] ChanNum: The channel number we'll read from. Counting starts at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid Pixie channel number
 * @retval -3 - Invalid channel parameter name
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanPar(const char* ChanParName,
                                                           double* ChanParData,
                                                           unsigned short ModNum,
                                                           unsigned short ChanNum);

/**
 * @ingroup APP
 * @brief Retrieve a channel's histogram data from a Pixie module.
 *
 * Use this function to read out the histogram data from a Pixie-16 module’s histogram memory.
 * Before calling this function, the host code should allocate appropriate amount of memory to
 * store the histogram data. The default histogram length is 32768. Histogram data are 32-bit
 * unsigned integers.
 *
 * @param[out] Histogram: The histogram data that we read from the module.
 * @param[in] NumWords: The number of words we'll read out of the histogram memory.
 * @param[in] ModNum: The module number that we want the histogram from
 * @param[in] ChanNum: The channel number that we'd like to read from
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid Pixie channel number
 * @retval -3 - Failed to get the histogram data. If this happens reboot the module.
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadHistogramFromModule(unsigned int* Histogram,
                                                                    unsigned int NumWords,
                                                                    unsigned short ModNum,
                                                                    unsigned short ChanNum);

/**
 * @ingroup APP
 * @brief Retrieve statistics data from a module.
 *
 * Use this function to read out statistics data from a Pixie-16 module. Before calling this
 * function, the host code should allocate appropriate amount of memory to store the statistics
 * data. The number of statistics data for each module is fixed at 448. Statistics data are 32-bit
 * unsigned integers.
 *
 * @param[out] Statistics: Pointer to a data block with size 448 32-bit unsigned integers to hold
 *    the data read from the module.
 * @param[in] ModNum: The module number that we want statistics data from.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to get statistics data. If this happens, then reboot the module.
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadStatisticsFromModule(unsigned int* Statistics,
                                                                     unsigned short ModNum);


/**
 * @ingroup APP
 * @brief Retrieve histogram data from a Pixie module and then save the data to a file.
 *
 * Use this function to read histogram data from a Pixie-16 module and save the histogram data to
 * a file with file name specified by the user. First this function saves the histogram data to a
 * binary file, and it then saves the histogram data to an ASCII file with run statistics data
 * appended to the end of the ASCII file. **Existing files will be overwritten.**
 *
 * @param[in] FileName: The file name for the file containing the histogram data
 * @param[in] ModNum: The module number we'll read histogram data from. Counting from 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to get histogram data from module
 * @retval -3 - Failed to allocate memory to store histogram data for binary file
 * @retval -4 - Failed to open histogram data file
 * @retval -5 - Failed to open mca ascii output file
 * @retval -6 - Failed to allocate memory to store histogram data for ascii text file
 * @retval -7 - Failed to read histogram data from file
 * @retval -8 - Failed to read run statistics data from module
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveHistogramToFile(const char* FileName,
                                                                unsigned short ModNum);


/**
 * @ingroup APP
 * @brief Parse list-mode events from the data file to get the number of events for each module.
 *
 * The data file must be written with the Pixie16SaveExternalFIFODataToFile to ensure the proper
 * format.
 *
 * @see Pixie16SaveExternalFIFODataToFile
 *
 * @param[in] FileName: An absolute path to the file that we'll parse for events written with
 *     Pixie16SaveExternalFIFODataToFile.
 * @param[out] ModuleEvents: A pointer to an array containing the number of events each module
 *     recorded.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Null pointer *ModuleEvents
 * @retval -2 - Failed to open list-mode data file
 * @retval -3 - Failed to seek to the end of the file.
 * @retval -4 - Failed to seek to beginning of file.
 * @retval -5 - Failed to read event from file
 * @retval -6 - Failed to find next event in file.
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16GetModuleEvents(const char* FileName,
                                                            unsigned int* ModuleEvents);


/**
 * @ingroup APP
 * @brief Retrieve the detailed information of each event in the list-mode data file
 *
 * Use this function to retrieve the detailed information (except waveforms) of each event in the
 * list-mode data file for the designated module.
 * Before calling this function to get the individual events information, another function
 * Pixie16GetModuleEvents should be called first to determine the number of events that have been
 * recorded for each module. If the number of events for a given module is nEvents, a memory
 * block `*EventInformation` should be allocated with a length of (`nEvents*68`):
 * ```
 * EventInformation = (unsigned int*)malloc(sizeof(unsigned int) * nEvents * 68);
 * ```
 * where 68 is the length of the information records of each event (energy, timestamps, etc.) and
 * has the following structure.
 *
 * | Index | Value |
 * |-|-|
 * | EventInformation [0] | Event number  |
 * | EventInformation [1] | Channel number |
 * | EventInformation [2] | Slot number |
 * | EventInformation [3] | Crate number |
 * | EventInformation [4] | Header length |
 * | EventInformation [5] | Event length |
 * | EventInformation [6] | Finish code |
 * | EventInformation [7] | Event timestamp (lower 32-bit) |
 * | EventInformation [8] | Event timestamp (upper 16-bit) |
 * | EventInformation [9] | Event energy |
 * | EventInformation [10] | Trace length |
 * | EventInformation [11] | Trace location |
 * | EventInformation [67:12] | Not used |
 *
 * @see Pixie16SaveExternalFIFODataToFile
 * @see Pixie16GetModuleEvents
 *
 * @param[in] FileName: Absolute path to the file containing list-mode data written with
 *     Pixie16SaveExternalFIFODataToFile
 * @param[out] EventInformation: Pointer to an array with size `number_of_events  * 68`.
 * @param[in] ModuleNumber: The module number that we'll retrieve information about
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Null pointer  *EventInformation
 * @retval -2 - Target module number is invalid
 * @retval -3 - Failed to open list-mode data file
 * @retval -4 - Failed to seek to the end of the file.
 * @retval -5 - Failed to seek to beginning of file.
 * @retval -6 - Failed to read Word 0 from the event
 * @retval -7 - Failed to read Word 1 from the event
 * @retval -8 - Failed to read Word 2 from the event
 * @retval -9 - Failed to read Word 3 from the event
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16GetEventsInfo(const char* FileName,
                                                          unsigned int* EventInformation,
                                                          unsigned short ModuleNumber);


/**
 * @ingroup APP
 * @brief Retrieve a trace from list-mode data in a file.
 *
 * It uses the trace length and file location information obtained from function
 * Pixie16GetEventsInfo for the selected event. This function will only retrieve a single trace at
 * a time.
 *
 * @see Pixie16GetEventsInfo
 * @see Pixie16SaveExternalFIFODataToFile
 *
 * @param[in] FileName: Absolute path to the list-mode data file written with
 *     Pixie16SaveExternalFIFODataToFile
 * @param[out] Trace_Data: A pointer to an array of 16-bit unsigned integers that will hold the
 *     traces. The array should have enough elements to hold NumWords entries.
 * @param[in] NumWords: The number of 16-bit unsigned integers we'll read from the data file.
 * @param[in] FileLocation: The location of the trace in the file.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Failed to open list-mode data file
 * @retval -2 - Failed to seek to the provided trace location
 * @retval -3 - Failed to read the trace from the file
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadListModeTrace(const char* FileName,
                                                              unsigned short* Trace_Data,
                                                              unsigned short NumWords,
                                                              unsigned int FileLocation);

/**
 * @ingroup APP
 * @brief Retrieve histogram data from a file.
 *
 * Use this function to read histogram data from a histogram data file. Before calling this
 * function, the host code should allocate appropriate amount of memory to store the histogram
 * data. The default histogram length is 32768. Histogram data are 32-bit unsigned integers.
 *
 * @see Pixie16SaveHistogramToFile
 *
 * @param[in] FileName: Absolute path to the list-mode data file written with
 *     Pixie16SavehistogramToFile
 * @param[out] Histogram: Pointer to an array of 32-bit unsigned integers to hold the
 *     histogram data read from the file. It should contain `NumWords` elements.
 * @param[in] NumWords: The number of words to be read out.
 * @param[in] ModNum: The module number that we'll process with counting starting at 0.
 * @param[in] ChanNum: The channel number that we'll process with counting starting at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Failed to open the histogram file
 * @retval -2 - No histogram data is available for this channel
 * @retval -3 - Failed to seek to the end of the histogram file
 * @retval -4 - Failed to seek to the beginning of the histogram file
 * @retval -5 - Failed to seek to the histogram data in the file
 * @retval -6 - Failed to read the histogram data from the file.
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadHistogramFromFile(const char* FileName,
                                                                  unsigned int* Histogram,
                                                                  unsigned int NumWords,
                                                                  unsigned short ModNum,
                                                                  unsigned short ChanNum);

/**
 * @ingroup APP
 * @brief Save DSP parameters to a settings file
 *
 * Use this function to save DSP parameters to a settings file. It will first read the values of
 * DSP parameters on each Pixie-16 module and then write them to the settings file. Each module has
 * exactly 1280 DSP parameter values (32-bit unsigned integers), and depending on the value of
 * PRESET_MAX_MODULES (defined in pixie16app_defs.h), the settings file should have exactly
 * `(1280 * PRESET_MAX_MODULES * 4)` bytes when stored on the computer hard drive.
 *
 * @param[in] FileName: Absolute path to where we will write the DSP parameter file.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Failed to read DSP parameter values from the Pixie-16 modules
 * @retval -2 - Failed to open the DSP parameters file
 * @retval -3 - Failed to write DSP parameter values
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveDSPParametersToFile(const char* FileName);


/**
 * @ingroup APP
 * @brief Load DSP parameters from a settings file
 *
 * Use this function to read DSP parameters from a settings file and then download the settings to
 * Pixie-16 modules that are installed in the system. Each module has exactly 1280 DSP parameter
 * values (32-bit unsigned integers), and depending on the value of PRESET_MAX_MODULES
 * (defined in pixie16app_defs.h), the settings file should have exactly
 * `(1280 * PRESET_MAX_MODULES * 4)` bytes when stored on the computer hard drive.
 * @param[in] FileName: Absolute path to where we will write the DSP parameter file.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Size of DSPParFile is invalid
 * @retval -2 - Failed to program Fippi in module
 * @retval -3 - Failed to set DACs in module
 * @retval -4 - Failed to open the DSP parameters file
 * @retval -5 - Failed to seek to the end of the file
 * @retval -6 - Failed to seek to the beginning of the file
 * @retval -7 - Failed to read settings from the file
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16LoadDSPParametersFromFile(const char* FileName);


/**
 * @ingroup APP
 * @brief Copy DSP parameters from one module to other modules.
 *
 * Use this function to copy DSP parameters from one module to the others that are installed in
 * the system.
 *
 * @param[in] BitMask: A bit pattern that designates what items should be copied from the source
 *     module to the destination module(s). For example, a value of 4097
 *     (2<sup>0</sup> + 2<sup>12</sup>) will copy the filter parameters and QDC.
 *     | Bit | Item |
 *     |-|-|
 *     | 0 | Filter (energy and trigger) |
 *     | 1 | Analog signal conditioning (polarity, dc-offset,   gain/attenuation) |
 *     | 2 | Histogram control (minimum energy, binning factor) |
 *     | 3 | Decay time |
 *     | 4 | Pulse shape analysis (trace length and trace delay) |
 *     | 5 | Baseline control (baseline cut, baseline percentage) |
 *     | 7 | Channel CSRA register (good channel, trigger enabled, etc.)   |
 *     | 8 | CFD trigger (CFD delay, scaling factor) |
 *     | 9 | Trigger stretch lengths (veto, external trigger, etc.) |
 *     | 10 | FIFO delays (analog input delay, fast trigger output delay,   etc.) |
 *     | 11 | Multiplicity (bit masks, thresholds, etc.) |
 *     | 12 | QDC (QDC sum lengths) |
 * @param[in] SourceModule: The module that we'll read the settings from, starts counting at 0.
 * @param[in] SourceChannel: The channel we'll read settings from, starts counting at 0.
 * @param[in] DestinationMask: A pointer to an array that indicates the channel and module whose
 *     settings will be copied from the source channel and module. For instance, if there are 5
 *     modules (total 80 channels) in the system, DestinationMask would be defined as
 *     `DestinationMask[80]`, where `DestinationMask[0]` to `DestinationMask[15]` would select
 *     channel 0 to 15 of module 0, `DestinationMask[16]` to `DestinationMask[31]` would select
 *     channel 0 to 15 of module 1, and so on. If a given channel i is to be copied,
 *     then `DestinationMask[i]` should be set to 1, otherwise, it should be set to 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Failed to program Fippi in a module
 * @retval -2 - Failed to set DACs in a module
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CopyDSPParameters(unsigned short BitMask,
                                                              unsigned short SourceModule,
                                                              unsigned short SourceChannel,
                                                              unsigned short* DestinationMask);

/**
 * @ingroup PUBLIC_UTILITIES
 * @brief Read Pixie-16 message file.
 *
 * This function reads the message file in reverse line-order, and the maximum number of characters
 * to read is 65535.
 *
 * @note exclusively for VB GUI
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Null pointer *ReturnMsgStr
 * @retval -2 - Pixie-16 message file Pixie16msg.txt is empty
 * @retval -3 - Failed to allocate memory
 * @retval -4 - Inconsistent bytes read from the message file
 * @retval -5 - Could not seek to end of Pixie16msg.txt
 * @retval -6 - Could not open Pixie16msg.txt
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadMSGFile(char* ReturnMsgStr);

/**
 * @ingroup PUBLIC_UTILITIES
 * @brief Converts a decimal number to the IEEE 754 standard
 * @see https://en.wikipedia.org/wiki/IEEE_754
 * @param[in] DecimalNumber: The decimal number to convert
 * @returns The input value in IEEE 754 standard format.
 */
PIXIE16APP_EXPORT unsigned int PIXIE16APP_API Decimal2IEEEFloating(double DecimalNumber);

/**
 * @ingroup PUBLIC_UTILITIES
 * @brief Converts a IEEE 754 standard format number to a double.
 * @see https://en.wikipedia.org/wiki/IEEE_754
 * @param[in] IEEEFloatingNumber: The IEEE 754 standard format number to convert
 * @returns The input value in double format.
 */
PIXIE16APP_EXPORT double PIXIE16APP_API IEEEFloating2Decimal(unsigned int IEEEFloatingNumber);

/**
 * @ingroup APP
 * @brief Read list-mode data from the external FIFO of module and save the data to a file.
 *
 * This function first checks the status of the external FIFO of a Pixie-16 module. If the FIFO
 * data exceeds the size of EXTFIFO_READ_THRESH then we read list-mode data (32-bit unsigned
 * integers) from the external FIFO. This function essentially encapsulates both functions
 * Pixie16CheckExternalFIFOStatus and Pixie16ReadDataFromExternalFIFO within one function. The
 * number of words that are read from the external FIFO is recorded in variable `*nFIFOWords`.
 *
 * @see Pixie16CheckExternalFIFOStatus
 * @see Pixie16ReadDataFromExternalFIFO
 * @see EXTFIFO_READ_THRESH
 *
 * @param[in] FileName: Absolute path to the file that we'll write data into
 * @param[out] nFIFOWords: The number of words read from the FIFO and written to the file.
 * @param[in] ModNum: The module number for the module whose external FIFO we want to read.
 * @param[in] EndOfRunRead: Provide a 1 if this is the final read of a run, 0 otherwise. This is
 *     necessary since the external FIFO needs special treatment when the host reads the last few
 *     words from the external FIFO due to its pipelined structure.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to allocate memory to store list-mode data
 * @retval -3 - Failed to open list-mode data file
 * @retval -4 - Failed to read external FIFO status
 * @retval -5 - Failed to read data from external FIFO
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveExternalFIFODataToFile(const char* FileName,
                                                                       unsigned int* nFIFOWords,
                                                                       unsigned short ModNum,
                                                                       unsigned short EndOfRunRead);

/**
 * @ingroup APP
 * @brief Read from or write to a register in a module.
 * @param[in] ModNum: The module number that we'll be performing IO with. Numbered starting at 0.
 * @param[in] address: The memory address that we'd like to access on the module.
 * @param[in] direction: The direction of the access. Either MOD_READ (1) or MOD_WRITE (0)
 * @param[in,out] value: A pointer to a 32-bit unsigned integer (or array of integers) that holds
 *     the IO data.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid module number
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16RegisterIO(unsigned short ModNum, unsigned int address,
                                                       unsigned short direction,
                                                       unsigned int* value);


/**
 * @ingroup APP
 * @brief Read the value of the CSR of a module.
 *
 * @note Direct reading or writing by the host is not recommended!!

 * Use this function to read the host Control & Status Register (CSR) value. This register is
 * unrelated to the DSP parameters ModCSRA/B, ChanCSRA/B. It is used to control the operation of
 * the module and read directly by the host.
 *
 * @param[in] ModNum: The module number that we want to query
 * @param[out] CSR: Pointer to a 32-bit unsigned integer to hold the CSR value.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Successful
 * @retval -1 - Invalid Pixie module number
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadCSR(unsigned short ModNum, unsigned int* CSR);

/**
 * @ingroup APP
 * @brief Write a value to the CSR of a module.
 *
 * @note Direct reading or writing by the host is not recommended!!
 *
 * Use this function to write a value to the host Control & Status Register (CSR). This register
 * is unrelated to the DSP parameters ModCSRA/B, ChanCSRA/B. It is used to control the operation
 * of the module and read directly by the host.
 *
 * @param[in] ModNum
 * @param[in] CSR: A 32-bit unsigned integer that we'll write to the CSR.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Successful
 * @retval -1 - Invalid Pixie module number
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteCSR(unsigned short ModNum, unsigned int CSR);


/**
 * @ingroup APP
 * @brief Read the number of 32-bit words that the external FIFO currently has.
 *
 * Use this function to check the status of the external FIFO of a Pixie-16 module while a
 * list-mode data acquisition run is in progress. The function returns the number of words
 * (32-bit) that the external FIFO currently has. If the number of words is greater than a
 * user-set threshold, function Pixie16ReadDataFromExternalFIFO can then be used to read the data
 * from the external FIFO. The threshold can be set by the user to either minimize reading
 * overhead or to read data out of the FIFO as quickly as possible. The Pixie-16 API
 * (pixie16app_defs.h) has defined a threshold with value of 1024 for external FIFO read out
 * (EXTFIFO_READ_THRESH).
 *
 * @see Pixie16ReadDataFromExternalFIFO
 *
 * @param[out] nFIFOWords: The number of 32-bit unsigned integer words contained in the module's
 *     External FIFO
 * @param[in] ModNum: The module number to read from. Numbering starts counting at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Successful
 * @retval -1 - Invalid Pixie module number
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CheckExternalFIFOStatus(unsigned int* nFIFOWords,
                                                                    unsigned short ModNum);

/**
 * @ingroup APP
 * @brief Read data from the external FIFO.
 *
 * This function reads list mode data from the external FIFO of a Pixie-16 module. The data are
 * 32-bit unsigned integers. Normally, function Pixie16CheckExternalFIFOStatus is called first to
 * see how many words the external FIFO currently has, and then this function is called to read
 * the data from the FIFO.
 *
 * @see Pixie16CheckExternalFIFOStatus
 *
 * @param[out] ExtFIFO_Data: A pointer to a memory block containing enough space for `nFIFOWords`
 *    worth of 32-bit unsigned integers.
 * @param[in] nFIFOWords: The number of words that we're going to be reading.
 * @param[in] ModNum: The module number that we'll read from. Numbering starts at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Successful
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to read data from external FIFO
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadDataFromExternalFIFO(unsigned int* ExtFIFO_Data,
                                                                     unsigned int nFIFOWords,
                                                                     unsigned short ModNum);

/**
 * @ingroup APP
 * @brief Compute the fast-filter response of a trace listed in a list-mode data file.
 *
 * Use this function to compute fast filter response of a single event in the list-mode data file.
 * The algorithm implemented in this offline analysis function is equivalent to
 * the one implemented in the Pixie-16 hardware. So this function can be used to analyze how the
 * leading edge fast trigger filter and the CFD filter implemented in the hardware respond to the
 * shape of recorded traces. By analyzing the response of these filters, it is possible to
 * optimize the performance of the leading-edge trigger or CFD trigger by adjusting the
 * fast-filter and CFD parameters offline. Such optimized parameters can then be saved to a
 * settings file to be used for online data acquisition.
 *
 * This function uses the globally defined configuration arrays to obtain the filter parameters.
 * These parameters need to be loaded from a configuration file using
 * Pixie16LoadDSPParametersFromFile. Users will also need to set the module's offline variant with
 * the function Pixie16SetOfflineVariant.
 *
 * @see Pixie16SaveExternalFIFODataToFile
 * @see Pixie16SetOfflineVariant
 * @see Pixie16LoadDSPParametersFromFile
 *
 * @param[in] FileName: Absolute path to a file containing list-mode data written by
 *     Pixie16SaveExternalFIFODataToFile
 * @param[in] ModuleNumber: The module number that we'd like to analyze. Numbering starts at 0.
 * @param[in] ChannelNumber: The channel that we'd like to analyze. Numbering starts at 0.
 * @param[in] FileLocation: The memory address of the beginning of the trace in the file.
 * @param[in] RcdTraceLength: The length of the trace in the file. Maximum of 32768.
 * @param[in] RcdTrace: The recorded trace that we'll be analyzing. Maximum length of 32768.
 * @param[out] fastfilter: Pointer to an array of 32-bit unsigned integers with length matching
 *    `RcdTraceLength`. Maximum length of 32768.
 * @param[out] cfd: Pointer to an array of 32-bit unsigned integers with length matching
 *    `RcdTraceLength`. Maximum length of 32768.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Null pointer *RcdTrace
 * @retval -2 - Null pointer *fastfilter
 * @retval -3 - Null pointer *cfd
 * @retval -4 - Target module number is invalid
 * @retval -5 - Trace length is too short
 * @retval -6 - Failed to open list-mode data file
 * @retval -7 - Failed to seek to the trace in the file
 * @retval -8 - Failed to read the trace from the file
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ComputeFastFiltersOffline(
    const char* FileName, unsigned short ModuleNumber, unsigned short ChannelNumber,
    unsigned int FileLocation, unsigned short RcdTraceLength, unsigned short* RcdTrace,
    double* fastfilter, double* cfd);

/**
 * @ingroup APP
 * @brief Compute the slow-filter response of a trace listed in a list-mode data file.
 *
 * Use this function to compute slow-filter response of a single event in the list-mode data file.
 * The algorithm implemented in this offline analysis function is equivalent to
 * the one implemented in the Pixie-16 hardware. So this function can be used to analyze how the
 * leading edge fast trigger filter and the CFD filter implemented in the hardware respond to the
 * shape of recorded traces. By analyzing the response of these filters, it is possible to
 * optimize the performance of the leading-edge trigger or CFD trigger by adjusting the
 * fast-filter and CFD parameters offline. Such optimized parameters can then be saved to a
 * settings file to be used for online data acquisition.
 *
 * This function uses the globally defined configuration arrays to obtain the filter parameters.
 * These parameters need to be loaded from a configuration file using
 * Pixie16LoadDSPParametersFromFile. Users will also need to set the module's offline variant with
 * the function Pixie16SetOfflineVariant.
 *
 * @see Pixie16SaveExternalFIFODataToFile
 * @see Pixie16SetOfflineVariant
 * @see Pixie16LoadDSPParametersFromFile
 *
 * @param[in] FileName: Absolute path to a file containing list-mode data written by
 *     Pixie16SaveExternalFIFODataToFile
 * @param[in] ModuleNumber: The module number that we'd like to analyze. Numbering starts at 0.
 * @param[in] ChannelNumber: The channel that we'd like to analyze. Numbering starts at 0.
 * @param[in] FileLocation: The memory address of the beginning of the trace in the file.
 * @param[in] RcdTraceLength: The length of the trace in the file. Maximum of 32768.
 * @param[in] RcdTrace: The recorded trace that we'll be analyzing. Maximum length of 32768.
 * @param[out] slowfilter: Pointer to an array of 32-bit unsigned integers with length matching
 *    `RcdTraceLength`. Maximum length of 32768.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Null pointer *RcdTrace
 * @retval -2 - Null pointer *slowfilter
 * @retval -3 - Target module number is invalid
 * @retval -4 - Trace length is too short
 * @retval -5 - Failed to open list-mode data file
 * @retval -6 - Failed to seek to the trace in the file
 * @retval -7 - Failed to read the trace from the file
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ComputeSlowFiltersOffline(
    const char* FileName, unsigned short ModuleNumber, unsigned short ChannelNumber,
    unsigned int FileLocation, unsigned short RcdTraceLength, unsigned short* RcdTrace,
    double* slowfilter);

/**
 * @ingroup APP
 * @brief Set a module's offline variant to be used for offline analysis
 *
 * This function sets the offline variant for the provided module number. This function **must**
 * be called before Pixie16InitSystem to ensure that the correct variant is set.
 *
 * @param[in] ModuleNumber: The module number that we want to set
 * @param[in] variant: The variant of the module that we'd like to use for offline
 *     analysis.
 *     | Variant ID | Rev | SN | ADC Bits | ADC Sampling |
 *     |-|-|-|-|-|
 *     | 1 | 0xd | 275 | 12 | 100 |
 *     | 2 | 0xf | 1000 | 14 | 100 |
 *     | 3 | 0xf | 1000 | 12 | 250 |
 *     | 4 | 0xf | 1000 | 14 | 250 |
 *     | 5 | 0xf | 1000 | 12 | 500 |
 *     | 6 | 0xf | 1000 | 14 | 500 |
 *     | 7 | 0xf | 1000 | 16 | 250 |
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Module number is invalid
 * @retval -2 - module offline variant is invalid
 */
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SetOfflineVariant(unsigned short ModuleNumber,
                                                              unsigned short variant);

#ifdef __cplusplus
}
#endif  // End of notice for C++ compilers

#endif  // End of pixie16app_export.h
