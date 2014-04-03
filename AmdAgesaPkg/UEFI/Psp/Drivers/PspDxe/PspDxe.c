/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP DXE Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
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
#include "Tiano.h"
#include "EfiDriverLib.h"
#include "EfiDebug.h"
#include "PspBaseLib.h"
#include "PspMbox.h"
#include "PspPlatformProtocol.h"
#include "PspDebug.h"
#include "Filecode.h"

#include EFI_PROTOCOL_CONSUMER   (SmmControl)
#include EFI_PROTOCOL_CONSUMER   (MpService)
#include EFI_GUID_DEFINITION     (EventGroup)

#define FILECODE UEFI_PSP_DRIVERS_PSPDXE_PSPDXE_FILECODE



EFI_SMM_CONTROL_PROTOCOL              *mSmmControl;
PSP_PLATFORM_PROTOCOL                 *mPspPlatfromProtocol;
EFI_GUID gEfiMpServiceProtocolGuid = EFI_MP_SERVICES_PROTOCOL_GUID;

/*++

Routine Description:

  This function is invoked by EFI_EVENT_SIGNAL_LEGACY_BOOT.
  Before booting to legacy OS, inform SMM

Arguments:

  Event   - The triggered event.
  Context - Context for this event.

Returns:

  None

--*/
VOID
PspNotifySmmDrivers (
  VOID
  )
{

  UINT8 Cmd;
  UINTN Size;

  Cmd = mPspPlatfromProtocol->SwSmiCmdtoBuildContext;
  Size = sizeof (Cmd);

  PSP_DEBUG ("PspNotifySmmDrivers Cmd[%x]\n", Cmd);
  mSmmControl->Trigger (mSmmControl, &Cmd, &Size, FALSE, 0);
}

VOID
PSPProgBar3Msr (
  IN  VOID                                     *Buffer
  )
{
  UINT32 Bar3Addr;
  UINT64 Tmp64;
  //Get Bar3 Addr
  Bar3Addr = PspLibPciReadConfig ( (UINT32) (PSP_PCIE_BDA + 0x20));
  Tmp64 = Bar3Addr;
  PSP_DEBUG ("BAR3 Value %x\n", (UINTN) Tmp64);

  PspLibMsrWrite (0xC00110A2, &Tmp64);
  PspLibMsrRead (0xC00110A2, &Tmp64);
  PSP_DEBUG ("PSPMSR BAR Value %lx\n", Tmp64);
}

EFI_STATUS
PspInitBar3 (
  )
{
  EFI_STATUS  Status;
  UINT32      PciReg48;
  VOID        *UnusedBufPtr;
  EFI_MP_SERVICES_PROTOCOL *MpServices;

  PSP_DEBUG ("Locate MP Protocol\n");
  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpServices);

  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  UnusedBufPtr = NULL;
  PSPProgBar3Msr (UnusedBufPtr);
  Status = MpServices->StartupAllAPs (
                        MpServices,                                    // EFI_MP_SERVICES_PROTOCOL *this
                        (EFI_AP_PROCEDURE)PSPProgBar3Msr,              // EFI_AP_PROCEDURE
                        TRUE,                                          // BOOLEAN SingleThreaded? FALSE=execute all functions at the same time on all CPUs
                        NULL,                                          // EFI_EVENT WaitEvent OPTIONAL
                        0,                                             // UINTN Timeout (Unsupported)
                        NULL,                                          // VOID *ProcArguments OPTIONAL
                        NULL                                           // Failed CPUList OPTIONAL (unsupported)
                        );

  PSP_DEBUG ("Locking Bar3\n");
  //Lock Bar3
  PciReg48 = PspLibPciReadConfig ( (UINT32) (PSP_PCIE_BDA + 0x48));
  PciReg48 |= BIT12 + BIT20;
  PspLibPciWriteConfig ( (UINT32) (PSP_PCIE_BDA + 0x48), PciReg48);
  return EFI_SUCCESS;
}

/*++

Routine Description:

  This function is invoked by EFI_EVENT_SIGNAL_LEGACY_BOOT.
  Before booting to legacy OS, inform PSP that BIOS is transitioning
  preparing for S3 usage.

Arguments:

  Event   - The triggered event.
  Context - Context for this event.

Returns:

  None

--*/
VOID
PspDxeReadyToBoot (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS  Status;
  PSP_DEBUG ("PSP.ExitBootServicesEvent.Callback\n");

  //Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  //Always Init Bar3 Related Register
  PspInitBar3 ();

  //If PSP feature turn off, exit the driver
  Status = CheckPspDevicePresent ();
  if (EFI_ERROR (Status)) {
    return;
  }

  PspMboxBiosCmdExitBootServices ();
  PspNotifySmmDrivers ();
}

EFI_STATUS
PspDxeEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_EVENT                   ExitBootServicesEvent;
  EFI_STATUS                  Status;

  gST      = SystemTable;
  gBS      = SystemTable->BootServices;
  gRT      = SystemTable->RuntimeServices;

  PSP_DEBUG ("Psp.Drv.PspDxe Enter\n");


  Status = gBS->LocateProtocol (&gEfiSmmControlProtocolGuid, NULL, &mSmmControl);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return (Status);
  }

  //Locate Psp Platform Protocol to get customized information
  PSP_DEBUG ("\tLocate Psp Platform Protocol\n");
  Status = gBS->LocateProtocol (
                    &gPspPlatformProtocolGuid,
                    NULL,
                    &mPspPlatfromProtocol
                    );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  PSP_DEBUG ("\tCreate ExitBootServicesEvent\n");
  Status = gBS->CreateEventEx (
                  EFI_EVENT_NOTIFY_SIGNAL,
                  EFI_TPL_NOTIFY,
                  PspDxeReadyToBoot,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &ExitBootServicesEvent
                  );

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return (Status);
  }
  PSP_DEBUG ("PspDxe Exit\n");
  return EFI_SUCCESS;
}
