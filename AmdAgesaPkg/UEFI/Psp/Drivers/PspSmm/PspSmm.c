/* $NoKeywords:$ */
/**
 * @file
 *
 * FchPcieWakeWA Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD PSP UEFI Driver
 * @e sub-project   PspSmm Driver
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
#include "PspSmm.h"
#include "PspBaseLib.h"
#include "PspMbox.h"
#include "PspResumeServices.h"
#include "SmmLoad.h"
#include "PspDebug.h"
#include "Filecode.h"
#include EFI_PROTOCOL_CONSUMER   (LoadedImage)

#define FILECODE UEFI_PSP_DRIVERS_PSPSMM_PSPSMM_FILECODE

EFI_SMM_SYSTEM_TABLE                          *mSmst;
SMM_SAVE_AREA_CONTEXT                         *mSmmSaveAreaCopy;
PSP_PLATFORM_PROTOCOL                         mPspPlatformProtocolCopy;
PSP_SMM_RSM_MEM_INFO                          *mPspSmmRsmMemInfoPtr;

//
// Soft SMI handle to repond fot the SW SMI call coming from PSP
//
EFI_STATUS
EFIAPI
PspSxCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  FCH_SMM_SX_REGISTER_CONTEXT   *DispatchContext
  )
{
  PSP_DEBUG ("PSP.PspSmm.SleepCallBack\n");
  if (PspMboxBiosCmdSxInfo (DispatchContext->Type)) {
    return (EFI_UNSUPPORTED);
  }

  return (EFI_SUCCESS);
}

EFI_STATUS
EFIAPI
PspSmmSwSmiCallBack (
  IN  EFI_HANDLE                       DispatchHandle,
  IN  FCH_SMM_SW_REGISTER_CONTEXT      *DispatchContext
  )
{
  PSP_DEBUG ("PSP.PspSmm.SwSmiCallBack\n");

  if (EFI_ERROR (CopySmmSaveAreaToLocal (&mSmmSaveAreaCopy))) {
    return EFI_NOT_FOUND;
  }

  //
  // Setup MSR to instruct microcode to build the S3 save area for resume
  //
  UpdateS3SaveAreaMap ();

  return EFI_SUCCESS;
}

EFI_STATUS
PspSmmEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                      Status;
  BOOLEAN                         InSmm;
  EFI_SMM_BASE_PROTOCOL           *SmmBase;
  FCH_SMM_SX_DISPATCH_PROTOCOL    *SxDispatch;
  EFI_HANDLE                      SxHandle, SwHandle;
  FCH_SMM_SX_REGISTER_CONTEXT     SxRegisterContext;
  FCH_SMM_SW_DISPATCH_PROTOCOL    *AmdSwDispatch;
  FCH_SMM_SW_REGISTER_CONTEXT     SwRegisterContext;
  PSP_PLATFORM_PROTOCOL           *PspPlatfromProtocol;
  EFI_LOADED_IMAGE_PROTOCOL       *LoadedImage;
  EFI_PHYSICAL_ADDRESS            TempStackPtr;
  EFI_PHYSICAL_ADDRESS            AlignStackPtr;
  UINT32                          StackSize;

  //
  // Initialize EFI library
  //
  gST      = SystemTable;
  gBS      = SystemTable->BootServices;
  gRT      = gST->RuntimeServices;

  PSP_DEBUG ("Psp.Drv.PspSmm Enter\n");
  //If PSP feature turn off, exit the driver
  Status = CheckPspDevicePresent ();
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  InSmm    = FALSE;
  Status = gBS->LocateProtocol (
                  &gEfiSmmBaseProtocolGuid,
                  NULL,
                  &SmmBase
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }
  SmmBase->GetSmstLocation (
             SmmBase,
             &mSmst
             );
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

  //
  // We're now in SMM!
  //
  PSP_DEBUG ("\tRunning in SMM\n");
  //
  // We're now in SMM!
  //

  Status = gBS->LocateProtocol (
                  &gFchSmmSxDispatchProtocolGuid,
                  NULL,
                  &SxDispatch
                  );

  SxRegisterContext.Type  = AmdSxS3;
  SxRegisterContext.Phase = AmdSxEntry;
  SxRegisterContext.Order = 1;

  // Register with Smm driver to intercept Sx transition and inform Psp via mailbox in
  // that transition
  PSP_DEBUG ("\tRegister Sleep SMI\n");
  Status = SxDispatch->Register (
                          SxDispatch,
                          PspSxCallback,
                          &SxRegisterContext,
                          &SxHandle
                          );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  // Below code is only needed if the referecen BIOS intend to use New S3 resume path (Using Dram instead of CAR)
  // Even if PSP initialize the DRAM, the OEM bios still has a choice to use the old resume path
  // i.e. do not write to MSR to perform resume. So add customization token to skip below code
  // for those designs
  //Locate Psp Platform Protocol to get customized information
  PSP_DEBUG ("\tLocate Psp Platform Protocol\n");
  Status = gBS->LocateProtocol (
                    &gPspPlatformProtocolGuid,
                    NULL,
                    &PspPlatfromProtocol
                    );

  if (EFI_ERROR (Status)) {
    return Status;
  }
  //Dump Platform protocol info
  PSP_DEBUG ("\tPSP_PLATFORM_PROTOCOL::\n");
  PSP_DEBUG ("\t\tCpuContextResumeEnable:0x%x\n", PspPlatfromProtocol->CpuContextResumeEnable);
  PSP_DEBUG ("\t\tSwSmiCmdtoBuildContext:0x%x\n", PspPlatfromProtocol->SwSmiCmdtoBuildContext);
  PSP_DEBUG ("\t\tBspStackSize:0x%x\n", PspPlatfromProtocol->BspStackSize);
  PSP_DEBUG ("\t\tApStackSize:0x%x\n", PspPlatfromProtocol->ApStackSize);
  PSP_DEBUG ("\t\tRsmHandOffInfo:&0x%x\n", PspPlatfromProtocol->RsmHandOffInfo);
  PSP_DEBUG ("\t\t\tGdtOffset:0x%x\n", PspPlatfromProtocol->RsmHandOffInfo->GdtOffset );
  PSP_DEBUG ("\t\t\tCodeSelector:0x%x\n", PspPlatfromProtocol->RsmHandOffInfo->CodeSelector);
  PSP_DEBUG ("\t\t\tDataSelector:0x%x\n", PspPlatfromProtocol->RsmHandOffInfo->DataSelector);
  PSP_DEBUG ("\t\t\tRsmEntryPoint:0x%x\n", PspPlatfromProtocol->RsmHandOffInfo->RsmEntryPoint);
  PSP_DEBUG ("\t\t\tEdxResumeSignature:0x%x\n", PspPlatfromProtocol->RsmHandOffInfo->EdxResumeSignature);

  //Copy the data to local copy to make it available at SMM space
  gBS->SetMem (&mPspPlatformProtocolCopy,
               sizeof (PSP_PLATFORM_PROTOCOL),
               0);

  gBS->CopyMem (&mPspPlatformProtocolCopy,
                PspPlatfromProtocol,
                sizeof (PSP_PLATFORM_PROTOCOL)
                 );

  if (mPspPlatformProtocolCopy.CpuContextResumeEnable != TRUE) {
    // We are not supporting CPU context resume. So we can bail from here
    return EFI_SUCCESS;
  }


  //Allocate Memory for hold PSP_SMM_RSM_MEM_INFO
  PSP_DEBUG ("\tAllocate Memory for New S3 Resume\n");

  Status = gBS->AllocatePool (
                EfiReservedMemoryType,
                sizeof (PSP_SMM_RSM_MEM_INFO),
                &mPspSmmRsmMemInfoPtr);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //Allocate Memory for AGESA heap during SMM Resume
  //AGESA are request at 4K boudary
  Status = gBS->AllocatePages (
                  AllocateAnyPages,
                  EfiReservedMemoryType,
                  EFI_SIZE_TO_PAGES (SMM_RESUME_HEAP_SIZE),
                  &mPspSmmRsmMemInfoPtr->RsmHeapPtr);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  mPspSmmRsmMemInfoPtr->HeapSize = SMM_RESUME_HEAP_SIZE;

  //Allocate Memory for stack during SMM Resume
  //Allocate Double Stack Size for alignment
  StackSize = mPspPlatformProtocolCopy.BspStackSize + mPspPlatformProtocolCopy.ApStackSize * MAX_AP_COUNT;
  Status = gBS->AllocatePages (
                  AllocateAnyPages,
                  EfiReservedMemoryType,
                  EFI_SIZE_TO_PAGES (2 * StackSize),
                  &TempStackPtr);
  PSP_DEBUG ("Allocate TempStack 0x%x Size 0x%x\n", TempStackPtr, 2 * StackSize);

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //Align on Stack Size boundary
  AlignStackPtr = ((TempStackPtr + mPspPlatformProtocolCopy.BspStackSize) & (~(mPspPlatformProtocolCopy.BspStackSize - 1)));

  //Free Unused Pages
  if (AlignStackPtr - TempStackPtr) {
    PSP_DEBUG ("Free 0x%x Size 0x%x\n", TempStackPtr, (AlignStackPtr - TempStackPtr));
    gBS->FreePages (TempStackPtr, EFI_SIZE_TO_PAGES (AlignStackPtr - TempStackPtr));
  }
  PSP_DEBUG ("Free 0x%x Size 0x%x\n", AlignStackPtr + StackSize, (StackSize - (AlignStackPtr - TempStackPtr)));
  gBS->FreePages (AlignStackPtr + StackSize, EFI_SIZE_TO_PAGES (StackSize - (AlignStackPtr - TempStackPtr)));

  mPspSmmRsmMemInfoPtr->StackPtr = AlignStackPtr;
  PSP_DEBUG ("AlignStackPtr 0x%x\n", AlignStackPtr);

  mPspSmmRsmMemInfoPtr->BspStackSize = mPspPlatformProtocolCopy.BspStackSize;
  mPspSmmRsmMemInfoPtr->ApStackSize = mPspPlatformProtocolCopy.ApStackSize;

  //Allocate Memory for temporary storage during SMM Resume
  Status = gBS->AllocatePages (
                  AllocateAnyPages,
                  EfiReservedMemoryType,
                  EFI_SIZE_TO_PAGES (SMM_RESUME_TEMP_REGION_SIZE),
                  &mPspSmmRsmMemInfoPtr->TempRegionPtr);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mPspSmmRsmMemInfoPtr->TempRegionSize = SMM_RESUME_TEMP_REGION_SIZE;
  //Dump result of memory allocation
  PSP_DEBUG ("\tPspSmmRsmMemInfo::\n");
  PSP_DEBUG ("\t\tStackPtr:0x%x BspStackSize:0x%x ApStackSize:0x%x\n",
              mPspSmmRsmMemInfoPtr->StackPtr,
              mPspSmmRsmMemInfoPtr->BspStackSize,
              mPspSmmRsmMemInfoPtr->ApStackSize);
  PSP_DEBUG ("\t\tRsmHeapPtr:0x%x HeapSize:0x%x\n",
              mPspSmmRsmMemInfoPtr->RsmHeapPtr,
              mPspSmmRsmMemInfoPtr->HeapSize);
  PSP_DEBUG ("\t\tTempRegionPtr:0x%x TempRegionSize:0x%x\n",
              mPspSmmRsmMemInfoPtr->TempRegionPtr,
              mPspSmmRsmMemInfoPtr->TempRegionSize);

  Status = gBS->LocateProtocol (
                  &gFchSmmSwDispatchProtocolGuid,
                  NULL,
                  &AmdSwDispatch
                  );

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Hook for software SMI to prepare CPU context for Smm resume during S3-resume
  PSP_DEBUG ("\tRegister SW SMI for prepare CPU resume context\n");
  SwRegisterContext.AmdSwValue  = mPspPlatformProtocolCopy.SwSmiCmdtoBuildContext;
  SwRegisterContext.Order       = 0x80;
  Status = AmdSwDispatch->Register (
                            AmdSwDispatch,
                            PspSmmSwSmiCallBack,
                            &SwRegisterContext,
                            &SwHandle
                            );

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Publish protocol for others to register for resume time callback
  //
  PSP_DEBUG ("\tPublish resume register protocol\n");
  InstallPspResumeCallbackProtocol ();

  //Send Resume info to PSP to prevent resume module be tampered
  Status = gBS->HandleProtocol (
                    ImageHandle,
                    &gEfiLoadedImageProtocolGuid,
                    &LoadedImage
                    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  PSP_DEBUG ("\tASRF.Base:0x%x ASRF.Size:0x%x\n", LoadedImage->ImageBase, LoadedImage->ImageSize);
  Status = PspMboxBiosCmdResumeInfo (
                  (UINT64) LoadedImage->ImageBase,
                  LoadedImage->ImageSize);

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  PSP_DEBUG ("PspSmm Exit\n");

  return Status;
}

