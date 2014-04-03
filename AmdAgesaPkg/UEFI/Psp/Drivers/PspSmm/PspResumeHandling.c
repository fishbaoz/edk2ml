/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP related functions in SMM
 *
 * Contains PSP SMM Resume handler
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

#include "Fch.h"
#include "PspSmm.h"
#include "PspDebug.h"
#include "PspResumeServices.h"
#include "Filecode.h"

#define FILECODE UEFI_PSP_DRIVERS_PSPSMM_PSPRESUMEHANDLING_FILECODE
VOLATILE BOOLEAN              WaitForBspToFinishRestore = FALSE;
VOLATILE UINTN                CoreInSmm = 0;

STATIC UINT8 ApExitS3Count = 0;

VOID
GetSmmSaveArea (
  IN OUT   UINT64  *CoreSaveAreaOffset
  )
{
  *CoreSaveAreaOffset  = EfiReadMsr (0xC0010111) + 0xFE00;
}

EFI_STATUS
CopySmmSaveAreaToLocal (
  IN OUT   VOID **SmmSaveStart
  )
{
  UINTN   i;
  UINT64                  CoreSaveAreaOffset;
  SMM_SAVE_AREA_CONTEXT   *Dest;
  EFI_STATUS              Status;

  PSP_DEBUG ("CopySmmSaveAreaToLocal\n");
  // Allocate SMM buffer to save SMM save area for each core
  Status = mSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    (mSmst->NumberOfCpus)* (SMM_SAVE_SIZE + sizeof (UINT64)),
                    SmmSaveStart
                    );

  PSP_DEBUG ("SmmSaveArea at %08x\n", *SmmSaveStart);

  if (EFI_ERROR (Status)) {
    PSP_DEBUG ("Smm Pool Allocate fail\n");
    return (Status);
  }
  Dest = (SMM_SAVE_AREA_CONTEXT *)*SmmSaveStart;

  for (i = 0; i < mSmst->NumberOfCpus; i++) {
    if (i == 0) {
      GetSmmSaveArea (&CoreSaveAreaOffset);
    } else {
      mSmst->SmmStartupThisAp (
             GetSmmSaveArea,
             i,
             &CoreSaveAreaOffset
             );
    }
    PSP_DEBUG ("Core%d SMM SaveState0x%08x\n", i, CoreSaveAreaOffset);

    Dest->SaveAreaOffset = (VOID *)CoreSaveAreaOffset;
    PSP_DEBUG ("Copy SMM SaveState from 0x%08x to 0x%08x\n", Dest->SaveAreaOffset, Dest->SaveAreaBuffer);

    EfiCopyMem (Dest->SaveAreaBuffer,
      Dest->SaveAreaOffset,
      SMM_SAVE_SIZE
      );
    Dest++;
  }
  return Status;
}


VOID
EFIAPI
OverrideSmmSaveArea (
  IN OUT   VOID                *SmmSaveArea,
  IN       REGISTER_OVERRIDE   *RegisterOverride,
  IN       UINTN                CpuIndex
  )
{
  SMM_SAVE_MAP  *SmmSavePtr = (SMM_SAVE_MAP *)SmmSaveArea;


  SmmSavePtr->CS_Selector   = RegisterOverride->CodeSegOverrideValue;
  SmmSavePtr->CS_Attributes = RegisterOverride->CodeSegAttrib;
  SmmSavePtr->DS_Selector   = RegisterOverride->DateSegOverrideValue;
  SmmSavePtr->DS_Attributes = RegisterOverride->DataSegAttrib;
  SmmSavePtr->DS_Attributes = RegisterOverride->DataSegAttrib;
  SmmSavePtr->ES_Selector   = RegisterOverride->DateSegOverrideValue;
  SmmSavePtr->FS_Selector   = RegisterOverride->DateSegOverrideValue;
  SmmSavePtr->FS_Attributes = RegisterOverride->DataSegAttrib;
  SmmSavePtr->DS_Attributes = RegisterOverride->DataSegAttrib;
  SmmSavePtr->GS_Selector   = RegisterOverride->DateSegOverrideValue;
  SmmSavePtr->GS_Attributes = RegisterOverride->DataSegAttrib;
  SmmSavePtr->DS_Attributes = RegisterOverride->DataSegAttrib;
  SmmSavePtr->SS_Selector   = RegisterOverride->DateSegOverrideValue;
  SmmSavePtr->DS_Attributes = RegisterOverride->DataSegAttrib;


  SmmSavePtr->RAX = RegisterOverride->RaxOverrideValue;
  SmmSavePtr->RDX = RegisterOverride->RdxOverrideValue;

  SmmSavePtr->GDTR_Base   = RegisterOverride->GdtOverrideValue;
  SmmSavePtr->GDTR_Limit  = 0x3f;
  SmmSavePtr->IDTR_Base   = 0;
  SmmSavePtr->IDTR_Limit  = 0;

  SmmSavePtr->CR0 = RegisterOverride->Cr0;
  SmmSavePtr->CR3 = RegisterOverride->Cr3;
  SmmSavePtr->CR4 = RegisterOverride->Cr4;
  SmmSavePtr->RFLAGS = 0;

  if (CpuIndex == 0) {
    SmmSavePtr->RIP = RegisterOverride->RipBspOverrideValue;
    SmmSavePtr->RSP = RegisterOverride->StackBase + RegisterOverride->BspStackSize;
  } else {
    SmmSavePtr->RIP = RegisterOverride->RipApOverrideValue;
    SmmSavePtr->RSP = ( RegisterOverride->StackBase +
                        RegisterOverride->BspStackSize +
                        (RegisterOverride->ApStackSize * CpuIndex)
                       );
  }

  //Clear Auto Halt
  SmmSavePtr->Auto_Halt_Restart = 0;
}


EFI_STATUS
EFIAPI
RestoreSaveAreaFromLocal (
  IN OUT   SMM_SAVE_AREA_CONTEXT *SmmSaveStart,
  IN OUT   REGISTER_OVERRIDE     *RegisterOverride
  )
{
  UINTN   i;

  for (i = 0; i < mSmst->NumberOfCpus; i++) {

    PSP_DEBUG ("Restore SmmSave from %08x to %08x\n", SmmSaveStart->SaveAreaBuffer, SmmSaveStart->SaveAreaOffset);
    EfiCopyMem (
      SmmSaveStart->SaveAreaOffset,
      SmmSaveStart->SaveAreaBuffer,
      SMM_SAVE_SIZE
      );

    OverrideSmmSaveArea (
      SmmSaveStart->SaveAreaOffset,
      RegisterOverride,
      i
      );
    SmmSaveStart++;
  }
  return EFI_SUCCESS;
}


VOID
PspConnectedStandbyResumeHandling (
  )
{
  // Call Register callback for connected standby
  PspResumeServiceCallBack (ResumeFromConnectedStandby);
}

EFI_STATUS
EFIAPI
CustomizeSmmRsmVector (
  IN OUT   REGISTER_OVERRIDE       *RegisterOverride
  )
{
  RSM_HANDOFF_INFO *ResumeHandoffInfo;

  ResumeHandoffInfo = mPspPlatformProtocolCopy.RsmHandOffInfo;
  RegisterOverride->RipBspOverrideValue   = ResumeHandoffInfo->RsmEntryPoint;
  RegisterOverride->RipApOverrideValue    = ResumeHandoffInfo->RsmEntryPoint;
  RegisterOverride->RdxOverrideValue      = ResumeHandoffInfo->EdxResumeSignature;
  RegisterOverride->GdtOverrideValue      = ResumeHandoffInfo->GdtOffset;
  RegisterOverride->CodeSegOverrideValue  = ResumeHandoffInfo->CodeSelector;
  RegisterOverride->DateSegOverrideValue  = ResumeHandoffInfo->DataSelector;

  RegisterOverride->GdtOverrideLimit      = 0x3f;
  RegisterOverride->CodeSegAttrib         = 0xc9b;
  RegisterOverride->DataSegAttrib         = 0xc93;
  RegisterOverride->Cr0                   = 0x00000013;
  RegisterOverride->Cr3                   = 0x00;
  RegisterOverride->Cr4                   = 0x00040608;

  RegisterOverride->RaxOverrideValue      = (UINTN) mPspSmmRsmMemInfoPtr;      //Rax Point to mPspSmmRsmMemInfo structure
  RegisterOverride->StackBase             = mPspSmmRsmMemInfoPtr->StackPtr;
  RegisterOverride->BspStackSize          = mPspSmmRsmMemInfoPtr->BspStackSize; //Reserve Stack for BSP
  RegisterOverride->ApStackSize           = mPspSmmRsmMemInfoPtr->ApStackSize; //Reserve Stack for AP


  return (EFI_SUCCESS);
}

VOID
EFIAPI
PspS3ResumeHandling (
  VOID
  )
{
  REGISTER_OVERRIDE     RegisterOverride;

  //Set OEM customized info
  CustomizeSmmRsmVector (&RegisterOverride);

  // Patch Smm save area
  RestoreSaveAreaFromLocal (
    mSmmSaveAreaCopy,
    &RegisterOverride
  );

  // Now call all the Register callback handling
  PspResumeServiceCallBack ( ResumeFromS3);

}
BOOLEAN
IsS0i3Resume (
  )
{
  UINT32 S0I3Resume;

  S0I3Resume = ACPIMMIO32 (ACPI_MMIO_BASE + 0x1E00 + 0x94);
  S0I3Resume &= BIT9;
  return  (S0I3Resume? TRUE: FALSE);
}


BOOLEAN
IsS3Resume (
  )
{
  UINT16 SleepType;
  UINT16 AcpiPm1CntBlk;

  AcpiPm1CntBlk = ACPIMMIO16 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG62);
  PspLibIoRead (AccessWidth16, AcpiPm1CntBlk, &SleepType);
  SleepType = ((SleepType >> 10) & 7);
  return ((SleepType == 3) ? TRUE : FALSE);
}

UINT8
GetResumeType (
  )
{
  if (IsS0i3Resume ()) {
    return ResumeFromConnectedStandby;
  } else if (IsS3Resume ()) {
    return ResumeFromS3;
  } else {
    PSP_DEBUG ("Unsupported Resume type\n");
    ASSERT (FALSE);
    EFI_DEADLOOP ();  //Should never come here
    return InvalidResumeValue;
  }
}

VOID
ClearS3Exit (
  )
{
  //Increment Ap Exit S3 Count, Inform PSP FW when Last AP comes Here
  if (++ApExitS3Count >= (mSmst->NumberOfCpus)) {
    PspMboxBiosClearS3Status ();
    ApExitS3Count = 0;
  }
}

//
// This is the entry point of AP when we resume from sleep /CS and
// after PSP restore the memory. PSP release BSP and BSP will go through PEI Agesa
// driver to release AP. Here we setup stack etc and AP will rsm to SEC code
// AP from here
//
VOID
ApSmmResumeVector (
  )
{
  UINT8 ResumeType;

  PSP_DEBUG ("PSP.ApSmmResumeVector\n");

  ResumeType = GetResumeType (); //Check resume type is S3 or connected standby

  if (ResumeType == ResumeFromS3) {
    ClearS3Exit ();
    S3ExitToBios ();
  } else if (ResumeType == ResumeFromConnectedStandby) {
    // Call Register callback for connected standby
    PspConnectedStandbyResumeHandling ();
  }

  EFI_DEADLOOP ();
}

//
// This is the entry point of BSP when we resume from sleep and
// after PSP restore the memory will release the CPU code to run
// BSP from here
//
VOID
BspSmmResumeVector (
  )
{
  UINT8 ResumeType;
  PSP_DEBUG ("PSP.BspSmmResumeVector\n");

  ResumeType = GetResumeType (); //Check resume type is S3 or connected standby

  if (ResumeType == ResumeFromS3) {
    PSP_DEBUG ("S3 Resume Handling\n");

    //Init Bar for enable CPU2PSP commnunication in Early Secure S3 Path
    PspBarInitEarly ();
    ClearS3Exit ();
    PspS3ResumeHandling ();
    S3ExitToBios ();
  } else if (ResumeType == ResumeFromConnectedStandby) {
    // Call Register callback for connected standby
    PSP_DEBUG ("S0i3 resume Handling\n");
    PspConnectedStandbyResumeHandling ();
  }

  // we should never come here
  EFI_DEADLOOP ();
}

VOID  SetReleseVector (
  IN OUT   VOID *ResumeVector
  )
{
  PSP_DEBUG ("PSP.SmmReleaseVector at %08x\n", (UINTN) ResumeVector);
  EfiWriteMsr ( 0xC00110E0, (UINT64)ResumeVector);
}

VOID
UpdateS3SaveAreaMap (
  )
{
  UINTN i;
  PSP_DEBUG ("UpdateS3SaveAreaMap\n");
  for (i = 1; i < mSmst->NumberOfCpus; i++) {
    mSmst->SmmStartupThisAp (
             SetReleseVector,
             i,
             (VOID *)(UINTN)ApSmmResumeVector
             );

  }

  SetReleseVector ((VOID *) (UINTN) &BspSmmResumeVector);
}
