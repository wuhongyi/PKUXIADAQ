/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http:www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file pixie16app_defs.h
 * @brief Defines user-facing constants for use with the API.
 * @note This file will be deprecated July 31, 2023
 */

#ifndef XIA_PIXIE16APP_DEFS_H
#define XIA_PIXIE16APP_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup PUBLIC Public
 * @ingroup LEGACY
 * Functions for use in end-user software.
 *
 * @defgroup APP Application
 * @ingroup PUBLIC
 * A group of functions and macros providing the application interface.
 *
 * @defgroup PUBLIC_UTILITIES Utilities
 * @ingroup PUBLIC
 * A group of functions and macros providing support functionality to the API exposed to users.
 *
 * @defgroup PRIVATE_UTILITIES Utilities
 * @ingroup PRIVATE
 * A group of private functions providing support functionality to the API.
 */

#if defined(_WIN64) || defined(_WIN32)
#define PIXIE16APP_EXPORT __declspec(dllexport)
#define PIXIE16APP_API _stdcall
#else
#define PIXIE16APP_EXPORT
#define PIXIE16APP_API
#endif

/*-------------------------------------
    Define math constants
  -------------------------------------*/

#ifndef PI
#define PI 3.14159265358979
#endif

#ifndef PI2
#define PI2 6.28318530717959
#endif


/*------------------------------------- 
    Define boot patterns
  -------------------------------------*/
#define BOOTPATTERN_COMFPGA_BIT 0
#define BOOTPATTERN_SPFPGA_BIT 2
#define BOOTPATTERN_DSPCODE_BIT 3
#define BOOTPATTERN_DSPPAR_BIT 4
#define BOOTPATTERN_PROGFIPPI_BIT 5
#define BOOTPATTERN_SETDACS_BIT 6


/*-----------------------------------------------------------------
    size of system FPGA, trigger FPGA, Fippi, DSP parameters files
  -----------------------------------------------------------------*/
/**
 * @ingroup APP
 * size of communications FPGA configuration (32-bit word) in Revision B,C,D modules.
 */
#define N_COM_FPGA_CONF_REVBCD 162962
/**
 * @ingroup APP
 * size of signal processing FPGA configuration (32-bit word) in Revision B,C,D modules.
 */
#define N_SP_FPGA_CONF_REVBCD 162962
/**
 * @ingroup APP
 * size of communications FPGA configuration (32-bit word) in Revision F modules
 */
#define N_COM_FPGA_CONF_REVF 162962
/**
 * @ingroup APP
 * size of signal processing FPGA configuration (32-bit word) in Revision F modules
 */
#define N_SP_FPGA_CONF_REVF 371101
/**
 * @ingroup APP
 * number of DSP parameters (32-bit word)
 */
#define N_DSP_PAR 1280
/**
 * @ingroup APP
 * number of DSP I/O variables
 */
#define DSP_IO_BORDER 832


/*-----------------------------------------------------------------
    module specifications
  -----------------------------------------------------------------*/
/**
 * @ingroup APP
 * Preset maximum number of Pixie modules
 */
#define PRESET_MAX_MODULES 24
/**
 * @ingroup APP
 * Maximum number of channels in a Pixie-16 module.
 */
#define NUMBER_OF_CHANNELS 16
/**
 * @ingroup APP
 * DSP clock frequency in MHz
*/
#define DSP_CLOCK_MHZ 100
/**
 * @ingroup APP
 * System FPGA clock frequency in MHz (used for real time counting)
 */
#define SYSTEM_CLOCK_MHZ 100
/**
 * @ingroup APP
 * Pixie-16 DAC range is -1.5 V to +1.5 V
 */
#define DAC_VOLTAGE_RANGE 3.0
/**
 * @ingroup APP
 * Maximum ADC trace length for a channel
 */
#define MAX_ADC_TRACE_LEN 8192

/*-----------------------------------------------------------------
    run type
  -----------------------------------------------------------------*/

/**
 * @ingroup APP
 * New data run
 */
#define NEW_RUN 1
/**
 * @ingroup APP
 * Resume run
 */
#define RESUME_RUN 0

/**
 * @ingroup APP
 * List mode run
 */
#define LIST_MODE_RUN 0x100
/**
 * @ingroup APP
 * Histogram run
 */
#define HISTOGRAM_RUN 0x301


/*-----------------------------------------------------------------
    I/O mode
  -----------------------------------------------------------------*/

/**
 * @ingroup APP
 * Host read from modules
 */
#define MOD_READ 1
/**
 * @ingroup APP
 * Host write to modules
 */
#define MOD_WRITE 0


/*-----------------------------------------------------------------
    Data memory, buffer, histogram, and list mode data structure
  -----------------------------------------------------------------*/
// The next 4 defines are all 32 - bit wide.
#define DSP_IMBUFFER_START_ADDR 0x40000
#define DSP_IMBUFFER_END_ADDR 0x5FFFF

#define DSP_EMBUFFER_START_ADDR 0x0
#define DSP_EMBUFFER_END_ADDR 0x7FFFF

/**
 * @ingroup APP
 * DSP data memory address
 */
#define DATA_MEMORY_ADDRESS 0x4A000
/**
 * @ingroup APP
 * histogram memory buffer in external memory
 */
#define HISTOGRAM_MEMORY_ADDRESS 0x0
/**
 * @ingroup APP
 * Maximum MCA histogram length
 */
#define MAX_HISTOGRAM_LENGTH 32768
/**
 * @ingroup APP
 * Address of I/O output buffer
 */
#define IO_BUFFER_ADDRESS 0x50000
/**
 * @ingroup APP
 * Length of I/O output buffer
 */
#define IO_BUFFER_LENGTH 65536
/**
 * @ingroup APP
 * Length of external FIFO
 */
#define EXTERNAL_FIFO_LENGTH 131072

/**
 * @ingroup APP
 * Information length for each event
 */
#define EVENT_INFO_LENGTH 68
/**
 * @ingroup APP
 * Information length for each channel
 */
#define CHANNEL_INFO_LENGTH 4
/**
 * @ingroup APP
 * Information length for each event header
 */
#define EVENT_INFO_HEADER_LENGTH 4


/*-------------------------------------
	Length limits for certain DSP parameters
 --------------------------------------*/

#define FASTFILTER_MAX_LEN 127
#define FAST_THRESHOLD_MAX 65535
#define MIN_FASTLENGTH_LEN 2

#define SLOWFILTER_MAX_LEN 127
#define MIN_SLOWLENGTH_LEN 2
#define MIN_SLOWGAP_LEN 3

#define FASTFILTERRANGE_MAX 0
#define FASTFILTERRANGE_MIN 0

#define SLOWFILTERRANGE_MAX 6
#define SLOWFILTERRANGE_MIN 1

#define FASTTRIGBACKLEN_MAX 4095
#define FASTTRIGBACKLEN_MIN_100MHZFIPCLK 1
#define FASTTRIGBACKLEN_MIN_125MHZFIPCLK 2

#define CFDDELAY_MAX 63
#define CFDDELAY_MIN 1

#define CFDSCALE_MAX 7

#define CFDTHRESH_MAX 65535
#define CFDTHRESH_MIN 1

#define EXTTRIGSTRETCH_MAX 4095
#define EXTTRIGSTRETCH_MIN 1

#define VETOSTRETCH_MAX 4095
#define VETOSTRETCH_MIN 1

#define EXTDELAYLEN_MAX_REVBCD 255
#define EXTDELAYLEN_MAX_REVF 511
#define EXTDELAYLEN_MIN 0

#define FASTTRIGBACKDELAY_MAX_REVBCD 255
#define FASTTRIGBACKDELAY_MAX_REVF 511
#define FASTTRIGBACKDELAY_MIN 0

#define QDCLEN_MAX 32767
#define QDCLEN_MIN 1

#define TRACELEN_MIN_500MHZADC 10
#define TRACELEN_MIN_250OR100MHZADC 4

#define TRACEDELAY_MAX 1023

#define CHANTRIGSTRETCH_MAX 4095
#define CHANTRIGSTRETCH_MIN 1

/*-------------------------------------
	CHANCSRA bits definitions
 --------------------------------------*/

/**
 * @ingroup APP
 * fast trigger selection - 1: select external fast trigger; 0: select group trigger
 */
#define CCSRA_FTRIGSEL 0
/**
 * @ingroup APP
 * module validation signal selection (RevD & RevF only)
 * * 1: select module gate signal
 * * 0: select global validation signal
 */
#define CCSRA_EXTTRIGSEL 1
/**
 * @ingroup APP
 * good-channel bit - 1: channel data will be read out; 0: channel data will not be read out
 */
#define CCSRA_GOOD 2
/**
 * @ingroup APP
 * channel validation signal selection - 1: select channel gate signal; 0: select channel validation signal (RevD & RevF only)
 */
#define CCSRA_CHANTRIGSEL 3
/**
 * @ingroup APP
 * block data acquisition if trace or header DPMs are full - 1: enable; 0: disable
 */
#define CCSRA_SYNCDATAACQ 4
/**
 * @ingroup APP
 * input signal polarity control
 */
#define CCSRA_POLARITY 5
/**
 * @ingroup APP
 * veto channel trigger - 1: enable; 0: disable
 */
#define CCSRA_VETOENA 6
/**
 * @ingroup APP
 * histogram energy in the on-board MCA
 */
#define CCSRA_HISTOE 7
/**
 * @ingroup APP
 * trace capture and associated header data - 1: enable; 0: disable
 */
#define CCSRA_TRACEENA 8
/**
 * @ingroup APP
 * QDC summing and associated header data - 1: enable; 0: dsiable
 */
#define CCSRA_QDCENA 9
/**
 * @ingroup APP
 * CFD for real time, trace capture and QDC capture - 1: enable; 0: disable
 */
#define CCSRA_CFDMODE 10
/**
 * @ingroup APP
 * global trigger for validation - 1: enable; 0: disable
 */
#define CCSRA_GLOBTRIG 11
/**
 * @ingroup APP
 * raw energy sums and baseline in event header - 1: enable; 0: disable
 */
#define CCSRA_ESUMSENA 12
/**
 * @ingroup APP
 * channel trigger for validation - 1: enable; 0: disable
 */
#define CCSRA_CHANTRIG 13
/**
 * @ingroup APP
 * Control input relay: 1: connect, 0: disconnect
 */
#define CCSRA_ENARELAY 14

/**
 * @ingroup APP
 * Used in conjunction with ChanCSRA Bit 16 to control pile-up behavior.
 *
 * | Bit 15 | Bit 16 | Effect |
 * |---|---|---|
 * | 0 | 0 | record all events (trace, timestamps, etc., but no energy for piled-up events) |
 * | 0 | 1 | only record single events (trace, energy, timestamps, etc.) (i.e., reject piled-up events) |
 * | 1 | 0 | record trace, timestamps, etc., for piled-up events but do not record trace for single events |
 * | 1 | 1 | only record trace, timestamps, etc., for piled-up events (i.e., reject single events) |
 */
#define CCSRA_PILEUPCTRL 15
/**
 * @ingroup APP
 * Used in conjunction with ChanCSRA Bit 15 to control pile-up behavior.
 *
 * | Bit 15 | Bit 16 | Effect |
 * |---|---|---|
 * | 0 | 0 | record all events (trace, timestamps, etc., but no energy for piled-up events) |
 * | 0 | 1 | only record single events (trace, energy, timestamps, etc.) (i.e., reject piled-up events) |
 * | 1 | 0 | record trace, timestamps, etc., for piled-up events but do not record trace for single events |
 * | 1 | 1 | only record trace, timestamps, etc., for piled-up events (i.e., reject single events) |
 */
#define CCSRA_INVERSEPILEUP 16

/**
 * @ingroup APP
 * Enable "no trace for large pulses" feature - 1: enable; 0: disable
 */
#define CCSRA_ENAENERGYCUT 17
/**
 * @ingroup APP
 * Group trigger selection - 1: external group trigger; 0: local fast trigger
 */
#define CCSRA_GROUPTRIGSEL 18
/**
 * @ingroup APP
 * Channel veto selection - 1: channel validation trigger; 0: front panel channel veto
 */
#define CCSRA_CHANVETOSEL 19
/**
 * @ingroup APP
 * Module veto selection - 1: module validation trigger; 0: front panel module veto
 */
#define CCSRA_MODVETOSEL 20
/**
 * @ingroup APP
 * External timestamps in event header - 1: enable; 0: disable
 */
#define CCSRA_EXTTSENA 21

/*-------------------------------------
	MODCSRB bits definitions
 --------------------------------------*/
/**
 * @ingroup APP
 * Control pullups for PXI trigger lines on the backplane through CPLD
 */
#define MODCSRB_CPLDPULLUP 0
/**
 * @ingroup APP
 * Set this module as the Director module (1) or non-Director module (0)
 */
#define MODCSRB_DIRMOD 4
/**
 * @ingroup APP
 * Control chassis master module: 1: chassis master module; 0: chassis non-master module
 */
#define MODCSRB_CHASSISMASTER 6
/**
 * @ingroup APP
 * Select global fast trigger source
 */
#define MODCSRB_GFTSEL 7
/**
 * @ingroup APP
 * Select external trigger source
 */
#define MODCSRB_ETSEL 8
/**
 * @ingroup APP
 * Control external INHIBIT signal: use INHIBIT (1) or don't use INHIBIT (0)
 */
#define MODCSRB_INHIBITENA 10
/**
 * @ingroup APP
 * Distribute clock and triggers in multiple crates: multiple crates (1) or only single crate (0)
 */
#define MODCSRB_MULTCRATES 11
/**
 * @ingroup APP
 * Sort (1) or don't sort events based on their timestamps
 */
#define MODCSRB_SORTEVENTS 12
/**
 * @ingroup APP
 * Enable connection of fast triggers to backplane
 */
#define MODCSRB_BKPLFASTTRIG 13

/*-------------------------------------
	CPLD CSR bits definitions
 --------------------------------------*/
/**
 * @ingroup APP
 * Control connections of PXI nearest neighbor lines (J2) onto the backplane for Rev-B/C/D modules
 */
#define CPLDCSR_BPCONNECT 12
/**
 * @ingroup APP
 * Control backplane pullups: 1: pulled up, 0: not pulled up
 */
#define CPLDCSR_PULLUP 13

/*-------------------------------------
	Control parameters
 --------------------------------------*/
/**
 * @ingroup APP
 * Maximum length of parameter names
 */
#define MAX_PAR_NAME_LENGTH 65
/**
 * @ingroup APP
 * number of random indices (currently only used for tau finder)
 */
#define RANDOMINDICES_LENGTH 8192
/**
 * @ingroup APP
 * Maximum length of error message
 */
#define MAX_ERRMSG_LENGTH 1024

/**
 * @ingroup APP
 * Length of each baselines length (default: 2 timestamp words + 16 baselines)
 */
#define BASELINES_BLOCK_LEN 18
/**
 * @ingroup APP
 * Maximum number of baselines available after each baseline acquisition run
 */
#define MAX_NUM_BASELINES 3640
/**
 * @ingroup APP
 * Maximum number of DC values available after each RampOffsetDACs run
 */
#define MAX_NUM_DCVALUES 16384

/**
 * @ingroup APP
 * Reading out threshold for external FIFO watermmark level
 */
#define EXTFIFO_READ_THRESH 1024

/**
 * @ingroup APP
 * PCI register address in the System FPGA for stopping run
 */
#define PCI_STOPRUN_REGADDR 0x44

/*-------------------------------------
	Frequently used Control Tasks
 --------------------------------------*/

/**
 * @ingroup APP
 * Set DACs
 */
#define SET_DACS 0
/**
 * @ingroup APP
 * Enable detect signal input
 */
#define ENABLE_INPUT 1
/**
 * @ingroup APP
 * Ramp Offset DACs
 */
#define RAMP_OFFSETDACS 3
/**
 * @ingroup APP
 * Acquire ADC traces
 */
#define GET_TRACES 4
/**
 * @ingroup APP
 * Program FIPPIs
 */
#define PROGRAM_FIPPI 5
/**
 * @ingroup APP
 * Get baselines
 */
#define GET_BASELINES 6
/**
 * @ingroup APP
 * Adjust DC-offsets
 */
#define ADJUST_OFFSETS 7
/**
 * @ingroup APP
 * Find decay time Tau
 */
#define TAU_FINDER 8
/**
 * @ingroup APP
 * Reset ADCs
 */
#define RESET_ADC 23

#ifdef __cplusplus
}
#endif

#endif
