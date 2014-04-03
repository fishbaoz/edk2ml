/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP TPM ACPI related functions
 *
 * Contains PSP TPM interface
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 282908 $   @e \$Date: 2014-01-20 00:54:02 -0600 (Mon, 20 Jan 2014) $
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
 *
 ***************************************************************************/

#include <PiDxe.h>
#include "PspDebug.h"
//fTPM ACPI Object (AML code binary)
#include "fTPMAmlData.h"
#include "fTPMAcpi.h"
#include <Guid/EventGroup.h>
#include <Library/PspBaseLib.h>
#include <Library/PspfTpmLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/AcpiSupport.h>
#include <Protocol/AcpiSystemDescriptionTable.h>

EFI_EVENT                   mfTPMAcpiEventHandle;

STATIC TPM2_ACPI_TABLE  Tpm2AcpiTable = {
  {
    SIGNATURE_32 ('T','P','M','2'),
    sizeof (TPM2_ACPI_TABLE),
    0x03,                             // ACPI Revision.
    0x00,
    TPM2_ACPI_OEM_ID,                 // OEM ID (filled in below).
    TPM2_ACPI_TABLE_OEM_ID,           // OEM Table ID (filled in below).
    TPM2_ACPI_OEM_REVISION ,          // ACPI OEM Revision (filled in below).
    1,                                // OEM Creator ID (filled in below).
    1                                 // OEM Creator Revision (filled in below).
   },
   0,                                 // Flags
   0,                                 // ControlArea
   0x02                               // StartMethod
};

VOID
EFIAPI
fTPMAcpiEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS                  Status;
  EFI_ACPI_SUPPORT_PROTOCOL   *AcpiSupport;
  UINTN                       TableHandle;
  TPM2_CONTROL_AREA           *Tpm2ControlArea;
  UINT8                       *Tpm2AcpiDataPtr;
  UINT32                      *Memory32Fixed;
  EFI_ACPI_TABLE_VERSION      Version;
  EFI_PHYSICAL_ADDRESS        PspBar1Addr;
  gBS->CloseEvent (Event);
//
// Locate ACPISupport table. Bail if absent
//
  Status = gBS->LocateProtocol (
                  &gEfiAcpiSupportProtocolGuid,
                  NULL,
                  &AcpiSupport);
  if (EFI_ERROR (Status)) {
    return;
  }

  if (GetPspBar1Addr (&PspBar1Addr)) {
    return;
  }
  Tpm2ControlArea = (TPM2_CONTROL_AREA *) (UINTN) PspBar1Addr;
  // Update the TPM ACPI Table for ControlArea location
  Tpm2AcpiTable.ControlArea = (EFI_PHYSICAL_ADDRESS) Tpm2ControlArea + 0x10;

  //
  // Install the ACPI Table
  //
  PSP_DEBUG ("\tInstall ACPI TPM2 Table\n");
  TableHandle = 0;
  Version = (EFI_ACPI_TABLE_VERSION_1_0B | EFI_ACPI_TABLE_VERSION_2_0 | EFI_ACPI_TABLE_VERSION_3_0 |
             EFI_ACPI_TABLE_VERSION_4_0) ;
  Status = AcpiSupport->SetAcpiTable (
                  AcpiSupport,
                  &Tpm2AcpiTable,
                  TRUE,
                  Version,
                  &TableHandle
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  PSP_DEBUG ("\tUpdate _CRS Object with actual value\n");

  //Update _CRS Object with actual value
  for (Tpm2AcpiDataPtr = ((UINT8 *)fTPMAmlData + sizeof (EFI_ACPI_DESCRIPTION_HEADER));
       Tpm2AcpiDataPtr <= ((UINT8 *)fTPMAmlData + ((EFI_ACPI_DESCRIPTION_HEADER *)fTPMAmlData)->Length);
       Tpm2AcpiDataPtr++
       ) {
    Memory32Fixed = (UINT32 *)Tpm2AcpiDataPtr;
    switch (*Memory32Fixed) {
    //TPM2.0 Command Buffer allocate by BIOS, should be updated during POST
    //Memory32Fixed (ReadWrite, 0xBBBBBBBB, 0x100000)
    case 0xBBBBBBBB:
      *Memory32Fixed = (UINT32) (Tpm2ControlArea->CommandAddress);
      PSP_DEBUG ("Tpm2ControlArea->CommandAddress %x\n", Tpm2ControlArea->CommandAddress);
      Tpm2AcpiDataPtr += (sizeof (UINT32) - 1);
      break;
    //TPM2.0 Response Buffer allocate by BIOS, should be updated during POST
    //Memory32Fixed (ReadWrite, 0xCCCCCCCC, 0x100000)
    case 0xCCCCCCCC:
      *Memory32Fixed = (UINT32) (Tpm2ControlArea->ResponseAddress);
      PSP_DEBUG ("Tpm2ControlArea->ResponseAddress %x\n", Tpm2ControlArea->ResponseAddress);
      Tpm2AcpiDataPtr += (sizeof (UINT32) - 1);
      break;
    default:
      break;
    }
  }
  PSP_DEBUG ("\tInstall Tpm SSDT table\n");

  TableHandle = 0;
  AcpiSupport->SetAcpiTable (
                 AcpiSupport,
                 fTPMAmlData,
                 TRUE,
                 Version,
                 &TableHandle
                 );
  if (EFI_ERROR (Status)) {
    return;
  }
  PSP_DEBUG ("PublishTables ACPI table\n");

  Status = AcpiSupport->PublishTables (AcpiSupport, Version);
  if (EFI_ERROR (Status)) {
    return;
  }

  PSP_DEBUG ("fTPMAcpiEvent exit\n");

}

EFI_STATUS
fTpmAcpiEntryPoint (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )

{
  EFI_STATUS                  Status;

  //If PSP feature turn off, exit the driver
  Status = CheckPspDevicePresent ();
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  PSP_DEBUG ("Psp.Drv.fTpmAcpiEntryPoint Enter\n");

  mfTPMAcpiEventHandle = NULL;
  //Right after gEfiPciEnumerationCompleteProtocolGuid, the PCIE MMIO decode haven't
  //been enabled,create the event at readytoboot

  Status = gBS->CreateEventEx (
             EVT_NOTIFY_SIGNAL,
             TPL_NOTIFY,
             fTPMAcpiEvent,
             NULL,
             &gEfiEventReadyToBootGuid,
             &mfTPMAcpiEventHandle
             );

  PSP_DEBUG ("fTpmAcpiEntryPoint Exit\n");
  return Status;
}
