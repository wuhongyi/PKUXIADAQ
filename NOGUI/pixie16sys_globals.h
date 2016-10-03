#ifndef PIXIE16SYS_GLOBALS_H
#define PIXIE16SYS_GLOBALS_H

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
*		pixie16sys_globals.h
*
* Description:
*
*		Declaration of pixie16 library global variables.
*
* $Rev: 15626 $
* $Id: pixie16sys_globals.h 15626 2010-05-07 18:51:20Z htan $
******************************************************************************/

// If this is compiled by a C++ compiler, make it
// clear that these are C routines.
#ifdef __cplusplus
extern "C" {
#endif
	
	PLX_UINT_PTR 		VAddr[SYS_MAX_NUM_MODULES];			// PCI device virutal address
	PLX_DEVICE_OBJECT	SYS_hDevice[SYS_MAX_NUM_MODULES];	// PCI device handle
	unsigned short		SYS_Number_Modules;					// Total number of modules in the crate
	unsigned short		SYS_Offline;						// SYS_Offline = 1: offline mode; SYS_Offline = 0: Online mode

	double  Ns_Per_Cycle;									// The time needed for each cycle, in ns
	
	
	
#ifdef __cplusplus
}
#endif	// End of notice for C++ compilers

#endif	// End of pixie16sys_globals.h
