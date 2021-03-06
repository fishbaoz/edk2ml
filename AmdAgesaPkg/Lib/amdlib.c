/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Library
 *
 * Contains interface to the AMD AGESA library
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ******************************************************************************
 *
 * Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
 *
 * AMD is granting you permission to use this software (the Materials)
 * pursuant to the terms and conditions of your Software License Agreement
 * with AMD.  This header does *NOT* give you permission to use the Materials
 * or any rights under AMD's intellectual property.  Your use of any portion
 * of these Materials shall constitute your acceptance of those terms and
 * conditions.  If you do not agree to the terms and conditions of the Software
 * License Agreement, please do not use any portion of these Materials.
 *
 * CONFIDENTIALITY:  The Materials and all other information, identified as
 * confidential and provided to you by AMD shall be kept confidential in
 * accordance with the terms and conditions of the Software License Agreement.
 *
 * LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
 * PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
 * OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
 * IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
 * (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
 * INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
 * GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
 * RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
 * EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
 * THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
 *
 * AMD does not assume any responsibility for any errors which may appear in
 * the Materials or any other related information provided to you by AMD, or
 * result from use of the Materials or any related information.
 *
 * You agree that you will not reverse engineer or decompile the Materials.
 *
 * NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
 * further information, software, technical information, know-how, or show-how
 * available to you.  Additionally, AMD retains the right to modify the
 * Materials at any time, without notice, and is not obligated to provide such
 * modified Materials to you.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
 * "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
 * subject to the restrictions as set forth in FAR 52.227-14 and
 * DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
 * Government constitutes acknowledgement of AMD's proprietary rights in them.
 *
 * EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
 * direct product thereof will be exported directly or indirectly, into any
 * country prohibited by the United States Export Administration Act and the
 * regulations thereunder, without the required authorization from the U.S.
 * government nor will be used for any purpose prohibited by the same.
 ******************************************************************************
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include "AGESA.h"
#include "Ids.h"
#include "cpuRegisters.h"
#include "amdlib.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE LIB_AMDLIB_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


BOOLEAN
STATIC
GetPciMmioAddress (
     OUT   UINT64            *MmioAddress,
     OUT   UINT32            *MmioSize,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
STATIC
LibAmdGetDataFromPtr (
  IN       ACCESS_WIDTH AccessWidth,
  IN       VOID         *Data,
  IN       VOID         *DataMask,
     OUT   UINT32       *TemData,
     OUT   UINT32       *TempDataMask
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*---------------------------------------------------------------------------------------*/
/**
 * Read IO port
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] IoAddress     IO port address
 * @param[in] Value         Pointer to save data
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdIoRead (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT16 IoAddress,
     OUT   VOID *Value,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  switch (AccessWidth) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    *(UINT8 *) Value = ReadIo8 (IoAddress);
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    *(UINT16 *) Value = ReadIo16 (IoAddress);
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    *(UINT32 *) Value = ReadIo32 (IoAddress);
    break;
  default:
    ASSERT (FALSE);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write IO port
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] IoAddress     IO port address
 * @param[in] Value         Pointer to data
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdIoWrite (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT16 IoAddress,
  IN       VOID *Value,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  switch (AccessWidth) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    WriteIo8 (IoAddress, *(UINT8 *) Value);
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    WriteIo16 (IoAddress, *(UINT16 *) Value);
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    WriteIo32 (IoAddress, *(UINT32 *) Value);
    break;
  default:
    ASSERT (FALSE);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * IO read modify write
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] IoAddress     IO address
 * @param[in] Data          OR data
 * @param[in] DataMask      Mask to be used before data write back to register.
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdIoRMW (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT16 IoAddress,
  IN       VOID *Data,
  IN       VOID *DataMask,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32  TempData;
  UINT32  TempMask;
  UINT32  Value;
  LibAmdGetDataFromPtr (AccessWidth, Data,  DataMask, &TempData, &TempMask);
  LibAmdIoRead (AccessWidth, IoAddress, &Value, StdHeader);
  Value = (Value & (~TempMask)) | TempData;
  LibAmdIoWrite (AccessWidth, IoAddress, &Value, StdHeader);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Poll IO register
 *
 *  Poll register until (RegisterValue & DataMask) ==  Data
 *
 * @param[in] AccessWidth   Access width
 * @param[in] IoAddress     IO address
 * @param[in] Data          Data to compare
 * @param[in] DataMask      And mask
 * @param[in] Delay         Poll for time in 100ns (not supported)
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdIoPoll (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT16 IoAddress,
  IN       VOID *Data,
  IN       VOID *DataMask,
  IN       UINT64 Delay,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32  TempData;
  UINT32  TempMask;
  UINT32  Value;
  LibAmdGetDataFromPtr (AccessWidth, Data,  DataMask, &TempData, &TempMask);
  do {
    LibAmdIoRead (AccessWidth, IoAddress, &Value, StdHeader);
  } while (TempData != (Value & TempMask));
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read memory/MMIO
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] MemAddress    Memory address
 * @param[in] Value         Pointer to data
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdMemRead (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT64 MemAddress,
     OUT   VOID *Value,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  switch (AccessWidth) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    *(UINT8 *) Value = Read64Mem8 (MemAddress);
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    *(UINT16 *) Value = Read64Mem16 (MemAddress);
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    *(UINT32 *) Value = Read64Mem32 (MemAddress);
    break;
  default:
    ASSERT (FALSE);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write memory/MMIO
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] MemAddress    Memory address
 * @param[in] Value         Pointer to data
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdMemWrite (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT64 MemAddress,
  IN       VOID *Value,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{

  switch (AccessWidth) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    Write64Mem8 (MemAddress, *((UINT8 *) Value));
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    Write64Mem16 (MemAddress, *((UINT16 *) Value));
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    Write64Mem32 (MemAddress, *((UINT32 *) Value));
    break;
  default:
    ASSERT (FALSE);
  }
}
/*---------------------------------------------------------------------------------------*/
/**
 * Memory/MMIO read modify write
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] MemAddress    Memory address
 * @param[in] Data          OR data
 * @param[in] DataMask      Mask to be used before data write back to register.
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdMemRMW (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT64 MemAddress,
  IN       VOID *Data,
  IN       VOID *DataMask,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32  TempData;
  UINT32  TempMask;
  UINT32  Value;
  LibAmdGetDataFromPtr (AccessWidth, Data,  DataMask, &TempData, &TempMask);
  LibAmdMemRead (AccessWidth, MemAddress, &Value, StdHeader);
  Value = (Value & (~TempMask)) | TempData;
  LibAmdMemWrite (AccessWidth, MemAddress, &Value, StdHeader);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Poll Mmio
 *
 *  Poll register until (RegisterValue & DataMask) ==  Data
 *
 * @param[in] AccessWidth   Access width
 * @param[in] MemAddress    Memory address
 * @param[in] Data          Data to compare
 * @param[in] DataMask      AND mask
 * @param[in] Delay         Poll for time in 100ns (not supported)
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdMemPoll (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT64 MemAddress,
  IN       VOID *Data,
  IN       VOID *DataMask,
  IN       UINT64 Delay,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32  TempData;
  UINT32  TempMask;
  UINT32  Value;
  LibAmdGetDataFromPtr (AccessWidth, Data,  DataMask, &TempData, &TempMask);
  do {
    LibAmdMemRead (AccessWidth, MemAddress, &Value, StdHeader);
  } while (TempData != (Value & TempMask));
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read PCI config space
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] PciAddress    Pci address
 * @param[in] Value         Pointer to data
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdPciRead (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR PciAddress,
     OUT   VOID *Value,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32 LegacyPciAccess;
  UINT32 MMIOSize;
  UINT64 RMWrite;
  UINT64 RMWritePrevious;
  UINT64 MMIOAddress;

  ASSERT (StdHeader != NULL);
  ASSERT (PciAddress.AddressValue != ILLEGAL_SBDFO);
  if (!GetPciMmioAddress (&MMIOAddress, &MMIOSize, StdHeader)) {
    // We need to convert our "portable" PCI address into a "real" PCI access
    LegacyPciAccess = ((1 << 31) + (PciAddress.Address.Register & 0xFC) + (PciAddress.Address.Function << 8) + (PciAddress.Address.Device << 11) + (PciAddress.Address.Bus << 16) + ((PciAddress.Address.Register & 0xF00) << (24 - 8)));
    if (PciAddress.Address.Register <= 0xFF) {
      LibAmdIoWrite (AccessWidth32, IOCF8, &LegacyPciAccess, StdHeader);
      LibAmdIoRead (AccessWidth, IOCFC + (UINT16) (PciAddress.Address.Register & 0x3), Value, StdHeader);
    } else {
      LibAmdMsrRead  (NB_CFG, &RMWritePrevious, StdHeader);
      RMWrite = RMWritePrevious | 0x0000400000000000;
      LibAmdMsrWrite (NB_CFG, &RMWrite, StdHeader);
      LibAmdIoWrite (AccessWidth32, IOCF8, &LegacyPciAccess, StdHeader);
      LibAmdIoRead (AccessWidth, IOCFC + (UINT16) (PciAddress.Address.Register & 0x3), Value, StdHeader);
      LibAmdMsrWrite (NB_CFG, &RMWritePrevious, StdHeader);
    }
    IDS_HDT_CONSOLE (LIB_PCI_RD, "~PCI RD %08x = %08x\n", LegacyPciAccess, *(UINT32 *)Value);
  } else {
    // Setup the MMIO address
    ASSERT ((MMIOAddress + MMIOSize) > (MMIOAddress + (PciAddress.AddressValue & 0x0FFFFFFF)));
    MMIOAddress += (PciAddress.AddressValue & 0x0FFFFFFF);
    LibAmdMemRead (AccessWidth, MMIOAddress, Value, StdHeader);
    IDS_HDT_CONSOLE (LIB_PCI_RD, "~MMIO RD %08x = %08x\n", (UINT32) MMIOAddress, *(UINT32 *)Value);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write PCI config space
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] PciAddress    Pci address
 * @param[in] Value         Pointer to data
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdPciWrite (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR PciAddress,
  IN       VOID *Value,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32 LegacyPciAccess;
  UINT32 MMIOSize;
  UINT64 RMWrite;
  UINT64 RMWritePrevious;
  UINT64 MMIOAddress;

  ASSERT (StdHeader != NULL);
  ASSERT (PciAddress.AddressValue != ILLEGAL_SBDFO);
  if (!GetPciMmioAddress (&MMIOAddress, &MMIOSize, StdHeader)) {
    // We need to convert our "portable" PCI address into a "real" PCI access
    LegacyPciAccess = ((1 << 31) + (PciAddress.Address.Register & 0xFC) + (PciAddress.Address.Function << 8) + (PciAddress.Address.Device << 11) + (PciAddress.Address.Bus << 16) + ((PciAddress.Address.Register & 0xF00) << (24 - 8)));
    if (PciAddress.Address.Register <= 0xFF) {
      LibAmdIoWrite (AccessWidth32, IOCF8, &LegacyPciAccess, StdHeader);
      LibAmdIoWrite (AccessWidth, IOCFC + (UINT16) (PciAddress.Address.Register & 0x3), Value, StdHeader);
    } else {
      LibAmdMsrRead  (NB_CFG, &RMWritePrevious, StdHeader);
      RMWrite = RMWritePrevious | 0x0000400000000000;
      LibAmdMsrWrite (NB_CFG, &RMWrite, StdHeader);
      LibAmdIoWrite (AccessWidth32, IOCF8, &LegacyPciAccess, StdHeader);
      LibAmdIoWrite (AccessWidth, IOCFC + (UINT16) (PciAddress.Address.Register & 0x3), Value, StdHeader);
      LibAmdMsrWrite (NB_CFG, &RMWritePrevious, StdHeader);
    }
    IDS_HDT_CONSOLE (LIB_PCI_WR, "~PCI WR %08x = %08x\n", LegacyPciAccess, *(UINT32 *)Value);
  } else {
    // Setup the MMIO address
    ASSERT ((MMIOAddress + MMIOSize) > (MMIOAddress + (PciAddress.AddressValue & 0x0FFFFFFF)));
    MMIOAddress += (PciAddress.AddressValue & 0x0FFFFFFF);
    LibAmdMemWrite (AccessWidth, MMIOAddress, Value, StdHeader);
    IDS_HDT_CONSOLE (LIB_PCI_WR, "~MMIO WR %08x = %08x\n", (UINT32) MMIOAddress, *(UINT32 *)Value);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * PCI read modify write
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] PciAddress    Pci address
 * @param[in] Data          OR Data
 * @param[in] DataMask      Mask to be used before data write back to register.
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdPciRMW (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR PciAddress,
  IN       VOID *Data,
  IN       VOID *DataMask,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32  TempData;
  UINT32  TempMask;
  UINT32  Value;
  LibAmdGetDataFromPtr (AccessWidth, Data,  DataMask, &TempData, &TempMask);
  LibAmdPciRead (AccessWidth, PciAddress, &Value, StdHeader);
  Value = (Value & (~TempMask)) | TempData;
  LibAmdPciWrite (AccessWidth, PciAddress, &Value, StdHeader);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Poll PCI config space register
 *
 *  Poll register until (RegisterValue & DataMask) ==  Data
 *
 * @param[in] AccessWidth   Access width
 * @param[in] PciAddress    Pci address
 * @param[in] Data          Data to compare
 * @param[in] DataMask      AND mask
 * @param[in] Delay         Poll for time in 100ns (not supported)
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdPciPoll (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR PciAddress,
  IN       VOID *Data,
  IN       VOID *DataMask,
  IN       UINT64 Delay,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32  TempData;
  UINT32  TempMask;
  UINT32  Value;
  LibAmdGetDataFromPtr (AccessWidth, Data,  DataMask, &TempData, &TempMask);
  do {
    LibAmdPciRead (AccessWidth, PciAddress, &Value, StdHeader);
  } while (TempData != (Value & TempMask));
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get MMIO base address for PCI accesses
 *
 * @param[out] MmioAddress   PCI MMIO base address
 * @param[out] MmioSize      Size of region in bytes
 * @param[in]  StdHeader     Standard configuration header
 *
 * @retval    TRUE          MmioAddress/MmioSize are valid
 */
BOOLEAN
STATIC
GetPciMmioAddress (
     OUT   UINT64            *MmioAddress,
     OUT   UINT32            *MmioSize,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  BOOLEAN MmioIsEnabled;
  UINT32  EncodedSize;
  UINT64  LocalMsrRegister;

  ASSERT (StdHeader != NULL);

  MmioIsEnabled = FALSE;
  LibAmdMsrRead (MSR_MMIO_Cfg_Base, &LocalMsrRegister, StdHeader);
  if ((LocalMsrRegister & BIT0) != 0) {
    *MmioAddress = LocalMsrRegister & 0xFFFFFFFFFFF00000;
    EncodedSize = (UINT32) ((LocalMsrRegister & 0x3C) >> 2);
    *MmioSize = ((1 << EncodedSize) * 0x100000);
    MmioIsEnabled = TRUE;
  }
  return MmioIsEnabled;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read field of PCI config register.
 *
 *
 *
 * @param[in]   Address       Pci address  (register must be DWORD aligned)
 * @param[in]   Highbit       High bit position of the field in DWORD
 * @param[in]   Lowbit        Low bit position of the field in DWORD
 * @param[out]  Value         Pointer to data
 * @param[in]   StdHeader     Standard configuration header
 */
VOID
LibAmdPciReadBits (
  IN       PCI_ADDR Address,
  IN       UINT8 Highbit,
  IN       UINT8 Lowbit,
     OUT   UINT32 *Value,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  ASSERT (Highbit < 32 && Lowbit < 32 && Highbit >= Lowbit && (Address.AddressValue & 3) == 0);

  LibAmdPciRead (AccessWidth32, Address, Value, StdHeader);
  *Value >>= Lowbit;  // Shift

  // A 1 << 32 == 1 << 0 due to x86 SHL instruction, so skip if that is the case

  if ((Highbit - Lowbit) != 31) {
    *Value &= (((UINT32) 1 << (Highbit - Lowbit + 1)) - 1);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write field of PCI config register.
 *
 *
 *
 * @param[in]   Address       Pci address  (register must be DWORD aligned)
 * @param[in]   Highbit       High bit position of the field in DWORD
 * @param[in]   Lowbit        Low bit position of the field in DWORD
 * @param[in]   Value         Pointer to data
 * @param[in]   StdHeader     Standard configuration header
 */
VOID
LibAmdPciWriteBits (
  IN       PCI_ADDR Address,
  IN       UINT8 Highbit,
  IN       UINT8 Lowbit,
  IN       UINT32 *Value,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32 Temp;
  UINT32 Mask;

  ASSERT (Highbit < 32 && Lowbit < 32 && Highbit >= Lowbit && (Address.AddressValue & 3) == 0);

  // A 1<<32 == 1<<0 due to x86 SHL instruction, so skip if that is the case

  if ((Highbit - Lowbit) != 31) {
    Mask = (((UINT32) 1 << (Highbit - Lowbit + 1)) - 1);
  } else {
    Mask = (UINT32) 0xFFFFFFFF;
  }

  LibAmdPciRead (AccessWidth32, Address, &Temp, StdHeader);
  Temp &= ~(Mask << Lowbit);
  Temp |= (*Value & Mask) << Lowbit;
  LibAmdPciWrite (AccessWidth32, Address, &Temp, StdHeader);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Locate next capability pointer
 *
 *  Given a SBDFO this routine will find the next PCI capabilities list entry.
 *  if the end of the list is reached, or if a problem is detected, then ILLEGAL_SBDFO is
 *  returned.
 *  To start a new search from the head of the list, specify a SBDFO with an offset of zero.
 *
 * @param[in,out] Address       Pci address
 * @param[in]     StdHeader     Standard configuration header
 */

VOID
LibAmdPciFindNextCap (
  IN OUT   PCI_ADDR *Address,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  PCI_ADDR Base;
  UINT32 Offset;
  UINT32 Temp;
  PCI_ADDR TempAddress;

  ASSERT (Address != NULL);
  ASSERT (*(UINT32 *) Address != ILLEGAL_SBDFO);

  Base.AddressValue = Address->AddressValue;
  Offset = Base.Address.Register;
  Base.Address.Register = 0;

  Address->AddressValue = (UINT32) ILLEGAL_SBDFO;

  // Verify that the SBDFO points to a valid PCI device SANITY CHECK
  LibAmdPciRead (AccessWidth32, Base, &Temp, StdHeader);
  if (Temp == 0xFFFFFFFF) {
    ASSERT (FALSE);
    return; // There is no device at this address
  }

  // Verify that the device supports a capability list
  TempAddress.AddressValue = Base.AddressValue + 0x04;
  LibAmdPciReadBits (TempAddress, 20, 20, &Temp, StdHeader);
  if (Temp == 0) {
    return; // This PCI device does not support capability lists
  }

  if (Offset != 0) {
    // If we are continuing on an existing list
    TempAddress.AddressValue = Base.AddressValue + Offset;
    LibAmdPciReadBits (TempAddress, 15, 8, &Temp, StdHeader);
  } else {
    // We are starting on a new list
    TempAddress.AddressValue = Base.AddressValue + 0x34;
    LibAmdPciReadBits (TempAddress, 7, 0, &Temp, StdHeader);
  }

  if (Temp == 0) {
    return; // We have reached the end of the capabilities list
  }

  // Error detection and recovery- The statement below protects against
  //   PCI devices with broken PCI capabilities lists.  Detect a pointer
  //   that is not uint32 aligned, points into the first 64 reserved DWORDs
  //   or points back to itself.
  if (((Temp & 3) != 0) || (Temp == Offset) || (Temp < 0x40)) {
    ASSERT (FALSE);
    return;
  }

  Address->AddressValue = Base.AddressValue + Temp;
  return;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set memory with value
 *
 *
 * @param[in,out] Destination   Pointer to memory range
 * @param[in]     Value         Value to set memory with
 * @param[in]     FillLength    Size of the memory range
 * @param[in]     StdHeader     Standard configuration header (Optional)
 */
VOID
LibAmdMemFill (
  IN       VOID *Destination,
  IN       UINT8 Value,
  IN       UINTN FillLength,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8 *Dest;
  ASSERT (StdHeader != NULL);
  Dest = Destination;
  while ((FillLength--) != 0) {
    *Dest++ = Value;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Copy memory
 *
 *
 * @param[in,out] Destination   Pointer to destination buffer
 * @param[in]     Source        Pointer to source buffer
 * @param[in]     CopyLength    buffer length
 * @param[in]     StdHeader     Standard configuration header (Optional)
 */
VOID
LibAmdMemCopy (
  IN       VOID *Destination,
  IN       VOID *Source,
  IN       UINTN CopyLength,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8 *Dest;
  UINT8 *SourcePtr;
  ASSERT (StdHeader != NULL);
  Dest = Destination;
  SourcePtr = Source;
  while ((CopyLength--) != 0) {
    *Dest++ = *SourcePtr++;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Verify checksum of binary image (B1/B2/B3)
 *
 *
 * @param[in]   ImagePtr      Pointer to image  start
 * @retval      TRUE          Checksum valid
 * @retval      FALSE         Checksum invalid
 */
BOOLEAN
LibAmdVerifyImageChecksum (
  IN       VOID *ImagePtr
  )
{
  // Assume ImagePtr points to the binary start ($AMD)
  // Checksum is on an even boundary in AMD_IMAGE_HEADER

  UINT16 Sum;
  UINT32 i;

  Sum = 0;

  i = ((AMD_IMAGE_HEADER*) ImagePtr)->ImageSize;

  while (i > 1) {
    Sum = Sum + *((UINT16 *)ImagePtr);
    ImagePtr = (VOID *) ((UINT8 *)ImagePtr + 2);
    i = i - 2;
  }
  if (i > 0) {
    Sum = Sum + *((UINT8 *) ImagePtr);
  }

  return (Sum == 0)?TRUE:FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Locate AMD binary image that contain specific module
 *
 *
 * @param[in]   StartAddress    Pointer to start range
 * @param[in]   EndAddress      Pointer to end range
 * @param[in]   Alignment       Image address alignment
 * @param[in]   ModuleSignature Module signature.
 * @retval     NULL  if image not found
 * @retval     pointer to image header
 */
VOID *
LibAmdLocateImage (
  IN       VOID *StartAddress,
  IN       VOID *EndAddress,
  IN       UINT32 Alignment,
  IN       CHAR8 ModuleSignature[8]
  )

{
  UINT8 *CurrentPtr;
  AMD_MODULE_HEADER *ModuleHeaderPtr;
  UINT64 *SearchStr;
  UINT64 *InputStr;

  CurrentPtr = StartAddress;
  InputStr = (UINT64 *)ModuleSignature;

  // Search from start to end incrementing by alignment
  while ((CurrentPtr >= (UINT8 *) StartAddress) && (CurrentPtr < (UINT8 *) EndAddress)) {
    // First find a binary image
    if (*((UINT32 *) CurrentPtr) == IMAGE_SIGNATURE) {
      if (LibAmdVerifyImageChecksum (CurrentPtr)) {
        // If we have a valid image, search module linked list for a match
        ModuleHeaderPtr = (AMD_MODULE_HEADER*) ((UINT8 *)CurrentPtr + ((AMD_IMAGE_HEADER *) CurrentPtr)->ModuleInfoOffset);
        while (ModuleHeaderPtr != NULL) {
          SearchStr = (UINT64 *)&ModuleHeaderPtr->ModuleIdentifier;
          if (*InputStr == *SearchStr) {
            return  CurrentPtr;
          }
          ModuleHeaderPtr = (AMD_MODULE_HEADER *)ModuleHeaderPtr->NextBlock;
        }
      }
    }
    CurrentPtr += Alignment;
  }
  return NULL;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Returns the package type mask for the processor
 *
 *
 * @param[in]     StdHeader     Standard configuration header (Optional)
 */

//  Returns the package type mask for the processor
UINT32
LibAmdGetPackageType (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32      ProcessorPackageType;
  CPUID_DATA  CpuId;

  LibAmdCpuidRead (0x80000001, &CpuId, StdHeader);
  ProcessorPackageType = (UINT32) (CpuId.EBX_Reg >> 28) & 0xF; // bit 31:28
  return (UINT32) (1 << ProcessorPackageType);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Returns the package type mask for the processor
 *
 *
 * @param[in]     AccessWidth     Access width
 * @param[in]     Data            data
 * @param[in]     DataMask        data
 * @param[out]    TemData         typecast data
 * @param[out]    TempDataMask    typecast data
 */


VOID
STATIC
LibAmdGetDataFromPtr (
  IN       ACCESS_WIDTH AccessWidth,
  IN       VOID         *Data,
  IN       VOID         *DataMask,
     OUT   UINT32       *TemData,
     OUT   UINT32       *TempDataMask
  )
{
  switch (AccessWidth) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    *TemData = (UINT32)*(UINT8 *) Data;
    *TempDataMask = (UINT32)*(UINT8 *) DataMask;
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    *TemData = (UINT32)*(UINT16 *) Data;
    *TempDataMask = (UINT32)*(UINT16 *) DataMask;
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    *TemData = *(UINT32 *) Data;
    *TempDataMask = *(UINT32 *) DataMask;
    break;
  default:
    IDS_ERROR_TRAP;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Returns the package type mask for the processor
 *
 *
 * @param[in]     AccessWidth     Access width
 * @retval        Width in number of bytes
 */


UINT8
LibAmdAccessWidth (
  IN       ACCESS_WIDTH AccessWidth
  )
{
  UINT8 Width;

  switch (AccessWidth) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    Width = 1;
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    Width = 2;
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    Width = 4;
    break;
  case AccessWidth64:
  case AccessS3SaveWidth64:
    Width = 8;
    break;
  default:
    Width = 0;
    IDS_ERROR_TRAP;
  }
  return Width;
}
