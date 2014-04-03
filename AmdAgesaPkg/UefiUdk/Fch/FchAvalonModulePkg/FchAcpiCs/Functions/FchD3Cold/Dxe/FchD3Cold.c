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
#include "FchD3Cold.h"
#include "Filecode.h"
#define FILECODE UEFI_FCH_AVALON_DXE_FCHACPICS_FUNCTIONS_FCHD3COLD_FCHD3COLD_FILECODE

EFI_GUID gAmdFchD3ColdAcpiGuid   = AMD_FCH_D3COLD_ACPI_GUID;

/*********************************************************************************
 * Name: FchD3ColdInit
 *
 * Description
 *   Entry point of the AMD FCH D3Cold support driver
 *
 * Input
 *   ImageHandle : EFI Image Handle for the DXE driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchD3ColdInit (
  IN    EFI_HANDLE                  ImageHandle,
  IN    EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS                        Status;
  FCH_INIT_PROTOCOL                 *pAmdFchInitProtocol;
  VOID                              *Registration;

  Status = gBS->LocateProtocol (
                  &gFchInitProtocolGuid,
                  NULL,
                  &pAmdFchInitProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (pAmdFchInitProtocol->FchPolicy.Misc.FchCsSupport.FchCsD3Cold == FALSE) {
    return EFI_UNSUPPORTED;
  }

  //
  // Register event
  //
  EfiCreateProtocolNotifyEvent (
    &gFchAcpiProtocolGuid,
    TPL_CALLBACK,
    FchD3ColdAcpiConfigureNotify,
    NULL,
    &Registration
    );

  EfiCreateProtocolNotifyEvent (
    &gFchAcpiTableInstallGuid,
    TPL_CALLBACK,
    FchD3ColdAcpiInstallNotify,
    NULL,
    &Registration
    );

  return EFI_SUCCESS;
}

VOID
EFIAPI
FchD3ColdAcpiConfigureNotify (
  IN EFI_EVENT                              Event,
  IN VOID                                   *Context
  )
{
  EFI_STATUS                        Status;
  FCH_ACPI_PROTOCOL                 *FchAcpiProtocol;
  FCH_INIT_PROTOCOL                 *AmdFchInit;

  UINT32  SataBaseSig;
  UINT32  UsbBaseSig;
  UINT32  SdBaseSig;
  UINT32  *SataBase;
  UINT32  *UsbBase;
  UINT32  *SdBase;
  UINT32  SataBaseOffset;
  UINT32  UsbBaseOffset;
  UINT32  SdBaseOffset;

  SataBaseSig = SIGNATURE_32 ('S', 'A', 'B', 'B');
  UsbBaseSig  = SIGNATURE_32 ('U', 'S', 'B', 'B');
  SdBaseSig   = SIGNATURE_32 ('S', 'D', 'B', 'B');
  Status = gBS->LocateProtocol (
                  &gFchAcpiProtocolGuid,
                  NULL,
                  &FchAcpiProtocol
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = gBS->LocateProtocol (
                  &gFchInitProtocolGuid,
                  NULL,
                  &AmdFchInit
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  SataBase = (UINT32 *) &(AmdFchInit->FchPolicy.Sata);
  UsbBase  = (UINT32 *) &(AmdFchInit->FchPolicy.Usb);
  SdBase   = (UINT32 *) &(AmdFchInit->FchPolicy.Sd);

  Status = FchAcpiProtocol->FchAcpiAddDataField (
                              FchAcpiProtocol,
                              FCH_ACPI_FIELD_LENGTH_DWORD,
                              &SataBase,
                              SataBaseSig,
                              &SataBaseOffset
                              );

  Status = FchAcpiProtocol->FchAcpiAddDataField (
                              FchAcpiProtocol,
                              FCH_ACPI_FIELD_LENGTH_DWORD,
                              &UsbBase,
                              UsbBaseSig,
                              &UsbBaseOffset
                              );

  Status = FchAcpiProtocol->FchAcpiAddDataField (
                              FchAcpiProtocol,
                              FCH_ACPI_FIELD_LENGTH_DWORD,
                              &SdBase,
                              SdBaseSig,
                              &SdBaseOffset
                              );
}

VOID
EFIAPI
FchD3ColdAcpiInstallNotify (
  IN EFI_EVENT                              Event,
  IN VOID                                   *Context
  )
{
  UINTN                                     Index;
  INTN                                      Instance;
  UINTN                                     Size;
  UINTN                                     NumberOfHandles;
  UINTN                                     TableHandle;
  UINTN                                     TableSize;
  UINT32                                    FvStatus;
  EFI_STATUS                                Status;
  EFI_HANDLE                                *HandleBuffer;
  EFI_FV_FILETYPE                           FileType;
  EFI_FV_FILE_ATTRIBUTES                    Attributes;
  EFI_ACPI_COMMON_HEADER                    *CurrentTable;
  EFI_ACPI_DESCRIPTION_HEADER               *FchD3ColdAcpiBlockPtr  = NULL;

  EFI_ACPI_TABLE_PROTOCOL                   *AcpiTableProtocol;
  EFI_ACPI_SDT_PROTOCOL                     *AcpiSdtProtocol;
  FCH_ACPI_PROTOCOL                         *FchAcpiProtocol;
  EFI_FIRMWARE_VOLUME2_PROTOCOL              *FwVol = NULL;

  Status = gBS->LocateProtocol (
                  &gEfiAcpiTableProtocolGuid,
                  NULL,
                  &AcpiTableProtocol
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = gBS->LocateProtocol (
                  &gFchAcpiProtocolGuid,
                  NULL,
                  &FchAcpiProtocol
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = gBS->LocateProtocol (
                  &gEfiAcpiSdtProtocolGuid,
                  NULL,
                  &AcpiSdtProtocol
                  );
  if (EFI_ERROR (Status)) {
    return;
  }
  Status = AcpiSdtProtocol->RegisterNotify (
                              TRUE,
                              FchDsdtHwReducedAcpi
                              );
  ASSERT (!EFI_ERROR (Status));


  FvStatus = 0;
  //
  // Locate protocol.
  //
  Status = gBS->LocateHandleBuffer (
                   ByProtocol,
                   &gEfiFirmwareVolume2ProtocolGuid,
                   NULL,
                   &NumberOfHandles,
                   &HandleBuffer
                   );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Looking for FV with FCH ACPI Data Block file
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {
    //
    // Get the protocol on this handle
    // This should not fail because of LocateHandleBuffer
    //
    Status = gBS->HandleProtocol (
                     HandleBuffer[Index],
                     &gEfiFirmwareVolume2ProtocolGuid,
                     (VOID**) &FwVol
                     );
    ASSERT_EFI_ERROR (Status);

    //
    // See if it has the ACPI storage file
    //
    Size      = 0;
    FvStatus  = 0;
    Status = FwVol->ReadFile (
                      FwVol,
                      &gAmdFchD3ColdAcpiGuid,
                      NULL,
                      &Size,
                      &FileType,
                      &Attributes,
                      &FvStatus
                      );

    //
    // If we found it, then we are done
    //
    if (Status == EFI_SUCCESS) {
      break;
    }
  }

  //
  // Read tables from the storage file.
  //
  Instance = 0;
  CurrentTable = NULL;
  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                      FwVol,
                      &gAmdFchD3ColdAcpiGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      &CurrentTable,
                      &Size,
                      &FvStatus
                      );

    if (!EFI_ERROR (Status)) {
      //
      // Check the table ID to modify the table
      //
      if (((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId == SIGNATURE_64 ('F', 'C', 'H', 'C', 'S', 'D', '3', 0)) {
        FchD3ColdAcpiBlockPtr = (EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable;

        Status = FchD3ColdUpdateAcpiTable (FchAcpiProtocol, &FchD3ColdAcpiBlockPtr);

        TableHandle = 0;
        TableSize = FchD3ColdAcpiBlockPtr->Length;

        //
        // Install ACPI table
        //
        Status = AcpiTableProtocol->InstallAcpiTable (
                                      AcpiTableProtocol,
                                      FchD3ColdAcpiBlockPtr,
                                      TableSize,
                                      &TableHandle
                                      );

        //
        // Free memory allocated by ReadSection
        //
        gBS->FreePool (CurrentTable);

        if (EFI_ERROR (Status)) {
          return;
        }
      }

      //
      // Increment the instance
      //
      Instance++;
      CurrentTable = NULL;
    }
  }

  //
  // Our exit status is determined by the success of the previous operations
  // If the protocol was found, Instance already points to it.
  //
  //
  // Free any allocated buffers
  //
  gBS->FreePool (HandleBuffer);

}

EFI_STATUS
FchD3ColdUpdateAcpiTable (
  IN       FCH_ACPI_PROTOCOL                *FchAcpiProtocol,
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER      **AcpiTable
  )
{
  UINT8                                     *CurrentPtr;
  EFI_ACPI_DESCRIPTION_HEADER               *CurrentTable;
  UINT32                                    DevAddress;
  UINT32                                    NewDevName;
  UINT32                                    OldDevName;
  EFI_STATUS                                Status;

  CurrentPtr = (UINT8 *)AcpiTable;
  CurrentTable = *AcpiTable;

  // update PCI0
  DevAddress = 0x00000000;
  OldDevName = SIGNATURE_32 ('P', 'C', 'I', '0');
  Status = FchAcpiProtocol->FchAcpiGetIntDevPathName (FchAcpiProtocol, DevAddress, &NewDevName);
  Status = FchAcpiProtocol->FchAcpiTool.UpdateAcpiName (CurrentTable, &OldDevName, &NewDevName, 4);

  // update EHCI1
  DevAddress = 0x00120000;
  OldDevName = SIGNATURE_32 ('E', 'H', 'C', '1');
  Status = FchAcpiProtocol->FchAcpiGetIntDevPathName (FchAcpiProtocol, DevAddress, &NewDevName);
  Status = FchAcpiProtocol->FchAcpiTool.UpdateAcpiName (CurrentTable, &OldDevName, &NewDevName, 4);

  // update EHC2
  DevAddress = 0x00130000;
  OldDevName = SIGNATURE_32 ('E', 'H', 'C', '2');
  Status = FchAcpiProtocol->FchAcpiGetIntDevPathName (FchAcpiProtocol, DevAddress, &NewDevName);
  Status = FchAcpiProtocol->FchAcpiTool.UpdateAcpiName (CurrentTable, &OldDevName, &NewDevName, 4);

  // update EHC3
  DevAddress = 0x00160000;
  OldDevName = SIGNATURE_32 ('E', 'H', 'C', '3');
  Status = FchAcpiProtocol->FchAcpiGetIntDevPathName (FchAcpiProtocol, DevAddress, &NewDevName);
  Status = FchAcpiProtocol->FchAcpiTool.UpdateAcpiName (CurrentTable, &OldDevName, &NewDevName, 4);

  // update SATA
  DevAddress = 0x00110000;
  OldDevName = SIGNATURE_32 ('S', 'A', 'T', 'A');
  Status = FchAcpiProtocol->FchAcpiGetIntDevPathName (FchAcpiProtocol, DevAddress, &NewDevName);
  Status = FchAcpiProtocol->FchAcpiTool.UpdateAcpiName (CurrentTable, &OldDevName, &NewDevName, 4);

  // update XHC0
  DevAddress = 0x00100000;
  OldDevName = SIGNATURE_32 ('X', 'H', 'C', '0');
  Status = FchAcpiProtocol->FchAcpiGetIntDevPathName (FchAcpiProtocol, DevAddress, &NewDevName);
  Status = FchAcpiProtocol->FchAcpiTool.UpdateAcpiName (CurrentTable, &OldDevName, &NewDevName, 4);

  // update SDIO
  DevAddress = 0x00140007;
  OldDevName = SIGNATURE_32 ('S', 'D', 'I', 'O');
  Status = FchAcpiProtocol->FchAcpiGetIntDevPathName (FchAcpiProtocol, DevAddress, &NewDevName);
  Status = FchAcpiProtocol->FchAcpiTool.UpdateAcpiName (CurrentTable, &OldDevName, &NewDevName, 4);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FchDsdtHwReducedAcpi (
  IN EFI_ACPI_SDT_HEADER    *Table,
  IN EFI_ACPI_TABLE_VERSION Version,
  IN UINTN                  TableKey
  )
{
  BOOLEAN                                   HwReducedAcpi;
  EFI_STATUS                                Status;
  FCH_INIT_PROTOCOL                         *FchInitProtocol;
  EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE *Fadt;

  Status = EFI_SUCCESS;

  //
  // Check Signture and update FADT table
  //
  if ((Table->Signature == EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE) && (Table->Revision == 0x05 ))  {
    Fadt = (EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE *)Table;

    Status = gBS->LocateProtocol (
                    &gFchInitProtocolGuid,
                    NULL,
                    &FchInitProtocol
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    HwReducedAcpi = FchInitProtocol->FchPolicy.Misc.FchCsSupport.FchCsHwReduced;

    Fadt->Flags |= LOW_POWER_S0_IDLE_CAPABLE;
    if (HwReducedAcpi) {
      Fadt->Flags |= HW_REDUCED_ACPI;
    }
  }
  return Status;
}
