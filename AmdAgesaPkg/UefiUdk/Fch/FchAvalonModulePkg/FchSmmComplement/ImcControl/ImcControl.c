/* $NoKeywords:$ */
/**
 * @file
 *
 * Imc Control Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD Hudson-2 UEFI Driver
 * @e sub-project   Imc Control Driver
 * @e \$Revision: 283503 $   @e \$Date: 2014-01-24 18:31:20 -0600 (Fri, 24 Jan 2014) $
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
#define FILECODE UEFI_SMM_FCHSMMCOMPLEMENT_IMCCONTROL_IMCCONTROL_FILECODE
#include <Library/FchSmmLib.h>
#include "ImcControl.h"

EFI_SMM_CPU_IO2_PROTOCOL   *mSmmIo;
UINT32                     mImcThermalEnable;   ///< Bit0=81FunZone0 support(1=On;0=Off); bit1-3=81FunZone1-Zone3;

EFI_STATUS
EFIAPI
ImcControlS3SleepEntryCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SX_REGISTER_CONTEXT *DispatchContext,
  IN OUT   VOID                              *CommBuffer OPTIONAL,
  IN OUT   UINTN                             *CommBufferSize  OPTIONAL
  )
{
  FchImcWakeup ();
  FchImcIdle ();
  FchImcSleep ();
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ImcControlS4SleepEntryCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SX_REGISTER_CONTEXT *DispatchContext,
  IN OUT   VOID                              *CommBuffer OPTIONAL,
  IN OUT   UINTN                             *CommBufferSize  OPTIONAL
  )
{
  FchImcWakeup ();
  FchImcIdle ();
  FchImcSleep ();
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ImcControlS5SleepEntryCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SX_REGISTER_CONTEXT *DispatchContext,
  IN OUT   VOID                              *CommBuffer OPTIONAL,
  IN OUT   UINTN                             *CommBufferSize  OPTIONAL
  )
{
  FchImcWakeup ();
  FchImcIdle ();
  FchImcSleep ();
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ImcControlThermalZoneEnableCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  FchImcWakeup ();
  FchImcThermalZoneEnable ();
  return EFI_SUCCESS;
}

EFI_STATUS
ImcControlSmmRegister (
  VOID
  )
{
  EFI_STATUS                               Status;
  FCH_SMM_SX_DISPATCH2_PROTOCOL            *AmdSxDispatch;
  FCH_SMM_SX_REGISTER_CONTEXT              SxRegisterContext;
  EFI_HANDLE                               SxHandle;
  FCH_SMM_SW_DISPATCH2_PROTOCOL            *AmdSwDispatch;
  FCH_SMM_SW_REGISTER_CONTEXT              SwRegisterContext;
  EFI_HANDLE                               SwHandle;

  //
  // Register Imc Control SX SMM
  //
  Status = gSmst->SmmLocateProtocol (
                    &gFchSmmSxDispatch2ProtocolGuid,
                    NULL,
                    &AmdSxDispatch
                    );
  ASSERT_EFI_ERROR (Status);

  SxRegisterContext.Type  = SxS3;
  SxRegisterContext.Phase = SxEntry;
  SxRegisterContext.Order = 0x80;

  Status = AmdSxDispatch->Register (
                             AmdSxDispatch,
                             ImcControlS3SleepEntryCallback,
                             &SxRegisterContext,
                             &SxHandle
                             );

  SxRegisterContext.Type  = SxS4;
  SxRegisterContext.Phase = SxEntry;
  SxRegisterContext.Order = 0x80;

  Status = AmdSxDispatch->Register (
                             AmdSxDispatch,
                             ImcControlS4SleepEntryCallback,
                             &SxRegisterContext,
                             &SxHandle
                             );

  SxRegisterContext.Type  = SxS5;
  SxRegisterContext.Phase = SxEntry;
  SxRegisterContext.Order = 0x80;

  Status = AmdSxDispatch->Register (
                             AmdSxDispatch,
                             ImcControlS5SleepEntryCallback,
                             &SxRegisterContext,
                             &SxHandle
                             );

  //
  //  Locate SMM SW dispatch protocol
  //
  Status = gSmst->SmmLocateProtocol (
                    &gFchSmmSwDispatch2ProtocolGuid,
                    NULL,
                    &AmdSwDispatch
                    );
  ASSERT_EFI_ERROR (Status);

  SwRegisterContext.AmdSwValue  = PcdGet8 (PcdFchOemAfterPciRestoreSwSmi);
  SwRegisterContext.Order       = 0x90;
  Status = AmdSwDispatch->Register (
                            AmdSwDispatch,
                            ImcControlThermalZoneEnableCallback,
                            &SwRegisterContext,
                            &SwHandle
                            );

  SwRegisterContext.AmdSwValue  = PcdGet8 (PcdFchOemEnableAcpiSwSmi);
  SwRegisterContext.Order       = 0x90;
  Status = AmdSwDispatch->Register (
                            AmdSwDispatch,
                            ImcControlThermalZoneEnableCallback,
                            &SwRegisterContext,
                            &SwHandle
                            );
  return Status;
}

/*********************************************************************************
 * Name: ImcControlEntry
 *
 * Description
 *   Entry point of the ImcControl driver
 *   Register Smm callbacks for Imc Work-around
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
ImcControlEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )

{
  EFI_STATUS                  Status;
  FCH_INIT_PROTOCOL           *AmdFchInit;

  mSmmIo = &gSmst->SmmIo;
  if (mSmmIo == NULL) {
    return EFI_LOAD_ERROR;
  }

  Status = gBS->LocateProtocol (
                    &gFchInitProtocolGuid,
                    NULL,
                    &AmdFchInit
                    );
  if (!EFI_ERROR (Status)) {
    mImcThermalEnable = AmdFchInit->FchPolicy.Imc.EcStruct.IMCFUNSupportBitMap;
  } else {
    return EFI_ABORTED;
  }

  Status = ImcControlSmmRegister ();
  return Status;
}


