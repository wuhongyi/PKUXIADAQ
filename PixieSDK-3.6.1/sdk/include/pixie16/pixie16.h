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

/** @file pixie16.h
 * @brief C wrappers for the C++ API that expose the same signature as the legacy code
 */

#ifndef PIXIE_H
#define PIXIE_H

#include <stddef.h>

#include <pixie/os_compat.hpp>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * We have some preprocessor constants defined here for use by customers in their
 * code. Since this glue is to support legacy C applications, we assume that
 * users will only be dealing with Rev F or older.
 */
/**
 * @brief Defines the Channel CSRA bit corresponding to the hardware's gain settings.
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 */
#define CCSRA_ENARELAY 14
/**
 * @brief Defines the Channel CSRA bit corresponding a good channel.
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 */
#define CCSRA_GOOD 2
/**
 * @brief Defines the Channel CSRA bit corresponding to the channel's polarity.
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 */
#define CCSRA_POLARITY 5
/**
 * @brief Defines the size of the list-mode data FIFO on-board the module in bytes.
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 */
#define EXTERNAL_FIFO_LENGTH 131072
/**
 * @brief Defines the code used to define a list mode run.
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 */
#define LIST_MODE_RUN 0x100
/**
 * @brief Defines the maximum length for an ADC trace obtained via
 *   ::Pixie16AcquireADCTrace and ::Pixie16ReadSglChanADCTrace
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 */
#define MAX_ADC_TRACE_LEN 8192
/**
 * @brief Defines the maximum histogram length obtained from
 *   ::Pixie16ReadHistogramFromModule
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 */
#define MAX_HISTOGRAM_LENGTH 32768
/**
 * @brief Defines the maximum number of baselines obtained by
 *   ::Pixie16AcquireBaselines and read out with ::Pixie16ReadSglChanBaselines
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 */
#define MAX_NUM_BASELINES 3640
/**
 * @brief Defines the value that will start a new data run when calling
 *   ::Pixie16StartHistogramRun or ::Pixie16StartListModeRun
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 */
#define NEW_RUN 1
/**
 * @brief This variable defines the maximum number of channels on a Pixie-16
 *   Rev A-F board.
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 */
#define NUMBER_OF_CHANNELS 16
/**
 * @brief Defines the value that will resume a data run when calling
*   ::Pixie16StartHistogramRun or ::Pixie16StartListModeRun
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 */
#define RESUME_RUN 0
/**
 * @brief Defines the maximum number of modules that the API can handle.
 * @note This is an artificial limit imposed to maintain compatibility with the
 *    Legacy API.
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 */
#define SYS_MAX_NUM_MODULES 32

/**
 * @ingroup PIXIE16_API
 * @brief Defines a data structure used to configure the List-mode data FIFO worker
 *
 * Xia recommends asynchronous mode and the XIA list mode decoder for optimal system performance.
 */
struct fifo_worker_config {
    /**
     * @brief Specifies the bandwidth that the worker can use.
     *
     * The maximum bandwidth available in a PCI bus is 100 MB / s. When using the default bandwidth
     * value (0) the individual modules will use the maximum data rate available. This rate can
     * vary depending on the type of crate (PCI vs PCIe) and the trigger rates in the module.
     *
     * By default, the system will balance the data rate approximately equally amongst all the
     * modules in the system. One could use this value to adjust the bandwidth for a low-count
     * rate module could be reduced to give preference to a module that's got a higher rate.
     *
     * @note A value of 0 indicates no limit on the bandwidth.
     * | Units | Min | Max | Default |
     * |---|---|---|---|
     * | MB/s | 1 | 100 | 0 |
     */
    size_t bandwidth_mb_per_sec;
    /**
     * @brief Defines how many buffers a worker can have.
     *
     * A buffer contains the data read from the on-board list-mode data FIFO. They are a fixed
     * size of 64 MB. The worker will periodically attempt to compact the buffer pool to ensure that
     * there is always a buffer available to read data from the hardware.
     *
     * | Units | Min | Max | Default |
     * |---|---|---|---|
     * | None | 10 | 10000000 | 100 |
     */
    size_t buffers;
    /**
     * @brief The threshold that the on-board FIFO must reach before we execute a read.
     *
     * This value works in tandem with ::hold_usecs to ensure that we're reading data out of the
     * module before it can overflow. Adjusting this value incorrectly may result
     * in a loss of data.
     *
     * | Units | Min | Max | Default |
     * |---|---|---|---|
     * | bytes | 512 | 8192 | 1024 |
     */
    size_t dma_trigger_level_bytes;
    /**
     * @brief The amount of time that data waits in the on-board fifo before being read out.
     *
     * The hold time is reset on every transfer from the FIFO no matter the run task state.
     * If we are idle and reading data the hold time is reset so the waiting period only starts to
     * decay once we do not see data.
     *
     * This value works in tandem with ::dma_trigger_level_bytes to ensure that we're reading
     * data out of the module before it can overflow. Adjusting this value incorrectly may result
     * in a loss of data.
     *
     * | Units | Min | Max | Default |
     * |---|---|---|---|
     * | microseconds | 1000 | 100000 | 10000 |
     */
    size_t hold_usecs;
    /**
     * @brief The amount of time the worker will sit idle before checking the hardware for data.
     * @note This value is ignored during list-mode data runs, see ::run_wait_usecs.
     *
     * | Units | Min | Max | Default |
     * |---|---|---|---|
     * | microseconds | 10000 | 1000000 | 150000 |
     */
    size_t idle_wait_usecs;
    /**
     * @brief Specifies the amount of time that worker will wait after successfully reading data.
     *
     * | Units | Min | Max | Default |
     * |---|---|---|---|
     * | microseconds | 500 | 200000 | 5000 |
     *
     * A value of 0 enables synchronous FIFO reading. When set to 0 a request for the FIFO status
     * will read any data in the external FIFO memory into the buffers ready for reading. Warning,
     * this mode is not recommended and may be removed in the future. Enabling this mode means the
     * rate your application's reads a module's FIFO memory determines effective throughput
     * of the system and that may not reflect XIA's performance figures. If your application is not
     * able to meeting the required timing list mode data may be lost as the external FIFO memory
     * may overflow. The transfer process from the FIPPI through the DSP. the external FIFO
     * memory to the PC's memory is asychronous by the nature of the hardware design and any apparent
     * data alignment is purely coincidental. XIA recommends list mode data be treated as a stream
     * and the XIA decoder API be used to ensure smooth and fast decoding of the read buffers.
     */
    size_t run_wait_usecs;
};

/**
 * @ingroup PIXIE16_API
 * @brief Defines a data structure used to provide users information about the module.
 */
#define PIXIE16_API_MOD_CONFIG_MAX_STRING (256)
struct module_config {
    unsigned short adc_bit_resolution;
    unsigned short adc_sampling_frequency;
    char sys_fpga[PIXIE16_API_MOD_CONFIG_MAX_STRING];
    char dsp_code[PIXIE16_API_MOD_CONFIG_MAX_STRING];
    char dsp_var[PIXIE16_API_MOD_CONFIG_MAX_STRING];
    unsigned short number;
    unsigned short number_of_channels;
    unsigned short revision;
    unsigned int serial_number;
    unsigned short slot;
    char sp_fpga[PIXIE16_API_MOD_CONFIG_MAX_STRING];
};

/**
 * @ingroup PIXIE16_API
 * @brief Defines a data structure used to provide users FIFO statistics for a module.
 */
struct module_fifo_stats {
    size_t in; /** Data into the data FIFO queue */
    size_t out; /** Data out of the data FIFO queue */
    size_t dma_in; /** DMA data read from the module into the data FIFO queue */
    size_t overflows; /** FIFO queue overflows */
    size_t dropped; /** FIFO queue data dropped */
    size_t hw_overflows; /** Estimate of HW FIFO overflows */
};

/**
 * @defgroup PIXIE_SDK PixieSDK
 * Documentation group for the PixieSDK functions/classes/macros.
 *
 * @defgroup PIXIE16_API Pixie16 API
 * @ingroup PIXIE_SDK
 * C wrappers to the PixieSDK that provide backward compatibility with the Legacy C implementation.
 */

/**
 * @ingroup PIXIE16_API
 * @brief Test a bit in a 16-bit unsigned integer.
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 * @param[in] bit The bit position to test, which starts counting at 0.
 * @param[in] value A 16-bit number that we'll test to see if the requested bit is true.
 * @return 1 if the bit was set, 0 otherwise.
 */
PIXIE_EXPORT unsigned short PIXIE_API APP16_TstBit(unsigned short bit, unsigned short value);

/**
 * @ingroup PIXIE16_API
 * @brief Set a bit in a 16-bit unsigned integer.
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 * @param[in] bit The bit position to set, which starts counting at 0.
 * @param[in] value A 16-bit number that will have its bit set to true.
 * @return The new integer after the bit has been toggled if the bit was between 0 - 15, the input
 *         value otherwise.
 */
PIXIE_EXPORT unsigned short PIXIE_API APP16_SetBit(unsigned short bit, unsigned short value);

/**
 * @ingroup PIXIE16_API
 * @brief Clear a bit in a 16-bit unsigned integer.
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 * @param[in] bit The bit position to clear, which starts counting at 0.
 * @param[in] value A 16-bit number that will have its bit set to false.
 * @return The new integer after the bit has been toggled if the bit was between 0 - 15, the input
 *         value otherwise.
 */
PIXIE_EXPORT unsigned short PIXIE_API APP16_ClrBit(unsigned short bit, unsigned short value);

/**
 * @ingroup PIXIE16_API
 * @brief Set a bit in a 32-bit unsigned integer.
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 * @param[in] bit The bit position to set, which starts counting at 0.
 * @param[in] value A 32-bit number that will have its bit set to true.
 * @return The new integer after the bit has been toggled if the bit was between 0 - 31, the input
 *         value otherwise.
 */
PIXIE_EXPORT unsigned int PIXIE_API APP32_SetBit(unsigned short bit, unsigned int value);

/**
 * @ingroup PIXIE16_API
 * @brief Clear a bit in a 32-bit unsigned integer.
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 * @param[in] bit The bit position to clear, which starts counting at 0.
 * @param[in] value A 32-bit number that will have its bit set to false.
 * @return The new integer after the bit has been toggled if the bit was between 0 - 31, the input
 *         value otherwise.
 */
PIXIE_EXPORT unsigned int PIXIE_API APP32_ClrBit(unsigned short bit, unsigned int value);

/**
 * @ingroup PIXIE16_API
 * @brief Test a bit in a 32-bit unsigned integer.
 * @warning This variable will be deprecated July 31, 2023 with the Legacy API.
 * @param[in] bit The bit position to test, which starts counting at 0.
 * @param[in] value A 32-bit number that we'll test to see if the requested bit is true.
 * @return 1 if the bit was set, 0 otherwise.
 */
PIXIE_EXPORT unsigned int PIXIE_API APP32_TstBit(unsigned short bit, unsigned int value);

/**
 * @ingroup PIXIE16_API
 * @brief Converts an IEEE 754 standard format number to a double.
 * @see https://en.wikipedia.org/wiki/IEEE_754
 * @param[in] IEEEFloatingNumber The IEEE 754 standard format number to convert
 * @returns The input value in double format.
 */
PIXIE_EXPORT double PIXIE_API IEEEFloating2Decimal(unsigned int IEEEFloatingNumber);

/**
 * @ingroup PIXIE16_API
 * @brief Converts a decimal number to the IEEE 754 standard
 * @see https://en.wikipedia.org/wiki/IEEE_754
 * @param[in] DecimalNumber The decimal number to convert
 * @returns The input value in IEEE 754 standard format.
 */
PIXIE_EXPORT unsigned int PIXIE_API Decimal2IEEEFloating(double DecimalNumber);

/**
 * @ingroup PIXIE16_API
 * @brief Gets the statistics buffer size in words (32-bit unsigned integers).
 * @see Pixie16ReadStatisticsFromModule
 * @returns The number of words needed to hold all the statistics data.
 */
PIXIE_EXPORT unsigned int PIXIE_API Pixie16GetStatisticsSize(void);

/**
 * @ingroup PIXIE16_API
 * @brief Gets the error string associated with the provided error code.
 *
 * Since exceptions are purely a C++ construct, we wrap all the SDK calls with
 * exception handling. These wrappers catch the exception, transform its integer
 * value to negative, and then return that as the function's exit code. This
 * keeps us in line with previous functionality.
 *
 * This function allows users to obtain the return code's text by providing the
 * return code and a buffer to hold the resulting message.
 *
 * This function will return an error status if the provided buffer isn't large
 * enough to hold the resulting text.
 *
 * @param[in] return_code The return code that we'd like the error text for.
 * @param[out] buf The char buffer that will hold the resultant text. It's
 *     recommended that you provide a buffer of at least 1024. In practice, the
 *     message are much shorter than this, but that may change in the future.
 * @param[in] buf_size The size of the data buffer to hold the return code text.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API PixieGetReturnCodeText(int return_code, char* buf,
                                                  unsigned int buf_size);

/**
 * @ingroup PIXIE16_API
 * @brief Gets the maximum histogram buffer size in words (32-bit unsigned integers).
 *
 * The histogram buffer size may vary based on the hardware's channel configuration
 * This function allows users to determine the specific size of the
 * histogram data block for the specific module/channel combination. This size
 * can then be used to allocate vectors of the correct size for the histogram data.
 *
 * @param[in] mod_num The module that holds the channel we want the size for.
 * @param[in] chan_num The channel that we want the histogram size for.
 * @param[out] hist_length The length of the histogram associated with the given module/channel.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API PixieGetHistogramLength(unsigned short mod_num, unsigned short chan_num,
                                                   unsigned int* hist_length);

/**
 * @ingroup PIXIE16_API
 * @brief Gets the maximum trace buffer size in words (32-bit unsigned integers).
 *
 * The trace buffer size may vary based on the hardware's channel configuration
 * This function allows users to determine the specific size of the
 * histogram data block for the specific module/channel combination. This size
 * can then be used to allocate vectors of the correct size for the histogram data.
 *
 * @param[in] mod_num The module that holds the channel we want the size for.
 * @param[in] chan_num The channel that we want the histogram size for.
 * @param[out] trace_length The length of the trace associated with the given module/channel.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API PixieGetTraceLength(unsigned short mod_num, unsigned short chan_num,
                                               unsigned int* trace_length);

/**
 * @ingroup PIXIE16_API
 * @brief Gets the maximum number of baseline samples for a channel.
 *
 * The get_baselines control task always collects the maximum number of baselines for each
 * channel. Users can then request fewer baselines than that, but upto the maximum provided
 * by this function.
 *
 * @see Pixie16AcquireBaselines
 * @see Pixie16ReadSglChanBaselines
 *
 * @param[in] mod_num The module that holds the channel we want the size for.
 * @param[in] chan_num The channel that we want the histogram size for.
 * @param[out] max_num_baselines The maximum number of baselines that we will collect
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API PixieGetMaxNumBaselines(unsigned short mod_num, unsigned short chan_num,
                                                   unsigned int* max_num_baselines);

/**
 * @ingroup PIXIE16_API
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
 * ::Pixie16ReadSglChanADCTrace to read the ADC trace data out to the host computer,
 * channel by channel.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16AcquireADCTrace
 *
 * @see Pixie16ReadSglChanADCTrace
 *
 * @param[in] ModNum The module number that we'll be working with
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16AcquireADCTrace(unsigned short ModNum);

/**
 * @ingroup PIXIE16_API
 * @brief Acquire baselines
 *
 * Use this function to acquire baselines from Pixie-16 modules. Specify the module using ModNum
 * which starts counting at 0. If ModNum is set to be less than the total number of modules in the
 * system, only the module specified by ModNum will have its baselines acquired. But if ModNum is
 * set to be equal to the total number of modules in the system, then all modules in the system
 * will have their baselines acquired.
 *
 * After the successful return of this function, the DSP’s internal memory will be filled with
 * baselines data. Users should then call ::Pixie16ReadSglChanBaselines to read the
 * baselines data out to the host computer, channel by channel.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16AcquireBaselines
 *
 * @see Pixie16ReadSglChanBaselines
 *
 * @param[in] ModNum The module number to get baselines from
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16AcquireBaselines(unsigned short ModNum);

/**
 * @ingroup PIXIE16_API
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
 * 12-bit ADC scale ([0,4095]).
 *
 * The main purpose of this function is to ensure the input signals fall within the voltage range
 * of the ADCs so that all input signals can be digitized by the ADCs properly.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16AdjustOffsets
 *
 * @param[in] ModNum The module number to adjust baselines for
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16AdjustOffsets(unsigned short ModNum);

/**
 * @ingroup PIXIE16_API
 * @brief Find the BLcut value for the selected channel and return it using pointer *BLcut.
 *
 * Use this function to find the Baseline Cut value for one channel of a Pixie-16 module. The
 * baseline cut value is then downloaded to the DSP, where baselines are captured and averaged
 * over time. The cut value would prevent a bad baseline value from being used in the averaging
 * process, i.e., if a baseline value is outside the baseline cut range, it will not be used for
 * computing the baseline average. Averaging baselines over time improves energy resolution
 * measurement.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16BLcutFinder
 *
 * @param[in] ModNum The module number to work with, starts counting at 0.
 * @param[in] ChanNum The channel number to work with, starts counting at 0.
 * @param[out] BLcut The Baseline cut as determined by the calculation.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16BLcutFinder(unsigned short ModNum, unsigned short ChanNum,
                                              unsigned int* BLcut);

/**
 * @ingroup PIXIE16_API
 *
 * @brief Boot one or more modules in the system.
 *
 * Use this function to boot Pixie modules so that they can be set up for data taking. The
 * function downloads to the Pixie modules the communication (or system) FPGA configurations,
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
 * | 0 | Boot communication FPGA  | All Pixie Revisions  |
 * | 1 | Boot trigger FPGA  | Revision A - Ignored  |
 * | 2 | Boot signal processing FPGA  | All Pixie Revisions  |
 * | 3 | Boot digital signal processor (DSP)  | All Pixie Revisions  |
 * | 4 | Download DSP I/O parameters  | All Pixie Revisions  |
 * | 5 | Program on-board FPGAs  | All Pixie Revisions  |
 * | 6 | Set on-board DACs  | All Pixie Revisions  |
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16BootModule
 *
 * @param[in] ComFPGAConfigFile name of communications FPGA configuration file
 * @param[in] SPFPGAConfigFile name of signal processing FPGA configuration file
 * @param[in] TrigFPGAConfigFile name of trigger FPGA configuration file (!!! IGNORED)
 * @param[in] DSPCodeFile name of executable code file for digital signal processor (DSP)
 * @param[in] DSPParFile name of DSP parameter file
 * @param[in] DSPVarFile name of DSP variable names file
 * @param[in] ModNum The module number. If set to Number_Modules we'll boot all modules.
 * @param[in] BootPattern boot pattern bit mask
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16BootModule(const char* ComFPGAConfigFile,
                                             const char* SPFPGAConfigFile,
                                             const char* TrigFPGAConfigFile,
                                             const char* DSPCodeFile, const char* DSPParFile,
                                             const char* DSPVarFile, unsigned short ModNum,
                                             unsigned short BootPattern);

/**
 * @ingroup PIXIE16_API
 * @brief Read the number of 32-bit words that the external FIFO currently has.
 *
 * Use this function to check the status of the external FIFO of a Pixie-16 module while a
 * list-mode data acquisition run is in progress. The function returns the number of words
 * (32-bit) that the external FIFO currently has. If the number of words is greater than a
 * user-set threshold, ::Pixie16ReadDataFromExternalFIFO can then be used to read the data
 * from the external FIFO. The threshold can be set by the user to either minimize reading
 * overhead or to read data out of the FIFO as quickly as possible. The Pixie-16 API
 * (pixie16app_defs.h) has defined a threshold with value of 1024 for external FIFO read out
 * (#EXTFIFO_READ_THRESH).
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16CheckExternalFIFOStatus
 *
 * @see Pixie16ReadDataFromExternalFIFO
 *
 * @param[out] nFIFOWords The number of 32-bit unsigned integer words contained in the module's
 *     External FIFO
 * @param[in] ModNum The module number to read from. Numbering starts counting at 0.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16CheckExternalFIFOStatus(unsigned int* nFIFOWords,
                                                          unsigned short ModNum);

/**
 * @ingroup PIXIE16_API
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
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16CheckRunStatus
 *
 * @param[in] ModNum The module number to interrogate and counting starts at 0.
 * @returns The result of the check or an error code.
 * @retval  0 - No run is in progress
 * @retval  1 - Run is still in progress
 * @retval The value of the xia::pixie::error::code if there was an error.
 */
PIXIE_EXPORT int PIXIE_API Pixie16CheckRunStatus(unsigned short ModNum);

/**
 * @ingroup PIXIE16_API
 * @brief Compute input count rate of a channel
 *
 * Use this function to calculate the input count rate on one channel of a Pixie-16 module. This
 * function does not communicate with Pixie-16 modules. Before calling this function,
 * ::Pixie16ReadStatisticsFromModule should be called to read statistics data from the
 * module.
 *
 * The *Statistics array is filled with data from a Pixie-16 module after calling function
 * ::Pixie16ReadStatisticsFromModule.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16ComputeInputCountRate
 *
 * @note The units on the return value are triggers per second.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics A pointer to the statistics array returned by ::Pixie16ReadStatisticsFromModule
 * @param[in] ModNum ModNum is the module number which starts counting at 0.
 * @param[in] ChanNum ChanNum is the channel number which starts counting at 0.
 * @return 0 if the live time was 0. The number of triggers divided by the live time in seconds
 *     otherwise.
 */
PIXIE_EXPORT double PIXIE_API Pixie16ComputeInputCountRate(unsigned int* Statistics,
                                                           unsigned short ModNum,
                                                           unsigned short ChanNum);

/**
 * @ingroup PIXIE16_API
 * @brief Compute live time that a channel accumulated in a run
 *
 * Use this function to calculate the channel's live time as measured by the FPGA. This time
 * excludes time during which the input signal was out of the ADC's voltage range or when the data
 * run was stopped. This value should always be equal to, or less than, the Real time obtained
 * from ::Pixie16ComputeRealTime.
 *
 * This function does not communicate with Pixie-16 modules. The Statistics array is filled with
 * data from a Pixie-16 module after calling function, ::Pixie16ReadStatisticsFromModule.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16ComputeLiveTime
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics A pointer to the statistics array returned by ::Pixie16ReadStatisticsFromModule
 * @param[in] ModNum ModNum is the module number, which starts counting at 0.
 * @param[in] ChanNum ChanNum is the channel number, which starts counting at 0.
 * @return The live time of the module in seconds.
 */
PIXIE_EXPORT double PIXIE_API Pixie16ComputeLiveTime(unsigned int* Statistics,
                                                     unsigned short ModNum, unsigned short ChanNum);

/**
 * @ingroup PIXIE16_API
 * @brief Compute output count rate of a channel
 *
 * Use this function to calculate the output count rate on one channel of a Pixie-16 module. This
 * function does not communicate with Pixie-16 modules. Before calling this function
 * ::Pixie16ReadStatisticsFromModule should be called to read statistics data from the
 * module.
 *
 * The Statistics array is filled with data from a Pixie-16 module after calling function
 * ::Pixie16ReadStatisticsFromModule.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16ComputeOutputCountRate
 *
 * @note The return value's units are channel events per second.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics A pointer to the statistics array returned by ::Pixie16ReadStatisticsFromModule
 * @param[in] ModNum ModNum is the module number which starts counting at 0.
 * @param[in] ChanNum ChanNum is the channel number which starts counting at 0.
 * @return 0 if the live time was 0. Otherwise, the number of channel events divided by the
 *     live time in seconds.
 */
PIXIE_EXPORT double PIXIE_API Pixie16ComputeOutputCountRate(unsigned int* Statistics,
                                                            unsigned short ModNum,
                                                            unsigned short ChanNum);

/**
 * @ingroup PIXIE16_API
 * @brief Compute number of events processed by a module
 *
 * @note This function is only used by Rev-A modules.
 *
 * @warning This function is not currently implemented.
 *
 * Use this function to calculate the number of events that have been processed by a Pixie-16
 * module during a data acquisition run. This function does not communicate with Pixie-16 modules.
 * Before calling this function, ::Pixie16ReadStatisticsFromModule should be
 * called to read statistics data from the module first.
 *
 * The Statistics array is filled with data from a Pixie-16 module after calling function
 * ::Pixie16ReadStatisticsFromModule.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16ComputeProcessedEvents
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics A pointer to the statistics array returned by ::Pixie16ReadStatisticsFromModule
 * @param[in] ModNum ModNum is the module number, which starts counting at 0.
 * @return The number of events processed by the module.
 */
PIXIE_EXPORT double PIXIE_API Pixie16ComputeProcessedEvents(unsigned int* Statistics,
                                                            unsigned short ModNum);

/**
 * @ingroup PIXIE16_API
 * @brief Compute the number of triggers seen by the FPGA's trigger filter.
 *
 * This function does not communicate with Pixie-16 modules.
 * Before calling this function ::Pixie16ReadStatisticsFromModule should be
 * called to read statistics data from the module.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16ComputeRawInputCount
 *
 * @param[in] Statistics A pointer to the statistics array returned by ::Pixie16ReadStatisticsFromModule
 * @param[in] ModNum ModNum is the module number which starts counting at 0.
 * @param[in] ChanNum ChanNum is the channel number which starts counting at 0.
 * @return The raw number fast triggers that the channel observed.
 */
PIXIE_EXPORT double PIXIE_API Pixie16ComputeRawInputCount(unsigned int* Statistics,
                                                          unsigned short ModNum,
                                                          unsigned short ChanNum);

/**
 * @ingroup PIXIE16_API
 * @brief Compute the number of channel events handled by the DSP.
 *
 * This differs from ::Pixie16ComputeRawInputCount in that these events may have
 * additional validation applied. This number should always be equal to or
 * less than the raw input counts.
 *
 * This function does not communicate with Pixie-16 modules.
 * Before calling this function ::Pixie16ReadStatisticsFromModule should be
 * called to read statistics data from the module.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16ComputeRawOutputCount
 *
 * @param[in] Statistics A pointer to the statistics array returned by ::Pixie16ReadStatisticsFromModule
 * @param[in] ModNum ModNum is the module number which starts counting at 0.
 * @param[in] ChanNum ChanNum is the channel number which starts counting at 0.
 * @return The raw number of events handled by the DSP.
 */
PIXIE_EXPORT double PIXIE_API Pixie16ComputeRawOutputCount(unsigned int* Statistics,
                                                           unsigned short ModNum,
                                                           unsigned short ChanNum);

/**
 * @ingroup PIXIE16_API
 * @brief Compute real time that a module accumulated in a run
 *
 * Use this function to calculate the real time that a Pixie-16 module has spent on data
 * acquisition.This function does not communicate with Pixie-16 modules.
 * Before calling this function, ::Pixie16ReadStatisticsFromModule should be
 * called to read statistics data from the module first.
 *
 * The Statistics array is filled with data from a Pixie-16 module after calling function
 * ::Pixie16ReadStatisticsFromModule.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16ComputeRealTime
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics A pointer to the statistics array returned by ::Pixie16ReadStatisticsFromModule
 * @param[in] ModNum The module number, which starts counting at 0.
 * @return The number of seconds that the module spent on data acquisition.
 */
PIXIE_EXPORT double PIXIE_API Pixie16ComputeRealTime(unsigned int* Statistics,
                                                     unsigned short ModNum);

/**
 * @ingroup PIXIE16_API
 * @brief Copy DSP parameters from one module to other modules.
 * @note This function is **not** implemented at this time.
 *
 * Use this function to copy DSP parameters from one module to the others that are installed in
 * the system.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16CopyDSPParameters
 *
 * @param[in] BitMask A bit pattern that designates what items should be copied from the source
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
 * @param[in] SourceModule The module that we'll read the settings from, starts counting at 0.
 * @param[in] SourceChannel The channel we'll read settings from, starts counting at 0.
 * @param[in] DestinationMask A pointer to an array that indicates the channel and module whose
 *     settings will be copied from the source channel and module. For instance, if there are 5
 *     modules (total 80 channels) in the system, DestinationMask would be defined as
 *     `DestinationMask[80]`, where `DestinationMask[0]` to `DestinationMask[15]` would select
 *     channel 0 to 15 of module 0, `DestinationMask[16]` to `DestinationMask[31]` would select
 *     channel 0 to 15 of module 1, and so on. If a given channel i is to be copied,
 *     then `DestinationMask[i]` should be set to 1, otherwise, it should be set to 0.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16CopyDSPParameters(unsigned short BitMask,
                                                    unsigned short SourceModule,
                                                    unsigned short SourceChannel,
                                                    unsigned short* DestinationMask);

/**
 * @ingroup PIXIE16_API
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
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16EndRun
 *
 * @param[in] ModNum The module number to interrogate and counting starts at 0.
 */
PIXIE_EXPORT int PIXIE_API Pixie16EndRun(unsigned short ModNum);

/**
 * @ingroup PIXIE16_API
 *
 * @brief Release resources used by the modules before exiting the application.
 *
 * This function to cleanly shut down a module or modules when it(they) reach
 * the end of their lifecycle. Closing the module
 *   * releases the PCI virtual address,
 *   * stops any in-progress runs,
 *   * removes the list-mode data workers.
 *
 * @note This function should be called before a user’s application exits.
 *
 * ### Example
 * #### Exit a single module
 * \snippet snippets/api_function_examples.c Pixie16ExitSystem - single module
 * #### Exit all modules
 * \snippet snippets/api_function_examples.c Pixie16ExitSystem - all modules
 *
 * @param[in] ModNum The module number to close, which starts counting at 0. If
 *   ModNum is equal to the number of modules in the system then all modules will
 *   exit.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16ExitSystem(unsigned short ModNum);

/**
 * @ingroup PIXIE16_API
 *
 * @brief Initializes the system by mapping PXI slots to modules
 *
 * Use this function to configure the Pixie modules in the PXI chassis.
 *
 * NumModules is the total number of Pixie-16 modules installed in the system. PXISlotMap is the
 * pointer to an array that must have at least as many entries as there are Pixie-16 modules in the
 * chassis.
 *
 * PXISlotMap serves as a simple mapping of the logical module number and the physical slot number
 * that the modules reside in. The logical module number runs from 0. For instance, in a system
 * with 5 Pixie modules, these 5 modules may occupy slots 3 through 7. The user must fill
 * PXISlotMap as follows: PXISlotMap = {3, 4, 5, 6, 7 ...} since module number 0 resides in slot
 * number 3, etc. To find out in which slot a module is located, any piece of subsequent code can
 * use the expression PXISlotMap[ModNum], where ModNum is the logic module number.
 *
 * OfflineMode is used to indicate to the API whether the system is running in OFFLINE mode (1) or
 * ONLINE mode (0). OFFLINE mode is useful for situations where no Pixie-16 modules are present but
 * users can still test their calls to the API functions in their application software. Offline
 * mode is not supported in the SDK.
 *
 * This function must be called as the first step in the boot process. It makes the modules known
 * to the system and "opens" each module for communication.
 *
 * Modules present in the crate but not listed in the slot map will left available for use by
 * another process. Note, this call will hold a module not in the map until the slot assignment
 * process closes the module.
 *
 * The log file "Pixie16Msg.log" will be opened for logging at the beginning of this function. Prior
 * to calling this function. The environment variable PIXIE16_LOG_LEVEL can be set to one of the
 * following logging levels. If PIXIE16_LOG_LEVEL is not set, default logging level is INFO.
 *
 * | PIXIE16_LOG_LEVEL | Description |
 * |---|---|
 * | DEBUG   | All messages, including information only relevant to the developers at XIA |
 * | INFO    | All meesages except for debug messages |
 * | WARNING | All warning and error messages |
 * | ERROR   | Only messages that cause a routine to end its execution early |
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16InitSystem
 *
 * @param[in] NumModules The total number of Pixie modules in the system provided by the user. If
 *     0 all slots are check for modules automatically.
 * @param[in] PXISlotMap An array containing the PXI slot number for each module. The array index
 *     indicates which module number the slot number takes. The first slot number is Module 0.
 * @param[in] OfflineMode Used to tell the API that there are no modules connected to the system.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16InitSystem(unsigned short NumModules, unsigned short* PXISlotMap,
                                             unsigned short OfflineMode);

/**
 * @ingroup PIXIE16_API
 * @brief Read data from the external FIFO.
 *
 * This function reads list mode data from the external FIFO of a Pixie-16 module. The data are
 * 32-bit unsigned integers. Normally, ::Pixie16CheckExternalFIFOStatus is called first to
 * see how many words the external FIFO currently has, and then this function is called to read
 * the data from the FIFO.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16ReadDataFromExternalFIFO
 *
 * @see Pixie16CheckExternalFIFOStatus
 *
 * @param[out] ExtFIFO_Data A pointer to a memory block containing enough space for `nFIFOWords`
 *    worth of 32-bit unsigned integers.
 * @param[in] nFIFOWords The number of words that we're going to be reading.
 * @param[in] ModNum The module number that we'll read from. Numbering starts at 0.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16ReadDataFromExternalFIFO(unsigned int* ExtFIFO_Data,
                                                           unsigned int nFIFOWords,
                                                           unsigned short ModNum);

/**
 * @ingroup PIXIE16_API
 * @brief Load DSP parameters from a settings file
 *
 * Use this function to read DSP parameters from a settings file and then download the settings to
 * Pixie-16 modules that are installed in the system. This function supports both legacy settings
 * files and JSON settings files.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16LoadDSPParametersFromFile
 *
 * @param[in] FileName Absolute path to where we will write the DSP parameter file.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16LoadDSPParametersFromFile(const char* FileName);

/**
 * @ingroup PIXIE16_API
 * @brief Retrieve a channel's histogram data from a Pixie module.
 *
 * Use this function to read out the histogram data from a Pixie-16 module’s histogram memory.
 * Before calling this function, the host code should allocate appropriate amount of memory to
 * store the histogram data. The maximum histogram length is provided by #MAX_HISTOGRAM_LENGTH.
 * Histogram data are 32-bit unsigned integers.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16ReadHistogramFromModule
 *
 * @param[out] Histogram The histogram data that we read from the module.
 * @param[in] NumWords The number of words we'll read out of the histogram memory.
 * @param[in] ModNum The module number that we want the histogram from
 * @param[in] ChanNum The channel number that we'd like to read from
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16ReadHistogramFromModule(unsigned int* Histogram,
                                                          unsigned int NumWords,
                                                          unsigned short ModNum,
                                                          unsigned short ChanNum);

/**
 * @ingroup PIXIE16_API
 * @brief Read information stored on each module, including its revision, serial number, and ADC.
 *
 * Use this function to read information stored on each module, including its revision, serial
 * number, ADC bits and sampling rate. This should be done after initializing the PCI communication.
 * Information from the module can be used to select the appropriate firmware, DSP, and
 * configuration parameters files before booting the module.
 *
 * @warning This function cannot be used to retrieve the module's channel count. @see PixieGetModuleInfo
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16ReadModuleInfo
 *
 * @param[in] ModNum The module number (counts from 0) that we'll read information
 * @param[out] ModRev The revision read from the on-board EEPROM
 * @param[out] ModSerNum The serial number read from the on-board EEPROM
 * @param[out] ModADCBits The ADC bit resolution read from the on-board EEPROM
 * @param[out] ModADCMSPS The ADC sampling frequency read from the on-board EEPROM
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16ReadModuleInfo(unsigned short ModNum, unsigned short* ModRev,
                                                 unsigned int* ModSerNum,
                                                 unsigned short* ModADCBits,
                                                 unsigned short* ModADCMSPS);

/**
 * @ingroup PIXIE16_API
 * @brief Read information about each module.
 *
 * Use this function to read configuration information about the requested module.
 * This should be done after initializing the PCI communication. If this function is called before
 * booting the module, then the firmware information will be unpopulated.
 *
 * Information from the module can be used to select the appropriate firmware, DSP, and
 * configuration parameters files before booting the module.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c PixieGetModuleInfo
 *
 * @param[in] mod_num The module number (counts from 0) that we'll read information
 * @param[out] cfg A pointer to the module_config object that will store the retrieved data.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API PixieGetModuleInfo(unsigned short mod_num,
                                              struct module_config* cfg);

/**
 * @ingroup PIXIE16_API
 * @brief Acquire ADC traces from a Pixie module.
 *
 * Use this function to read ADC trace data from a Pixie-16 module. Before calling this function,
 * ::Pixie16AcquireADCTrace should be called to fill the DSP internal memory first.
 * Also, the host code should allocate appropriate amount of memory to store the trace data. The
 * ADC trace data length for each channel is #MAX_ADC_TRACE_LEN. Since the trace data are
 * 16-bit unsigned integers (for hardware variants with less than 16-bit ADCs only the lower
 * 12-bit or 14-bit contain real data), two consecutive 16-bit words are packed into one 32-bit
 * word in the DSP internal memory. So for each channel, 4096 32-bit words are read out first
 * from the DSP, and then each 32-bit word is unpacked to form two 16-bit words.
 *
 * Specify the module using ModNum and the channel on the module using ChanNum. Note that both the
 * modules and channels are counted starting at 0.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16ReadSglChanADCTrace
 *
 * @see Pixie16AcquireADCTrace
 *
 * @param[out] Trace_Buffer Pointer to the data buffer that will hold the data.
 * @param[in] Trace_Length Length of the traces for the module.
 * @param[in] ModNum The module that we want to read
 * @param[in] ChanNum The channel that we want to read
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16ReadSglChanADCTrace(unsigned short* Trace_Buffer,
                                                      unsigned int Trace_Length,
                                                      unsigned short ModNum,
                                                      unsigned short ChanNum);

/**
 * @ingroup PIXIE16_API
 * @brief Read baselines from a channel in a module
 *
 * Use this function to read baseline data from a Pixie-16 module. Before calling this function,
 * ::Pixie16AcquireBaselines should be called to fill the DSP internal memory first.
 * Users can use the ::PixieGetMaxNumBaselines function to ensure that they allocate enough
 * memory to hold the baseline data.
 * In the DSP internal memory, each baseline is a 32-bit IEEE floating point number. After being
 * read out to the host, this function will convert each baseline data to a decimal number. In
 * addition to baseline values, time stamps corresponding to each baseline are also returned after
 * this function call.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16ReadSglChanBaselines
 *
 * @see Pixie16AcquireBaselines
 *
 * @param[out] Baselines An array of doubles to hold the baseline values
 * @param[in] TimeStamps The timestamp that the baselines were obtained at
 * @param[in] NumBases The number of baselines values read
 * @param[in] ModNum The module number to read the baselines from, counting from 0.
 * @param[in] ChanNum The channel number to read the baselines from, counting from 0.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16ReadSglChanBaselines(double* Baselines, double* TimeStamps,
                                                       unsigned short NumBases,
                                                       unsigned short ModNum,
                                                       unsigned short ChanNum);

/**
 * @ingroup PIXIE16_API
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
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16ReadSglChanPar
 *
 * @param[in] ChanParName The name of the channel parameter that we'll read
 * @param[out] ChanParData Contains the value we read from the channel.
 * @param[in] ModNum The module number we'll read from. Counting starts at 0.
 * @param[in] ChanNum The channel number we'll read from. Counting starts at 0.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16ReadSglChanPar(const char* ChanParName, double* ChanParData,
                                                 unsigned short ModNum, unsigned short ChanNum);

/**
 * @ingroup PIXIE16_API
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
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16ReadSglModPar
 *
 * @param[in] ModParName The name of the module parameter that we'll read
 * @param[out] ModParData Contains the value of the requested parameters.
 * @param[in] ModNum The module number we'll read from. Numbering starts counting at 0.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16ReadSglModPar(const char* ModParName, unsigned int* ModParData,
                                                unsigned short ModNum);

/**
 * @ingroup PIXIE16_API
 * @brief Retrieve statistics data from a module.
 *
 * Use this function to read out statistics data from a Pixie-16 module. Before calling this
 * function, the host code should allocate appropriate amount of memory to store the statistics
 * data. The number of statistics data for each module can vary based on the firmware. Statistics
 * data are 32-bit unsigned integers.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16ReadStatisticsFromModule
 *
 * @param[out] Statistics A pointer to the statistics array containing enough space to hold the
*     statistics data as 32-bit unsigned integers. The necessary size can be obtained from
*     ::Pixie16GetStatisticsSize.
 * @param[in] ModNum The module number that we want statistics data from.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16ReadStatisticsFromModule(unsigned int* Statistics,
                                                           unsigned short ModNum);

/**
 * @ingroup PIXIE16_API
 * @brief Save DSP parameters to a settings file
 *
 * @note The settings file is an internally managed file and should not be modified by the user.
 *
 * This function saves the DSP parameters to the provided file name in JSON format. The
 * JSON format replaces the legacy binary format.
 *
 * The data file now contains only the information necessary to initialize a module's
 * variables and metadata about the module that was previously used. The metadata contains
 *
 *   * the module's serial number,
 *   * the firmware associated with the module,
 *   * the ADC information for each channel of the module,
 *   * and other information that may be useful to reconstruct a specific setup.
 *
 * The API will only write out settings for active modules within the system. For example,
 * if a crate has 5 modules (4 online and 1 offline), then the settings file will contain
 * entries for the 4 online modules.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16SaveDSPParametersToFile
 *
 * @param[in] FileName Absolute path to where we will write the DSP parameter file.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16SaveDSPParametersToFile(const char* FileName);

/**
 * @ingroup PIXIE16_API
 * @brief Retrieve histogram data from a Pixie module and then save the data to a file.
 *
 * @warning This function is not currently implemented.
 * @warning This function will be deprecated July 31, 2023 with the Legacy API.
 *
 * Use this function to read histogram data from a Pixie-16 module and save the histogram data to
 * a file with file name specified by the user. First, this function saves the histogram data to a
 * binary file, and it then saves the histogram data to an ASCII file with run statistics data
 * appended to the end of the ASCII file. **Existing files will be overwritten.**
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16SaveHistogramToFile
 *
 * @param[in] FileName The file name for the file containing the histogram data
 * @param[in] ModNum The module number we'll read histogram data from. Counting from 0.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16SaveHistogramToFile(const char* FileName, unsigned short ModNum);

/**
 * @ingroup PIXIE16_API
 * @brief Set all DACs.
 *
 * Use this function to reprogram the on-board digital to analog converters (DAC) of the Pixie-16
 * modules. In this operation the DSP uses data from the DSP parameters that were previously
 * downloaded.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16SetDACs
 *
 * @param[in] ModNum The module number, which starts counting at 0.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16SetDACs(unsigned short ModNum);

/**
 * @ingroup PIXIE16_API
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
 * prematurely terminate the run by calling ::Pixie16EndRun. On completion, final histogram and
 * statistics data will be available.
 *
 * ModNum is the module number which starts counting at 0. If ModNum is set to be less than the
 * total number of modules in the system, only the module specified by ModNum will have its
 * histogram run started. But if ModNum is set to be equal to the total number of modules in the
 * system, then all modules in the system will have their runs started together.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16StartHistogramRun
 *
 * @param[in] ModNum The module number that we'd like to work against. Starts counting at 0.
 * @param[in] mode How we'll handle existing data when starting up the run. Use mode=NEW_RUN (=1)
 *     to erase histograms and statistics information before launching the new
 *     run. Use mode=RESUME_RUN (=0) to resume an earlier run.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16StartHistogramRun(unsigned short ModNum, unsigned short mode);

/**
 * @ingroup PIXIE16_API
 * @brief Start or resume a list-mode run.
 *
 * Use this function to start a list-mode data acquisition run in Pixie-16 modules. list-mode runs
 * are used to collect data on an event-by-event basis, gathering energies, timestamps, pulse shape
 * analysis values, and waveforms for each event. Runs will continue until the user terminates the
 * run by calling ::Pixie16EndRun. To start the data acquisition, this function has to be
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
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16StartListModeRun
 *
 * @param[in] ModNum The module number that we'd like to work against. Starts counting at 0.
 * @param[in] RunType The type of run that we're going to be executing. There's only one 0x100.
 * @param[in] mode How we'll handle existing data when starting up the run. Use mode=NEW_RUN (=1)
 *     to erase histograms and statistics information before launching the new
 *     run. Note that this will cause a startup delay of up to 1 millisecond.
 *     Use mode=RESUME_RUN (=0) to resume an earlier run. This mode has a startup delay of only a
 *     few microseconds.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16StartListModeRun(unsigned short ModNum, unsigned short RunType,
                                                   unsigned short mode);

/**
 * @ingroup PIXIE16_API
 * @brief Find the exponential decay constant of the detector/preamplifier signals.
 * Use this function to find the exponential decay time constants (Tau value) of the detector or
 * preamplifier signal that is connected to each of the 16 channels of a Pixie-16 module. Tau
 * is used as part of the on-board energy calculations.
 *
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16TauFinder
 *
 * @see [High Rate Pulse Processing Algorithms for Microcalorimeters](https://xia.com/wp-content/uploads/2021/03/High-Rate-Pulse-Processing-Algorithms-for-Microcalorimeters-Tan-2008-.pdf)
 *
 * @param[in] ModNum The module number to work with. Starts counting at 0.
 * @param[out] Tau Pointer to an array big enough to hold the calculated tau value for each
 *     channel of the module. A value of -1.0 indicates a failed calculation for that channel.
 *     Units are microseconds.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16TauFinder(unsigned short ModNum, double* Tau);

/**
 * @ingroup PIXIE16_API
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
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16WriteSglChanPar
 *
 * @param[in] ChanParName The name of the channel parameter that we'll write
 * @param[in] ChanParData The value that we'll write to the provided channel
 * @param[in] ModNum The module number we'll write to. Counting starts at 0.
 * @param[in] ChanNum The channel number we'll write to. Counting starts at 0.
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16WriteSglChanPar(const char* ChanParName, double ChanParData,
                                                  unsigned short ModNum, unsigned short ChanNum);

/**
 * @ingroup PIXIE16_API
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
 * ### Example
 * \snippet snippets/api_function_examples.c Pixie16WriteSglModPar
 *
 * @param[in] ModParName The name of the module parameter that we'll write
 * @param[in] ModParData The value of the module parameter that we'll write
 * @param[in] ModNum The module number we'll write to. Numbering starts counting at 0.
 *
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API Pixie16WriteSglModPar(const char* ModParName, unsigned int ModParData,
                                                 unsigned short ModNum);

/**
 * @ingroup PIXIE_API
 *
 * These functions provide new API interfaces not provided in the Legacy API.
 */

/**
 * @ingroup PIXIE_API
 * @brief An enum defining the various ways users can boot a crate.
 *
 * This enumeration replaces the old boot flags. The previous flags allowed users
 * to potentially put the system into a state that prevented proper operation.
 * These flags now represent the most commonly used boot procedures in use.
 *
 */
enum PIXIE_BOOT_MODE {
    /**
     * @brief Probe the modules to see if they are online. No other action is
     *    performed.
     */
    PIXIE_BOOT_PROBE,
    /**
     * @brief Reset the programmable devices on a module and load them. If
     *    settings are provided import them and synchronize the settings
     *    with the hardware.
     */
    PIXIE_BOOT_RESET_LOAD,
    /**
     * @brief Import the settings and synchronize the settings with the
     *    hardware. The module must be online.
     */
    PIXIE_BOOT_SETTINGS_LOAD
};

/**
 * @ingroup PIXIE_API
 * @brief Boot a crate
 *
 * Boot a crate modules. This operation is faster than booting modules
 * individually. Registered firmware sets are used to boot the modules. The
 * COMM, FIPPI FPGA and DSP code and variable map files are loaded together.
 *
 * The provided settings file is loaded after the modules have been booted
 * online. This does not depend on state of @ref fast_boot.
 *
 * A module is considered offline if the COMMS, FIPPI or DSP is not loaded and
 * running.
 *
 * @param[in] settings_file The path to a crate's settings file
 * @param[in] boot_mode The mode to boot the module with
 *
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API PixieBootCrate(const char* settings_file,
                                          const enum PIXIE_BOOT_MODE boot_mode);

/**
 * @ingroup PIXIE_API
 * @brief Gets a worker configuration from the specified module
 * @param mod_num The module number to get the configuration from.
 * @param worker_config A pointer to the configuration object to fill with the information
 * @return The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API PixieGetWorkerConfiguration(unsigned short mod_num,
                                                       struct fifo_worker_config* worker_config);

/**
 * @ingroup PIXIE_API
 * @brief Register a module's firmware set
 *
 * Registering a module firmware sets makes it available for use whem booting a
 * crate. A module number can be optionally provided to specify a specific
 * firmware set for that module. A @ref ModNum of 0 makes the firmware set
 * available to all matching modules.
 *
 * @param[in] version The firmware version as an integer
 * @param[in] revision The associated hardware revision as an integer. Ex. Rev F = 0xF, Rev H = 0x11
 * @param[in] adc_msps The ADC sampling frequency associated with the firmware. Ex. 250
 * @param[in] adc_bits The ADC bit resolution associated with the firmware. Ex. 14
 * @param[in] device The device associated with the firmware. One of dsp, var, fippi, sys
 * @param[in] path The absolute path to the firmware file on the host system.
 * @param[in] ModNum The module that will be registered with this firmware.
 *
 * @returns The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API PixieRegisterFirmware(const unsigned int version, const int revision,
                                                 const int adc_msps, const int adc_bits,
                                                 const char* device, const char* path,
                                                 unsigned short ModNum);

/**
 * @ingroup PIXIE_API
 * @brief Sets a worker configuration in the specified module
 * @param mod_num The module number to set the configuration.
 * @param worker_config A pointer to the configuration object with the necessary information
 * @return The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API PixieSetWorkerConfiguration(unsigned short mod_num,
                                                       struct fifo_worker_config* worker_config);

/**
 * @ingroup PIXIE_API
 * @brief Read the run's statistics for the module. if a run as finished the statistics
 * are for the last run.
 * @param mod_num The module number to read the statistics from.
 * @param fifo_stats A pointer to the statistics the module data is copied too.
 * @return The value of the xia::pixie::error::code indicating the result of the operation
 */
PIXIE_EXPORT int PIXIE_API PixieReadRunFifoStats(unsigned short mod_num,
                                                 struct module_fifo_stats* fifo_stats);

#ifdef __cplusplus
}
#endif

#endif /* PIXIE_H */
