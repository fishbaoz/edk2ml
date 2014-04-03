//* $NoKeywords:$ */
/**
 * @file
 *
 * PSP Mailbox related functions
 *
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
#include "Tiano.h"
#include "PspMbox.h"
#include "EfiCommonLib.h"
#include "PspBaseLib.h"
#include "PspfTpmLib.h"
#include "PspDebug.h"
#include "Filecode.h"

#define FILECODE UEFI_PSP_LIBRARY_PSPFTPMLIB_PSPFTPMLIB_FILECODE

VOID
DumpfTPMControlArea (
  IN       TPM2_CONTROL_AREA        *iTpmControlArea
  )
{
  PSP_DEBUG ("fTPM CmdAddr:0x%x CmdSize:0x%x RespAddr:0x%x RespSize:0x%x\n",
              (UINTN) iTpmControlArea->CommandAddress, iTpmControlArea->CommandSize,
              (UINTN) iTpmControlArea->ResponseAddress, iTpmControlArea->ResponseSize);
}

VOID
DumpfTPMCommandBuffer (
  IN       TPM2_CONTROL_AREA        *iTpmControlArea
  )
{
  UINT32 i;
  UINT8 *BufPtr;

  DumpfTPMControlArea (iTpmControlArea);

  PSP_DEBUG ("Command Buffer:\n");
  BufPtr = (UINT8*) (UINTN) (iTpmControlArea->CommandAddress);

  PSP_DEBUG ("00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15\n");

  for (i = 0; i < 0x80; i++, BufPtr++) {
    PSP_DEBUG ("%02x ", *(BufPtr));
    if ((i != 0) && ((i + 1) % 16 == 0)) {
      PSP_DEBUG ("\n");
    }
  }
  PSP_DEBUG ("\n");
}

VOID
DumpfTPMResponseBuffer (
  IN       TPM2_CONTROL_AREA        *iTpmControlArea
  )
{
  UINT32 i;
  UINT8 *BufPtr;

  DumpfTPMControlArea (iTpmControlArea);

  PSP_DEBUG ("Response Buffer:\n");
  BufPtr = (UINT8*) (UINTN) (iTpmControlArea->ResponseAddress);

  PSP_DEBUG ("00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15\n");

  for (i = 0; i < 0x80; i++, BufPtr++) {
    PSP_DEBUG ("%02x ", *(BufPtr));
    if ((i != 0) && ((i + 1) % 16 == 0)) {
      PSP_DEBUG ("\n");
    }
  }
  PSP_DEBUG ("\n");
}


/**
 *  Set Command,Response buffer registers, Buffer address equal to NULL will skip the setting
 *
 * @param[in]    CommandBuffer              Point to the TPM command buffer
 * @param[in]    CommandSize                Size of the TPM command buffer
 * @param[in]    ResponseBuffer             Point to the TPM response buffer
 * @param[in]    ResponseSize               Size of the TPM response buffer
 *
 * @return       EFI_SUCCESS                Command executed successfully
 * @return       EFI_UNSUPPORTED            Device unsupported
 *
 */

EFI_STATUS
iTpmAssignMemory (
  IN     UINTN                CommandBuffer,
  IN     UINT32               CommandSize,
  IN     UINTN                ResponseBuffer,
  IN     UINT32               ResponseSize
  )
{

  TPM2_CONTROL_AREA        *iTpmControlArea;
  EFI_PHYSICAL_ADDRESS     PspBar1Addr;

  if (EFI_ERROR (
        GetPspBar1Addr (&PspBar1Addr)
        )
      ) {
    return EFI_UNSUPPORTED;
  }
  iTpmControlArea = (TPM2_CONTROL_AREA *) (UINTN) PspBar1Addr;

  if (CommandBuffer) {
    iTpmControlArea->CommandAddress  = CommandBuffer;
    iTpmControlArea->CommandSize     = (UINT32)CommandSize;
  }

  if (ResponseBuffer) {
    iTpmControlArea->ResponseAddress = ResponseBuffer;
    iTpmControlArea->ResponseSize    = (UINT32)ResponseSize;
  }

  return (EFI_SUCCESS);
}

/**
 *  GET TPM related Info
 *
 * @param[in,out] iTPMStatus              Used to hold more detail info (Unused Currently)
 *
 * @return       EFI_SUCCESS              fTPM function supported
 * @return       EFI_UNSUPPORTED          fTPM function unsupported
 *
 */
EFI_STATUS
iTpmGetInfo (
  IN OUT   UINTN                *iTPMStatus
  )
{
  IN TPM2_CONTROL_AREA        *iTpmControlArea;
  EFI_PHYSICAL_ADDRESS        PspBar1Addr;

  if (EFI_ERROR (
        GetPspBar1Addr (&PspBar1Addr)
        )
      ) {
    return EFI_UNSUPPORTED;
  }
  iTpmControlArea = (TPM2_CONTROL_AREA *) (UINTN) PspBar1Addr;

  return (EFI_SUCCESS);
}

/**
 *  Send a TPM command
 *
 * @param[in]    CommandBuffer              Point to the TPM command buffer
 * @param[in]    CommandSize                Size of the TPM command buffer
 *
 * @return       EFI_SUCCESS                Command executed successfully
 * @return       EFI_UNSUPPORTED            Device unsupported
 * @return       EFI_TIMEOUT                Command fail due the time out
 * @return       EFI_DEVICE_ERROR           Command fail due the error status set
 * @return       EFI_BUFFER_TOO_SMALL       Response buffer too small to hold the response
 *
 */
EFI_STATUS
iTpmSendCommand (
  IN     VOID                 *CommandBuffer,
  IN     UINT32                CommandSize
  )
{
  TPM2_CONTROL_AREA    *iTpmControlArea;
  EFI_PHYSICAL_ADDRESS PspBar1Addr;

  PSP_DEBUG ("PSP.fTPM.SendCmd\n");
  //Validate Input parameters
  if ((CommandBuffer == NULL) || (CommandSize == 0)) {
    PSP_DEBUG ("\tInvalid parameters (PSP.fTPM.SendCmd)\n");
    return EFI_INVALID_PARAMETER;
  }

  if (EFI_ERROR (CheckITPMSupported ())) {
    PSP_DEBUG ("\tfTPM Unsupported (PSP.fTPM.SendCmd)\n");
    return EFI_UNSUPPORTED;
  }

  if (EFI_ERROR (
        GetPspBar1Addr (&PspBar1Addr)
        )
      ) {
    PSP_DEBUG ("\tfTPM Bar Error (PSP.fTPM.SendCmd)\n");
    return EFI_UNSUPPORTED;
  }
  iTpmControlArea = (TPM2_CONTROL_AREA *) (UINTN) PspBar1Addr;

  ASSERT (iTpmControlArea->CommandAddress);
  ASSERT (iTpmControlArea->CommandSize);
  EfiCommonLibCopyMem ((VOID *) (UINTN) iTpmControlArea->CommandAddress, CommandBuffer, CommandSize);

  //Alway use the pre-allocated buffer to hold the response
  ASSERT (iTpmControlArea->ResponseAddress);

  iTpmControlArea->ResponseSize =  iTPM_RESPONSE_BUFFER_SIZE;
  DEBUG_CODE (
    DumpfTPMCommandBuffer (iTpmControlArea);
  )

  EfiCommonLibSetMem ((VOID *) (UINTN) iTpmControlArea->ResponseAddress, iTpmControlArea->ResponseSize, 0x0);

  //Dump fTPM Control Area
  //Issue the command
  iTpmControlArea->StatusStart = 1;

  // wait for fTPM to finish
  if (EFI_ERROR (iTpmWaitStatus ())) {
    PSP_DEBUG ("\n Command time out (PSP.fTPM.SendCmd)\n");
    return EFI_TIMEOUT;
  }

  if (iTpmControlArea->StatusError) {
    PSP_DEBUG ("\n Command error (PSP.fTPM.SendCmd)\n");
    return (EFI_DEVICE_ERROR);
  }

  return (EFI_SUCCESS);

}

/**
 *  Wait for a TPM command's completeness
 *
 *
 * @return       EFI_SUCCESS                Command executed successfully
 * @return       EFI_UNSUPPORTED            Device unsupported
 * @return       EFI_TIMEOUT                Command fail due the time out
 *
 */
EFI_STATUS
iTpmWaitStatus (
  )
{
  TPM2_CONTROL_AREA        *iTpmControlArea;
  EFI_PHYSICAL_ADDRESS        PspBar1Addr;

  if (EFI_ERROR (
        GetPspBar1Addr (&PspBar1Addr)
        )
      ) {
    return EFI_UNSUPPORTED;
  }
  iTpmControlArea = (TPM2_CONTROL_AREA *) (UINTN) PspBar1Addr;

  while ( (iTpmControlArea->StatusStart) &&
    !(iTpmControlArea->StatusError));

  return EFI_SUCCESS;
}

/**
 *  Get a TPM command's response
 *
 * @param[in]    ResponseBuffer             Point to the TPM response buffer
 * @param[in]    ResponseSize               Size of the TPM response buffer
 *
 * @return       EFI_SUCCESS                Command executed successfully
 * @return       EFI_UNSUPPORTED            Device unsupported
 * @return       EFI_TIMEOUT                Command fail due the time out
 * @return       EFI_DEVICE_ERROR           Command fail due the error status set
 * @return       EFI_BUFFER_TOO_SMALL       Response buffer too small to hold the response
 *
 */
EFI_STATUS
iTpmGetResponse (
  IN OUT   VOID                  *ResponseBuffer,
  IN OUT   UINT32                *ResponseSize
  )
{
  TPM2_CONTROL_AREA *iTpmControlArea;
  EFI_PHYSICAL_ADDRESS        PspBar1Addr;

  PSP_DEBUG ("PSP.fTPM.GetResponse\n");
  //Validate Input parameters
  if ((ResponseBuffer == NULL) || (ResponseSize == NULL) || (*ResponseSize == 0)) {
    PSP_DEBUG ("\tInvalid parameters (PSP.fTPM.GetResponse)\n");
    return EFI_INVALID_PARAMETER;
  }

  if (EFI_ERROR (
        GetPspBar1Addr (&PspBar1Addr)
        )
      ) {
    return EFI_UNSUPPORTED;
  }
  iTpmControlArea = (TPM2_CONTROL_AREA *) (UINTN) PspBar1Addr;


  ASSERT (iTpmControlArea->ResponseAddress);
  if (iTpmControlArea->ResponseAddress == 0) {
    PSP_DEBUG ("\tResponse Buffer Null (PSP.fTPM.GetResponse)\n");
    DumpfTPMControlArea (iTpmControlArea);
    return EFI_UNSUPPORTED;
  }

  // wait for fTPM to finish
  if (EFI_ERROR (iTpmWaitStatus ())) {
    PSP_DEBUG ("\n Command time out (PSP.fTPM.GetResponse)\n");
    return EFI_TIMEOUT;
  }

  if (iTpmControlArea->StatusError) {
    PSP_DEBUG ("\n Command error (PSP.fTPM.GetResponse)\n");
    return (EFI_DEVICE_ERROR);
  }

  DEBUG_CODE (
    DumpfTPMResponseBuffer (iTpmControlArea);
  )

  if (iTpmControlArea->ResponseSize) {
    EfiCommonLibCopyMem ( ResponseBuffer, (VOID *) (UINTN)iTpmControlArea->ResponseAddress, *ResponseSize);
  }
  iTpmControlArea->ResponseSize = iTPM_RESPONSE_BUFFER_SIZE;

  return (EFI_SUCCESS);
}

/**
 *  Execute a TPM command
 *
 * @param[in]    CommandBuffer              Point to the TPM command buffer
 * @param[in]    CommandSize                Size of the TPM command buffer
 * @param[in]    ResponseBuffer             Point to the TPM response buffer
 * @param[in]    ResponseSize               Size of the TPM response buffer
 *
 * @return       EFI_SUCCESS                Command executed successfully
 * @return       EFI_UNSUPPORTED            Device unsupported
 * @return       EFI_TIMEOUT                Command fail due the time out
 * @return       EFI_DEVICE_ERROR           Command fail due the error status set
 * @return       EFI_BUFFER_TOO_SMALL       Response buffer too small to hold the response
 *
 */
EFI_STATUS
iTpmExecuteCommand (
  IN       VOID                 *CommandBuffer,
  IN       UINT32               CommandSize,
  IN OUT   VOID                 *ResponseBuffer,
  IN OUT   UINT32               *ResponseSize
  )
{
  EFI_STATUS Status;
  PSP_DEBUG ("PSP.fTPM Execute Cmd\n");
  Status = iTpmSendCommand (CommandBuffer, CommandSize);
  if (EFI_ERROR (Status)) {
    PSP_DEBUG ("\tSend Command Error (PSP.fTPM Execute Cmd)\n");
    return Status;
  }

  Status = iTpmGetResponse (ResponseBuffer, ResponseSize);
  if (EFI_ERROR (Status)) {
    PSP_DEBUG ("\tSend Response Error (PSP.fTPM Execute Cmd)\n");
    return Status;
  }
  return (EFI_SUCCESS);
}

