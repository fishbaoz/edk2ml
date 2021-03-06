/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH SMM Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH SMM Driver
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
#include "FchSmm.h"
#define FILECODE UEFI_SMM_FCHSMM_SXSMI_FILECODE

extern  UINT8                        *mFchPciIrqRoutingTable;
extern FCH_INIT_PROTOCOL                gFchInitInSmm;

EFI_STATUS
FchSmmRegisterSxSmi (
  VOID
  )
{
  EFI_STATUS                               Status;
  FCH_SMM_SX_DISPATCH2_PROTOCOL            *AmdSxDispatch;
  FCH_SMM_SX_REGISTER_CONTEXT              SxRegisterContext;
  EFI_HANDLE                               SxHandle;

  //
  // Register AMD SX SMM
  //
  Status = gSmst->SmmLocateProtocol (
                  &gFchSmmSxDispatch2ProtocolGuid,
                  NULL,
                  &AmdSxDispatch
                  );
  ASSERT_EFI_ERROR (Status);

  SxRegisterContext.Type  = SxS3;
  SxRegisterContext.Phase = SxEntry;
  SxRegisterContext.Order = 1;

  Status = AmdSxDispatch->Register (
                             AmdSxDispatch,
                             AmdSmiS3SleepEntryCallback,
                             &SxRegisterContext,
                             &SxHandle
                             );

  SxRegisterContext.Type  = SxS4;
  SxRegisterContext.Phase = SxEntry;
  SxRegisterContext.Order = 1;

  Status = AmdSxDispatch->Register (
                             AmdSxDispatch,
                             AmdSmiS4SleepEntryCallback,
                             &SxRegisterContext,
                             &SxHandle
                             );

  SxRegisterContext.Type  = SxS5;
  SxRegisterContext.Phase = SxEntry;
  SxRegisterContext.Order = 1;

  Status = AmdSxDispatch->Register (
                             AmdSxDispatch,
                             AmdSmiS5SleepEntryCallback,
                             &SxRegisterContext,
                             &SxHandle
                             );

  return Status;
}

/**
 * FixPsp4Ehang
 *
 *
 * @retval  VOID
 *
 */
VOID
FixPsp4Ehang (
  OUT VOID
  )
{
  UINT32          IoApicNumber;
  for (IoApicNumber = 0; IoApicNumber < 24; IoApicNumber++) {
    ACPIMMIO32 (0xFEC00000) = 0x10 + (IoApicNumber << 1);
    ACPIMMIO32 (0xFEC00010) = BIT16;
  }
  ACPIMMIO32 (FCH_AOACx94S013_CONTROL) |= FCH_AOACx94S013_CONTROL_ARBITER_DIS + FCH_AOACx94S013_CONTROL_INTERRUPT_DIS;
}

/*++

Routine Description:


Arguments:

  DispatchHandle  - The handle of this callback, obtained when registering
  DispatchContext - Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT

Returns:

  None.

--*/
EFI_STATUS
EFIAPI
AmdSmiS3SleepEntryCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SX_REGISTER_CONTEXT *DispatchContext,
  IN OUT   VOID                              *CommBuffer OPTIONAL,
  IN OUT   UINTN                             *CommBufferSize  OPTIONAL
  )
{
  UINT8          Index;
  UINT8          *pData;
  FCH_DATA_BLOCK      *pFchPolicy;
  pFchPolicy = &gFchInitInSmm.FchPolicy;
  // Save entire FCH PCI IRQ routing space (C00/C01)
  pData = mFchPciIrqRoutingTable;
  Index = 0xFF;
  do {
    Index++;
    LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGC00, &Index);
    LibFchSmmIoRead (&gSmst->SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGC01, pData++);
  } while (Index != 0xFF);

  //Put Usb3 to S0 power rail
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEE, AccessWidth8, (UINT32)~(BIT0 + BIT1), (BIT1 + BIT0));
  BackUpCG2 ();
  FixPsp4Ehang ();
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AmdSmiS4SleepEntryCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SX_REGISTER_CONTEXT *DispatchContext,
  IN OUT   VOID                              *CommBuffer OPTIONAL,
  IN OUT   UINTN                             *CommBufferSize  OPTIONAL
  )
{
  //Put Usb3 to S0 power rail
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEE, AccessWidth8, (UINT32)~(BIT0 + BIT1), (BIT1 + BIT0));
  BackUpCG2 ();
  FixPsp4Ehang ();
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AmdSmiS5SleepEntryCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SX_REGISTER_CONTEXT *DispatchContext,
  IN OUT   VOID                              *CommBuffer OPTIONAL,
  IN OUT   UINTN                             *CommBufferSize  OPTIONAL
  )
{
  BackUpCG2 ();
  //RwMem (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG04 + 2, AccessWidth8, ~BIT3, BIT3);
  FixPsp4Ehang ();
  return EFI_SUCCESS;
}
