#ifndef PIXIE16SYS_COMMON_H
#define PIXIE16SYS_COMMON_H

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
*		pixie16sys_common.h
*
* Description:
*
*		Declaration of prototypes of common functions.
*
* $Rev: 15626 $
* $Id: pixie16sys_common.h 15626 2010-05-07 18:51:20Z htan $
******************************************************************************/

// If this is compiled by a C++ compiler, make it
// clear that these are C routines.
#ifdef __cplusplus
extern "C" {
#endif
	
	unsigned short SYS16_SetBit(unsigned short bit, unsigned short value);
	unsigned short SYS16_ClrBit(unsigned short bit, unsigned short value);
	unsigned short SYS16_TstBit(unsigned short bit, unsigned short value);
	unsigned int SYS32_SetBit(unsigned short bit, unsigned int value);
	unsigned int SYS32_ClrBit(unsigned short bit, unsigned int value);
	unsigned int SYS32_TstBit(unsigned short bit, unsigned int value);
	
	
	int get_ns_per_cycle(double *ns_per_cycle);
	void wait_for_a_short_time(int cycles);
	
	int Pixie_DSP_Memory_Burst_Read (
	  unsigned int *dsp_data,        // DSP data for the I/O
	  unsigned int dsp_address,      // DSP data memory address
	  unsigned int nWords,           // Number of DSP data words for the I/O
	  unsigned short ModNum );       // The Pixie module for the I/O
	
	
	int I2CM24C64_start(unsigned short ModNum);

	int I2CM24C64_stop(unsigned short ModNum);
	int I2CM24C64_byte_write(unsigned short ModNum, char ByteToSend);
	int I2CM24C64_byte_read(unsigned short ModNum, char *ByteToReceive);

	char I2CM24C64_getACK(unsigned short ModNum);
	char I2CM24C64_sendACK(unsigned short ModNum);


#ifdef __cplusplus
}
#endif	// End of notice for C++ compilers

#endif	// End of pixie16sys_common.h
