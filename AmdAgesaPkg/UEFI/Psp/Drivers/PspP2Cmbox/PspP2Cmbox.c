/* $NoKeywords:$ */
/**
 * @file
 *
 * PspP2Cmbox Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD PSP UEFI Driver
 * @e sub-project   PspP2Cmbox Driver
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
#include "PspP2Cmbox.h"
#include "Filecode.h"
#include "PspBaseLib.h"
#include "SmmLoad.h"
#include "Fch.h"

#include EFI_PROTOCOL_CONSUMER   (FchSmmMiscDispatch)

#define FILECODE UEFI_PSP_DRIVERS_PSPP2CMBOX_PSPP2CMBOX_FILECODE

#include EFI_PROTOCOL_CONSUMER   (SmmStatusCode)
///@todo Using SmmStatusCode Protocol instead DEBUG for RT

/// @todo move this out of this file to common place
/// @todo make it customizable via build option etc. Also check the Size of this buffer is
/// sufficient for PSP communication

#define PSP_DATA_BLOCK_SIZE       (4*1024)
#define PSP_P2CMBOX_SW_SMI_CALLBACK    0xAD
//#define PSP2BIOS_USING_SW_SMI


EFI_SMM_SYSTEM_TABLE                  *mSmst;
EFI_SMM_CPU_IO_INTERFACE              *mSmmIo;
UINT64                                mTsegBase = 0;
UINT64                                mTsegSize = 0;
BIOS_MBOX                             *PspToBiosMbox = NULL;

SMM_TRIGGER_INFO  mSmmTrigInfo =  {
                                      0x00,                     // Address location
                                      0x00,                     // AddressType
                                      0x00,                     // Address width 0: Byte
                                      0x00,                     // And Mask
                                      0x00  // OrMask
                                  };

EFI_STATUS
CheckMboxIntegrity (
  IN  MBOX_BUFFER_HEADER *MboxBuffer
  )
{
  if ( (MboxBuffer == NULL) ||
       (mTsegBase == 0)  ||
       ( (UINT64)MboxBuffer < mTsegBase) ||
       (UINT64)MboxBuffer > (mTsegBase + mTsegSize) ||
       (MboxBuffer->TotalSize > PSP_DATA_BLOCK_SIZE )
      ) {
    return (EFI_INVALID_PARAMETER);
  }

  return (EFI_SUCCESS);

}

EFI_STATUS
P2CmboxSmmCallBackWorker (
  VOID
  )
{
  UINT32                MboxCmd;
  UINT32                *pMboxSts;
  MBOX_DEFAULT_BUFFER   *MboxBuffer;
  EFI_STATUS            Status;

  Status = EFI_SUCCESS;

  MboxCmd     = PspToBiosMbox->MboxCmd;
  pMboxSts     = (UINT32 *)&(PspToBiosMbox->MboxSts);
  MboxBuffer  = (MBOX_DEFAULT_BUFFER *)&(PspToBiosMbox->Buffer.Dflt);
  PSP_DEBUG ("Psp.P2CMbox Cmd:0x%x\n", MboxCmd);

  // basic security check. Unless the buffer is not in SMM space ignore
  Status = CheckMboxIntegrity ( (MBOX_BUFFER_HEADER *)MboxBuffer);
  PSP_DEBUG ("\tCheckMboxIntegrity[%x]\n", Status);

  if (!EFI_ERROR (Status)) {
    Status = ProcessPspCommand (
               MboxCmd,
               MboxBuffer
               );
  }

  *pMboxSts = MBOX_STATUS_INITIALIZED;
  if (EFI_ERROR (Status)) {
    *pMboxSts |= MBOX_STATUS_ERROR;
  }

  PspToBiosMbox->MboxCmd  = 0;     //Clear the command register

  // Trigger interrupt to PSP. Inform command processed
  PspMboxBiosCmdNop ();

  PSP_DEBUG ("P2CMbox Exit[0x%x] \n", *pMboxSts );

  return (Status);
}
//
// Soft SMI handle to repond to sw smi call coming from PSP
//
EFI_STATUS
EFIAPI
P2CmboxSwSmiCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  FCH_SMM_SW_REGISTER_CONTEXT   *DispatchContext
  )
{
  return P2CmboxSmmCallBackWorker ();
}

EFI_STATUS
EFIAPI
P2CmboxMiscSmiCallback (
  IN  EFI_HANDLE                       DispatchHandle,
  IN  FCH_SMM_MISC_REGISTER_CONTEXT   *MiscRegisterContext
  )
{
  EFI_STATUS Status;

  Status = P2CmboxSmmCallBackWorker ();
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98) &= ~BIT25;  // Deassert fakeSts0

  return Status;
}

EFI_STATUS
InitFchFakeStsInfo (
  IN OUT   SMM_TRIGGER_INFO *SmmTriggerInfoPtr
  )
{
  SmmTriggerInfoPtr->Address = ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98;
  SmmTriggerInfoPtr->AddressType = SMM_TRIGGER_MEM;
  SmmTriggerInfoPtr->ValueWidth = SMM_TRIGGER_DWORD;
  SmmTriggerInfoPtr->ValueAndMask = ~BIT25;
  SmmTriggerInfoPtr->ValueOrMask = BIT25;
  return EFI_SUCCESS;
}

EFI_STATUS
EnablePspFakeStsSmi (
  VOID
  )
{
  FCH_SMM_MISC_DISPATCH_PROTOCOL    *AmdFchSmmMiscDispatch;
  EFI_HANDLE                        MiscHandle;
  FCH_SMM_MISC_REGISTER_CONTEXT     MiscRegisterContext;
  EFI_STATUS                        Status;

  // enable PSP SMM via Fake Sts0
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG84) |= BIT1;  //Clear FakeSts0

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98) &= ~BIT25;  // Deassert fakeSts0

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGA8) &= ~(BIT3 + BIT2);  // Enable SMI [3:2] = 1
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGA8) |= BIT2;

  MiscRegisterContext.SmiStatusReg = FCH_SMI_REG84;
  MiscRegisterContext.SmiStatusBit = BIT1;
  MiscRegisterContext.Order        = 0x80;

  Status = gBS->LocateProtocol (
                    &gFchSmmMiscDispatchProtocolGuid,
                    NULL,
                    &AmdFchSmmMiscDispatch
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = AmdFchSmmMiscDispatch->Register (
                                      AmdFchSmmMiscDispatch,
                                      &P2CmboxMiscSmiCallback,
                                      &MiscRegisterContext,
                                      &MiscHandle
                                      );

  return Status;
}
EFI_STATUS
EnablePspSwSmi (
  VOID
  )
{
  EFI_STATUS                      Status;
  FCH_SMM_SW_REGISTER_CONTEXT     SwContext;
  EFI_HANDLE                      SwHandle;
  FCH_SMM_SW_DISPATCH_PROTOCOL    *SwDispatch;
  EFI_SMM_CONTROL_PROTOCOL        *SmmControl;
  EFI_SMM_CONTROL_REGISTER         SmmReg;

  //
  //  Get the Sw dispatch protocol
  //
  /// @todo - Fixme Find way to locate this protocol with in SMM & not use gBS
  Status = gBS->LocateProtocol (
                  &gFchSmmSwDispatchProtocolGuid,
                  NULL,
                  &SwDispatch
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SwContext.AmdSwValue  = PSP_P2CMBOX_SW_SMI_CALLBACK;
  SwContext.Order       = 0x80;
  Status = SwDispatch->Register (
                          SwDispatch,
                          &P2CmboxSwSmiCallback,
                          &SwContext,
                          &SwHandle
                          );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (
                        &gEfiSmmControlProtocolGuid,
                        NULL,
                        &SmmControl
                        );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = SmmControl->GetRegisterInfo (SmmControl, &SmmReg);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mSmmTrigInfo.Address = (UINT64) SmmReg.SmiTriggerRegister;
  mSmmTrigInfo.AddressType = SMM_TRIGGER_IO;
  mSmmTrigInfo.ValueWidth = SMM_TRIGGER_BYTE;
  mSmmTrigInfo.ValueAndMask = 0;
  mSmmTrigInfo.ValueOrMask = PSP_P2CMBOX_SW_SMI_CALLBACK;
  PSP_DEBUG ("\tSwSmiTriggerReg:0x%x\n", SmmReg.SmiTriggerRegister);

  return EFI_SUCCESS;
}

EFI_STATUS
PspP2CmboxEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                      Status;
  BOOLEAN                         InSmm;
  EFI_SMM_BASE_PROTOCOL           *SmmBase;

  //If PSP feature turn off, exit the driver
  Status = CheckPspDevicePresent ();
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Initialize EFI library
  //
  InSmm    = FALSE;
  gST      = SystemTable;
  gBS      = SystemTable->BootServices;
  gRT      = gST->RuntimeServices;

  PSP_DEBUG ("Psp.Drv.P2Cmbox Enter\n");

  Status = gBS->LocateProtocol (
                  &gEfiSmmBaseProtocolGuid,
                  NULL,
                  &SmmBase
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SmmBase->GetSmstLocation (
             SmmBase,
             &mSmst
             );
  mSmmIo = &mSmst->SmmIo;
  SmmBase->InSmm (
             SmmBase,
             &InSmm
             );


  if (!InSmm) {
    return (RegDriverToSmm (
              ImageHandle,
              SystemTable,
              SmmBase
              ));

  }
 PSP_DEBUG ("\tRunning in SMM\n");
  //
  // We're now in SMM!
  //
  // Init PSP storage Lib
  //
  PSP_DEBUG ("\tStorage Lib Init\n");
  Status =  PspStorageInit (SystemTable);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Allocate SMM buffer for PSP-> BIOS communication
  PSP_DEBUG ("\tAllocate SMM buffer for P2C Mbox\n");
  Status = mSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    PSP_DATA_BLOCK_SIZE,
                    &PspToBiosMbox
                    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Initialize the data structure
  EfiZeroMem (PspToBiosMbox, PSP_DATA_BLOCK_SIZE);
  PspToBiosMbox->MboxCmd      = 0;
  *(UINT32 *)&(PspToBiosMbox->MboxSts) = MBOX_STATUS_INITIALIZED;

  #ifdef PSP2BIOS_USING_SW_SMI
  PSP_DEBUG ("\tP2C using SW SMI\n");
  Status = EnablePspSwSmi ();
  if (EFI_ERROR (Status)) {
    return Status;
  }
  #else
  PSP_DEBUG ("\tP2C using fake SMI\n");
  InitFchFakeStsInfo (&mSmmTrigInfo);
  Status = EnablePspFakeStsSmi ();
  if (EFI_ERROR (Status)) {
    return Status;
  }
  #endif
  //Get TsegBase, TsegSize
  PspLibMsrRead (MSR_SMMADDR, &mTsegBase);
  mTsegBase &= MSR_SMMADDR_TSEGBASE_BITS;

  PspLibMsrRead (MSR_SMMMASK, &mTsegSize);
  mTsegSize = (~(mTsegSize & MSR_SMMMASK_TSEGMASK_BITS) + 1) & MSR_SMMMASK_TSEGMASK_BITS;


  PSP_DEBUG ("\tSMMBase:0x%x SMMLength:0x%x\n\tPSPSmmDataRegion:0x%x PspSmmDataLength:0x%x\n", mTsegBase, mTsegSize, PspToBiosMbox, PSP_DATA_BLOCK_SIZE);
  //Dump mSmmTrigInfo
  PSP_DEBUG ("\tSmmTrigInfo::Addr:0x%x AddrType:0x%x Width:0x%x AndMask:0x%x OrMask:0x%x\n",
              mSmmTrigInfo.Address,
              mSmmTrigInfo.AddressType,
              mSmmTrigInfo.ValueWidth,
              mSmmTrigInfo.ValueAndMask,
              mSmmTrigInfo.ValueOrMask);

  if (PspMboxBiosCmdSmmInfo (
        mTsegBase,
        mTsegSize,
        (UINT64) PspToBiosMbox,
        PSP_DATA_BLOCK_SIZE,
        &mSmmTrigInfo
        )) {
    return (EFI_DEVICE_ERROR);
  }
  PSP_DEBUG ("P2Cmbox Exit\n");

  return Status;
}

