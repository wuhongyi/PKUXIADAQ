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
*		pixie16sys.c
*
* Description:
*
*		This file contains all the Pixie16 library functions that can be accessed
*		externally by the user application library code.
*
* $Rev: 27118 $
* $Id: pixie16sys.c 27118 2013-05-17 01:55:17Z htan $
******************************************************************************/

#include "PlxApi.h"
#include "def21160.h"

#include "pixie16sys_common.h"
#include "pixie16sys_defs.h"
#include "pixie16sys_globals.h"
#include "pixie16sys_export.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
#include <windows.h>
#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI
#include <unistd.h>
#include <ctype.h>
#endif

char *PCISysIniFile = {"pxisys.ini"};


/****************************************************************
*	Pixie_InitSystem:
*		Initialize Pixie16 system by mapping PXI slots to Pixie16
*		modules and assigning vitural addresses to the PLX9054 chips
*		of those modules.
*
*		Return Value:
*			 0 - Successful
*			-1 - Failed to measure host computer speed (ns per cycle)
*			-2 - Can't open PXI system initialization file
*			-3 - Unable to close the PLX device
*			-4 - Unable to find module 0 in the system
*			-5 - Could not open PCI Device
*			-6 - Unable to map a PCI BAR and obtain a virtual address
*
****************************************************************/

int Pixie_InitSystem (unsigned short NumModules, unsigned short *PXISlotMap, unsigned short OfflineMode)
{	
	PLX_DEVICE_KEY    DeviceKey;
	PLX_STATUS        rc;
	
	FILE *pci_ini_file = NULL;  // File pointer
	char sbuffer[100]  = {0};   // Temporary buffer
	char ErrMSG[MAX_ERRMSG_LENGTH], BusNoOffset;
	unsigned int  ModSerNum;
	unsigned char PCIDeviceNum[SYS_MAX_NUM_MODULES], PCIBusNum[SYS_MAX_NUM_MODULES];
	unsigned char PlxBusNum[SYS_MAX_NUM_MODULES], PlxDeviceNum[SYS_MAX_NUM_MODULES];
	unsigned char Bus_Number[SYS_MAX_NUM_MODULES];        // PCI bus number for each module
	unsigned char Device_Number[SYS_MAX_NUM_MODULES];     // PCI device number for each module
	unsigned char TotalPlx9054Devices, PlxModIndex[SYS_MAX_NUM_MODULES];
	unsigned int  i;
	unsigned char slotnum, minPCIBusNum, minPLXBusNum, minSlotNum;
	unsigned char MappedPCIBusCount, FoundPCIBusCount, CurPCIBus;
	unsigned short k, m;
	int retval;

	// First, update a few global variables
	SYS_Number_Modules = NumModules;  // the total number modules in the crate
	SYS_Offline = OfflineMode;        // SYS_Offline = 1: offline mode; SYS_Offline = 0: Online mode
	
	if(SYS_Offline == 1)  // Returns immediately for offline analysis
	{
		return(0);
	}
	
	// Measure host computer speed (ns per cycle)
	retval = get_ns_per_cycle(&Ns_Per_Cycle);
	if(retval < 0)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie_InitSystem): Failed to measure host computer speed (ns per cycle), retval = %d", retval);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}

#if(PRINT_DEBUG_MSG == 1)
	sprintf(ErrMSG, "(Pixie_InitSystem): Host computer speed (ns per cycle) = %f", Ns_Per_Cycle);
	Pixie_Print_MSG(ErrMSG);
#endif
	
	// Open input file for reading
	pci_ini_file = fopen(PCISysIniFile, "r");
	if(pci_ini_file == NULL) // File can not be opened
	{
		sprintf(ErrMSG, "*ERROR* (Pixie_InitSystem): Could not open PXI system initialization file %s for reading", PCISysIniFile);
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}
	
	// Read PCIDeviceNumber from the pxisys.ini file 
	do{
		fgets(sbuffer, 100, pci_ini_file);
		if(sbuffer[0] == '[')
		{
			if (sbuffer[6] == ']')
			{	
				sbuffer[6] = '\0';
				slotnum = (unsigned char)atoi(& sbuffer[5]);
			}
			else
			{ 
				sbuffer[7] = '\0';
				slotnum = (unsigned char)atoi(& sbuffer[5]);
			}

			fgets(sbuffer, 100, pci_ini_file);
			fgets(sbuffer, 100, pci_ini_file);
			fgets(sbuffer, 100, pci_ini_file);
			fgets(sbuffer, 100, pci_ini_file);
			PCIBusNum[slotnum] = (unsigned char)atoi(& sbuffer[15]);
			fgets(sbuffer, 100, pci_ini_file);
			PCIDeviceNum[slotnum] = (unsigned char)atoi(& sbuffer[18]);
		}
		
		if(feof(pci_ini_file))
			break;
		
	}while(1);
	fclose(pci_ini_file);

	// Set the Device_Number and Bus_Number for each Pixie-16 module installed in the system
	minPCIBusNum = MAX_PCI_BUS;
	k = 0;
	MappedPCIBusCount = 0;
	CurPCIBus = 0xFF;
	minSlotNum = 0;
	do
	{
		Bus_Number[k] = PCIBusNum[*(PXISlotMap + k)];
		Device_Number[k] = PCIDeviceNum[*(PXISlotMap + k)];
	
		// Update minPLXBusNum
		if(minPCIBusNum > Bus_Number[k])
		{
			minPCIBusNum = Bus_Number[k];
		}

		// Check if there is a new PCI bus
		if(CurPCIBus != Bus_Number[k])
		{
			CurPCIBus = Bus_Number[k];
			MappedPCIBusCount ++;
			minSlotNum = (unsigned char)*(PXISlotMap + k);
		}
		k ++; 
	}while(k < SYS_Number_Modules);
	
	// Find all the PLX devices installed in the system	
	TotalPlx9054Devices = 0;
	minPLXBusNum = MAX_PCI_BUS;
	FoundPCIBusCount = 0;
	CurPCIBus = 0xFF;
	do
	{
		// Clear key structure to find first device
		memset(&DeviceKey, PCI_FIELD_IGNORE, sizeof(PLX_DEVICE_KEY));
		
		// Specify Vendor ID (PLX Vendor ID is 0x10b5)
		DeviceKey.VendorId = 0x10b5;
		// Specify Device ID (PLX Device ID is 0x9054)
		DeviceKey.DeviceId = 0x9054;
				
		rc = PlxPci_DeviceFind(&DeviceKey, TotalPlx9054Devices);
		if(rc == ApiSuccess)
		{
			// Record PLX device information
			PlxBusNum[TotalPlx9054Devices] = DeviceKey.bus;
			PlxDeviceNum[TotalPlx9054Devices] = DeviceKey.slot;
			sprintf(ErrMSG, "Device ID = 0x%04x, Vendor ID = 0x%04x, Bus Number = 0x%02x, Device number = 0x%02x", DeviceKey.DeviceId, DeviceKey.VendorId, DeviceKey.bus, DeviceKey.slot);
			Pixie_Print_MSG(ErrMSG);

			// Update minPLXBusNum
			if(minPLXBusNum > DeviceKey.bus)
			{
				minPLXBusNum = DeviceKey.bus;
			}

			// Check if there is a new PCI bus
			if(CurPCIBus != PlxBusNum[TotalPlx9054Devices])
			{
				CurPCIBus = PlxBusNum[TotalPlx9054Devices];
				FoundPCIBusCount ++;
			}

			// Increment found Plx9054 devices by 1
			TotalPlx9054Devices ++;
		}
		else
		{
			break;
		}
	}while(1);

	if(TotalPlx9054Devices == 0)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie_InitSystem): Can't find any PLX devices, PlxPci_DeviceFind rc=%d", rc);
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}

	if(TotalPlx9054Devices < SYS_Number_Modules)
	{
		sprintf(ErrMSG, "*ERROR* (Pixie_InitSystem): Can't find all Pixie-16 modules that were specified (#found = %d, #specified = %d)", TotalPlx9054Devices, SYS_Number_Modules);
		Pixie_Print_MSG(ErrMSG);
		return(-4);
	}

	// Map user specified modules to modules found by PlxPci_DeviceFind
	// They are not necessary in the same order

	// First compute BusNoOffset
	if(MappedPCIBusCount == FoundPCIBusCount)
	{
		BusNoOffset = minPLXBusNum - minPCIBusNum;
	}
	// This is the case where MappedPCIBusCount < FoundPCIBusCount.
	// This could happen if a user does not want to boot all the
	// modules installed in the system.
	else
	{
		if(minSlotNum < 8)
		{
			// Mapped module with minimum PCI bus number is in the first PCI bus segment of a 14-slot crate
			BusNoOffset = minPLXBusNum - minPCIBusNum;
		}
		else
		{
			// Mapped module with minimum PCI bus number is in the second PCI bus segment of a 14-slot crate
			BusNoOffset = minPLXBusNum - (minPCIBusNum - 1);
		}
	}
	
	// Now map modules
	for(k=0; k<SYS_Number_Modules; k++)
	{ 
		// Update PCI bus number for each Pixie-16 module
		Bus_Number[k] += BusNoOffset;

		i = 0;
		PlxModIndex[k] = SYS_MAX_NUM_MODULES;
		do
		{
			if((Bus_Number[k] == PlxBusNum[i]) && (Device_Number[k] == PlxDeviceNum[i]))
			{
				PlxModIndex[k] = (unsigned char)i;
				break;
			}
			i ++;
		}while(i < TotalPlx9054Devices);
		if(PlxModIndex[k] == SYS_MAX_NUM_MODULES)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_InitSystem): Can't match module # %d with one found by the PLX driver", k);
			Pixie_Print_MSG(ErrMSG);
			return(-5);
		}
	}

#if(PRINT_DEBUG_MSG == 1)
	for(k=0;k<SYS_Number_Modules;k++)
	{
		sprintf(ErrMSG, "PXI module number %d =>  PCI bus number %d, PCI device number %d", k , Bus_Number[k], Device_Number[k]);
		Pixie_Print_MSG(ErrMSG);
	}
#endif

	// Open PCI devices for all modules
	for(k=0; k<SYS_Number_Modules; k++)
	{
		// Clear key structure
		memset(&DeviceKey, PCI_FIELD_IGNORE, sizeof(PLX_DEVICE_KEY));

		// Specify Vendor ID (PLX Vendor ID is 0x10b5)
		DeviceKey.VendorId = 0x10b5;
		// Specify Device ID (PLX Device ID is 0x9054)
		DeviceKey.DeviceId = 0x9054;

		// Use the previously mapped PlxModIndex as the Device Number in the search
		rc = PlxPci_DeviceFind(&DeviceKey, PlxModIndex[k]);
		if(rc == ApiSuccess)
		{
			rc = PlxPci_DeviceOpen(&DeviceKey, &SYS_hDevice[k]);
			if(rc != ApiSuccess) // Print error if failure
			{
				sprintf(ErrMSG, "*ERROR* (Pixie_InitSystem): Could not open PCI Device Number (%d) at Bus Number %d; rc=%d", Device_Number[k], Bus_Number[k], rc);
				Pixie_Print_MSG(ErrMSG);
			
				// Before return, we need to close those PCI devices that are already opened
				for(m=0; m<k; m++)
				{
					retval = Pixie_ClosePCIDevices(m);
					if(retval < 0)
					{
						sprintf(ErrMSG, "*ERROR* (Pixie_InitSystem): Could not unmap PCI BAR for Module=%d; rc=%d", m, rc);
						Pixie_Print_MSG(ErrMSG);
					}
				}
			
				return(-6);
			}
			else
			{
#if(PRINT_DEBUG_MSG == 1)
				sprintf(ErrMSG, "(Pixie_InitSystem): Successfully opened Device Number (%d) at Bus Number %d for Module # %d", Device_Number[k], Bus_Number[k], k);
				Pixie_Print_MSG(ErrMSG);
				sprintf(ErrMSG, "Device ID = 0x%04x, Vendor ID = 0x%04x, Bus Number = 0x%02x, Slot number = 0x%02x", DeviceKey.DeviceId, DeviceKey.VendorId, DeviceKey.bus, DeviceKey.slot);
				Pixie_Print_MSG(ErrMSG);
#endif
			
				// Map a PCI BAR into user virtual space and return the virtual address
				// for the opened PCI device. For PLX 9054, Space 0 is at PCI BAR 2.
				rc = PlxPci_PciBarMap(&SYS_hDevice[k], 2, (VOID**)&VAddr[k]);
				if(rc != ApiSuccess)
				{
					sprintf(ErrMSG, "*ERROR* (Pixie_InitSystem): Unable to map a PCI BAR and obtain a virtual address for Module=%d; rc=%d", k, rc);
					Pixie_Print_MSG(ErrMSG);
					
					// Before return, we need to close those PCI devices that are already opened
					for(m=0; m<k; m++)
					{
						retval = Pixie_ClosePCIDevices(m);
						if(retval < 0)
						{
							sprintf(ErrMSG, "*ERROR* (Pixie_InitSystem): Could not unmap PCI BAR for Module=%d; rc=%d", m, rc);
							Pixie_Print_MSG(ErrMSG);
						}
					}
					
					return(-7);
				}
				else
				{
#if(PRINT_DEBUG_MSG == 1)
					sprintf(ErrMSG, "VAddr[%d][%d]=0x%lx", Bus_Number[k], k, VAddr[k]);
					Pixie_Print_MSG(ErrMSG);
#endif
				}

				// Read module serial number which is stored in the beginning of the I2C serial EEPROM.
				// First three words of EEPROM stores serial number and revision number
				retval = I2CM24C64_Sequential_Read(k, 0, 3, sbuffer);
				if(retval < 0)
				{
					sprintf(ErrMSG, "*ERROR* (Pixie_InitSystem): Could not read serial number for Module=%d; retval=%d", k, retval);
					Pixie_Print_MSG(ErrMSG);
				}
				// Starting with serial number 256, serial number is stored in the first two bytes of EEPROM, follwed by
				// revision number, which is at least 11 (i.e. Rev-B)
				if (sbuffer[2] >= 11)
				{
					ModSerNum = (unsigned short)(unsigned char)sbuffer[0] + 256 * (unsigned short)(unsigned char)sbuffer[1];
				}
				else
				{
					ModSerNum = (unsigned short)(unsigned char)sbuffer[0];
				}
#if(PRINT_DEBUG_MSG == 1)
				sprintf(ErrMSG, "(Pixie_InitSystem): Module # %d SERIAL NUMBER = %d", k, ModSerNum);
				Pixie_Print_MSG(ErrMSG);
#endif
			}
		}
		else
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_InitSystem): PlxPci_DeviceFind failed for PCI Device Number (%d) at Bus Number %d; rc=%d", Device_Number[k], Bus_Number[k], rc);
			Pixie_Print_MSG(ErrMSG);
			
			// Before return, we need to close those PCI devices that are already opened
			for(m=0; m<k; m++)
			{
				retval = Pixie_ClosePCIDevices(m);
				if(retval < 0)
				{
					sprintf(ErrMSG, "*ERROR* (Pixie_InitSystem): Could not unmap PCI BAR for Module=%d; rc=%d", m, rc);
					Pixie_Print_MSG(ErrMSG);
				}
			}
			
			return(-8);
		}
	}
	
	return(0);
}


/****************************************************************
*	Pixie_ClosePCIDevices:
*		Unmap PCI BAR and close PLX PCI devices.
*
*		Return Value:
*			 0 - successful
*			-1 - Unable to unmap the PCI BAR
*			-2 - Unable to close the PCI device
*
****************************************************************/

int Pixie_ClosePCIDevices(unsigned short ModNum)
{
	PLX_STATUS      rc;
	unsigned short  k;
	char            ErrMSG[MAX_ERRMSG_LENGTH];
	
	if(SYS_Offline == 1)  // Returns immediately for offline analysis
	{
		return(0);
	}
	
	if(ModNum == SYS_Number_Modules)	// Close PCI devices in all modules
	{
		for(k=0; k<SYS_Number_Modules; k+=1)
		{
			// Unmaps a previously mapped PCI BAR from user virtual space
			rc = PlxPci_PciBarUnmap(&SYS_hDevice[k], (VOID**)&VAddr[k]);
			if(rc != ApiSuccess)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie_ClosePCIDevices): Unable to unmap the PCI BAR for module %d; rc=%d", k, rc);
				Pixie_Print_MSG(ErrMSG);
				return(-1);
			}
			
			// Release the PLX device
			rc = PlxPci_DeviceClose(&SYS_hDevice[k]);
			if (rc != ApiSuccess)
			{
				sprintf(ErrMSG, "*ERROR* (Pixie_ClosePCIDevices): Unable to close the PLX device for module %d; rc=%d", k, rc);
				Pixie_Print_MSG(ErrMSG);
				return(-2);
			}
		}
	}
	else if(ModNum < SYS_Number_Modules)	// Close PCI devices in only one module
	{
		// Unmaps a previously mapped PCI BAR from user virtual space
		rc = PlxPci_PciBarUnmap(&SYS_hDevice[ModNum], (VOID**)&VAddr[ModNum]);
		if(rc != ApiSuccess)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_ClosePCIDevices): Unable to unmap the PCI BAR for module %d; rc=%d", ModNum, rc);
			Pixie_Print_MSG(ErrMSG);
			return(-1);
		}
		
		// Release the PLX device
		rc = PlxPci_DeviceClose(&SYS_hDevice[ModNum]);
		if (rc != ApiSuccess)
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_ClosePCIDevices): Unable to close the PLX device for module %d; rc=%d", ModNum, rc);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
	}
	
	return(0);
}


/****************************************************************
*	Pixie_Boot_CompFPGA:
*		Download system chip FPGA configuration.
*
*		Return Value:
*			 0 - boot successful
*			-2 - Clearing system FPGA MM timed out
*			-3 - Downloading system FPGA timed out
*
****************************************************************/

int Pixie_Boot_CompFPGA(unsigned short ModNum, unsigned int *Com_FPGA_conf, unsigned int NumComFPGAconf)
{
	unsigned int buffer[8];
	unsigned int k, counter0, counter1;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	if(SYS_Offline == 1)  // Returns immediately for offline analysis
	{
		return(0);
	}
	
	// Initialize counter0 to 0
	counter0 = 0;
	
	// Set communication FPGA Program*=0 to clear it
StartSys:
	Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
	buffer[0] &= 0xFFFFF000;
	buffer[0] |= 0x00000551; 
	Pixie_Register_IO(ModNum, CFG_CTRLCS, SYS_MOD_WRITE, buffer);
	
	// Set communication FPGA Program*=1 to start configuration
	Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
	buffer[0] &= 0xFFFFF000;
	buffer[0] |= 0x00000553; 
	Pixie_Register_IO(ModNum, CFG_CTRLCS, SYS_MOD_WRITE, buffer);
	
	// A very short wait - 200 us
	wait_for_a_short_time((int)(200000.0/Ns_Per_Cycle));
	
	// Initialize counter1 to 0
	counter1 = 0;
	
ReadINIT:
	Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
	if ((buffer[0] & 0x01) != 0x01) // if yes, finished clearing communication FPGA
	{
		counter1 ++;
		if(counter1 < 100)
		{
			// A very short wait - 200 us
			wait_for_a_short_time((int)(200000.0/Ns_Per_Cycle));
			goto ReadINIT;
		}
		else
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Boot_ComFPGA): Clearing communication FPGA in module %d timed out.", ModNum);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
	}
	else 
	{
		for (k=0; k<NumComFPGAconf; k++)
		{
			// Downloading communication FPGA configuration
			Pixie_Register_IO(ModNum, CFG_DATACS, SYS_MOD_WRITE, &Com_FPGA_conf[k]);
			
			// A very short wait - 2 us
			wait_for_a_short_time((int)(2000.0/Ns_Per_Cycle));
		}
	}
	
	Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
	if ((buffer[0] & 0x03) != 0x03) // if yes, download communication FPGA successful
	{
		counter0 ++;
		if(counter0 < 10)
		{
			goto StartSys;
		}
		else
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Boot_ComFPGA): Downloading communication FPGA to module %d timed out", ModNum);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}
	}
	else
	{
		sprintf(ErrMSG, "Downloaded communication FPGA successfully in module %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
	}
	
	return(0);
}


/****************************************************************
*	Pixie_Boot_FIPPI function:
*		Download RTPU FPGA (FIPPI).
*
*		Return Value:
*			0  - boot successful
*			-2 - Clearing FIPPI configuration timed out
*			-3 - Downloading FIP0&1 FPGA timed out
*
****************************************************************/

int Pixie_Boot_FIPPI(unsigned short ModNum, unsigned int *SP_FPGA_conf, unsigned int NumSPFPGAconf)
{
	unsigned int buffer[4];
	unsigned int k, counter0, counter1;
	char ErrMSG[MAX_ERRMSG_LENGTH];
	
	if(SYS_Offline == 1)  // Returns immediately for offline analysis
	{
		return(0);
	}
	
	// Initialize counter0 to 0
	counter0 = 0;
	
	// Set all SP FPGA Chip Program*=0 to clear configuration
StartFip12:
	Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
	buffer[0] &= 0xFFFFF000;
	buffer[0] |= 0x00000052; // Only booting FiPPI for 8 channels
	
	Pixie_Register_IO(ModNum, CFG_CTRLCS, SYS_MOD_WRITE, buffer);
	
	// Set all SP FPGA Chip Program*=1 to start configuration
	Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
	buffer[0] &= 0xFFFFF000;
	buffer[0] |= 0x000000F2;
	
	Pixie_Register_IO(ModNum, CFG_CTRLCS, SYS_MOD_WRITE, buffer);
	
	// A very short wait - 200 us
	wait_for_a_short_time((int)(200000.0/Ns_Per_Cycle));
	
	// Initialize counter to 0
	counter1 = 0;
	
ReadINITf12:
	Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
	if ((buffer[0] & 0x53) != 0x53) // if yes, finished clearing FIPPI configuration
	{
		counter1 ++;
		if(counter1 < 100)
		{
			sprintf(ErrMSG, "(Pixie_Boot_FIPPI): Clearing SP FPGAs 1&2, CFG_RDCS=0x%x", buffer[0]);
			Pixie_Print_MSG(ErrMSG);

			// A very short wait - 200 us
			wait_for_a_short_time((int)(200000.0/Ns_Per_Cycle));
			goto ReadINITf12;
		}
		else
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Boot_FIPPI): Clearing FIPPI configuration 1&2 timed out in module %d", ModNum);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
	}
	else 
	{
		for (k=0; k<NumSPFPGAconf; k++)
		{
			Pixie_Register_IO(ModNum, CFG_DATACS, SYS_MOD_WRITE, &SP_FPGA_conf[k]);
			
			// A very short wait - 2 us
			wait_for_a_short_time((int)(2000.0/Ns_Per_Cycle));
		}
	}
	
	Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
	if ((buffer[0] & 0xA3) != 0xA3) // if yes, download all SP FPGA successful
	{
		counter0 ++;
		if(counter0 < 10)
		{
			sprintf(ErrMSG, "(Pixie_Boot_FIPPI): Downloading SP FPGAs 1&2, CFG_RDCS=0x%x", buffer[0]);
			Pixie_Print_MSG(ErrMSG);
			goto StartFip12;
		}
		else
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Boot_FIPPI): Downloading SP FPGAs 1&2 timed out in module %d", ModNum);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}
	}
	else
	{
		sprintf(ErrMSG, "Downloaded SP FPGAs 1&2 successfully in module %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
	}
	
	// Initialize counter0 to 0
	counter0 = 0;
	
	// Set all SP FPGA Chip Program*=0 to clear configuration
StartFip34:
	Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
	buffer[0] &= 0xFFFFF000;
	buffer[0] |= 0x000005A2; // Temporarily only booting FiPPI for 8 channels
	
	// Write to an offset from the PCI-to-Local space window
	Pixie_Register_IO(ModNum, CFG_CTRLCS, SYS_MOD_WRITE, buffer);
	
	// Set all SP FPGA Chip Program*=1 to start configuration
	buffer[0] &= 0xFFFFF000;
	buffer[0] |= 0x00000FA2;
	
	Pixie_Register_IO(ModNum, CFG_CTRLCS, SYS_MOD_WRITE, buffer);
	
	// A very short wait - 200 us
	wait_for_a_short_time((int)(200000.0/Ns_Per_Cycle));
	
	// Initialize counter to 0
	counter1 = 0;
	
ReadINITf34:
	Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
	if ((buffer[0] & 0x5A3) != 0x5A3) // if yes, finished clearing FIPPI configuration
	{
		counter1 ++;
		if(counter1 < 100)
		{
			sprintf(ErrMSG, "(Pixie_Boot_FIPPI): Clearing SP FPGAs 3&4, CFG_RDCS=0x%x", buffer[0]);
			Pixie_Print_MSG(ErrMSG);

			// A very short wait - 200 us
			wait_for_a_short_time((int)(200000.0/Ns_Per_Cycle));
			goto ReadINITf34;
		}
		else
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Boot_FIPPI): Clearing FIPPI configuration 3&4 timed out in module %d", ModNum);
			Pixie_Print_MSG(ErrMSG);
			return(-2);
		}
	}
	else 
	{
		for (k=0; k<NumSPFPGAconf; k++)
		{
			Pixie_Register_IO(ModNum, CFG_DATACS, SYS_MOD_WRITE, &SP_FPGA_conf[k]);
			
			// A very short wait - 2 us
			wait_for_a_short_time((int)(2000.0/Ns_Per_Cycle));
		}
	}
	
	Pixie_Register_IO(ModNum, CFG_RDCS, SYS_MOD_READ, buffer);
	if ((buffer[0] & 0xAA3) != 0xAA3)  // if yes, download all SP FPGA successful
	{
		counter0 ++;
		if(counter0 < 10)
		{
			sprintf(ErrMSG, "(Pixie_Boot_FIPPI): Downloading SP FPGAs 3&4, CFG_RDCS=0x%x", buffer[0]);
			Pixie_Print_MSG(ErrMSG);
			goto StartFip34;
		}
		else
		{
			sprintf(ErrMSG, "*ERROR* (Pixie_Boot_FIPPI): Downloading SP FPGAs 3&4 timed out in module %d", ModNum);
			Pixie_Print_MSG(ErrMSG);
			return(-3);
		}
	}
	else
	{
		sprintf(ErrMSG, "Downloaded SP FPGAs 3&4 successfully in module %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
	}
	
#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
	Sleep(10);
#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI
	usleep(10000);
#endif

	Pixie_Register_IO(ModNum, CFG_DCMRST, SYS_MOD_WRITE, buffer);	// reset DCM
	
	return(0);
}


/****************************************************************
*	Pixie_Boot_DSP:
*		Download DSP code.
*
*		Return Value:
*			 0 - boot DSP successful
*			-1 - Can not set timer resolution to 1 ms
*			-2 - Set DSP SYSCON in module failed
*			-3 - Set DSP DMAC10 in module failed
*			-4 - tagnum is invalid
*			-5 - DSP can't finish initialization on reset
*
****************************************************************/

int Pixie_Boot_DSP(unsigned short ModNum, unsigned int *DSP_code, unsigned int NumDSPCodeWords)
{
	unsigned int buffer[0x5];      // Temporary buffer
	unsigned int i, count;         // Index and counters
	char ErrMSG[MAX_ERRMSG_LENGTH]; // Output message
	unsigned int tagnum, trytimes, wordcount, CSR;
#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
	DWORD start;
#endif
	unsigned short bootsuccess, looptimes;
	
	if(SYS_Offline == 1)  // Returns immediately for offline analysis
	{
		return(0);
	}
	
#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
	// Set timer resolution to 1 ms
	if (timeBeginPeriod (1) != TIMERR_NOERROR)
	{
		sprintf(ErrMSG, "Can not set timer resolution to 1 ms");
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}
#endif

	//********************************************
	// Try a limited number of times until the DSP
	// is booted successfully even though under normal
	// circumstances only one try will be sufficient
	//********************************************
	looptimes = 0;
	bootsuccess = 0;
	do
	{
		//********************************************
		// DSP codes download, with ready enabled,
		// no burst, 32 to 48 bits packing
		//********************************************
		
		// Reset DSP
		Pixie_ReadCSR(ModNum, &CSR);
		CSR = SYS32_SetBit(DSPDOWNLOAD, CSR);
		CSR = SYS32_SetBit(DSPRESET, CSR);
		Pixie_WrtCSR(ModNum, CSR);
		
		// Wait for 1 ms
	#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
		start = timeGetTime ();
		while (timeGetTime () < (start + 1));
	#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI
		usleep(1000);
	#endif
		
		Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer); // HBR done for reset
		
		// Wait for 1 ms
	#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
		start = timeGetTime ();
		while (timeGetTime () < (start + 1));
	#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI
		usleep(1000);
	#endif
		
		buffer[0]=SYSCON;
		Pixie_Register_IO(ModNum, EXT_MEM_TEST, SYS_MOD_WRITE, buffer);	// SYSCON address
		Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);	// HBR request
		
		// A short wait - 1 ms
		wait_for_a_short_time((int)(1000000.0/Ns_Per_Cycle));
		
		trytimes = 0;
		
	RetryS: 
		
		Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_READ, buffer);	// read data from DSP SYSCON
		if(buffer[0] != 0x10)
		{
			// A short wait - 1 ms
			wait_for_a_short_time((int)(1000000.0/Ns_Per_Cycle));
			
			trytimes ++;
			if(trytimes > 5)
			{
				sprintf(ErrMSG, "Set DSP SYSCON in module #%d failed, SYSCON=0x%x", ModNum, buffer[0]);
				Pixie_Print_MSG(ErrMSG);
				return(-2);
			}
			
			goto RetryS;
		}
		else
		{
			trytimes = 0;
			
	RetryD:
				
			buffer[0]=DMAC10;
			Pixie_Register_IO(ModNum, EXT_MEM_TEST, SYS_MOD_WRITE, buffer);
			Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_READ, buffer);	// read data from DSP DMAC10
			if(buffer[0] != 0xA1)
			{
				trytimes ++;
				if(trytimes > 5)
				{
					sprintf(ErrMSG, "Set DSP DMAC10 in module #%d failed, DMAC10=0x%x", ModNum, buffer[0]);
					Pixie_Print_MSG(ErrMSG);
					return(-3);
				}
				
				goto RetryD;
			}
			else
			{
				// count will keep track the number of DSP_code words that have been downloaded
				count = 0;
				
				buffer[0]=EPB0;
				Pixie_Register_IO(ModNum, EXT_MEM_TEST, SYS_MOD_WRITE, buffer);	//  DSP EPB0
				
				// Download the boot kernel
				for(i=0; i<256; i++)
				{
					Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[i*3]);		// write to EPB0		
					Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[i*3+1]);	// write to EPB0
					Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[i*3+2]);	// write to EPB0
					
					count += 3;
				}
				
				Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);	// De-assert HBR 
				do
				{
					tagnum = DSP_code[count];
					
					switch(tagnum)
					{
						
					case ZERO_DM16:
					case ZERO_PM16:
					case ZERO_DM32:
					case ZERO_PM32:
					case ZERO_DM40:
					case ZERO_PM40:
					case ZERO_PM48:
					case ZERO_DM64:
					case ZERO_PM64:
						
						Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);	// HBR request
						
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count]);	// write to EPB0
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+1]);	// write to EPB0
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+2]);	// write to EPB0
						
						Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);	// HBR done
						
						count += 3;
						// wordcount shows the number of DSP_code word for this section
						wordcount = DSP_code[count];
						
						Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);	// HBR request
						
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count]);	// write to EPB0
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+1]);	// write to EPB0
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+2]);	// write to EPB0
						
						Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);	// HBR done
						
						count += 3;
						
						// Wait for 2 ms
	#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
						start = timeGetTime ();
						while (timeGetTime () < (start + 2));
	#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI
						usleep(2000);
	#endif
						
						break;
						
					case INIT_DM16:
					case INIT_PM16:
					case INIT_DM32:
					case INIT_PM32:
					case INIT_PM48:
						
						Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);	// HBR request 
						
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count]);	// write to EPB0
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+1]);	// write to EPB0
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+2]);	// write to EPB0
						
						Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);	// HBR done
						
						count += 3;
						// wordcount shows the number of DSP_code word for this section
						wordcount = DSP_code[count];
						
						Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);	// HBR request 
						
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count]);	// write to EPB0
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+1]);	// write to EPB0
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+2]);	// write to EPB0
						
						Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);	// HBR done
						
						count += 3;
						
						for(i=0; i<wordcount; i++)
						{
							
							Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);	// HBR request 
							
							Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+i*3]);	// write to EPB0
							Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+i*3+1]);	// write to EPB0
							Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+i*3+2]);	// write to EPB0
							
							Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);	// HBR done
						}
						
						count += 3 * wordcount;
						
						break;
						
					case INIT_DM64:
					case INIT_PM64:
						
						Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);	// HBR request 
						
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count]);	// write to EPB0
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+1]);	// write to EPB0
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+2]);	// write to EPB0
						
						Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);	// HBR done
						
						count += 3;
						// wordcount shows the number of DSP_code word for this section
						wordcount = DSP_code[count];
						
						Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);	// HBR request 
						
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count]);	// write to EPB0
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+1]);	// write to EPB0
						Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+2]);	// write to EPB0
						
						Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);	// HBR done
						
						count += 3;
						
						for(i=0; i<wordcount; i++)
						{
							
							Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);	// HBR request 
							
							Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+i*6]);	// write to EPB0
							Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+i*6+1]);	// write to EPB0
							Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+i*6+2]);	// write to EPB0
							
							Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);	// HBR done
							
							Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);	// HBR request 
							
							Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+i*6+3]);	// write to EPB0
							Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+i*6+4]);	// write to EPB0
							Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+i*6+5]);	// write to EPB0
							
							Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);	// HBR done
						}
						
						count += 6 * wordcount;
						
						break;
						
						
					case FINAL_INIT:
						
						Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);	// HBR request 
						
						for(i=0; i<258; i++)
						{
							Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+i*3]);	// write to EPB0
							Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+i*3+1]);	// write to EPB0
							Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &DSP_code[count+i*3+2]);	// write to EPB0
						}
						
						count += 258*3;
						
						Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);	// HBR done
						
						break;
						
					default:
						sprintf(ErrMSG, "tagnum=%u, check if it is valid", tagnum);
						Pixie_Print_MSG(ErrMSG);
						return(-4);
					}
					
				}while(count < NumDSPCodeWords);
			}
		}

		//********************************************
		// Wait until DSP variable PowerUpInitDone is
		// set to 1 by the DSP after it completes its
		// initialization tasks on hard RESET. This
		// should prevent premature downloading of DSP
		// parameters by the host, which would cause 
		// corruption of DSP internal memory.
		//********************************************
		trytimes = 0;
		do
		{
			Pixie_DSP_Memory_IO(buffer, POWERUPINITDONE_ADDRESS, 1, SYS_MOD_READ, ModNum);
			if(buffer[0] == 1)
			{
				bootsuccess = 1;
				break;
			}
	#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
			Sleep(1);
	#elif PIXIE16_SYSAPI_VER == PIXIE16_LINUX_SYSAPI
			usleep(1000);
	#endif
			trytimes ++;
		}while(trytimes < 10);
		if(bootsuccess == 0)
		{
			sprintf(ErrMSG, "PowerUpInitDone stuck at 0 in the DSP of module %d - can't finish reset initialization", ModNum);
			Pixie_Print_MSG(ErrMSG);
		}

		looptimes ++;
	}while((bootsuccess == 0) && (looptimes < 10));
	
#if PIXIE16_SYSAPI_VER == PIXIE16_WINDOWS_SYSAPI
	timeEndPeriod (1);
#endif
	
	if(bootsuccess == 1)
	{
		if(looptimes > 1)
		{
			sprintf(ErrMSG, "After %d tries, downloaded DSP code in module #%d successfully", looptimes, ModNum);
			Pixie_Print_MSG(ErrMSG);
		}
		else
		{
			sprintf(ErrMSG, "Downloaded DSP code in module #%d successfully", ModNum);
			Pixie_Print_MSG(ErrMSG);
		}
		return(0);  
	}
	else
	{
		sprintf(ErrMSG, "Failed to download DSP code in module #%d after %d tries", ModNum, looptimes);
		Pixie_Print_MSG(ErrMSG);
		return(-5);  
	}

}
