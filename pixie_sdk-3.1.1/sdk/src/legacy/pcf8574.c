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

/** @file pcf8574.c
 * @brief Implements PCF8574 communication to read physical slots from the module.
 * @note This file will be deprecated July 31, 2023
*/

#include <pixie16sys_common.h>
#include <pixie16sys_defs.h>
#include <pixie16sys_export.h>
#include <pixie16sys_globals.h>


int PCF8574_Read_One_Byte(unsigned short ModNum, char* ByteValue) {
    char IOByte;
    char ackvalue;

    // Check if ModNum is valid
    if (ModNum >= SYS_MAX_NUM_MODULES) {
        Pixie_Print_Error(PIXIE_FUNC, "Invalid Pixie module number %d", ModNum);
        return (-1);
    }

    // Send "START"
    PCF8574_start(ModNum);

    // Send Device Select Code
    IOByte = (char) 0x43;
    PCF8574_byte_write(ModNum, IOByte);

    // Get Acknowledge
    ackvalue = PCF8574_getACK(ModNum);
    if (ackvalue != 0) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to get Acknowledge after sending DevSel byte");
        return (-2);
    }

    // Receive one byte
    PCF8574_byte_read(ModNum, ByteValue);

    Pixie_Print_Info(PIXIE_FUNC, "ByteValue read = %x", *ByteValue);

    // Send Acknowledge
    PCF8574_sendACK(ModNum);

    // Send "STOP"
    PCF8574_stop(ModNum);

    return (0);
}


/**
 * @ingroup PCF8574
 * @brief Bus master sends "START" to PCF8574
 * @param[in] ModNum The module that we want to communicate with.
 * @returns A status code indicating the result of the operation
 * @retval 0 - Success
 */
int PCF8574_start(unsigned short ModNum) {
    unsigned int buffer[4];

    //***************************
    //	Set SCL and SDA to 1
    //***************************

    buffer[0] = SDA | SCL | CTRL;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int) (6000.0 / (double) Ns_Per_Cycle));

    //***************************
    //	Set SDA to 0 while keep SCL at 1
    //***************************

    buffer[0] = SCL | CTRL;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int) (6000.0 / (double) Ns_Per_Cycle));

    return (0);
}


/**
 * @ingroup PCF8574
 * @brief Bus master sends "STOP" to PCF8574
 * @param[in] ModNum The module that we want to communicate with.
 * @returns A status code indicating the result of the operation
 * @retval 0 - Success
 */
int PCF8574_stop(unsigned short ModNum) {
    unsigned int buffer[4];

    //***************************
    //	Set SCL to 1 and SDA to 0
    //***************************

    buffer[0] = SCL | CTRL;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int) (6000.0 / (double) Ns_Per_Cycle));

    //***************************
    //	Set SDA to 1 while keep SCL at 1
    //***************************

    buffer[0] = SDA | SCL | CTRL;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int) (6000.0 / (double) Ns_Per_Cycle));

    return (0);
}


/**
 * @ingroup PCF8574
 * @brief Bus master sends a byte to PCF8574
 * @param[in] ModNum: The module that we want to communicate with.
 * @param[in] ByteToSend: The value to write to the chip.
 * @returns A status code indicating the result of the operation
 * @retval 0 - Success
 */
int PCF8574_byte_write(unsigned short ModNum, char ByteToSend) {
    short i;
    unsigned int buffer[4];

    /* Initialize buffer[0] */

    buffer[0] = CTRL;

    for (i = 7; i >= 0; i--) {
        //***************************
        //	Set SCL to 0
        //***************************

        buffer[0] = (unsigned int) SYS16_ClrBit(1, (unsigned short) buffer[0]);
        Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

        /* Wait for 6000 ns */
        wait_for_a_short_time((int) (6000.0 / (double) Ns_Per_Cycle));

        //***************************
        //	Send bit i
        //***************************

        if (SYS16_TstBit((unsigned short) i, (unsigned short) ByteToSend) == 1)
            buffer[0] = SDA | CTRL;
        else
            buffer[0] = CTRL;

        Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

        /* Wait for 1000 ns */
        wait_for_a_short_time((int) (1000.0 / (double) Ns_Per_Cycle));

        //***************************
        //	Set SCL to 1
        //***************************
        buffer[0] = SYS16_SetBit(1, (unsigned short) buffer[0]);
        Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

        /* Wait for 6000 ns */
        wait_for_a_short_time((int) (6000.0 / (double) Ns_Per_Cycle));
    }

    //************************************************************
    //	Set SCL and CTRL to 0 to release bus for acknowledge
    //***********************************************************

    buffer[0] = (buffer[0] & 0x1);
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int) (6000.0 / (double) Ns_Per_Cycle));

    return (0);
}


/**
 * @ingroup PCF8574
 * @brief Bus master receives a byte from PCF8574
 * @param[in] ModNum: The module that we want to communicate with.
 * @param[out] ByteToReceive: The value read from the chip.
 * @returns A status code indicating the result of the operation
 * @retval 0 - Success
 */
int PCF8574_byte_read(unsigned short ModNum, char* ByteToReceive) {
    short i;
    unsigned int buffer[4];
    char ByteReceived;

    buffer[0] = 0x0;

    for (i = 7; i >= 0; i--) {
        //***************************
        //	Set SCL to 1
        //***************************

        buffer[0] = (unsigned int) SYS16_SetBit(1, (unsigned short) buffer[0]);
        Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

        /* Wait for 6000 ns */
        wait_for_a_short_time((int) (6000.0 / (double) Ns_Per_Cycle));

        //***************************
        //	Receive bit i
        //***************************

        buffer[0] = 0x0;

        Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_READ, buffer);
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
        Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

        /* Wait for 6000 ns */
        wait_for_a_short_time((int) (6000.0 / (double) Ns_Per_Cycle));
    }

    return (0);
}


/**
 * @ingroup PCF8574
 * @brief Bus master receives ACKNOWLEDGE from PCF8574
 * @note keep CTRL = 0 to leave bus to memory for reading
 * @param[in] ModNum: The module that we want to communicate with.
 * @returns A status code indicating the result of the operation
 * @retval 0 - Success
 */
char PCF8574_getACK(unsigned short ModNum) {
    unsigned int buffer[4], rbuf[4];
    char retval;

    //***************************
    //	Set SCL to 1
    //***************************

    buffer[0] = SCL;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int) (6000.0 / (double) Ns_Per_Cycle));

    //***************************
    //	Read SDA
    //***************************
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_READ, rbuf);

    //***************************
    //	Set SCL to 0
    //***************************

    buffer[0] = 0x0;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int) (6000.0 / (double) Ns_Per_Cycle));

    retval = (char) (rbuf[0] & 0x1);
    return (retval);
}


/**
 * @ingroup PCF8574
 * @brief Bus master sends ACKNOWLEDGE to PCF8574
 * @param[in] ModNum: The module that we want to communicate with.
 * @returns A status code indicating the result of the operation
 * @retval 0 - Success
 */
char PCF8574_sendACK(unsigned short ModNum) {
    unsigned int buffer[4];

    //***************************
    //	Pull SDA down to LOW
    //***************************
    buffer[0] = CTRL;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 1000 ns */
    wait_for_a_short_time((int) (1000.0 / (double) Ns_Per_Cycle));

    //***************************
    //	Set SCL to 1 while keep SDA LOW
    //***************************

    buffer[0] = SCL | CTRL;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int) (6000.0 / (double) Ns_Per_Cycle));

    //***************************
    //	Set SCL to 0
    //***************************

    buffer[0] = 0x0;
    Pixie_Register_IO(ModNum, PCF8574_ADDR, SYS_MOD_WRITE, buffer);

    /* Wait for 6000 ns */
    wait_for_a_short_time((int) (6000.0 / (double) Ns_Per_Cycle));

    return (0);
}
