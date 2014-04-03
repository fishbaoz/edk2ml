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
#include "FchAcpiCs.h"
#include "Filecode.h"
#define FILECODE UEFI_FCH_AVALON_DXE_FCHACPICS_FCHACPICS_FILECODE

EFI_GUID gAmdFchAcpiGuid   = AMD_FCH_ACPI_GUID;

AMD_FCH_COMMON_DATA *mFchCommonData;

EFI_DRIVER_ENTRY_POINT (FchAcpiCsInit);
/*********************************************************************************
 * Name: FchAcpiCsInit
 *
 * Description
 *   Entry point of the AMD FCH ACPI driver
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
FchAcpiCsInit (
  IN    EFI_HANDLE                  ImageHandle,
  IN    EFI_SYSTEM_TABLE            *SystemTable
  )
{
  VOID                              *Registration;
  EFI_STATUS                        Status;
  EFI_EVENT                         Event;

  DxeInitializeDriverLib (ImageHandle, SystemTable);

  Status = LibFchDxeInitialization (SystemTable);

  // Allocate memory for the private data
  Status = gBS->AllocatePool (
                  EfiReservedMemoryType,
                  sizeof (AMD_FCH_COMMON_DATA),
                  &mFchCommonData
                  );
  ASSERT_EFI_ERROR (Status);

  // Initialize the Config table with default value
  Status = FchInitDefaultConfigDxe (mFchCommonData);

  //Register event to do FCH ACPI Initialization at Ready-to-Boot
  Status = EfiCreateEventReadyToBoot (
             EFI_TPL_CALLBACK,
             FchAcpiReadyToBootInit,
             NULL,
             &Event
             );

  //
  // Register event for FchAcpiInstallNotify
  //
  EfiLibCreateProtocolNotifyEvent (
    &gEfiAcpiSdtProtocolGuid,
    EFI_TPL_CALLBACK,
    FchAcpiInstallNotify,
    NULL,
    &Registration
    );

  //
  // Register event for FchAcpiInstallNotify
  //
  EfiLibCreateProtocolNotifyEvent (
    &gEfiPciIoProtocolGuid,
    EFI_TPL_CALLBACK,
    FchAcpiInstall2Notify,
    NULL,
    &Registration
    );

  return Status;

}

/*********************************************************************************
 * Name: FchInitDefaultConfigDxe
 *
 * Description
 *
 * Input
 *   FchData : pointer to the SB DXE driver private data structure
 *
 * Output
 *   EFI_SUCCESS : initialization completed successfully
 *
 *********************************************************************************/
EFI_STATUS
FchInitDefaultConfigDxe (
  IN OUT   AMD_FCH_COMMON_DATA  *FchData
  )
{
  //
  // Clear all parameters
  //
  gBS->SetMem (
         FchData,
         sizeof (AMD_FCH_COMMON_DATA),
         0
         );
  FchData->Signature = EFI_SIGNATURE_32 ('F', 'C', 'H', '_');

  return EFI_SUCCESS;
}

VOID
EFIAPI
FchAcpiInstallNotify (
  IN EFI_EVENT                              Event,
  IN VOID                                   *Context
  )
{
  EFI_STATUS                                Status;
  EFI_ACPI_SDT_PROTOCOL                     *AcpiSdtProtocol;

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
                              FchAcpiInstall
                              );
  ASSERT (!EFI_ERROR (Status));

  return;
}

VOID
EFIAPI
FchAcpiInstall2Notify (
  IN EFI_EVENT                              Event,
  IN VOID                                   *Context
  )
{
  UINT8                                     *Temp;
  UINT8                                     *AmdAcpiDataTree;
  UINTN                                     Index;
  UINTN                                     TableHandle;
  EFI_STATUS                                Status;
  EFI_HANDLE                                Handle;
  EFI_ACPI_DESCRIPTION_HEADER               *Table;
  EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE *Fadt;
  EFI_ACPI_TABLE_VERSION                    Version;
  EFI_ACPI_SUPPORT_PROTOCOL                 *AcpiSupport;
  FCH_ACPI_PROTOCOL                         *FchAcpiProtocol;
  EFI_PCI_IO_PROTOCOL                       *PciIo;

  Status = EFI_SUCCESS;
  AmdAcpiDataTree = NULL;

  Status = gBS->LocateProtocol (
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &PciIo
                  );

  if (EFI_ERROR (Status)) {
    return;
  }

  Status = gBS->LocateProtocol (
                  &gFchAcpiProtocolGuid,
                  NULL,
                  &FchAcpiProtocol
                  );

  if (!EFI_ERROR (Status)) {
    return;
  }

  Status = gBS->LocateProtocol (
                  &gEfiAcpiSupportGuid,
                  NULL,
                  &AcpiSupport
                  );

  if (EFI_ERROR (Status)) {
    return;
  }

  Index  = 0;
  TableHandle = 0;
  do {
    Table  = NULL;
    Status = AcpiSupport->GetAcpiTable (
                            AcpiSupport,
                            Index,
                            &Table,
                            &Version,
                            &TableHandle
                            );
    if (EFI_ERROR (Status)) {
      break;
    }
    //
    // Check Signture and update DSDT table
    //
    if (Table->Signature == EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
      Status = AmdBuildAcpiDsdtTree ((EFI_ACPI_DESCRIPTION_HEADER *)Table, &AmdAcpiDataTree);
      if (EFI_ERROR (Status)) {
        return;
      }

      Status = gBS->AllocatePool (
                      EfiBootServicesData,
                      0x01,
                      &Temp
                      );
      ASSERT_EFI_ERROR (Status);

      *Temp = FCH_ACPI_FIELD_INFO_END;

      Status = gBS->AllocatePool (
                      EfiBootServicesData,
                      sizeof (FCH_ACPI_PROTOCOL),
                      &FchAcpiProtocol
                      );
      ASSERT_EFI_ERROR (Status);

      FchAcpiProtocol->Revision                      = FCH_ACPI_PROTOCOL_REV;
      FchAcpiProtocol->FchAcpiGetIntDevPathName      = AmdFchGetIntDevPath;
      FchAcpiProtocol->FchAcpiAddDataField           = AmdFchDataDxeAddItem;
      FchAcpiProtocol->FchAcpiEnableGpio             = AmdFchAcpiEnableGpio;
      FchAcpiProtocol->FchAcpiEnableAsf              = AmdFchAcpiEnableAsf;
      FchAcpiProtocol->FchAcpiTool.FindNextSig       = FchFindNextSig;
      FchAcpiProtocol->FchAcpiTool.UpdateAcpiName    = UpdateAcpiName;
      FchAcpiProtocol->FchAcpiTool.GetPkgLength      = FchAcpiPkgLength;
      FchAcpiProtocol->FchAcpiData.DataLength        = sizeof (AMD_FCH_COMMON_DATA);
      FchAcpiProtocol->FchAcpiData.AcpiInfoLength    = 0x01;
      FchAcpiProtocol->FchAcpiData.FchData           = mFchCommonData;
      FchAcpiProtocol->FchAcpiData.AcpiFieldNames    = Temp;

      Handle = NULL;
      Status = gBS->InstallProtocolInterface (
                      &Handle,
                      &gFchAcpiProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      FchAcpiProtocol
                      );
      if (EFI_ERROR (Status)) {
        return;
      }
    }

    //
    // Check Signture and update FADT table
    //
    if (Table->Signature == EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE) {
      Fadt = (EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE *)Table;
      mFchCommonData->SciInterrupt = Fadt->SciInt;
    }

    gBS->FreePool (Table);
    Index++;
  } while (TRUE);

}


EFI_STATUS
EFIAPI
FchAcpiInstall (
  IN EFI_ACPI_SDT_HEADER    *Table,
  IN EFI_ACPI_TABLE_VERSION Version,
  IN UINTN                  TableKey
  )
{
  UINT8                                     *Temp;
  UINT8                                     *AmdAcpiDataTree;
  EFI_STATUS                                Status;
  EFI_HANDLE                                Handle;
  FCH_ACPI_PROTOCOL                         *FchAcpiProtocol;
  EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE *Fadt;

  Status = EFI_SUCCESS;
  AmdAcpiDataTree = NULL;

  Status = gBS->LocateProtocol (
                  &gFchAcpiProtocolGuid,
                  NULL,
                  &FchAcpiProtocol
                  );

  if (!EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Search DSDT table
  //
  if (Table->Signature == EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
    Status = AmdBuildAcpiDsdtTree ((EFI_ACPI_DESCRIPTION_HEADER *)Table, &AmdAcpiDataTree);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    0x01,
                    &Temp
                    );
    ASSERT_EFI_ERROR (Status);

    *Temp = FCH_ACPI_FIELD_INFO_END;

    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    sizeof (FCH_ACPI_PROTOCOL),
                    &FchAcpiProtocol
                    );
    ASSERT_EFI_ERROR (Status);

    FchAcpiProtocol->Revision                      = FCH_ACPI_PROTOCOL_REV;
    FchAcpiProtocol->FchAcpiGetIntDevPathName      = AmdFchGetIntDevPath;
    FchAcpiProtocol->FchAcpiAddDataField           = AmdFchDataDxeAddItem;
    FchAcpiProtocol->FchAcpiEnableGpio             = AmdFchAcpiEnableGpio;
    FchAcpiProtocol->FchAcpiEnableAsf              = AmdFchAcpiEnableAsf;
    FchAcpiProtocol->FchAcpiTool.FindNextSig       = FchFindNextSig;
    FchAcpiProtocol->FchAcpiTool.UpdateAcpiName    = UpdateAcpiName;
    FchAcpiProtocol->FchAcpiTool.GetPkgLength      = FchAcpiPkgLength;
    FchAcpiProtocol->FchAcpiData.DataLength        = sizeof (AMD_FCH_COMMON_DATA);
    FchAcpiProtocol->FchAcpiData.AcpiInfoLength    = 0x01;
    FchAcpiProtocol->FchAcpiData.FchData           = mFchCommonData;
    FchAcpiProtocol->FchAcpiData.AcpiFieldNames    = Temp;

    Handle = NULL;
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gFchAcpiProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    FchAcpiProtocol
                    );

    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  //
  // Check Signture and update FADT table
  //
  if (Table->Signature == EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE) {
    Fadt = (EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE *)Table;
    mFchCommonData->SciInterrupt = Fadt->SciInt;
  }
  return Status;
}

VOID
EFIAPI
FchAcpiReadyToBootInit (
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
  EFI_HANDLE                                Handle;
  EFI_FV_FILETYPE                           FileType;
  EFI_FV_FILE_ATTRIBUTES                    Attributes;
  EFI_ACPI_COMMON_HEADER                    *CurrentTable;
  EFI_ACPI_DESCRIPTION_HEADER               *FchAcpiBlockPtr  = NULL;

  EFI_ACPI_TABLE_PROTOCOL                   *AcpiTableProtocol;
  FCH_ACPI_PROTOCOL                         *FchAcpiProtocol;
  EFI_FIRMWARE_VOLUME_PROTOCOL              *FwVol = NULL;

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

  FvStatus = 0;
  //
  // Locate protocol.
  //
  Status = gBS->LocateHandleBuffer (
                   ByProtocol,
                   &gEfiFirmwareVolumeProtocolGuid,
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
                     &gEfiFirmwareVolumeProtocolGuid,
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
                      &gAmdFchAcpiGuid,
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
                      &gAmdFchAcpiGuid,
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
      if (((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId == EFI_SIGNATURE_64 ('F', 'C', 'H', 'A', 'C', 'P', 'I', 0)) {
        FchAcpiBlockPtr = (EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable;

        Status = FchUpdateAcpiDataTable (FchAcpiProtocol, &FchAcpiBlockPtr);

        TableHandle = 0;
        TableSize = FchAcpiBlockPtr->Length;

        //
        // Install ACPI table
        //
        Status = AcpiTableProtocol->InstallAcpiTable (
                                      AcpiTableProtocol,
                                      FchAcpiBlockPtr,
                                      TableSize,
                                      &TableHandle
                                      );

        //
        // Free memory allocated by ReadSection
        //
        gBS->FreePool (FchAcpiBlockPtr);

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

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gFchAcpiTableInstallGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );

  if (EFI_ERROR (Status)) {
    return;
  }
}
