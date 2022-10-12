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

/** @file communication.c
 * @brief Implements internal functions used to directly access Pixie-16 hardware.
 * @note This file will be deprecated July 31, 2023
 */

#include <stdio.h>

#include <PlxApi.h>

#include <pixie16sys_common.h>
#include <pixie16sys_defs.h>
#include <pixie16sys_export.h>
#include <pixie16sys_globals.h>

#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#else
#include <ctype.h>
#include <unistd.h>
#endif

/*
 * Set to 1 to enable register tracing
 */
int Pixie_Register_IO_Trace;


int Pixie_DSP_Memory_IO(unsigned int* dsp_data, unsigned int dsp_address, unsigned int nWords,
                        unsigned short direction, unsigned short ModNum) {
    unsigned int numBlocks, remainWords;
    int retval;
    unsigned int buffer[128], CSR, k;
    unsigned int blockSize = 8192;

    if (SYS_Offline == 1) {
        return (0);
    }

    // Always use single word write
    if (direction == SYS_MOD_WRITE) {
        Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);  // HBR request

        Pixie_ReadCSR(ModNum, &CSR);
        CSR = SYS32_ClrBit(DSPDOWNLOAD, CSR);  // Enable address automatic increment
        Pixie_WrtCSR(ModNum, CSR);

        buffer[0] = dsp_address;  // start address
        Pixie_Register_IO(ModNum, EXT_MEM_TEST, SYS_MOD_WRITE, buffer);

        // Write to data memory
        for (k = 0; k < nWords; k++) {
            // Write one word to DSP data memory
            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_WRITE, &dsp_data[k]);
        }

        Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done
    } else if (direction == SYS_MOD_READ) {
        // Check if this is a single-word read
        if (nWords == 1) {
            Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);  // HBR request

            buffer[0] = dsp_address;  // start address
            Pixie_Register_IO(ModNum, EXT_MEM_TEST, SYS_MOD_WRITE, buffer);

            // Read one word from DSP data memory
            Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_READ, dsp_data);

            Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done
        } else {
            // multiple reads; use DMA burst reading
            //***********************************************
            //	DMA burst reading using fixed-size blocks
            //***********************************************
            numBlocks = (unsigned int) ((double) nWords / (double) blockSize);
            remainWords = nWords - numBlocks * blockSize;

            // Read blocks
            for (k = 0; k < numBlocks; k++) {
                retval = Pixie_DSP_Memory_Burst_Read(
                    dsp_data + k * blockSize, dsp_address + k * blockSize, blockSize, ModNum);
                if (retval < 0) {
                    Pixie_Print_Error(PIXIE_FUNC, "Reading DSP memory blocks failed, retval=%d",
                                      retval);
                    return (-1);
                }
            }

            // Read remaining words
            if (remainWords > 0) {
                if (remainWords <
                    48)  // Use single-word read if the remaining words are not greater than 48
                {
                    Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);  // HBR request

                    Pixie_ReadCSR(ModNum, &CSR);
                    CSR = SYS32_ClrBit(DSPDOWNLOAD, CSR);  // Enable address automatic increment
                    Pixie_WrtCSR(ModNum, CSR);

                    buffer[0] = dsp_address + numBlocks * blockSize;  // start address
                    Pixie_Register_IO(ModNum, EXT_MEM_TEST, SYS_MOD_WRITE, buffer);

                    for (k = 0; k < remainWords; k++) {
                        Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_READ,
                                          dsp_data + numBlocks * blockSize + k);
                    }

                    Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done
                } else {
                    retval = Pixie_DSP_Memory_Burst_Read(dsp_data + numBlocks * blockSize,
                                                         dsp_address + numBlocks * blockSize,
                                                         remainWords, ModNum);
                    if (retval < 0) {
                        Pixie_Print_Error(PIXIE_FUNC,
                                          "Reading DSP memory remaining words failed, retval=%d",
                                          retval);
                        return (-2);
                    }
                }
            }
        }
    }

    return (0);
}


/**
 * @ingroup COMMUNICATION
 * @brief Read data from Pixie DSP data memory using DMA burst mode.
 * @note This function depends on the PLX major revision number!
 * @param[out] dsp_data: DSP data for the I/O
 * @param[in] dsp_address: DSP data memory address
 * @param[in] nWords: Number of DSP data words for the I/O
 * @param[in] ModNum: The Pixie module for the I/O
 * @return A status code indicating the result of the operation
 * @retval  0 - I/O successful
 * @retval -1 - DSP DMAC11 is still active so can't start a new one
 * @retval -2 - Failed to open PLX DMA channel
 * @retval -3 - Failed to read FIFO watermark in System FPGA
 * @retval -4 - PlxPci_DmaTransferUserBuffer failed while reading from DSP memory
 * @retval -5 - PlxPci_DmaChannelClose failed after reading from DSP memory
 */
int Pixie_DSP_Memory_Burst_Read(unsigned int* dsp_data, unsigned int dsp_address,
                                unsigned int nWords, unsigned short ModNum) {
    unsigned int buffer[128];
    unsigned int dummy[8];
    unsigned int csr, count;

    PLX_STATUS rc;
    PLX_DMA_PROP DmaProp;
    PLX_DMA_PARAMS DmaParams;

    unsigned int localAddress = 0x300000;

    if (SYS_Offline == 1)  // Returns immediately for offline analysis
    {
        return (0);
    }

    //*********************************
    //	Request DSP bus
    //*********************************

    Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);  // HBR request

    // Check if DSP DMAC11 is already active
    buffer[0] = DMASTAT;
    Pixie_Register_IO(ModNum, EXT_MEM_TEST, SYS_MOD_WRITE, buffer);
    Pixie_Register_IO(ModNum, WRT_DSP_MMA, SYS_MOD_READ, buffer);  // read DSP DMASTAT
    if (SYS32_TstBit(11, buffer[0]) == 1) {
        Pixie_Print_Error(PIXIE_FUNC,
                          "DSP DMAC11 is still active so can't start a new one, ModNum=%d", ModNum);
        return (-1);
    }

    Pixie_ReadCSR(ModNum, &csr);
    csr = SYS32_ClrBit(DSPDOWNLOAD, csr);  // Enable address automatic increment
    Pixie_WrtCSR(ModNum, csr);

    buffer[0] = dsp_address;  // start address
    Pixie_Register_IO(ModNum, WRT_DSP_II11, SYS_MOD_WRITE, buffer);

    buffer[0] = nWords;  // number of words to read per DMA transfer
    Pixie_Register_IO(ModNum, WRT_DSP_C11, SYS_MOD_WRITE, buffer);

    buffer[0] = 1;  // Step size
    Pixie_Register_IO(ModNum, WRT_DSP_IM11, SYS_MOD_WRITE, buffer);

    buffer[0] = nWords;  // EC11
    Pixie_Register_IO(ModNum, WRT_DSP_EC11, SYS_MOD_WRITE, buffer);

    buffer[0] = 0x905;  // DMAC11
    Pixie_Register_IO(ModNum, WRT_DSP_DMAC11, SYS_MOD_WRITE, buffer);

    buffer[0] = nWords / 2;  // Used for setting watermark level of system FPGA FIFO
    Pixie_Register_IO(ModNum, RD_WRT_FIFO_WML, SYS_MOD_WRITE, buffer);

    //*********************************
    //	Release DSP bus
    //*********************************

    Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done

    // Read from data memory

    // Clear DMA descriptor structure
    memset(&DmaProp, 0, sizeof(PLX_DMA_PROP));

    // Set up DMA configuration structure
    DmaProp.ReadyInput = 1;
    DmaProp.Burst = 1;
    DmaProp.BurstInfinite = 1;
#if PLX_SDK_VERSION_MAJOR == 6 || PLX_SDK_VERSION_MAJOR == 7 || PLX_SDK_VERSION_MAJOR == 8
    DmaProp.ConstAddrLocal = 1;
#else
    DmaProp.LocalAddrConst = 1;
#endif
    DmaProp.LocalBusWidth = 2;  // 32-bit bus

    rc = PlxPci_DmaChannelOpen(&SYS_hDevice[ModNum], 0, &DmaProp);
    if (rc != ApiSuccess) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to open PLX DMA channel in module %d, rc=%d", ModNum,
                          rc);
        //*********************************
        //	Disable DSP DMA
        //*********************************
        Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);  // HBR request
        buffer[0] = 0x904;  // DMAC11
        Pixie_Register_IO(ModNum, WRT_DSP_DMAC11, SYS_MOD_WRITE, buffer);
        Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done

        return (-2);
    }

    // Get the internal FIFO in System FPGA ready for data read-out
    Pixie_Register_IO(ModNum, SET_INT_FIFO, SYS_MOD_WRITE, dummy);

    // Waiting for the FIFO to be ready
    count = 0;
    do {
        Pixie_ReadCSR(ModNum, &csr);
        if (SYS32_TstBit(EXTFIFO_WML, csr))
            break;

        count++;
    } while (count < 1000);

    if (count == 1000) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to read FIFO watermark in System FPGA in module %d",
                          ModNum);
        //*********************************
        //	Close PLX SGL DMA channel
        //*********************************
        rc = PlxPci_DmaChannelClose(&SYS_hDevice[ModNum], 0);
        if (rc != ApiSuccess) {
            Pixie_Print_Error(
                PIXIE_FUNC,
                "PlxPci_DmaChannelClose failed after reading FIFO watermark failed in System FPGA in module %d, rc=%d",
                ModNum, rc);
        }

        //*********************************
        //	Disable DSP DMA
        //*********************************
        Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);  // HBR request
        buffer[0] = 0x904;  // DMAC11
        Pixie_Register_IO(ModNum, WRT_DSP_DMAC11, SYS_MOD_WRITE, buffer);
        Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done

        return (-3);
    }

    // Clear DMA parameters
    memset(&DmaParams, 0, sizeof(PLX_DMA_PARAMS));

#if PLX_SDK_VERSION_MAJOR == 6 || PLX_SDK_VERSION_MAJOR == 7 || PLX_SDK_VERSION_MAJOR == 8
    DmaParams.UserVa = PLX_PTR_TO_INT(dsp_data);
#else
    DmaParams.u.UserVa = (PLX_UINT_PTR) dsp_data;
#endif

    DmaParams.LocalAddr = localAddress;
    DmaParams.ByteCount = nWords * 4;  // Read nWords*4 bytes
#if PLX_SDK_VERSION_MAJOR == 6 || PLX_SDK_VERSION_MAJOR == 7 || PLX_SDK_VERSION_MAJOR == 8
    DmaParams.Direction = PLX_DMA_LOC_TO_PCI;
#else
    DmaParams.LocalToPciDma = 1;
#endif

    rc = PlxPci_DmaTransferUserBuffer(&SYS_hDevice[ModNum], 0, &DmaParams, DMATRANSFER_TIMEOUT);
    if (rc != ApiSuccess) {
        Pixie_Print_Error(
            PIXIE_FUNC,
            "PlxPci_DmaTransferUserBuffer failed while reading from DSP memory in module %d, rc=%d",
            ModNum, rc);

        //*********************************
        //	Disable DSP DMA
        //*********************************
        Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);  // HBR request
        buffer[0] = 0x904;  // DMAC11
        Pixie_Register_IO(ModNum, WRT_DSP_DMAC11, SYS_MOD_WRITE, buffer);
        Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done

        return (-4);
    }

    rc = PlxPci_DmaChannelClose(&SYS_hDevice[ModNum], 0);
    if (rc != ApiSuccess) {
        // Reset the device if a DMA is in-progress
        if (rc == ApiDmaInProgress) {
            PlxPci_DeviceReset(&SYS_hDevice[ModNum]);
            // Attempt to close again
            PlxPci_DmaChannelClose(&SYS_hDevice[ModNum], 0);
        } else {
            Pixie_Print_Error(
                PIXIE_FUNC,
                "PlxPci_DmaChannelClose failed after reading from DSP memory in module %d, rc=%d",
                ModNum, rc);

            //*********************************
            //	Disable DSP DMA
            //*********************************
            Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);  // HBR request
            buffer[0] = 0x904;  // DMAC11
            Pixie_Register_IO(ModNum, WRT_DSP_DMAC11, SYS_MOD_WRITE, buffer);
            Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done

            return (-5);
        }
    }

    //*********************************
    //	Disable DSP DMA
    //*********************************
    Pixie_Register_IO(ModNum, REQUEST_HBR, SYS_MOD_WRITE, buffer);  // HBR request

    buffer[0] = 0x904;  // DMAC11
    Pixie_Register_IO(ModNum, WRT_DSP_DMAC11, SYS_MOD_WRITE, buffer);
    Pixie_Register_IO(ModNum, HBR_DONE, SYS_MOD_WRITE, buffer);  // HBR done

    return (0);
}


int Pixie_Read_ExtFIFOStatus(unsigned int* nFIFOWords, unsigned short ModNum) {

    if (SYS_Offline == 1) {
        return (0);
    }

    Pixie_Register_IO(ModNum, RD_WRT_FIFO_WML, SYS_MOD_READ, nFIFOWords);

    return (0);
}


int Pixie_ExtFIFO_Read(unsigned int* extfifo_data, unsigned int nWords, unsigned short ModNum) {
    unsigned int count, wml;

    PLX_STATUS rc;
    PLX_DMA_PROP DmaProp;
    PLX_DMA_PARAMS DmaParams;

    unsigned int localAddress = 0x200000;

    if (SYS_Offline == 1)  // Returns immediately for offline analysis
    {
        return (0);
    }

    // Clear DMA descriptor structure
    memset(&DmaProp, 0, sizeof(PLX_DMA_PROP));

    // Set up DMA configuration structure
    DmaProp.ReadyInput = 1;
    DmaProp.Burst = 1;
    DmaProp.BurstInfinite = 1;
#if PLX_SDK_VERSION_MAJOR == 6 || PLX_SDK_VERSION_MAJOR == 7 || PLX_SDK_VERSION_MAJOR == 8
    DmaProp.ConstAddrLocal = 1;
#else
    DmaProp.LocalAddrConst = 1;
#endif
    DmaProp.LocalBusWidth = 2;  // 32-bit bus

    rc = PlxPci_DmaChannelOpen(&SYS_hDevice[ModNum], 0, &DmaProp);
    if (rc != ApiSuccess) {
        Pixie_Print_Error(PIXIE_FUNC, "Failed to open PLX DMA channel in module %d, rc=%d", ModNum,
                          rc);
        return (-1);
    }

    // Get external FIFO ready for data read-out
    Pixie_Register_IO(ModNum, SET_EXT_FIFO, SYS_MOD_WRITE, &nWords);

    // Waiting for the FIFO to be ready
    count = 0;
    do {
        Pixie_Register_IO(ModNum, RD_WRT_FIFO_WML, SYS_MOD_READ, &wml);
        if (wml >= nWords)
            break;

        count++;
    } while (count < 1000);

    if (count == 1000) {
        Pixie_Print_Error(
            PIXIE_FUNC,
            "Failed to read FIFO watermark in System FPGA in module %d, nWords=%u, wml=%u", ModNum,
            nWords, wml);

        //*********************************
        //	Close PLX SGL DMA channel
        //*********************************
        rc = PlxPci_DmaChannelClose(&SYS_hDevice[ModNum], 0);
        if (rc != ApiSuccess) {
            Pixie_Print_Error(
                PIXIE_FUNC,
                "PlxPci_DmaChannelClose failed after reading FIFO watermark failed in System FPGA in module %d, rc=%d",
                ModNum, rc);
        }

        return (-2);
    }

    // Clear DMA parameters
    memset(&DmaParams, 0, sizeof(PLX_DMA_PARAMS));

#if PLX_SDK_VERSION_MAJOR == 6 || PLX_SDK_VERSION_MAJOR == 7 || PLX_SDK_VERSION_MAJOR == 8
    DmaParams.UserVa = PLX_PTR_TO_INT(extfifo_data);
#else
    DmaParams.u.UserVa = (PLX_UINT_PTR) extfifo_data;
#endif
    DmaParams.LocalAddr = localAddress;
    DmaParams.ByteCount = nWords * 4;  // Read nWords*4 bytes
#if PLX_SDK_VERSION_MAJOR == 6 || PLX_SDK_VERSION_MAJOR == 7 || PLX_SDK_VERSION_MAJOR == 8
    DmaParams.Direction = PLX_DMA_LOC_TO_PCI;
#else
    DmaParams.LocalToPciDma = 1;
#endif

    rc = PlxPci_DmaTransferUserBuffer(&SYS_hDevice[ModNum], 0, &DmaParams, DMATRANSFER_TIMEOUT);
    if (rc != ApiSuccess) {
        Pixie_Print_Error(
            PIXIE_FUNC,
            "PlxPci_DmaTransferUserBuffer failed while reading from external FIFO in module %d, rc=%d",
            ModNum, rc);
        return (-3);
    }

    rc = PlxPci_DmaChannelClose(&SYS_hDevice[ModNum], 0);
    if (rc != ApiSuccess) {
        // Reset the device if a DMA is in-progress
        if (rc == ApiDmaInProgress) {
            PlxPci_DeviceReset(&SYS_hDevice[ModNum]);
            // Attempt to close again
            PlxPci_DmaChannelClose(&SYS_hDevice[ModNum], 0);
        } else {
            Pixie_Print_Error(
                PIXIE_FUNC,
                "PlxPci_DmaChannelClose failed after reading from DSP memory in module %d, rc=%d",
                ModNum, rc);
            return (-4);
        }
    }

    return (0);
}


int Pixie_Main_Memory_IO(unsigned int* memory_data, unsigned int memory_address,
                         unsigned int nWords, unsigned short direction, unsigned short ModNum) {
    unsigned int k, dummy[16], CSR;
    PLX_STATUS rc;
    PLX_DMA_PROP DmaProp;
    PLX_DMA_PARAMS DmaParams;
    unsigned int localAddress = 0x400000;

    if (SYS_Offline == 1)  // Returns immediately for offline analysis
    {
        return (0);
    }

    // Set bit 2 of CSR to indicate that PCI is accessing the external memory
    Pixie_ReadCSR(ModNum, &CSR);
    CSR = SYS32_SetBit(PCIACTIVE, CSR);
    // Write back CSR
    Pixie_WrtCSR(ModNum, CSR);

    if (direction == SYS_MOD_WRITE) {  // Write to external memory

        // Write the starting address
        Pixie_Register_IO(ModNum, WRT_EXT_MEM, SYS_MOD_WRITE, &memory_address);

        for (k = 0; k < nWords; k++) {
            Pixie_Register_IO(ModNum, localAddress, SYS_MOD_WRITE, memory_data);
            memory_data++;
        }
    } else if (direction == SYS_MOD_READ) {  // Read from external memory

        // Set starting address in the external memory
        Pixie_Register_IO(ModNum, WRT_EXT_MEM, SYS_MOD_WRITE, &memory_address);

        // Set up short FIFO in System FPGA
        Pixie_Register_IO(ModNum, SET_EXMEM_FIFO, SYS_MOD_WRITE, dummy);

        // Clear DMA descriptor structure
        memset(&DmaProp, 0, sizeof(PLX_DMA_PROP));

        // Set up DMA configuration structure
        DmaProp.ReadyInput = 1;
        DmaProp.Burst = 1;
        DmaProp.BurstInfinite = 1;
#if PLX_SDK_VERSION_MAJOR == 6 || PLX_SDK_VERSION_MAJOR == 7 || PLX_SDK_VERSION_MAJOR == 8
        DmaProp.ConstAddrLocal = 1;
#else
        DmaProp.LocalAddrConst = 1;
#endif
        DmaProp.LocalBusWidth = 2;  // 32-bit bus

        rc = PlxPci_DmaChannelOpen(&SYS_hDevice[ModNum], 0, &DmaProp);
        if (rc != ApiSuccess) {
            Pixie_Print_Error(PIXIE_FUNC, "Failed to open PLX DMA channel in module %d, rc=%d",
                              ModNum, rc);

            // Clear bit 2 of CSR to release PCI's control of external memory
            CSR &= 0xFFFB;
            Pixie_Register_IO(ModNum, CSR_ADDR, SYS_MOD_WRITE, &CSR);

            return (-1);
        }

        // Clear DMA parameters
        memset(&DmaParams, 0, sizeof(PLX_DMA_PARAMS));

#if PLX_SDK_VERSION_MAJOR == 6 || PLX_SDK_VERSION_MAJOR == 7 || PLX_SDK_VERSION_MAJOR == 8
        DmaParams.UserVa = PLX_PTR_TO_INT(memory_data);
#else
        DmaParams.u.UserVa = (PLX_UINT_PTR) memory_data;
#endif
        DmaParams.LocalAddr = localAddress;
        DmaParams.ByteCount = nWords * 4;  // Read nWords*4 bytes
#if PLX_SDK_VERSION_MAJOR == 6 || PLX_SDK_VERSION_MAJOR == 7 || PLX_SDK_VERSION_MAJOR == 8
        DmaParams.Direction = PLX_DMA_LOC_TO_PCI;
#else
        DmaParams.LocalToPciDma = 1;
#endif

        rc = PlxPci_DmaTransferUserBuffer(&SYS_hDevice[ModNum], 0, &DmaParams, DMATRANSFER_TIMEOUT);
        if (rc != ApiSuccess) {
            Pixie_Print_Error(
                PIXIE_FUNC,
                "PlxPci_DmaTransferUserBuffer failed while reading from external memory in module %d, rc=%d",
                ModNum, rc);

            // Clear bit 2 of CSR to release PCI's control of external memory
            CSR &= 0xFFFB;
            Pixie_Register_IO(ModNum, CSR_ADDR, SYS_MOD_WRITE, &CSR);

            return (-2);
        }

        rc = PlxPci_DmaChannelClose(&SYS_hDevice[ModNum], 0);
        if (rc != ApiSuccess) {
            // Reset the device if a DMA is in-progress
            if (rc == ApiDmaInProgress) {
                PlxPci_DeviceReset(&SYS_hDevice[ModNum]);
                // Attempt to close again
                PlxPci_DmaChannelClose(&SYS_hDevice[ModNum], 0);
            } else {
                Pixie_Print_Error(
                    PIXIE_FUNC,
                    "PlxPci_DmaChannelClose failed after reading from external memory in module %d, rc=%d",
                    ModNum, rc);

                // Clear bit 2 of CSR to release PCI's control of external memory
                CSR &= 0xFFFB;
                Pixie_Register_IO(ModNum, CSR_ADDR, SYS_MOD_WRITE, &CSR);

                return (-3);
            }
        }
    }

    // Clear bit 2 of CSR to release PCI's control of external memory
    CSR &= 0xFFFB;
    Pixie_Register_IO(ModNum, CSR_ADDR, SYS_MOD_WRITE, &CSR);

    return (0);
}


int Pixie_Clear_Main_Memory(unsigned int memory_address, unsigned int nWords,
                            unsigned short ModNum) {
    unsigned int CSR, count;

    if (SYS_Offline == 1) {
        return (0);
    }

    // Set bit 2 of CSR to indicate that PCI is accessing the external memory
    Pixie_ReadCSR(ModNum, &CSR);
    CSR = SYS32_SetBit(PCIACTIVE, CSR);
    // Write back CSR
    Pixie_WrtCSR(ModNum, CSR);

    // First, write the starting address
    Pixie_Register_IO(ModNum, WRT_EXT_MEM, SYS_MOD_WRITE, &memory_address);

    // Now, write the number of words to clear. This will also immediately kick
    // off the clearing of the external memory
    Pixie_Register_IO(ModNum, WRT_CLR_EXTMEM, SYS_MOD_WRITE, &nWords);

    // Wait until the clearing is done
    count = 0;
    do {
#if defined(_WIN64) || defined(_WIN32)
        Sleep(1);
#else
        usleep(100);
#endif

        Pixie_ReadCSR(ModNum, &CSR);

        // If bit CLREXTMEM_ACTIVE of CSR is 0, the clearing is done
        if ((CSR & (0x1 << CLREXTMEM_ACTIVE)) == 0x0000)
            break;

        count++;
    } while (count < 1000);

    if (count == 1000)  // Timed out
    {
        Pixie_Print_Error(PIXIE_FUNC,
                          "Clearing external memory by System FPGA timedout in Module %d", ModNum);

        // Clear bit 2 of CSR to release PCI's control of external memory
        CSR &= 0xFFFB;
        Pixie_Register_IO(ModNum, CSR_ADDR, SYS_MOD_WRITE, &CSR);

        return (-1);
    }

    // Clear bit 2 of CSR to release PCI's control of external memory
    CSR &= 0xFFFB;
    Pixie_Register_IO(ModNum, CSR_ADDR, SYS_MOD_WRITE, &CSR);

    return (0);
}


int Pixie_Register_IO(unsigned short ModNum, unsigned int address, unsigned short direction,
                      unsigned int* value) {
    if (SYS_Offline == 1) {
        return (0);
    }

    if (direction == SYS_MOD_WRITE)  // Write to register
    {
        *(unsigned int*) (VAddr[ModNum] + address) = *value;
    } else if (direction == SYS_MOD_READ)  // Read from register
    {
        *value = *(unsigned int*) (VAddr[ModNum] + address);
    }

    if (Pixie_Register_IO_Trace) {
        Pixie_Print_Debug(PIXIE_FUNC, "IO %c %2d %p:%02x %s %08x",
                          direction == SYS_MOD_WRITE ? 'w' : 'r', ModNum,
                          (unsigned int*) VAddr[ModNum], address,
                          direction == SYS_MOD_WRITE ? "<=" : "=>", *value);
    }

    return (0);
}


void Pixie_ReadCSR(unsigned short ModNum, unsigned int* CSR) {
    if (SYS_Offline == 1) {
        return;
    }

    *CSR = *(unsigned int*) (VAddr[ModNum] + CSR_ADDR);

    if (Pixie_Register_IO_Trace) {
        Pixie_Print_Debug(PIXIE_FUNC, "IO %c %2d %p:%02x %s %08x", 'r', ModNum,
                          (unsigned int*) VAddr[ModNum], CSR_ADDR, "=>", *CSR);
    }
}


void Pixie_WrtCSR(unsigned short ModNum, unsigned int CSR) {
    if (SYS_Offline == 1)  // Returns immediately for offline analysis
    {
        return;
    }

    *(unsigned int*) (VAddr[ModNum] + CSR_ADDR) = CSR;

    if (Pixie_Register_IO_Trace) {
        Pixie_Print_Debug(PIXIE_FUNC, "IO %c %2d %p:%02x %s %08x", 'w', ModNum,
                          (unsigned int*) VAddr[ModNum], CSR_ADDR, "<=", CSR);
    }
}
