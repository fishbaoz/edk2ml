/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH DXE Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH ACPI Driver
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*****************************************************************************
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
#include "FchAcpiCs.h"
#include "FchIntDev.h"
#include "Filecode.h"
#define FILECODE UEFI_FCH_AVALON_DXE_FCHACPICS_FCHACPICSCONTROL_FILECODE

EFI_STATUS
EFIAPI
AmdFchGetIntDevPath (
  IN       FCH_ACPI_PROTOCOL    *FchAcpi,
  IN       UINT32               IntDevAddress,
  IN OUT   UINT32               *PathName
  )
{
  UINT8                         Index;
  EFI_STATUS                    Status;

  Status = AmdFchGetSpbDevPath (FchAcpi, IntDevAddress, PathName);
  if (!EFI_ERROR (Status)) {
    return Status;
  }

  for (Index = 0; Index < NumOfFchIntDev; Index++) {
    if (mFchIntDev[Index].AcpiAddress == IntDevAddress) {
      *PathName = mFchIntDev[Index].AcpiName;
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
AmdFchGetSpbDevPath (
  IN       FCH_ACPI_PROTOCOL    *FchAcpi,
  IN       UINT32               IntDevAddress,
  IN OUT   UINT32               *PathName
  )
{

  switch (IntDevAddress) {
  case FCH_ACPI_DEVICE_ASF_SMBUS:
    *PathName = SIGNATURE_32 ('S', 'M', 'B', '1');
    return EFI_SUCCESS;

  case FCH_ACPI_DEVICE_GPIO:
    *PathName = SIGNATURE_32 ('G', 'P', 'I', 'O');
    return EFI_SUCCESS;

  default:
    return EFI_NOT_FOUND;
  }
}

EFI_STATUS
EFIAPI
CheckDataBuffer (
  IN       VOID                 *PathName
  )
{
  UINT8         Index;
  UINT8         *Value;

  Value = (UINT8*)PathName;
  for (Index = 0; Index < AMD_FCH_INTERNAM_DEVICE_NAMEPATH_LENGTH; Index++) {
    if (Value[Index] != 0) {
      return EFI_INVALID_PARAMETER;
    }
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AmdBuildAcpiDsdtTree (
  IN       EFI_ACPI_DESCRIPTION_HEADER      *Table,
  IN OUT   VOID                             **AcpiDsdtTree
  )
{
  UINT8                                     *DataStart;
  UINT32                                    TableDataSize;
  EFI_STATUS                                Status;
//  AMD_ACPI_NAMESPACE_LOCATION               *ScopeNameList;

  Status = EFI_SUCCESS;

  TableDataSize  = Table->Length;
  TableDataSize  = (UINT32) ((UINTN)TableDataSize - sizeof (EFI_ACPI_DESCRIPTION_HEADER));

  DataStart      = (UINT8 *)Table;
  DataStart      = (UINT8*) ((UINTN)DataStart + sizeof (EFI_ACPI_DESCRIPTION_HEADER));

//  Status = AmdGetScopeList (DataStart, TableDataSize, &ScopeNameList);
  Status = FchBuildInternalDeviceList (DataStart, TableDataSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return Status;
}

EFI_STATUS
EFIAPI
FchBuildInternalDeviceList (
  IN UINT8                                  *AmlStart,
  IN UINT32                                 AmlSize
  )
{
  UINT8                                     Index;
  UINT8                                     ByteCount;
  UINT8                                     *CurrentAdrAml;
  UINT8                                     *CurrentDevAml;
  UINT8                                     *CurrentPci0Aml;
  UINT32                                    Size;
  UINT32                                    Pci0Length;
  UINT32                                    FchLpcAddr;
  UINT32                                    AcpiName;
  EFI_STATUS                                Status;

  FchLpcAddr = 0x00140003;
  CurrentPci0Aml = NULL;
  Pci0Length = 0;
  CurrentAdrAml = AmlStart;
  Status = EFI_SUCCESS;

  while ((UINTN)CurrentAdrAml < ((UINTN)AmlStart + (UINTN)AmlSize)) {
    Size = (UINT32) ((UINTN)AmlStart + (UINTN)AmlSize - (UINTN)CurrentAdrAml);
    Status = FchFindNextAdr32 (&CurrentAdrAml, Size, FchLpcAddr);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    CurrentDevAml = CurrentAdrAml;
    Status = FchFindParentDev (AmlStart, &CurrentDevAml, &AcpiName);
    if (EFI_ERROR (Status)) {
      continue;
    }
    mFchIntDev[2].AcpiName = AcpiName;

    CurrentPci0Aml = CurrentDevAml;
    Status = FchFindPci0NameFromIntDev (AmlStart, &CurrentPci0Aml, &AcpiName);
    if (EFI_ERROR (Status)) {
      continue;
    }
    mFchIntDev[0].AcpiName = AcpiName;
    FchAcpiPkgLength ((UINT8*) ((UINTN)CurrentPci0Aml + 2), &ByteCount, &Pci0Length);

    if (Status == EFI_SUCCESS) {
      break;
    }
  }

  if (Status == EFI_SUCCESS) {
    for (Index = 1; Index < NumOfFchIntDev; Index++) {
      if (Index == 0x02) {
        continue;
      }

      CurrentAdrAml = CurrentPci0Aml;
      while ((UINTN)CurrentAdrAml < ((UINTN)CurrentPci0Aml + (UINTN)Pci0Length)) {
        Size = (UINT32) ((UINTN)CurrentPci0Aml + (UINTN)Pci0Length - (UINTN)CurrentAdrAml);
        Status = FchFindNextAdr32 (&CurrentAdrAml, Size, mFchIntDev[Index].AcpiAddress);
        if (EFI_ERROR (Status)) {
          break;
        }

        CurrentDevAml = CurrentAdrAml;
        Status = FchFindParentDev (CurrentPci0Aml, &CurrentDevAml, &AcpiName);
        if (EFI_ERROR (Status)) {
          continue;
        }
        mFchIntDev[Index].AcpiName = AcpiName;
        if (Status == EFI_SUCCESS) {
          break;
        }
      }
    }
    return EFI_SUCCESS;
  }
  return EFI_UNSUPPORTED;
}

EFI_STATUS
UpdateAcpiName (
  IN       VOID                                *AcpiTable,
  IN       VOID                                *OldName,
  IN       VOID                                *NewName,
  IN       UINT8                               NameLength
  )
{
  UINT8                         *CurrentAml;
  EFI_ACPI_DESCRIPTION_HEADER   *Table;

  Table = (EFI_ACPI_DESCRIPTION_HEADER*)AcpiTable;

  for (CurrentAml = (UINT8 *)Table;
       (UINTN)CurrentAml <= ((UINTN)Table + (UINTN)(Table->Length) - (UINTN)NameLength);
       CurrentAml++
       ) {
    if (CheckData (CurrentAml, OldName, NameLength)) {
      CopyMem (CurrentAml, NewName, NameLength);
    }
  }
  return EFI_SUCCESS;
}

BOOLEAN
CheckData (
  IN       VOID                       *Ptr1,
  IN       VOID                       *Ptr2,
  IN       UINT8                      Len
  )
{
  UINT8                   Index;
  UINT8                   *Data1;
  UINT8                   *Data2;

  Data1 = (UINT8*)Ptr1;
  Data2 = (UINT8*)Ptr2;
  for (Index = 0; Index < Len; Index++) {
    if (Data1[Index] != Data2[Index]) {
      return FALSE;
    }
  }
  return TRUE;
}

EFI_STATUS
EFIAPI
AmdFchDataDxeAddItem (
  IN       FCH_ACPI_PROTOCOL                  *FchAcpiProtocol,
  IN       UINT8                               Length,
  IN       VOID                                *Data,
  IN       UINT32                              AcpiName,
     OUT   UINT32                              *DataOffset
  )
{
  EFI_STATUS                                Status;
  UINT8                                     *TempBuffer;
  UINT8                                     *DataByte;
  UINT32                                    NewDataLength;
  UINT32                                    Offset;
  UINT32                                    NewAcpiInfoLength;
  FCH_ACPI_FIELD_NAME                       *NewAcpiField;
  FCH_ACPI_DATA_INTERFACE                   FchAcpiData;

  Status = EFI_SUCCESS;

  FchAcpiData = FchAcpiProtocol->FchAcpiData;
  Offset = FchAcpiData.DataLength;
  NewDataLength = Offset + Length;
  NewAcpiInfoLength = FchAcpiData.AcpiInfoLength + sizeof (FCH_ACPI_FIELD_NAME);

  Status = gBS->AllocatePool (
                  EfiReservedMemoryType,
                  NewDataLength,
                  &TempBuffer
                  );

  ASSERT_EFI_ERROR (Status);

  ZeroMem (TempBuffer, NewDataLength);
  CopyMem (TempBuffer, FchAcpiData.FchData, FchAcpiData.DataLength);
  CopyMem ((UINT8 *) ((UINTN)TempBuffer + (UINTN)Offset), Data, Length);
  gBS->FreePool (FchAcpiData.FchData);
  FchAcpiProtocol->FchAcpiData.FchData    = TempBuffer;
  FchAcpiProtocol->FchAcpiData.DataLength = NewDataLength;

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  NewAcpiInfoLength,
                  &TempBuffer
                  );

  ASSERT_EFI_ERROR (Status);

  ZeroMem (TempBuffer, NewAcpiInfoLength);
  CopyMem (TempBuffer, FchAcpiData.AcpiFieldNames, FchAcpiData.AcpiInfoLength);

  NewAcpiField = (FCH_ACPI_FIELD_NAME *) ((UINTN)TempBuffer + (UINTN) (FchAcpiData.AcpiInfoLength) - 1);
  NewAcpiField->FieldLength = Length;
  NewAcpiField->Offset      = Offset;
  NewAcpiField->AcpiName    = AcpiName;
  DataByte  = (UINT8 *) ((UINTN)TempBuffer + (UINTN)NewAcpiInfoLength - 1);
  *DataByte = FCH_ACPI_FIELD_INFO_END;
  gBS->FreePool (FchAcpiData.AcpiFieldNames);
  FchAcpiProtocol->FchAcpiData.AcpiFieldNames = TempBuffer;
  FchAcpiProtocol->FchAcpiData.AcpiInfoLength = NewAcpiInfoLength;

  *DataOffset = Offset;

  return Status;
}

UINT32
LGpePinToString (
  IN       UINT32                           PinNumber
  )
{
  UINT32   PinString;

  PinNumber &= 0xFF;
  PinString = (0x4C5F + (((PinNumber >> 4) + 0x30) << 0x10) + (((PinNumber & 0xF) + 0x30) << 0x18));
  return PinString;
}

EFI_STATUS
FchAcpiUpdateGpePin (
  IN       FCH_ACPI_PROTOCOL                *FchAcpiProtocol,
  IN       EFI_ACPI_DESCRIPTION_HEADER      *AcpiTable,
  IN       UINT32                           CurrentPin,
  IN       UINT32                           NewPin
  )
{
  UINT8                                     *CurrentAml;
  UINT32                                    TblLength;
  UINT32                                    Signature;
  UINT32                                    LPinOld;
  UINT32                                    LPinNew;
  EFI_STATUS                                Status;

  if (AcpiTable != NULL) {
    Signature = SIGNATURE_32 ('_', 'G', 'P', 'E');
    LPinOld = LGpePinToString (CurrentPin);
    LPinNew = LGpePinToString (NewPin);
    TblLength = AcpiTable->Length;
    CurrentAml = (UINT8 *) ((UINTN)AcpiTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER));
    Status = FchFindNextSig (&CurrentAml, TblLength - sizeof (EFI_ACPI_DESCRIPTION_HEADER), Signature);
    if (EFI_ERROR (Status)) {
      return Status;
    }

  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AmdFchAcpiEnableGpio (
  IN       FCH_ACPI_PROTOCOL                *FchAcpiProtocol,
  IN       BOOLEAN                          Enable
  )
{
  EFI_STATUS                                Status;
  AMD_FCH_COMMON_DATA                       *FchCommanData;
  FCH_ACPI_DATA_INTERFACE                   FchAcpiData;

  Status = EFI_SUCCESS;

  FchAcpiData = FchAcpiProtocol->FchAcpiData;
  FchCommanData = (AMD_FCH_COMMON_DATA *) (FchAcpiData.FchData);
  if (Enable) {
    FchCommanData->FchGpioAcpiEnable = 1;
  } else {
    FchCommanData->FchGpioAcpiEnable = 0;
  }

  return Status;
}

EFI_STATUS
EFIAPI
AmdFchAcpiEnableAsf (
  IN       FCH_ACPI_PROTOCOL                *FchAcpiProtocol,
  IN       BOOLEAN                          Enable
  )
{
  EFI_STATUS                                Status;
  AMD_FCH_COMMON_DATA                       *FchCommanData;
  FCH_ACPI_DATA_INTERFACE                   FchAcpiData;

  Status = EFI_SUCCESS;

  FchAcpiData = FchAcpiProtocol->FchAcpiData;
  FchCommanData = (AMD_FCH_COMMON_DATA *) (FchAcpiData.FchData);
  if (Enable) {
    FchCommanData->FchAsfAcpiEnable = 1;
  } else {
    FchCommanData->FchAsfAcpiEnable = 0;
  }

  return Status;
}

EFI_STATUS
FchUpdateAcpiDataTable (
  IN       FCH_ACPI_PROTOCOL                *FchAcpiProtocol,
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER      **AcpiTable
  )
{
  EFI_STATUS                                Status;
  EFI_ACPI_DESCRIPTION_HEADER               *CurrentTable;
  UINT8                                     *FchAcpiUpdatePtr;
  UINT8                                     *NextBlockPtr;
  UINT8                                     *NewTable;
  UINT8                                     *NewField;
  UINT32                                    Size;
  UINT32                                    NewFieldSize;
  UINT32                                    NewSize;
  UINT32                                    Offset;
  UINT32                                    CurrentFieldLength;
  UINT32                                    *Signature;
  FCH_ACPI_DATA_INTERFACE                   FchAcpiData;

  Offset = 0;
  CurrentFieldLength = 0;
  CurrentTable = *AcpiTable;
  FchAcpiData  = FchAcpiProtocol->FchAcpiData;

  for (FchAcpiUpdatePtr = (UINT8 *) ((UINTN)CurrentTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER));
       (UINTN)FchAcpiUpdatePtr <= ((UINTN)CurrentTable + (UINTN) (CurrentTable->Length));
       FchAcpiUpdatePtr++
       ) {
    Signature = (UINT32*) FchAcpiUpdatePtr;
    switch (*Signature) {

    case (SIGNATURE_32 ('F', 'C', 'H', 'D')):
      if (FchIsAmlOpRegionObject (FchAcpiUpdatePtr)) {
        //get pointer to field
        FchAcpiUpdatePtr += 0x0A;
        switch (*FchAcpiUpdatePtr) {
        case 0x00:
        case 0x01:
        case 0xFF:
          FchAcpiUpdatePtr++;
          break;

        case 0x0A:
          FchAcpiUpdatePtr += 0x02;
          break;

        case 0x0B:
          FchAcpiUpdatePtr += 0x03;
          break;

        case 0x0C:
          FchAcpiUpdatePtr += 0x05;
          break;
        }
        Status = FchGetFieldOffset (FchAcpiUpdatePtr, &CurrentFieldLength, &Offset);
        NextBlockPtr = (UINT8 *) ((UINTN)FchAcpiUpdatePtr + (UINTN)CurrentFieldLength);
        FchUpdateAcpiField (FchAcpiData.AcpiFieldNames, FchAcpiData.AcpiInfoLength, FchAcpiUpdatePtr, &NewField, &NewFieldSize);
        NewSize = CurrentTable->Length - CurrentFieldLength + NewFieldSize;

        //adjust size for possible scope pkglength change
        NewSize = FchAdjustFieldLength (NewSize - sizeof (EFI_ACPI_DESCRIPTION_HEADER) - 1, \
                                        CurrentTable->Length - sizeof (EFI_ACPI_DESCRIPTION_HEADER) - 1)\
                    + sizeof (EFI_ACPI_DESCRIPTION_HEADER) + 1;

        //check data size for possible change of table size
        if (FchAcpiData.DataLength > 0xFFFF) {
          NewSize += 0x3;
        } else if (FchAcpiData.DataLength > 0xFF) {
          NewSize ++;
        }

        Status = gBS->AllocatePool (
                        EfiBootServicesData,
                        NewSize,
                        &NewTable
                        );

        ASSERT_EFI_ERROR (Status);

        CopyMem (NewTable, CurrentTable, sizeof (EFI_ACPI_DESCRIPTION_HEADER));
        FchAcpiUpdatePtr = (UINT8 *) ((UINTN)NewTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER));

        //update Scope & Operation Region
        FchUpdateAcpiDataScope ((UINT8*) ((UINTN)CurrentTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER)),
                                 &FchAcpiUpdatePtr,
                                 NewSize - sizeof (EFI_ACPI_DESCRIPTION_HEADER) - 1,
                                 FchAcpiData.FchData,
                                 FchAcpiData.DataLength
                                 );

        CopyMem (FchAcpiUpdatePtr, NewField, NewFieldSize);

        FchAcpiUpdatePtr = (UINT8 *) ((UINTN)FchAcpiUpdatePtr + (UINTN)NewFieldSize);
        Size = (UINT32) ((UINTN)CurrentTable + (UINTN) (CurrentTable->Length) - (UINTN)NextBlockPtr);
        CopyMem (FchAcpiUpdatePtr, NextBlockPtr, Size);

        gBS->FreePool (CurrentTable);
        gBS->FreePool (NewField);
        CurrentTable = (EFI_ACPI_DESCRIPTION_HEADER*)NewTable;
        CurrentTable->Length = NewSize;
        *AcpiTable = CurrentTable;
      }
      break;

    default:
      break;
    }
  }

  CurrentTable = *AcpiTable;
  Status = FchUpdateAcpiGpioController (FchAcpiProtocol, CurrentTable);

  CurrentTable = *AcpiTable;
  Status = FchUpdateAcpiAsf (FchAcpiProtocol, CurrentTable);

  return Status;
}

EFI_STATUS
FchUpdateAcpiGpioController (
  IN       FCH_ACPI_PROTOCOL                *FchAcpiProtocol,
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER      *AcpiTable
  )
{
  UINT8                                     *CurrentPtr;
  UINT8                                     *GpioDevPtr;
  UINT8                                     ByteCount;
  UINT8                                     GpioIrq;
  UINT32                                    DevLength;
  UINT32                                    GpioDevName;
  UINT32                                    ResourceName;
  EFI_STATUS                                Status;

  Status = EFI_SUCCESS;
  GpioDevName = SIGNATURE_32 ('G', 'P', 'I', 'O');
  ResourceName = SIGNATURE_32 ('R', 'B', 'U', 'F');
  GpioIrq = (UINT8) (mFchCommonData->SciInterrupt);
  LibFchIndirectIoWrite (EfiPciWidthUint8, FCH_IOMAP_REGC00, FCH_IRQ_GPIO | FCH_IRQ_IOAPIC, &GpioIrq);

  CurrentPtr = (UINT8 *)AcpiTable;
  Status = FchFindNextDevice (&CurrentPtr, AcpiTable->Length, GpioDevName);
  GpioDevPtr = CurrentPtr;

  FchAcpiPkgLength ((UINT8*) (CurrentPtr + 2), &ByteCount, &DevLength);
  Status = FchFindNextSig (&CurrentPtr, DevLength + 2, ResourceName);

  CurrentPtr += 13;  //4+4+5
  *CurrentPtr = GpioIrq;

  return Status;
}

UINT8
FchGetAsfIrq (
  VOID
  )
{
  UINT8  AsfIrq;

  LibFchIndirectIoRead (EfiPciWidthUint8, FCH_IOMAP_REGC00, FCH_IRQ_ASF | FCH_IRQ_IOAPIC, &AsfIrq);

  return AsfIrq;
}

UINT16
FchGetAsfIoBase (
  VOID
  )
{
  UINT16  AsfIoBase;

  LibFchIndirectIoRead (EfiPciWidthUint16, FCH_IOMAP_REGCD6, FCH_PMIOA_REG28, &AsfIoBase);
  AsfIoBase |= BIT0;
  LibFchIndirectIoWrite (EfiPciWidthUint16, FCH_IOMAP_REGCD6, FCH_PMIOA_REG28, &AsfIoBase);

  AsfIoBase &= 0xFFE0; //BIT 15:5

  return AsfIoBase;
}

EFI_STATUS
FchUpdateAcpiAsf (
  IN       FCH_ACPI_PROTOCOL                *FchAcpiProtocol,
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER      *AcpiTable
  )
{
  UINT8                                     *CurrentPtr;
  UINT8                                     *AsfDevPtr;
  UINT8                                     ByteCount;
  UINT8                                     AsfIrq;
  UINT16                                    *WordPtr;
  UINT16                                    AsfBase;
  UINT32                                    DevLength;
  UINT32                                    AsfDevName;
  UINT32                                    ResourceName;
  EFI_STATUS                                Status;

  Status = EFI_SUCCESS;
  AsfDevName = SIGNATURE_32 ('S', 'M', 'B', '1');
  ResourceName = SIGNATURE_32 ('_', 'C', 'R', 'S');
  AsfIrq = FchGetAsfIrq ();
  AsfBase = FchGetAsfIoBase ();

  CurrentPtr = (UINT8 *)AcpiTable;
  Status = FchFindNextDevice (&CurrentPtr, AcpiTable->Length, AsfDevName);
  AsfDevPtr = CurrentPtr;

  FchAcpiPkgLength ((UINT8*) (CurrentPtr + 2), &ByteCount, &DevLength);
  Status = FchFindNextSig (&CurrentPtr, DevLength + 2, ResourceName);

  CurrentPtr += 10;  //4+4+2
  WordPtr = (INT16 *)CurrentPtr;
  *WordPtr = AsfBase;
  *(++WordPtr) = AsfBase;

  CurrentPtr += 7;  //2+2+2+1
  if (AsfIrq <= 7) {
    *CurrentPtr = 1 << AsfIrq;
  } else {
    *(CurrentPtr++) = 0;
    *CurrentPtr = 1 << (AsfIrq - 8);
  }

  return Status;
}