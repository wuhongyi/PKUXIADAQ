#ifndef PIXIE16SYS_EXPORT_H
#define PIXIE16SYS_EXPORT_H

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
*		pixie16sys_export.h
*
* Description:
*
*		Function prototypes seen by user applications.
*
* $Rev: 18950 $
* $Id: pixie16sys_export.h 18950 2011-06-01 00:53:43Z htan $
******************************************************************************/

// If this is compiled by a C++ compiler, make it
// clear that these are C routines.
#ifdef __cplusplus
extern "C" {
#endif
	
#ifndef __PIXIE16SYS_DEFS_H
#include "pixie16sys_defs.h"
#endif
	
	
	PIXIE16SYS_EXPORT int Pixie_InitSystem (
		unsigned short NumModules,
		unsigned short *PXISlotMap,
		unsigned short OfflineMode );
	
	PIXIE16SYS_EXPORT int Pixie_ClosePCIDevices (
		unsigned short ModNum );
	
	PIXIE16SYS_EXPORT int Pixie_Boot_CompFPGA (
		unsigned short ModNum,
		unsigned int   *Com_FPGA_conf,
		unsigned int   NumComFPGAconf );

	PIXIE16SYS_EXPORT int Pixie_Boot_TrigFPGA (
		unsigned short ModNum,
		unsigned int   *Trig_FPGA_conf,
		unsigned int   NumTrigFPGAconf );

	PIXIE16SYS_EXPORT int Pixie_Boot_FIPPI (
		unsigned short ModNum,
		unsigned int   *SP_FPGA_conf,
		unsigned int   NumSPFPGAconf );
	
	PIXIE16SYS_EXPORT int Pixie_Boot_DSP (
		unsigned short ModNum,
		unsigned int   *DSP_code,
		unsigned int   NumDSPCodeWords );
	
	PIXIE16SYS_EXPORT int Pixie_DSP_Memory_IO (
		unsigned int   *dsp_data,   // DSP data for the I/O
		unsigned int   dsp_address, // DSP data memory address
		unsigned int   nWords,      // Number of DSP data words for the I/O
		unsigned short direction,   // I/O direction
		unsigned short ModNum );    // The Pixie module for the I/O
	
	PIXIE16SYS_EXPORT int Pixie_Read_ExtFIFOStatus (
		unsigned int   *nFIFOWords, // Return the number of 32-bit words in the external FIFO
		unsigned short ModNum );    // The Pixie module being addressed

	PIXIE16SYS_EXPORT int Pixie_ExtFIFO_Read (
		unsigned int   *extfifo_data,// To receive the external FIFO data
		unsigned int   nWords,       // Number of external FIFO data words to read
		unsigned short ModNum );     // The Pixie module for the I/O

	PIXIE16SYS_EXPORT int Pixie_Main_Memory_IO (
		unsigned int   *memory_data,    // Memory data for the I/O
		unsigned int   memory_address,  // Main memory address
		unsigned int   nWords,          // Number of data words for the I/O
		unsigned short direction,       // I/O direction
		unsigned short ModNum );        // The Pixie module for the I/O
	
	PIXIE16SYS_EXPORT int Pixie_Clear_Main_Memory (
		unsigned int memory_address,    // Main memory address
		unsigned int nWords,            // Number of data words for the I/O
		unsigned short ModNum );        // The Pixie module for the I/O

	PIXIE16SYS_EXPORT int Pixie_Register_IO (
		unsigned short ModNum,     // the Pixie module to communicate to
		unsigned int   address,    // register address
		unsigned short direction,  // either MOD_READ or MOD_WRITE
		unsigned int   *value );   // holds or receives the data
	
	PIXIE16SYS_EXPORT void Pixie_ReadCSR (
		unsigned short ModNum,
		unsigned int   *CSR );
	
	PIXIE16SYS_EXPORT void Pixie_WrtCSR (
		unsigned short ModNum,
		unsigned int   CSR );
	
	PIXIE16SYS_EXPORT int Pixie_Print_MSG (
		char *message );

	PIXIE16SYS_EXPORT int I2CM24C64_Write_One_Byte (
		unsigned short ModNum,		// Pixie module module
		unsigned short Address,		// The address to write this byte
		char *ByteValue );			// The byte value

	PIXIE16SYS_EXPORT int I2CM24C64_Read_One_Byte (
		unsigned short ModNum,		// Pixie module number
		unsigned short Address,		// The address to read this byte
		char *ByteValue );			// The byte value

	PIXIE16SYS_EXPORT int I2CM24C64_Page_Write (
		unsigned short ModNum,			// Pixie module module
		unsigned short Address,			// The starting address to write this page
		unsigned short NumBytesToWrite,	// The number of bytes for this page to write
		char *ByteValue );				// The byte values

	PIXIE16SYS_EXPORT int I2CM24C64_Sequential_Read (
		unsigned short ModNum,			// Pixie module number
		unsigned short Address,			// The starting address to read
		unsigned short NumBytesToRead,	// Number of bytes to read (maximum 32 bytes)
		char *ByteValue );				// The byte values

#ifdef __cplusplus
}
#endif	// End of notice for C++ compilers

#endif	// End of pixie16sys_export.h
