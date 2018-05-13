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
*		utilities.c
*
* Description:
*
*		This file contains all the utility funtions used for memory I/O or
*		data acquisition runs.
*
* $Rev: 39391 $
* $Id: utilities.c 39391 2018-04-22 16:56:22Z htan $
*****************************************************************************/

#include "pixie16app_globals.h"
#include "pixie16sys_export.h"
#include "pixie16app_export.h"
#include "pixie16app_common.h"
#include "xia_common.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#if PIXIE16_APPAPI_VER == PIXIE16_WINDOWS_APPAPI
#include <windows.h>
#elif PIXIE16_APPAPI_VER == PIXIE16_LINUX_APPAPI
#include <unistd.h>
#endif


/****************************************************************
*	Pixie_Start_Run:
*		Start a new run or resume a run in a Pixie module or multiple
*		modules.
*
*		Return Value:
*			 0 - Success
*			-1 - Failed to stop the run that is already in progress
*
****************************************************************/

int Pixie_Start_Run (
	unsigned short mode,         // mode = NEW_RUN or RESUME_RUN
	unsigned short run_task,     // run task number
	unsigned short control_task, // control task number
	unsigned short ModNum )
{
	unsigned int   value, CSR;
	unsigned int   buffer[MAX_HISTOGRAM_LENGTH * 4] = {0};
	unsigned short k;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	int retval;
	
	if(ModNum == Number_Modules)  // Start run in all modules
	{
		// Prepare for a run: ending a previous run, clear external memory, set parameters
		for(k = 0; k < Number_Modules; k ++)
		{
			// Check if there is a run in progresss; if so, end it
			retval = Pixie_Check_Run_Status(k);
			if(retval == 1)
			{
				retval = Pixie_End_Run(k);
				// Check if Pixie_End_Run returned without errors
				if(retval < 0)
				{
					sprintf(ErrMSG, "*ERROR* (Pixie_Start_Run): Pixie_End_Run failed in module %d; retval=%d", k, retval);
					Pixie_Print_MSG(ErrMSG);
					return(-1);
				}
			}
			
			// Clear external memory first before starting a new data acquisition run
			if((mode == NEW_RUN) && (run_task != 0) && (control_task == 0))
			{
				// Clear histogram memory; channel by channel
				Pixie_Main_Memory_IO(buffer, MAX_HISTOGRAM_LENGTH * 0, MAX_HISTOGRAM_LENGTH * 4, MOD_WRITE, k);
				Pixie_Main_Memory_IO(buffer, MAX_HISTOGRAM_LENGTH * 4, MAX_HISTOGRAM_LENGTH * 4, MOD_WRITE, k);
				Pixie_Main_Memory_IO(buffer, MAX_HISTOGRAM_LENGTH * 8, MAX_HISTOGRAM_LENGTH * 4, MOD_WRITE, k);
				Pixie_Main_Memory_IO(buffer, MAX_HISTOGRAM_LENGTH *12, MAX_HISTOGRAM_LENGTH * 4, MOD_WRITE, k);
			}
			
			// Set RunTask
			value   = (unsigned int)run_task;
			Pixie_DSP_Memory_IO(&value, RunTask_Address[k], 1, MOD_WRITE, k);
			
			// Set ControlTask
			value   = (unsigned int)control_task;
			Pixie_DSP_Memory_IO(&value, ControlTask_Address[k], 1, MOD_WRITE, k);
			
			// Set RESUME
			value   = (unsigned int)mode;
			Pixie_DSP_Memory_IO(&value, Resume_Address[k], 1, MOD_WRITE, k);
		}
		
		// Set CSR to start run in all modules
		for(k = 0; k < Number_Modules; k ++)
		{
			// Read CSR
			Pixie_ReadCSR(k, &CSR);
			CSR = APP32_SetBit(RUNENA, CSR);	// Set bit RUNENA of CSR to enable run
			Pixie_WrtCSR(k, CSR);
		}
	}
	else if(ModNum < Number_Modules)  // Start run in one module only		
	{
		// Check if there is a run in progresss; if so, end it
		retval = Pixie_Check_Run_Status(ModNum);
		if(retval == 1)
		{
			retval = Pixie_End_Run(ModNum);
			// Check if Pixie_End_Run returned without errors
			if(retval < 0)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie_Start_Run): Pixie_End_Run failed in module %d; retval=%d", ModNum, retval);
				Pixie_Print_MSG(ErrMSG);
				return(-1);
			}
		}
		
		// Clear external memory first before starting a new data acquisition run
		if((mode == NEW_RUN) && (run_task != 0) && (control_task == 0))
		{
			// Clear histogram memory; channel by channel
			Pixie_Main_Memory_IO(buffer, MAX_HISTOGRAM_LENGTH * 0, MAX_HISTOGRAM_LENGTH * 4, MOD_WRITE, ModNum);
			Pixie_Main_Memory_IO(buffer, MAX_HISTOGRAM_LENGTH * 4, MAX_HISTOGRAM_LENGTH * 4, MOD_WRITE, ModNum);
			Pixie_Main_Memory_IO(buffer, MAX_HISTOGRAM_LENGTH * 8, MAX_HISTOGRAM_LENGTH * 4, MOD_WRITE, ModNum);
			Pixie_Main_Memory_IO(buffer, MAX_HISTOGRAM_LENGTH *12, MAX_HISTOGRAM_LENGTH * 4, MOD_WRITE, ModNum);
		}
		
		// Set RunTask
		value   = (unsigned int)run_task;
		Pixie_DSP_Memory_IO(&value, RunTask_Address[ModNum], 1, MOD_WRITE, ModNum);
		
		// Set ControlTask
		value   = (unsigned int)control_task;
		Pixie_DSP_Memory_IO(&value, ControlTask_Address[ModNum], 1, MOD_WRITE, ModNum);
		
		// Set RESUME
		value   = (unsigned int)mode;
		Pixie_DSP_Memory_IO(&value, Resume_Address[ModNum], 1, MOD_WRITE, ModNum);
		
		// Set CSR to start run in this module
		Pixie_ReadCSR(ModNum, &CSR);
		CSR = APP32_SetBit(RUNENA, CSR);	// Set bit RUNENA of CSR to enable run
		Pixie_WrtCSR(ModNum, CSR);
	}
	
	return(0);
}


/****************************************************************
*	Pixie_End_Run:
*		End run in a single Pixie module or multiple modules.
*
*		Return Value:
*			 0 - Success
*			-1 - Failed to stop the run
*
****************************************************************/

int Pixie_End_Run (
	unsigned short ModNum )
{
	unsigned int   CSR, tcount;
	unsigned short k, sumActive, Active[PRESET_MAX_MODULES];
	int retval;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	if(ModNum == Number_Modules)  // Stop run in all modules
	{
		for(k = 0; k < Number_Modules; k ++)
		{
			
			// Read CSR and clear bit RUNENA to stop the run
			Pixie_ReadCSR(k, &CSR);
			CSR = APP32_ClrBit(RUNENA, CSR);
			Pixie_WrtCSR(k, CSR);
			
			Active[k] = 1;  // To be used for checking run status below 
		}
		
		// Check if the run has been really ended
		tcount = 0;
		sumActive = Number_Modules;
		
		do
		{
			for(k = 0; k < Number_Modules; k ++)
			{
				if(Active[k] == 1)
				{
					retval = Pixie_Check_Run_Status(k);
					if( retval == 0)
					{
						Active[k] = 0;	// Clear Active for a module whose run is done
						sumActive --;
					}
				}
			}
			
			if(sumActive == 0)
			{
				break;		// Run in all modules is done
			}
			
#if PIXIE16_APPAPI_VER == PIXIE16_WINDOWS_APPAPI
			Sleep(1);
#elif PIXIE16_APPAPI_VER == PIXIE16_LINUX_APPAPI
			usleep(1000);
#endif
			
			tcount++;
			
		} while(tcount < 100); // TimeOut = 100 ms
		
		// Check if there is any module whose run has not ended
		for(k = 0; k < Number_Modules; k ++)
		{
			if(Active[k] == 1)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie_End_Run): module %d failed to stop its run", k);
				Pixie_Print_MSG(ErrMSG);
			}
		}
		
		if(sumActive != 0)
		{
			return(-1);		// Not all modules stopped the run successfully
		}
		else
		{
			return(0);		// All modules stopped the run successfully
		}
	}
	else if(ModNum < Number_Modules)  // Stop run in one module only
	{		
		// Read CSR and clear bit RUNENA to stop the run
		Pixie_ReadCSR(ModNum, &CSR);
		CSR = APP32_ClrBit(RUNENA, CSR);
		Pixie_WrtCSR(ModNum, CSR);
		
		// Check if the run has been really ended.
		tcount=0;
		do
		{
			retval = Pixie_Check_Run_Status(ModNum);
			if( retval == 0)
			{
				break;
			}
			
#if PIXIE16_APPAPI_VER == PIXIE16_WINDOWS_APPAPI
			Sleep(1);
#elif PIXIE16_APPAPI_VER == PIXIE16_LINUX_APPAPI
			usleep(1000);
#endif
			tcount++;
		} while(tcount < 100); // TimeOut = 100 ms
		
		if(tcount == 100)  // Timed out
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_End_Run): module %d failed to stop its run", ModNum);
			Pixie_Print_MSG(ErrMSG);
			return(-1);
		}
		else
		{
			return(0);		// The module stopped the run successfully
		}
	}
	
	return(0);
}


/****************************************************************
*	Pixie_Check_Run_Status:
*		Check if a run is in progress.
*
*		Return Value:
*			0:	no run in progress
*			1:	a run in progress
*
****************************************************************/

int Pixie_Check_Run_Status (
	unsigned short ModNum )
{	
	unsigned int CSR;
	
	Pixie_ReadCSR(ModNum, &CSR);
	return( (CSR & (0x1 << RUNACTIVE)) >> RUNACTIVE);
}


/****************************************************************
*	Pixie_Control_Task_Run:
*		Perform a control task run.
*
*		Return Value:
*			 0 - successful
*			-1 - failed to start the control task run
*			-2 - control task run timed out
*
****************************************************************/

int Pixie_Control_Task_Run (
	unsigned short ModNum,			// Pixie module number
	unsigned short ControlTask,		// Control task number
	unsigned int   Max_Poll )		// Timeout control in unit of ms for control task run
{
	unsigned int tcount;
	unsigned short k, sumActive, Active[PRESET_MAX_MODULES];
	int retval;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if running in OFFLINE mode
	if( Offline == 1 )	  // Returns immediately for offline analysis
	{
		return(0);
	}

	if(ModNum == Number_Modules)  // Start control task run in all modules
	{
		// Start control task run: NEW_RUN and RunTask = 0
		retval = Pixie_Start_Run(NEW_RUN, 0, ControlTask, Number_Modules);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Control_Task_Run): failed to start control task %d in all modules", ControlTask);
			Pixie_Print_MSG(ErrMSG);
			return(-1);
		}
		
		// A short wait before polling the run status
#if PIXIE16_APPAPI_VER == PIXIE16_WINDOWS_APPAPI
		Sleep(1);
#elif PIXIE16_APPAPI_VER == PIXIE16_LINUX_APPAPI
		usleep(1000);
#endif
		
		// Initialize counters and status indicators
		for(k = 0; k < Number_Modules; k ++)
		{
			Active[k] = 1;  // To be used for checking run status below 
		}
		tcount = 0;
		sumActive = Number_Modules;
		
		// Check if the run in each module has ended
		do
		{
			for(k = 0; k < Number_Modules; k ++)
			{
				if(Active[k] == 1)
				{
					retval = Pixie_Check_Run_Status(k);
					if( retval == 0)
					{
						Active[k] = 0;	// Clear Active for a module whose run is done
						sumActive --;
					}
				}
			}
			
			if(sumActive == 0)
			{
				break;		// Run in all modules is done
			}
			
#if PIXIE16_APPAPI_VER == PIXIE16_WINDOWS_APPAPI
			Sleep(1);
#elif PIXIE16_APPAPI_VER == PIXIE16_LINUX_APPAPI
			usleep(1000);
#endif
			tcount++;
			
		} while(tcount < Max_Poll); // TimeOut = Max_Poll ms
		
		// Check if there is any module whose run has not ended
		for(k = 0; k < Number_Modules; k ++)
		{
			if(Active[k] == 1)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie_Control_Task_Run): control task %d in module %d timed out", ControlTask, k);
				Pixie_Print_MSG(ErrMSG);
			}
		}
		
		if(sumActive != 0)
		{
			return(-1);		// Not all modules finished the control task in time
		}
		else
		{
			return(0);		// All modules finished the control task in time
		}
	}
	else if(ModNum < Number_Modules)  // Start control task run in one module only
	{		
		// Start control task run: NEW_RUN and RunTask = 0
		retval = Pixie_Start_Run(NEW_RUN, 0, ControlTask, ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Control_Task_Run): failed to start control task %d in module %d", ControlTask, ModNum);
			Pixie_Print_MSG(ErrMSG);
			return(-1);
		}
		
		// A short wait before polling the run status
#if PIXIE16_APPAPI_VER == PIXIE16_WINDOWS_APPAPI
		Sleep(1);
#elif PIXIE16_APPAPI_VER == PIXIE16_LINUX_APPAPI
		usleep(1000);
#endif
		
		// Check if the run in the module has ended
		tcount = 0;
		
		do
		{
#if PIXIE16_APPAPI_VER == PIXIE16_WINDOWS_APPAPI
			Sleep(1);
#elif PIXIE16_APPAPI_VER == PIXIE16_LINUX_APPAPI
			usleep(1000);
#endif
			retval = Pixie_Check_Run_Status(ModNum);
			tcount++;
			
		} while( (retval != 0) && (tcount < Max_Poll) ); // TimeOut = Max_Poll ms
		
		
		if(tcount >= Max_Poll)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Control_Task_Run): control task %d in module %d timed out", ControlTask, ModNum);
			Pixie_Print_MSG(ErrMSG);
			return(-2); // Time Out
		}
		else
		{
			return(0); // Normal finish
		}
	}
	
	return(0);
}


/****************************************************************
*	Pixie_Broadcast:
*		Broadcast certain global parameters to all Pixie modules.
*
*		Return Value:
*			 0 - success
*			-1 - invalid parameter to be broadcasted
*
****************************************************************/

int Pixie_Broadcast (
	char *str,                      // variable name whose value is to be broadcasted
	unsigned short SourceModule )   // the source module number
{
	unsigned int value;
	unsigned short i;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	if(strcmp(str,"SYNCH_WAIT")==0)
	{		
		// Get SynchWait
		value = Pixie_Devices[SourceModule].DSP_Parameter_Values[SynchWait_Address[SourceModule] - DATA_MEMORY_ADDRESS];
		
		// Update SynchWait in all modules
		for(i=0; i<Number_Modules; i++)
		{
			Pixie_Devices[i].DSP_Parameter_Values[SynchWait_Address[i] - DATA_MEMORY_ADDRESS] = value;
			
			// Download it to the module
			Pixie16IMbufferIO(&value, 1, SynchWait_Address[i], MOD_WRITE, i);
		}
	}
	else if(strcmp(str,"IN_SYNCH")==0)
	{
		// Get InSynch
		value = Pixie_Devices[SourceModule].DSP_Parameter_Values[InSynch_Address[SourceModule] - DATA_MEMORY_ADDRESS];
		
		// Update InSynch in all modules
		for(i=0; i<Number_Modules; i++)
		{
			Pixie_Devices[i].DSP_Parameter_Values[InSynch_Address[i] - DATA_MEMORY_ADDRESS] = value;
			
			// Download it to the module
			Pixie16IMbufferIO(&value, 1, InSynch_Address[i], MOD_WRITE, i);
		}
	}
	else if(strcmp(str,"HOST_RT_PRESET")==0)
	{
		// Get HostRunTimePreset
		value = Pixie_Devices[SourceModule].DSP_Parameter_Values[HRTP_Address[SourceModule] - DATA_MEMORY_ADDRESS];
		
		// Update InSynch in all modules
		for(i=0; i<Number_Modules; i++)
		{
			Pixie_Devices[i].DSP_Parameter_Values[HRTP_Address[i] - DATA_MEMORY_ADDRESS] = value;
			
			// Download it to the module
			Pixie16IMbufferIO(&value, 1, HRTP_Address[i], MOD_WRITE, i);
		}
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie_Broadcast): invalid global parameter %s", str);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	return(0);
}


/****************************************************************
*	Pixie_ComputeFIFO:
*		Update FIFO settings (TriggerDelay and PAFLength) when the
*		following DSP parameters are changed:
*			SlowLength, SlowGap, SlowFilterRange
*
*		Return Value:
*			0 - success
*
****************************************************************/

int Pixie_ComputeFIFO (
	unsigned int   TraceDelay,	// current trace dealy value
	unsigned short ModNum,		// Pixie module number
	unsigned short ChanNum )	// Pixie channel number
{
	
	unsigned int PAFLength, TriggerDelay, FifoLength;
	unsigned int SlowFilterRange, FastFilterRange, PeakSep;
	
	// Get the DSP parameter SlowFilterRange
	SlowFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
	FastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
	
	// Get the DSP parameter PeakSep
	PeakSep = Pixie_Devices[ModNum].DSP_Parameter_Values[PeakSep_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
	
	// Re-calculate TriggerDelay
	TriggerDelay = (unsigned int)(((double)PeakSep - 1.0) * pow(2.0, (double)SlowFilterRange));
	
	// Re-calculate PAFLength
	PAFLength = (unsigned int)((double)TriggerDelay / pow(2.0, (double)FastFilterRange))  + TraceDelay;
	
	// Get the current FIFO Length
	FifoLength = Pixie_Devices[ModNum].DSP_Parameter_Values[FIFOLength_Address[ModNum] - DATA_MEMORY_ADDRESS];
	
	if(PAFLength > FifoLength)  // PAFLength must be not larger than FifoLength
	{
		PAFLength = FifoLength - 1; // Keep TraceDelay while reducing TriggerDelay
		TriggerDelay = (PAFLength - TraceDelay) * (unsigned int)pow(2.0, (double)FastFilterRange);
	}
	
	// Update the DSP parameter TriggerDelay
	Pixie_Devices[ModNum].DSP_Parameter_Values[TriggerDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = TriggerDelay;
	// Download to the selected Pixie module
	Pixie16IMbufferIO(&TriggerDelay, 1, (unsigned int)(TriggerDelay_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
	
	// Update the DSP parameter PAFLength
	Pixie_Devices[ModNum].DSP_Parameter_Values[PAFlength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = PAFLength;
	// Download to the selected Pixie module
	Pixie16IMbufferIO(&PAFLength, 1, (unsigned int)(PAFlength_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
	
	return(0);
}


/****************************************************************
*	Decimal2IEEEFloating:
*		Convert a decimal fractional number to a IEEE 754 standrad
*		floating point number (1-bit sign, 8-bit exponent, and 23-bit
*		mantissa).
*
*		Return Value:
*			the converted IEEE floating point number
*
****************************************************************/

PIXIE16APP_EXPORT unsigned int PIXIE16APP_API Decimal2IEEEFloating(double DecimalNumber)
{
	unsigned int IEEEFloatingNum, IntPart, IntPartHex, FractPartHex;
	double AbsDecNum, FractPart;
	unsigned short k, m, exponent;
	short n;
	char signbit;
	
	// Check signbit
	if(DecimalNumber > 0)
	{
		signbit = 0;
	}
	else if(DecimalNumber < 0)
	{
		signbit = 1;
	}
	else	// DecimalNumber = 0
	{
		return(0);
	}
	
	// Get the absolute value
	AbsDecNum = fabs(DecimalNumber);
	
	// Get the integer part
	IntPart = (unsigned int)floor(AbsDecNum);
	
	// Convert the integer part
	IntPartHex = 0;
	k = 0;
	do
	{
		if((IntPart % 2) == 1)
		{
			IntPartHex = APP32_SetBit(k, IntPartHex);
		}
		else
		{
			IntPartHex = APP32_ClrBit(k, IntPartHex);
		}
		
		k ++;
		
		IntPart /= 2;
		
	} while((IntPart > 0) && (k < 32));
	
	// Get the fractional part
	FractPart = AbsDecNum - floor(AbsDecNum);
	
	// Convert the fractional part
	FractPartHex = 0;
	m = 0;
	do
	{
		if((FractPart * 2.0) >= 1.0)
		{
			FractPartHex = APP32_SetBit((unsigned short)(31 - m), FractPartHex);
			FractPart = FractPart * 2.0 - 1.0;
		}
		else
		{
			FractPartHex = APP32_ClrBit((unsigned short)(31 - m), FractPartHex);
			FractPart = FractPart * 2.0;
		}
		
		m ++;
		
	} while((FractPart > 0) && (m < 32));
	
	// Combine the integer and fractional part
	// First, find the exponent
	if(IntPartHex > 0)
	{
		exponent = 127 + k - 1;	// 127 is the exponent bias
		
		// Add sign bit first
		IEEEFloatingNum = signbit * (unsigned int)pow(2.0, 31.0);
		
		// Add the exponent
		IEEEFloatingNum += exponent * (unsigned int)pow(2.0, 23.0);
		
		// Add the mantissa
		IntPartHex = APP32_ClrBit((unsigned short)(k - 1), IntPartHex);
		IEEEFloatingNum += IntPartHex * (unsigned int)pow(2.0, (double)(22 - (k - 2)));
		IEEEFloatingNum += FractPartHex >> (32 - (23 - (k - 1)));
	}
	else	// IntPartHex = 0
	{
		// Search the fist non-zero bit in FractPartHex
		for(n = 31; n >= 0; n --)
		{
			if(APP32_TstBit(n, FractPartHex) == 1)
			{
				break;
			}
		}
		
		exponent = 127 - (32 - n);	// 127 is the exponent bias
		
		// Add sign bit first
		IEEEFloatingNum = signbit * (unsigned int)pow(2.0, 31.0);
		
		// Add the exponent
		IEEEFloatingNum += exponent * (unsigned int)pow(2.0, 23.0);
		
		// Add the mantissa
		FractPartHex = APP32_ClrBit(n, FractPartHex);
		if(n >= 23)
		{
			IEEEFloatingNum += FractPartHex >> (n - 23);
		}
		else
		{
			IEEEFloatingNum += FractPartHex << (23 - n);
		}
	}
	
	return(IEEEFloatingNum);
												
}


/****************************************************************
*	IEEEFloating2Decimal:
*		Convert a IEEE 754 standrad floating point number (1-bit
*		sign, 8-bit exponent, and 23-bit mantissa) to a decimal
*		fractional number.
*
*		Return Value:
*			the converted decimal fractional number
*
****************************************************************/

PIXIE16APP_EXPORT double PIXIE16APP_API IEEEFloating2Decimal(unsigned int IEEEFloatingNumber)
{
	short signbit, exponent;
	double mantissa, DecimalNumber;
	
	signbit = (short)(IEEEFloatingNumber >> 31);
	exponent = (short)((IEEEFloatingNumber & 0x7F800000) >> 23) - 127;
	mantissa = 1.0 + (double)(IEEEFloatingNumber & 0x7FFFFF) / pow(2.0, 23.0);
	if(signbit == 0)
	{
		DecimalNumber = mantissa * pow(2.0, (double)exponent);
	}
	else
	{
		DecimalNumber = - mantissa * pow(2.0, (double)exponent);
	}
	
	return(DecimalNumber);
	
}


/****************************************************************
*	Pixie_Init_DSPVarAddress:
*		Receive DSP I/O parameter name addressee from the DSP .var file.
*
*		Return Value:
*			 0 - load successful
*			-1 - premature EOF encountered
*			-2 - DSP variable name not found
*			-3 - DSP .var file not found
*
****************************************************************/

int Pixie_Init_DSPVarAddress(char *DSPVarFile, unsigned short ModNum)
{
	char DSP_Parameter_Names[N_DSP_PAR][MAX_PAR_NAME_LENGTH];
	char *DSP_Parameter_AddrStr;
	unsigned int DSP_Parameter_Addr[N_DSP_PAR];
	char DSPParaName[MAX_PAR_NAME_LENGTH], str[MAX_PAR_NAME_LENGTH], ErrMSG[MAX_ERRMSG_LENGTH];
	unsigned short k;
	FILE* namesFile = NULL;
	
	// Open DSP .var file
	namesFile = fopen(DSPVarFile, "r");
	if(namesFile != NULL)
	{
		// Clear all names
		for(k=0; k<N_DSP_PAR; k++)
		{
			strcpy(DSP_Parameter_Names[k],"");
		}
		
		// DSP_Parameter_AddrStr is 11 characters long: 0x + 8 hex characters + NULL terminator
		DSP_Parameter_AddrStr = (char *)malloc(sizeof(char) * 10 + 1);
		
		// Read names
		for(k=0; k<N_DSP_PAR; k++)
		{
			if( fgets(DSPParaName, MAX_PAR_NAME_LENGTH, namesFile) != NULL )  // Read one line
			{
				sscanf(DSPParaName, "%s %s", DSP_Parameter_AddrStr, DSP_Parameter_Names[k]);
				DSP_Parameter_Addr[k] = strtol(DSP_Parameter_AddrStr, NULL, 0);
			}
			else  // Reached end of file
			{
				break;
			}
		}
		
		// Release memory
		free(DSP_Parameter_AddrStr);
		
		// Initialize addressee
		
		//--------------------
		//	Module parameters
		//--------------------
		
		ModNum_Address[ModNum] = 0;
		ModCSRA_Address[ModNum] = 0;
		ModCSRB_Address[ModNum] = 0;
		ModFormat_Address[ModNum] = 0;
		RunTask_Address[ModNum] = 0;
		ControlTask_Address[ModNum] = 0;
		MaxEvents_Address[ModNum] = 0;
		CoincPattern_Address[ModNum] = 0;
		CoincWait_Address[ModNum] = 0;
		SynchWait_Address[ModNum] = 0;
		InSynch_Address[ModNum] = 0;
		Resume_Address[ModNum] = 0;
		SlowFilterRange_Address[ModNum] = 0;
		FastFilterRange_Address[ModNum] = 0;
		ChanNum_Address[ModNum] = 0;
		HostIO_Address[ModNum] = 0;
		UserIn_Address[ModNum] = 0;
		U00_Address[ModNum] = 0;
		FastTrigBackplaneEna_Address[ModNum] = 0;
		CrateID_Address             [ModNum] = 0;
		SlotID_Address              [ModNum] = 0;
		ModID_Address               [ModNum] = 0;
		TrigConfig_Address          [ModNum] = 0;
		HRTP_Address[ModNum] = 0;
		
		//--------------------
		//	Channel parameters
		//--------------------
		
		ChanCSRa_Address[ModNum] = 0;
		ChanCSRb_Address[ModNum] = 0;
		GainDAC_Address[ModNum] = 0;
		OffsetDAC_Address[ModNum] = 0;
		DigGain_Address[ModNum] = 0;
		SlowLength_Address[ModNum] = 0;
		SlowGap_Address[ModNum] = 0;
		FastLength_Address[ModNum] = 0;
		FastGap_Address[ModNum] = 0;
		PeakSample_Address[ModNum] = 0;
		PeakSep_Address[ModNum] = 0;
		CFDThresh_Address[ModNum] = 0;
		FastThresh_Address[ModNum] = 0;
		ThreshWidth_Address[ModNum] = 0;
		PAFlength_Address[ModNum] = 0;
		TriggerDelay_Address[ModNum] = 0;
		ResetDelay_Address[ModNum] = 0;
		ChanTrigStretch_Address[ModNum] = 0;
		TraceLength_Address[ModNum] = 0;
		TrigOutLen_Address[ModNum] = 0;
		EnergyLow_Address[ModNum] = 0;
		Log2Ebin_Address[ModNum] = 0;
		Log2Bweight_Address[ModNum] = 0;
		MultiplicityMaskL_Address[ModNum] = 0;
		PSAoffset_Address[ModNum] = 0;
		PSAlength_Address[ModNum] = 0;
		Integrator_Address[ModNum] = 0;
		BLcut_Address[ModNum] = 0;
		Integrator_Address[ModNum] = 0;
		BaselinePercent_Address[ModNum] = 0;
		FtrigoutDelay_Address[ModNum] = 0;
		Log2Bweight_Address[ModNum] = 0;
		PreampTau_Address[ModNum] = 0;
		MultiplicityMaskH_Address[ModNum] = 0;
		FastTrigBackLen_Address[ModNum] = 0;
		Xwait_Address[ModNum] = 0;
		CFDDelay_Address            [ModNum] = 0;
		CFDScale_Address            [ModNum] = 0;
		ExternDelayLen_Address      [ModNum] = 0;
		ExtTrigStretch_Address      [ModNum] = 0;
		VetoStretch_Address         [ModNum] = 0;
		QDCLen0_Address             [ModNum] = 0;
		QDCLen1_Address             [ModNum] = 0;
		QDCLen2_Address             [ModNum] = 0;
		QDCLen3_Address             [ModNum] = 0;
		QDCLen4_Address             [ModNum] = 0;
		QDCLen5_Address             [ModNum] = 0;
		QDCLen6_Address             [ModNum] = 0;
		QDCLen7_Address             [ModNum] = 0;

		//--------------------
		//	Results parameters
		//--------------------
		
		RealTimeA_Address[ModNum] = 0;
		RealTimeB_Address[ModNum] = 0;
		RunTimeA_Address[ModNum] = 0;
		RunTimeB_Address[ModNum] = 0;
		GSLTtime_Address[ModNum] = 0;
		NumEventsA_Address[ModNum] = 0;
		NumEventsB_Address[ModNum] = 0;
		DSPerror_Address[ModNum] = 0;
		SynchDone_Address[ModNum] = 0;
		BufHeadLen_Address[ModNum] = 0;
		EventHeadLen_Address[ModNum] = 0;
		ChanHeadLen_Address[ModNum] = 0;
		UserOut_Address[ModNum] = 0;
		AOutBuffer_Address[ModNum] = 0;
		LOutBuffer_Address[ModNum] = 0;
		AECorr_Address[ModNum] = 0;
		LECorr_Address[ModNum] = 0;
		HardwareID_Address[ModNum] = 0;
		HardVariant_Address[ModNum] = 0;
		FIFOLength_Address[ModNum] = 0;
		FippiID_Address[ModNum] = 0;
		FippiVariant_Address[ModNum] = 0;
		DSPrelease_Address[ModNum] = 0;
		DSPbuild_Address[ModNum] = 0;
		DSPVariant_Address[ModNum] = 0;
		U20_Address[ModNum] = 0;
		LiveTimeA_Address[ModNum] = 0;
		LiveTimeB_Address[ModNum] = 0;
		FastPeaksA_Address[ModNum] = 0;
		FastPeaksB_Address[ModNum] = 0;
		OverflowA_Address[ModNum] = 0;
		OverflowB_Address[ModNum] = 0;
		InSpecA_Address[ModNum] = 0;
		InSpecB_Address[ModNum] = 0;
		UnderflowA_Address[ModNum] = 0;
		UnderflowB_Address[ModNum] = 0;
		ChanEventsA_Address[ModNum] = 0;
		ChanEventsB_Address[ModNum] = 0;
		AutoTau_Address[ModNum] = 0;
		U30_Address[ModNum] = 0;
		
		// Find Address
		for(k=0; k<N_DSP_PAR; k++)
		{
			strcpy(str, DSP_Parameter_Names[k]);

			//--------------------
			//	Module parameters
			//--------------------
			
			if(strcmp(str, "ModNum") == 0)
				ModNum_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "ModCSRA") == 0)
				ModCSRA_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "ModCSRB") == 0)
				ModCSRB_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "ModFormat") == 0)
				ModFormat_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "RunTask") == 0)
				RunTask_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "ControlTask") == 0)
				ControlTask_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "MaxEvents") == 0)
				MaxEvents_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "CoincPattern") == 0)
				CoincPattern_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "CoincWait") == 0)
				CoincWait_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "SynchWait") == 0)
				SynchWait_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "InSynch") == 0)
				InSynch_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "Resume") == 0)
				Resume_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "SlowFilterRange") == 0)
				SlowFilterRange_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "FastFilterRange") == 0)
				FastFilterRange_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "ChanNum") == 0)
				ChanNum_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "HostIO") == 0)
				HostIO_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "UserIn") == 0)
				UserIn_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "U00") == 0)
				U00_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "FastTrigBackplaneEna") == 0)
				FastTrigBackplaneEna_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "CrateID") == 0)
				CrateID_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "SlotID") == 0)
				SlotID_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "ModID") == 0)
				ModID_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "TrigConfig") == 0)
				TrigConfig_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "HostRunTimePreset") == 0)
				HRTP_Address[ModNum] = DSP_Parameter_Addr[k];

			//--------------------
			//	Channel parameters
			//--------------------
			
			else if(strcmp(str, "ChanCSRa") == 0)
				ChanCSRa_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "ChanCSRb") == 0)
				ChanCSRb_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "GainDAC") == 0)
				GainDAC_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "OffsetDAC") == 0)
				OffsetDAC_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "DigGain") == 0)
				DigGain_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "SlowLength") == 0)
				SlowLength_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "SlowGap") == 0)
				SlowGap_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "FastLength") == 0)
				FastLength_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "FastGap") == 0)
				FastGap_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "PeakSample") == 0)
				PeakSample_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "PeakSep") == 0)
				PeakSep_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "CFDThresh") == 0)
				CFDThresh_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "FastThresh") == 0)
				FastThresh_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "ThreshWidth") == 0)
				ThreshWidth_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "PAFlength") == 0)
				PAFlength_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "TriggerDelay") == 0)
				TriggerDelay_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "ResetDelay") == 0)
				ResetDelay_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "ChanTrigStretch") == 0)
				ChanTrigStretch_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "TraceLength") == 0)
				TraceLength_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "TrigOutLen") == 0)
				TrigOutLen_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "EnergyLow") == 0)
				EnergyLow_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "Log2Ebin") == 0)
				Log2Ebin_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "Log2Bweight") == 0)
				Log2Bweight_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "MultiplicityMaskL") == 0)
				MultiplicityMaskL_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "PSAoffset") == 0)
				PSAoffset_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "PSAlength") == 0)
				PSAlength_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "Integrator") == 0)
				Integrator_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "BLcut") == 0)
				BLcut_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "Integrator") == 0)
				Integrator_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "BaselinePercent") == 0)
				BaselinePercent_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "FtrigoutDelay") == 0)
				FtrigoutDelay_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "Log2Bweight") == 0)
				Log2Bweight_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "PreampTau") == 0)
				PreampTau_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "MultiplicityMaskH") == 0)
				MultiplicityMaskH_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "FastTrigBackLen") == 0)
				FastTrigBackLen_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "Xwait") == 0)
				Xwait_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "CFDDelay") == 0)
				CFDDelay_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "CFDScale") == 0)
				CFDScale_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "ExternDelayLen") == 0)
				ExternDelayLen_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "ExtTrigStretch") == 0)
				ExtTrigStretch_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "VetoStretch") == 0)
				VetoStretch_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "QDCLen0") == 0)
				QDCLen0_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "QDCLen1") == 0)
				QDCLen1_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "QDCLen2") == 0)
				QDCLen2_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "QDCLen3") == 0)
				QDCLen3_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "QDCLen4") == 0)
				QDCLen4_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "QDCLen5") == 0)
				QDCLen5_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "QDCLen6") == 0)
				QDCLen6_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "QDCLen7") == 0)
				QDCLen7_Address[ModNum] = DSP_Parameter_Addr[k];
			
			//--------------------
			//	Results parameters
			//--------------------
			
			else if(strcmp(str, "RealTimeA") == 0)
				RealTimeA_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "RealTimeB") == 0)
				RealTimeB_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "RunTimeA") == 0)
				RunTimeA_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "RunTimeB") == 0)
				RunTimeB_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "GSLTtime") == 0)
				GSLTtime_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "NumEventsA") == 0)
				NumEventsA_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "NumEventsB") == 0)
				NumEventsB_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "DSPerror") == 0)
				DSPerror_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "SynchDone") == 0)
				SynchDone_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "BufHeadLen") == 0)
				BufHeadLen_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "EventHeadLen") == 0)
				EventHeadLen_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "ChanHeadLen") == 0)
				ChanHeadLen_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "UserOut") == 0)
				UserOut_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "AOutBuffer") == 0)
				AOutBuffer_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "LOutBuffer") == 0)
				LOutBuffer_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "AECorr") == 0)
				AECorr_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "LECorr") == 0)
				LECorr_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "HardwareID") == 0)
				HardwareID_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "HardVariant") == 0)
				HardVariant_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "FIFOLength") == 0)
				FIFOLength_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "FippiID") == 0)
				FippiID_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "FippiVariant") == 0)
				FippiVariant_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "DSPrelease") == 0)
				DSPrelease_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "DSPbuild") == 0)
				DSPbuild_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "DSPVariant") == 0)
				DSPVariant_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "U20") == 0)
				U20_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "LiveTimeA") == 0)
				LiveTimeA_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "LiveTimeB") == 0)
				LiveTimeB_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "FastPeaksA") == 0)
				FastPeaksA_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "FastPeaksB") == 0)
				FastPeaksB_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "OverflowA") == 0)
				OverflowA_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "OverflowB") == 0)
				OverflowB_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "InSpecA") == 0)
				InSpecA_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "InSpecB") == 0)
				InSpecB_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "UnderflowA") == 0)
				UnderflowA_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "UnderflowB") == 0)
				UnderflowB_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "ChanEventsA") == 0)
				ChanEventsA_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "ChanEventsB") == 0)
				ChanEventsB_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "AutoTau") == 0)
				AutoTau_Address[ModNum] = DSP_Parameter_Addr[k];
			else if(strcmp(str, "U30") == 0)
				U30_Address[ModNum] = DSP_Parameter_Addr[k];
		}
		
		// Check if all the DSP variable names have been found in the file
		
		//--------------------
		//	Module parameters
		//--------------------
		
		if(ModNum_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ModNum was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(ModCSRA_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ModCSRA was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(ModCSRB_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ModCSRB was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(ModFormat_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ModFormat was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(RunTask_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): RunTask was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(ControlTask_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ControlTask was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(MaxEvents_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): MaxEvents was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(CoincPattern_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): CoincPattern was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(CoincWait_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): CoincWait was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(SynchWait_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): SynchWait was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(InSynch_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): InSynch was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(Resume_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): Resume was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(SlowFilterRange_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): SlowFilterRange was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(FastFilterRange_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): FastFilterRange was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(ChanNum_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ChanNum was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(HostIO_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): HostIO was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(UserIn_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): UserIn was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(U00_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): U00 was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}	
		if(FastTrigBackplaneEna_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): FastTrigBackplaneEna was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(CrateID_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): CrateID was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(SlotID_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): SlotID was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(ModID_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ModID was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(TrigConfig_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): TrigConfig was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(HRTP_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): HostRunTimePreset was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		
		//--------------------
		//	Channel parameters
		//--------------------
		
		if(ChanCSRa_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ChanCSRa was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(ChanCSRb_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ChanCSRb was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(GainDAC_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): GainDAC was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(OffsetDAC_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): OffsetDAC was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(DigGain_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): DigGain was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(SlowLength_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): SlowLength was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(SlowGap_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): SlowGap was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(FastLength_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): FastLength was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(FastGap_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): FastGap was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(PeakSample_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): PeakSample was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(PeakSep_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): PeakSep was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(CFDThresh_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): CFDThresh was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(FastThresh_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): FastThresh was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(ThreshWidth_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ThreshWidth was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(PAFlength_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): PAFlength was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(TriggerDelay_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): TriggerDelay was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(ResetDelay_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ResetDelay was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(ChanTrigStretch_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ChanTrigStretch was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(TraceLength_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): TraceLength was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(TrigOutLen_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): TrigOutLen was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(EnergyLow_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): EnergyLow was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(Log2Ebin_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): Log2Ebin was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(Log2Bweight_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): Log2Bweight was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(EnergyLow_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): EnergyLow was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(MultiplicityMaskL_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): MultiplicityMaskL was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(PSAoffset_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): PSAoffset was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(PSAlength_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): PSAlength was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(Integrator_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): Integrator was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(BLcut_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): BLcut was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(Integrator_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): Integrator was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(BaselinePercent_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): BaselinePercent was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(FtrigoutDelay_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): FtrigoutDelay was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(Log2Bweight_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): Log2Bweight was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(PreampTau_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): PreampTau was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(MultiplicityMaskH_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): MultiplicityMaskH was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(FastTrigBackLen_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): FastTrigBackLen was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(Xwait_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): Xait was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(CFDDelay_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): CFDDelay was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(CFDScale_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): CFDScale was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(ExternDelayLen_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ExternDelayLen was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(ExtTrigStretch_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ExtTrigStretch was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(VetoStretch_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): VetoStretch was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(QDCLen0_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): QDCLen0 was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(QDCLen1_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): QDCLen1 was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(QDCLen2_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): QDCLen2 was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(QDCLen3_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): QDCLen3 was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(QDCLen4_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): QDCLen4 was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(QDCLen5_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): QDCLen5 was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(QDCLen6_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): QDCLen6 was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(QDCLen7_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): QDCLen7 was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		
		//--------------------
		//	Results parameters
		//--------------------
		
		
		if(RealTimeA_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): RealTimeA was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(RealTimeB_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): RealTimeB was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(RunTimeA_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): RunTimeA was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(RunTimeB_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): RunTimeB was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(GSLTtime_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): GSLTtime was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(NumEventsA_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): NumEventsA was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(NumEventsB_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): NumEventsB was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(DSPerror_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): DSPerror was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(NumEventsB_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): NumEventsB was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(SynchDone_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): SynchDone was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(BufHeadLen_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): BufHeadLen was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(EventHeadLen_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): EventHeadLen was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(ChanHeadLen_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ChanHeadLen was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(UserOut_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): UserOut was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(AOutBuffer_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): AOutBuffer was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(UserOut_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): UserOut was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(LOutBuffer_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): LOutBuffer was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(AECorr_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): AECorr was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(LECorr_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): LECorr was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(HardwareID_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): HardwareID was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(HardVariant_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): HardVariant was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(FIFOLength_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): FIFOLength was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(FippiID_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): FippiID was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(FippiVariant_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): FippiVariant was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(FippiID_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): FippiID was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(DSPrelease_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): DSPrelease was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(DSPbuild_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): DSPbuild was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(DSPVariant_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): DSPVariant was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(U20_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): U20 was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(LiveTimeA_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): LiveTimeA was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(LiveTimeB_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): LiveTimeB was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(FastPeaksA_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): FastPeaksA was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(FastPeaksB_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): FastPeaksB was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(OverflowA_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): OverflowA was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(OverflowB_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): OverflowB was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(InSpecA_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): InSpecA was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(InSpecB_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): InSpecB was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(UnderflowA_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): UnderflowA was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(UnderflowB_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): UnderflowB was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(ChanEventsA_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ChanEventsA was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(ChanEventsB_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): ChanEventsB was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(AutoTau_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): AutoTau was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		if(U30_Address[ModNum] == 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): U30 was not found in the DSP .var file %s", DSPVarFile);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie_Init_DSPVarAddress): DSP .var file %s can't be opened", DSPVarFile);
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}
	
	return(0);
}


/****************************************************************
*	Pixie_Copy_DSPVarAddress:
*		Copy DSP variable address between modules.
*
*		Return Value:
*			 0 - success
*			-1 - Invalid source module number
*			-2 - Invalid destination module number
*
****************************************************************/

int Pixie_Copy_DSPVarAddress(unsigned short SourceModNum, unsigned short DestinationModNum)
{
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if SourceModNum is valid
	if( SourceModNum >= Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie_Copy_DSPVarAddress): invalid source module number %d", SourceModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	// Check if DestinationModNum is valid
	if( DestinationModNum >= Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie_Copy_DSPVarAddress): invalid destination module number %d", DestinationModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	// Copy Address
	
	//--------------------
	//	Module parameters
	//--------------------
	ModNum_Address[DestinationModNum] = ModNum_Address[SourceModNum];
	ModCSRA_Address[DestinationModNum] = ModCSRA_Address[SourceModNum];
	ModCSRB_Address[DestinationModNum] = ModCSRB_Address[SourceModNum];
	ModFormat_Address[DestinationModNum] = ModFormat_Address[SourceModNum];
	RunTask_Address[DestinationModNum] = RunTask_Address[SourceModNum];
	ControlTask_Address[DestinationModNum] = ControlTask_Address[SourceModNum];
	MaxEvents_Address[DestinationModNum] = MaxEvents_Address[SourceModNum];
	CoincPattern_Address[DestinationModNum] = CoincPattern_Address[SourceModNum];
	CoincWait_Address[DestinationModNum] = CoincWait_Address[SourceModNum];
	SynchWait_Address[DestinationModNum] = SynchWait_Address[SourceModNum];
	InSynch_Address[DestinationModNum] = InSynch_Address[SourceModNum];
	Resume_Address[DestinationModNum] = Resume_Address[SourceModNum];
	SlowFilterRange_Address[DestinationModNum] = SlowFilterRange_Address[SourceModNum];
	FastFilterRange_Address[DestinationModNum] = FastFilterRange_Address[SourceModNum];
	ChanNum_Address[DestinationModNum] = ChanNum_Address[SourceModNum];
	HostIO_Address[DestinationModNum] = HostIO_Address[SourceModNum];
	UserIn_Address[DestinationModNum] = UserIn_Address[SourceModNum];
	U00_Address[DestinationModNum] = U00_Address[SourceModNum];
	FastTrigBackplaneEna_Address[DestinationModNum] = FastTrigBackplaneEna_Address[SourceModNum];
	CrateID_Address             [DestinationModNum] = CrateID_Address             [SourceModNum];
	SlotID_Address              [DestinationModNum] = SlotID_Address              [SourceModNum];
	ModID_Address               [DestinationModNum] = ModID_Address               [SourceModNum];
	TrigConfig_Address          [DestinationModNum] = TrigConfig_Address          [SourceModNum];
	HRTP_Address[DestinationModNum] = HRTP_Address[SourceModNum];

	//--------------------
	//	Channel parameters
	//--------------------
	
	ChanCSRa_Address[DestinationModNum] = ChanCSRa_Address[SourceModNum];
	ChanCSRb_Address[DestinationModNum] = ChanCSRb_Address[SourceModNum];
	GainDAC_Address[DestinationModNum] = GainDAC_Address[SourceModNum];
	OffsetDAC_Address[DestinationModNum] = OffsetDAC_Address[SourceModNum];
	DigGain_Address[DestinationModNum] = DigGain_Address[SourceModNum];
	SlowLength_Address[DestinationModNum] = SlowLength_Address[SourceModNum];
	SlowGap_Address[DestinationModNum] = SlowGap_Address[SourceModNum];
	FastLength_Address[DestinationModNum] = FastLength_Address[SourceModNum];
	FastGap_Address[DestinationModNum] = FastGap_Address[SourceModNum];
	PeakSample_Address[DestinationModNum] = PeakSample_Address[SourceModNum];
	PeakSep_Address[DestinationModNum] = PeakSep_Address[SourceModNum];
	CFDThresh_Address[DestinationModNum] = CFDThresh_Address[SourceModNum];
	FastThresh_Address[DestinationModNum] = FastThresh_Address[SourceModNum];
	ThreshWidth_Address[DestinationModNum] = ThreshWidth_Address[SourceModNum];
	PAFlength_Address[DestinationModNum] = PAFlength_Address[SourceModNum];
	TriggerDelay_Address[DestinationModNum] = TriggerDelay_Address[SourceModNum];
	ResetDelay_Address[DestinationModNum] = ResetDelay_Address[SourceModNum];
	ChanTrigStretch_Address[DestinationModNum] = ChanTrigStretch_Address[SourceModNum];
	TraceLength_Address[DestinationModNum] = TraceLength_Address[SourceModNum];
	TrigOutLen_Address[DestinationModNum] = TrigOutLen_Address[SourceModNum];
	EnergyLow_Address[DestinationModNum] = EnergyLow_Address[SourceModNum];
	Log2Ebin_Address[DestinationModNum] = Log2Ebin_Address[SourceModNum];
	Log2Bweight_Address[DestinationModNum] = Log2Bweight_Address[SourceModNum];

	MultiplicityMaskL_Address[DestinationModNum] = MultiplicityMaskL_Address[SourceModNum];

	PSAoffset_Address[DestinationModNum] = PSAoffset_Address[SourceModNum];
	PSAlength_Address[DestinationModNum] = PSAlength_Address[SourceModNum];
	Integrator_Address[DestinationModNum] = Integrator_Address[SourceModNum];

	BLcut_Address[DestinationModNum] = BLcut_Address[SourceModNum];
	BaselinePercent_Address[DestinationModNum] = BaselinePercent_Address[SourceModNum];
	Integrator_Address[DestinationModNum] = Integrator_Address[SourceModNum];

	FtrigoutDelay_Address[DestinationModNum] = FtrigoutDelay_Address[SourceModNum];

	Log2Bweight_Address[DestinationModNum] = Log2Bweight_Address[SourceModNum];
	PreampTau_Address[DestinationModNum] = PreampTau_Address[SourceModNum];

	MultiplicityMaskH_Address[DestinationModNum] = MultiplicityMaskH_Address[SourceModNum];

	FastTrigBackLen_Address[DestinationModNum] = FastTrigBackLen_Address[SourceModNum];
	
	Xwait_Address[DestinationModNum] = Xwait_Address[SourceModNum];

	CFDDelay_Address            [DestinationModNum] = CFDDelay_Address            [SourceModNum];
	CFDScale_Address            [DestinationModNum] = CFDScale_Address            [SourceModNum];
	ExternDelayLen_Address      [DestinationModNum] = ExternDelayLen_Address      [SourceModNum];
	ExtTrigStretch_Address      [DestinationModNum] = ExtTrigStretch_Address      [SourceModNum];
	VetoStretch_Address         [DestinationModNum] = VetoStretch_Address         [SourceModNum];
	QDCLen0_Address             [DestinationModNum] = QDCLen0_Address             [SourceModNum];
	QDCLen1_Address             [DestinationModNum] = QDCLen1_Address             [SourceModNum];
	QDCLen2_Address             [DestinationModNum] = QDCLen2_Address             [SourceModNum];
	QDCLen3_Address             [DestinationModNum] = QDCLen3_Address             [SourceModNum];
	QDCLen4_Address             [DestinationModNum] = QDCLen4_Address             [SourceModNum];
	QDCLen5_Address             [DestinationModNum] = QDCLen5_Address             [SourceModNum];
	QDCLen6_Address             [DestinationModNum] = QDCLen6_Address             [SourceModNum];
	QDCLen7_Address             [DestinationModNum] = QDCLen7_Address             [SourceModNum];
	
	//--------------------
	//	Results parameters
	//--------------------
	
	RealTimeA_Address[DestinationModNum] = RealTimeA_Address[SourceModNum];
	RealTimeB_Address[DestinationModNum] = RealTimeB_Address[SourceModNum];
	RunTimeA_Address[DestinationModNum] = RunTimeA_Address[SourceModNum];
	RunTimeB_Address[DestinationModNum] = RunTimeB_Address[SourceModNum];
	GSLTtime_Address[DestinationModNum] = GSLTtime_Address[SourceModNum];
	NumEventsA_Address[DestinationModNum] = NumEventsA_Address[SourceModNum];
	NumEventsB_Address[DestinationModNum] = NumEventsB_Address[SourceModNum];
	DSPerror_Address[DestinationModNum] = DSPerror_Address[SourceModNum];
	SynchDone_Address[DestinationModNum] = SynchDone_Address[SourceModNum];
	BufHeadLen_Address[DestinationModNum] = BufHeadLen_Address[SourceModNum];
	EventHeadLen_Address[DestinationModNum] = EventHeadLen_Address[SourceModNum];
	ChanHeadLen_Address[DestinationModNum] = ChanHeadLen_Address[SourceModNum];
	UserOut_Address[DestinationModNum] = UserOut_Address[SourceModNum];
	AOutBuffer_Address[DestinationModNum] = AOutBuffer_Address[SourceModNum];
	LOutBuffer_Address[DestinationModNum] = LOutBuffer_Address[SourceModNum];
	AECorr_Address[DestinationModNum] = AECorr_Address[SourceModNum];
	LECorr_Address[DestinationModNum] = LECorr_Address[SourceModNum];
	HardwareID_Address[DestinationModNum] = HardwareID_Address[SourceModNum];
	HardVariant_Address[DestinationModNum] = HardVariant_Address[SourceModNum];
	FIFOLength_Address[DestinationModNum] = FIFOLength_Address[SourceModNum];
	FippiID_Address[DestinationModNum] = FippiID_Address[SourceModNum];
	FippiVariant_Address[DestinationModNum] = FippiVariant_Address[SourceModNum];
	DSPrelease_Address[DestinationModNum] = DSPrelease_Address[SourceModNum];
	DSPbuild_Address[DestinationModNum] = DSPbuild_Address[SourceModNum];
	DSPVariant_Address[DestinationModNum] = DSPVariant_Address[SourceModNum];
	U20_Address[DestinationModNum] = U20_Address[SourceModNum];
	LiveTimeA_Address[DestinationModNum] = LiveTimeA_Address[SourceModNum];
	LiveTimeB_Address[DestinationModNum] = LiveTimeB_Address[SourceModNum];
	FastPeaksA_Address[DestinationModNum] = FastPeaksA_Address[SourceModNum];
	FastPeaksB_Address[DestinationModNum] = FastPeaksB_Address[SourceModNum];
	OverflowA_Address[DestinationModNum] = OverflowA_Address[SourceModNum];
	OverflowB_Address[DestinationModNum] = OverflowB_Address[SourceModNum];
	InSpecA_Address[DestinationModNum] = InSpecA_Address[SourceModNum];
	InSpecB_Address[DestinationModNum] = InSpecB_Address[SourceModNum];
	UnderflowA_Address[DestinationModNum] = UnderflowA_Address[SourceModNum];
	UnderflowB_Address[DestinationModNum] = UnderflowB_Address[SourceModNum];
	ChanEventsA_Address[DestinationModNum] = ChanEventsA_Address[SourceModNum];
	ChanEventsB_Address[DestinationModNum] = ChanEventsB_Address[SourceModNum];
	AutoTau_Address[DestinationModNum] = AutoTau_Address[SourceModNum];
	U30_Address[DestinationModNum] = U30_Address[SourceModNum];
	
	return(0);
}


/****************************************************************
*	Pixie16ReadMSGFile (exclusively for VB GUI):
*		Read Pixie-16 message file, but in reverse line-order,
*		and the maximum number of characters to read is 65535.
*
*		Return Value:
*			 0 - Success
*			-1 - Null pointer *ReturnMsgStr
*			-2 - Pixie-16 message file Pixie16msg.txt is empty
*			-3 - Failed to allocate memory
*			-4 - Inconsistent bytes read from the message file
*			-5 - Could not seek to end of Pixie16msg.txt
*			-6 - Could not open Pixie16msg.txt
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadMSGFile (
	char *ReturnMsgStr )
{
	int  n, len, firstline, totalchars;
	FILE *msgfil;
	char *data, *b, *e, *end;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if ReturnMsgStr is valid
	if(ReturnMsgStr == NULL)
	{
		sprintf(ErrMSG, "*Error* (Pixie16ReadMSGFile): Null pointer *ReturnMsgStr");
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	msgfil = fopen ("Pixie16msg.txt","rb");
	if(msgfil != NULL)
	{
		if(fseek(msgfil, 0, SEEK_END) == 0)
		{
			len = ftell(msgfil);
			rewind(msgfil);
			if(len == 0)
			{
				fclose(msgfil);
				sprintf(ErrMSG, "*ERROR* (Pixie16ReadMSGFile): Pixie-16 message file Pixie16msg.txt is empty");
				Pixie_Print_MSG(ErrMSG);
				return(-2);
			}
			
			// Allocate memory
			if( (data = (char *)malloc(sizeof(char) * (len+1))) == NULL)
			{
				fclose(msgfil);
				sprintf(ErrMSG, "*ERROR* (Pixie16ReadMSGFile): failed to allocate memory");
				Pixie_Print_MSG(ErrMSG);
				return(-3);
			}
			
			// Read all bytes
			if((n = fread(data, 1, len, msgfil)) < len)
			{
				fclose(msgfil);
				sprintf(ErrMSG, "*ERROR* (Pixie16ReadMSGFile): expected %d, got %d bytes from Pixie16msg.txt", len, n);
				Pixie_Print_MSG(ErrMSG);
				return(-4);
			}
			
			// Delimit the lines and write them out in reverse order
			end = data + n;
			b = end;
			e = end;
			*end = 0;
			firstline = 0;
			totalchars = 0;
			while ((e > data) && (totalchars < 65535))
			{
				// Find the beginning of the current line
				while (b > data)
				{
					if (*b == '\n')
					{
						b++;
						break;
					}
					
					b --;
				}
				
				// Write out the line, including the EOL character(s)
				if(firstline == 0)
				{
					totalchars += (e - b + 1);
					strncpy(ReturnMsgStr, b, e - b + 1);
					firstline = 1;
				}
				else
				{
					totalchars += (e - b);
					if(totalchars < 65535)
					{
						strncat(ReturnMsgStr, b, e - b);
					}
					
				}
				
				// Point e at the end of the next line
				e = b;
				b --;
				
				if ((b > data) && (*b == '\n'))
				{
					b = b - 2;
				}
			}
		}
		else
		{
			fclose(msgfil);
			sprintf(ErrMSG, "*ERROR* (Pixie16ReadMSGFile): could not seek to end of Pixie16msg.txt");
			Pixie_Print_MSG(ErrMSG);
			return(-5);
		}
		
		fclose(msgfil);
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadMSGFile): could not open Pixie16msg.txt");
		Pixie_Print_MSG(ErrMSG);
		return(-6);
	}
	
	return(0);
}


/****************************************************************
*	Pixie_CopyDSPParameters:
*		Copy DSP parameters to the specific modules and
*		channels.
*
*		Return Value:
*			 0 - Success
*
****************************************************************/

int Pixie_CopyDSPParameters (
	unsigned short BitMask,				// copy/extract bit mask pattern
	unsigned short SourceModule,		// source module number
	unsigned short SourceChannel,		// source Pixie channel
	unsigned short DestinationModule,	// destination module number
	unsigned short DestinationChannel )	// destination channel number
{
	unsigned int valA, valB;
	
	// Filter (energy and trigger)
	if( APP16_TstBit(0, BitMask) )
	{
		//****************************//
		// Energy filter
		//****************************//
		
		// Copy rise time
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[SlowLength_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[SlowLength_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
		// Copy flat top
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[SlowGap_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[SlowGap_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
		// Copy peaksep
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[PeakSep_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[PeakSep_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
		// Copy peaksample
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[PeakSample_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[PeakSample_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
		// Copy trigger delay
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[TriggerDelay_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[TriggerDelay_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
		// Copy paflength
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[PAFlength_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[PAFlength_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
		// Copy blcut
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[BLcut_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[BLcut_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
		// Copy CoincWait
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[CoincWait_Address[DestinationModule] - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[CoincWait_Address[SourceModule] - DATA_MEMORY_ADDRESS];
		
		// Copy SlowFilterRange
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[SlowFilterRange_Address[DestinationModule] - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[SlowFilterRange_Address[SourceModule] - DATA_MEMORY_ADDRESS];
		
		//****************************//
		// Trigger filter
		//****************************//
		
		// Copy rise time
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[FastLength_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[FastLength_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
		// Copy flat top
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[FastGap_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[FastGap_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
		// Copy threshold
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[FastThresh_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[FastThresh_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
		
		// Copy FastFilterRange
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[FastFilterRange_Address[DestinationModule] - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[FastFilterRange_Address[SourceModule] - DATA_MEMORY_ADDRESS];
	}
	
	// Analog signal conditioning
	if( APP16_TstBit(1, BitMask) )
	{
		// Copy gain (attenuation) and polarity
		valA = Pixie_Devices[SourceModule].DSP_Parameter_Values[ChanCSRa_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
		valB = Pixie_Devices[DestinationModule].DSP_Parameter_Values[ChanCSRa_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS];
		if( APP32_TstBit(CCSRA_ENARELAY, valA) )
		{
			valB = APP32_SetBit(CCSRA_ENARELAY, valB);
		}
		else
		{
			valB = APP32_ClrBit(CCSRA_ENARELAY, valB);
		}
		
		if( APP32_TstBit(CCSRA_POLARITY, valA) )
		{
			valB = APP32_SetBit(CCSRA_POLARITY, valB);
		}
		else
		{
			valB = APP32_ClrBit(CCSRA_POLARITY, valB);
		}
		
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[ChanCSRa_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = valB;
		
		// Copy offset DAC
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[OffsetDAC_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[OffsetDAC_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
	}
	
	// Histogram control
	if( APP16_TstBit(2, BitMask) )
	{
		// Copy energylow
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[EnergyLow_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[EnergyLow_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
		// Copy log2ebin
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[Log2Ebin_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[Log2Ebin_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
	}
	
	// Decay time
	if( APP16_TstBit(3, BitMask) )
	{
		// Copy preamp tau
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[PreampTau_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[PreampTau_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
	}
	
	// Pulse shape analysis
	if( APP16_TstBit(4, BitMask) )
	{
		// Copy trace length
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[TraceLength_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[TraceLength_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
		// Copy trigger delay
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[TriggerDelay_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[TriggerDelay_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
		// Copy paflength
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[PAFlength_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[PAFlength_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
	}
	
	// Baseline control
	if( APP16_TstBit(5, BitMask) )
	{
		// Copy blcut
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[BLcut_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[BLcut_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
		// Copy baseline percent
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[BaselinePercent_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[BaselinePercent_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
		// Copy baseline average
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[Log2Bweight_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[Log2Bweight_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
	}

	// Channel CSRA register
	if( APP16_TstBit(7, BitMask) )
	{
		// Copy CHANCSRA
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[ChanCSRa_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[ChanCSRa_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
	}

	// CFD Trigger
	if( APP16_TstBit(8, BitMask) )
	{
		// Copy CFDDelay
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[CFDDelay_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[CFDDelay_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];

		// Copy CFDScale
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[CFDScale_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[CFDScale_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];

		// Copy CFDThresh
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[CFDThresh_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[CFDThresh_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
	}

	// Trigger Stretch Lengths
	if( APP16_TstBit(9, BitMask) )
	{
		// Copy External Trigger Stretch
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[ExtTrigStretch_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[ExtTrigStretch_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];

		// Copy Channel Trigger Stretch
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[ChanTrigStretch_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[ChanTrigStretch_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];

		// Copy Veto Stretch
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[VetoStretch_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[VetoStretch_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];

		// Copy Fast Trigger Backplane Length
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[FastTrigBackLen_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[FastTrigBackLen_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
	}

	// FIFO Delays
	if( APP16_TstBit(10, BitMask) )
	{
		// Copy External Delay Length
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[ExternDelayLen_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[ExternDelayLen_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];

		// Copy Fast Trigger Backplane Delay
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[FtrigoutDelay_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[FtrigoutDelay_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
	}

	// Multiplicity
	if( APP16_TstBit(11, BitMask) )
	{
		// Copy Multiplicity Mask Low
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[MultiplicityMaskL_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[MultiplicityMaskL_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];

		// Copy Multiplicity Mask High
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[MultiplicityMaskH_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[MultiplicityMaskH_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
	}

	// QDC
	if( APP16_TstBit(12, BitMask) )
	{
		// Copy QDCLen0
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[QDCLen0_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[QDCLen0_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];

		// Copy QDCLen1
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[QDCLen1_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[QDCLen1_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];

		// Copy QDCLen2
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[QDCLen2_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[QDCLen2_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];

		// Copy QDCLen3
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[QDCLen3_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[QDCLen3_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];

		// Copy QDCLen4
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[QDCLen4_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[QDCLen4_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];

		// Copy QDCLen5
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[QDCLen5_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[QDCLen5_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];

		// Copy QDCLen6
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[QDCLen6_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[QDCLen6_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];

		// Copy QDCLen7
		Pixie_Devices[DestinationModule].DSP_Parameter_Values[QDCLen7_Address[DestinationModule] + DestinationChannel - DATA_MEMORY_ADDRESS] = Pixie_Devices[SourceModule].DSP_Parameter_Values[QDCLen7_Address[SourceModule] + SourceChannel - DATA_MEMORY_ADDRESS];
	}
	
	return(0);
}

