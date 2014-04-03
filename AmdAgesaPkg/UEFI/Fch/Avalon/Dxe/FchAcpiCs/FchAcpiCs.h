/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH DXE Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH DXE Driver
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
#ifndef _FCH_ACPI_H_
#define _FCH_ACPI_H_

#include "Acpi3_0.h"
#include "AcpiCommon.h"
#include "EfiDriverlib.h"
#include "FchDxeLib.h"

#include EFI_PROTOCOL_DEFINITION (PciIo)
#include EFI_PROTOCOL_DEFINITION (PciRootBridgeIo)
#include EFI_PROTOCOL_DEPENDENCY (SmmBase)
#include EFI_PROTOCOL_DEPENDENCY (LoadedImage)
#include EFI_PROTOCOL_CONSUMER   (FchInitProtocol)
#include EFI_PROTOCOL_CONSUMER   (AcpiSupport)
#include EFI_PROTOCOL_CONSUMER   (AcpiTable)
#include EFI_PROTOCOL_CONSUMER   (AcpiSystemDescriptionTable)
#include EFI_PROTOCOL_PRODUCER   (FchAcpiProtocol)

#define AMD_FCH_ACPI_GUID \
  { 0x6D56EAF7, 0x92CF, 0x43F7, 0x89, 0x68, 0xBE, 0x64, 0x53, 0x20, 0x51, 0xAE }


///
/// AMD_FCH_COMMON_DATA
///
typedef struct {
  UINT32             Signature;             ///< Signature
  UINT8              FchGpioAcpiEnable:1;   ///< FchGpioAcpiEnable
  UINT8              FchAsfAcpiEnable:1;    ///< FchAsfAcpiEnable
  UINT8              Byte4ReserveBit:6;     ///< Reserved for furture use
  UINT8              ByteReserve;           ///< Reserved for furture use
  UINT16             SciInterrupt;          ///< SciInterrupt
  UINT32             Reserve[30];           ///< Reserved for furture use
} AMD_FCH_COMMON_DATA;

EFI_STATUS
EFIAPI
FchAcpiCsInit (
  IN    EFI_HANDLE                          ImageHandle,
  IN    EFI_SYSTEM_TABLE                    *SystemTable
);

VOID
EFIAPI
FchAcpiReadyToBootInit (
  IN EFI_EVENT                              Event,
  IN VOID                                   *Context
);


EFI_STATUS
EFIAPI
FchAcpiInstall (
  IN EFI_ACPI_SDT_HEADER                    *Table,
  IN EFI_ACPI_TABLE_VERSION                 Version,
  IN UINTN                                  TableKey
);

VOID
EFIAPI
FchAcpiInstallNotify (
  IN EFI_EVENT                              Event,
  IN VOID                                   *Context
);

VOID
EFIAPI
FchAcpiInstall2Notify (
  IN EFI_EVENT                              Event,
  IN VOID                                   *Context
);

EFI_STATUS
FchInitDefaultConfigDxe (
  IN OUT   AMD_FCH_COMMON_DATA              *FchData
);

EFI_STATUS
EFIAPI
AmdFchGetIntDevPath (
  IN       FCH_ACPI_PROTOCOL                *FchAcpi,
  IN       UINT32                           IntDevAddress,
  IN OUT   UINT32                           *PathName
);

EFI_STATUS
EFIAPI
AmdFchDataDxeAddItem (
  IN       FCH_ACPI_PROTOCOL                *FchAcpiProtocol,
  IN       UINT8                            Length,
  IN       VOID                             *Data,
  IN       UINT32                           AcpiName,
     OUT   UINT32                           *DataOffset
);

EFI_STATUS
EFIAPI
AmdFchAcpiEnableGpio (
  IN       FCH_ACPI_PROTOCOL                *FchAcpiProtocol,
  IN       BOOLEAN                          Enable
);

EFI_STATUS
EFIAPI
AmdFchAcpiEnableAsf (
  IN       FCH_ACPI_PROTOCOL                *FchAcpiProtocol,
  IN       BOOLEAN                          Enable
);

EFI_STATUS
EFIAPI
AmdFchGetSpbDevPath (
  IN       FCH_ACPI_PROTOCOL                *FchAcpi,
  IN       UINT32                           IntDevAddress,
  IN OUT   UINT32                           *PathName
);

EFI_STATUS
EFIAPI
AmdBuildAcpiDsdtTree (
  IN       EFI_ACPI_DESCRIPTION_HEADER      *Table,
  IN OUT   VOID                             **AcpiDsdtTree
);

EFI_STATUS
EFIAPI
FchBuildInternalDeviceList (
  IN       UINT8                            *AmlStart,
  IN       UINT32                           AmlSize
);

EFI_STATUS
EFIAPI
FchFindNextSig (
  IN OUT   UINT8                            **Aml,
  IN       UINT32                           Size,
  IN       UINT32                           Signature
);

EFI_STATUS
EFIAPI
FchFindNextAdr32 (
  IN OUT   UINT8                            **Aml,
  IN       UINT32                           Size,
  IN       UINT32                           AdrValue
);

EFI_STATUS
EFIAPI
FchFindNextDevice (
  IN OUT   UINT8                            **Aml,
  IN       UINT32                           Size,
  IN       UINT32                           DeviceName
);

EFI_STATUS
EFIAPI
FchFindParentDev (
  IN       UINT8                            *AmlStart,
  IN OUT   UINT8                            **AmlPtr,
     OUT   UINT32                           *AcpiName
);

EFI_STATUS
EFIAPI
FchFindPci0NameFromIntDev (
  IN       UINT8                            *AmlStart,
  IN OUT   UINT8                            **AmlPtr,
     OUT   UINT32                           *AcpiName
);

EFI_STATUS
EFIAPI
FchChkPci0 (
  IN       UINT8                            *AmlPtr
);

EFI_STATUS
EFIAPI
CheckDataBuffer (
  IN       VOID                             *PathName
);

BOOLEAN
FchIsAmlOpRegionObject (
  IN UINT8                                  *AmlPointer
);

EFI_STATUS
FchSetOpRegion (
  IN UINT8                                  *AmlPointer,
  IN VOID*                                  RegionStartAddress,
  IN UINT32                                 RegionSize
);

VOID
FchAcpiPkgLength (
  IN       UINT8                            *AmlPointer,
     OUT   UINT8                            *ByteCount,
     OUT   UINT32                           *PkgLength
);

VOID
FchAddPkLength (
  IN OUT   UINT8                            **AmlPointer,
  IN       UINT32                           PkgLength
);

VOID
FchAddRegionLength (
  IN OUT   UINT8                            **AmlPointer,
  IN       UINT32                           RegionLength
);

EFI_STATUS
FchGetFieldOffset (
  IN       UINT8                            *AmlPointer,
     OUT   UINT32                           *Length,
     OUT   UINT32                           *Offset
);

EFI_STATUS
FchGenerateFieldListAml (
  IN       FCH_ACPI_FIELD_NAME              *FieldList,
  IN       UINT32                           FieldListSize,
  IN OUT   UINT32                           *Offset,
     OUT   UINT8                            **AmlPointer,
     OUT   UINT32                           *Length
);

UINT32
FchAdjustFieldLength (
  IN       UINT32                           NewLength,
  IN       UINT32                           OldLength
);

EFI_STATUS
FchMergeField (
  IN       UINT8                            *OldFieldAml,
  IN       UINT8                            *AddFieldAml,
  IN       UINT8                            *NewFieldAml,
  IN       UINT32                           TotalLength,
  IN       UINT32                           OldLength
);

EFI_STATUS
FchUpdateAcpiField (
  IN       FCH_ACPI_FIELD_NAME              *FieldList,
  IN       UINT32                           FieldListSize,
  IN       UINT8                            *AmlPointer,
  IN OUT   UINT8                            **NewAmlPointer,
     OUT   UINT32                           *Length
);

EFI_STATUS
FchUpdateAcpiDataScope (
  IN       UINT8                            *OldScope,
  IN OUT   UINT8                            **AmlPointer,
  IN       UINT32                           ScopeSize,
  IN       UINT32                           *DataAddress,
  IN       UINT32                           DataSize
);

EFI_STATUS
UpdateAcpiName (
  IN       VOID                             *AcpiTable,
  IN       VOID                             *OldName,
  IN       VOID                             *NewName,
  IN       UINT8                            NameLength
);

BOOLEAN
CheckData (
  IN       VOID                             *Ptr1,
  IN       VOID                             *Ptr2,
  IN       UINT8                            Len
);

EFI_STATUS
FchUpdateAcpiDataTable (
  IN       FCH_ACPI_PROTOCOL                *FchAcpiProtocol,
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER      **AcpiTable
);

EFI_STATUS
FchUpdateAcpiGpioController (
  IN       FCH_ACPI_PROTOCOL                *FchAcpiProtocol,
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER      *AcpiTable
);

EFI_STATUS
FchUpdateAcpiAsf (
  IN       FCH_ACPI_PROTOCOL                *FchAcpiProtocol,
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER      *AcpiTable
);

extern AMD_FCH_COMMON_DATA *mFchCommonData;
#endif