#ifndef PIXIE16SYS_DEFS_H
#define PIXIE16SYS_DEFS_H

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
*		pixie16sys_defs.h
*
* Description:
*
*		Declaration of pixie16 library constants.
*
* $Rev: 28166 $
* $Id: pixie16sys_defs.h 28166 2013-10-07 18:33:28Z htan $
******************************************************************************/

// If this is compiled by a C++ compiler, make it
// clear that these are C routines.
#ifdef __cplusplus
extern "C" {
#endif	

/*------------------------------------- 
	At which platform to compile this code -
	Windows or Linux?
  -------------------------------------*/
#define PIXIE16_WINDOWS_SYSAPI    0
#define PIXIE16_LINUX_SYSAPI      1
	// Changing PIXIE16_SYSAPI_VER here affects the code globally
#define PIXIE16_SYSAPI_VER        PIXIE16_LINUX_SYSAPI	
	

/*------------------------------------- 
	Define EXPORT macro
  -------------------------------------*/
	
#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
#define PIXIE16SYS_EXPORT __declspec(dllexport)
#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI
#define PIXIE16SYS_EXPORT
#endif
	
	
#ifndef PCI_CODE
#define PCI_CODE
#endif
	
#ifndef LITTLE_ENDIAN   // LITTLE_ENDIAN: least significant byte stored first
#define LITTLE_ENDIAN   // BIG_ENDIAN:    most significant byte stored first
#endif
	
	
#define SYS_MAX_NUM_MODULES  32   // Preset maximum number of modules for defining data structures
#define SYS_MAX_PXI_SLOTS    14   // Maximum number of PXI slots for one PXI chassis (currently 14-slot is the biggest chassis)


/*-------------------------------------
	Multiplication factor for the ns_per_cycle
  -------------------------------------*/
#define NSMULTIPLIER        1.0


/*-------------------------------------
	Option for printing out debugging messages 
  -------------------------------------*/
#define PRINT_DEBUG_MSG			1
	

/*------------------------------------- 
	DSP code tag types
  -------------------------------------*/
	
#define FINAL_INIT  0X0000
#define ZERO_DM16   0X0001
#define ZERO_DM32   0X0002
#define ZERO_DM40   0X0003
#define INIT_DM16   0X0004
#define INIT_DM32   0X0005
#define INIT_DM40   0X0006
#define ZERO_PM16   0X0007
#define ZERO_PM32   0X0008
#define ZERO_PM40   0X0009
#define ZERO_PM48   0X000A
#define INIT_PM16   0X000B
#define INIT_PM32   0X000C
#define INIT_PM40   0X000D
#define INIT_PM48   0X000E
#define ZERO_DM64   0X000F
#define INIT_DM64   0X0010
#define ZERO_PM64   0X0011
#define INIT_PM64   0X0012


/*------------------------------------- 
	I/O Mode
  -------------------------------------*/
	
#define SYS_MOD_READ        1       // Host read from modules
#define SYS_MOD_WRITE       0       // Host write to modules
	
#define MAX_ERRMSG_LENGTH	1024    // Maximum length of error message


 /*------------------------------------- 
	Host PCI CSR bits definitions
 --------------------------------------*/ 
#define RUNENA               0  // Enable run
#define DSPDOWNLOAD          1  // Enable DSP code download
#define PCIACTIVE            2  // Indicate PCI I/O is active
#define PULLUP_CTRL          3  // Control pull-up for the SYNC lines
#define DSPRESET             4  // Reset DSP
#define EXTFIFO_WML          6  // External FIFO watermark level indicator
#define RUNACTIVE           13  // Run active indicator
#define CLREXTMEM_ACTIVE    15  // Clearing external memory active indicator




/*------------------------------------- 
	PCI address decoding and control registers
-------------------------------------*/

#define CFG_DATACS      0x00
#define CFG_CTRLCS      0x04
#define CFG_RDCS        0x08
#define I2CM24C64_ADDR	0x10
#define CFG_DCMRST      0x14
#define CSR_ADDR        0x48

#define REQUEST_HBR     0x80
#define HBR_DONE        0x84
#define WRT_EXT_MEM     0xC0
#define EXT_MEM_TEST    0xC4
#define WRT_DSP_MMA     0xC8
#define SET_EXT_FIFO    0xCC

#define WRT_DSP_DMAC11  0xD0
#define WRT_DSP_II11    0xD4
#define WRT_CLR_EXTMEM  0xD8
#define WRT_DSP_C11     0xDC
#define WRT_DSP_IM11    0xE0
#define WRT_DSP_EC11    0xE4
#define RD_WRT_FIFO_WML 0xE8
#define SET_EXMEM_FIFO  0xEC
#define SET_INT_FIFO    0xF0
#define WRT_RESET_RFCNT 0xF4

/*------------------------------------- 
	Special DSP varaible address
-------------------------------------*/
#define POWERUPINITDONE_ADDRESS      0x4A03F      // Address for DSP variable PowerUpInitDone

/*------------------------------------- 
	PLX DMA transfer timeout setting
-------------------------------------*/
#define DMATRANSFER_TIMEOUT          5*1000       // PLX DMA transfer timeout limit, in ms


#ifdef __cplusplus
}
#endif	// End of notice for C++ compilers

#endif	// End of pixie16sys_defs.h
