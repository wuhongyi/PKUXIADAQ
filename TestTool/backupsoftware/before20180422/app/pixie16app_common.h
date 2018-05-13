#ifndef __PIXIE16APP_COMMON_H
#define __PIXIE16APP_COMMON_H

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
 *     contributors may be used to endorse or promote products
 *     derived from this software without specific prior
 *     written permission.
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
 *		pixie16app_common.h
 *
 * Description:
 *
 *		Function prototypes shared among multiple files.
 *
 * $Rev: 27118 $
 * $Id: pixie16app_common.h 27118 2013-05-17 01:55:17Z htan $
 ******************************************************************************/

// If this is compiled by a C++ compiler, make it
// clear that these are C routines.
#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------- 
	Functions that are only used locally
  -------------------------------------*/

int Pixie_Start_Run (
	unsigned short mode,           // mode = NEW_RUN or RESUME_RUN
	unsigned short run_task,       // run task number
	unsigned short control_task,   // control task number
	unsigned short ModNum );

int Pixie_End_Run (
	unsigned short ModNum );

int Pixie_Check_Run_Status (
	unsigned short ModNum );

int Pixie_Control_Task_Run (
	unsigned short ModNum,        // Pixie module number
	unsigned short ControlTask,   // Control task number
	unsigned int   Max_Poll );    // Timeout control in unit of ms for control task run

int Pixie_Broadcast (
	char *str,                      // variable name whose value is to be broadcasted
	unsigned short SourceModule );  // the source module number

int Pixie_ComputeFIFO (
	unsigned int TraceDelay,    // current trace dealy value
	unsigned short ModNum,      // Pixie module number
	unsigned short ChanNum );   // Pixie channel number

int Pixie_CopyDSPParameters (
	unsigned short BitMask,              // copy/extract bit mask pattern
	unsigned short SourceModule,         // source module number
	unsigned short SourceChannel,        // source Pixie channel
	unsigned short DestinationModule,    // destination module number
	unsigned short DestinationChannel ); // destination channel number

int Pixie_Init_DSPVarAddress(char *DSPVarFile, unsigned short ModNum);

int Pixie_Copy_DSPVarAddress(unsigned short SourceModNum, unsigned short DestinationModNum);

#ifdef __cplusplus
}
#endif

#endif


