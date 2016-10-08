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
*		pixie16.c
*
* Description:
*
*		This file contains all the Pixie16 interface routines.
*
* $Rev: 33531 $
* $Id: pixie16app.c 33531 2015-10-14 23:31:11Z htan $
******************************************************************************/

#include "pixie16app_globals.h"
#include "pixie16sys_export.h"
#include "pixie16app_export.h"
#include "pixie16app_common.h"
#include "xia_common.h"

#if PIXIE16_APPAPI_VER == PIXIE16_WINDOWS_APPAPI
#include <windows.h>
#elif PIXIE16_APPAPI_VER == PIXIE16_LINUX_APPAPI
#include <stdlib.h>
#include <unistd.h>
#endif

#include <stdio.h>


/****************************************************************
*	Pixie16InitSystem:
*		Initialize Pixie16 system by mapping PXI slots to Pixie16
*		modules and assigning vitural addresses to the PLX9054 chips
*		of those modules.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid total number of Pixie16 modules
*			-2 - Null pointer *PXISlotMap
*			-3 - Failed to initialize system
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16InitSystem (
	unsigned short NumModules,    // total number of Pixie16 modules in the system
	unsigned short *PXISlotMap,   // an array containing the PXI slot number for each pixie16 module
	unsigned short OfflineMode )  // specify if the system is in offline mode
{
	int retval;    // return value
	char ErrMSG[MAX_ERRMSG_LENGTH];	// message
	unsigned short k;
	
	// Check the validity of total number of Pixie16 modules
	if( (NumModules < 1) || (NumModules > PRESET_MAX_MODULES) )  // At least one module is necessary
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16InitSystem): invalid total number (%d) of Pixie16 modules", NumModules);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	else
	{
		Number_Modules = NumModules;  // Number_Modules is a global variable
	}
	
	Offline = OfflineMode;
	
	// The physical address (slot number) of each Pixie16 module is in PXISlotMap
	if( PXISlotMap == NULL )  // Check if PXISlotMap is valid
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16InitSystem): null pointer *PXISlotMap");
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	else
	{
		// Scan PCI devices and assign address
		retval = Pixie_InitSystem(Number_Modules, PXISlotMap, Offline);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16InitSystem): failed to initialize system, retval=%d", retval);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}
		
		// Read and store module information
		for(k = 0; k < Number_Modules; k ++)
		{
			retval = Pixie16ReadModuleInfo(k, &Module_Information[k].Module_Rev, &Module_Information[k].Module_SerNum, &Module_Information[k].Module_ADCBits, &Module_Information[k].Module_ADCMSPS);
			if(retval < 0)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16InitSystem): failed to read module information in module %d, retval=%d", k, retval);
				Pixie_Print_MSG(ErrMSG);
				return(-3);
			}
		}

		return(0);
	}
}


/****************************************************************
*	Pixie16ExitSystem:
*		Release resources used by Pixie-16 PCI devices before existing
*		the Pixie16 application.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Failed to close Pixie module
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ExitSystem (
	unsigned short ModNum )		// module number
{
	
	int retval;    // return values
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ExitSystem): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	retval = Pixie_ClosePCIDevices(ModNum);
	if(retval < 0)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ExitSystem): failed to close PCI device, retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	return(0);
}


/****************************************************************
*	Pixie16ReadModuleInfo:
*		Read information stored on each module, including its
*		revision, serial number, ADC characteristics.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Failed to read from I2C serial EEPROM
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadModuleInfo (
	unsigned short ModNum,			// module number
	unsigned short *ModRev,			// returned module revision
	unsigned int   *ModSerNum,		// returned module serial number
	unsigned short *ModADCBits,		// returned module ADC bits
	unsigned short *ModADCMSPS )	// returned module ADC sampling rate
{
	
	int retval;    // return values
	char ErrMSG[MAX_ERRMSG_LENGTH];
	char sbuffer[100]  = {0};   // Temporary buffer
	unsigned short startaddr;
	
	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadModuleInfo): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}

	// Check if running in OFFLINE mode
	if( Offline == 1 )	  // Returns a set of default module information for offline analysis
	{
		*ModRev = 0xF;
		*ModSerNum = 1000;
		*ModADCBits = 12;
		*ModADCMSPS = 250;
		return(0);
	}

	// Read module serial number which is stored in the beginning of the I2C serial EEPROM.
	// First three words of EEPROM stores serial number and revision number
	startaddr = 0;
	retval = I2CM24C64_Sequential_Read(ModNum, startaddr, 3, sbuffer);
	if(retval < 0)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadModuleInfo): Could not read serial number for Module=%d; retval=%d", ModNum, retval);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	// Starting with serial number 256, serial number is stored in the first two bytes of EEPROM, follwed by
	// revision number, which is at least 11 (i.e. Rev-B)
	if (sbuffer[2] >= 11)
	{
		*ModSerNum = (unsigned short)(unsigned char)sbuffer[0] + 256 * (unsigned short)(unsigned char)sbuffer[1];
		*ModRev = (unsigned short)(unsigned char)sbuffer[2];

		// For serial number below 275, no ADC information is stored in the I2C serial EEPROM.
		// However, all such modules have 12-bit, 100 MSPS ADCs.
		if(*ModSerNum < 275)
		{
			*ModADCBits = 12;
			*ModADCMSPS = 100;
		}
		// For serial number between 1000 and 1034, no ADC information is stored in the I2C serial EEPROM.
		// However, all such modules have 12-bit, 250 MSPS ADCs.
		else if((*ModSerNum >= 1000) && (*ModSerNum <= 1034))
		{
			*ModADCBits = 12;
			*ModADCMSPS = 250;
		}
		// For serial number 1035 and above, ADC information is stored in the I2C serial EEPROM.
		else if(*ModSerNum >= 1035)
		{
			// ADC information is stored in the I2C serial EEPROM, starting at address 99
			startaddr = 99;
			
			retval = I2CM24C64_Sequential_Read(ModNum, startaddr, 3, sbuffer);
			if(retval < 0)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16ReadModuleInfo): Could not read ADC information for Module=%d; retval=%d", ModNum, retval);
				Pixie_Print_MSG(ErrMSG);
				return(-2);
			}

			*ModADCBits = (unsigned short)(unsigned char)sbuffer[0];
			*ModADCMSPS = (unsigned short)(unsigned char)sbuffer[1] + 256 * (unsigned short)(unsigned char)sbuffer[2];
		}
	}
	else
	{
		*ModSerNum = (unsigned short)(unsigned char)sbuffer[0];
		*ModRev = (unsigned short)(unsigned char)sbuffer[1];

		// Prior to serial number 256, no ADC information is stored in the I2C serial EEPROM.
		// However, all such modules have 12-bit, 100 MSPS ADCs.
		*ModADCBits = 12;
		*ModADCMSPS = 100;
	}

	return(0);
}


/****************************************************************
*	Pixie16BootModule
*		Boot one Pixie16 module or all modules.
*
*		Return Value:
*			 0 - Success
*
*			-1 - Invalid pixie16 module number
*			-2 - Size of ComFPGAConfigFile is invalid
*			-3 - Failed to boot Communication FPGA
*			-4 - Failed to allocate memory to store ComFPGAConfig
*			-5 - Failed to open ComFPGAConfigFile
*
*			-6 - Size of TrigFPGAConfigFile is invalid
*			-7 - Failed to boot trigger FPGA
*			-8 - Failed to allocate memory to store TrigFPGAConfigFile
*			-9 - Failed to open TrigFPGAConfigFile 
*
*			-10 - Size of SPFPGAConfigFile is invalid
*			-11 - Failed to boot signal processing FPGA
*			-12 - Failed to allocate memory to store SPFPGAConfigFile
*			-13 - Failed to open SPFPGAConfigFile 
*
*			-14 - Failed to boot DSP
*			-15 - Failed to allocate memory to store DSP executable code
*			-16 - Failed to open DSPCodeFile 
*
*			-17 - Size of DSPParFile is invalid
*			-18 - Failed to open DSPParFile
*
*			-19 - Can't initialize DSP variable indices
*			-20 - Can't copy DSP variable indices
*
*			-21 - Failed to program Fippi in a module
*			-22 - Failed to set DACs in a module
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16BootModule (
	char *ComFPGAConfigFile,      // name of communications FPGA configuration file
	char *SPFPGAConfigFile,       // name of signal processing FPGA configuration file
	char *TrigFPGAConfigFile,     // name of trigger FPGA configuration file
	char *DSPCodeFile,            // name of executable code file for digital signal processor (DSP)
	char *DSPParFile,             // name of DSP parameter file
	char *DSPVarFile,             // name of DSP variable names file
	unsigned short ModNum,        // pixie module number
	unsigned short BootPattern )  // boot pattern bit mask
{
	int retval;    // return values
	char ErrMSG[MAX_ERRMSG_LENGTH];
	FILE *configfil;
	unsigned int *configuration;
	unsigned short k, dspcode;
	unsigned int TotalWords, CSR;
	unsigned int CPLD_CSR;
	unsigned int FPGA_ConfigSize;
	unsigned short sourcemodule;
	
	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	//**************************************************
	// Boot communication FPGA
	//**************************************************
	
	if((APP16_TstBit(BOOTPATTERN_COMFPGA_BIT, BootPattern)) && (Offline == 0))
	{
		// Open communication FPGA configuration file
		if( (configfil = fopen(ComFPGAConfigFile, "rb")) != NULL )  // Make sure ComFPGAConfigFile is opened successfully
		{
			// Download to one or all modules
			if(ModNum == Number_Modules)	// Download to all modules
			{
				sourcemodule = 0;		// All modules are assumed to have the same revision
			}
			else	// Download to a single module only
			{
				sourcemodule = ModNum;	// Use the specified module
			}

			// Set FPGA configuration size based on module revision
			if ((Module_Information[sourcemodule].Module_Rev == 0xB) || (Module_Information[sourcemodule].Module_Rev == 0xC) || (Module_Information[sourcemodule].Module_Rev == 0xD))
				FPGA_ConfigSize = N_COM_FPGA_CONF_REVBCD;
			else if (Module_Information[sourcemodule].Module_Rev == 0xF)
				FPGA_ConfigSize = N_COM_FPGA_CONF_REVF;

			// Check if file size is consistent with predefined length FPGA_ConfigSize
			fseek(configfil, 0, SEEK_END);
			TotalWords = (ftell(configfil) + 1) / 4;

			if(TotalWords != FPGA_ConfigSize)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): size of ComFPGAConfigFile is invalid. Check ComFPGAConfigFile name");
				Pixie_Print_MSG(ErrMSG);
				return(-2);
			}
			
			// Allocate memory
			if( (configuration = (unsigned int *)malloc(sizeof(unsigned int) * FPGA_ConfigSize)) != NULL)
			{
				// Point configfil to the beginning of file
				fseek(configfil, 0, SEEK_SET);
				
				// Read communication FPGA configuration
				fread(configuration, sizeof(unsigned int), FPGA_ConfigSize, configfil);

				if(ModNum == Number_Modules)	// Download to all modules
				{
					for(k=0; k<Number_Modules; k++)
					{
						printf("Start to boot Communication FPGA in module %d\n", k);

						retval = Pixie_Boot_CompFPGA(k, configuration, FPGA_ConfigSize);
						if(retval < 0)
						{
							sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to boot Communication FPGA in module %d, retval=%d", k, retval);
							Pixie_Print_MSG(ErrMSG);
							
							// free allocated memory and close opened files
							free(configuration);
							fclose(configfil);
							
							return(-3);
						}
					}
				}
				else	// Download to a single module only
				{
					printf("Start to boot Communication FPGA in module %d\n", ModNum);

					retval = Pixie_Boot_CompFPGA(ModNum, configuration, FPGA_ConfigSize);
					if(retval < 0)
					{
						sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to boot Communication FPGA in module %d, retval=%d", ModNum, retval);
						Pixie_Print_MSG(ErrMSG);
							
						// free allocated memory and close opened files
						free(configuration);
						fclose(configfil);
							
						return(-3);
					}
				}
				
				// free allocated memory
				free(configuration);
			}
			else
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to allocate memory to store ComFPGAConfig");
				Pixie_Print_MSG(ErrMSG);
				return(-4);
			}
			
			// close opened files
			fclose(configfil);
		}
		else
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to open ComFPGAConfigFile %s", ComFPGAConfigFile);
			Pixie_Print_MSG(ErrMSG);
			return(-5);
		}
	}
	
	//**************************************************
	// Boot signal processing FPGA
	//**************************************************
	
	if((APP16_TstBit(BOOTPATTERN_SPFPGA_BIT, BootPattern)) && (Offline == 0))
	{
		// Open signal processing FPGA configuration file
		if( (configfil = fopen(SPFPGAConfigFile, "rb")) != NULL )  // Make sure SPFPGAConfigFile is opened successfully
		{
			// Download to one or all modules
			if(ModNum == Number_Modules)	// Download to all modules
			{
				sourcemodule = 0;		// All modules are assumed to have the same revision
			}
			else	// Download to a single module only
			{
				sourcemodule = ModNum;	// Use the specified module
			}

			// Set FPGA configuration size based on module revision
			if ((Module_Information[sourcemodule].Module_Rev == 0xB) || (Module_Information[sourcemodule].Module_Rev == 0xC) || (Module_Information[sourcemodule].Module_Rev == 0xD))
				FPGA_ConfigSize = N_SP_FPGA_CONF_REVBCD;
			else if (Module_Information[sourcemodule].Module_Rev == 0xF)
				FPGA_ConfigSize = N_SP_FPGA_CONF_REVF;

			// Check if file size is consistent with predefined length FPGA_ConfigSize
			fseek(configfil, 0, SEEK_END);
			TotalWords = (ftell(configfil) + 1) / 4;

			if(TotalWords != FPGA_ConfigSize)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): size of SPFPGAConfigFile is invalid. Check SPFPGAConfigFile name");
				Pixie_Print_MSG(ErrMSG);
				return(-10);
			}
			
			// Allocate memory
			if( (configuration = (unsigned int *)malloc(sizeof(unsigned int) * FPGA_ConfigSize)) != NULL)
			{
				// Point configfil to the beginning of file
				fseek(configfil, 0, SEEK_SET);
				
				// Read trigger FPGA configuration
				fread(configuration, sizeof(unsigned int), FPGA_ConfigSize, configfil);
				
				// Download to one or all modules
				if(ModNum == Number_Modules)	// Download to all modules
				{
					for(k=0; k<Number_Modules; k++)
					{
						printf("Start to boot signal processing FPGA in module %d\n", k);

						retval = Pixie_Boot_FIPPI(k, configuration, FPGA_ConfigSize);
						if(retval < 0)
						{
							sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to boot signal processing FPGA in module %d, retval=%d", k, retval);
							Pixie_Print_MSG(ErrMSG);
							
							// free allocated memory and close opened files
							free(configuration);
							fclose(configfil);
							
							return(-11);
						}
					}
				}
				else	// Download to a single module only
				{
					printf("Start to boot signal processing FPGA in module %d\n", ModNum);

					retval = Pixie_Boot_FIPPI(ModNum, configuration, FPGA_ConfigSize);
					if(retval < 0)
					{
						sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to boot signal processing FPGA in module %d, retval=%d", ModNum, retval);
						Pixie_Print_MSG(ErrMSG);
						
						// free allocated memory and close opened files
						free(configuration);
						fclose(configfil);
						
						return(-11);
					}
				}
				
				// free allocated memory
				free(configuration);
			}
			else
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to allocate memory to store SPFPGAConfig");
				Pixie_Print_MSG(ErrMSG);
				return(-12);
			}
			
			// close opened files
			fclose(configfil);
		}
		else
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to open SPFPGAConfigFile %s", SPFPGAConfigFile);
			Pixie_Print_MSG(ErrMSG);
			return(-13);
		}
	}
	
	//**************************************************
	// Boot Digital Signal Processor (DSP)
	//**************************************************
	if((APP16_TstBit(BOOTPATTERN_DSPCODE_BIT, BootPattern)) && (Offline == 0))
	{
		// Open DSP executable code file
		if( (configfil = fopen(DSPCodeFile, "rb")) != NULL )  // Make sure DSPCodeFile is opened successfully
		{
			// Check the file size
			fseek(configfil, 0, SEEK_END);
			TotalWords = (ftell(configfil) + 1) / 2;
			
			// Allocate memory
			if( (configuration = (unsigned int *)malloc(sizeof(unsigned int) * TotalWords)) != NULL)
			{
				// Point configfil to the beginning of file
				fseek(configfil, 0, SEEK_SET);
				
				// Read DSP executable code
				for(k=0; k<TotalWords; k++)
				{
					fread(&dspcode, sizeof(unsigned short), 1, configfil);
					
					// Rearrange byte order
					configuration[k] = (unsigned int)((dspcode >> 8) + (dspcode & 0xFF) * 256);
				}
				
				// Download to one or all modules
				if(ModNum == Number_Modules)	// Download to all modules
				{
					for(k=0; k<Number_Modules; k++)
					{
						printf("Start to boot DSP in module %d\n", k);

						retval = Pixie_Boot_DSP(k, configuration, TotalWords);
						if(retval < 0)
						{
							sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to boot DSP in module %d, retval=%d", k, retval);
							Pixie_Print_MSG(ErrMSG);
							
							// free allocated memory and close opened files
							free(configuration);
							fclose(configfil);
							
							return(-14);
						}
					}
				}
				else	// Download to a single module only
				{
					printf("Start to boot DSP in module %d\n", ModNum);

					retval = Pixie_Boot_DSP(ModNum, configuration, TotalWords);
					if(retval < 0)
					{
						sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to boot DSP in module %d, retval=%d", ModNum, retval);
						Pixie_Print_MSG(ErrMSG);
						
						// free allocated memory and close opened files
						free(configuration);
						fclose(configfil);
						
						return(-14);
					}
				}
				
				// free allocated memory
				free(configuration);
			}
			else
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to allocate memory to store DSP executable code");
				Pixie_Print_MSG(ErrMSG);
				return(-15);
			}
			
			// close opened files
			fclose(configfil);
		}
		else
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to open DSPCodeFile %s", DSPCodeFile);
			Pixie_Print_MSG(ErrMSG);
			return(-16);
		}
	}
	
	//**************************************************
	// Download DSP I/O parameters
	//**************************************************
	
	if(APP16_TstBit(BOOTPATTERN_DSPPAR_BIT, BootPattern))
	{
		// Open DSP parameters file
		if( (configfil = fopen(DSPParFile, "rb")) != NULL )  // Make sure DSPParFile is opened successfully
		{
			// Check if file size is consistent with predefined length (N_DSP_PAR * PRESET_MAX_MODULES)
			fseek(configfil, 0, SEEK_END);
			TotalWords = (ftell(configfil) + 1) / 4;
			if(TotalWords != (N_DSP_PAR * PRESET_MAX_MODULES))
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): size of DSPParFile is invalid. Check DSPParFile name");
				Pixie_Print_MSG(ErrMSG);
				// close opened files
				fclose(configfil);
				return(-17);
			}
			
			// Point configfil to the beginning of file
			fseek(configfil, 0, SEEK_SET);
			
			// Read DSP parameters
			for(k=0; k<PRESET_MAX_MODULES; k++)
			{
				fread(Pixie_Devices[k].DSP_Parameter_Values, sizeof(unsigned int), N_DSP_PAR, configfil);
				
				// Force correct module number
				Pixie_Devices[k].DSP_Parameter_Values[0] = k;
			}
			
			// Download to one or all modules
			if(ModNum == Number_Modules)	// Download to all modules
			{
				for(k=0; k<Number_Modules; k++)
				{
					Pixie_DSP_Memory_IO(Pixie_Devices[k].DSP_Parameter_Values, DATA_MEMORY_ADDRESS, DSP_IO_BORDER, MOD_WRITE, k);
				}
			}
			else	// Download to just one module
			{
				Pixie_DSP_Memory_IO(Pixie_Devices[ModNum].DSP_Parameter_Values, DATA_MEMORY_ADDRESS, DSP_IO_BORDER, MOD_WRITE, ModNum);
			}
			
			// close opened files
			fclose(configfil);
		}
		else
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to open DSPParFile %s", DSPParFile);
			Pixie_Print_MSG(ErrMSG);
			return(-18);
		}
	}
	
	//**************************************************
	// Read DSP variable names from file
	//**************************************************
	
	if(ModNum == Number_Modules)	// Copy to all modules
	{
		// First read DSP variable indices for Module #0
		retval =  Pixie_Init_DSPVarAddress(DSPVarFile, 0);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): can't initialize DSP variable indices for module 0");
			Pixie_Print_MSG(ErrMSG);
			return(-19);
		}
		
		// Copy DSP variable indices from Module #0 to other modules
		for(k=1; k<Number_Modules; k++)
		{
			retval =  Pixie_Copy_DSPVarAddress(0, k);
			if(retval < 0)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): can't copy DSP variable indices from module 0 to module %d", k);
				Pixie_Print_MSG(ErrMSG);
				return(-20);
			}	
		}
	}
	else
	{
		// Only read DSP variable indices for Module #0
		if(ModNum == 0)
		{
			retval =  Pixie_Init_DSPVarAddress(DSPVarFile, ModNum);
			if(retval < 0)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): can't initialize DSP variable indices for module %d", ModNum);
				Pixie_Print_MSG(ErrMSG);
				return(-19);
			}
		}
		else // Copy DSP variable indices from Module #0 to other modules
		{
			retval =  Pixie_Copy_DSPVarAddress(0, ModNum);
			if(retval < 0)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): can't copy DSP variable indices from module 0 to module %d", ModNum);
				Pixie_Print_MSG(ErrMSG);
				return(-20);
			}	
		}
	}
	
	//**************************************************
	// Program Fippi
	//**************************************************
	if((APP16_TstBit(BOOTPATTERN_PROGFIPPI_BIT, BootPattern)) && (Offline == 0))
	{
		// Program Fippi in one or all modules
		if(ModNum == Number_Modules)	// Program Fippi in all modules
		{
			for(k=0; k<Number_Modules; k++)
			{
				retval = Pixie16ProgramFippi(k);
				if(retval < 0)
				{
					sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to program Fippi in module %d, retval=%d", k, retval);
					Pixie_Print_MSG(ErrMSG);
					return(-21);
				}
			}
		}
		else	// Program Fippi in just one module
		{
			retval = Pixie16ProgramFippi(ModNum);
			if(retval < 0)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to program Fippi in module %d, retval=%d", ModNum, retval);
				Pixie_Print_MSG(ErrMSG);
				return(-21);
			}
		}
	}
	
	//**************************************************
	// Set DACs
	//**************************************************
	if((APP16_TstBit(BOOTPATTERN_SETDACS_BIT, BootPattern)) && (Offline == 0))
	{
		// Set DACs in one or all modules
		if(ModNum == Number_Modules)	// Set DACs in all modules
		{
			for(k=0; k<Number_Modules; k++)
			{
				retval = Pixie16SetDACs(k);
				if(retval < 0)
				{
					sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to set DACs in module %d, retval=%d", k, retval);
					Pixie_Print_MSG(ErrMSG);
					return(-22);
				}
			}
		}
		else	// Set DACs in just one module
		{
			retval = Pixie16SetDACs(ModNum);
			if(retval < 0)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to set DACs in module %d, retval=%d", ModNum, retval);
				Pixie_Print_MSG(ErrMSG);
				return(-22);
			}
		}
	}

	//**************************************************
	// Set pullups for the trigger lines on the backplane
	//
	// Also enable connections of PXI nearest neighbor
	// lines (J2) onto the backplane if the module is
	// a Rev-B or C module
	//**************************************************
	
	if(ModNum == Number_Modules)	// Set modules all at once
	{
		for(k=0; k<Number_Modules; k++)
		{
			CPLD_CSR = 0xAAA;
			
			// Pull-up
			if( APP32_TstBit(MODCSRB_CPLDPULLUP, Pixie_Devices[k].DSP_Parameter_Values[ModCSRB_Address[k] - DATA_MEMORY_ADDRESS]) )
			{
				CPLD_CSR = APP32_SetBit(CPLDCSR_PULLUP, CPLD_CSR);
			}
			else
			{
				CPLD_CSR = APP32_ClrBit(CPLDCSR_PULLUP, CPLD_CSR);
			}
			
			// Enable connections of PXI nearest neighbor lines (J2) onto the
			// backplane if the module is a Rev-B or C module
			if ((Module_Information[k].Module_Rev == 0xB) || (Module_Information[k].Module_Rev == 0xC))
			{
				CPLD_CSR = APP32_SetBit(CPLDCSR_BPCONNECT, CPLD_CSR);
			}

			Pixie_Register_IO(k, CFG_CTRLCS, MOD_WRITE, &CPLD_CSR);
		}
	}
	else
	{
		// Set module by module
		
		CPLD_CSR = 0xAAA;
		
		// Pull-up
		if( APP32_TstBit(MODCSRB_CPLDPULLUP, Pixie_Devices[ModNum].DSP_Parameter_Values[ModCSRB_Address[ModNum] - DATA_MEMORY_ADDRESS]) )
		{
			CPLD_CSR = APP32_SetBit(CPLDCSR_PULLUP, CPLD_CSR);
		}
		else
		{
			CPLD_CSR = APP32_ClrBit(CPLDCSR_PULLUP, CPLD_CSR);
		}
		
		// Enable connections of PXI nearest neighbor lines (J2) onto the
		// backplane if the module is a Rev-B or C module
		if ((Module_Information[ModNum].Module_Rev == 0xB) || (Module_Information[ModNum].Module_Rev == 0xC))
		{
			CPLD_CSR = APP32_SetBit(CPLDCSR_BPCONNECT, CPLD_CSR);
		}

		Pixie_Register_IO(ModNum, CFG_CTRLCS, MOD_WRITE, &CPLD_CSR);
	}

	//**************************************************
	// Set pullups for the SYNCH lines on the backplane
	//**************************************************
	
	if(ModNum == Number_Modules)	// Set modules all at once
	{
		for(k=0; k<Number_Modules; k++)
		{
			Pixie_ReadCSR(k, &CSR);
			if( APP32_TstBit(MODCSRB_CHASSISMASTER, Pixie_Devices[k].DSP_Parameter_Values[ModCSRB_Address[k] - DATA_MEMORY_ADDRESS]) )
			{
				CSR = APP32_SetBit(PULLUP_CTRL, CSR);
			}
			else
			{
				CSR = APP32_ClrBit(PULLUP_CTRL, CSR);
			}
			Pixie_WrtCSR(k, CSR);
		}
	}
	else
	{
		// Set module by module
		Pixie_ReadCSR(ModNum, &CSR);
		if( APP32_TstBit(MODCSRB_CHASSISMASTER, Pixie_Devices[ModNum].DSP_Parameter_Values[ModCSRB_Address[ModNum] - DATA_MEMORY_ADDRESS]) )
		{
			CSR = APP32_SetBit(PULLUP_CTRL, CSR);
		}
		else
		{
			CSR = APP32_ClrBit(PULLUP_CTRL, CSR);
		}
		Pixie_WrtCSR(ModNum, CSR);
	}

	//**************************************************
	// Reset ADCs for RevF modules
	//**************************************************

	if(ModNum == Number_Modules)	// Set modules all at once
	{
		for(k=0; k<Number_Modules; k++)
		{
			if(Module_Information[k].Module_Rev == 0xF)
			{
				retval = Pixie_Control_Task_Run(k, RESET_ADC, 10000);
				if(retval == -1)
				{
					sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to start RESET_ADC run in module %d; retval=%d", k, retval);
					Pixie_Print_MSG(ErrMSG);
					return(-23);
				}
				else if(retval == -2)
				{
					sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): RESET_ADC run timed out in module %d; retval=%d", k, retval);
					Pixie_Print_MSG(ErrMSG);
					return(-24); // Time Out
				}
			}
		}
	}
	else
	{
		// Set module by module
		if(Module_Information[ModNum].Module_Rev == 0xF)
		{
				retval = Pixie_Control_Task_Run(ModNum,	RESET_ADC, 10000);
				if(retval == -1)
				{
					sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): failed to start RESET_ADC run in module %d; retval=%d", ModNum, retval);
					Pixie_Print_MSG(ErrMSG);
					return(-23);
				}
				else if(retval == -2)
				{
					sprintf(ErrMSG, "*ERROR* (Pixie16BootModule): RESET_ADC run timed out in module %d; retval=%d", ModNum, retval);
					Pixie_Print_MSG(ErrMSG);
					return(-24); // Time Out
				}
		}
	}

	return(0);  // success
}


/****************************************************************
*	Pixie16AcquireADCTrace:
*		Acquire ADC traces from a Pixie module.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Failed to start run
*			-3 - Acquiring ADC traces timed out
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AcquireADCTrace (
	unsigned short ModNum )    // module number
{
	int retval;    // return values
	unsigned int count;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if running in OFFLINE mode
	if( Offline == 1 )	  // Returns immediately for offline analysis
	{
		return(0);
	}

	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16AcquireADCTrace): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	retval = Pixie_Start_Run(NEW_RUN, 0, GET_TRACES, ModNum);
	if(retval < 0)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16AcquireADCTrace): failed to start run; retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	count=0;
	do
	{
#if PIXIE16_APPAPI_VER == PIXIE16_WINDOWS_APPAPI
		Sleep(1);
#elif PIXIE16_APPAPI_VER == PIXIE16_LINUX_APPAPI
		usleep(1000);
#endif
		
		retval = Pixie_Check_Run_Status(ModNum);
		count++;
	} while ( (retval !=  0) && (count < 10000)); // The maximal waiting time is 10 s
	
	if(count>=10000)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16AcquireADCTrace): acquiring ADC traces in module #%d timed out", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-3); // Time Out
	}
	
	return(0);
}


/****************************************************************
*	Pixie16ReadSglChanADCTrace:
*		Acquire ADC traces from a Pixie module.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Invalid Pixie channel number
*			-3 - Invalid trace length
*			-4 - Failed to allocate memory to store ADC traces
*			-5 - Failed to read ADC traces
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanADCTrace (
	unsigned short *Trace_Buffer,   // trace data
	unsigned int   Trace_Length,    // trace length
	unsigned short ModNum,          // module number
	unsigned short ChanNum )        // channel number
{
	unsigned int count;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	unsigned int *ADCData;
	int retval;
	
	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadSglChanADCTrace): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}

	// Check if ChanNum is valid
	if( ChanNum >= NUMBER_OF_CHANNELS )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadSglChanADCTrace): invalid Pixie channel number %d", ChanNum);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	// Check if Trace_Length is valid
	if( Trace_Length > MAX_ADC_TRACE_LEN )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadSglChanADCTrace): invalid ADC trace length %u for channel %d", Trace_Length, ChanNum);
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}
	
	// Allocate memory to store ADC trace data temporarily
	if( (ADCData = (unsigned int *)malloc(sizeof(unsigned int) * Trace_Length)) != NULL)
	{
		// Read out ADC trace for the requested channel
		retval = Pixie_DSP_Memory_IO(ADCData, (IO_BUFFER_ADDRESS + MAX_ADC_TRACE_LEN / 2 * ChanNum), (Trace_Length / 2), MOD_READ, ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16ReadSglChanADCTrace): reading ADC trace failed for module %d, retval=%d", ModNum, retval);
			Pixie_Print_MSG(ErrMSG);

			// Release memory
			free(ADCData);
			return(-5);
		}
		// Transfer to Trace_Buffer
		for(count = 0; count < (Trace_Length / 2); count ++)
		{
			Trace_Buffer[count*2] = (unsigned short) (ADCData[count] & 0x0000FFFF);
			Trace_Buffer[count*2+1] = (unsigned short) ((ADCData[count] & 0xFFFF0000) >> 16);
		}
		
		// Release memory
		free(ADCData);
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadSglChanADCTrace): failed to allocate memory to store ADC trace data for module %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-4);
	}
	
	return(0);
}


/****************************************************************
*	Pixie16IMbufferIO:
*		Read or write data from/to the DSP internal memory of
*		a Pixie module.
*
*		Return Value:
*			 0 - success
*			-1 - null pointer for buffer data
*			-2 - number of buffer words exceeds the limit
*			-3 - invalid DSP internal memory address
*			-4 - invalid I/O direction
*			-5 - invalid Pixie module number
*			-6 - I/O Failure
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16IMbufferIO (
	unsigned int   *Buffer,       // buffer data
	unsigned int   NumWords,      // number of buffer words to read or write
	unsigned int   Address,       // buffer address
	unsigned short Direction,     // I/O direction
	unsigned short ModNum )       // module number
{
	int retval;    // return values
	char ErrMSG[MAX_ERRMSG_LENGTH];
	unsigned int k;

	// Check if Buffer is a valid pointer
	if( Buffer == NULL )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16IMbufferIO): null pointer for buffer data");
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	// Check if NumWords is valid
	if( NumWords > (DSP_IMBUFFER_END_ADDR - DSP_IMBUFFER_START_ADDR + 1) )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16IMbufferIO): %u (number of buffer words to read or write) exceeds the limit %d", NumWords, (DSP_IMBUFFER_END_ADDR - DSP_IMBUFFER_START_ADDR + 1));
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	// Check if Address is valid
	if( (Address < DSP_IMBUFFER_START_ADDR) || (Address > DSP_IMBUFFER_END_ADDR) )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16IMbufferIO): invalid DSP internal memory address %u", Address);
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}
	
	// Check if Direction is valid
	if( (Direction != MOD_READ) && (Direction != MOD_WRITE) )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16IMbufferIO): invalid I/O direction %d", Direction);
		Pixie_Print_MSG(ErrMSG);
		return(-4);
	}
	
	// Check if ModNum is valid
	if( ModNum >= Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16IMbufferIO): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-5);
	}
	
	// Check if running in OFFLINE mode
	if( Offline == 1 )
	{
		// Only return parameter values stored in DSP_Parameter_Values for MOD_READ
		if( (Direction == MOD_READ) )
		{
			for(k=0; k<NumWords; k++)
			{
				Buffer[k] = Pixie_Devices[ModNum].DSP_Parameter_Values[Address + k - DATA_MEMORY_ADDRESS];
			}
		}
	}
	
	// I/O
	retval = Pixie_DSP_Memory_IO(Buffer, Address, NumWords, Direction, ModNum);
	if(retval < 0)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16IMbufferIO): IMbuffer I/O failed for module %d, retval=%d", ModNum, retval);
		Pixie_Print_MSG(ErrMSG);
		return(-6);
	}
	
	return(0);
}


/****************************************************************
*	Pixie16EMbufferIO:
*		Read or write data from/to the external memory of a
*		Pixie module.
*
*		Return Value:
*			 0 - success
*			-1 - null pointer for buffer data
*			-2 - number of buffer words exceeds the limit
*			-3 - invalid DSP external memory address
*			-4 - invalid I/O direction
*			-5 - invalid Pixie module number
*			-6 - I/O Failure
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16EMbufferIO (
	unsigned int   *Buffer,       // buffer data
	unsigned int   NumWords,      // number of buffer words to read or write
	unsigned int   Address,       // buffer address
	unsigned short Direction,     // I/O direction
	unsigned short ModNum )       // module number
{
	int retval;    // return values
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if Buffer is a valid pointer
	if( Buffer == NULL )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16EMbufferIO): null pointer for buffer data");
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	// Check if NumWords is valid
	if( NumWords > (DSP_EMBUFFER_END_ADDR - DSP_EMBUFFER_START_ADDR + 1) )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16EMbufferIO): %u (number of buffer words to read or write) exceeds the limit %d", NumWords, (DSP_EMBUFFER_END_ADDR - DSP_EMBUFFER_START_ADDR + 1));
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	// Check if Address is valid
	if( (Address < DSP_EMBUFFER_START_ADDR) || (Address > DSP_EMBUFFER_END_ADDR) )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16EMbufferIO): invalid DSP internal memory address %u", Address);
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}
	
	// Check if Direction is valid
	if( (Direction != MOD_READ) && (Direction != MOD_WRITE) )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16EMbufferIO): invalid I/O direction %d", Direction);
		Pixie_Print_MSG(ErrMSG);
		return(-4);
	}
	
	// Check if ModNum is valid
	if( ModNum >= Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16EMbufferIO): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-5);
	}
	
	// I/O
	retval = Pixie_Main_Memory_IO(Buffer, Address, NumWords, Direction, ModNum);
	if(retval < 0)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16EMbufferIO): EMbuffer I/O failed for module %d, retval=%d", ModNum, retval);
		Pixie_Print_MSG(ErrMSG);
		return(-6);
	}
	
	return(0);
}


/****************************************************************
*	Pixie16StartListModeRun:
*		Start or resume a list mode run.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Invalid mode
*			-3 - Failed to start list mode run
*			-4 - Invalid run type
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16StartListModeRun (
	unsigned short ModNum,    // module number
	unsigned short RunType,   // run type
	unsigned short mode )     // run mode
{
	int retval;    // return values
	char  ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if running in OFFLINE mode
	if( Offline == 1 )	  // Returns immediately for offline analysis
	{
		return(0);
	}

	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16StartListModeRun): Invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	if( (mode != NEW_RUN) && (mode != RESUME_RUN) )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16StartListModeRun): Invalid mode %d", mode);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	if( RunType != LIST_MODE_RUN )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16StartListModeRun): Invalid run type %d", mode);
		Pixie_Print_MSG(ErrMSG);
		return(-4);
	}

	// Start the list mode run now
	retval = Pixie_Start_Run(mode, RunType, 0, ModNum);
	if(retval < 0)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16StartListModeRun): failed to start list mode run, retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}
	
	return(0);
}


/****************************************************************
*	Pixie16StartHistogramRun:
*		Start or resume a histogram run.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Invalid mode
*			-3 - Failed to start histogram run
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16StartHistogramRun (
	unsigned short ModNum,    // module number
	unsigned short mode )     // run mode
{
	int retval;    // return values
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if running in OFFLINE mode
	if( Offline == 1 )	  // Returns immediately for offline analysis
	{
		return(0);
	}

	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16StartHistogramRun): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	if( (mode != NEW_RUN) && (mode != RESUME_RUN) )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16StartHistogramRun): Invalid mode %d", mode);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	// Start the histogram run now
	retval = Pixie_Start_Run(mode, HISTOGRAM_RUN, 0, ModNum);
	if(retval < 0)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16StartHistogramRun): failed to start histogram run, retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}
	
	return(0);
}


/****************************************************************
*	Pixie16CheckRunStatus function:
*		Poll the run status of a Pixie module.
*
*		Return Value:
*			 0 - No run is in progress
*			 1 - Run is still in progress
*			-1 - Invalid Pixie module number
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CheckRunStatus (
	unsigned short ModNum )    // Pixie module number  
{
	int retval;    // return values
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16CheckRunStatus): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	retval = Pixie_Check_Run_Status(ModNum);
	return(retval);
}


/****************************************************************
*	Pixie16EndRun:
*		Stop the run in a Pixie module.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Failed to stop the run
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16EndRun (
	unsigned short ModNum )    // Pixie module number  
{
	int retval;    // return values
	char ErrMSG[MAX_ERRMSG_LENGTH];
	unsigned short modulenumber, k;
	unsigned int   dummy, CSR;
	
	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16EndRun): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}

	// Stop run in the Director module (module #0) - a SYNC interrupt should be generated
	// to stop run in all modules simultaneously, if DSP parameter SynchWait is set to 1.
	// If SynchWait is set to 0, then CSR bit 0 is cleared to stop the run.
	modulenumber = 0;
	retval = Pixie_Register_IO(modulenumber, PCI_STOPRUN_REGADDR, MOD_WRITE, &dummy);

	if(ModNum == Number_Modules)  // Stop run in all modules
	{
		for(k = 0; k < Number_Modules; k ++)
		{
			// Read CSR and clear bit RUNENA to stop the run
			Pixie_ReadCSR(k, &CSR);
			CSR = APP32_ClrBit(RUNENA, CSR);
			Pixie_WrtCSR(k, CSR);
		}
	}
	else if(ModNum < Number_Modules)  // Stop run in one module only
	{		
		// Read CSR and clear bit RUNENA to stop the run
		Pixie_ReadCSR(ModNum, &CSR);
		CSR = APP32_ClrBit(RUNENA, CSR);
		Pixie_WrtCSR(ModNum, CSR);
	}

	return(0);
}


/****************************************************************
*	Run statistics functions
****************************************************************/

PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeInputCountRate (
	unsigned int   *Statistics,
	unsigned short ModNum,
	unsigned short ChanNum )
{
	double FastPeaks, LiveTime;
	
	// The fast peaks start after DSP_IO_BORDER
	FastPeaks = (double)Statistics[FastPeaksA_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS - DSP_IO_BORDER] * pow(2.0, 32.0);
	FastPeaks += (double)Statistics[FastPeaksB_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS - DSP_IO_BORDER];
	
	// The live times start after DSP_IO_BORDER
	LiveTime = (double)Statistics[LiveTimeA_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS - DSP_IO_BORDER] * pow(2.0, 32.0);
	LiveTime += (double)Statistics[LiveTimeB_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS - DSP_IO_BORDER];

	if(Module_Information[ModNum].Module_ADCMSPS == 100)
		LiveTime *= 1.0e-6 / (double)Module_Information[ModNum].Module_ADCMSPS;
	else if(Module_Information[ModNum].Module_ADCMSPS == 250)
		LiveTime *= 2.0 * 1.0e-6 / (double)Module_Information[ModNum].Module_ADCMSPS;
	else if(Module_Information[ModNum].Module_ADCMSPS == 500)
		LiveTime *= 5.0 * 1.0e-6 / (double)Module_Information[ModNum].Module_ADCMSPS;
	
	if(LiveTime == 0.0)
		return(0.0);
	else
		return( FastPeaks/LiveTime );
}

PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeOutputCountRate (
	unsigned int   *Statistics,
	unsigned short ModNum,
	unsigned short ChanNum )
{
	double ChanEvents, RealTime;
	
	// The channel processed events start after DSP_IO_BORDER
	ChanEvents = (double)Statistics[ChanEventsA_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS - DSP_IO_BORDER] * pow(2.0, 32.0);
	ChanEvents += (double)Statistics[ChanEventsB_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS - DSP_IO_BORDER];
	
	// The real times start after DSP_IO_BORDER
	RealTime = (double)Statistics[RunTimeA_Address[ModNum] - DATA_MEMORY_ADDRESS - DSP_IO_BORDER] * pow(2.0, 32.0);
	RealTime += (double)Statistics[RunTimeB_Address[ModNum] - DATA_MEMORY_ADDRESS - DSP_IO_BORDER];
	RealTime *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;
	
	if(RealTime == 0.0)
		return(0.0);
	else
		return( ChanEvents/RealTime );
}

PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeLiveTime (
	unsigned int   *Statistics,
	unsigned short ModNum,
	unsigned short ChanNum )
{
	double LiveTime;
	
	// The live times start after DSP_IO_BORDER
	LiveTime = (double)Statistics[LiveTimeA_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS - DSP_IO_BORDER] * pow(2.0, 32.0);
	LiveTime += (double)Statistics[LiveTimeB_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS - DSP_IO_BORDER];

	if(Module_Information[ModNum].Module_ADCMSPS == 100)
		LiveTime *= 1.0e-6 / (double)Module_Information[ModNum].Module_ADCMSPS;
	else if(Module_Information[ModNum].Module_ADCMSPS == 250)
		LiveTime *= 2.0 * 1.0e-6 / (double)Module_Information[ModNum].Module_ADCMSPS;
	else if(Module_Information[ModNum].Module_ADCMSPS == 500)
		LiveTime *= 5.0 * 1.0e-6 / (double)Module_Information[ModNum].Module_ADCMSPS;
	
	return( LiveTime );
}

PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeProcessedEvents (
	unsigned int   *Statistics,
	unsigned short ModNum )
{
	double ProcessedEvents;
	
	// The processed events start after DSP_IO_BORDER
	ProcessedEvents = (double)Statistics[NumEventsA_Address[ModNum] - DATA_MEMORY_ADDRESS - DSP_IO_BORDER] * pow(2.0, 32.0);
	ProcessedEvents += (double)Statistics[NumEventsB_Address[ModNum] - DATA_MEMORY_ADDRESS - DSP_IO_BORDER];
	
	return( ProcessedEvents );
}

PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeRealTime (
	unsigned int   *Statistics,
	unsigned short ModNum )
{
	double RealTime;
	
	// The real time starts after DSP_IO_BORDER
	RealTime = (double)Statistics[RunTimeA_Address[ModNum] - DATA_MEMORY_ADDRESS - DSP_IO_BORDER] * pow(2.0, 32.0);
	RealTime += (double)Statistics[RunTimeB_Address[ModNum] - DATA_MEMORY_ADDRESS - DSP_IO_BORDER];
	RealTime *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;
	
	return( RealTime );
}


/****************************************************************
*	Tools functions
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16complexFFT (
	double *data,
	unsigned int length )
{
	double tempR;
	double tempI;
	double theta;
	double wr;
	double wi;
	double wpr;
	double wpi;
	double wtemp;
	
	unsigned int nComplex;
	unsigned int m;
	unsigned int mmax;
	unsigned int iStep;
	unsigned int i, j;
	
	nComplex = 2 * length;
	
	// Do the bit reversal re-ordering
	j = 0;
	for (i = 0; i < nComplex; i += 2) 
	{
		if (j > i) 
		{
			tempR = data[j];
			tempI = data[j+1];
			data[j]   = data[i];
			data[j+1] = data[i+1];
			data[i]   = tempR;
			data[i+1] = tempI;
		}
		
		m = nComplex / 2;
		while ((m >= 2) && (j >= m)) 
		{
			j = j - m;
			m = m / 2;
		}
		
		j = j + m;
	}
	
	// Do the recursive FFT
	mmax = 2;
	while (nComplex > mmax)
	{
		iStep = 2 * mmax;
		theta = PI2 / mmax;
		wpr = sin(0.5 * theta);
		wpr = -2. * wpr * wpr;
		wpi = sin(theta);
		wr = 1.;
		wi = 0.;
		for (m = 0; m < mmax; m += 2)
		{
			for (i = m; i < nComplex; i += iStep)
			{
				j = i + mmax;
				tempR = wr * data[j]   - wi * data[j+1];
				tempI = wr * data[j+1] + wi * data[j];
				data[j]   = data[i]   - tempR;
				data[j+1] = data[i+1] - tempI;
				data[i]   = data[i]   + tempR;
				data[i+1] = data[i+1] + tempI;
			}
			wtemp = wr;
			wr = wr * wpr - wi * wpi + wr;
			wi = wi * wpr + wtemp * wpi + wi;
		}
		mmax = iStep;
	}
	
	return 0;
	
}

/****************************************************************
*	APP16_TstBit:
*		Test a bit in a 16-bit unsigned integer.
*
****************************************************************/

PIXIE16APP_EXPORT unsigned short PIXIE16APP_API APP16_TstBit (
	unsigned short bit,
	unsigned short value )
{
	return(((value & (unsigned short)(pow(2.0, (double)bit))) >> bit));
}

/****************************************************************
*	APP16_SetBit:
*		Set a bit in a 16-bit unsigned integer.
*
****************************************************************/

PIXIE16APP_EXPORT unsigned short PIXIE16APP_API APP16_SetBit (
	unsigned short bit,
	unsigned short value )
{
	return(value | (unsigned short)(pow(2.0, (double)bit)));
}

/****************************************************************
*	APP16_ClrBit:
*		Clear a bit in a 16-bit unsigned integer.
*
****************************************************************/

PIXIE16APP_EXPORT unsigned short PIXIE16APP_API APP16_ClrBit (
	unsigned short bit,
	unsigned short value )
{
	value = APP16_SetBit(bit, value);
	return(value ^ (unsigned short)(pow(2.0, (double)bit)));
}

/****************************************************************
*	APP32_SetBit:
*		Set a bit in a 32-bit unsigned integer.
*
****************************************************************/

PIXIE16APP_EXPORT unsigned int PIXIE16APP_API APP32_SetBit ( 
	unsigned short bit,
	unsigned int value )
{
	return(value | (unsigned int)(pow(2.0, (double)bit)));
}

/****************************************************************
*	APP32_ClrBit:
*		Clear a bit in a 32-bit unsigned integer.
*
****************************************************************/

PIXIE16APP_EXPORT unsigned int PIXIE16APP_API APP32_ClrBit (
	unsigned short bit,
	unsigned int value )
{
	value = APP32_SetBit(bit, value);
	return(value ^ (unsigned int)(pow(2.0, (double)bit)));
}

/****************************************************************
*	APP32_TstBit:
*		Test a bit in a 32-bit unsigned integer.
*
****************************************************************/

PIXIE16APP_EXPORT unsigned int PIXIE16APP_API APP32_TstBit (
	unsigned short bit,
	unsigned int value )
{
	return(((value & (unsigned int)(pow(2.0, (double)bit))) >> bit));
}


/****************************************************************
*	Control_Task_Run functions
****************************************************************/

/****************************************************************
*	Pixie16SetDACs:
*		Set all DACs.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Failed to start the SET_DACs run
*			-3 - SET_DACs run timed out
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SetDACs (
	unsigned short ModNum )
{
	int retval;
	unsigned int Max_Poll;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16SetDACs): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	Max_Poll = 10000;	// The maximal waiting time is 10 s
	retval = Pixie_Control_Task_Run(ModNum,	SET_DACS, Max_Poll);
	if(retval == -1)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16SetDACs): failed to start SET_DACS run; retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	else if(retval == -2)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16SetDACs): SET_DACS run timed out; retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-3); // Time Out
	}
	
	return(0); // Normal finish
}


/****************************************************************
*	Pixie16ProgramFippi:
*		Program Fippi.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Failed to start the PROGRAM_FIPPI run
*			-3 - PROGRAM_FIPPI run timed out
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ProgramFippi (
	unsigned short ModNum )
{
	int retval;
	unsigned int Max_Poll;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ProgramFippi): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	Max_Poll = 10000;	// The maximal waiting time is 10 s
	retval = Pixie_Control_Task_Run(ModNum,	PROGRAM_FIPPI, Max_Poll);
	if(retval == -1)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ProgramFippi): failed to start PROGRAM_FIPPI run; retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	else if(retval == -2)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ProgramFippi): PROGRAM_FIPPI run timed out; retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-3); // Time Out
	}
	
	return(0); // Normal finish
	
}


/****************************************************************
*	Pixie16AdjustOffsets:
*		Adjust DC-Offsets.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Failed to start the ADJUST_OFFSETS run
*			-3 - ADJUST_OFFSETS run timed out
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AdjustOffsets (
	unsigned short ModNum )
{
	int retval;
	unsigned int Max_Poll;
	char ErrMSG[MAX_ERRMSG_LENGTH];

	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16AdjustOffsets): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	Max_Poll = 10000;	// The maximal waiting time is 10 s
	retval = Pixie_Control_Task_Run(ModNum,	ADJUST_OFFSETS, Max_Poll);
	if(retval == -1)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16AdjustOffsets): failed to start ADJUST_OFFSETS run; retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	else if(retval == -2)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16AdjustOffsets): ADJUST_OFFSETS run timed out; retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-3); // Time Out
	}
	
	return(0); // Normal finish
	
}


/****************************************************************
*	Pixie16AcquireBaselines:
*		Acquire baselines.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Failed to start the GET_BASELINES run
*			-3 - GET_BASELINES run timed out
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AcquireBaselines (
	unsigned short ModNum )    // module number
{
	int retval;
	unsigned int Max_Poll, sfr;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16AcquireBaselines): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	// Get the SlowFilteRange value
	sfr = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
	
	// Compute Max_Poll, in ms
	// Factor 2 added to avoid time out
	Max_Poll = 2 * (unsigned int)(2.0 * (double)SLOWFILTER_MAX_LEN * pow(2.0, (double)sfr) * (double)MAX_NUM_BASELINES / (double)DSP_CLOCK_MHZ / 1000.0);
	
	// Need extra delay if SlowFilterRange is lower than 3 due to GetBaselines overhead in the DSP
	if(sfr <= 2)
	{
		Max_Poll = (unsigned int)((double)Max_Poll * pow(2.0, (3.0 - (double)sfr)));
	}

	// Acquire the baselines
	retval = Pixie_Control_Task_Run(ModNum,	GET_BASELINES, Max_Poll);
	if(retval == -1)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16AcquireBaselines): failed to start GET_BASELINES run; retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	else if(retval == -2)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16AcquireBaselines): GET_BASELINES run timed out; retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-3); // Time Out
	}
	
	return(0); // Normal finish
}


/****************************************************************
*	Pixie16ReadSglChanBaselines:
*		Read baselines for a given channel.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Requested number of baselines exceeded the limit
*			-3 - Failed to allocate memory to store baselines
*			-4 - Failed to read baselines 
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanBaselines (
	double *Baselines,          // returned baselines values
	double *TimeStamps,         // time stamp for each baseline value
	unsigned short NumBases,    // number of baseline values to read
	unsigned short ModNum,      // module number
	unsigned short ChanNum )    // channel number
{
	unsigned int k;
	unsigned int *buffer;
	double startbasetime;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	int retval;
	
	// Check if ModNum is valid
	if( ModNum >= Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadSglChanBaselines): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	// Check if NumBases is within limit
	if( NumBases > MAX_NUM_BASELINES )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadSglChanBaselines): requested number of baselines (%d) exceeded the limit (%d)", NumBases, MAX_NUM_BASELINES);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	// Allocate memory
	if( (buffer = (unsigned int *)malloc(sizeof(unsigned int) * IO_BUFFER_LENGTH)) != NULL)
	{
		
		retval = Pixie_DSP_Memory_IO(buffer, IO_BUFFER_ADDRESS, IO_BUFFER_LENGTH, MOD_READ, ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16ReadSglChanBaselines): reading baselines failed for module %d, retval=%d", ModNum, retval);
			Pixie_Print_MSG(ErrMSG);

			// Release memory
			free(buffer);
			return(-4);
		}
		
		// Compute timestamp at the beginning of the baseline capture
		startbasetime = (double)buffer[0] * pow(2.0, 32.0);
		startbasetime += (double)buffer[1];
		startbasetime *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;
		
		// Convert the baselines for the selected channel
		for(k=0; k<NumBases; k++)
		{
			TimeStamps[k] = (double)buffer[2 + k * BASELINES_BLOCK_LEN] * pow(2.0, 32.0);
			TimeStamps[k] += (double)buffer[2 + k * BASELINES_BLOCK_LEN + 1];
			TimeStamps[k] *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;
			TimeStamps[k] -= startbasetime;
			
			Baselines[k] = IEEEFloating2Decimal(buffer[2 + k * BASELINES_BLOCK_LEN + 2 + ChanNum]);
		}
		
		// Release memory
		free(buffer);
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadSglChanBaselines): failed to allocate memory to store baselines for module %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}
	
	return(0); // Normal finish
	
}


/****************************************************************
*	Pixie16RampOffsetDACs:
*		Ramp OffsetDACs.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Null pointer *DCValues
*			-3 - requested number of DC values exceeded the limit
*			-4 - Failed to start the RAMP_OFFSETDACS run
*			-5 - RAMP_OFFSETDACS run timed out
*			-6 - Failed to read DC values
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16RampOffsetDACs (
	double *DCValues,          // returned DC offset values
	unsigned short NumDCVals,  // number of DC values to read
	unsigned short ModNum )
{
	int retval;
	unsigned int Max_Poll, k;
	unsigned int buffer[MAX_NUM_DCVALUES];
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if ModNum is valid
	if( ModNum >= Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16RampOffsetDACs): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}

	// Check if DCValues is valid
	if(DCValues == NULL)
	{
		sprintf(ErrMSG, "*Error* (Pixie16RampOffsetDACs): Null pointer *DCValues");
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}

	// Check if NumDCVals is within limit
	if( NumDCVals > MAX_NUM_DCVALUES )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16RampOffsetDACs): requested number of DC values (%d) exceeded the limit (%d)", NumDCVals, MAX_NUM_DCVALUES);
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}

	Max_Poll = 10000;	// The maximal waiting time is 10 s
	retval = Pixie_Control_Task_Run(ModNum,	RAMP_OFFSETDACS, Max_Poll);
	if(retval == -1)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16RampOffsetDACs): failed to start RAMP_OFFSETDACS run; retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-4);
	}
	else if(retval == -2)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16RampOffsetDACs): RAMP_OFFSETDACS run timed out; retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-5); // Time Out
	}
	
	// Read out data memory
	retval = Pixie_DSP_Memory_IO(buffer, IO_BUFFER_ADDRESS, MAX_NUM_DCVALUES, MOD_READ, ModNum);
	if(retval < 0)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16RampOffsetDACs): reading OffsetDAC values failed for module %d, retval=%d", ModNum, retval);
		Pixie_Print_MSG(ErrMSG);
		return(-6);
	}
		
	// Convert DC values
	for(k = 0; k < NumDCVals; k++)
	{
		*DCValues++ = IEEEFloating2Decimal(buffer[k]);
	}

	return(0); // Normal finish
	
}


/****************************************************************
*	Pixie16ControlTaskRun:
*		Perform a control task run.
*
*		Return Value:
*			 0 - successful
*			-1 - Invalid Pixie module number
*			-2 - control task run failed
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ControlTaskRun (
	unsigned short ModNum,        // Pixie module number
	unsigned short ControlTask,   // Control task number
	unsigned int Max_Poll )       // Timeout control in unit of ms for control task run
{
	int retval;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ControlTaskRun): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	retval = Pixie_Control_Task_Run(ModNum,	ControlTask, Max_Poll);
	if(retval < 0)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ControlTaskRun): control task run %d failed", ControlTask);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	return(0);
}


/****************************************************************
*	Pixie16BLcutFinder:
*		Find the BLcut value for the selected channel and return it
*		using pointer *BLcut.
*
*		Return Value:
*			 0 - successful
*			-1 - failed to collect baselines
*			-2 - failed to read baselines from the data memory
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16BLcutFinder (
	unsigned short ModNum,    // Pixie module number
	unsigned short ChanNum,   // Pixie channel number
	unsigned int *BLcut )     // BLcut return value
{
	unsigned int value, KeepLog, KeepBLCut, localBlCut, LC, k;
	double baseline[MAX_NUM_BASELINES], ts[MAX_NUM_BASELINES];
	double sdev, sdevCount, BLsigma, val;
	int retval;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	/*****************************************************
	*
	*	Set proper DSP parameters for collecting baselines 
	*
	*****************************************************/
	
	// Store the DSP parameter Log2BWeight value
	KeepLog = Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Bweight_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
	// Temporarily set Log2BWeight to 0
	value = 0;
	Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Bweight_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = value;
	Pixie16IMbufferIO(&value, 1, (unsigned int)(Log2Bweight_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
	
	// Store the DSP parameter BLCut value
	KeepBLCut = Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
	// Temporarily set BLcut to 0
	value = 0;
	Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = value;
	Pixie16IMbufferIO(&value, 1, (unsigned int)(BLcut_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
	
	/*****************************************************
	*
	*	Start to collect baselines 
	*
	*****************************************************/
	
	sdev=0.0;
	sdevCount=0.0;
	LC=0;
	localBlCut = 0;
	
	do
	{
		// Start Control Task COLLECT_BASES to collect baselines
		retval = Pixie16AcquireBaselines(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16BLcutFinder): failed to collect baselines in module %d", ModNum);
			Pixie_Print_MSG(ErrMSG);
			
			// Restore DSP parameter Log2BWeight
			Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Bweight_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = KeepLog;
			Pixie16IMbufferIO(&KeepLog, 1, (unsigned int)(Log2Bweight_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
			
			// Restore DSP parameter BLCut
			Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = KeepBLCut;
			Pixie16IMbufferIO(&KeepBLCut, 1, (unsigned int)(BLcut_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
			
			return(-1); // failed to collect baselines
		}
		
		// Read the baselines from the data memory
		retval = Pixie16ReadSglChanBaselines(baseline, ts, MAX_NUM_BASELINES, ModNum, ChanNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16BLcutFinder): failed to read baselines in module %d channel %d", ModNum, ChanNum);
			Pixie_Print_MSG(ErrMSG);
			
			return(-2); // failed to read baselines from the data memory
		}
		
		for(k=0; k<(MAX_NUM_BASELINES-1); k+=2)
		{
			val=fabs(baseline[k]-baseline[k+1]);
			
			if(val!=0)
			{
				// We need a safety check here
				if((val < (10.0*baseline[k])) && (val < (10.0*baseline[k+1])))
				{
					if(localBlCut==0)
					{
						sdev+=val;
						sdevCount+=1.0;
					}
					else
					{
						if(val<localBlCut)
						{
							sdev+=val;
							sdevCount+=1.0;
						}
					}
				}
			}
		}
		
		LC+=1;
		if(LC>10) 
			break;
		
	}while(sdevCount < 1000);
	
	// Calculate BLcut
	if(sdevCount > 0)
	{
		BLsigma=sdev*sqrt(PI/2)/sdevCount;
		localBlCut=(unsigned int)floor(8.0*BLsigma);
	}
	else
	{
		localBlCut = 0;
	}
	
	// Write the new BLCut value to the DSP
	Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = localBlCut;
	Pixie16IMbufferIO(&localBlCut, 1, (unsigned int)(BLcut_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
	
	
	/*****************************************************
	*
	*	Collect baselines again using the newly calculated BLcut
	*
	*****************************************************/
	
	sdev=0.0;
	sdevCount=0.0;
	LC=0;
	localBlCut = 0;
	
	do
	{
		// Start Control Task COLLECT_BASES to collect baselines
		retval = Pixie16AcquireBaselines(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16BLcutFinder): failed to collect baselines in module %d", ModNum);
			Pixie_Print_MSG(ErrMSG);
			
			// Restore DSP parameter Log2BWeight
			Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Bweight_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = KeepLog;
			Pixie16IMbufferIO(&KeepLog, 1, (unsigned int)(Log2Bweight_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
			
			// Restore DSP parameter BLCut
			Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = KeepBLCut;
			Pixie16IMbufferIO(&KeepBLCut, 1, (unsigned int)(BLcut_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
			
			return(-1); // failed to collect baselines
		}
		
		// Read the baselines from the data memory
		retval = Pixie16ReadSglChanBaselines(baseline, ts, MAX_NUM_BASELINES, ModNum, ChanNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16BLcutFinder): failed to read baselines in module %d channel %d", ModNum, ChanNum);
			Pixie_Print_MSG(ErrMSG);
			
			return(-2); // failed to read baselines from the data memory
		}
		
		for(k=0; k<(MAX_NUM_BASELINES-1); k+=2)
		{
			val=fabs(baseline[k]-baseline[k+1]);
			
			if(val!=0)
			{
				// We need a safety check here
				if((val < (10.0*baseline[k])) && (val < (10.0*baseline[k+1])))
				{
					if(localBlCut==0)
					{
						sdev+=val;
						sdevCount+=1.0;
					}
					else
					{
						if(val<localBlCut)
						{
							sdev+=val;
							sdevCount+=1.0;
						}
					}
				}
			}
		}
		
		LC+=1;
		if(LC>10) 
			break;
		
	}while(sdevCount < 1000);
	
	// Calculate BLcut
	if(sdevCount > 0)
	{
		BLsigma=sdev*sqrt(PI/2)/sdevCount;
		localBlCut=(unsigned int)floor(8.0*BLsigma);
	}
	else
	{
		localBlCut = 0;
	}
	
	// Write the new BLCut value to the DSP
	Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = localBlCut;
	Pixie16IMbufferIO(&localBlCut, 1, (unsigned int)(BLcut_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
	
	/*****************************************************
	*
	*	Restore several DSP parameters
	*
	*****************************************************/
	
	// Restore DSP parameter Log2BWeight
	Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Bweight_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = KeepLog;
	Pixie16IMbufferIO(&KeepLog, 1, (unsigned int)(Log2Bweight_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
	
	// Update BLcut return value
	*BLcut = localBlCut;
	
	return(0);
}


/****************************************************************
*	Pixie16TauFinder:
*		Find the exponential decay constant of the detector/preamplifier
*		signals connected to a Pixie module.
*			
*		Tau is used to return the newly found Tau values of 16 channels, and
*		should be a 16-element array in double data format. A '-1.0' Tau value
*		for a channel means the Tau_Finder was not successful for such a channel.
*
*		Return Value:
*			 0 - success
*			-1 - invalid Pixie module number
*			-2 - failed to start TAU_FINDER run
*			-3 - TAU_FINDER run timed out
*			-5 - failed to find sufficient number of pulses
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16TauFinder (
	unsigned short ModNum,   // Pixie module number
	double         *Tau )    // 16 returned Tau values, in µs
{

	char           ErrMSG[MAX_ERRMSG_LENGTH];
	unsigned int   Max_Poll;
	unsigned int   autotau[NUMBER_OF_CHANNELS];
	unsigned short k;
	int            retval;

	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16TauFinder): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	Max_Poll = 10000;	// The maximal waiting time is 10 s
	retval = Pixie_Control_Task_Run(ModNum,	TAU_FINDER, Max_Poll);
	if(retval == -1)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16TauFinder): failed to start TAU_FINDER run; retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	else if(retval == -2)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16TauFinder): TAU_FINDER run timed out; retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-3); // Time Out
	}

	// Read back the tau values found by the DSP
	Pixie16IMbufferIO(autotau, NUMBER_OF_CHANNELS, AutoTau_Address[ModNum], MOD_READ, ModNum);

	// Return found tau values to the calling function
	for(k = 0; k < NUMBER_OF_CHANNELS; k++)
	{
		if(autotau[k] == 0xFFFFFFFF)	// 0xFFFFFFFF indicates unsuccessful tau finding for this channel
		{
			*Tau++ = -1.0;
		}
		else
		{
			*Tau++ = IEEEFloating2Decimal(autotau[k]);
		}
	}

	return(0);
}

/****************************************************************
*	Pixie16WriteSglModPar:
*		Write a module parameter to a Pixie module.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Invalid module parameter name
*			-3 - Failed to program Fippi after downloading module parameter
*			-4 - Failed to find BLcut after downloading module parameter
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteSglModPar (
	char *ModParName,          // the name of the module parameter
	unsigned int   ModParData, // the module parameter value to be written to the module
	unsigned short ModNum )    // module number
{
	unsigned short k;
	unsigned int paflength, triggerdelay, tracedelay, fastfilterrange, LastFastFilterRange, baselinecut;
	unsigned int CSR, slowfilterrange;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	int retval;
	unsigned int CPLD_CSR;
	
	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglModPar): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	// Write module parameter
	
	if(strcmp(ModParName,"MODULE_CSRA") == 0)
	{
		// Update the DSP parameter ModCSRA
		Pixie_Devices[ModNum].DSP_Parameter_Values[ModCSRA_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&ModParData, 1, ModCSRA_Address[ModNum], MOD_WRITE, ModNum);
		
		// Program FiPPI to apply ModCSRA settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglModPar): ProgramFippi failed after downloading ModCSRA, retval=%d", retval);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}
	}
	else if(strcmp(ModParName,"MODULE_CSRB") == 0)
	{
		// Update the DSP parameter ModCSRB
		Pixie_Devices[ModNum].DSP_Parameter_Values[ModCSRB_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&ModParData, 1, ModCSRB_Address[ModNum], MOD_WRITE, ModNum);

		// Program FiPPI to apply ModCSRB settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglModPar): ProgramFippi failed after downloading ModCSRB, retval=%d", retval);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}

		//**************************************************
		// Set up Pull-up resistors
		//**************************************************
		
		CPLD_CSR = 0xAAA;

		// Pull-up
		if( APP32_TstBit(MODCSRB_CPLDPULLUP, ModParData) )
		{
			CPLD_CSR = APP32_SetBit(CPLDCSR_PULLUP, CPLD_CSR);
		}
		else
		{
			CPLD_CSR = APP32_ClrBit(CPLDCSR_PULLUP, CPLD_CSR);
		}

		// Enable connections of PXI nearest neighbor lines (J2) onto the
		// backplane if the module is a Rev-B or C module
		if ((Module_Information[ModNum].Module_Rev == 0xB) || (Module_Information[ModNum].Module_Rev == 0xC))
		{
			CPLD_CSR = APP32_SetBit(CPLDCSR_BPCONNECT, CPLD_CSR);
		}

		Pixie_Register_IO(ModNum, CFG_CTRLCS, MOD_WRITE, &CPLD_CSR);

		//**************************************************
		// Set pullups for the SYNCH lines on the backplane
		//**************************************************
	
		Pixie_ReadCSR(ModNum, &CSR);
		if( APP32_TstBit(MODCSRB_CHASSISMASTER, ModParData) )
		{
			CSR = APP32_SetBit(PULLUP_CTRL, CSR);
		}
		else
		{
			CSR = APP32_ClrBit(PULLUP_CTRL, CSR);
		}
		Pixie_WrtCSR(ModNum, CSR);

	}
	else if(strcmp(ModParName,"MODULE_FORMAT") == 0)
	{
		// Update the DSP parameter ModFormat
		Pixie_Devices[ModNum].DSP_Parameter_Values[ModFormat_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&ModParData, 1, ModFormat_Address[ModNum], MOD_WRITE, ModNum);
	}
	else if(strcmp(ModParName,"MAX_EVENTS") == 0)
	{
		// Update the DSP parameter MaxEvents
		Pixie_Devices[ModNum].DSP_Parameter_Values[MaxEvents_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&ModParData, 1, MaxEvents_Address[ModNum], MOD_WRITE, ModNum);
	}
	else if(strcmp(ModParName,"SYNCH_WAIT") == 0)
	{
		// Update the DSP parameter SynchWait
		Pixie_Devices[ModNum].DSP_Parameter_Values[SynchWait_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&ModParData, 1, SynchWait_Address[ModNum], MOD_WRITE, ModNum);
		
		// When changing SynchWait in one Pixie module, we need to broadcast it to all other modules as well
		Pixie_Broadcast("SYNCH_WAIT", ModNum);
	}
	else if(strcmp(ModParName,"IN_SYNCH") == 0)
	{
		// Update the DSP parameter InSynch
		Pixie_Devices[ModNum].DSP_Parameter_Values[InSynch_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&ModParData, 1, InSynch_Address[ModNum], MOD_WRITE, ModNum);
		
		// When changing InSynch in one Pixie module, we need to broadcast it to all other modules as well
		Pixie_Broadcast("IN_SYNCH", ModNum);
	}
	else if(strcmp(ModParName,"SLOW_FILTER_RANGE") == 0)
	{
		// Check slow filter range limit
		slowfilterrange = ModParData;

		if(slowfilterrange > SLOWFILTERRANGE_MAX)
		{
			slowfilterrange = SLOWFILTERRANGE_MAX;

			sprintf(ErrMSG, "*WARNING* (Pixie16WriteSglModPar): Maximum SlowFilterRange that is currently supported is %d", slowfilterrange);
			Pixie_Print_MSG(ErrMSG);
		}
		if(slowfilterrange < SLOWFILTERRANGE_MIN)
		{
			slowfilterrange = SLOWFILTERRANGE_MIN;

			sprintf(ErrMSG, "*WARNING* (Pixie16WriteSglModPar): Minimum SlowFilterRange that is currently supported is %d", slowfilterrange);
			Pixie_Print_MSG(ErrMSG);
		}
		
		// Update the DSP parameter SlowFilterRange
		Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] = slowfilterrange;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&slowfilterrange, 1, SlowFilterRange_Address[ModNum], MOD_WRITE, ModNum);
		
		/*******************************************/
		//	We need to recompute FIFO settings
		/*******************************************/
		fastfilterrange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		for(k=0; k<NUMBER_OF_CHANNELS; k++)
		{
			// Get the current TraceDelay
			paflength = Pixie_Devices[ModNum].DSP_Parameter_Values[PAFlength_Address[ModNum] + k - DATA_MEMORY_ADDRESS];
			triggerdelay = Pixie_Devices[ModNum].DSP_Parameter_Values[TriggerDelay_Address[ModNum] + k - DATA_MEMORY_ADDRESS];
			tracedelay = paflength - (unsigned int)((double)triggerdelay / pow(2.0, (double)fastfilterrange));
			
			// Update FIFO settings (TriggerDelay and PAFLength)
			Pixie_ComputeFIFO(tracedelay, ModNum, k);
		}
		
		// Program FiPPI to apply settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglModPar): ProgramFippi failed after downloading SlowFilterRange, retval=%d", retval);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}
		
		/*******************************************/
		//	We also need to update baseline cut value
		/*******************************************/
		for(k=0; k<NUMBER_OF_CHANNELS; k++)
		{
			retval = Pixie16BLcutFinder(ModNum, k, &baselinecut);
			if(retval < 0)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglModPar): BLcutFinder failed in module %d after downloading SlowFilterRange, retval=%d", k, retval);
				Pixie_Print_MSG(ErrMSG);
				return(-4);
			}
		}
	}
	else if(strcmp(ModParName,"FAST_FILTER_RANGE") == 0)
	{
		// Get the last fast filter range
		LastFastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		
		// Check fast filter range limit
		fastfilterrange = ModParData;

		if(fastfilterrange > FASTFILTERRANGE_MAX)
		{
			fastfilterrange = FASTFILTERRANGE_MAX;

			sprintf(ErrMSG, "*WARNING* (Pixie16WriteSglModPar): Maximum FastFilterRange that is currently supported is %d", fastfilterrange);
			Pixie_Print_MSG(ErrMSG);
		}
		if(fastfilterrange < FASTFILTERRANGE_MIN)
		{
			fastfilterrange = FASTFILTERRANGE_MIN;

			sprintf(ErrMSG, "*WARNING* (Pixie16WriteSglModPar): Minimum FastFilterRange that is currently supported is %d", fastfilterrange);
			Pixie_Print_MSG(ErrMSG);
		}
		
		// Update the new DSP parameter FastFilterRange
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] = fastfilterrange;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&fastfilterrange, 1, FastFilterRange_Address[ModNum], MOD_WRITE, ModNum);
		
		/*******************************************/
		//	We need to recompute FIFO settings
		/*******************************************/
		for(k=0; k<NUMBER_OF_CHANNELS; k++)
		{
			// Get the current TraceDelay
			paflength = Pixie_Devices[ModNum].DSP_Parameter_Values[PAFlength_Address[ModNum] + k - DATA_MEMORY_ADDRESS];
			triggerdelay = Pixie_Devices[ModNum].DSP_Parameter_Values[TriggerDelay_Address[ModNum] + k - DATA_MEMORY_ADDRESS];
			tracedelay = paflength - (unsigned int)((double)triggerdelay / pow(2.0, (double)LastFastFilterRange));
			
			// Update FIFO settings (TriggerDelay and PAFLength)
			Pixie_ComputeFIFO(tracedelay, ModNum, k);
		}
		
		// Program FiPPI to apply settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglModPar): ProgramFippi failed after downloading FastFilterRange, retval=%d", retval);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}
	}
	else if(strcmp(ModParName,"FastTrigBackplaneEna") == 0)
	{		
		// Update the new DSP parameter FastTrigBackplaneEna
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastTrigBackplaneEna_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&ModParData, 1, FastTrigBackplaneEna_Address[ModNum], MOD_WRITE, ModNum);
		
		// Program FiPPI to apply settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglModPar): ProgramFippi failed after downloading FastTrigBackplaneEna, retval=%d", retval);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}			
	}
	else if(strcmp(ModParName,"CrateID") == 0)
	{		
		// Update the new DSP parameter CrateID
		Pixie_Devices[ModNum].DSP_Parameter_Values[CrateID_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&ModParData, 1, CrateID_Address[ModNum], MOD_WRITE, ModNum);	

		// Program FiPPI to apply settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglModPar): ProgramFippi failed after downloading CrateID, retval=%d", retval);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}
	}	
	else if(strcmp(ModParName,"SlotID") == 0)
	{		
		// Update the new DSP parameter SlotID
		Pixie_Devices[ModNum].DSP_Parameter_Values[SlotID_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&ModParData, 1, SlotID_Address[ModNum], MOD_WRITE, ModNum);	

		// Program FiPPI to apply settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglModPar): ProgramFippi failed after downloading SlotID, retval=%d", retval);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}
	}	
	else if(strcmp(ModParName,"ModID") == 0)
	{		
		// Update the new DSP parameter ModID
		Pixie_Devices[ModNum].DSP_Parameter_Values[ModID_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&ModParData, 1, ModID_Address[ModNum], MOD_WRITE, ModNum);	

		// Program FiPPI to apply settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglModPar): ProgramFippi failed after downloading ModID, retval=%d", retval);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}
	}
	else if(strcmp(ModParName,"TrigConfig0") == 0)
	{		
		// Update the new DSP parameter TrigConfig0
		Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&ModParData, 1, TrigConfig_Address[ModNum], MOD_WRITE, ModNum);
		
		// Program FiPPI to apply settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglModPar): ProgramFippi failed after downloading TrigConfig0, retval=%d", retval);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}			
	}
	else if(strcmp(ModParName,"TrigConfig1") == 0)
	{		
		// Update the new DSP parameter TrigConfig1
		Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 1 - DATA_MEMORY_ADDRESS] = ModParData;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&ModParData, 1, TrigConfig_Address[ModNum] + 1, MOD_WRITE, ModNum);
		
		// Program FiPPI to apply settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglModPar): ProgramFippi failed after downloading TrigConfig1, retval=%d", retval);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}			
	}
	else if(strcmp(ModParName,"TrigConfig2") == 0)
	{		
		// Update the new DSP parameter TrigConfig2
		Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 2 - DATA_MEMORY_ADDRESS] = ModParData;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&ModParData, 1, TrigConfig_Address[ModNum] + 2, MOD_WRITE, ModNum);
		
		// Program FiPPI to apply settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglModPar): ProgramFippi failed after downloading TrigConfig2, retval=%d", retval);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}			
	}
	else if(strcmp(ModParName,"TrigConfig3") == 0)
	{		
		// Update the new DSP parameter TrigConfig3
		Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 3 - DATA_MEMORY_ADDRESS] = ModParData;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&ModParData, 1, TrigConfig_Address[ModNum] + 3, MOD_WRITE, ModNum);
		
		// Program FiPPI to apply settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglModPar): ProgramFippi failed after downloading TrigConfig3, retval=%d", retval);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}			
	}
	else if(strcmp(ModParName,"HOST_RT_PRESET") == 0)
	{		
		// Update the new DSP parameter HRTP
		Pixie_Devices[ModNum].DSP_Parameter_Values[HRTP_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&ModParData, 1, HRTP_Address[ModNum], MOD_WRITE, ModNum);

		// When changing HostRunTimePreset in one Pixie module, we need to broadcast it to all other modules as well
		Pixie_Broadcast("HOST_RT_PRESET", ModNum);
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglModPar): invalid module parameter name %s", ModParName);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	return(0);
}


/****************************************************************
*	Pixie16ReadSglModPar:
*		Read a module parameter from a Pixie module.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Invalid module parameter name
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglModPar (
	char *ModParName,           // the name of the module parameter
	unsigned int   *ModParData, // the module parameter value to be read from the module
	unsigned short ModNum )     // module number
{
	
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if ModNum is valid
	if( ModNum >= Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadSglModPar): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	// Read module parameter
	
	if(strcmp(ModParName,"MODULE_NUMBER") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, ModNum_Address[ModNum], MOD_READ, ModNum);
		
		// Update module parameter ModNum
		Pixie_Devices[ModNum].DSP_Parameter_Values[ModNum_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
	}
	else if(strcmp(ModParName,"MODULE_CSRA") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, ModCSRA_Address[ModNum], MOD_READ, ModNum);
		
		// Update module parameter ModCSRA
		Pixie_Devices[ModNum].DSP_Parameter_Values[ModCSRA_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
	}
	else if(strcmp(ModParName,"MODULE_CSRB") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, ModCSRB_Address[ModNum], MOD_READ, ModNum);
		
		// Update module parameter ModCSRB
		Pixie_Devices[ModNum].DSP_Parameter_Values[ModCSRB_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
	}
	else if(strcmp(ModParName,"MODULE_FORMAT") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, ModFormat_Address[ModNum], MOD_READ, ModNum);
		
		// Update module parameter ModFormat
		Pixie_Devices[ModNum].DSP_Parameter_Values[ModFormat_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
	}
	else if(strcmp(ModParName,"MAX_EVENTS") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, MaxEvents_Address[ModNum], MOD_READ, ModNum);
		
		// Update module parameter MaxEvents
		Pixie_Devices[ModNum].DSP_Parameter_Values[MaxEvents_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
	}
	else if(strcmp(ModParName,"SYNCH_WAIT") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, SynchWait_Address[ModNum], MOD_READ, ModNum);
		
		// Update module parameter SynchWait
		Pixie_Devices[ModNum].DSP_Parameter_Values[SynchWait_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
	}
	else if(strcmp(ModParName,"IN_SYNCH") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, InSynch_Address[ModNum], MOD_READ, ModNum);
		
		// Update module parameter InSynch
		Pixie_Devices[ModNum].DSP_Parameter_Values[InSynch_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
	}
	else if(strcmp(ModParName,"SLOW_FILTER_RANGE") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, SlowFilterRange_Address[ModNum], MOD_READ, ModNum);
		
		// Update module parameter SlowFilterRange
		Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
	}
	else if(strcmp(ModParName,"FAST_FILTER_RANGE") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, FastFilterRange_Address[ModNum], MOD_READ, ModNum);
		
		// Update module parameter FastFilterRange
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
	}
	else if(strcmp(ModParName,"FastTrigBackplaneEna") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, FastTrigBackplaneEna_Address[ModNum], MOD_READ, ModNum);
		
		// Update module parameter FastTrigBackplaneEna
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastTrigBackplaneEna_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
	}	
	else if(strcmp(ModParName,"CrateID") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, CrateID_Address[ModNum], MOD_READ, ModNum);
		
		// Update module parameter CrateID
		Pixie_Devices[ModNum].DSP_Parameter_Values[CrateID_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
	}	
	else if(strcmp(ModParName,"SlotID") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, SlotID_Address[ModNum], MOD_READ, ModNum);
		
		// Update module parameter SlotID
		Pixie_Devices[ModNum].DSP_Parameter_Values[SlotID_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
	}	
	else if(strcmp(ModParName,"ModID") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, ModID_Address[ModNum], MOD_READ, ModNum);
		
		// Update module parameter ModID
		Pixie_Devices[ModNum].DSP_Parameter_Values[ModID_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
	}
	else if(strcmp(ModParName,"TrigConfig0") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, TrigConfig_Address[ModNum], MOD_READ, ModNum);
		
		// Update module parameter TrigConfig0
		Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
	}	
	else if(strcmp(ModParName,"TrigConfig1") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, TrigConfig_Address[ModNum] + 1, MOD_READ, ModNum);
		
		// Update module parameter TrigConfig1
		Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 1 - DATA_MEMORY_ADDRESS] = *ModParData;
	}
	else if(strcmp(ModParName,"TrigConfig2") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, TrigConfig_Address[ModNum] + 2, MOD_READ, ModNum);
		
		// Update module parameter TrigConfig2
		Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 2 - DATA_MEMORY_ADDRESS] = *ModParData;
	}
	else if(strcmp(ModParName,"TrigConfig3") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, TrigConfig_Address[ModNum] + 3, MOD_READ, ModNum);
		
		// Update module parameter TrigConfig3
		Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 3 - DATA_MEMORY_ADDRESS] = *ModParData;
	}
	else if(strcmp(ModParName,"HOST_RT_PRESET") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(ModParData, 1, HRTP_Address[ModNum], MOD_READ, ModNum);
		
		// Update module parameter HostRTPreset
		Pixie_Devices[ModNum].DSP_Parameter_Values[HRTP_Address[ModNum] - DATA_MEMORY_ADDRESS] = *ModParData;
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadSglModPar): invalid module parameter name %s", ModParName);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	return(0);
}


/****************************************************************
*	Pixie16WriteSglChanPar:
*		Write a channel parameter to a Pixie module.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Invalid Pixie channel number
*			-3 - Invalid channel parameter name
*			-4 - Programing Fippi failed downloading channel parameter
*			-5 - BLcutFinder failed downloading channel parameter
*			-6 - SetDACs failed downloading channel parameter
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteSglChanPar (
	char *ChanParName,         // the name of the channel parameter
	double ChanParData,        // the channel parameter value to be written to the module
	unsigned short ModNum,     // module number
	unsigned short ChanNum )   // channel number
{
	
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	unsigned int FL, FG, SL, SG, FastFilterRange, SlowFilterRange, FifoLength;
	unsigned int fastthresh, peaksample, peaksep, preamptau, tracelength, tracedelay;
	unsigned int paflength, triggerdelay, offsetdac;
	unsigned int xwait, lastxwait;
	unsigned int baselinepercent, energylow, log2ebin, newchancsra, oldchancsra, chancsrb;
	unsigned int baselinecut, fasttrigbacklen, baselineaverage;
	int retval;
	unsigned int cfddelay, cfdscale, qdclen, exttrigstretch, vetostretch, externdelaylen, multiplicitymaskl, multiplicitymaskh, ftrigoutdelay;
	unsigned int chantrigstretch, cfdthresh;

	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	// Check if ChanNum is valid
	if( ChanNum >= NUMBER_OF_CHANNELS )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): invalid Pixie channel number %d", ChanNum);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	// Write channel parameter
	
	if(strcmp(ChanParName,"TRIGGER_RISETIME") == 0)
	{
		
		// Calculate fast length
		FastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			FL = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS / pow(2.0, (double)FastFilterRange));
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			FL = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2) / pow(2.0, (double)FastFilterRange));
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			FL = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5) / pow(2.0, (double)FastFilterRange));

		// Check fast length limit
		FG = Pixie_Devices[ModNum].DSP_Parameter_Values[FastGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		if( (FL + FG) > FASTFILTER_MAX_LEN )
		{
			FL = FASTFILTER_MAX_LEN - FG;
		}
		if( FL < MIN_FASTLENGTH_LEN )
		{
			FL = MIN_FASTLENGTH_LEN;
			if( (FL + FG) > FASTFILTER_MAX_LEN )
			{
				FG = FASTFILTER_MAX_LEN - MIN_FASTLENGTH_LEN;
			}
		}
		
		// Update DSP parameter FastLength
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = FL;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&FL, 1, (unsigned int)(FastLength_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
		// Update DSP parameter FastGap
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = FG;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&FG, 1, (unsigned int)(FastGap_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
		// Program FiPPI to apply FastLength settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading TriggerRiseTime, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"TRIGGER_FLATTOP") == 0)
	{
		
		// Calculate fast gap
		FastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			FG = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS / pow(2.0, (double)FastFilterRange));
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			FG = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2) / pow(2.0, (double)FastFilterRange));
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			FG = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5) / pow(2.0, (double)FastFilterRange));
		
		// Check fast gap limit
		FL = Pixie_Devices[ModNum].DSP_Parameter_Values[FastLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		if( (FL + FG) > FASTFILTER_MAX_LEN )
		{
			FG = FASTFILTER_MAX_LEN - FL;
		}
		
		// Update DSP parameter FastGap
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = FG;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&FG, 1, (unsigned int)(FastGap_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
		// Program FiPPI to apply FastGap settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading TriggerFlatTop, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"TRIGGER_THRESHOLD") == 0)
	{
		
		// Calculate FastThresh
		FL = Pixie_Devices[ModNum].DSP_Parameter_Values[FastLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			fastthresh = (unsigned int)(ChanParData * (double)FL);
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			fastthresh = (unsigned int)(ChanParData * (double)FL * 2.0);
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			fastthresh = (unsigned int)(ChanParData * (double)FL * 5.0);

		// Check FastThresh limit
		if( fastthresh >= FAST_THRESHOLD_MAX )
		{
			fastthresh = (unsigned int)(((double)FAST_THRESHOLD_MAX / (double)FL - 0.5) * (double)FL);	// in ADC counts
		}
		
		// Update DSP parameter FastThresh
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastThresh_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = fastthresh;
		
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&fastthresh, 1, (unsigned int)(FastThresh_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
		// Program FiPPI to apply FastThresh settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading TriggerThreshold, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if((strcmp(ChanParName,"ENERGY_RISETIME") == 0) || (strcmp(ChanParName,"ENERGY_FLATTOP") == 0))
	{
		// Get the current TraceDelay
		FastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		paflength = Pixie_Devices[ModNum].DSP_Parameter_Values[PAFlength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		triggerdelay = Pixie_Devices[ModNum].DSP_Parameter_Values[TriggerDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		tracedelay = paflength - (unsigned int)((double)triggerdelay / pow(2.0, (double)FastFilterRange));
		
		// Get the current SlowFilterRange
		SlowFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		
		if(strcmp(ChanParName,"ENERGY_RISETIME") == 0)
		{
			
			// Calculate slow length
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			SL = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS / pow(2.0, (double)SlowFilterRange));
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			SL = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2) / pow(2.0, (double)SlowFilterRange));
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			SL = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5) / pow(2.0, (double)SlowFilterRange));

			// Check slow length limit
			SG = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
			if( (SL + SG) > SLOWFILTER_MAX_LEN)
			{
				SL = SLOWFILTER_MAX_LEN - SG;
			}
			if( SL < MIN_SLOWLENGTH_LEN)
			{
				SL = MIN_SLOWLENGTH_LEN;
				if((SL + SG) > SLOWFILTER_MAX_LEN)
				{
					SG = SLOWFILTER_MAX_LEN - MIN_SLOWLENGTH_LEN;
				}
			}
		}
		else if(strcmp(ChanParName,"ENERGY_FLATTOP") == 0)
		{
			
			// Calculate slow gap
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			SG = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS / pow(2.0, (double)SlowFilterRange));
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			SG = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2) / pow(2.0, (double)SlowFilterRange));
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			SG = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5) / pow(2.0, (double)SlowFilterRange));
			
			// Check slow gap limit
			SL = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
			if( (SL + SG) > SLOWFILTER_MAX_LEN)
			{
				SG = SLOWFILTER_MAX_LEN - SL;
			}
			if( SG < MIN_SLOWGAP_LEN)
			{
				SG = MIN_SLOWGAP_LEN;
				if((SL + SG) > SLOWFILTER_MAX_LEN)
				{
					SL = SLOWFILTER_MAX_LEN - MIN_SLOWGAP_LEN;
				}
			}
		}
		
		// Update DSP parameter SlowLength
		Pixie_Devices[ModNum].DSP_Parameter_Values[SlowLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = SL;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&SL, 1, (unsigned int)(SlowLength_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
		// Update DSP parameter SlowGap
		Pixie_Devices[ModNum].DSP_Parameter_Values[SlowGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = SG;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&SG, 1, (unsigned int)(SlowGap_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
		// Re-calculate PeakSample and PeakSep
		switch (SlowFilterRange)
		{
			case 1:
				peaksample = SL + SG - 3;
				break;
			case 2:
				peaksample = SL + SG - 2;
				break;
			case 3:
				peaksample = SL + SG - 2;
				break;
			case 4:
				peaksample = SL + SG - 1;
				break;
			case 5:
				peaksample = SL + SG;
				break;
			case 6:
				peaksample = SL + SG + 1;
				break;
			default:
				peaksample = SL + SG - 2;
				break;
		}

		peaksep = SL + SG;

		// Update DSP parameter PeakSample
		Pixie_Devices[ModNum].DSP_Parameter_Values[PeakSample_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = peaksample;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&peaksample, 1, (unsigned int)(PeakSample_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
		// Update DSP parameter PeakSep
		Pixie_Devices[ModNum].DSP_Parameter_Values[PeakSep_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = peaksep;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&peaksep, 1, (unsigned int)(PeakSep_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
		// Update FIFO settings (TriggerDelay and PAFLength)
		Pixie_ComputeFIFO(tracedelay, ModNum, ChanNum);
		
		// Program FiPPI to apply settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading EnergyRiseTime or EnergyFlatTop, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
		
		// Update baseline cut value
		retval = Pixie16BLcutFinder(ModNum, ChanNum, &baselinecut);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): BLcutFinder failed in module %d channel %d after downloading EnergyRiseTime or EnergyFlatTop, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-5);
		}
	}
	else if(strcmp(ChanParName,"TAU") == 0)
	{
		
		// Calculate PreampTau
		preamptau = Decimal2IEEEFloating(ChanParData);
		
		// Update DSP parameter PreampTau
		Pixie_Devices[ModNum].DSP_Parameter_Values[PreampTau_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = preamptau;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&preamptau, 1, (unsigned int)(PreampTau_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
		// Use Program_FiPPI to recompute the coefficients
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading Tau, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
		
		// Update baseline cut value
		retval = Pixie16BLcutFinder(ModNum, ChanNum, &baselinecut);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): BLcutFinder failed in module %d channel %d after downloading Tau, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-5);
		}
	}
	else if(strcmp(ChanParName,"TRACE_LENGTH") == 0)
	{
		
		// Get the current FastFilterRange
		FastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		
		// Compute the requested TraceLength
		tracelength = (unsigned int)(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS / pow(2.0, (double)FastFilterRange));

		if(Module_Information[ModNum].Module_ADCMSPS == 500)
		{
			// Ensure TraceLength is multiple of 10 to fit 10-to-5*2 packing in the FPGA
			tracelength = ((unsigned int)(tracelength / 10)) * 10;
			if(tracelength < TRACELEN_MIN_500MHZADC)
			{
				// Enforce a minimum value
				tracelength = TRACELEN_MIN_500MHZADC;
			}
		}
		else if((Module_Information[ModNum].Module_ADCMSPS == 250) || (Module_Information[ModNum].Module_ADCMSPS == 100))
		{
			// Ensure TraceLength is an even number to fit 2-to-1 packing in the FPGA
			tracelength = APP32_ClrBit(0, tracelength);
			if(tracelength < TRACELEN_MIN_250OR100MHZADC)
			{
				// Enforce a minimum value
				tracelength = TRACELEN_MIN_250OR100MHZADC;
			}
		}

		// Check if TraceLength exceeds FifoLength
		FifoLength = Pixie_Devices[ModNum].DSP_Parameter_Values[FIFOLength_Address[ModNum] - DATA_MEMORY_ADDRESS];
		if(tracelength > FifoLength)
		{
			tracelength = FifoLength;
		}
		
		// Update DSP parameter TraceLength
		Pixie_Devices[ModNum].DSP_Parameter_Values[TraceLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = tracelength;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&tracelength, 1, (unsigned int)(TraceLength_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply TraceLength settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading TraceLength, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"TRACE_DELAY") == 0)
	{
		
		// Get the current FastFilterRange
		FastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		
		// Check if TraceDelay exceeds TraceLength
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			tracedelay = (unsigned int)(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS / pow(2.0, (double)FastFilterRange));
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			tracedelay = (unsigned int)(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2) / pow(2.0, (double)FastFilterRange));
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			tracedelay = (unsigned int)(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5) / pow(2.0, (double)FastFilterRange));

		tracelength = Pixie_Devices[ModNum].DSP_Parameter_Values[TraceLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		if(tracedelay > tracelength)
		{
			tracedelay = (unsigned int)((double)tracelength / 2.0);
		}
		
		// Check if TraceDelay exceeds TRACEDELAY_MAX
		if(tracedelay > TRACEDELAY_MAX)
		{
			tracedelay = TRACEDELAY_MAX;
		}

		// Update FIFO settings (TriggerDelay and PAFLength)
		Pixie_ComputeFIFO(tracedelay, ModNum, ChanNum);
		
		// Program FiPPI to apply settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading TraceDelay, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"VOFFSET") == 0)
	{
		
		offsetdac = (unsigned int)(65536.0 * (ChanParData / DAC_VOLTAGE_RANGE  + 1.0 / 2.0));
		
		// Check limit
		if(offsetdac > 65535)
		{
			offsetdac = 65535;	// a 16-bit DAC
		}
		
		// Update DSP parameter OffsetDAC
		Pixie_Devices[ModNum].DSP_Parameter_Values[OffsetDAC_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = offsetdac;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&offsetdac, 1, (unsigned int)(OffsetDAC_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
		// Set DACs to apply the new DAC settings
		retval = Pixie16SetDACs(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): SetDACs failed in module %d channel %d after downloading OffsetDACs, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-6);
		}
	}
	else if(strcmp(ChanParName,"XDT") == 0)
	{
		// Get the last Xwait
		lastxwait = Pixie_Devices[ModNum].DSP_Parameter_Values[Xwait_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		// Compute the new Xwait
		xwait = (unsigned int)ROUND(ChanParData * (double)DSP_CLOCK_MHZ);
		if((Module_Information[ModNum].Module_ADCMSPS == 100) || (Module_Information[ModNum].Module_ADCMSPS == 500))
		{
			// For 100 MSPS RevB/C/D or RevF, or 500 MSPS RevF, xwait should be multiples of 6

			if(xwait < 6)	// xwait should be at least 6
			{
				xwait = 6;
			}
			
			if(xwait > lastxwait) // increase Xwait
			{
				xwait = (unsigned int)(ceil((double)xwait / 6.0) * 6.0);
			}
			else	// decrease Xwait
			{
				xwait = (unsigned int)(floor((double)xwait / 6.0) * 6.0);
			}
		}
		else
		{
			// For 250 RevF, xwait should be multiples of 8

			if(xwait < 8)	// xwait should be at least 8
			{
				xwait = 8;
			}
			
			if(xwait > lastxwait) // increase Xwait
			{
				xwait = (unsigned int)(ceil((double)xwait / 8.0) * 8.0);
			}
			else	// decrease Xwait
			{
				xwait = (unsigned int)(floor((double)xwait / 8.0) * 8.0);
			}
		}

		// Update DSP parameter Xwait
		Pixie_Devices[ModNum].DSP_Parameter_Values[Xwait_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = xwait;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&xwait, 1, (unsigned int)(Xwait_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
	}
	else if(strcmp(ChanParName,"BASELINE_PERCENT") == 0)
	{
		
		// Get the new BaselinePercent
		baselinepercent = (unsigned int)ChanParData;
		
		// Check limit
		if(baselinepercent < 1)
		{
			baselinepercent = 1;
		}
		else if(baselinepercent > 99)
		{
			baselinepercent = 99;
		}
		
		// Update DSP parameter BaselinePercent
		Pixie_Devices[ModNum].DSP_Parameter_Values[BaselinePercent_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = baselinepercent;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&baselinepercent, 1, (unsigned int)(BaselinePercent_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
	}
	else if(strcmp(ChanParName,"EMIN") == 0)
	{
		
		// Get the new EnergyLow
		energylow = (unsigned int)ChanParData;
		
		// Update DSP parameter EnergyLow
		Pixie_Devices[ModNum].DSP_Parameter_Values[EnergyLow_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = energylow;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&energylow, 1, (unsigned int)(EnergyLow_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
	}
	else if(strcmp(ChanParName,"BINFACTOR") == 0)
	{
		
		// Get the new Log2Ebin
		log2ebin = (unsigned int)ChanParData;
		
		// Check limit
		if(log2ebin < 1)
		{
			log2ebin = 1;
		}
		if(log2ebin > 6)
		{
			log2ebin = 6;
		}
		
		// Convert to a signed negative number
		log2ebin = (unsigned int)(pow(2.0, 32.0) - (double)log2ebin);
		
		// Update DSP parameter Log2Ebin
		Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Ebin_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = log2ebin;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&log2ebin, 1, (unsigned int)(Log2Ebin_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
	}
	else if(strcmp(ChanParName,"BASELINE_AVERAGE") == 0)
	{
		
		// Get the new baselineaverage
		baselineaverage = (unsigned int)ChanParData;

		// Check limit
		if(baselineaverage > 16)
		{
			baselineaverage = 16;
		}
		
		// Convert to a signed negative number
		if(baselineaverage > 0)
		{
			baselineaverage = (unsigned int)(pow(2.0, 32.0) - (double)baselineaverage);
		}
		
		// Update DSP parameter Log2Bweight
		Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Bweight_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = baselineaverage;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&baselineaverage, 1, (unsigned int)(Log2Bweight_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
	
	}
	else if(strcmp(ChanParName,"CHANNEL_CSRA") == 0)
	{
		
		// Get the new ChanCSRa
		newchancsra = (unsigned int)ChanParData;
		oldchancsra = Pixie_Devices[ModNum].DSP_Parameter_Values[ChanCSRa_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
		// Update DSP parameter ChanCSRa
		Pixie_Devices[ModNum].DSP_Parameter_Values[ChanCSRa_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = newchancsra;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&newchancsra, 1, (unsigned int)(ChanCSRa_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
		// Program FiPPI to apply settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading ChanCSRA, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
		
		// Set DACs to apply the new DAC settings
		retval = Pixie16SetDACs(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): SetDACs failed in module %d channel %d after downloading ChanCSRA, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-6);
		}
	
		// Check if we need to update baseline cut value (only needed if Vgain changed)
		if(APP32_TstBit(CCSRA_ENARELAY, newchancsra) != APP32_TstBit(CCSRA_ENARELAY, oldchancsra))
		{
			retval = Pixie16BLcutFinder(ModNum, ChanNum, &baselinecut);
			if(retval < 0)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): BLcutFinder failed in module %d channel %d after downloading ChanCSRA, retval=%d", ModNum, ChanNum, retval);
				Pixie_Print_MSG(ErrMSG);
				return(-5);
			}
		}
	}
	else if(strcmp(ChanParName,"CHANNEL_CSRB") == 0)
	{
		
		// Get the new ChanCSRb
		chancsrb = (unsigned int)ChanParData;
		
		// Update DSP parameter ChanCSRb
		Pixie_Devices[ModNum].DSP_Parameter_Values[ChanCSRb_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = chancsrb;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&chancsrb, 1, (unsigned int)(ChanCSRb_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
	}
	else if(strcmp(ChanParName,"BLCUT") == 0)
	{
		
		// Get the new BLcut
		baselinecut = (unsigned int)ChanParData;
		
		// Update DSP parameter BLcut
		Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = baselinecut;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&baselinecut, 1, (unsigned int)(BLcut_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
	}
	else if(strcmp(ChanParName,"FASTTRIGBACKLEN") == 0)
	{
		
		// Get the new FastTrigBackLen
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			fasttrigbacklen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			fasttrigbacklen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2));
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			fasttrigbacklen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));

		// Range check for FastTrigBackLen
		if((Module_Information[ModNum].Module_ADCMSPS == 100) || (Module_Information[ModNum].Module_ADCMSPS == 500))
		{
			if(fasttrigbacklen < FASTTRIGBACKLEN_MIN_100MHZFIPCLK)
			{
				fasttrigbacklen = FASTTRIGBACKLEN_MIN_100MHZFIPCLK;
			}
		}
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
		{
			if(fasttrigbacklen < FASTTRIGBACKLEN_MIN_125MHZFIPCLK)
			{
				fasttrigbacklen = FASTTRIGBACKLEN_MIN_125MHZFIPCLK;
			}
		}

		if(fasttrigbacklen > FASTTRIGBACKLEN_MAX)
		{
			fasttrigbacklen = FASTTRIGBACKLEN_MAX;
		}
		
		// Update DSP parameter FastTrigBackLen
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastTrigBackLen_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = fasttrigbacklen;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&fasttrigbacklen, 1, (unsigned int)(FastTrigBackLen_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);
		
		// Program FiPPI to apply FastTrigBackLen settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading FastTrigBackLen, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"CFDDelay") == 0)
	{
		// Get the new CFDDelay
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			cfddelay = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			cfddelay = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2));
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			cfddelay = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));

		// Range check for CFDDelay
		if(cfddelay < CFDDELAY_MIN)
		{
			cfddelay = CFDDELAY_MIN;
		}
		if(cfddelay > CFDDELAY_MAX)
		{
			cfddelay = CFDDELAY_MAX;
		}

		// Update DSP parameter CFDDelay
		Pixie_Devices[ModNum].DSP_Parameter_Values[CFDDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = cfddelay;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&cfddelay, 1, (unsigned int)(CFDDelay_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply CFDDelay settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading CFDDelay, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"CFDScale") == 0)
	{
		// Get the new CFDScale
		cfdscale = (unsigned int)ChanParData;

		// Range check for the value of CFDScale
		if(cfdscale > CFDSCALE_MAX)
		{
			cfdscale = CFDSCALE_MAX;
		}

		// Update DSP parameter CFDScale
		Pixie_Devices[ModNum].DSP_Parameter_Values[CFDScale_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = cfdscale;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&cfdscale, 1, (unsigned int)(CFDScale_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply CFDScale settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading CFDScale, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"CFDThresh") == 0)
	{
		// Get the new CFDThresh
		cfdthresh = (unsigned int)ChanParData;

		// Range check for the value of CFDThresh
		if( (cfdthresh >= CFDTHRESH_MIN) && (cfdthresh <= CFDTHRESH_MAX) )
		{
			// Update DSP parameter CFDThresh
			Pixie_Devices[ModNum].DSP_Parameter_Values[CFDThresh_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = cfdthresh;
			// Download to the selected Pixie module
			Pixie16IMbufferIO(&cfdthresh, 1, (unsigned int)(CFDThresh_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

			// Program FiPPI to apply CFDThresh settings to the FPGA
			retval = Pixie16ProgramFippi(ModNum);
			if(retval < 0)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading CFDThresh, retval=%d", ModNum, ChanNum, retval);
				Pixie_Print_MSG(ErrMSG);
				return(-4);
			}
		}
		else
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): Value of CFDThresh being set was out of its valid range");
			Pixie_Print_MSG(ErrMSG);
		}
	}
	else if(strcmp(ChanParName,"QDCLen0") == 0)
	{
		// Get the new QDCLen0
		if(Module_Information[ModNum].Module_ADCMSPS == 500)
			qdclen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));
		else
			qdclen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);

		// Range check for QDC length
		if(qdclen < QDCLEN_MIN)
		{
			qdclen = QDCLEN_MIN;
		}
		if(qdclen > QDCLEN_MAX)
		{
			qdclen = QDCLEN_MAX;
		}

		// Update DSP parameter QDCLen0
		Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen0_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&qdclen, 1, (unsigned int)(QDCLen0_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply QDCLen0 settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading QDCLen0, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"QDCLen1") == 0)
	{
		// Get the new QDCLen1
		if(Module_Information[ModNum].Module_ADCMSPS == 500)
			qdclen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));
		else
			qdclen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);

		// Range check for QDC length
		if(qdclen < QDCLEN_MIN)
		{
			qdclen = QDCLEN_MIN;
		}
		if(qdclen > QDCLEN_MAX)
		{
			qdclen = QDCLEN_MAX;
		}

		// Update DSP parameter QDCLen1
		Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen1_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&qdclen, 1, (unsigned int)(QDCLen1_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply QDCLen1 settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading QDCLen1, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"QDCLen2") == 0)
	{
		// Get the new QDCLen2
		if(Module_Information[ModNum].Module_ADCMSPS == 500)
			qdclen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));
		else
			qdclen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);

		// Range check for QDC length
		if(qdclen < QDCLEN_MIN)
		{
			qdclen = QDCLEN_MIN;
		}
		if(qdclen > QDCLEN_MAX)
		{
			qdclen = QDCLEN_MAX;
		}

		// Update DSP parameter QDCLen2
		Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen2_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&qdclen, 1, (unsigned int)(QDCLen2_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply QDCLen2 settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading QDCLen2, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"QDCLen3") == 0)
	{
		// Get the new QDCLen3
		if(Module_Information[ModNum].Module_ADCMSPS == 500)
			qdclen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));
		else
			qdclen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);

		// Range check for QDC length
		if(qdclen < QDCLEN_MIN)
		{
			qdclen = QDCLEN_MIN;
		}
		if(qdclen > QDCLEN_MAX)
		{
			qdclen = QDCLEN_MAX;
		}

		// Update DSP parameter QDCLen3
		Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen3_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&qdclen, 1, (unsigned int)(QDCLen3_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply QDCLen3 settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading QDCLen3, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"QDCLen4") == 0)
	{
		// Get the new QDCLen4
		if(Module_Information[ModNum].Module_ADCMSPS == 500)
			qdclen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));
		else
			qdclen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);

		// Range check for QDC length
		if(qdclen < QDCLEN_MIN)
		{
			qdclen = QDCLEN_MIN;
		}
		if(qdclen > QDCLEN_MAX)
		{
			qdclen = QDCLEN_MAX;
		}

		// Update DSP parameter QDCLen4
		Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen4_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&qdclen, 1, (unsigned int)(QDCLen4_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply QDCLen4 settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading QDCLen4, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"QDCLen5") == 0)
	{
		// Get the new QDCLen5
		if(Module_Information[ModNum].Module_ADCMSPS == 500)
			qdclen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));
		else
			qdclen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);

		// Range check for QDC length
		if(qdclen < QDCLEN_MIN)
		{
			qdclen = QDCLEN_MIN;
		}
		if(qdclen > QDCLEN_MAX)
		{
			qdclen = QDCLEN_MAX;
		}

		// Update DSP parameter QDCLen5
		Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen5_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&qdclen, 1, (unsigned int)(QDCLen5_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply QDCLen5 settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading QDCLen5, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"QDCLen6") == 0)
	{
		// Get the new QDCLen6
		if(Module_Information[ModNum].Module_ADCMSPS == 500)
			qdclen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));
		else
			qdclen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);

		// Range check for QDC length
		if(qdclen < QDCLEN_MIN)
		{
			qdclen = QDCLEN_MIN;
		}
		if(qdclen > QDCLEN_MAX)
		{
			qdclen = QDCLEN_MAX;
		}

		// Update DSP parameter QDCLen6
		Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen6_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&qdclen, 1, (unsigned int)(QDCLen6_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply QDCLen6 settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading QDCLen6, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"QDCLen7") == 0)
	{
		// Get the new QDCLen7
		if(Module_Information[ModNum].Module_ADCMSPS == 500)
			qdclen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));
		else
			qdclen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);

		// Range check for QDC length
		if(qdclen < QDCLEN_MIN)
		{
			qdclen = QDCLEN_MIN;
		}
		if(qdclen > QDCLEN_MAX)
		{
			qdclen = QDCLEN_MAX;
		}

		// Update DSP parameter QDCLen7
		Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen7_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&qdclen, 1, (unsigned int)(QDCLen7_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply QDCLen7 settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading QDCLen7, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"ExtTrigStretch") == 0)
	{
		// Get the new ExtTrigStretch
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			exttrigstretch = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			exttrigstretch = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2));
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			exttrigstretch = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));

		// Range check for ExtTrigStretch
		if(exttrigstretch < EXTTRIGSTRETCH_MIN)
		{
			exttrigstretch = EXTTRIGSTRETCH_MIN;
		}
		if(exttrigstretch > EXTTRIGSTRETCH_MAX)
		{
			exttrigstretch = EXTTRIGSTRETCH_MAX;
		}

		// Update DSP parameter ExtTrigStretch
		Pixie_Devices[ModNum].DSP_Parameter_Values[ExtTrigStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = exttrigstretch;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&exttrigstretch, 1, (unsigned int)(ExtTrigStretch_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply ExtTrigStretch settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading ExtTrigStretch, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"VetoStretch") == 0)
	{
		// Get the new VetoStretch
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			vetostretch = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			vetostretch = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2));
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			vetostretch = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));

		// Range check for VetoStretch
		if(vetostretch < VETOSTRETCH_MIN)
		{
			vetostretch = VETOSTRETCH_MIN;
		}
		if(vetostretch > VETOSTRETCH_MAX)
		{
			vetostretch = VETOSTRETCH_MAX;
		}

		// Update DSP parameter VetoStretch
		Pixie_Devices[ModNum].DSP_Parameter_Values[VetoStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = vetostretch;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&vetostretch, 1, (unsigned int)(VetoStretch_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply VetoStretch settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading VetoStretch, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"MultiplicityMaskL") == 0)
	{
		// Get the new MultiplicityMaskL
		multiplicitymaskl = (unsigned int)ChanParData;

		// Update DSP parameter MultiplicityMaskL
		Pixie_Devices[ModNum].DSP_Parameter_Values[MultiplicityMaskL_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = multiplicitymaskl;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&multiplicitymaskl, 1, (unsigned int)(MultiplicityMaskL_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply MultiplicityMaskL settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading MultiplicityMaskL, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"MultiplicityMaskH") == 0)
	{
		// Get the new MultiplicityMaskH
		multiplicitymaskh = (unsigned int)ChanParData;

		// Update DSP parameter MultiplicityMaskH
		Pixie_Devices[ModNum].DSP_Parameter_Values[MultiplicityMaskH_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = multiplicitymaskh;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&multiplicitymaskh, 1, (unsigned int)(MultiplicityMaskH_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply MultiplicityMaskH settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading MultiplicityMaskH, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"ExternDelayLen") == 0)
	{
		// Get the new ExternDelayLen
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			externdelaylen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			externdelaylen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2));
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			externdelaylen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));

		// Range check for ExternDelayLen
		if(externdelaylen < EXTDELAYLEN_MIN)
		{
			externdelaylen = EXTDELAYLEN_MIN;
		}

		if ((Module_Information[ModNum].Module_Rev == 0xB) || (Module_Information[ModNum].Module_Rev == 0xC) || (Module_Information[ModNum].Module_Rev == 0xD))
		{
			if(externdelaylen > EXTDELAYLEN_MAX_REVBCD)
			{
				externdelaylen = EXTDELAYLEN_MAX_REVBCD;
			}

		}
		else if (Module_Information[ModNum].Module_Rev == 0xF)
		{
			if(externdelaylen > EXTDELAYLEN_MAX_REVF)
			{
				externdelaylen = EXTDELAYLEN_MAX_REVF;
			}
		}

		// Update DSP parameter ExternDelayLen
		Pixie_Devices[ModNum].DSP_Parameter_Values[ExternDelayLen_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = externdelaylen;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&externdelaylen, 1, (unsigned int)(ExternDelayLen_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply ExternDelayLen settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading ExternDelayLen, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"FtrigoutDelay") == 0)
	{
		// Get the new FtrigoutDelay
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			ftrigoutdelay = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			ftrigoutdelay = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2));
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			ftrigoutdelay = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));

		// Range check for FtrigoutDelay
		if(ftrigoutdelay < FASTTRIGBACKDELAY_MIN)
		{
			ftrigoutdelay = FASTTRIGBACKDELAY_MIN;
		}
		
		if ((Module_Information[ModNum].Module_Rev == 0xB) || (Module_Information[ModNum].Module_Rev == 0xC) || (Module_Information[ModNum].Module_Rev == 0xD))
		{
			if(ftrigoutdelay > FASTTRIGBACKDELAY_MAX_REVBCD)
			{
				ftrigoutdelay = FASTTRIGBACKDELAY_MAX_REVBCD;
			}
		}
		else if (Module_Information[ModNum].Module_Rev == 0xF)
		{
			if(ftrigoutdelay > FASTTRIGBACKDELAY_MAX_REVF)
			{
				ftrigoutdelay = FASTTRIGBACKDELAY_MAX_REVF;
			}
		}

		// Update DSP parameter FtrigoutDelay
		Pixie_Devices[ModNum].DSP_Parameter_Values[FtrigoutDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = ftrigoutdelay;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&ftrigoutdelay, 1, (unsigned int)(FtrigoutDelay_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply FtrigoutDelay settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading FtrigoutDelay, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else if(strcmp(ChanParName,"ChanTrigStretch") == 0)
	{
		// Get the new ChanTrigStretch
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			chantrigstretch = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			chantrigstretch = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2));
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			chantrigstretch = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));

		// Range check for ChanTrigStretch
		if(chantrigstretch < CHANTRIGSTRETCH_MIN)
		{
			chantrigstretch = CHANTRIGSTRETCH_MIN;
		}
		if(chantrigstretch > CHANTRIGSTRETCH_MAX)
		{
			chantrigstretch = CHANTRIGSTRETCH_MAX;
		}

		// Update DSP parameter ChanTrigStretch
		Pixie_Devices[ModNum].DSP_Parameter_Values[ChanTrigStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = chantrigstretch;
		// Download to the selected Pixie module
		Pixie16IMbufferIO(&chantrigstretch, 1, (unsigned int)(ChanTrigStretch_Address[ModNum] + ChanNum), MOD_WRITE, ModNum);

		// Program FiPPI to apply ChanTrigStretch settings to the FPGA
		retval = Pixie16ProgramFippi(ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): ProgramFippi failed in module %d channel %d after downloading ChanTrigStretch, retval=%d", ModNum, ChanNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-4);
		}
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): invalid channel parameter name %s", ChanParName);
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}
	
	return(0);
}


/****************************************************************
*	Pixie16ReadSglChanPar:
*		Read a channel parameter from a Pixie module.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Invalid Pixie channel number
*			-3 - Invalid channel parameter name
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanPar (
	char *ChanParName,         // the name of the channel parameter
	double *ChanParData,       // the channel parameter value to be read from the module
	unsigned short ModNum,     // module number
	unsigned short ChanNum )   // channel number
{
	
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	unsigned int FL, FG, SL, SG, FastFilterRange, SlowFilterRange;
	unsigned int fastthresh, preamptau, tracelength;
	unsigned int paflength, triggerdelay, offsetdac;
	unsigned int xwait;
	unsigned int baselinepercent, energylow, log2ebin, chancsra, chancsrb;
	unsigned int baselinecut, fasttrigbacklen, baselineaverage;
	unsigned int cfddelay, cfdscale, qdclen, exttrigstretch, vetostretch, externdelaylen, multiplicitymaskl, multiplicitymaskh, ftrigoutdelay;
	unsigned int chantrigstretch, cfdthresh;

	// Check if ModNum is valid
	if( ModNum >= Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadSglChanPar): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	// Check if ChanNum is valid
	if( ChanNum >= NUMBER_OF_CHANNELS )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadSglChanPar): invalid Pixie channel number %d", ChanNum);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	// Read channel parameter
	
	if(strcmp(ChanParName,"TRIGGER_RISETIME") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&FL, 1, (unsigned int)(FastLength_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = FL;
		
		Pixie16IMbufferIO(&FastFilterRange, 1, FastFilterRange_Address[ModNum], MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] = FastFilterRange;
		
		// Update channel parameter TriggerRiseTime
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			*ChanParData = (double)FL * pow(2.0, (double)FastFilterRange) / (double)Module_Information[ModNum].Module_ADCMSPS;
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			*ChanParData = (double)FL * pow(2.0, (double)FastFilterRange) / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)FL * pow(2.0, (double)FastFilterRange) / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
	
	}
	else if(strcmp(ChanParName,"TRIGGER_FLATTOP") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&FG, 1, (unsigned int)(FastGap_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = FG;
		
		Pixie16IMbufferIO(&FastFilterRange, 1, FastFilterRange_Address[ModNum], MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] = FastFilterRange;
		
		// Update channel parameter TriggerFlatTop
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			*ChanParData = (double)FG * pow(2.0, (double)FastFilterRange) / (double)Module_Information[ModNum].Module_ADCMSPS;
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			*ChanParData = (double)FG * pow(2.0, (double)FastFilterRange) / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)FG * pow(2.0, (double)FastFilterRange) / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
		
	}
	else if(strcmp(ChanParName,"TRIGGER_THRESHOLD") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&fastthresh, 1, (unsigned int)(FastThresh_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastThresh_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = fastthresh;
		
		Pixie16IMbufferIO(&FL, 1, (unsigned int)(FastLength_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = FL;
		
		// Update channel parameter TriggerThreshold
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			*ChanParData = (double)fastthresh / (double)FL;
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			*ChanParData = (double)fastthresh / ((double)FL * 2.0);
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)fastthresh / ((double)FL * 5.0);

	}
	else if(strcmp(ChanParName,"ENERGY_RISETIME") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&SL, 1, (unsigned int)(SlowLength_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[SlowLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = SL;
		
		Pixie16IMbufferIO(&SlowFilterRange, 1, SlowFilterRange_Address[ModNum], MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] = SlowFilterRange;
		
		// Update channel parameter EnergyRiseTime
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			*ChanParData = (double)SL * pow(2.0, (double)SlowFilterRange) / (double)Module_Information[ModNum].Module_ADCMSPS;
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			*ChanParData = (double)SL * pow(2.0, (double)SlowFilterRange) / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)SL * pow(2.0, (double)SlowFilterRange) / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
		
	}
	else if(strcmp(ChanParName,"ENERGY_FLATTOP") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&SG, 1, (unsigned int)(SlowGap_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[SlowGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = SG;
		
		Pixie16IMbufferIO(&SlowFilterRange, 1, SlowFilterRange_Address[ModNum], MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] = SlowFilterRange;
		
		// Update channel parameter EnergyFlatTop
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			*ChanParData = (double)SG * pow(2.0, (double)SlowFilterRange) / (double)Module_Information[ModNum].Module_ADCMSPS;
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			*ChanParData = (double)SG * pow(2.0, (double)SlowFilterRange) / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)SG * pow(2.0, (double)SlowFilterRange) / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);

	}
	else if(strcmp(ChanParName,"TAU") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&preamptau, 1, (unsigned int)(PreampTau_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[PreampTau_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = preamptau;
		
		// Update channel parameter PreampTau
		*ChanParData = IEEEFloating2Decimal(preamptau);
		
	}
	else if(strcmp(ChanParName,"TRACE_LENGTH") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&tracelength, 1, (unsigned int)(TraceLength_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[TraceLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = tracelength;
		Pixie16IMbufferIO(&FastFilterRange, 1, FastFilterRange_Address[ModNum], MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] = FastFilterRange;
		
		// Update channel parameter TraceLength
		*ChanParData = (double)tracelength / (double)Module_Information[ModNum].Module_ADCMSPS * pow(2.0, (double)FastFilterRange);
		
	}
	else if(strcmp(ChanParName,"TRACE_DELAY") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&paflength, 1, (unsigned int)(PAFlength_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[PAFlength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = paflength;
		Pixie16IMbufferIO(&triggerdelay, 1, (unsigned int)(TriggerDelay_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[TriggerDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = triggerdelay;
		Pixie16IMbufferIO(&FastFilterRange, 1, FastFilterRange_Address[ModNum], MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] = FastFilterRange;
		
		// Update channel parameter TraceDelay
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			*ChanParData = (double)(paflength - (unsigned int)((double)triggerdelay / pow(2.0, (double)FastFilterRange))) / (double)Module_Information[ModNum].Module_ADCMSPS * pow(2.0, (double)FastFilterRange);
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			*ChanParData = (double)(paflength - (unsigned int)((double)triggerdelay / pow(2.0, (double)FastFilterRange))) / (double)(Module_Information[ModNum].Module_ADCMSPS / 2) * pow(2.0, (double)FastFilterRange);
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)(paflength - (unsigned int)((double)triggerdelay / pow(2.0, (double)FastFilterRange))) / (double)(Module_Information[ModNum].Module_ADCMSPS / 5) * pow(2.0, (double)FastFilterRange);
		
	}
	else if(strcmp(ChanParName,"VOFFSET") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&offsetdac, 1, (unsigned int)(OffsetDAC_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[OffsetDAC_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = offsetdac;
		
		// Update channel parameter DCoffset
		*ChanParData = (double)offsetdac / 65536.0 * DAC_VOLTAGE_RANGE - DAC_VOLTAGE_RANGE / 2.0;
		
	}
	else if(strcmp(ChanParName,"XDT") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&xwait, 1, (unsigned int)(Xwait_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[Xwait_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = xwait;
		
		// Update channel parameter Xdt
		*ChanParData = (double)xwait / (double)DSP_CLOCK_MHZ;		
	}
	else if(strcmp(ChanParName,"BASELINE_PERCENT") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&baselinepercent, 1, (unsigned int)(BaselinePercent_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[BaselinePercent_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = baselinepercent;
		
		// Update channel parameter BaselinePercent
		*ChanParData = (double)baselinepercent;
		
	}
	else if(strcmp(ChanParName,"EMIN") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&energylow, 1, (unsigned int)(EnergyLow_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[EnergyLow_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = energylow;
		
		// Update channel parameter EnergyLow
		*ChanParData = (double)energylow;
		
	}
	else if(strcmp(ChanParName,"BINFACTOR") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&log2ebin, 1, (unsigned int)(Log2Ebin_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Ebin_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = log2ebin;
		
		// Update channel parameter BinFactor
		*ChanParData = (double)(pow(2.0, 32.0) - log2ebin);
		
	}
	else if(strcmp(ChanParName,"BASELINE_AVERAGE") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&baselineaverage, 1, (unsigned int)(Log2Bweight_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Bweight_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = baselineaverage;
		
		// Update channel parameter Baseline Average
		if(baselineaverage == 0)
		{
			*ChanParData = 0.0;
		}
		else
		{
			*ChanParData = (double)(pow(2.0, 32.0) - baselineaverage);
		}
		
	}
	else if(strcmp(ChanParName,"CHANNEL_CSRA") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&chancsra, 1, (unsigned int)(ChanCSRa_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[ChanCSRa_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = chancsra;
		
		// Update channel parameter ChanCSRA
		*ChanParData = (double)chancsra;
		
	}
	else if(strcmp(ChanParName,"CHANNEL_CSRB") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&chancsrb, 1, (unsigned int)(ChanCSRb_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[ChanCSRb_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = chancsrb;
		
		// Update channel parameter ChanCSRB
		*ChanParData = (double)chancsrb;
		
	}
	else if(strcmp(ChanParName,"BLCUT") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&baselinecut, 1, (unsigned int)(BLcut_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = baselinecut;
		
		// Update channel parameter BaselineCut
		*ChanParData = (double)baselinecut;
		
	}
	else if(strcmp(ChanParName,"FASTTRIGBACKLEN") == 0)
	{
		
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&fasttrigbacklen, 1, (unsigned int)(FastTrigBackLen_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[FastTrigBackLen_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = fasttrigbacklen;
		
		// Update channel parameter FastTrigBackLen
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			*ChanParData = (double)fasttrigbacklen / (double)Module_Information[ModNum].Module_ADCMSPS;
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			*ChanParData = (double)fasttrigbacklen / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)fasttrigbacklen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
		
	}
	else if(strcmp(ChanParName,"CFDDelay") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&cfddelay, 1, (unsigned int)(CFDDelay_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[CFDDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = cfddelay;
		
		// Update channel parameter CFDDelay
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			*ChanParData = (double)cfddelay / (double)Module_Information[ModNum].Module_ADCMSPS;
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			*ChanParData = (double)cfddelay / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)cfddelay / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);

	}
	else if(strcmp(ChanParName,"CFDScale") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&cfdscale, 1, (unsigned int)(CFDScale_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[CFDScale_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = cfdscale;
		
		// Update channel parameter CFDScale
		*ChanParData = (double)cfdscale;
	}
	else if(strcmp(ChanParName,"CFDThresh") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&cfdthresh, 1, (unsigned int)(CFDThresh_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[CFDThresh_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = cfdthresh;
		
		// Update channel parameter CFDThresh
		*ChanParData = (double)cfdthresh;
	}
	else if(strcmp(ChanParName,"QDCLen0") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&qdclen, 1, (unsigned int)(QDCLen0_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen0_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
		
		// Update channel parameter QDCLen0
		if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)qdclen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
		else
			*ChanParData = (double)qdclen / (double)Module_Information[ModNum].Module_ADCMSPS;
	}
	else if(strcmp(ChanParName,"QDCLen1") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&qdclen, 1, (unsigned int)(QDCLen1_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen1_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
		
		if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)qdclen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
		else
			*ChanParData = (double)qdclen / (double)Module_Information[ModNum].Module_ADCMSPS;
	}
	else if(strcmp(ChanParName,"QDCLen2") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&qdclen, 1, (unsigned int)(QDCLen2_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen2_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
		
		if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)qdclen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
		else
			*ChanParData = (double)qdclen / (double)Module_Information[ModNum].Module_ADCMSPS;
	}
	else if(strcmp(ChanParName,"QDCLen3") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&qdclen, 1, (unsigned int)(QDCLen3_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen3_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
		
		if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)qdclen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
		else
			*ChanParData = (double)qdclen / (double)Module_Information[ModNum].Module_ADCMSPS;
	}
	else if(strcmp(ChanParName,"QDCLen4") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&qdclen, 1, (unsigned int)(QDCLen4_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen4_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
		
		if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)qdclen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
		else
			*ChanParData = (double)qdclen / (double)Module_Information[ModNum].Module_ADCMSPS;
	}
	else if(strcmp(ChanParName,"QDCLen5") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&qdclen, 1, (unsigned int)(QDCLen5_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen5_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
		
		if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)qdclen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
		else
			*ChanParData = (double)qdclen / (double)Module_Information[ModNum].Module_ADCMSPS;
	}
	else if(strcmp(ChanParName,"QDCLen6") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&qdclen, 1, (unsigned int)(QDCLen6_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen6_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
		
		if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)qdclen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
		else
			*ChanParData = (double)qdclen / (double)Module_Information[ModNum].Module_ADCMSPS;
	}
	else if(strcmp(ChanParName,"QDCLen7") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&qdclen, 1, (unsigned int)(QDCLen7_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen7_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
		
		if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)qdclen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
		else
			*ChanParData = (double)qdclen / (double)Module_Information[ModNum].Module_ADCMSPS;
	}
	else if(strcmp(ChanParName,"ExtTrigStretch") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&exttrigstretch, 1, (unsigned int)(ExtTrigStretch_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[ExtTrigStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = exttrigstretch;
		
		// Update channel parameter ExtTrigStretch
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			*ChanParData = (double)exttrigstretch / (double)Module_Information[ModNum].Module_ADCMSPS;
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			*ChanParData = (double)exttrigstretch / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)exttrigstretch / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
	}
	else if(strcmp(ChanParName,"VetoStretch") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&vetostretch, 1, (unsigned int)(VetoStretch_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[VetoStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = vetostretch;
		
		// Update channel parameter VetoStretch
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			*ChanParData = (double)vetostretch / (double)Module_Information[ModNum].Module_ADCMSPS;
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			*ChanParData = (double)vetostretch / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)vetostretch / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
	}
	else if(strcmp(ChanParName,"MultiplicityMaskL") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&multiplicitymaskl, 1, (unsigned int)(MultiplicityMaskL_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[MultiplicityMaskL_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = multiplicitymaskl;
		
		// Update channel parameter MultiplicityMaskL
		*ChanParData = (double)multiplicitymaskl;
	}
	else if(strcmp(ChanParName,"MultiplicityMaskH") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&multiplicitymaskh, 1, (unsigned int)(MultiplicityMaskH_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[MultiplicityMaskH_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = multiplicitymaskh;
		
		// Update channel parameter MultiplicityMaskH
		*ChanParData = (double)multiplicitymaskh;
	}
	else if(strcmp(ChanParName,"ExternDelayLen") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&externdelaylen, 1, (unsigned int)(ExternDelayLen_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[ExternDelayLen_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = externdelaylen;
		
		// Update channel parameter ExternDelayLen
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			*ChanParData = (double)externdelaylen / (double)Module_Information[ModNum].Module_ADCMSPS;
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			*ChanParData = (double)externdelaylen / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)externdelaylen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
	}
	else if(strcmp(ChanParName,"FtrigoutDelay") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&ftrigoutdelay, 1, (unsigned int)(FtrigoutDelay_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[FtrigoutDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = ftrigoutdelay;
		
		// Update channel parameter FtrigoutDelay
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			*ChanParData = (double)ftrigoutdelay / (double)Module_Information[ModNum].Module_ADCMSPS;
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			*ChanParData = (double)ftrigoutdelay / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)ftrigoutdelay / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
	}
	else if(strcmp(ChanParName,"ChanTrigStretch") == 0)
	{
		// Read from the selected Pixie module
		Pixie16IMbufferIO(&chantrigstretch, 1, (unsigned int)(ChanTrigStretch_Address[ModNum] + ChanNum), MOD_READ, ModNum);
		Pixie_Devices[ModNum].DSP_Parameter_Values[ChanTrigStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = chantrigstretch;
		
		// Update channel parameter ChanTrigStretch
		if(Module_Information[ModNum].Module_ADCMSPS == 100)
			*ChanParData = (double)chantrigstretch / (double)Module_Information[ModNum].Module_ADCMSPS;
		else if(Module_Information[ModNum].Module_ADCMSPS == 250)
			*ChanParData = (double)chantrigstretch / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
		else if(Module_Information[ModNum].Module_ADCMSPS == 500)
			*ChanParData = (double)chantrigstretch / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadSglChanPar): invalid channel parameter name %s", ChanParName);
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}
	
	return(0);
}


/****************************************************************
*	Pixie16ReadHistogramFromModule:
*		Retrieve histogram data from a Pixie module.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Invalid Pixie channel number
*			-3 - Failed to get the histogram data
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadHistogramFromModule (
	unsigned int   *Histogram,  // histogram data
	unsigned int   NumWords,    // number of words to be read out
	unsigned short ModNum,      // module number
	unsigned short ChanNum)     // channel number
{
	int retval;    // return values
	unsigned int Histo_Address;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if ModNum is valid
	if( ModNum >= Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadHistogramFromModule): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	if( ChanNum >= NUMBER_OF_CHANNELS )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadHistogramFromModule): invalid Pixie channel number %d", ChanNum);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	Histo_Address = MAX_HISTOGRAM_LENGTH * ChanNum + HISTOGRAM_MEMORY_ADDRESS;
	retval = Pixie_Main_Memory_IO(Histogram, Histo_Address, NumWords, MOD_READ, ModNum);
	if(retval < 0)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadHistogramFromModule): failed to get histogram data from module %d, retval=%d", ModNum, retval);
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}

	return(0);
}


/****************************************************************
*	Pixie16ReadStatisticsFromModule:
*		Retrieve statistics data from a Pixie module.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Failed to get statistics data
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadStatisticsFromModule (
	unsigned int   *Statistics,   // run statistics data
	unsigned short ModNum )       // module number
{
	int retval;    // return values
	unsigned short nWords, k;       // number of words
	unsigned int   DSP_address;     // Start address in DSP memory
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if ModNum is valid
	if( ModNum >= Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadStatisticsFromModule): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	nWords = N_DSP_PAR - DSP_IO_BORDER;
	
	// Check if running in OFFLINE mode
	if( Offline == 1 )
	{
		for(k=0; k<nWords; k+=1)
		{
			Statistics[k] = Pixie_Devices[ModNum].DSP_Parameter_Values[DSP_IO_BORDER + k];
		}
	}
	else
	{
		DSP_address = DATA_MEMORY_ADDRESS + DSP_IO_BORDER;
		retval = Pixie_DSP_Memory_IO(Statistics, DSP_address, nWords, MOD_READ, ModNum);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16ReadStatisticsFromModule): failed to get statistics data from module %d, retval=%d", ModNum, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
	}
	
	return(0);
}


/****************************************************************
*	Pixie16SaveHistogramToFile:
*		Retrieve histogram data from a Pixie module and then save
*		the data to a file.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Failed to get histogram data from module
*			-3 - Failed to allocate memory to store histogram data for binary file
*			-4 - Failed to open histogram data file
*			-5 - Failed to open mca ascii output file
*			-6 - Failed to allocate memory to store histogram data for ascii text file
*			-7 - Failed to read histogram data from file
*			-8 - Failed to read run statistics data from module
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveHistogramToFile (
	char *FileName,          // histogram data file name
	unsigned short ModNum)   // module number
{
	
	char ErrMSG[MAX_ERRMSG_LENGTH];
	FILE *HistFile = NULL;
	unsigned int *histdata;
	unsigned int histo_addr;
	unsigned short k;
	int retval;
	unsigned int index;
	unsigned int *MCAData[NUMBER_OF_CHANNELS];
	unsigned short ModuleNumber, ChannelNumber;
	unsigned short strlength;
	FILE *outfil_mca;
	char outfilename[1024];
	unsigned int run_statistics[448];
	double realtime, livetime, icr, ocr;
	
	// Check if ModNum is valid
	if( ModNum > Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16SaveHistogramToFile): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}

	HistFile = fopen(FileName, "wb");
	if(HistFile != NULL)
	{
		// Allocate memory
		if( (histdata = (unsigned int *)malloc(sizeof(unsigned int) * MAX_HISTOGRAM_LENGTH)) != NULL)
		{
			for(k=0; k<NUMBER_OF_CHANNELS; k++)
			{
				histo_addr = MAX_HISTOGRAM_LENGTH * k + HISTOGRAM_MEMORY_ADDRESS;
				retval = Pixie_Main_Memory_IO(histdata, histo_addr, MAX_HISTOGRAM_LENGTH, MOD_READ, ModNum);
				if(retval < 0)
				{
					// Release memory
					free(histdata);
					
					// Close HistFile
					fclose(HistFile);

					sprintf(ErrMSG, "*ERROR* (Pixie16SaveHistogramToFile): failed to get histogram data from module %d, retval=%d", ModNum, retval);
					Pixie_Print_MSG(ErrMSG);
					return(-2);
				}
				
				fwrite(histdata, sizeof(unsigned int), MAX_HISTOGRAM_LENGTH, HistFile);
			}
			
			// Release memory
			free(histdata);
		}
		else
		{
			// Close HistFile
			fclose(HistFile);

			sprintf(ErrMSG, "*ERROR* (Pixie16SaveHistogramToFile): failed to allocate memory to store list mode data for module %d", ModNum);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}
		
		// Close HistFile
		fclose(HistFile);
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16SaveHistogramToFile): failed to open histogram data file %s", FileName);
		Pixie_Print_MSG(ErrMSG);
		return(-4);
	}


	// Save MCA data to files in ASCII text format
	
	// Generate histogram output file name
	strlength = strlen(FileName);

	strncpy(outfilename, FileName, strlength - 3);
	outfilename[strlength - 3] = '\0';
	strcat(outfilename, "asc");
	outfil_mca = fopen(outfilename, "w");
	if(outfil_mca == NULL)
	{
		sprintf(ErrMSG, "*Error* (Pixie16SaveHistogramToFile): open mca output file %s failed", outfilename);
		Pixie_Print_MSG(ErrMSG);
		return(-5);
	}

	for(ChannelNumber = 0; ChannelNumber < NUMBER_OF_CHANNELS; ChannelNumber ++)
	{
		MCAData[ChannelNumber] = (unsigned int *)malloc(sizeof(unsigned int) * MAX_HISTOGRAM_LENGTH);
		if(MCAData[ChannelNumber] == NULL)
		{
			sprintf(ErrMSG, "*Error* (Pixie16SaveHistogramToFile): failed to allocate memory for MCA data");
			Pixie_Print_MSG(ErrMSG);

			for(index = 0; index < ChannelNumber; index ++)
			{
				free(MCAData[index]);
			}

			// Close histogram output file
			fclose(outfil_mca);

			return(-6);
		}
	}
	
	ModuleNumber = 0;
	for(ChannelNumber = 0; ChannelNumber < NUMBER_OF_CHANNELS; ChannelNumber ++)
	{
		retval = Pixie16ReadHistogramFromFile(FileName, MCAData[ChannelNumber], MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*Error* (Pixie16SaveHistogramToFile): Pixie16ReadHistogramFromFile failed, retval=%d", retval);
			Pixie_Print_MSG(ErrMSG);
			fclose(outfil_mca);
			for(index = 0; index < NUMBER_OF_CHANNELS; index ++)
			{
				free(MCAData[index]);
			}
			return(-7);
		}
	}

	// Write MCA data
	for(index = 0; index < NUMBER_OF_CHANNELS; index ++)
	{
		fprintf(outfil_mca, "Channel#%d\t", index);
	}
	fprintf(outfil_mca, "\n");

	for(index = 0; index < MAX_HISTOGRAM_LENGTH; index ++)
	{
		for(ChannelNumber = 0; ChannelNumber < NUMBER_OF_CHANNELS; ChannelNumber ++)
		{
			fprintf(outfil_mca, "%d\t", MCAData[ChannelNumber][index]);
		}

		fprintf(outfil_mca, "\n");
	}		
		
	for(index = 0; index < NUMBER_OF_CHANNELS; index ++)
	{
		free(MCAData[index]);
	}


	// Append run statistics data to the ASCII MCA data file
	
	// Read raw run statistics data from the module
	retval = Pixie16ReadStatisticsFromModule(run_statistics, ModNum);
	if(retval < 0)
	{
		sprintf(ErrMSG, "*Error* (Pixie16SaveHistogramToFile): Pixie16ReadStatisticsFromModule failed, retval=%d", retval);
		Pixie_Print_MSG(ErrMSG);
		fclose(outfil_mca);
		return(-8);
	}

	// Compute real time
	realtime = Pixie16ComputeRealTime(run_statistics, ModNum);
	
	// Write real time to file
	fprintf(outfil_mca, "\nReal Time [s] = %f\n", realtime);

	// Write live time to file
	fprintf(outfil_mca, "\nLive Time [s]\n");

	for(index = 0; index < NUMBER_OF_CHANNELS; index ++)
	{
		fprintf(outfil_mca, "Channel#%d\t", index);
	}
	fprintf(outfil_mca, "\n");

	for(index = 0; index < NUMBER_OF_CHANNELS; index ++)
	{
		ChannelNumber = (unsigned short)index;
		livetime = Pixie16ComputeLiveTime(run_statistics, ModNum, ChannelNumber);
		fprintf(outfil_mca, "%f\t", livetime);
	}
	fprintf(outfil_mca, "\n");

	// Write input count rate to file
	fprintf(outfil_mca, "\nInput Count Rate [cps]\n");

	for(index = 0; index < NUMBER_OF_CHANNELS; index ++)
	{
		fprintf(outfil_mca, "Channel#%d\t", index);
	}
	fprintf(outfil_mca, "\n");

	for(index = 0; index < NUMBER_OF_CHANNELS; index ++)
	{
		ChannelNumber = (unsigned short)index;
		icr = Pixie16ComputeInputCountRate(run_statistics, ModNum, ChannelNumber);
		fprintf(outfil_mca, "%f\t", icr);
	}
	fprintf(outfil_mca, "\n");

	// Write output count rate to file
	fprintf(outfil_mca, "\nOutput Count Rate [cps]\n");

	for(index = 0; index < NUMBER_OF_CHANNELS; index ++)
	{
		fprintf(outfil_mca, "Channel#%d\t", index);
	}
	fprintf(outfil_mca, "\n");

	for(index = 0; index < NUMBER_OF_CHANNELS; index ++)
	{
		ChannelNumber = (unsigned short)index;
		ocr = Pixie16ComputeOutputCountRate(run_statistics, ModNum, ChannelNumber);
		fprintf(outfil_mca, "%f\t", ocr);
	}
	fprintf(outfil_mca, "\n");

	// Close MCA ASCII dta file
	fclose(outfil_mca);

	return(0);
	
}


/****************************************************************
*	Pixie16GetModuleEvents:
*		Parse the list mode events from the list mode data file to
*		get the number of events for each module
*
*		Return Value:
*			 0 - Success
*			-1 - Null pointer *ModuleEvents
*			-2 - Failed to open list mode data file
*			-3 - Module number read out is invalid
*			-4 - Run task read out is invalid
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16GetModuleEvents (
	char *FileName,                // the list mode data file name (with complete path)
	unsigned int *ModuleEvents )   // receives number of events for each module
{
	
	unsigned int eventdata, eventlength;
	unsigned int TotalWords, TotalSkippedWords;
	char  ErrMSG[MAX_ERRMSG_LENGTH];
	FILE *ListModeFile = NULL;
	
	// Check if ModuleEvents is valid
	if(ModuleEvents == NULL)
	{
		sprintf(ErrMSG, "*Error* (Pixie16GetModuleEvents): Null pointer *ModuleEvents");
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	// Open the list mode file
	ListModeFile = fopen(FileName, "rb");
	if(ListModeFile != NULL)
	{	
		// Get file length
		fseek(ListModeFile, 0, SEEK_END);
		TotalWords = (ftell(ListModeFile) + 1) / 4;
		
		// Point ListModeFile to the beginning of file
		fseek(ListModeFile, 0, SEEK_SET);
		
		// Initialize indicator and counter
		TotalSkippedWords = 0;
		
		do
		{
			fread(&eventdata, 4, 1, ListModeFile);
			eventlength = (eventdata & 0x3FFE0000) >> 17;
			TotalSkippedWords += eventlength;
			fseek(ListModeFile, (eventlength-1)*4, SEEK_CUR);
			ModuleEvents[0] ++;
		}while( TotalSkippedWords < TotalWords );
		
		fclose(ListModeFile);
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16GetModuleEvents): can't open list mode data file %s", FileName);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	return(0);
}


/****************************************************************
*	Pixie16GetEventsInfo:
*		Retrieve the detailed information of each event in the list
*		mode data file
*
*		Return Value:
*			 0 - Success
*			-1 - Null pointer *EventInformation
*			-2 - Target module number is invalid
*			-3 - Failed to open list mode data file
*			-4 - Module number read out is invalid
*			-5 - Run task read out is invalid
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16GetEventsInfo (
	char *FileName,                    // the list mode data file name (with complete path)
	unsigned int *EventInformation,    // to hold event information
	unsigned short ModuleNumber)       // the module whose events are to be retrieved
{
	
	unsigned int eventdata, headerlength, eventlength;
	unsigned int TotalWords, TotalSkippedWords, NumEvents;
	char  ErrMSG[MAX_ERRMSG_LENGTH];
	FILE *ListModeFile = NULL;
	
	// Check if EventInformation is valid
	if(EventInformation == NULL)
	{
		sprintf(ErrMSG, "*Error* (Pixie16GetEventsInfo): Null pointer *EventInformation");
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	if(ModuleNumber >= PRESET_MAX_MODULES)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16GetEventsInfo): Target module number is invalid %d", ModuleNumber);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	// Open the list mode file
	ListModeFile = fopen(FileName, "rb");
	if(ListModeFile != NULL)
	{	
		// Get file length
		fseek(ListModeFile, 0, SEEK_END);
		TotalWords = (ftell(ListModeFile) + 1) / 4;
		// Point ListModeFile to the beginning of file
		fseek(ListModeFile, 0, SEEK_SET);
		
		// Initialize indicator and counter
		TotalSkippedWords = 0;
		NumEvents = 0;
		
		do
		{
			fread(&eventdata, 4, 1, ListModeFile);
			
			// Event #
			EventInformation[EVENT_INFO_LENGTH * NumEvents]     = NumEvents;
			// Channel #
			EventInformation[EVENT_INFO_LENGTH * NumEvents + 1] = (eventdata & 0xF);	
			// Slot #
			EventInformation[EVENT_INFO_LENGTH * NumEvents + 2] = (eventdata & 0xF0) >> 4;	
			// Crate #
			EventInformation[EVENT_INFO_LENGTH * NumEvents + 3] = (eventdata & 0xF00) >> 8;	
			// Header length
			headerlength = (eventdata & 0x1F000) >> 12;
			EventInformation[EVENT_INFO_LENGTH * NumEvents + 4] = (eventdata & 0x1F000) >> 12;	
			// Event length
			eventlength = (eventdata & 0x3FFE0000) >> 17;
			EventInformation[EVENT_INFO_LENGTH * NumEvents + 5] = (eventdata & 0x3FFE0000) >> 17;	
			// Finish code
			EventInformation[EVENT_INFO_LENGTH * NumEvents + 6] = (eventdata & 0x80000000) >> 31;	

			fread(&eventdata, 4, 1, ListModeFile);
			// EventTime_Low
			EventInformation[EVENT_INFO_LENGTH * NumEvents + 7] = eventdata;	

			fread(&eventdata, 4, 1, ListModeFile);
			// EventTime_High
			EventInformation[EVENT_INFO_LENGTH * NumEvents + 8] = (eventdata & 0xFFFF);

			fread(&eventdata, 4, 1, ListModeFile);
			// Event Energy
			EventInformation[EVENT_INFO_LENGTH * NumEvents + 9] = (eventdata & 0xFFFF);
			// Trace Length
			EventInformation[EVENT_INFO_LENGTH * NumEvents + 10]= (eventdata & 0xFFFF0000) >> 16;
			// Trace location
			EventInformation[EVENT_INFO_LENGTH * NumEvents + 11]= TotalSkippedWords + headerlength;

			TotalSkippedWords += eventlength;
			NumEvents ++;
			fseek(ListModeFile, (eventlength - 4) * 4, SEEK_CUR);
		}while( TotalSkippedWords < TotalWords );
		
		fclose(ListModeFile);
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16GetEventsInfo): can't open list mode data file %s", FileName);
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}
	
	return(0);
}



/****************************************************************
*	Pixie16ReadListModeTrace:
*		Retrieve list mode data from a file.
*
*		Return Value:
*			 0 - Success
*			-1 - Failed to open list mode data file
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadListModeTrace (
	char *FileName,                 // list mode data file name
	unsigned short *Trace_Data,     // list mode trace data (16-bit words)
	unsigned short NumWords,        // number of 16-bit words to be read out
	unsigned int FileLocation)      // the location of the trace in the file
{
	
	char ErrMSG[MAX_ERRMSG_LENGTH];
	FILE *ListModeFile = NULL;
	
	// Open the list mode file
	ListModeFile = fopen(FileName, "rb");
	if(ListModeFile != NULL)
	{
		// Position ListModeFile to the requested trace location
		fseek(ListModeFile, FileLocation*4, SEEK_SET);
				
		// Read trace
		fread(Trace_Data, 2, NumWords, ListModeFile);
		
		fclose(ListModeFile);
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadListModeTrace): can't open list mode file %s", FileName);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	return(0);
	
}


/****************************************************************
*	Pixie16ReadHistogramFromFile:
*		Retrieve histogram data from a file.
*
*		Return Value:
*			 0 - Success
*			-1 - Failed to open the histogram file
*			-2 - No histogram data is available for this channel
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadHistogramFromFile (
	char  *FileName,            // the histogram data file name (with complete path)
	unsigned int   *Histogram,  // histogram data
	unsigned int   NumWords,    // number of words to be read out
	unsigned short ModNum,      // module number
	unsigned short ChanNum)     // channel number
{
	
	unsigned int Histo_Address, TotalWords;
	char  ErrMSG[MAX_ERRMSG_LENGTH];
	FILE *HistogramFile = NULL;
	
	// Open the histogram data file 
	HistogramFile = fopen(FileName, "rb");
	if(HistogramFile != NULL)
	{	
		// Get file length 
		fseek(HistogramFile, 0, SEEK_END);
		TotalWords = (ftell(HistogramFile) + 1) / 4;
		// Point HistogramFile to the beginning of file 
		fseek(HistogramFile, 0, SEEK_SET);

		Histo_Address = MAX_HISTOGRAM_LENGTH * ChanNum;
		if(Histo_Address > (TotalWords - NumWords)) 
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16ReadHistogramFromFile): no histogram data is available in file %s for channel %d of module %d", FileName, ChanNum, ModNum);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
		
		// Read the data
		
		// Point HistogramFile to the right location of the histogram data file
		fseek(HistogramFile, Histo_Address * 4, SEEK_CUR);
		fread(Histogram, 4, NumWords, HistogramFile);
		
		// Close the file
		fclose(HistogramFile);
		
		return(0);
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadHistogramFromFile): failed to open histogram file %s", FileName);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
}


/****************************************************************
*	Pixie16SaveDSPParametersToFile:
*		Save DSP parameters to a settings file.
*
*		Return Value:
*			 0 - Success
*			-1 - Failed to read DSP parameter values from the Pixie-16 modules
*			-2 - Failed to open the DSP parameters file
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveDSPParametersToFile (
	char  *FileName )     // the DSP parameters file name (with complete path)
{
	unsigned short ModNum;
	char  ErrMSG[MAX_ERRMSG_LENGTH];
	FILE *DSPSettingsFile = NULL;
	int retval;
	
	// Open the DSP parameters file
	DSPSettingsFile = fopen(FileName, "wb");
	if(DSPSettingsFile != NULL)
	{
		// Get the current DSP parameter values from the Pixie-16 board
		for(ModNum = 0; ModNum < Number_Modules; ModNum ++)
		{
			retval = Pixie16IMbufferIO(Pixie_Devices[ModNum].DSP_Parameter_Values, N_DSP_PAR, DATA_MEMORY_ADDRESS, MOD_READ, ModNum);
			if(retval < 0)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16SaveDSPParametersToFile): failed to read DSP parameter values from module %d, retval = %d", ModNum, retval);
				Pixie_Print_MSG(ErrMSG);
				return(-1);
			}
		}
		
		// Write DSP parameter values to the settings file
		for(ModNum = 0; ModNum < PRESET_MAX_MODULES; ModNum ++)
		{
			fwrite(Pixie_Devices[ModNum].DSP_Parameter_Values, sizeof(unsigned int), N_DSP_PAR, DSPSettingsFile);
		}
		
		// Close the file
		fclose(DSPSettingsFile);
		
		return(0);
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16SaveDSPParametersToFile): failed to open DSP parameters file %s", FileName);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
}


/****************************************************************
*	Pixie16LoadDSPParametersFromFile:
*		Load DSP parameters from a settings file.
*
*		Return Value:
*			 0 - Success
*			-1 - Size of DSPParFile is invalid
*			-2 - Failed to program Fippi in module
*			-3 - Failed to set DACs in module
*			-4 - Failed to open the DSP parameters file
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16LoadDSPParametersFromFile (
	char  *FileName )     // the DSP parameters file name (with complete path)
{
	unsigned short k;
	unsigned int TotalWords;
	int retval;
	char  ErrMSG[MAX_ERRMSG_LENGTH];
	FILE *DSPSettingsFile = NULL;
	
	// Open DSP parameters file
	if( (DSPSettingsFile = fopen(FileName, "rb")) != NULL )  // Make sure DSPSettingsFile is opened successfully
	{
		// Check if file size is consistent with predefined length (N_DSP_PAR * PRESET_MAX_MODULES)
		fseek(DSPSettingsFile, 0, SEEK_END);
		TotalWords = (ftell(DSPSettingsFile) + 1) / 4;
		if(TotalWords != (N_DSP_PAR * PRESET_MAX_MODULES))
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16LoadDSPParametersFromFile): size of DSPParFile is invalid. Check DSPParFile name %s", FileName);
			Pixie_Print_MSG(ErrMSG);
			// close opened files
			fclose(DSPSettingsFile);
			return(-1);
		}
		
		// Point configfil to the beginning of file
		fseek(DSPSettingsFile, 0, SEEK_SET);
		
		// Read DSP parameters
		for(k=0; k<PRESET_MAX_MODULES; k++)
		{
			fread(&Pixie_Devices[k].DSP_Parameter_Values[0], sizeof(unsigned int), N_DSP_PAR, DSPSettingsFile);
			
			// Force correct module number
			Pixie_Devices[k].DSP_Parameter_Values[0] = k;
		}
		
		// Download to all modules
		for(k=0; k<Number_Modules; k++)
		{
			Pixie_DSP_Memory_IO(&Pixie_Devices[k].DSP_Parameter_Values[0], DATA_MEMORY_ADDRESS, DSP_IO_BORDER, MOD_WRITE, k);
			
			// Always re-program fippi after downloading DSP parameters
			retval = Pixie16ProgramFippi(k);
			if(retval < 0)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16LoadDSPParametersFromFile): failed to program Fippi in module %d, retval=%d", k, retval);
				Pixie_Print_MSG(ErrMSG);
				// close opened files
				fclose(DSPSettingsFile);
				return(-2);
			}
			
			retval = Pixie16SetDACs(k);
			if(retval < 0)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16LoadDSPParametersFromFile): failed to set DACs in module %d, retval=%d", k, retval);
				Pixie_Print_MSG(ErrMSG);
				// close opened files
				fclose(DSPSettingsFile);
				return(-3);
			}
		}
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16LoadDSPParametersFromFile): failed to open DSP parameters file %s", FileName);
		Pixie_Print_MSG(ErrMSG);
		return(-4);
	}
	
	return(0);
}


/****************************************************************
*	Pixie16CopyDSPParameters:
*		Copy DSP parameters from one module to other modules.
*
*		Return Value:
*			 0 - Success
*			-1 - Failed to program Fippi in a module
*			-2 - Failed to set DACs in a module
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CopyDSPParameters (
	unsigned short BitMask,            // copy items bit mask
	unsigned short SourceModule,       // source module
	unsigned short SourceChannel,      // source channel
	unsigned short *DestinationMask )  // the destination module and channel bit mask
{
	unsigned short i, j, k;
	int retval;
	char  ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Copy settings to the destination modules and channels
	for(i=0; i<Number_Modules; i++)
	{
		for(j=0; j<NUMBER_OF_CHANNELS; j++)
		{
			if( DestinationMask[i * NUMBER_OF_CHANNELS + j] == 1 )
			{
				Pixie_CopyDSPParameters(BitMask, SourceModule, SourceChannel, i, j);
			}
		}
	}
	
	// Download to all modules
	for(k=0; k<Number_Modules; k++)
	{
		Pixie_DSP_Memory_IO(&Pixie_Devices[k].DSP_Parameter_Values[0], DATA_MEMORY_ADDRESS, DSP_IO_BORDER, MOD_WRITE, k);
		
		// Always re-program fippi after downloading DSP parameters
		retval = Pixie16ProgramFippi(k);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16CopyDSPParameters): failed to program Fippi in module %d, retval=%d", k, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-1);
		}
		
		retval = Pixie16SetDACs(k);
		if(retval < 0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16CopyDSPParameters): failed to set DACs in module %d, retval=%d", k, retval);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
	}
	
	return(0);
}


/****************************************************************
*	Pixie16SaveExternalFIFODataToFile:
*		Read list mode data from the external FIFO of a Pixie module
*		and then save the data to a file.
*
*		Return Value:
*			 0 - Success
*			-1 - Invalid Pixie module number
*			-2 - Failed to allocate memory to store list mode data
*			-3 - Failed to open list mode data file
*			-4 - Failed to read external FIFO status
*			-5 - Failed to read data from external FIFO 
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveExternalFIFODataToFile (
	char *FileName,              // list mode data file name
	unsigned int   *nFIFOWords,  // number of words read from external FIFO
	unsigned short ModNum,       // module number
	unsigned short EndOfRunRead) // indicator whether this is the end of run read
{
	unsigned int nWords;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	FILE *ListFile = NULL;
	unsigned int *lmdata;
	int retval;
	
	// Check if ModNum is valid
	if( ModNum >= Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16SaveExternalFIFODataToFile): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		*nFIFOWords = 0;
		return(-1);
	}
	
	// Allocate memory
	if( (lmdata = (unsigned int *)malloc(sizeof(unsigned int) * EXTERNAL_FIFO_LENGTH)) == NULL)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16SaveExternalFIFODataToFile): failed to allocate memory to store list mode data for module %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		*nFIFOWords = 0;
		return(-2);
	}

	ListFile = fopen(FileName, "ab");
	if(ListFile != NULL)
	{
		// Check how many words are in the external FIFO
		retval=Pixie_Read_ExtFIFOStatus(&nWords, ModNum);
		if(retval<0)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie16SaveExternalFIFODataToFile): failed to read external FIFO status in module %d, retval = %d", ModNum, retval);
			Pixie_Print_MSG(ErrMSG);
			fclose(ListFile);
			free(lmdata);
			*nFIFOWords = 0;
			return(-4);
		}

		// Read data out from the external FIFO if the the number of words exceeds threshold
		// if EndOfRunRead == 1, check if nWords is greater than 0
		if( ((EndOfRunRead == 0) && (nWords > EXTFIFO_READ_THRESH)) || ((EndOfRunRead == 1) && (nWords > 0)) )
		{
			retval = Pixie_ExtFIFO_Read(lmdata, nWords, ModNum);
			if(retval < 0)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie16SaveExternalFIFODataToFile): failed to read data from external FIFO in module %d, retval = %d", ModNum, retval);
				Pixie_Print_MSG(ErrMSG);
				fclose(ListFile);
				free(lmdata);
				*nFIFOWords = 0;
				return(-5);
			}
			else
			{
				fwrite(lmdata, 4, nWords, ListFile);
				*nFIFOWords = nWords;
			}
		}
		else
		{
			*nFIFOWords = 0;
		}
	}
	else
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16SaveExternalFIFODataToFile): failed to open list mode data file %s", FileName);
		Pixie_Print_MSG(ErrMSG);
		free(lmdata);
		*nFIFOWords = 0;
		return(-3);
	}

	fclose(ListFile);
	free(lmdata);

	return(0);
}


/****************************************************************
*	Pixie16RegisterIO:
*		Read from or write to a register in a module.
*
*		Return Value:
*			 0 - Successful
*			-1 - Invalid Pixie module number
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16RegisterIO (
	unsigned short ModNum,     // the Pixie module to communicate to
	unsigned int   address,    // register address
	unsigned short direction,  // either MOD_READ or MOD_WRITE
	unsigned int   *value)     // holds or receives the data
{
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if ModNum is valid
	if( ModNum >= Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16RegisterIO): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}

	Pixie_Register_IO(ModNum, address, direction, value);
	return(0);
}


/****************************************************************
*	Pixie16ReadCSR:
*		Read the value of the CSR of a module.
*
*		Return Value:
*			 0 - Successful
*			-1 - Invalid Pixie module number
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadCSR (
	unsigned short ModNum,
	unsigned int   *CSR)
{
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if ModNum is valid
	if( ModNum >= Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadCSR): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}

	Pixie_ReadCSR(ModNum, CSR);
	return(0);
}


/****************************************************************
*	Pixie16WriteCSR:
*		Write a value to the CSR of a module.
*
*		Return Value:
*			 0 - Successful
*			-1 - Invalid Pixie module number
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteCSR (
	unsigned short ModNum,
	unsigned int   CSR)
{
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if ModNum is valid
	if( ModNum >= Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16WriteCSR): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}

	Pixie_WrtCSR(ModNum, CSR);
	return(0);
}


/****************************************************************
*	Pixie16CheckExternalFIFOStatus:
*		Read the number of 32-bit words that the external FIFO
*		currently has.
*
*		Return Value:
*			 0 - Successful
*			-1 - Invalid Pixie module number
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CheckExternalFIFOStatus (
	unsigned int   *nFIFOWords,
	unsigned short ModNum)
{
	int retval;    // return values
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if ModNum is valid
	if( ModNum >= Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16CheckExternalFIFOStatus): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}

	retval = Pixie_Read_ExtFIFOStatus(nFIFOWords, ModNum);
	return(retval);
}


/****************************************************************
*	Pixie16ReadDataFromExternalFIFO:
*		Read data from the external FIFO.
*
*		Return Value:
*			 0 - Successful
*			-1 - Invalid Pixie module number
*			-2 - Failed to read data from external FIFO
*
****************************************************************/

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadDataFromExternalFIFO (
	unsigned int   *ExtFIFO_Data,// To receive the external FIFO data
	unsigned int   nFIFOWords,   // number of words to read from external FIFO
	unsigned short ModNum )      // module number
{
	int retval;    // return values
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	// Check if ModNum is valid
	if( ModNum >= Number_Modules )
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadDataFromExternalFIFO): invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
	
	retval = Pixie_ExtFIFO_Read(ExtFIFO_Data, nFIFOWords, ModNum);
	if(retval < 0)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie16ReadDataFromExternalFIFO): failed to read data from external FIFO in module %d, retval=%d", ModNum, retval);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	return(0);
}
