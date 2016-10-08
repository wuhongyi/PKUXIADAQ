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
*		i2cm24c64.c
*
* Description:
*
*		This file contains functions to access the 64Kbit serial I2C bus EEPROM
*		on Pixie-16.
*
* $Rev: 15626 $
* $Id: i2cm24c64.c 15626 2010-05-07 18:51:20Z htan $
******************************************************************************/

#include <stdio.h>
#include <math.h>

#include "PlxApi.h"

#include "pixie16sys_common.h"
#include "pixie16sys_defs.h"
#include "pixie16sys_globals.h"
#include "pixie16sys_export.h"


/****************************************************************
 *	I2CM24C64_Write_One_Byte:
 *		Bus master writes one byte to I2C serial EEPROM M24C64.
 *
 *		Return Value:
 *			 0 - Successful
 *			-1 - Invalid Pixie module number
 *			-2 - Failed to get Acknowledge after sending DevSel byte
 *			-3 - Failed to get Acknowledge after sending address byte (MSB)
 *			-4 - Failed to get Acknowledge after sending address byte (LSB)
 *			-5 - Failed to get Acknowledge after sending byte value
 *
 ****************************************************************/

int I2CM24C64_Write_One_Byte (
	unsigned short ModNum,		// Pixie module module
	unsigned short Address,		// The address to write this byte
	char *ByteValue )			// The byte value
{
	char ErrMSG[256], IOByte;
	char ackvalue, byteaddrMSB, byteaddrLSB;

	// Check if ModNum is valid
	if(ModNum >= SYS_Number_Modules)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Write_One_Byte): Invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}

	// Extract I2CM24C64 byte address
	byteaddrMSB = (char)((float)Address / 256.0);
	byteaddrLSB = (char)(fmod((double)Address, 256.0));

	// Send "START"
	I2CM24C64_start(ModNum);

	// Send Device Select Code
	IOByte = (char)0xA0;
	I2CM24C64_byte_write(ModNum, IOByte);
	
	// Get Acknowledge
	ackvalue = I2CM24C64_getACK(ModNum);
	if(ackvalue != 0)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Write_One_Byte): Failed to get Acknowledge after sending DevSel byte");
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}

	// Send address
	IOByte = byteaddrMSB;
	I2CM24C64_byte_write(ModNum, IOByte);
	// Get Acknowledge
	ackvalue = I2CM24C64_getACK(ModNum);
	if(ackvalue != 0)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Write_One_Byte): Failed to get Acknowledge after sending address byte (MSB)");
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}

	IOByte = byteaddrLSB;
	I2CM24C64_byte_write(ModNum, IOByte);
	// Get Acknowledge
	ackvalue = I2CM24C64_getACK(ModNum);
	if(ackvalue != 0)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Write_One_Byte): Failed to get Acknowledge after sending address byte (LSB)");
		Pixie_Print_MSG(ErrMSG);
		return(-4);
	}

	// Send byte value
	IOByte = *ByteValue;
	I2CM24C64_byte_write(ModNum, IOByte);

	// Get Acknowledge
	ackvalue = I2CM24C64_getACK(ModNum);
	if(ackvalue != 0)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Write_One_Byte): Failed to get Acknowledge after sending byte value");
		Pixie_Print_MSG(ErrMSG);
		return(-5);
	}

	// Send "STOP"
	I2CM24C64_stop(ModNum);

	return(0);
}


/****************************************************************
 *	I2CM24C64_Read_One_Byte:
 *		Bus master reads one byte from I2C serial EEPROM M24C64.
 *
 *		Return Value:
 *			 0 - Successful
 *			-1 - Invalid Pixie module number
 *			-2 - Failed to get Acknowledge after sending DevSel byte
 *			-3 - Failed to get Acknowledge after sending address byte (MSB)
 *			-4 - Failed to get Acknowledge after sending address byte (LSB)
 *			-5 - Failed to get Acknowledge after sending DevSel byte
 *
 ****************************************************************/

int I2CM24C64_Read_One_Byte (
	unsigned short ModNum,		// Pixie module number
	unsigned short Address,		// The address to read this byte
	char *ByteValue )			// The byte value
{
	char ErrMSG[256], IOByte;
	char ackvalue, byteaddrMSB, byteaddrLSB;

	// Check if ModNum is valid
	if(ModNum >= SYS_Number_Modules)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Read_One_Byte): Invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}

	// Extract I2CM24C64 byte address
	byteaddrMSB = (char)((float)Address / 256.0);
	byteaddrLSB = (char)(fmod((double)Address, 256.0));

	// Send "START"
	I2CM24C64_start(ModNum);

	// Send Device Select Code
	IOByte = (char)0xA0;
	I2CM24C64_byte_write(ModNum, IOByte);
	
	// Get Acknowledge
	ackvalue = I2CM24C64_getACK(ModNum);
	if(ackvalue != 0)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Read_One_Byte): Failed to get Acknowledge after sending DevSel byte");
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}

	// Send address
	IOByte = byteaddrMSB;
	I2CM24C64_byte_write(ModNum, IOByte);
	// Get Acknowledge
	ackvalue = I2CM24C64_getACK(ModNum);
	if(ackvalue != 0)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Read_One_Byte): Failed to get Acknowledge after sending address byte (MSB)");
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}

	IOByte = byteaddrLSB;
	I2CM24C64_byte_write(ModNum, IOByte);
	// Get Acknowledge
	ackvalue = I2CM24C64_getACK(ModNum);
	if(ackvalue != 0)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Read_One_Byte): Failed to get Acknowledge after sending address byte (LSB)");
		Pixie_Print_MSG(ErrMSG);
		return(-4);
	}

	// Send "START"
	I2CM24C64_start(ModNum);

	// Send Device Select Code
	IOByte = (char)0xA1;
	I2CM24C64_byte_write(ModNum, IOByte);
	
	// Get Acknowledge
	ackvalue = I2CM24C64_getACK(ModNum);
	if(ackvalue != 0)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Read_One_Byte): Failed to get Acknowledge after sending DevSel byte");
		Pixie_Print_MSG(ErrMSG);
		return(-5);
	}

	// Receive one byte
	I2CM24C64_byte_read(ModNum, ByteValue);
	
	// Send "STOP"
	I2CM24C64_stop(ModNum);

	return(0);
}


/****************************************************************
 *	I2CM24C64_Page_Write:
 *		Bus master writes one page to I2C serial EEPROM M24C64.
 *
 *		Return Value:
 *			 0 - Successful
 *			-1 - Invalid Pixie module number
 *			-2 - Failed to get Acknowledge after sending DevSel byte
 *			-3 - Failed to get Acknowledge after sending address byte (MSB)
 *			-4 - Failed to get Acknowledge after sending address byte (LSB)
 *			-5 - Failed to get Acknowledge after writing a byte
 *
 ****************************************************************/

int I2CM24C64_Page_Write (
	unsigned short ModNum,			// Pixie module module
	unsigned short Address,			// The starting address to write this page
	unsigned short NumBytesToWrite,	// The number of bytes for this page to write
	char *ByteValue )				// The byte values
{
	char ErrMSG[256], IOByte;
	char ackvalue, byteaddrMSB, byteaddrLSB;
	unsigned short k;

	// Check if ModNum is valid
	if(ModNum >= SYS_Number_Modules)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Page_Write): Invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}

	// Extract I2CM24C64 byte address
	byteaddrMSB = (char)((float)Address / 256.0);
	byteaddrLSB = (char)(fmod((double)Address, 256.0));

	// Send "START"
	I2CM24C64_start(ModNum);

	// Send Device Select Code
	IOByte = (char)0xA0;
	I2CM24C64_byte_write(ModNum, IOByte);
	
	// Get Acknowledge
	ackvalue = I2CM24C64_getACK(ModNum);
	if(ackvalue != 0)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Page_Write): Failed to get Acknowledge after sending DevSel byte");
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}

	// Send address
	IOByte = byteaddrMSB;
	I2CM24C64_byte_write(ModNum, IOByte);
	// Get Acknowledge
	ackvalue = I2CM24C64_getACK(ModNum);
	if(ackvalue != 0)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Page_Write): Failed to get Acknowledge after sending address byte (MSB)");
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}

	IOByte = byteaddrLSB;
	I2CM24C64_byte_write(ModNum, IOByte);
	// Get Acknowledge
	ackvalue = I2CM24C64_getACK(ModNum);
	if(ackvalue != 0)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Page_Write): Failed to get Acknowledge after sending address byte (LSB)");
		Pixie_Print_MSG(ErrMSG);
		return(-4);
	}

	for(k=0; k<NumBytesToWrite; k++)
	{
		IOByte = *ByteValue;
		I2CM24C64_byte_write(ModNum, IOByte);
		// Get Acknowledge
		ackvalue = I2CM24C64_getACK(ModNum);
		if(ackvalue != 0)
		{
			sprintf(ErrMSG, "*ERROR* (I2CM24C64_Page_Write): Failed to get Acknowledge after writing a byte");
			Pixie_Print_MSG(ErrMSG);
			return(-5);
		}

		// Increment address
		ByteValue ++;
	}

	// Send "STOP"
	I2CM24C64_stop(ModNum);

	return(0);
}


/****************************************************************
 *	I2CM24C64_Sequential_Read:
 *		Bus master reads multiple bytes from I2C serial EEPROM M24C64.
 *
 *		Return Value:
 *			 0 - Successful
 *			-1 - Invalid Pixie module number
 *			-2 - Failed to get Acknowledge after sending DevSel byte
 *			-3 - Failed to get Acknowledge after sending address byte (MSB)
 *			-4 - Failed to get Acknowledge after sending address byte (LSB)
 *			-5 - Failed to get Acknowledge after writing a byte
 *
 ****************************************************************/

int I2CM24C64_Sequential_Read (
	unsigned short ModNum,			// Pixie module number
	unsigned short Address,			// The starting address to read
	unsigned short NumBytesToRead,	// Number of bytes to read (maximum 32 bytes)
	char *ByteValue )				// The byte values
{
	char ErrMSG[256], IOByte;
	char ackvalue, byteaddrMSB, byteaddrLSB;
	unsigned short k;

	// Check if ModNum is valid
	if(ModNum >= SYS_Number_Modules)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Sequential_Read): Invalid Pixie module number %d", ModNum);
		Pixie_Print_MSG(ErrMSG);
		return(-1);
	}

	// Extract I2CM24C64 byte address
	byteaddrMSB = (char)((float)Address / 256.0);
	byteaddrLSB = (char)(fmod((double)Address, 256.0));

	// Send "START"
	I2CM24C64_start(ModNum);

	// Send Device Select Code
	IOByte = (char)0xA0;
	I2CM24C64_byte_write(ModNum, IOByte);
	
	// Get Acknowledge
	ackvalue = I2CM24C64_getACK(ModNum);
	if(ackvalue != 0)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Sequential_Read): Failed to get Acknowledge after sending DevSel byte");
		Pixie_Print_MSG(ErrMSG);
		return(-2);
	}

	// Send address
	IOByte = byteaddrMSB;
	I2CM24C64_byte_write(ModNum, IOByte);
	// Get Acknowledge
	ackvalue = I2CM24C64_getACK(ModNum);
	if(ackvalue != 0)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Sequential_Read): Failed to get Acknowledge after sending address byte (MSB)");
		Pixie_Print_MSG(ErrMSG);
		return(-3);
	}

	IOByte = byteaddrLSB;
	I2CM24C64_byte_write(ModNum, IOByte);
	// Get Acknowledge
	ackvalue = I2CM24C64_getACK(ModNum);
	if(ackvalue != 0)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Sequential_Read): Failed to get Acknowledge after sending address byte (LSB)");
		Pixie_Print_MSG(ErrMSG);
		return(-4);
	}

	// Send "START"
	I2CM24C64_start(ModNum);

	// Send Device Select Code
	IOByte = (char)0xA1;
	I2CM24C64_byte_write(ModNum, IOByte);
	
	// Get Acknowledge
	ackvalue = I2CM24C64_getACK(ModNum);
	if(ackvalue != 0)
	{
		sprintf(ErrMSG, "*ERROR* (I2CM24C64_Sequential_Read): Failed to get Acknowledge after sending DevSel byte");
		Pixie_Print_MSG(ErrMSG);
		return(-5);
	}

	for(k=0; k<NumBytesToRead; k++)
	{
		// Receive one byte
		I2CM24C64_byte_read(ModNum, ByteValue);
	
		if(k != (NumBytesToRead - 1))
		{
			// Send Acknowledge
			I2CM24C64_sendACK(ModNum);

			// Increment address
			ByteValue ++;
		}
	}

	// Send "STOP"
	I2CM24C64_stop(ModNum);

	return(0);
}


/* ----------------------------------------------------- */
// I2CM24C64_start:
//   Bus master sends "START" to M24C64
/* ----------------------------------------------------- */

int I2CM24C64_start(unsigned short ModNum)
{
	unsigned int buffer[4]; 

	//***************************
	//	Set SCL and SDA to 1
	//***************************

	buffer[0] = 0x7;	/* SDA = 1; SCL = 1; CTRL = 1 */
	Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

	/* Wait for 600 ns */
	wait_for_a_short_time((int)(600.0 / (double)Ns_Per_Cycle));

	//***************************
	//	Set SDA to 0 while keep SCL at 1
	//***************************

	buffer[0] = 0x6;	/* SDA = 0; SCL = 1; CTRL = 1 */
	Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);
	
	/* Wait for 600 ns */
	wait_for_a_short_time((int)(600.0 / (double)Ns_Per_Cycle));

	return(0);
}


/* ----------------------------------------------------- */
// I2CM24C64_stop:
//   Bus master sends "STOP" to M24C64
/* ----------------------------------------------------- */

int I2CM24C64_stop(unsigned short ModNum)
{
	unsigned int buffer[4]; 

	//***************************
	//	Set SCL to 1 and SDA to 0
	//***************************

	buffer[0] = 0x6;	/* SDA = 0; SCL = 1; CTRL = 1 */
	Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);
	
	/* Wait for 600 ns */
	wait_for_a_short_time((int)(600.0 / (double)Ns_Per_Cycle));

	//***************************
	//	Set SDA to 1 while keep SCL at 1
	//***************************

	buffer[0] = 0x7;	/* SDA = 1; SCL = 1; CTRL = 1 */
	Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

	/* Wait for 1300 ns */
	wait_for_a_short_time((int)(1300.0 / (double)Ns_Per_Cycle));

	return(0);
}


/* ----------------------------------------------------- */
// I2CM24C64_byte_write:
//   Bus master sends a byte to M24C64
/* ----------------------------------------------------- */

int I2CM24C64_byte_write(unsigned short ModNum, char ByteToSend)
{
	short i;
	unsigned int buffer[4];

	/* Initialize buffer[0] */

	buffer[0] = 0x4;	/* SDA = 0; SCL = 0; CTRL = 1 */

	for(i = 7; i >= 0; i --)
	{
		//***************************
		//	Set SCL to 0
		//***************************

		buffer[0] = (unsigned int)SYS16_ClrBit(1, (unsigned short)buffer[0]);
		Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

		/* Wait for 700 ns */
		wait_for_a_short_time((int)(700.0 / (double)Ns_Per_Cycle));

		//***************************
		//	Send bit i
		//***************************
		
		if(SYS16_TstBit((unsigned short)i, (unsigned short)ByteToSend) == 1)
			buffer[0] = 0x5;	/* SDA = 1; SCL = 0; CTRL = 1 */
		else
			buffer[0] = 0x4;	/* SDA = 0; SCL = 0; CTRL = 1 */

		Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

		/* Wait for 600 ns */
		wait_for_a_short_time((int)(600.0 / (double)Ns_Per_Cycle));

		//***************************
		//	Set SCL to 1
		//***************************
		buffer[0] = SYS16_SetBit(1, (unsigned short)buffer[0]);
		Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

		/* Wait for 600 ns */
		wait_for_a_short_time((int)(600.0 / (double)Ns_Per_Cycle));
	}

	//************************************************************
	//	Set SCL and CTRL to 0 to release bus for acknowledge
	//***********************************************************

	buffer[0] = (buffer[0] & 0x1);
	Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

	/* Wait for 600 ns */
	wait_for_a_short_time((int)(600.0 / (double)Ns_Per_Cycle));

	return(0);
}


/* ----------------------------------------------------- */
// I2CM24C64_byte_read:
//   Bus master receives a byte from M24C64
/* ----------------------------------------------------- */

int I2CM24C64_byte_read(unsigned short ModNum, char *ByteToReceive)
{
	short i;
	unsigned int buffer[4];
	char ByteReceived;

	buffer[0] = 0x0;	/* SDA = 0; SCL = 0; CTRL = 0 */

	for(i = 7; i >= 0; i --)
	{
		//***************************
		//	Set SCL to 1
		//***************************

		buffer[0] = (unsigned int)SYS16_SetBit(1, (unsigned short)buffer[0]);
		Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

        /* Wait for 600 ns */
		wait_for_a_short_time((int)(600.0 / (double)Ns_Per_Cycle));

		//***************************
		//	Receive bit i
		//***************************
        
        buffer[0] = 0x0;

		Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_READ, buffer);
		ByteReceived = (char)buffer[0];

		if(SYS16_TstBit(0, (unsigned short)ByteReceived) == 1)
			*ByteToReceive = (char)SYS16_SetBit((unsigned short)i, (unsigned short)*ByteToReceive);
		else
			*ByteToReceive = (char)SYS16_ClrBit((unsigned short)i, (unsigned short)*ByteToReceive);

		//***************************
		//	Set SCL to 0
		//***************************
		buffer[0] = (unsigned int)SYS16_ClrBit(1, (unsigned short)buffer[0]);
		Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

		/* Wait for 600 ns */
		wait_for_a_short_time((int)(600.0 / (double)Ns_Per_Cycle));
	}

	return(0);
}


/* ----------------------------------------------------- */
// I2CM24C64_getACK:
//   Bus master receives ACKNOWLEDGE from M24C64
//   keep CTRL = 0 to leave bus to memory for reading
/* ----------------------------------------------------- */

char I2CM24C64_getACK(unsigned short ModNum)
{
	unsigned int buffer[4], rbuf[4];
	char retval;

	//***************************
	//	Set SCL to 1
	//***************************

	buffer[0] = 0x2;	/* SDA = 0; SCL = 1; CTRL = 0 */
	Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

	/* Wait for 300 ns */
	wait_for_a_short_time((int)(300.0 / (double)Ns_Per_Cycle));

	//***************************
	//	Read SDA
	//***************************
	Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_READ, rbuf);
	
	//***************************
	//	Set SCL to 0
	//***************************

	buffer[0] = 0x0;	/* SDA = 0; SCL = 0; CTRL = 0 */
	Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);
	
	/* Wait for 600 ns */
	wait_for_a_short_time((int)(600.0 / (double)Ns_Per_Cycle));
    
    retval = (char)(rbuf[0] & 0x1);
	return(retval);
}


/* ----------------------------------------------------- */
// I2CM24C64_sendACK:
//   Bus master sends ACKNOWLEDGE to M24C64   
/* ----------------------------------------------------- */

char I2CM24C64_sendACK(unsigned short ModNum)
{
	unsigned int buffer[4];
	
	//***************************
	//	Pull SDA down to LOW
	//***************************
	buffer[0] = 0x4;	/* SDA = 0; SCL = 0; CTRL = 1 */
	Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);
	
	/* Wait for 300 ns */
	wait_for_a_short_time((int)(300.0 / (double)Ns_Per_Cycle));

	//***************************
	//	Set SCL to 1 while keep SDA LOW
	//***************************

	buffer[0] = 0x6;	/* SDA = 0; SCL = 1; CTRL = 1 */
	Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

	/* Wait for 600 ns */
	wait_for_a_short_time((int)(600.0 / (double)Ns_Per_Cycle));

	//***************************
	//	Set SCL to 0
	//***************************

	buffer[0] = 0x0;	/* SDA = 0; SCL = 0; CTRL = 0 */
	Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);
	
	/* Wait for 300 ns */
	wait_for_a_short_time((int)(300.0 / (double)Ns_Per_Cycle));

	return(0);
}
