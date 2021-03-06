/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Hudson-2 Child SMI Dispatcher Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/******************************************************************************
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
****************************************************************************/

#include "FchSmmSwDispatcher.h"
#include "Filecode.h"
#define FILECODE UEFI_SMM_FCHSMMDISPATCHER_FCHSMMSWDISPATCHER_FCHSMMSWDISPATCHER_FILECODE


EFI_STATUS
EFIAPI
FchSmmSwDispatch2Register (
  IN       CONST FCH_SMM_SW_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_SW_HANDLER_ENTRY_POINT2      DispatchFunction,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT    *RegisterContext,
     OUT   EFI_HANDLE                           *DispatchHandle
  );

EFI_STATUS
EFIAPI
FchSmmSwDispatch2UnRegister (
  IN       CONST FCH_SMM_SW_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                           DispatchHandle
  );

FCH_SMM_SW_DISPATCH2_PROTOCOL gFchSmmSwDispatch2Protocol = {
  FchSmmSwDispatch2Register,
  FchSmmSwDispatch2UnRegister,
  (UINTN) MAX_SW_SMI_VALUE
};


/*----------------------------------------------------------------------------------------*/
/**
 * FCH SMM SW dispatcher handler
 *
 *
 * @param[in]       SmmImageHandle        Image Handle
 * @param[in, out]   OPTIONAL CommunicationBuffer   Communication Buffer (see PI 1.2 for more details)
 * @param[in, out]   OPTIONAL SourceSize            Buffer size (see PI 1.2 for more details)

 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmSwDispatchHandler (
  IN       EFI_HANDLE   SmmImageHandle,
  IN OUT   VOID         *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN        *SourceSize OPTIONAL
  )
{
  EFI_STATUS                  Status;
  FCH_SMM_SW_NODE             *CurrentFchSmmSwNodePtr;
  UINT16                      SwSmiValue;
  UINT8                       SwSmiCmdAddress;
  UINT16                      SwSmiDispatched;
  UINTN                       SizeOfFchSmmSwContext;
  UINT8                       i;
  EFI_SMM_SAVE_STATE_IO_INFO  IoInfo;

  Status = EFI_NOT_FOUND;
  SwSmiDispatched = 0;

  SwSmiCmdAddress = ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG6A);
  LibFchSmmIoRead (&gSmst->SmmIo, SMM_IO_UINT16, SwSmiCmdAddress, &SwSmiValue);

  ASSERT (mSmmCpuProtocol != NULL);
  i = (UINT8) gSmst->NumberOfCpus;
  do {
    EfiSmmSwContext->SwSmiCpuIndex = i - 1;
    Status = mSmmCpuProtocol->ReadSaveState (
                                mSmmCpuProtocol,
                                sizeof (EFI_SMM_SAVE_STATE_IO_INFO),
                                EFI_SMM_SAVE_STATE_REGISTER_IO,
                                EfiSmmSwContext->SwSmiCpuIndex,
                                &IoInfo
                                );
    if ((Status == EFI_SUCCESS) && (IoInfo.IoPort == SwSmiCmdAddress) && (IoInfo.IoData == (UINT8) SwSmiValue)) {
      break;
    }
  } while (--i);
  EfiSmmSwContext->CommandPort = (UINT8) SwSmiValue;
  EfiSmmSwContext->DataPort = (UINT8) (SwSmiValue >> 8);

  if (HeadFchSmmSwNodePtr->FchSwNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    CurrentFchSmmSwNodePtr = HeadFchSmmSwNodePtr;
    while (CurrentFchSmmSwNodePtr->FchSwNodePtr!= NULL) {
      if ((UINT8) CurrentFchSmmSwNodePtr->Context.AmdSwValue == (UINT8) SwSmiValue) {
        if (CurrentFchSmmSwNodePtr->CallBack2Function != NULL) {
          SizeOfFchSmmSwContext = (UINTN) sizeof (FCH_SMM_SW_CONTEXT);
          Status = CurrentFchSmmSwNodePtr->CallBack2Function (
                                          CurrentFchSmmSwNodePtr->DispatchHandle,
                                          &CurrentFchSmmSwNodePtr->Context,
                                          EfiSmmSwContext,
                                          &SizeOfFchSmmSwContext
                                          );
          DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] SW SMM handler dispatched: SwValue = 0x%x, Order = 0x%x, return - %r\n", \
                 CurrentFchSmmSwNodePtr->Context.AmdSwValue, CurrentFchSmmSwNodePtr->Context.Order, Status));
        }

        SwSmiDispatched++;
      }
      CurrentFchSmmSwNodePtr = CurrentFchSmmSwNodePtr->FchSwNodePtr;
    }
    if (SwSmiDispatched <= 0) {
      Status = EFI_NOT_FOUND;
    } else {
      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG88) = SmiCmdPort;
      Status = EFI_SUCCESS;
    }
  }
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG88) = SmiCmdPort;
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register SW child handler
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       DispatchFunction      Callback Function
 * @param[in, out]  RegisterContext       Register contecxt (see PI 1.1 for more details)
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmSwDispatch2Register (
  IN       CONST FCH_SMM_SW_DISPATCH2_PROTOCOL *This,
  IN       FCH_SMM_SW_HANDLER_ENTRY_POINT2     DispatchFunction,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT   *RegisterContext,
     OUT   EFI_HANDLE                          *DispatchHandle
  )
{
  EFI_STATUS      Status;
  FCH_SMM_SW_NODE *NewFchSmmSwNodePtr;
  FCH_SMM_SW_NODE *CurrentFchSmmSwNodePtr;
  FCH_SMM_SW_NODE *PreviousFchSmmSwNodePtr;

  Status = EFI_OUT_OF_RESOURCES;

  if (DispatchFunction == NULL || RegisterContext == NULL || DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0) |= BIT22;
  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (FCH_SMM_SW_NODE),
                    &NewFchSmmSwNodePtr
                    );
  ASSERT_EFI_ERROR (Status);

  NewFchSmmSwNodePtr->CallBack2Function = DispatchFunction;
  NewFchSmmSwNodePtr->Context = *RegisterContext;
  *DispatchHandle = &NewFchSmmSwNodePtr->DispatchHandle;
  NewFchSmmSwNodePtr->DispatchHandle = *DispatchHandle;

  DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] Registering SW SMM handler: SwValue = 0x%x, Order = 0x%x\n", RegisterContext->AmdSwValue, RegisterContext->Order));
  if (HeadFchSmmSwNodePtr->FchSwNodePtr == NULL) {
    NewFchSmmSwNodePtr->FchSwNodePtr = HeadFchSmmSwNodePtr;
    HeadFchSmmSwNodePtr = NewFchSmmSwNodePtr;
  } else {
    PreviousFchSmmSwNodePtr = HeadFchSmmSwNodePtr;
    CurrentFchSmmSwNodePtr = HeadFchSmmSwNodePtr;
    while (CurrentFchSmmSwNodePtr->FchSwNodePtr != NULL) {
      if (NewFchSmmSwNodePtr->Context.Order <= CurrentFchSmmSwNodePtr->Context.Order &&
          CurrentFchSmmSwNodePtr->Context.AmdSwValue == NewFchSmmSwNodePtr->Context.AmdSwValue) {

        if (PreviousFchSmmSwNodePtr == CurrentFchSmmSwNodePtr) {
          NewFchSmmSwNodePtr->FchSwNodePtr = HeadFchSmmSwNodePtr;
          HeadFchSmmSwNodePtr = NewFchSmmSwNodePtr;
          Status = EFI_SUCCESS;
          return Status;
        }
        NewFchSmmSwNodePtr->FchSwNodePtr = PreviousFchSmmSwNodePtr->FchSwNodePtr;
        PreviousFchSmmSwNodePtr->FchSwNodePtr = NewFchSmmSwNodePtr;

        Status = EFI_SUCCESS;
        return  Status;
      }
      PreviousFchSmmSwNodePtr = CurrentFchSmmSwNodePtr;
      CurrentFchSmmSwNodePtr = CurrentFchSmmSwNodePtr->FchSwNodePtr;
    }
    PreviousFchSmmSwNodePtr->FchSwNodePtr = NewFchSmmSwNodePtr;
    NewFchSmmSwNodePtr->FchSwNodePtr = CurrentFchSmmSwNodePtr;
  }
  Status = EFI_SUCCESS;
  return  Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Unregister SW child handler
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       DispatchHandle        Dispatch Handle
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmSwDispatch2UnRegister (
  IN       CONST FCH_SMM_SW_DISPATCH2_PROTOCOL *This,
  IN       EFI_HANDLE                          DispatchHandle
  )
{
  EFI_STATUS      Status;
  FCH_SMM_SW_NODE *CurrentFchSmmSwNodePtr;
  FCH_SMM_SW_NODE *PreviousFchSmmSwNodePtr;

  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (HeadFchSmmSwNodePtr->FchSwNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
    return  Status;
  } else {
    PreviousFchSmmSwNodePtr = HeadFchSmmSwNodePtr;
    CurrentFchSmmSwNodePtr = HeadFchSmmSwNodePtr;
    if (CurrentFchSmmSwNodePtr->DispatchHandle == DispatchHandle) {
      HeadFchSmmSwNodePtr = CurrentFchSmmSwNodePtr->FchSwNodePtr;
    } else {
      while (CurrentFchSmmSwNodePtr->DispatchHandle != DispatchHandle) {
        PreviousFchSmmSwNodePtr = CurrentFchSmmSwNodePtr;
        CurrentFchSmmSwNodePtr = CurrentFchSmmSwNodePtr->FchSwNodePtr;
        if (CurrentFchSmmSwNodePtr->DispatchHandle == NULL) {
          Status = EFI_NOT_FOUND;
          return  Status;
        }
      }
      PreviousFchSmmSwNodePtr->FchSwNodePtr = CurrentFchSmmSwNodePtr->FchSwNodePtr;
    }

    Status = gSmst->SmmFreePool (
                      CurrentFchSmmSwNodePtr
                      );
    ASSERT_EFI_ERROR (Status);
  }
  Status = EFI_SUCCESS;
  return  Status;
}


EFI_STATUS
EFIAPI
EfiSmmSwDispatch2Register (
  IN       CONST EFI_SMM_SW_DISPATCH2_PROTOCOL   *This,
  IN       EFI_SMM_HANDLER_ENTRY_POINT2          DispatchFunction,
  IN OUT   EFI_SMM_SW_REGISTER_CONTEXT           *RegisterContext,
     OUT   EFI_HANDLE                            *DispatchHandle
  )
{
  FCH_SMM_SW_REGISTER_CONTEXT    FchRegisterContext;

  FchRegisterContext.AmdSwValue = RegisterContext->SwSmiInputValue;
  FchRegisterContext.Order = 0x80;      // SMM_DEFAULT_ORDER
  return FchSmmSwDispatch2Register (
           &gFchSmmSwDispatch2Protocol,
           DispatchFunction,
           &FchRegisterContext,
           DispatchHandle
           );
}


EFI_STATUS
EFIAPI
EfiSmmSwDispatch2UnRegister (
  IN       CONST EFI_SMM_SW_DISPATCH2_PROTOCOL *This,
  IN       EFI_HANDLE                          DispatchHandle
  )
{
  return FchSmmSwDispatch2UnRegister (
           &gFchSmmSwDispatch2Protocol,
           DispatchHandle
           );
}

EFI_SMM_SW_DISPATCH2_PROTOCOL gEfiSmmSwDispatch2Protocol = {
  EfiSmmSwDispatch2Register,
  EfiSmmSwDispatch2UnRegister,
  (UINTN) MAX_SW_SMI_VALUE
};

