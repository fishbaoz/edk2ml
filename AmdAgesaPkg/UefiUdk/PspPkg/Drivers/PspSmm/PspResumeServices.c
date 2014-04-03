/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP related functions in SMM
 *
 * Contains PSP SMM Resume services
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

#include "PspSmm.h"
#include "PspDebug.h"
#include "PspResumeServices.h"

LIST_ENTRY   PspHeadList;

PSP_RESUME_SERVICE_PROTOCOL PspResumeProtocol = {
  PspRegister,
  PspUnregister,
};

EFI_STATUS
EFIAPI
InstallPspResumeCallbackProtocol (
  )
{
  EFI_STATUS        Status;
  EFI_HANDLE        Handle;

  InitializeListHead (&PspHeadList);

  Handle = NULL;
  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gPspResumeServiceProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &PspResumeProtocol
                    );

  return Status;
}

//
// Add callback note in terms with priority. Highest first
//
VOID
EFIAPI
AddCallbackNode (
  IN  PSP_SMM_CALLBACK_NODE *NewNode
  )
{
  LIST_ENTRY          *Node;

  if (IsListEmpty (&PspHeadList)) {
    InsertTailList (&PspHeadList, &(NewNode->ListEntry));
    return;
  }

 for (Node = GetFirstNode (&PspHeadList);
      Node != & PspHeadList;
      Node = GetNextNode (&PspHeadList, Node)) {

    if (NewNode->CallbackPriority >= ((PSP_SMM_CALLBACK_NODE *)Node)->CallbackPriority) {
      InsertHeadList (Node->BackLink, &(NewNode->ListEntry));
      return;
    }
  }

  InsertTailList (&PspHeadList, &(NewNode->ListEntry));

  return;
}

EFI_STATUS
EFIAPI
PspRegister (
  IN       PSP_RESUME_SERVICE_PROTOCOL        *This,
  IN       PSP_RESUME_CALLBACK                CallBackFunction,
  IN OUT   VOID                               *Context,
  IN       UINTN                             CallbackPriority,
     OUT   EFI_HANDLE                         *DispatchHandle
  )
{
  EFI_STATUS              Status;
  PSP_SMM_CALLBACK_NODE   *NewPspNode;

  Status = EFI_OUT_OF_RESOURCES;

  if (CallBackFunction == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gSmst->SmmAllocatePool (
                       EfiRuntimeServicesData,
                       sizeof (PSP_SMM_CALLBACK_NODE),
                       &NewPspNode
                       );

  if (EFI_ERROR (Status)) {
    return (Status);
  }

  NewPspNode->CallBackFunction   = CallBackFunction;
  NewPspNode->Context            = Context;
  NewPspNode->CallbackPriority   = CallbackPriority;
  InitializeListHead (&(NewPspNode->ListEntry));

  AddCallbackNode (NewPspNode);
  if (DispatchHandle != NULL) {
    *DispatchHandle                = (EFI_HANDLE)NewPspNode;
  }
  return  EFI_SUCCESS;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Unregister child handler
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       DispatchHandle
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
PspUnregister (
  IN       PSP_RESUME_SERVICE_PROTOCOL  *This,
  IN       EFI_HANDLE                   DispatchHandle
  )
{
  EFI_STATUS  Status;
  Status = EFI_NOT_FOUND;

  RemoveEntryList (DispatchHandle);

  gSmst->SmmFreePool (DispatchHandle);
  return  Status;
}

EFI_STATUS
EFIAPI
PspResumeServiceCallBack (
  IN       UINT8 ResumeType
   )
{
  LIST_ENTRY   *Node;
  CHAR8 *ResumeTypeStr;

  ResumeTypeStr = ((ResumeType == ResumeFromConnectedStandby) ? "S0i3" : ((ResumeType == ResumeFromS3) ? "S3" : "Unsupported"));

  PSP_DEBUG ("Psp.ResumeServiceCallBack %s\n", ResumeTypeStr);

  for (Node = GetFirstNode (&PspHeadList);
       Node != &PspHeadList;
       Node = GetNextNode (&PspHeadList, Node)) {
    PSP_DEBUG ("Call PspResumeService.Hook at 0x%08x\n", (UINTN) ((PSP_SMM_CALLBACK_NODE *)Node)->CallBackFunction);
    // Call all registered callback function in order of priority and pass the resume type
    ((PSP_SMM_CALLBACK_NODE *)Node)->CallBackFunction (
      ResumeType,
      ((PSP_SMM_CALLBACK_NODE *)Node)->Context
    );
  }

  return EFI_SUCCESS;
}
