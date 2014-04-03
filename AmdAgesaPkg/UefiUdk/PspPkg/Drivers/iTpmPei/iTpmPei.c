/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU POST API, and related functions.
 *
 * Contains code that initialized the CPU after memory init.
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
/*++
Module Name:

  PspPei.c
  Init PSP device
  Intercept MemoryDiscovery protocol and
    a) Send mailbox to PSP to inform the DRAM information

Abstract:
--*/

#include "PiPei.h"
#include <Library/BaseMemoryLib.h>
#include <Library/PspBaseLib.h>
#include <Library/PspMboxLib.h>
#include <Library/PspfTpmLib.h>
#include <Ppi/iTpmPpi.h>
#include "PspDebug.h"
#include <Ppi/MemoryDiscovered.h>


EFI_STATUS
EFIAPI
iTpmMemoryDiscoveredPpiCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  );

EFI_STATUS
GetfTpmStatus (
  IN       PSP_iTPM_PPI         *This,
  IN OUT   UINTN                *iTPMStatus
  )
{
  return (iTpmGetInfo (iTPMStatus));
}

EFI_STATUS
SendfTpmCommand (
  IN     PSP_iTPM_PPI         *This,
  IN     VOID                 *CommandBuffer,
  IN     UINTN                 CommandSize
  )
{
  return iTpmSendCommand (
      CommandBuffer,
      CommandSize);
}

EFI_STATUS
GetfTpmResponse (
  IN       PSP_iTPM_PPI          *This,
  IN OUT   VOID                  *ResponseBuffer,
  IN OUT   UINTN                 *ResponseSize
  )
{

  EFI_STATUS Status;
  Status = iTpmGetResponse (
                            ResponseBuffer,
                            ResponseSize
                          );
  return (Status);
}

EFI_STATUS
ExecutefTpmCommand (
  IN       PSP_iTPM_PPI         *This,
  IN       VOID                 *CommandBuffer,
  IN       UINTN                CommandSize,
  IN OUT   VOID                 *ResponseBuffer,
  IN OUT   UINTN                *ResponseSize
  )
{
  return iTpmExecuteCommand (
                              CommandBuffer,
                              CommandSize,
                              ResponseBuffer,
                              ResponseSize
                             );
}


STATIC PSP_iTPM_PPI mTpmPpi = {
  ExecutefTpmCommand,
  GetfTpmStatus,
  SendfTpmCommand,
  GetfTpmResponse,
};


STATIC EFI_PEI_PPI_DESCRIPTOR mTpmPpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &giTpmPpiGuid,
  &mTpmPpi
};

EFI_PEI_NOTIFY_DESCRIPTOR   mMemoryDicoverPpiCallback = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  iTpmMemoryDiscoveredPpiCallback
};

EFI_STATUS
EFIAPI
iTpmMemoryDiscoveredPpiCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
{
  EFI_STATUS                  Status;
  UINTN                       *iTpmCommandBuffer;
  UINTN                       *iTpmResponseBuffer;

  Status = (*PeiServices)->AllocatePages (
                              PeiServices,
                              EfiReservedMemoryType, //EfiConventionalMemory
                              EFI_SIZE_TO_PAGES (iTPM_COMMAND_BUFFER_SIZE + \
                                                  iTPM_RESPONSE_BUFFER_SIZE),
                              (EFI_PHYSICAL_ADDRESS *)&iTpmCommandBuffer
                              );

  if (EFI_ERROR (Status)) {
    PSP_DEBUG ("Allocate Memory fail\n");
    return Status;
  }

  iTpmResponseBuffer      = (UINTN *) ( (UINTN)iTpmCommandBuffer + iTPM_COMMAND_BUFFER_SIZE);

  SetMem (iTpmCommandBuffer, iTPM_COMMAND_BUFFER_SIZE + iTPM_RESPONSE_BUFFER_SIZE, 0);
  //Send PSP mailbox command to ensure Ctrl Area have been initialed
  CheckITPMSupported ();

  Status = iTpmAssignMemory (
    (UINTN)iTpmCommandBuffer,
    iTPM_COMMAND_BUFFER_SIZE,
    (UINTN)iTpmResponseBuffer,
    iTPM_RESPONSE_BUFFER_SIZE
  );
  PSP_DEBUG ("Psp.iTpmPei.MemoryDiscoveredPpiCallback\n");
  PSP_DEBUG ("\tAllocate CmdBuf:0x%x ResBuf:0x%x\n", iTpmCommandBuffer, iTpmResponseBuffer);

  return Status;
}

/*++

Routine Description:

  PSP Driver Entry. Initialize PSP device and report DRAM info to PSP once found

Arguments:

Returns:

  EFI_STATUS

--*/
EFI_STATUS
PeiiTpmDriverEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_BOOT_MODE         BootMode;
  EFI_STATUS            Status = EFI_SUCCESS;

  PSP_DEBUG ("Psp.Drv.iTpmPei Enter\n");
  //If PSP feature turn off, exit the driver
  Status = CheckPspDevicePresent ();
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Check if Boot mode is boot then hook for MemoryDiscovery protocol
  //
  Status = (*PeiServices)->GetBootMode (
             PeiServices,
             &BootMode
             );
  if (BootMode != BOOT_ON_S3_RESUME) {
    Status = (**PeiServices).NotifyPpi (PeiServices, &mMemoryDicoverPpiCallback);
  }
  PSP_DEBUG ("\tInstall PspTis PPI\n");
  Status = (**PeiServices).InstallPpi (PeiServices, &mTpmPpiList);
  PSP_DEBUG ("iTpmPei Exit\n");
  return Status;
}

