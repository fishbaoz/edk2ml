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
#include "Filecode.h"
#define FILECODE UEFI_FCH_AVALON_DXE_FCHACPICS_FCHACPICSAML_FILECODE

EFI_STATUS
EFIAPI
FchFindNextSig (
  IN OUT   UINT8                            **Aml,
  IN       UINT32                           Size,
  IN       UINT32                           Signature
  )
{
  UINT8                                     *CurrentPtr;
  UINT32                                    *CurrentSig;

  CurrentPtr = *Aml;
  while ((UINTN)CurrentPtr < ((UINTN) (*Aml) + (UINTN)Size)) {
    CurrentSig = (UINT32*)CurrentPtr;
    if (*CurrentSig == Signature) {
      *Aml = CurrentPtr;
      return EFI_SUCCESS;
    }
    CurrentPtr++;
  }
  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
FchFindNextAdr32 (
  IN OUT   UINT8                            **Aml,
  IN       UINT32                           Size,
  IN       UINT32                           AdrValue
  )
{
  UINT8                                     *CurrentPtr;
  UINT32                                    CurrentSize;
  UINT32                                    *Signature;
  EFI_STATUS                                Status;

  CurrentPtr = *Aml;
  while ((UINTN)CurrentPtr < ((UINTN) (*Aml) + (UINTN)Size)) {
    CurrentSize = (UINT32) ((UINTN) (*Aml) + (UINTN)Size - (UINTN)CurrentPtr);
    Status = FchFindNextSig (&CurrentPtr, CurrentSize, AdrValue);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    CurrentPtr += 0x04;      //pointer to Aml next to _ADR value
    Signature = (UINT32*) (CurrentPtr - 9);
    if (*Signature == SIGNATURE_32 ('_', 'A', 'D', 'R')) {
      if ((*(CurrentPtr - 0x05) == 0x0C) && (*(CurrentPtr - 0x0A) == 0x08)) {
        *Aml = CurrentPtr - 0x0A;
        return EFI_SUCCESS;
      }
    }
  }
  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
FchFindNextDevice (
  IN OUT   UINT8                            **Aml,
  IN       UINT32                           Size,
  IN       UINT32                           DeviceName
  )
{
  UINT8                                     *CurrentPtr;
  UINT32                                    CurrentSize;
  UINT32                                    Signature;
  EFI_STATUS                                Status;

  CurrentPtr = *Aml;
  while ((UINTN)CurrentPtr < ((UINTN) (*Aml) + (UINTN)Size)) {
    CurrentSize = (UINT32) ((UINTN) (*Aml) + (UINTN)Size - (UINTN)CurrentPtr);
    Status = FchFindNextSig (&CurrentPtr, CurrentSize, DeviceName);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = FchFindParentDev (*Aml, &CurrentPtr, &Signature);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    if ( Signature == DeviceName) {
      *Aml = CurrentPtr;
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
FchFindParentDev (
  IN       UINT8                            *AmlStart,
  IN OUT   UINT8                            **AmlPtr,
     OUT   UINT32                           *AcpiName
  )
{
  UINT8                                     ByteCount;
  UINT8                                     *OrgPtr;
  UINT8                                     *CurrentPtr;
  UINT16                                    *OpCode;
  UINT32                                    *Signature;
  UINT32                                    PkgLength;

  OrgPtr = *AmlPtr;
  CurrentPtr = OrgPtr;

  while ((UINTN)CurrentPtr >= (UINTN)AmlStart) {
    CurrentPtr--;
    OpCode = (UINT16*)CurrentPtr;
    if (*OpCode == 0x825B) {
      FchAcpiPkgLength ( (UINT8*) (CurrentPtr + 2), &ByteCount, &PkgLength);
      if ((UINTN)OrgPtr < ((UINTN)CurrentPtr + (UINTN)PkgLength + 2)) {
        Signature = (UINT32*) ((UINTN)CurrentPtr + (UINTN)ByteCount + 2);
        *AcpiName = *Signature;
        *AmlPtr = CurrentPtr;
        return EFI_SUCCESS;
      }
    }
  }
  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
FchFindPci0NameFromIntDev (
  IN       UINT8                            *AmlStart,
  IN OUT   UINT8                            **AmlPtr,
     OUT   UINT32                           *AcpiName
  )
{
  EFI_STATUS                                Status;

  Status = FchFindParentDev (AmlStart, AmlPtr, AcpiName);

  Status = FchChkPci0 (*AmlPtr);

  return Status;
}

EFI_STATUS
EFIAPI
FchChkPci0 (
  IN       UINT8                            *AmlPtr
  )
{
  UINT8                                     *BlockStart;
  UINT8                                     *CurrentPtr;
  UINT8                                     *SegPtr;
  UINT8                                     ByteCount;
  UINT32                                    SearchDword;
  UINT32                                    PkgLength;
  UINT32                                    CurrentSize;
  UINT32                                    *Signature;
  EFI_STATUS                                Status;

  CurrentPtr  = AmlPtr;
  CurrentPtr += 0x02;
  FchAcpiPkgLength (CurrentPtr, &ByteCount, &PkgLength);

  CurrentPtr = (UINT8*) ((UINTN)CurrentPtr + (UINTN)ByteCount + 0x04);
  BlockStart = CurrentPtr;

  while ((UINTN)CurrentPtr < ((UINTN)AmlPtr + (UINTN)PkgLength + 2)) {
    SearchDword = 0x030AD041; //PNP0A03
    CurrentSize = (UINT32) ((UINTN)AmlPtr + (UINTN)PkgLength + 2 - (UINTN)CurrentPtr);
    Status = FchFindNextSig (&CurrentPtr, CurrentSize, SearchDword);
    if (EFI_ERROR (Status)) {
      break;
    }
    Signature = (UINT32*) (CurrentPtr - 5);
    if ((*Signature == SIGNATURE_32 ('_', 'C', 'I', 'D')) || (*Signature == SIGNATURE_32 ('_', 'H', 'I', 'D'))) {
      SegPtr = BlockStart;
      SearchDword = SIGNATURE_32 ('_', 'S', 'E', 'G');
      CurrentSize = (UINT32) ((UINTN)AmlPtr + (UINTN)PkgLength + 2 - (UINTN)SegPtr);
      Status = FchFindNextSig (&SegPtr, CurrentSize, SearchDword);
      if (Status == EFI_SUCCESS) {
        if (*(SegPtr + 1) != 0) {
          return EFI_NOT_FOUND;
        }
      }
      return EFI_SUCCESS;
    }
  }

  CurrentPtr = BlockStart;
  while ((UINTN)CurrentPtr < ((UINTN)AmlPtr + (UINTN)PkgLength + 2)) {
    SearchDword = 0x080AD041; //PNP0A08
    CurrentSize = (UINT32) ((UINTN)AmlPtr + (UINTN)PkgLength + 2 - (UINTN)CurrentPtr);
    Status = FchFindNextSig (&CurrentPtr, CurrentSize, SearchDword);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    Signature = (UINT32*) (CurrentPtr - 5);
    if ((*Signature == SIGNATURE_32 ('_', 'C', 'I', 'D')) || (*Signature == SIGNATURE_32 ('_', 'H', 'I', 'D'))) {
      SegPtr = BlockStart;
      SearchDword = SIGNATURE_32 ('_', 'S', 'E', 'G');
      CurrentSize = (UINT32) ((UINTN)AmlPtr + (UINTN)PkgLength + 2 - (UINTN)SegPtr);
      Status = FchFindNextSig (&SegPtr, CurrentSize, SearchDword);
      if (Status == EFI_SUCCESS) {
        if (*(SegPtr + 1) != 0) {
          return EFI_NOT_FOUND;
        }
      }
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

/*
EFI_STATUS
EFIAPI
AmdGetScopeList (
  IN UINT8                                  *AmlStart,
  IN UINT32                                 AmlSize,
  IN OUT AMD_ACPI_NAMESPACE_LOCATION        **ScopeList
  )
{
  UINT8                                     *Buffer;
  UINT8                                     *AmlPtr;
  UINT8                                     ByteCount;
  UINT32                                    PackageLength;
  EFI_STATUS                                Status;

  AmlPtr = AmlStart;
  Status = EFI_SUCCESS;

  while ((UINTN)AmlPtr < ((UINTN)AmlStart + (UINTN)DataSize)) {
    switch (*AmlPtr) {
    case ACPI_OBJECTS_ZERO:
    case ACPI_OBJECTS_ONE:
    case ACPI_OBJECTS_ONES:
      (UINTN)AmlPtr += 0x1;
      break;

    case ACPI_OBJECTS_BYTE_PREFIX:
      (UINTN)AmlPtr += 0x2;
      break;

    case ACPI_OBJECTS_WORD_PREFIX:
      (UINTN)AmlPtr += 0x3;
      break;

    case ACPI_OBJECTS_DWORD_PREFIX:
      (UINTN)AmlPtr += 0x5;
      break;

    case ACPI_OBJECTS_QWORD_PREFIX:
      (UINTN)AmlPtr += 0x9;
      break;

    case ACPI_OBJECTS_NAME:
      break;

    case ACPI_OBJECTS_ALIAS:
      break;

    case ACPI_OBJECTS_SCOPE:
      break;

    case ACPI_OBJECTS_OP:
      (UINTN)AmlPtr += 0x1;

      switch (*AmlPtr) {
      case ACPI_OBJECTS_OP_FIELD:
      case ACPI_OBJECTS_OP_INDEX_FIELD:
        AmlPtr ++;
        FchAcpiPkgLength (AmlPtr, &ByteCount, &PackageLength);
        AmlPtr = (UINT8*)((UINTN)AmlPtr + (UINTN)PackageLength);
        break;

      case ACPI_OBJECTS_OP_OPREGION:
        (UINTN)AmlPtr += 0x5;
        if ((*AmlPtr) < 0x0A)  AmlPtr++;
        else if ((((*AmlPtr) > 0x40) && ((*AmlPtr) < 0x5B)) || ((*AmlPtr) == 0x5F)) {
          AmlPtr = (UINT8*)((UINTN)AmlPtr + 0x04);        //pre-difined fields
        } else {
        //
        }

        case ACPI_REGION_SYSTEMMEMORY:
          (UINTN)AmlPtr += 0x5;
          break;
        default:
          break;
        }
        break;

      default:
        break;
      }

      break;

    default:
      break;
    }

  }

  return Status;
}
*/
BOOLEAN
FchIsAmlOpRegionObject (
  IN UINT8     *AmlPointer
  )
{
  UINT16                 *Operation;

  Operation = (UINT16*) (AmlPointer - 2);
  if (*Operation == 0x805b ) {
    return TRUE;
  }
  return FALSE;
}

EFI_STATUS
FchSetOpRegion (
  IN UINT8               *AmlPointer,
  IN VOID*               RegionStartAddress,
  IN UINT32              RegionSize
  )
{
  UINT8                         AddressPrefix;
  UINT32                        *Address;
  UINT32                        *DwordSize;
  UINT8                         SizePrefix;
  UINT16                        *WordSize;
  UINT8                         *ByteSize;

  AddressPrefix = *(AmlPointer + 5);
//  ASSERT (AddressPrefix == AML_DWORD_PREFIX);
  Address = (UINT32*) (AmlPointer + 6);
//  ASSERT ((((UINTN)RegionStartAddress) & 0xFFFFFFFF) == (UINTN)RegionStartAddress);
  *Address = (UINT32)(UINTN)RegionStartAddress;
  SizePrefix = *(AmlPointer + 10);
  if (SizePrefix == 0x0C) {
    DwordSize = (UINT32*) (AmlPointer + 11);
    *DwordSize = RegionSize;
  } else if (SizePrefix == 0x0B) {
    ASSERT (RegionSize < 0x10000);
    RegionSize &= 0xFFFF;
    WordSize = (UINT16*) (AmlPointer + 11);
    *WordSize = (UINT16) RegionSize;
  } else if (SizePrefix == 0x0A) {
    ASSERT (RegionSize < 0x100);
    RegionSize &= 0xFF;
    ByteSize = (UINT8*) (AmlPointer + 11);
    *ByteSize = (UINT8) RegionSize;
  } else {
//    EFI_DEADLOOP ();  // Wrong assignment could corrupt the DSDT
  }

  return EFI_SUCCESS;
}

VOID
FchAcpiPkgLength (
  IN       UINT8     *AmlPointer,
     OUT   UINT8     *ByteCount,
     OUT   UINT32    *PkgLength
  )
{
  UINT8   Byte0;
  UINT8   Byte1;
  UINT8   Byte2;
  UINT8   Byte3;

  Byte0 = *AmlPointer;
  Byte1 = *(AmlPointer + 1);
  Byte2 = *(AmlPointer + 2);
  Byte3 = *(AmlPointer + 3);

  switch (Byte0 & 0xC0) {
  case 0x00:
    *ByteCount  = 0x01;
    *PkgLength = (UINT32)(Byte0 & 0x3F);
    break;

  case 0x40:
    *ByteCount  = 0x02;
    *PkgLength = (UINT32)(Byte0 & 0x0F) + (UINT32)Byte1 * 0x10;
    break;

  case 0x80:
    *ByteCount  = 0x03;
    *PkgLength = (UINT32)(Byte0 & 0x0F) + (UINT32)Byte1 * 0x10 + (UINT32)Byte2 * 0x1000;
    break;

  case 0xC0:
    *ByteCount  = 0x04;
    *PkgLength = (UINT32)(Byte0 & 0x0F) + (UINT32)Byte1 * 0x10 + (UINT32)Byte2 * 0x1000 + (UINT32)Byte3 * 0x100000;
    break;
  }

  return;

}

VOID
FchAddPkLength (
  IN OUT   UINT8     **AmlPointer,
  IN       UINT32    PkgLength
  )
{
  UINT8       *LocalPtr;
  UINT8       Byte0;
  UINT8       Byte1;
  UINT8       Byte2;
  UINT8       Byte3;

  Byte0 = 0;
  Byte1 = 0;
  Byte2 = 0;
  Byte3 = 0;
  LocalPtr = *AmlPointer;

  if (PkgLength < 64) {
    Byte0 = (UINT8)PkgLength;
    *LocalPtr = Byte0;
    LocalPtr++;
  } else {
    Byte0 = ((UINT8)PkgLength & 0xF) + 0x40;
    *LocalPtr = Byte0;
    LocalPtr++;
    Byte1 = (UINT8) ((PkgLength >> 4) & 0xFF);
    *LocalPtr = Byte1;
    LocalPtr++;
    if (PkgLength > 0xFFF) {
      *(LocalPtr - 2) += 0x40;
      Byte2 = (UINT8) ((PkgLength >> 12) & 0xFF);
      *LocalPtr = Byte2;
      LocalPtr++;
      if (PkgLength > 0xFFFFF) {
        *(LocalPtr - 3) += 0x40;
        Byte3 = (UINT8) ((PkgLength >> 20) & 0xFF);
        *LocalPtr = Byte3;
        LocalPtr++;
        if (PkgLength > 0xFFFFFFF) {
        }
      }
    }
  }
  *AmlPointer = LocalPtr;
}

VOID
FchAddRegionLength (
  IN OUT   UINT8     **AmlPointer,
  IN       UINT32    RegionLength
  )
{
  UINT8       *LocalPtr;

  LocalPtr = *AmlPointer;

  if (RegionLength > 0xffff) {
    *LocalPtr = 0x0C;
    LocalPtr++;
    *(UINT32*)LocalPtr = RegionLength;
    LocalPtr += 0x04;
  } else if (RegionLength > 0xff) {
    *LocalPtr = 0x0B;
    LocalPtr++;
    *(UINT16*)LocalPtr = (UINT16)RegionLength;
    LocalPtr += 0x02;
  } else {
    *LocalPtr = 0x0A;
    LocalPtr++;
    *LocalPtr = (UINT8)RegionLength;
    LocalPtr ++;
  }
  *AmlPointer = LocalPtr;
}

EFI_STATUS
FchGetFieldOffset (
  IN       UINT8               *AmlPointer,
     OUT   UINT32              *Length,
     OUT   UINT32              *Offset
  )
{
  UINT8      ByteCount;
  UINT8      *CurrentPtr;
  UINT16     *FieldOp;
  UINT32     CurrentOffset;
  UINT32     PkLength;
  UINT32     FieldLength;

  CurrentOffset = 0;
  PkLength      = 0;
  FieldLength   = 0;
  FieldOp = (UINT16 *)AmlPointer;
  if ( (*FieldOp) != 0x815B) {
    return EFI_ABORTED;
  }

  CurrentPtr = AmlPointer + 0x02;

  FchAcpiPkgLength (CurrentPtr, &ByteCount, &FieldLength);

  FieldLength += 0x02;
  *Length = FieldLength;

  CurrentPtr += ByteCount;
  CurrentPtr += 0x5;                   //Namestring + AccessFlag

  while ((UINTN)CurrentPtr < ((UINTN)AmlPointer + (UINTN)FieldLength)) {
    switch (*CurrentPtr) {
    case 0x00:
      //ReservedField
      CurrentPtr++;
      FchAcpiPkgLength (CurrentPtr, &ByteCount, &PkLength);
      CurrentPtr = (UINT8*) ((UINTN)CurrentPtr + (UINTN)ByteCount);
      CurrentOffset += PkLength;
      break;

    case 0x01:
      //AccessField
      CurrentPtr += 0x03;
      break;

    case 0x02:
      //ConnectField
      CurrentPtr += 0x05;
      break;

    case 0x03:
      //ExtendedAccessField
      CurrentPtr += 0x04;
      break;

    default:
      //NamedField
      CurrentPtr += 0x04;
      FchAcpiPkgLength (CurrentPtr, &ByteCount, &PkLength);
      CurrentPtr = (UINT8*) ((UINTN)CurrentPtr + (UINTN)ByteCount);
      CurrentOffset += PkLength;
      break;
    }
  }

  *Offset = CurrentOffset;

  return EFI_SUCCESS;
}

EFI_STATUS
FchGenerateFieldListAml (
  IN       FCH_ACPI_FIELD_NAME    *FieldList,
  IN       UINT32                  FieldListSize,
  IN OUT   UINT32                  *Offset,
     OUT   UINT8                   **AmlPointer,
     OUT   UINT32                  *Length
  )
{
  UINT8                    *Temp;
  UINT8                    *AmlPtr;
  UINT32                   Count;
  UINT32                   Index;
  UINT32                   PkLength;
  UINT32                   FieldSize;
  UINT32                   CurrentOffset;
  UINT32                   *NameString;
  EFI_STATUS               Status;

  //generate aml binary for append Fields definition
  Count = (FieldListSize - 1) / (sizeof (FCH_ACPI_FIELD_NAME));
  FieldSize = Count * 8 + 5;                           // 8: NameString + PkLength
                                                       // 5: revserved field for offset gap
  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  FieldSize,
                  &Temp
                  );

  ASSERT_EFI_ERROR (Status);

  AmlPtr = Temp;
  CurrentOffset = *Offset;

  for (Index = 0; Index < Count; Index++) {
    if ( CurrentOffset > (FieldList[Index].Offset * 8)) {
      return EFI_ABORTED;
    } else if (CurrentOffset < (FieldList[Index].Offset * 8)) {
      *AmlPtr = 0x00;
      AmlPtr++;
      PkLength = (FieldList[Index].Offset * 8) - CurrentOffset;
      FchAddPkLength (&AmlPtr, PkLength);
      CurrentOffset += PkLength;
    }

    NameString = (UINT32*)AmlPtr;
    *NameString = FieldList[Index].AcpiName;
    AmlPtr += 0x04;
    switch (FieldList[Index].FieldLength) {
    case FCH_ACPI_FIELD_LENGTH_BYTE:
      *AmlPtr = 0x08;
      AmlPtr++;
      CurrentOffset += 0x08;
      break;

    case FCH_ACPI_FIELD_LENGTH_WORD:
      *AmlPtr = 0x10;
      AmlPtr++;
      CurrentOffset += 0x10;
      break;

    case FCH_ACPI_FIELD_LENGTH_DWORD:
      *AmlPtr = 0x20;
      AmlPtr++;
      CurrentOffset += 0x20;
      break;

    default:
      return EFI_ABORTED;
    }
  }

  *Offset = CurrentOffset;
  *Length = (UINT32)((UINTN)AmlPtr - (UINTN)Temp);
  *AmlPointer = Temp;
  return Status;
}

UINT32
FchAdjustFieldLength (
  IN       UINT32                 NewLength,
  IN       UINT32                 OldLength
  )
{
  UINT32 Length;

  Length = NewLength;
  //adjust new filed Length by checking possible PkLength change
  if (OldLength < 64) {
    if (Length > 0xFFFFF) {
      Length += 0x03;
    } else if (Length > 0xFFF) {
      Length += 0x02;
    } else if (Length > 63) {
      Length += 0x01;
    }
  } else if  (OldLength < 0x1000) {
    if (Length > 0xFFFFF) {
      Length += 0x02;
    } else if (Length > 0xFFF) {
      Length += 0x01;
    }
  } else if  (OldLength < 0x100000) {
    if (Length > 0xFFFFF) {
      Length += 0x01;
    }
  }
  return Length;
}


EFI_STATUS
FchMergeField (
  IN       UINT8                   *OldFieldAml,
  IN       UINT8                   *AddFieldAml,
  IN       UINT8                   *NewFieldAml,
  IN       UINT32                  TotalLength,
  IN       UINT32                  OldLength
  )
{
  UINT8                    ByteCount;
  UINT8                    *AmlPtr;
  UINT16                   *FieldOp;
  UINT32                   Length;

  AmlPtr = NewFieldAml;

  //first add FieldOP 0x5B 0x81
  FieldOp  = (UINT16 *)AmlPtr;
  *FieldOp = 0x815B;
  AmlPtr  += 0x02;

  //add PkLength
  FchAddPkLength (&AmlPtr, TotalLength - 2);

  //find start of field block in old aml data
  FchAcpiPkgLength (OldFieldAml + 2, &ByteCount, &Length);

  //copy field block from original aml
  CopyMem (AmlPtr, (UINT8 *) ((UINTN)OldFieldAml + 2 + (UINTN)ByteCount), OldLength - 2 - (UINT32)ByteCount);

  AmlPtr  = (UINT8*) ((UINTN)OldLength - 2 - (UINTN)ByteCount + (UINTN)AmlPtr);
  Length  = (UINT32) ((UINTN)AmlPtr - (UINTN)NewFieldAml);
  Length  = TotalLength - Length;
  CopyMem (AmlPtr, AddFieldAml, Length);

  return EFI_SUCCESS;
}

EFI_STATUS
FchUpdateAcpiField (
  IN       FCH_ACPI_FIELD_NAME    *FieldList,
  IN       UINT32                  FieldListSize,
  IN       UINT8                   *AmlPointer,
  IN OUT   UINT8                   **NewAmlPointer,
     OUT   UINT32                  *Length
  )
{
  UINT8                    *Temp;
  UINT32                   CurrentOffset;
  UINT32                   CurrentLength;
  UINT32                   TotalLength;
  EFI_STATUS               Status;

  CurrentOffset = 0;
  CurrentLength = 0;
  TotalLength   = 0;
  Status = FchGetFieldOffset (AmlPointer, &CurrentLength, &CurrentOffset);

  Status = FchGenerateFieldListAml (FieldList, FieldListSize, &CurrentOffset, &Temp, &TotalLength);

  TotalLength += CurrentLength;
  //adjust new filed Length by checking possible PkLength change
  TotalLength = FchAdjustFieldLength (TotalLength - 2, CurrentLength - 2) + 2;

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  TotalLength,
                  NewAmlPointer
                  );

  ASSERT_EFI_ERROR (Status);

  Status = FchMergeField (AmlPointer, Temp, *NewAmlPointer, TotalLength, CurrentLength);
  *Length = TotalLength;
  gBS->FreePool (Temp);

  return Status;
}

EFI_STATUS
FchUpdateAcpiDataScope (
  IN       UINT8                   *OldScope,
  IN OUT   UINT8                   **AmlPointer,
  IN       UINT32                  ScopeSize,
  IN       UINT32                  *DataAddress,
  IN       UINT32                  DataSize
  )
{
  UINT8                        *AmlPtr;
  UINT8                        *OldAmlPtr;
  UINT8                        ByteCount;
  UINT32                       PkgLength;
  UINT32                       *Address;

  AmlPtr = *AmlPointer;
  OldAmlPtr = OldScope;

  *AmlPtr = 0x10;
  AmlPtr++;
  FchAddPkLength (&AmlPtr, ScopeSize);

  OldAmlPtr = (UINT8*) ((UINTN)OldAmlPtr + 1);
  FchAcpiPkgLength (OldAmlPtr, &ByteCount, &PkgLength);
  OldAmlPtr = (UINT8*) ((UINTN)OldAmlPtr + (UINTN)ByteCount);
  CopyMem (AmlPtr, OldAmlPtr, 0x0D);      //0x0D: copy to Region Offset

  AmlPtr += 0x0D;
  Address = (UINT32*)AmlPtr;
  *Address = (UINT32)((UINTN)DataAddress);
  AmlPtr += 0x04;

  FchAddRegionLength (&AmlPtr, DataSize);

  *AmlPointer = AmlPtr;
  return EFI_SUCCESS;
}

