/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH WHEA DXE Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH WHEA DXE Driver
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

#include "AmdFchWheaSmm.h"
#include <Library/UefiDriverEntryPoint.h>
#include <Library/SmmServicesTableLib.h>

AMD_FCH_WHEA_EINJ_BUFFER   *mEinjData;

/********************************************************************************
 * Name: AmdFchWheaInitEntry
 *
 * Description
 *   AmdFchWheaSmmInit Entrypoint
 *
 * Input
 *
 * Output
 *   EFI_UNSUPPORTED : unsupported function
 *
 *********************************************************************************/
EFI_STATUS
AmdFchWheaSmmInitEntry (
  IN EFI_HANDLE                             ImageHandle,
  IN EFI_SYSTEM_TABLE                       *SystemTable
  )
{
  EFI_STATUS                                Status;
  FCH_INIT_PROTOCOL                         *AmdFchInit;
  FCH_SMM_SW_DISPATCH2_PROTOCOL             *AmdSwDispatch;
  FCH_SMM_SW_REGISTER_CONTEXT               SwRegisterContext;
  EFI_HANDLE                                SwHandle;
  FCH_SMM_MISC_DISPATCH_PROTOCOL            *AmdFchSmmMiscDispatch;
  FCH_SMM_MISC_REGISTER_CONTEXT             MiscRegisterContext;
  EFI_HANDLE                                MiscHandle;

  Status = gBS->LocateProtocol (
                  &gFchInitProtocolGuid,
                  NULL,
                  &AmdFchInit
                  );
  ASSERT_EFI_ERROR (Status);

  if (AmdFchInit->FchPolicy.Gpp.PcieAer == 0) {
    return Status;
  }

  Status = gBS->LocateProtocol (
                  &gEfiAmdFchWheaDataProtocolGuid,
                  NULL,
                  &mEinjData
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gSmst->SmmLocateProtocol (
                    &gFchSmmSwDispatch2ProtocolGuid,
                    NULL,
                    &AmdSwDispatch
                    );
  ASSERT_EFI_ERROR (Status);

  SwRegisterContext.AmdSwValue  = EINJ_TRIGGER_ACTION_SWSMI;
  Status = AmdSwDispatch->Register (
                            AmdSwDispatch,
                            AmdSmiEinjTriggerActionCallBack,
                            &SwRegisterContext,
                            &SwHandle
                            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gSmst->SmmLocateProtocol (
                    &gFchSmmMiscDispatchProtocolGuid,
                    NULL,
                    &AmdFchSmmMiscDispatch
                    );
  ASSERT_EFI_ERROR (Status);

  MiscRegisterContext.SmiStatusReg = FCH_SMI_REG88;
  MiscRegisterContext.SmiStatusBit = BIT21;
  MiscRegisterContext.Order        = 0x80;
  Status = AmdFchSmmMiscDispatch->Register (
                                    AmdFchSmmMiscDispatch,
                                    AmdMiscFchWheaHwSmiCallback,
                                    &MiscRegisterContext,
                                    &MiscHandle
                                    );

  MiscRegisterContext.SmiStatusReg = FCH_SMI_REG88;
  MiscRegisterContext.SmiStatusBit = BIT22;
  MiscRegisterContext.Order        = 0x80;
  Status = AmdFchSmmMiscDispatch->Register (
                                    AmdFchSmmMiscDispatch,
                                    AmdMiscFchWheaHwSmiCallback,
                                    &MiscRegisterContext,
                                    &MiscHandle
                                    );

  MiscRegisterContext.SmiStatusReg = FCH_SMI_REG88;
  MiscRegisterContext.SmiStatusBit = BIT23;
  MiscRegisterContext.Order        = 0x80;
  Status = AmdFchSmmMiscDispatch->Register (
                                    AmdFchSmmMiscDispatch,
                                    AmdMiscFchWheaHwSmiCallback,
                                    &MiscRegisterContext,
                                    &MiscHandle
                                    );

  MiscRegisterContext.SmiStatusReg = FCH_SMI_REG88;
  MiscRegisterContext.SmiStatusBit = BIT24;
  MiscRegisterContext.Order        = 0x80;
  Status = AmdFchSmmMiscDispatch->Register (
                                    AmdFchSmmMiscDispatch,
                                    AmdMiscFchWheaHwSmiCallback,
                                    &MiscRegisterContext,
                                    &MiscHandle
                                    );

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB4) &= ~(BIT11 + BIT13 + BIT15 + BIT17);
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB4) |= (BIT10 + BIT12 + BIT14 + BIT16);

  return Status;
}

EFI_STATUS
EFIAPI
AmdSmiEinjTriggerActionCallBack (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  EFI_STATUS          Status;
  Status = EFI_SUCCESS;

  if (AmdSmiEinjValidate ()) {

    AmdSmiEinjSystemContextSave ();

    AmdSmiEinjErrorInjectionSetup ();

    AmdSmiEinjClrStatus ();

    AmdSmiEinjErrorInjection ();
  }

  return Status;
}

EFI_STATUS
EFIAPI
AmdMiscFchWheaHwSmiCallback (
  IN  EFI_HANDLE                            DispatchHandle,
  IN  FCH_SMM_MISC_REGISTER_CONTEXT         *DispatchContext
  )
{
  EFI_STATUS          Status;
  Status = EFI_SUCCESS;

  AmdSmiEinjChkErr ();
  AmdSmiEinjClrStatus ();
  AmdSmiEinjErrInjDis ();
  AmdSmiEinjSystemContextRestore ();

  return Status;
}

