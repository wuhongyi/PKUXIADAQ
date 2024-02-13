#ifndef __PEX_API_H
#define __PEX_API_H

/*******************************************************************************
 * Copyright 2013-2019 Avago Technologies
 * Copyright (c) 2009 to 2012 PLX Technology Inc.  All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directorY of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

/******************************************************************************
 *
 * File Name:
 *
 *     PexApi.h
 *
 * Description:
 *
 *     This file contains all the PLX API function prototypes
 *
 * Revision:
 *
 *     09-01-19 : PCI/PCIe SDK v8.10
 *
 ******************************************************************************/


#include "PlxTypes.h"


#ifdef __cplusplus
extern "C" {
#endif


/******************************************
 *             Definitions
 ******************************************/
// DLL support
#ifndef EXPORT
    #if defined(PLX_MSWINDOWS)
        #define EXPORT __declspec(dllexport)
    #else
        #define EXPORT
    #endif
#endif




/******************************************
 *      Device Selection Functions
 *****************************************/
PLX_STATUS EXPORT
PlxPci_DeviceOpen(
    PLX_DEVICE_KEY    *pKey,
    PLX_DEVICE_OBJECT *pDevice
    );

PLX_STATUS EXPORT
PlxPci_DeviceClose(
    PLX_DEVICE_OBJECT *pDevice
    );

PLX_STATUS EXPORT
PlxPci_DeviceFind(
    PLX_DEVICE_KEY *pKey,
    U16             DeviceNumber
    );

PLX_STATUS EXPORT
PlxPci_DeviceFindEx(
    PLX_DEVICE_KEY *pKey,
    U16             DeviceNumber,
    PLX_API_MODE    ApiMode,
    PLX_MODE_PROP  *pModeProp
    );

PLX_STATUS EXPORT
PlxPci_I2cGetPorts(
    PLX_API_MODE  ApiMode,
    U32          *pI2cPorts
    );


/******************************************
 *    Query for Information Functions
 *****************************************/
PLX_STATUS EXPORT
PlxPci_ApiVersion(
    U8 *pVersionMajor,
    U8 *pVersionMinor,
    U8 *pVersionRevision
    );

PLX_STATUS EXPORT
PlxPci_I2cVersion(
    U16          I2cPort,
    PLX_VERSION *pVersion
    );

PLX_STATUS EXPORT
PlxPci_DriverVersion(
    PLX_DEVICE_OBJECT *pDevice,
    U8                *pVersionMajor,
    U8                *pVersionMinor,
    U8                *pVersionRevision
    );

PLX_STATUS EXPORT
PlxPci_DriverProperties(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_DRIVER_PROP   *pDriverProp
    );

PLX_STATUS EXPORT
PlxPci_DriverScheduleRescan(
    PLX_DEVICE_OBJECT *pDevice
    );

PLX_STATUS EXPORT
PlxPci_ChipTypeGet(
    PLX_DEVICE_OBJECT *pDevice,
    U16               *pChipType,
    U8                *pRevision
    );

PLX_STATUS EXPORT
PlxPci_ChipTypeSet(
    PLX_DEVICE_OBJECT *pDevice,
    U16                ChipType,
    U8                 Revision
    );

PLX_STATUS EXPORT
PlxPci_ChipGetPortMask(
    U32            ChipID,
    U8             Revision,
    PEX_CHIP_FEAT *PtrFeat
    );

PLX_STATUS EXPORT
PlxPci_GetPortProperties(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_PORT_PROP     *pPortProp
    );


/******************************************
 *        Device Control Functions
 *****************************************/
PLX_STATUS EXPORT
PlxPci_DeviceReset(
    PLX_DEVICE_OBJECT *pDevice
    );


/******************************************
 *        Register Access Functions
 *****************************************/
U32 EXPORT
PlxPci_PciRegisterRead(
    U8          bus,
    U8          slot,
    U8          function,
    U16         offset,
    PLX_STATUS *pStatus
    );

PLX_STATUS EXPORT
PlxPci_PciRegisterWrite(
    U8  bus,
    U8  slot,
    U8  function,
    U16 offset,
    U32 value
    );

U32 EXPORT
PlxPci_PciRegisterReadFast(
    PLX_DEVICE_OBJECT *pDevice,
    U16                offset,
    PLX_STATUS        *pStatus
    );

PLX_STATUS EXPORT
PlxPci_PciRegisterWriteFast(
    PLX_DEVICE_OBJECT *pDevice,
    U16                offset,
    U32                value
    );

U32 EXPORT
PlxPci_PciRegisterRead_BypassOS(
    U8          bus,
    U8          slot,
    U8          function,
    U16         offset,
    PLX_STATUS *pStatus
    );

PLX_STATUS EXPORT
PlxPci_PciRegisterWrite_BypassOS(
    U8  bus,
    U8  slot,
    U8  function,
    U16 offset,
    U32 value
    );


/******************************************
 * Device-specific Register Functions
 *****************************************/
U32 EXPORT
PlxPci_PlxRegisterRead(
    PLX_DEVICE_OBJECT *pDevice,
    U32                offset,
    PLX_STATUS        *pStatus
    );

PLX_STATUS EXPORT
PlxPci_PlxRegisterWrite(
    PLX_DEVICE_OBJECT *pDevice,
    U32                offset,
    U32                value
    );

U32 EXPORT
PlxPci_PlxMappedRegisterRead(
    PLX_DEVICE_OBJECT *pDevice,
    U32                offset,
    PLX_STATUS        *pStatus
    );

PLX_STATUS EXPORT
PlxPci_PlxMappedRegisterWrite(
    PLX_DEVICE_OBJECT *pDevice,
    U32                offset,
    U32                value
    );

U32 EXPORT
PlxPci_PlxMailboxRead(
    PLX_DEVICE_OBJECT *pDevice,
    U16                mailbox,
    PLX_STATUS        *pStatus
    );

PLX_STATUS EXPORT
PlxPci_PlxMailboxWrite(
    PLX_DEVICE_OBJECT *pDevice,
    U16                mailbox,
    U32                value
    );


/******************************************
 *         PCI Mapping Functions
 *****************************************/
PLX_STATUS EXPORT
PlxPci_PciBarProperties(
    PLX_DEVICE_OBJECT *pDevice,
    U8                 BarIndex,
    PLX_PCI_BAR_PROP  *pBarProperties
    );

PLX_STATUS EXPORT
PlxPci_PciBarMap(
    PLX_DEVICE_OBJECT  *pDevice,
    U8                  BarIndex,
    VOID              **pVa
    );

PLX_STATUS EXPORT
PlxPci_PciBarUnmap(
    PLX_DEVICE_OBJECT  *pDevice,
    VOID              **pVa
    );


/******************************************
 *   BAR I/O & Memory Access Functions
 *****************************************/
PLX_STATUS EXPORT
PlxPci_IoPortRead(
    PLX_DEVICE_OBJECT *pDevice,
    U64                port,
    VOID              *pBuffer,
    U32                ByteCount,
    PLX_ACCESS_TYPE    AccessType
    );

PLX_STATUS EXPORT
PlxPci_IoPortWrite(
    PLX_DEVICE_OBJECT *pDevice,
    U64                port,
    VOID              *pBuffer,
    U32                ByteCount,
    PLX_ACCESS_TYPE    AccessType
    );

PLX_STATUS EXPORT
PlxPci_PciBarSpaceRead(
    PLX_DEVICE_OBJECT *pDevice,
    U8                 BarIndex,
    U32                offset,
    VOID              *pBuffer,
    U32                ByteCount,
    PLX_ACCESS_TYPE    AccessType,
    BOOLEAN            bOffsetAsLocalAddr
    );

PLX_STATUS EXPORT
PlxPci_PciBarSpaceWrite(
    PLX_DEVICE_OBJECT *pDevice,
    U8                 BarIndex,
    U32                offset,
    VOID              *pBuffer,
    U32                ByteCount,
    PLX_ACCESS_TYPE    AccessType,
    BOOLEAN            bOffsetAsLocalAddr
    );


/******************************************
 *       Physical Memory Functions
 *****************************************/
PLX_STATUS EXPORT
PlxPci_PhysicalMemoryAllocate(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_PHYSICAL_MEM  *pMemoryInfo,
    BOOLEAN            bSmallerOk
    );

PLX_STATUS EXPORT
PlxPci_PhysicalMemoryFree(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_PHYSICAL_MEM  *pMemoryInfo
    );

PLX_STATUS EXPORT
PlxPci_PhysicalMemoryMap(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_PHYSICAL_MEM  *pMemoryInfo
    );

PLX_STATUS EXPORT
PlxPci_PhysicalMemoryUnmap(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_PHYSICAL_MEM  *pMemoryInfo
    );

PLX_STATUS EXPORT
PlxPci_CommonBufferProperties(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_PHYSICAL_MEM  *pMemoryInfo
    );

PLX_STATUS EXPORT
PlxPci_CommonBufferMap(
    PLX_DEVICE_OBJECT  *pDevice,
    VOID              **pVa
    );

PLX_STATUS EXPORT
PlxPci_CommonBufferUnmap(
    PLX_DEVICE_OBJECT  *pDevice,
    VOID              **pVa
    );


/******************************************
 *     Interrupt Support Functions
 *****************************************/
PLX_STATUS EXPORT
PlxPci_InterruptEnable(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_INTERRUPT     *pPlxIntr
    );

PLX_STATUS EXPORT
PlxPci_InterruptDisable(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_INTERRUPT     *pPlxIntr
    );

PLX_STATUS EXPORT
PlxPci_NotificationRegisterFor(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_INTERRUPT     *pPlxIntr,
    PLX_NOTIFY_OBJECT *pEvent
    );

PLX_STATUS EXPORT
PlxPci_NotificationWait(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_NOTIFY_OBJECT *pEvent,
    U64                Timeout_ms
    );

PLX_STATUS EXPORT
PlxPci_NotificationStatus(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_NOTIFY_OBJECT *pEvent,
    PLX_INTERRUPT     *pPlxIntr
    );

PLX_STATUS EXPORT
PlxPci_NotificationCancel(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_NOTIFY_OBJECT *pEvent
    );


/******************************************
 *     Serial EEPROM Access Functions
 *****************************************/
PLX_EEPROM_STATUS EXPORT
PlxPci_EepromPresent(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_STATUS        *pStatus
    );

BOOLEAN EXPORT
PlxPci_EepromProbe(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_STATUS        *pStatus
    );

PLX_STATUS EXPORT
PlxPci_EepromGetAddressWidth(
    PLX_DEVICE_OBJECT *pDevice,
    U8                *pWidth
    );

PLX_STATUS EXPORT
PlxPci_EepromSetAddressWidth(
    PLX_DEVICE_OBJECT *pDevice,
    U8                 width
    );

PLX_STATUS EXPORT
PlxPci_EepromCrcUpdate(
    PLX_DEVICE_OBJECT *pDevice,
    U32               *pCrc,
    BOOLEAN            bUpdateEeprom
    );

PLX_STATUS EXPORT
PlxPci_EepromCrcGet(
    PLX_DEVICE_OBJECT *pDevice,
    U32               *pCrc,
    U8                *pCrcStatus
    );

PLX_STATUS EXPORT
PlxPci_EepromReadByOffset(
    PLX_DEVICE_OBJECT *pDevice,
    U32                offset,
    U32               *pValue
    );

PLX_STATUS EXPORT
PlxPci_EepromWriteByOffset(
    PLX_DEVICE_OBJECT *pDevice,
    U32                offset,
    U32                value
    );

PLX_STATUS EXPORT
PlxPci_EepromReadByOffset_16(
    PLX_DEVICE_OBJECT *pDevice,
    U32                offset,
    U16               *pValue
    );

PLX_STATUS EXPORT
PlxPci_EepromWriteByOffset_16(
    PLX_DEVICE_OBJECT *pDevice,
    U32                offset,
    U16                value
    );


/******************************************
 *     SPI Flash Functions
 *****************************************/
PLX_STATUS EXPORT
PlxPci_SpiFlashPropGet(
    PLX_DEVICE_OBJECT *PtrDev,
    U8                 ChipSel,
    PEX_SPI_OBJ       *PtrSpi
    );

PLX_STATUS EXPORT
PlxPci_SpiFlashErase(
    PLX_DEVICE_OBJECT *PtrDev,
    PEX_SPI_OBJ       *PtrSpi,
    U32                StartOffset,
    U8                 BoolWaitComplete
    );

PLX_STATUS EXPORT
PlxPci_SpiFlashReadBuffer(
    PLX_DEVICE_OBJECT *PtrDev,
    PEX_SPI_OBJ       *PtrSpi,
    U32                StartOffset,
    U8                *PtrRxBuff,
    U32                SizeRx
    );

U32 EXPORT
PlxPci_SpiFlashReadByOffset(
    PLX_DEVICE_OBJECT *PtrDev,
    PEX_SPI_OBJ       *PtrSpi,
    U32                Offset,
    PLX_STATUS        *PtrStatus
    );

PLX_STATUS EXPORT
PlxPci_SpiFlashWriteBuffer(
    PLX_DEVICE_OBJECT *PtrDev,
    PEX_SPI_OBJ       *PtrSpi,
    U32                StartOffset,
    U8                *PtrTxBuff,
    U32                SizeTx
    );

PLX_STATUS EXPORT
PlxPci_SpiFlashWriteByOffset(
    PLX_DEVICE_OBJECT *PtrDev,
    PEX_SPI_OBJ       *PtrSpi,
    U32                Offset,
    U32                Data
    );

PLX_STATUS EXPORT
PlxPci_SpiFlashGetStatus(
    PLX_DEVICE_OBJECT *PtrDev,
    PEX_SPI_OBJ       *PtrSpi
    );


/******************************************
 *          PLX VPD Functions
 *****************************************/
U32 EXPORT 
PlxPci_VpdRead(
    PLX_DEVICE_OBJECT *pDevice,
    U16                offset,
    PLX_STATUS        *pStatus
    );

PLX_STATUS EXPORT
PlxPci_VpdWrite(
    PLX_DEVICE_OBJECT *pDevice,
    U16                offset,
    U32                value
    );


/******************************************
 *            DMA Functions
 *****************************************/
PLX_STATUS EXPORT
PlxPci_DmaChannelOpen(
    PLX_DEVICE_OBJECT *pDevice,
    U8                 channel,
    PLX_DMA_PROP      *pDmaProp
    );

PLX_STATUS EXPORT
PlxPci_DmaGetProperties(
    PLX_DEVICE_OBJECT *pDevice,
    U8                 channel,
    PLX_DMA_PROP      *pDmaProp
    );

PLX_STATUS EXPORT
PlxPci_DmaSetProperties(
    PLX_DEVICE_OBJECT *pDevice,
    U8                 channel,
    PLX_DMA_PROP      *pDmaProp
    );

PLX_STATUS EXPORT
PlxPci_DmaControl(
    PLX_DEVICE_OBJECT *pDevice,
    U8                 channel,
    PLX_DMA_COMMAND    command
    );

PLX_STATUS EXPORT
PlxPci_DmaStatus(
    PLX_DEVICE_OBJECT *pDevice,
    U8                 channel
    );

PLX_STATUS EXPORT
PlxPci_DmaTransferBlock(
    PLX_DEVICE_OBJECT *pDevice,
    U8                 channel,
    PLX_DMA_PARAMS    *pDmaParams,
    U64                Timeout_ms
    );

PLX_STATUS EXPORT
PlxPci_DmaTransferUserBuffer(
    PLX_DEVICE_OBJECT *pDevice,
    U8                 channel,
    PLX_DMA_PARAMS    *pDmaParams,
    U64                Timeout_ms
    );

PLX_STATUS EXPORT
PlxPci_DmaChannelClose(
    PLX_DEVICE_OBJECT *pDevice,
    U8                 channel
    );


/******************************************
 *   Performance Monitoring Functions
 *****************************************/
PLX_STATUS EXPORT
PlxPci_PerformanceInitializeProperties(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_PERF_PROP     *pPerfObject
    );

PLX_STATUS EXPORT
PlxPci_PerformanceMonitorControl(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_PERF_CMD       command
    );

PLX_STATUS EXPORT
PlxPci_PerformanceResetCounters(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_PERF_PROP     *pPerfProps,
    U8                 NumOfObjects
    );

PLX_STATUS EXPORT
PlxPci_PerformanceGetCounters(
    PLX_DEVICE_OBJECT *pDevice,
    PLX_PERF_PROP     *pPerfProps,
    U8                 NumOfObjects
    );

PLX_STATUS EXPORT
PlxPci_PerformanceCalcStatistics(
    PLX_PERF_PROP  *pPerfProp,
    PLX_PERF_STATS *pPerfStats,
    U32             ElapsedTime_ms
    );


/******************************************
 *    Multi-Host Switch Functions
 *****************************************/
PLX_STATUS EXPORT
PlxPci_MH_GetProperties(
    PLX_DEVICE_OBJECT   *pDevice,
    PLX_MULTI_HOST_PROP *pMHProp
    );

PLX_STATUS EXPORT
PlxPci_MH_MigratePorts(
    PLX_DEVICE_OBJECT *pDevice,
    U16                VS_Source,
    U16                VS_Dest,
    U32                DsPortMask,
    BOOLEAN            bResetSrc
    );


/******************************************
 *  PLX Non-Transparent Port Functions
 *****************************************/
PLX_STATUS EXPORT
PlxPci_Nt_ReqIdProbe(
    PLX_DEVICE_OBJECT *pDevice,
    BOOLEAN            bRead,
    U16               *pReqId
    );

PLX_STATUS EXPORT
PlxPci_Nt_LutProperties(
    PLX_DEVICE_OBJECT *pDevice,
    U16                LutIndex,
    U16               *pReqId,
    U32               *pFlags,
    BOOLEAN           *pbEnabled
    );

PLX_STATUS EXPORT
PlxPci_Nt_LutAdd(
    PLX_DEVICE_OBJECT *pDevice,
    U16               *pLutIndex,
    U16                ReqId,
    U32                flags
    );

PLX_STATUS EXPORT
PlxPci_Nt_LutDisable(
    PLX_DEVICE_OBJECT *pDevice,
    U16                LutIndex
    );




#ifdef __cplusplus
}
#endif

#endif
