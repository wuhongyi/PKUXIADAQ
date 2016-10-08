#ifndef PIXIE16APP_GLOBALS_H
#define PIXIE16APP_GLOBALS_H

/*----------------------------------------------------------------------
 * Copyright (c) 2005 - 2009, XIA LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, 
 * with or without modification, are permitted provided 
 * that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above 
 *     copyright notice, this list of conditions and the 
 *     following disclaimer.
 *   * Redistributions in binary form must reproduce the 
 *     above copyright notice, this list of conditions and the 
 *     following disclaimer in the documentation and/or other 
 *     materials provided with the distribution.
 *   * Neither the name of XIA LLC nor the names of its
 *     contributors may be used to endorse or promote
 *     products derived from this software without 
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE.
 *----------------------------------------------------------------------*/

/******************************************************************************
 *
 * File Name:
 *
 *		pixie16app_globals.h
 *
 * Description:
 *
 *		Declaration of global variables and data arrays.
 *
 * $Rev: 29161 $
 * $Id: pixie16app_globals.h 29161 2014-01-09 19:27:58Z htan $
 ******************************************************************************/

// If this is compiled by a C++ compiler, make it
// clear that these are C routines.
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PIXIE16APP_DEFS_H
	#include "pixie16app_defs.h"
#endif


unsigned short Number_Modules;                       // Total number of modules in the crate
unsigned short Random_Set[RANDOMINDICES_LENGTH];     // Random indices
unsigned short Offline;   // Offline = 1: offline analysis; Offline = 0: online analysis


/*-----------------------------------------------------------------
    Pixie global data structure
  -----------------------------------------------------------------*/

struct Pixie_Configuration
{
	
	// DSP I/O parameter values
	unsigned int DSP_Parameter_Values[N_DSP_PAR];

};

// Define PRESET_MAX_MODULES Pixie devices
struct Pixie_Configuration Pixie_Devices[PRESET_MAX_MODULES];


struct Module_Info
{
	// Module information
	unsigned short Module_Rev;
	unsigned int   Module_SerNum;
	unsigned short Module_ADCBits;
	unsigned short Module_ADCMSPS;
};

// Define PRESET_MAX_MODULES Pixie devices
struct Module_Info Module_Information[PRESET_MAX_MODULES];


/*-----------------------------------------------------------------
    DSP I/O parameters address
		We initialize these addressee when we boot the modules, then
		we can use them when we need to change DSP parameters.
  -----------------------------------------------------------------*/

//--------------------
//	Module parameters
//--------------------

unsigned int ModNum_Address[PRESET_MAX_MODULES];         // ModNum
unsigned int ModCSRA_Address[PRESET_MAX_MODULES];        // ModCSRA
unsigned int ModCSRB_Address[PRESET_MAX_MODULES];        // ModCSRB
unsigned int ModFormat_Address[PRESET_MAX_MODULES];      // ModFormat
unsigned int RunTask_Address[PRESET_MAX_MODULES];        // RunTask
unsigned int ControlTask_Address[PRESET_MAX_MODULES];    // ControlTask
unsigned int MaxEvents_Address[PRESET_MAX_MODULES];      // MaxEvents
unsigned int CoincPattern_Address[PRESET_MAX_MODULES];   // CoincPattern
unsigned int CoincWait_Address[PRESET_MAX_MODULES];      // CoincWait
unsigned int SynchWait_Address[PRESET_MAX_MODULES];      // SynchWait
unsigned int InSynch_Address[PRESET_MAX_MODULES];        // InSynch
unsigned int Resume_Address[PRESET_MAX_MODULES];         // Resume
unsigned int SlowFilterRange_Address[PRESET_MAX_MODULES];// SlowFilterRange
unsigned int FastFilterRange_Address[PRESET_MAX_MODULES];// FastFilterRange
unsigned int ChanNum_Address[PRESET_MAX_MODULES];        // ChanNum
unsigned int HostIO_Address[PRESET_MAX_MODULES];         // HostIO
unsigned int UserIn_Address[PRESET_MAX_MODULES];         // UserIn
unsigned int U00_Address[PRESET_MAX_MODULES];            // U00
unsigned int FastTrigBackplaneEna_Address[PRESET_MAX_MODULES]; // Enabling sending fast trigger to backplane
unsigned int CrateID_Address             [PRESET_MAX_MODULES]; // CrateID
unsigned int SlotID_Address              [PRESET_MAX_MODULES]; // SlotID
unsigned int ModID_Address               [PRESET_MAX_MODULES]; // ModID
unsigned int TrigConfig_Address          [PRESET_MAX_MODULES]; // General configuration registers
unsigned int HRTP_Address[PRESET_MAX_MODULES];           // HostRunTimePreset

//--------------------
//	Channel parameters
//--------------------

unsigned int ChanCSRa_Address[PRESET_MAX_MODULES];       // ChanCSRa
unsigned int ChanCSRb_Address[PRESET_MAX_MODULES];       // ChanCSRb

unsigned int GainDAC_Address[PRESET_MAX_MODULES];        // GainDAC

unsigned int OffsetDAC_Address[PRESET_MAX_MODULES];      // OffsetDAC

unsigned int DigGain_Address[PRESET_MAX_MODULES];        // DigGain

unsigned int SlowLength_Address[PRESET_MAX_MODULES];     // SlowLength
unsigned int SlowGap_Address[PRESET_MAX_MODULES];        // SlowGap
unsigned int FastLength_Address[PRESET_MAX_MODULES];     // FastLength
unsigned int FastGap_Address[PRESET_MAX_MODULES];        // FastGap
unsigned int PeakSample_Address[PRESET_MAX_MODULES];     // PeakSample
unsigned int PeakSep_Address[PRESET_MAX_MODULES];        // PeakSep

unsigned int CFDThresh_Address[PRESET_MAX_MODULES];      // CFDThresh

unsigned int FastThresh_Address[PRESET_MAX_MODULES];     // FastThresh
unsigned int ThreshWidth_Address[PRESET_MAX_MODULES];    // ThreshWidth
unsigned int PAFlength_Address[PRESET_MAX_MODULES];      // PAFlength
unsigned int TriggerDelay_Address[PRESET_MAX_MODULES];   // TriggerDelay
unsigned int ResetDelay_Address[PRESET_MAX_MODULES];     // ResetDelay
unsigned int ChanTrigStretch_Address[PRESET_MAX_MODULES];// ChanTrigStretch
unsigned int TraceLength_Address[PRESET_MAX_MODULES];    // TraceLength
unsigned int TrigOutLen_Address[PRESET_MAX_MODULES];     // TrigOutLen
unsigned int EnergyLow_Address[PRESET_MAX_MODULES];      // EnergyLow
unsigned int Log2Ebin_Address[PRESET_MAX_MODULES];       // Log2Ebin

unsigned int MultiplicityMaskL_Address[PRESET_MAX_MODULES]; // Multiplicity contribution mask - low 32-bit

unsigned int PSAoffset_Address[PRESET_MAX_MODULES];      // PSAoffset
unsigned int PSAlength_Address[PRESET_MAX_MODULES];      // PSAlength
unsigned int Integrator_Address[PRESET_MAX_MODULES];     // Integrator

unsigned int BLcut_Address[PRESET_MAX_MODULES];          // BLcut
unsigned int BaselinePercent_Address[PRESET_MAX_MODULES];// BaselinePercent

unsigned int FtrigoutDelay_Address[PRESET_MAX_MODULES];  // Fast trigger output delay for system synchronization

unsigned int Log2Bweight_Address[PRESET_MAX_MODULES];    // Log2Bweight
unsigned int PreampTau_Address[PRESET_MAX_MODULES];      // PreampTau

unsigned int MultiplicityMaskH_Address[PRESET_MAX_MODULES]; // Multiplicity contribution mask - high 32-bit

unsigned int FastTrigBackLen_Address[PRESET_MAX_MODULES];	// FastTrigBackLen

unsigned int CFDDelay_Address      [PRESET_MAX_MODULES];	// CFD delay
unsigned int CFDScale_Address      [PRESET_MAX_MODULES];	// CFD scale
unsigned int ExternDelayLen_Address[PRESET_MAX_MODULES];	// delay length for each channel's input signal
unsigned int ExtTrigStretch_Address[PRESET_MAX_MODULES];	// external trigger stretch
unsigned int VetoStretch_Address   [PRESET_MAX_MODULES];	// veto stretch
unsigned int QDCLen0_Address       [PRESET_MAX_MODULES];	// QDC #0 length
unsigned int QDCLen1_Address       [PRESET_MAX_MODULES];	// QDC #1 length
unsigned int QDCLen2_Address       [PRESET_MAX_MODULES];	// QDC #2 length
unsigned int QDCLen3_Address       [PRESET_MAX_MODULES];	// QDC #3 length
unsigned int QDCLen4_Address       [PRESET_MAX_MODULES];	// QDC #4 length
unsigned int QDCLen5_Address       [PRESET_MAX_MODULES];	// QDC #5 length
unsigned int QDCLen6_Address       [PRESET_MAX_MODULES];	// QDC #6 length
unsigned int QDCLen7_Address       [PRESET_MAX_MODULES];	// QDC #7 length

unsigned int Xwait_Address[PRESET_MAX_MODULES];				// Xwait


//--------------------
//	Results parameters
//--------------------

unsigned int RealTimeA_Address[PRESET_MAX_MODULES];    // RealTimeA
unsigned int RealTimeB_Address[PRESET_MAX_MODULES];    // RealTimeB
unsigned int RunTimeA_Address[PRESET_MAX_MODULES];     // RunTimeA
unsigned int RunTimeB_Address[PRESET_MAX_MODULES];     // RunTimeB
unsigned int GSLTtime_Address[PRESET_MAX_MODULES];     // GSLTtime
unsigned int NumEventsA_Address[PRESET_MAX_MODULES];   // NumEventsA
unsigned int NumEventsB_Address[PRESET_MAX_MODULES];   // NumEventsB
unsigned int DSPerror_Address[PRESET_MAX_MODULES];     // DSPerror
unsigned int SynchDone_Address[PRESET_MAX_MODULES];    // SynchDone
unsigned int BufHeadLen_Address[PRESET_MAX_MODULES];   // BufHeadLen
unsigned int EventHeadLen_Address[PRESET_MAX_MODULES]; // EventHeadLen
unsigned int ChanHeadLen_Address[PRESET_MAX_MODULES];  // ChanHeadLen
unsigned int UserOut_Address[PRESET_MAX_MODULES];      // UserOut
unsigned int AOutBuffer_Address[PRESET_MAX_MODULES];   // AOutBuffer
unsigned int LOutBuffer_Address[PRESET_MAX_MODULES];   // LOutBuffer
unsigned int AECorr_Address[PRESET_MAX_MODULES];       // AECorr
unsigned int LECorr_Address[PRESET_MAX_MODULES];       // LECorr
unsigned int HardwareID_Address[PRESET_MAX_MODULES];   // HardwareID
unsigned int HardVariant_Address[PRESET_MAX_MODULES];  // HardVariant
unsigned int FIFOLength_Address[PRESET_MAX_MODULES];   // FIFOLength
unsigned int FippiID_Address[PRESET_MAX_MODULES];      // FippiID
unsigned int FippiVariant_Address[PRESET_MAX_MODULES]; // FippiVariant
unsigned int DSPrelease_Address[PRESET_MAX_MODULES];   // DSPrelease
unsigned int DSPbuild_Address[PRESET_MAX_MODULES];     // DSPbuild
unsigned int DSPVariant_Address[PRESET_MAX_MODULES];   // DSPVariant
unsigned int U20_Address[PRESET_MAX_MODULES];          // U20
unsigned int LiveTimeA_Address[PRESET_MAX_MODULES];    // LiveTimeA
unsigned int LiveTimeB_Address[PRESET_MAX_MODULES];    // LiveTimeB
unsigned int FastPeaksA_Address[PRESET_MAX_MODULES];   // FastPeaksA
unsigned int FastPeaksB_Address[PRESET_MAX_MODULES];   // FastPeaksB
unsigned int OverflowA_Address[PRESET_MAX_MODULES];    // OverflowA
unsigned int OverflowB_Address[PRESET_MAX_MODULES];    // OverflowB
unsigned int InSpecA_Address[PRESET_MAX_MODULES];      // InSpecA
unsigned int InSpecB_Address[PRESET_MAX_MODULES];      // InSpecB
unsigned int UnderflowA_Address[PRESET_MAX_MODULES];   // UnderflowA
unsigned int UnderflowB_Address[PRESET_MAX_MODULES];   // UnderflowB
unsigned int ChanEventsA_Address[PRESET_MAX_MODULES];  // ChanEventsA
unsigned int ChanEventsB_Address[PRESET_MAX_MODULES];  // ChanEventsB
unsigned int AutoTau_Address[PRESET_MAX_MODULES];      // AutoTau
unsigned int U30_Address[PRESET_MAX_MODULES];          // U30



#ifdef __cplusplus
}
#endif	// End of notice for C++ compilers

#endif	// End of pixie16app_globals.h

