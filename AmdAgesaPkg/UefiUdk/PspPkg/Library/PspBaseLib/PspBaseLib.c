/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP Base Library
 *
 * Contains interface to the PSP library
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 282908 $   @e \$Date: 2014-01-20 00:54:02 -0600 (Mon, 20 Jan 2014) $
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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PspBaseLib.h>

#define PSP_BAR1_TMP_BASE 0xFEA00000ul

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
     OUT   UINT32            *MmioSize
  );

VOID
STATIC
PspLibGetDataFromPtr (
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
 *
 */
VOID
PspLibIoRead (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT16 IoAddress,
     OUT   VOID *Value
  )
{
  switch (AccessWidth) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    *(UINT8 *) Value = PspLibReadIo8 (IoAddress);
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    *(UINT16 *) Value = PspLibReadIo16 (IoAddress);
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    *(UINT32 *) Value = PspLibReadIo32 (IoAddress);
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
 *
 */
VOID
PspLibIoWrite (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT16 IoAddress,
  IN       VOID *Value
  )
{
  switch (AccessWidth) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    PspLibWriteIo8 (IoAddress, *(UINT8 *) Value);
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    PspLibWriteIo16 (IoAddress, *(UINT16 *) Value);
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    PspLibWriteIo32 (IoAddress, *(UINT32 *) Value);
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
 *
 */
VOID
PspLibIoRMW (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT16 IoAddress,
  IN       VOID *Data,
  IN       VOID *DataMask
  )
{
  UINT32  TempData;
  UINT32  TempMask;
  UINT32  Value;
  PspLibGetDataFromPtr (AccessWidth, Data,  DataMask, &TempData, &TempMask);
  PspLibIoRead (AccessWidth, IoAddress, &Value);
  Value = (Value & (~TempMask)) | TempData;
  PspLibIoWrite (AccessWidth, IoAddress, &Value);
}
/*---------------------------------------------------------------------------------------*/
/**
 * Read memory/MMIO
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] MemAddress    Memory address
 * @param[in] Value         Pointer to data
 *
 */
VOID
PspLibMemRead (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT64 MemAddress,
     OUT   VOID *Value
  )
{
  switch (AccessWidth) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    *(UINT8 *) Value = *((UINT8 *) (UINTN) MemAddress);
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    *(UINT16 *) Value = *((UINT16 *) (UINTN) MemAddress);
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    *(UINT32 *) Value = *((UINT32 *) (UINTN) MemAddress);
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
 *
 */
VOID
PspLibMemWrite (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT64 MemAddress,
  IN       VOID *Value
  )
{
  switch (AccessWidth) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    *((UINT8 *) (UINTN) MemAddress) =  *((UINT8 *) Value);
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    *((UINT16 *) (UINTN) MemAddress) =  *((UINT16 *) Value);
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    *((UINT32 *) (UINTN) MemAddress) =  *((UINT32 *) Value);
    break;
  default:
    ASSERT (FALSE);
  }
}
/*---------------------------------------------------------------------------------------*/
/**
 * Read PCI config space
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] PciAddress    Pci address
 * @param[in] Value         Pointer to data
 *
 */
VOID
PspLibPciRead (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR PciAddress,
     OUT   VOID *Value
  )
{
  UINT32 LegacyPciAccess;
  UINT32 MMIOSize;
  UINT64 RMWrite;
  UINT64 RMWritePrevious;
  UINT64 MMIOAddress;

  ASSERT (PciAddress.AddressValue != ILLEGAL_SBDFO);
  if (!GetPciMmioAddress (&MMIOAddress, &MMIOSize)) {
    // We need to convert our "portable" PCI address into a "real" PCI access
    LegacyPciAccess = ((1 << 31) + (PciAddress.Address.Register & 0xFC) + (PciAddress.Address.Function << 8) + (PciAddress.Address.Device << 11) + (PciAddress.Address.Bus << 16) + ((PciAddress.Address.Register & 0xF00) << (24 - 8)));
    if (PciAddress.Address.Register <= 0xFF) {
      PspLibIoWrite (AccessWidth32, IOCF8, &LegacyPciAccess);
      PspLibIoRead (AccessWidth, IOCFC + (UINT16) (PciAddress.Address.Register & 0x3), Value);
    } else {
      PspLibMsrRead  (NB_CFG, &RMWritePrevious);
      RMWrite = RMWritePrevious | 0x0000400000000000;
      PspLibMsrWrite (NB_CFG, &RMWrite);
      PspLibIoWrite (AccessWidth32, IOCF8, &LegacyPciAccess);
      PspLibIoRead (AccessWidth, IOCFC + (UINT16) (PciAddress.Address.Register & 0x3), Value);
      PspLibMsrWrite (NB_CFG, &RMWritePrevious);
    }
  } else {
    // Setup the MMIO address
    ASSERT ((MMIOAddress + MMIOSize) > (MMIOAddress + (PciAddress.AddressValue & 0x0FFFFFFF)));
    MMIOAddress += (PciAddress.AddressValue & 0x0FFFFFFF);
    PspLibMemRead (AccessWidth, MMIOAddress, Value);
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
 *
 */
VOID
PspLibPciWrite (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR PciAddress,
  IN       VOID *Value
  )
{
  UINT32 LegacyPciAccess;
  UINT32 MMIOSize;
  UINT64 RMWrite;
  UINT64 RMWritePrevious;
  UINT64 MMIOAddress;

  ASSERT (PciAddress.AddressValue != ILLEGAL_SBDFO);
  if (!GetPciMmioAddress (&MMIOAddress, &MMIOSize)) {
    // We need to convert our "portable" PCI address into a "real" PCI access
    LegacyPciAccess = ((1 << 31) + (PciAddress.Address.Register & 0xFC) + (PciAddress.Address.Function << 8) + (PciAddress.Address.Device << 11) + (PciAddress.Address.Bus << 16) + ((PciAddress.Address.Register & 0xF00) << (24 - 8)));
    if (PciAddress.Address.Register <= 0xFF) {
      PspLibIoWrite (AccessWidth32, IOCF8, &LegacyPciAccess);
      PspLibIoWrite (AccessWidth, IOCFC + (UINT16) (PciAddress.Address.Register & 0x3), Value);
    } else {
      PspLibMsrRead  (NB_CFG, &RMWritePrevious);
      RMWrite = RMWritePrevious | 0x0000400000000000;
      PspLibMsrWrite (NB_CFG, &RMWrite);
      PspLibIoWrite (AccessWidth32, IOCF8, &LegacyPciAccess);
      PspLibIoWrite (AccessWidth, IOCFC + (UINT16) (PciAddress.Address.Register & 0x3), Value);
      PspLibMsrWrite (NB_CFG, &RMWritePrevious);
    }
  } else {
    // Setup the MMIO address
    ASSERT ((MMIOAddress + MMIOSize) > (MMIOAddress + (PciAddress.AddressValue & 0x0FFFFFFF)));
    MMIOAddress += (PciAddress.AddressValue & 0x0FFFFFFF);
    PspLibMemWrite (AccessWidth, MMIOAddress, Value);
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
 *
 */
VOID
PspLibPciRMW (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR PciAddress,
  IN       VOID *Data,
  IN       VOID *DataMask
  )
{
  UINT32  TempData;
  UINT32  TempMask;
  UINT32  Value;
  PspLibGetDataFromPtr (AccessWidth, Data,  DataMask, &TempData, &TempMask);
  PspLibPciRead (AccessWidth, PciAddress, &Value);
  Value = (Value & (~TempMask)) | TempData;
  PspLibPciWrite (AccessWidth, PciAddress, &Value);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get MMIO base address for PCI accesses
 *
 * @param[out] MmioAddress   PCI MMIO base address
 * @param[out] MmioSize      Size of region in bytes
 *
 * @retval    TRUE          MmioAddress/MmioSize are valid
 */
BOOLEAN
STATIC
GetPciMmioAddress (
  OUT   UINT64            *MmioAddress,
  OUT   UINT32            *MmioSize
  )
{
  BOOLEAN MmioIsEnabled;
  UINT32  EncodedSize;
  UINT64  LocalMsrRegister;

  MmioIsEnabled = FALSE;
  PspLibMsrRead (MSR_MMIO_Cfg_Base, &LocalMsrRegister);
  if ((LocalMsrRegister & BIT0) != 0) {
    *MmioAddress = LocalMsrRegister & 0xFFFFFFFFFFF00000;
    EncodedSize = (UINT32) (RShiftU64 ((LocalMsrRegister & 0x3C), 2));
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
 */
VOID
PspLibPciReadBits (
  IN       PCI_ADDR Address,
  IN       UINT8 Highbit,
  IN       UINT8 Lowbit,
     OUT   UINT32 *Value
  )
{
  ASSERT (Highbit < 32 && Lowbit < 32 && Highbit >= Lowbit && (Address.AddressValue & 3) == 0);

  PspLibPciRead (AccessWidth32, Address, Value);
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
 */
VOID
PspLibPciWriteBits (
  IN       PCI_ADDR Address,
  IN       UINT8 Highbit,
  IN       UINT8 Lowbit,
  IN       UINT32 *Value
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

  PspLibPciRead (AccessWidth32, Address, &Temp);
  Temp &= ~(Mask << Lowbit);
  Temp |= (*Value & Mask) << Lowbit;
  PspLibPciWrite (AccessWidth32, Address, &Temp);
}

UINT32
PspLibPciReadConfig (
  IN  UINT32 Register
  )
{
  UINT32 Value;
  PCI_ADDR PciAddr;

  Value = 0;
  PciAddr.AddressValue = MAKE_SBDFO (0, GET_PCI_BUS (Register), GET_PCI_DEV (Register), GET_PCI_FUNC (Register), GET_PCI_OFFSET (Register));

  PspLibPciRead (AccessWidth32, PciAddr, &Value);

  return Value;
}

VOID
PspLibPciWriteConfig (
  IN  UINT32 Register,
  IN  UINT32 Value
  )
{
  PCI_ADDR PciAddr;
  PciAddr.AddressValue = MAKE_SBDFO (0, GET_PCI_BUS (Register), GET_PCI_DEV (Register), GET_PCI_FUNC (Register), GET_PCI_OFFSET (Register));

  PspLibPciWrite (AccessWidth32, PciAddr, &Value);

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
PspLibGetDataFromPtr (
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
    ASSERT (FALSE);
  }
}


EFI_STATUS
GetPspDirBase (
  IN OUT   EFI_PHYSICAL_ADDRESS     *Address
  )
{
  UINTN                     i;
  FIRMWARE_ENTRY_TABLE      *FirmwareTableBase;
  CONST UINTN RomSigAddrTable[] =
  {
    0xFFFA0000, //  --> 512KB base
    0xFFF20000, //  --> 1MB base
    0xFFE20000, //  --> 2MB base
    0xFFC20000, //  --> 4MB base
    0xFF820000, //  --> 8MB base
    0xFF020000  //  --> 16MB base
  };

  for (i = 0; i < sizeof (RomSigAddrTable) / sizeof (UINTN); i++) {
    FirmwareTableBase  = (FIRMWARE_ENTRY_TABLE *) (UINTN) RomSigAddrTable[i];
    // Search flash for unique signature 0x55AA55AA
    if (FirmwareTableBase->Signature  == FIRMWARE_TABLE_SIGNATURE) {
      *Address = (EFI_PHYSICAL_ADDRESS)(FirmwareTableBase->PspDirBase);
      return EFI_SUCCESS;
    }
  }

  return (EFI_NOT_FOUND);
}

EFI_STATUS
PSPEntryInfo (
  IN       PSP_DIRECTORY_ENTRY_TYPE    EntryType,
  IN OUT   EFI_PHYSICAL_ADDRESS        *EntryAddress,
  IN       UINTN                       *EntrySize
  )
{
  PSP_DIRECTORY         *PspDir;
  UINTN                 i;

  if (EFI_ERROR (GetPspDirBase ((EFI_PHYSICAL_ADDRESS *)&PspDir ))) {
    return EFI_NOT_FOUND;
  }

  for (i = 0; i < PspDir->Header.TotalEntries; i++) {
    if (PspDir->PspEntry[i].Type == EntryType) {
      *EntryAddress = (EFI_PHYSICAL_ADDRESS) PspDir->PspEntry[i].Location;
      *EntrySize = PspDir->PspEntry[i].Size;
      return (EFI_SUCCESS);
    }
  }

  return (EFI_NOT_FOUND);
}

/**
  Check if PSP device is present

  @retval EFI_STATUS  0: Success, NonZero Error

**/
EFI_STATUS
CheckPspDevicePresent (
  VOID
  )
{
  UINT32 SecureFuseReg;
  //PROTO = 1, FRA = 0, PCPU_DIS = 0
  PspLibPciWriteConfig ( (UINT32)PCI_CONFIG_SMU_INDIRECT_INDEX, (UINT32)SMU_CC_PSP_FUSES_STATUS);
  SecureFuseReg = PspLibPciReadConfig ( (UINT32)PCI_CONFIG_SMU_INDIRECT_DATA);
  SecureFuseReg &= ( SMU_CC_PSP_FUSES_SECURE +
                     SMU_CC_PSP_FUSES_FRA_ENABLE +
                     SMU_CC_PSP_FUSES_PROTO +
                     SMU_CC_PSP_FUSES_PCPU_DIS);

  if ((SecureFuseReg == PSP_SECURE_PART) || (SecureFuseReg == PSP_PROTO_PART)) {
    return (EFI_SUCCESS);
  }
  return (EFI_UNSUPPORTED);
}

UINT32
PspLibPciReadPspConfig (
  IN UINT32 Offset
  )
{
  return (PspLibPciReadConfig  ((UINT32) (PSP_PCIE_BDA  + Offset)));
}

VOID
PspLibPciWritePspConfig (
  IN  UINT32 Offset,
  IN  UINT32 Value
  )
{
  PspLibPciWriteConfig (PSP_PCIE_BDA  + Offset, Value);
}

EFI_STATUS
EFIAPI
PspBarInitEarly ()
{
  UINT32 PspMmioSize;

  if (PspLibPciReadConfig  ( (UINT32) (PSP_PCIE_BDA)) == 0xffffffff) {
    return (EFI_NOT_FOUND);
  }
  /// Get PSP BAR1 Size
  PspLibPciWriteConfig ( (UINT32) (PSP_PCIE_BDA + PSP_PCIE_BAR1_REG), 0xFFFFFFFF);
  PspMmioSize = PspLibPciReadConfig ( (UINT32) (PSP_PCIE_BDA + PSP_PCIE_BAR1_REG));
  PspMmioSize = ~PspMmioSize + 1;
  /// Assign BAR1 Temporary Address
  PspLibPciWriteConfig ( (UINT32) (PSP_PCIE_BDA + PSP_PCIE_BAR1_REG), PSP_BAR1_TMP_BASE);
  PspLibPciWriteConfig ( (UINT32) (PSP_PCIE_BDA + 0x48), 0x4);
  PspLibPciWriteConfig ( (UINT32) (PSP_PCIE_BDA + PSP_PCIE_CMD_REG), 0x06);

  /// Enable GNB redirection to this space @todo use equate & also find proper fix
  PspLibPciWriteConfig ( ( (0x18 << 11) + (1 << 8) + 0xBC), ((PSP_BAR1_TMP_BASE + PspMmioSize -1) >> 8) & ~0xFF);
  PspLibPciWriteConfig ( ( (0x18 << 11) + (1 << 8) + 0xB8), (PSP_BAR1_TMP_BASE >> 8) | 3);
  /// Enable MsixBarEn, Bar2En, Bar1En, Bar0En
  /// Enable MsixBarEn, Bar1En, Bar3En
  PspLibPciWriteConfig ( (UINT32) (PSP_PCIE_BDA + 0x48), 0x34);

  return (EFI_SUCCESS);
}

/**
  Return the PspMMIO MMIO location

  @param[in] PspMmio Pointer to Psp MMIO address

  @retval EFI_STATUS  0: Success, NonZero Error
**/
EFI_STATUS
GetPspBar1Addr (
  IN OUT   EFI_PHYSICAL_ADDRESS *PspMmio
  )
{
  if (CheckPspDevicePresent () != EFI_SUCCESS) {
    return (EFI_UNSUPPORTED);
  }

  *PspMmio = (EFI_PHYSICAL_ADDRESS) PspLibPciReadPspConfig (PSP_PCIE_BAR1_REG);

  if (*PspMmio == 0xffffffff) {
    return (EFI_NOT_FOUND);
  }

  return (EFI_SUCCESS);
}

