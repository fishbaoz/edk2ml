/* $NoKeywords:$ */
/**
 * @file
 *
 * CS Resume SMM Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   CS Resume SMM Driver
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
#include "CsResumeSmm.h"
#include "CpuIA32.h"
#include "CsResumeSmmLib.h"
#include "CsResumeCommunicateProtocol.h"
#include "PspMbox.h"
#include "PspDebug.h"
#include "SmmLoad.h"
#include "Filecode.h"

#define FILECODE  UEFI_PSP_DRIVERS_CSRESUMESMM_CSRESUMESMM_FILECODE

#define APIC_BAR_MSR   (0x0000001Bul)
#define   BSP_BIT      (0x100ull)
#define CS_RESUME_CALLBACK_PRIORITY_LAST   (0x00000000ul)
#define CS_RESUME_CALLBACK_PRIORITY_FIRST  (0xFFFFFFFFul)

EFI_SMM_SYSTEM_TABLE                *mSmst;
VOID                                *mStaticStateData = NULL;

extern
VOID
ProcessSocReplayBuffer (
  IN       VOID *CsRestoreBuffer
  );

extern
VOID
PutExecutingSocCoreIntoCC6State (
  );


EFI_DRIVER_ENTRY_POINT (CsResumeSmmEntryPoint);



/*----------------------------------------------------------------------------------------*/
/**
 * Soft SMI handler for receiving the restoration buffer
 *
 *
 * @param[in]       DispatchHandle        The handle of this callback, obtained when registering
 * @param[in]       DispatchContext       Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT *
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
ReceiveCsRestorationData (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  FCH_SMM_SW_REGISTER_CONTEXT   *DispatchContext
  )
{
  EFI_STATUS                   Status;
  UINTN                        i;
  UINT8                        *Dest;
  UINT8                        *Source;
  CS_RESUME_COMMUNICATE_PROTOCOL *CsResume;
  FCH_SMM_SW_DISPATCH_PROTOCOL   *SwDispatch;

  if (mStaticStateData == NULL) {
    Status = gBS->LocateProtocol (&gCsResumeCommunicateProtocolGuid, NULL, &CsResume);
    if (EFI_ERROR (Status)) {
      return EFI_ABORTED;
    }

    Status = mSmst->SmmAllocatePool (
                      EfiRuntimeServicesData,
                      CsResume->BufferLength,
                      &mStaticStateData
                      );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Source = (UINT8 *) (CsResume->BufferPtr);
    Dest = (UINT8 *) mStaticStateData;
    for (i = 0; i < CsResume->BufferLength; i++) {
      Dest[i] = Source[i];
    }

    // Send Resume info to PSP
    Status = PspMboxBiosCmdResumeInfo (
                    (UINT64) mStaticStateData,
                    (UINT64) (CsResume->BufferLength));

    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    // UnRegister this SW SMI
    Status = gBS->LocateProtocol (
                    &gFchSmmSwDispatchProtocolGuid,
                    NULL,
                    &SwDispatch
                    );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    SwDispatch->UnRegister (SwDispatch, DispatchHandle);

    return EFI_SUCCESS;
  }

  return EFI_UNSUPPORTED;
}


/*----------------------------------------------------------------------------------------*/
/**
 * CS Resume driver main entry point
 *
 *
 * @param[in]       ImageHandle           Image Handle
 * @param[in]       SystemTable           Pointer to system globals
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
CsResumeSmmEntryPoint (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )

{
  EFI_STATUS                      Status;
  BOOLEAN                         InSmm;
  EFI_LOADED_IMAGE_PROTOCOL       *LoadedImage;
  EFI_SMM_BASE_PROTOCOL           *SmmBase;
  FCH_SMM_SW_DISPATCH_PROTOCOL    *SwDispatch;
  PSP_RESUME_SERVICE_PROTOCOL     *pPspServices;
  FCH_SMM_SW_REGISTER_CONTEXT     SwContext;
  EFI_HANDLE                      SwHandle;
  EFI_HANDLE                      RegistrationHandle;
  //
  // Initialize EFI library
  //
  InSmm    = FALSE;
  gST      = SystemTable;
  gRT      = SystemTable->RuntimeServices;
  gBS      = SystemTable->BootServices;

  PSP_DEBUG ("CsResumeSmm driver Enter\n");

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
    //
    // Not in SMM. Load this driver's image into SMRAM
    //
    return (RegDriverToSmm (
              ImageHandle,
              SystemTable,
              SmmBase
              ));

  } else {
    //
    // We're now in SMM!
    //
    PSP_DEBUG ("CsResumeSmm driver is running in SMM\n");

    Status = gBS->LocateProtocol (
                    &gPspResumeServiceProtocolGuid,
                    NULL,
                    &pPspServices
                    );
    if (EFI_ERROR (Status)) {
      return EFI_ABORTED;
    }

    PSP_DEBUG ("CsResumeSmm driver register CsResumeStaticStateRestore\n");
    Status = pPspServices->Register (
                             pPspServices,
                             CsResumeStaticStateRestore,
                             NULL,
                             CS_RESUME_CALLBACK_PRIORITY_FIRST,
                             &RegistrationHandle
                             );
    if (EFI_ERROR (Status)) {
      return EFI_ABORTED;
    }
    PSP_DEBUG ("CsResumeSmm driver register CsResumeReturnToOs\n");
    Status = pPspServices->Register (
                             pPspServices,
                             CsResumeReturnToOs,
                             NULL,
                             CS_RESUME_CALLBACK_PRIORITY_LAST,
                             &RegistrationHandle
                             );
    if (EFI_ERROR (Status)) {
      return EFI_ABORTED;
    }

    Status = gBS->LocateProtocol (
                    &gFchSmmSwDispatchProtocolGuid,
                    NULL,
                    &SwDispatch
                    );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    PSP_DEBUG ("CsResumeSmm driver register ReceiveCsRestorationData SW SMI\n");

    SwContext.AmdSwValue  = CS_RESTORATION_DATA_SWSMI;
    SwContext.Order       = 0x80;
    Status = SwDispatch->Register (
                            SwDispatch,
                            ReceiveCsRestorationData,
                            &SwContext,
                            &SwHandle
                            );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }
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

    Status = PspMboxBiosCmdResumeInfo (
                    (UINT64) LoadedImage->ImageBase,
                    LoadedImage->ImageSize);

    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }

  }
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * CS Resume path code to restore the SoC registers from the restoration buffer
 *
 *
 * @param[in]       ResumeType            Secure resume type
 * @param[in]       Context               Optional data
 *
 * @retval          EFI_SUCCESS           SoC restored
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
CsResumeStaticStateRestore (
  IN  RESUME_TYPE   ResumeType,
  IN  VOID          *Context
  )
{
  if (ResumeType == ResumeFromConnectedStandby) {
    if ((EfiReadMsr (APIC_BAR_MSR) & BSP_BIT) == BSP_BIT) {
      ProcessSocReplayBuffer (mStaticStateData);
    }
  }
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * CS Resume path code to return control back to the operating system
 *
 *
 * @param[in]       ResumeType            Secure resume type
 * @param[in]       Context               Optional data
 *
 * @retval          EFI_SUCCESS           SoC restored
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
CsResumeReturnToOs (
  IN  RESUME_TYPE   ResumeType,
  IN  VOID          *Context
  )
{
  if (ResumeType == ResumeFromConnectedStandby) {
    // Go to C6
    PutExecutingSocCoreIntoCC6State ();

    // Control should not return
    EFI_DEADLOOP ();
  }
  return EFI_SUCCESS;
}
