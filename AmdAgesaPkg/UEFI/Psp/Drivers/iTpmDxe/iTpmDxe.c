/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP TPM related functions in DXE
 *
 * Contains PSP TPM interface
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
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

#include "Tiano.h"
#include "EfiDriverLib.h"
#include "PspBaseLib.h"
#include "PspMbox.h"
#include "PspfTpmLib.h"
#include "iTpmProtocol.h"
#include "PspDebug.h"
#include "Filecode.h"

#define FILECODE UEFI_PSP_DRIVERS_ITPMDXE_ITPMDXE_FILECODE

EFI_STATUS
GetfTpmStatus (
  IN       PSP_iTPM_PROTOCOL    *This,
  IN OUT   UINTN                *iTPMStatus
  )
{

  return (iTpmGetInfo (iTPMStatus));
}

EFI_STATUS
SendfTpmCommand (
  IN     PSP_iTPM_PROTOCOL    *This,
  IN     VOID                 *CommandBuffer,
  IN     UINT32               CommandSize
  )
{
  return iTpmSendCommand (
      CommandBuffer,
      CommandSize);

}

EFI_STATUS
GetfTpmResponse (
  IN       PSP_iTPM_PROTOCOL     *This,
  IN OUT   VOID                  *ResponseBuffer,
  IN OUT   UINT32                *ResponseSize
  )
{
  EFI_STATUS Status;

  if (!ResponseBuffer) {
    return (EFI_NOT_FOUND);
  }

  Status =  iTpmGetResponse (
                            ResponseBuffer,
                            ResponseSize
                          );
  return (Status);
}

EFI_STATUS
ExecutefTpmCommand (
  IN       PSP_iTPM_PROTOCOL    *This,
  IN       VOID                 *CommandBuffer,
  IN       UINT32                CommandSize,
  IN OUT   VOID                 *ResponseBuffer,
  IN OUT   UINT32               *ResponseSize
  )
{
  return iTpmExecuteCommand (
                                CommandBuffer,
                                CommandSize,
                                ResponseBuffer,
                                ResponseSize
                               );
}


PSP_iTPM_PROTOCOL iTpmInfoProtocol = {
  ExecutefTpmCommand,
  GetfTpmStatus,
  SendfTpmCommand,
  GetfTpmResponse,
};

EFI_STATUS
iTpm2EntryPoint (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )

{
  EFI_STATUS                  Status;
  EFI_HANDLE                  Handle;
  UINTN                       AllocateSize;
  UINTN                       CommandBuffer;
  UINTN                       ResponseBuffer;

  //If PSP feature turn off, exit the driver
  Status = CheckPspDevicePresent ();
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  gST = SystemTable;
  gBS = SystemTable->BootServices;
  gRT = SystemTable->RuntimeServices;

  CommandBuffer = 0;
  ResponseBuffer = 0;

  PSP_DEBUG ("Psp.Drv.iTpmDxe Enter\n");

  //
  // Allocate pages for TPM Control area (as well as command & response buffer)
  AllocateSize =  ( iTPM_COMMAND_BUFFER_SIZE  + iTPM_RESPONSE_BUFFER_SIZE );

  Status = gBS->AllocatePages (
                  AllocateAnyPages,
                  EfiACPIMemoryNVS,
                  EFI_SIZE_TO_PAGES (AllocateSize),
                  (EFI_PHYSICAL_ADDRESS *)&CommandBuffer);

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  gBS->SetMem ((VOID *)CommandBuffer, AllocateSize, 0);
  ResponseBuffer = (UINTN)CommandBuffer + iTPM_COMMAND_BUFFER_SIZE;
  Status = iTpmAssignMemory (
    (UINTN)CommandBuffer,
    iTPM_COMMAND_BUFFER_SIZE,
    (UINTN)ResponseBuffer,
    iTPM_RESPONSE_BUFFER_SIZE
  );
  PSP_DEBUG ("\tAllocate CmdBuf:0x%x ResBuf:0x%x\n", CommandBuffer, ResponseBuffer);


  //
  // Install protocol to pass iTPM info to rest of BIOS drivers
  //
  Handle = NULL;
  PSP_DEBUG ("\tInstall PspTis protocol\n");
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &giTpmProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &iTpmInfoProtocol
                  );

  if (EFI_ERROR (Status)) {
    return (Status);
  }

  PSP_DEBUG ("iTpmDxe Exit\n");
  return Status;
}
