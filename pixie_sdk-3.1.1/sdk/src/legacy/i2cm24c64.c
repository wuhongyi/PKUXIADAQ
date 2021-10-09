/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file i2cm24c64.c
 * @brief Implements functions to access the Pixie-16's 64 Kb serial I2C bus EEPROM.
 * @note This file will be deprecated July 31, 2023
*/

#include <math.h>

#include <pixie16sys_common.h>
#include <pixie16sys_defs.h>
#include <pixie16sys_export.h>
#include <pixie16sys_globals.h>


int I2CM24C64_Write_One_Byte(unsigned short ModNum, unsigned short Address, char* ByteValue) {
    char IOByte;
    char ackvalue, byteaddrMSB, byteaddrLSB;

    // Check if ModNum is valid
    if (ModNum >= SYS_Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "Invalid Pixie module number %d", ModNum);
        return (-1);
    }

    // Extract I2CM24C64 byte address
    byteaddrMSB = (char) ((float) Address / 256.0);
    byteaddrLSB = (char) (fmod((double) Address, 256.0));

    // Send "START"
    I2CM24C64_start(ModNum);

    // Send Device Select Code
    IOByte = (char) 0xA0;
    I2CM24C64_byte_write(ModNum, IOByte);

    // Get Acknowledge
    ackvalue = I2CM24C64_getACK(ModNum);
    if (ackvalue != 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending DevSel byte");
        return (-2);
    }

    // Send address
    IOByte = byteaddrMSB;
    I2CM24C64_byte_write(ModNum, IOByte);
    // Get Acknowledge
    ackvalue = I2CM24C64_getACK(ModNum);
    if (ackvalue != 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending address byte (MSB)");
        return (-3);
    }

    IOByte = byteaddrLSB;
    I2CM24C64_byte_write(ModNum, IOByte);
    // Get Acknowledge
    ackvalue = I2CM24C64_getACK(ModNum);
    if (ackvalue != 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending address byte (LSB)");
        return (-4);
    }

    // Send byte value
    IOByte = *ByteValue;
    I2CM24C64_byte_write(ModNum, IOByte);

    // Get Acknowledge
    ackvalue = I2CM24C64_getACK(ModNum);
    if (ackvalue != 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending byte value");
        return (-5);
    }

    // Send "STOP"
    I2CM24C64_stop(ModNum);

    return (0);
}


int I2CM24C64_Read_One_Byte(unsigned short ModNum, unsigned short Address, char* ByteValue) {
    char IOByte;
    char ackvalue, byteaddrMSB, byteaddrLSB;

    // Check if ModNum is valid
    if (ModNum >= SYS_Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "Invalid Pixie module number %d", ModNum);
        return (-1);
    }

    // Extract I2CM24C64 byte address
    byteaddrMSB = (char) ((float) Address / 256.0);
    byteaddrLSB = (char) (fmod((double) Address, 256.0));

    // Send "START"
    I2CM24C64_start(ModNum);

    // Send Device Select Code
    IOByte = (char) 0xA0;
    I2CM24C64_byte_write(ModNum, IOByte);

    // Get Acknowledge
    ackvalue = I2CM24C64_getACK(ModNum);
    if (ackvalue != 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending DevSel byte");
        return (-2);
    }

    // Send address
    IOByte = byteaddrMSB;
    I2CM24C64_byte_write(ModNum, IOByte);
    // Get Acknowledge
    ackvalue = I2CM24C64_getACK(ModNum);
    if (ackvalue != 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending address byte (MSB)");
        return (-3);
    }

    IOByte = byteaddrLSB;
    I2CM24C64_byte_write(ModNum, IOByte);
    // Get Acknowledge
    ackvalue = I2CM24C64_getACK(ModNum);
    if (ackvalue != 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending address byte (LSB)");
        return (-4);
    }

    // Send "START"
    I2CM24C64_start(ModNum);

    // Send Device Select Code
    IOByte = (char) 0xA1;
    I2CM24C64_byte_write(ModNum, IOByte);

    // Get Acknowledge
    ackvalue = I2CM24C64_getACK(ModNum);
    if (ackvalue != 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending DevSel byte");
        return (-5);
    }

    // Receive one byte
    I2CM24C64_byte_read(ModNum, ByteValue);

    // Send "STOP"
    I2CM24C64_stop(ModNum);

    return (0);
}


int I2CM24C64_Page_Write(unsigned short ModNum, unsigned short Address,
                         unsigned short NumBytesToWrite, char* ByteValue) {
    char IOByte;
    char ackvalue, byteaddrMSB, byteaddrLSB;
    unsigned short k;

    // Check if ModNum is valid
    if (ModNum >= SYS_Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "Invalid Pixie module number %d", ModNum);
        return (-1);
    }

    // Extract I2CM24C64 byte address
    byteaddrMSB = (char) ((float) Address / 256.0);
    byteaddrLSB = (char) (fmod((double) Address, 256.0));

    // Send "START"
    I2CM24C64_start(ModNum);

    // Send Device Select Code
    IOByte = (char) 0xA0;
    I2CM24C64_byte_write(ModNum, IOByte);

    // Get Acknowledge
    ackvalue = I2CM24C64_getACK(ModNum);
    if (ackvalue != 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending DevSel byte");
        return (-2);
    }

    // Send address
    IOByte = byteaddrMSB;
    I2CM24C64_byte_write(ModNum, IOByte);
    // Get Acknowledge
    ackvalue = I2CM24C64_getACK(ModNum);
    if (ackvalue != 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending address byte (MSB)");
        return (-3);
    }

    IOByte = byteaddrLSB;
    I2CM24C64_byte_write(ModNum, IOByte);
    // Get Acknowledge
    ackvalue = I2CM24C64_getACK(ModNum);
    if (ackvalue != 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending address byte (LSB)");
        return (-4);
    }

    for (k = 0; k < NumBytesToWrite; k++) {
        IOByte = *ByteValue;
        I2CM24C64_byte_write(ModNum, IOByte);
        // Get Acknowledge
        ackvalue = I2CM24C64_getACK(ModNum);
        if (ackvalue != 0) {
            Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after writing a byte");
            return (-5);
        }

        // Increment address
        ByteValue++;
    }

    // Send "STOP"
    I2CM24C64_stop(ModNum);

    return (0);
}


int I2CM24C64_Sequential_Read(unsigned short ModNum, unsigned short Address,
                              unsigned short NumBytesToRead, char* ByteValue) {
    char IOByte;
    char ackvalue, byteaddrMSB, byteaddrLSB;
    unsigned short k;

    // Check if ModNum is valid
    if (ModNum >= SYS_Number_Modules) {
        Pixie_Print_Error(PIXIE_FUNC, "Invalid Pixie module number %d", ModNum);
        return (-1);
    }

    // Extract I2CM24C64 byte address
    byteaddrMSB = (char) ((float) Address / 256.0);
    byteaddrLSB = (char) (fmod((double) Address, 256.0));

    // Send "START"
    I2CM24C64_start(ModNum);

    // Send Device Select Code
    IOByte = (char) 0xA0;
    I2CM24C64_byte_write(ModNum, IOByte);

    // Get Acknowledge
    ackvalue = I2CM24C64_getACK(ModNum);
    if (ackvalue != 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending DevSel byte");
        return (-2);
    }

    // Send address
    IOByte = byteaddrMSB;
    I2CM24C64_byte_write(ModNum, IOByte);
    // Get Acknowledge
    ackvalue = I2CM24C64_getACK(ModNum);
    if (ackvalue != 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending address byte (MSB)");
        return (-3);
    }

    IOByte = byteaddrLSB;
    I2CM24C64_byte_write(ModNum, IOByte);
    // Get Acknowledge
    ackvalue = I2CM24C64_getACK(ModNum);
    if (ackvalue != 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending address byte (LSB)");
        return (-4);
    }

    // Send "START"
    I2CM24C64_start(ModNum);

    // Send Device Select Code
    IOByte = (char) 0xA1;
    I2CM24C64_byte_write(ModNum, IOByte);

    // Get Acknowledge
    ackvalue = I2CM24C64_getACK(ModNum);
    if (ackvalue != 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending DevSel byte");
        return (-5);
    }

    for (k = 0; k < NumBytesToRead; k++) {
        // Receive one byte
        I2CM24C64_byte_read(ModNum, ByteValue);

        if (k != (NumBytesToRead - 1)) {
            // Send Acknowledge
            I2CM24C64_sendACK(ModNum);

            // Increment address
            ByteValue++;
        }
    }

    // Send "STOP"
    I2CM24C64_stop(ModNum);

    return (0);
}


/**
 * @ingroup I2CM24C64
 * @brief Bus master sends "START" to M24C64
 * @param[in] ModNum The module that we want to communicate with.
 * @returns A status code indicating the result of the operation
 * @retval 0 - Success
 */
int I2CM24C64_start(unsigned short ModNum) {
    unsigned int buffer[4];

    //***************************
    //	Set SCL and SDA to 1
    //***************************

    buffer[0] = SDA | SCL | CTRL;
    Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 600 ns */
    wait_for_a_short_time((int) (600.0 / (double) Ns_Per_Cycle));

    //***************************
    //	Set SDA to 0 while keep SCL at 1
    //***************************

    buffer[0] = SCL | CTRL;
    Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 600 ns */
    wait_for_a_short_time((int) (600.0 / (double) Ns_Per_Cycle));

    return (0);
}


/**
 * @ingroup I2CM24C64
 * @brief Bus master sends "STOP" to M24C64
 * @param[in] ModNum The module that we want to communicate with.
 * @returns A status code indicating the result of the operation
 * @retval 0 - Success
 */
int I2CM24C64_stop(unsigned short ModNum) {
    unsigned int buffer[4];

    //***************************
    //	Set SCL to 1 and SDA to 0
    //***************************

    buffer[0] = SCL | CTRL;
    Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 600 ns */
    wait_for_a_short_time((int) (600.0 / (double) Ns_Per_Cycle));

    //***************************
    //	Set SDA to 1 while keep SCL at 1
    //***************************

    buffer[0] = SDA | SCL | CTRL;
    Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 1300 ns */
    wait_for_a_short_time((int) (1300.0 / (double) Ns_Per_Cycle));

    return (0);
}


/**
 * @ingroup I2CM24C64
 * @brief Bus master sends a byte to M24C64
 * @param[in] ModNum The module that we want to communicate with.
 * @param[in] ByteToSend: The byte that we want to write to the chip.
 * @returns A status code indicating the result of the operation
 * @retval 0 - Success
 */
int I2CM24C64_byte_write(unsigned short ModNum, char ByteToSend) {
    short i;
    unsigned int buffer[4];

    /* Initialize buffer[0] */

    buffer[0] = CTRL;

    for (i = 7; i >= 0; i--) {
        //***************************
        //	Set SCL to 0
        //***************************

        buffer[0] = (unsigned int) SYS16_ClrBit(1, (unsigned short) buffer[0]);
        Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

        /* Wait for 700 ns */
        wait_for_a_short_time((int) (700.0 / (double) Ns_Per_Cycle));

        //***************************
        //	Send bit i
        //***************************

        if (SYS16_TstBit((unsigned short) i, (unsigned short) ByteToSend) == 1)
            buffer[0] = SDA | CTRL;
        else
            buffer[0] = CTRL;

        Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

        /* Wait for 600 ns */
        wait_for_a_short_time((int) (600.0 / (double) Ns_Per_Cycle));

        //***************************
        //	Set SCL to 1
        //***************************
        buffer[0] = SYS16_SetBit(1, (unsigned short) buffer[0]);
        Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

        /* Wait for 600 ns */
        wait_for_a_short_time((int) (600.0 / (double) Ns_Per_Cycle));
    }

    //************************************************************
    //	Set SCL and CTRL to 0 to release bus for acknowledge
    //***********************************************************

    buffer[0] = (buffer[0] & 0x1);
    Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 600 ns */
    wait_for_a_short_time((int) (600.0 / (double) Ns_Per_Cycle));

    return (0);
}


/**
 * @ingroup I2CM24C64
 * @brief Bus master reads a byte to M24C64
 * @param[in] ModNum: The module that we want to communicate with.
 * @param[out] ByteToReceive: Pointer to variable to hold the return value.
 * @returns A status code indicating the result of the operation
 * @retval 0 - Success
 */
int I2CM24C64_byte_read(unsigned short ModNum, char* ByteToReceive) {
    short i;
    unsigned int buffer[4];
    char ByteReceived;

    buffer[0] = 0x0;

    for (i = 7; i >= 0; i--) {
        //***************************
        //	Set SCL to 1
        //***************************

        buffer[0] = (unsigned int) SYS16_SetBit(1, (unsigned short) buffer[0]);
        Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

        /* Wait for 600 ns */
        wait_for_a_short_time((int) (600.0 / (double) Ns_Per_Cycle));

        //***************************
        //	Receive bit i
        //***************************

        buffer[0] = 0x0;

        Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_READ, buffer);
        ByteReceived = (char) buffer[0];

        if (SYS16_TstBit(0, (unsigned short) ByteReceived) == 1)
            *ByteToReceive =
                (char) SYS16_SetBit((unsigned short) i, (unsigned short) *ByteToReceive);
        else
            *ByteToReceive =
                (char) SYS16_ClrBit((unsigned short) i, (unsigned short) *ByteToReceive);

        //***************************
        //	Set SCL to 0
        //***************************
        buffer[0] = (unsigned int) SYS16_ClrBit(1, (unsigned short) buffer[0]);
        Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

        /* Wait for 600 ns */
        wait_for_a_short_time((int) (600.0 / (double) Ns_Per_Cycle));
    }

    return (0);
}


/**
 * @ingroup I2CM24C64
 * @brief Bus master receives ACKNOWLEDGE from M24C64
 * @note keep CTRL = 0 to leave bus to memory for reading
 * @param[in] ModNum The module that we want to communicate with.
 * @returns A status code indicating the result of the operation
 * @retval 0 - Success
 */
char I2CM24C64_getACK(unsigned short ModNum) {
    unsigned int buffer[4], rbuf[4];
    char retval;

    //***************************
    //	Set SCL to 1
    //***************************

    buffer[0] = SCL;
    Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 300 ns */
    wait_for_a_short_time((int) (300.0 / (double) Ns_Per_Cycle));

    //***************************
    //	Read SDA
    //***************************
    Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_READ, rbuf);

    //***************************
    //	Set SCL to 0
    //***************************

    buffer[0] = 0x0; /* SDA = 0; SCL = 0; CTRL = 0 */
    Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 600 ns */
    wait_for_a_short_time((int) (600.0 / (double) Ns_Per_Cycle));

    retval = (char) (rbuf[0] & 0x1);
    return (retval);
}


/**
 * @ingroup I2CM24C64
 * @brief Bus master sends ACKNOWLEDGE to M24C64
 * @param[in] ModNum The module that we want to communicate with.
 * @returns A status code indicating the result of the operation
 * @retval 0 - Success
 */
char I2CM24C64_sendACK(unsigned short ModNum) {
    unsigned int buffer[4];

    //***************************
    //	Pull SDA down to LOW
    //***************************
    buffer[0] = CTRL;
    Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 300 ns */
    wait_for_a_short_time((int) (300.0 / (double) Ns_Per_Cycle));

    //***************************
    //	Set SCL to 1 while keep SDA LOW
    //***************************

    buffer[0] = SCL | CTRL;
    Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 600 ns */
    wait_for_a_short_time((int) (600.0 / (double) Ns_Per_Cycle));

    //***************************
    //	Set SCL to 0
    //***************************

    buffer[0] = 0x0;
    Pixie_Register_IO(ModNum, I2CM24C64_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 300 ns */
    wait_for_a_short_time((int) (300.0 / (double) Ns_Per_Cycle));

    return (0);
}
