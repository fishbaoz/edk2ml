/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD SMM DXE Driver for RAS
 *
 * AGESA RAS DXE Driver to perform RAS SMM functionality
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI/Addendum/AmdPlatformRas/RAS
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ****************************************************************************
 * AMD Generic Encapsulated Software Architecture
 *
 * Description: AmdRasSmmDxeDriver.c - AGESA RAS DXE Driver to perform RAS SMM
 * functionality.
 *
 ******************************************************************************
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

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AmdRasSmmDxeDriver.h"
#include "SmmRuntimeLib.h"
#include "CpuFuncs.h"
#include EFI_PROTOCOL_CONSUMER (LoadedImage)
#include EFI_PROTOCOL_CONSUMER (SmmBase)


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */



/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */



/*----------------------------------------------------------------------------------------
 *                   D R I V E R  L I B  G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
EFI_BOOT_SERVICES       *gBS = NULL;
EFI_RUNTIME_SERVICES    *gRT;
EFI_SMM_SYSTEM_TABLE    *gSmst;
STATIC EFI_HANDLE       mSmmImageHandle;
UINT32                  mCfgMmioBase;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

EFI_STATUS
InitializeRasRuntime (
  IN       EFI_HANDLE ImageHandle,
  IN       EFI_SYSTEM_TABLE *SystemTable
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

EFI_DRIVER_ENTRY_POINT (AmdRasSmmDxeDriverEntryPoint);

/*----------------------------------------------------------------------------------------
 *                          RAS Default Values
 *----------------------------------------------------------------------------------------
 */

// Initialize Northbridge Correctable Memory Error Data struct
RAS_NB_CORR_MEM_CONFIG gRasNbCorrMemConfig = {
{
  NB_CORR_MEM,          // AMD_RAS_DEVICE_ID - Numeric RAS Device ID
  TRUE,                 // Global enable for this error?
  SMI,                  // ERROR_MONITOR_TYPE - Enum Monitor type, e.g APIC, SMI, NMI, etc.
  sizeof (RAS_NB_CORR_MEM_CONFIG), // Size of this structure for checking
},
  TRUE,                 // BOOLEAN - Enable Threshold Counter?
  TRUE,                 // BOOLEAN - Set Lock Bit?
  0xFFE,                // UINT32 - Must not be zero or 0xFFF
  TRUE,                 // BOOLEAN - Enable Memory Parity?
  TRUE                  // BOOLEAN - Enable Correctable ECC?
};

RAS_NB_UNCORR_MEM_CONFIG gRasNbUnCorrMemConfig = {
{
  NB_UNCORR_MEM,          // AMD_RAS_DEVICE_ID - Numeric RAS Device ID
  TRUE,                   // Global enable for this error?
  SYNC_FLOOD,             // ERROR_MONITOR_TYPE - Enum Monitor type, e.g APIC, SMI, NMI, SYNC_FLOOD, etc.
  sizeof (RAS_NB_UNCORR_MEM_CONFIG), // Size of this structure for checking
},
  TRUE,                   // BOOLEAN - Set Lock Bit?
  TRUE,                   // BOOLEAN - Enable Memory Parity?
  TRUE,                   // BOOLEAN - Enable Uncorrectable ECC?
};


/**
 *---------------------------------------------------------------------------------------
 *
 *  AmdRasSmmDxeDriverEntryPoint
 *
 *  Description:
 *     Entry point for the RAS DXE driver.
 *
 *  Parameters:
 *    @param[in]     ImageHandle
 *    @param[in]     *SystemTable
 *
 *    @retval         EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/

EFI_STATUS
EFIAPI
AmdRasSmmDxeDriverEntryPoint (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                      Status;
  BOOLEAN                         InSmm;
  EFI_HANDLE                      hNrthBrdgError = NULL;
  EFI_SMM_BASE_PROTOCOL           *SmmBase = NULL;
  EFI_LOADED_IMAGE_PROTOCOL       *LoadedImage = NULL;
  EFI_DEVICE_PATH_PROTOCOL        *ImageDevicePath = NULL;
  EFI_DEVICE_PATH_PROTOCOL        *NewFilePath = NULL;
  AMD_RAS_DXE_INTERFACE_PROTOCOL  *RasIntProtocol = NULL;
  SMM_SETUP_CONFIG                SmmSetupCfg;


  // Initialize Global Variables
  Status = InitializeRasRuntime (
                                 ImageHandle,          // IN EFI_HANDLE
                                 SystemTable           // IN EFI_SYSTEM_TABLE
                                 );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;          // Error detected while trying to locate SMM Base Protocol
  }

  // Locate SMM Base Protocol
  Status  = gBS->LocateProtocol (
                                 &gEfiSmmBaseProtocolGuid, // IN EFI_GUID Published unique identifier of requested protocol GUID to locate
                                 NULL,                     // IN VOID* Published unique identifier of requested protocol GUID
                                 &SmmBase                  // OUT VOID** Returns address of pointer for SMM Base protocol interface
                                 );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;          // Error detected while trying to locate SMM Base Protocol
  }

  // Save the SMM Base system table pointer
  SmmBase->GetSmstLocation (SmmBase, &gSmst);

  // Determine if we are in SMM or not
  SmmBase->InSmm (SmmBase, &InSmm);

  if (!InSmm) {
    //
    // Not in SMM so load this driver's SMM image to memory
    //
    // Configure Northbridge Interrupt Event to make an I/O Write to SMI Command Port in the
    // Southbridge and cause a Software SMI for which we will handle in the NbSmiHandler
    // SMI service routine.
    Status = ConfigNbSmiProcessing ();

    // Query image handle to see if it supports a requested protocol and then get an address of the protocol image to be loaded
    Status = gBS->HandleProtocol (
                                  ImageHandle,                   // Handle of requested protocol
                                  &gEfiLoadedImageProtocolGuid,  // Published unique identifier of requested protocol GUID
                                  (VOID*)&LoadedImage            // Returns address of pointer for requested protocol interface of the image
                                  );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;            // Error detected while trying to query LoadImage protocol
    }

    // Query device handle of loaded image to see if it supports a requested protocol and then get an address of the prototocl device path
    Status = gBS->HandleProtocol (
                                  LoadedImage->DeviceHandle,   // Handle of requested protocol
                                  &gEfiDevicePathProtocolGuid, // Published unique identifier of requested protocol GUID
                                  (VOID*)&ImageDevicePath      // Returns address of pointer for requested protocol interface
                                  );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }

    // Add this image's device path to the loaded image device path image and return
    // a pointer to the new file path.
    NewFilePath = AddDevicePath (
                                 ImageDevicePath,      // IN EFI_DEVICE_PATH_PROTOCOL
                                 LoadedImage->FilePath // IN EFI_DEVICE_PATH_PROTOCOL Device Path Image to add
                                 );

    //Load the image in System Management RAM; it will automatically generate SMI.
    Status = SmmBase->Register (
                                SmmBase,         // IN *This Addreess of pointer for SMM Base protocol interface)
                                NewFilePath,     // IN EFI_DEVICE_PATH_PROTOCOL
                                NULL,            // IN SourceBuffer OPTIONAL
                                0,               // IN SourceSize OPTIONAL
                                &hNrthBrdgError, // OUT *ImageHandle
                                FALSE            // IN LegacyIA32Binary OPTIONAL
                                );

    // Free up residual heap memory if not equal to NULL
    if (NewFilePath) {
         gBS->FreePool (NewFilePath);
    }
  } else {
    // We are in SMM

    //
    // Callout to use platform-specific Setup
    //
    Status = AmdRasSmmCallout (ECC_HNDLR_CALLOUT_ID, &SmmSetupCfg);

    if (EFI_ERROR (Status)) {
      SmmSetupCfg.EccHandlerEnable = FALSE;   //Load default value = ECC Disabled
    }

    // Enable / Disable RAS MCA ECC Support
    if (SmmSetupCfg.EccHandlerEnable == TRUE) {
      // Callback to execute from within SMM, error occurs if called from Boot Services mode
      Status = SmmBase->RegisterCallback (
                                          SmmBase,        // IN *This
                                          ImageHandle,    // IN SmmImageHandle
                                          MC4SmmHandler,  // IN EFI SMM Dispatcher Callback Entry Point Address
                                          FALSE,          // IN Makelast (SMM Dispatcher should call this handler last) OPTIONAL
                                          FALSE           // IN FloatingPointSave OPTIONAL
                                          );
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return Status;
      }

      // Update Global Varibles for this driver
      Status = UpdateGlobalVars ();
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return Status;
      }
    }  //End-- Enable / Disable ECC Handler Support
  } // End-- if(!InSmm)

  if (!InSmm) {
    // Allocate memory for a new protocol structure
    Status = gBS->AllocatePool (
                                EfiBootServicesData,
                                sizeof (AMD_RAS_DXE_INTERFACE_PROTOCOL),
                                &RasIntProtocol
                                );

    if (RasIntProtocol == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    // Zero out memory buffer
    EfiCommonLibZeroMem (
                         RasIntProtocol,
                         sizeof (AMD_RAS_DXE_INTERFACE_PROTOCOL)
                         );

    // Locate AMD RAS Interface Protocol
    Status  = gBS->LocateProtocol (
                                   &gAmdRasDxeInterfaceProtocolGuid,  // IN EFI_GUID Published unique identifier of requested protocol GUID to locate
                                   NULL,                              // IN VOID* Published unique identifier of requested protocol GUID
                                   (VOID **)&RasIntProtocol           // OUT VOID** Returns address of pointer for SMM Base protocol interface
                                   );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return EFI_SUCCESS;         // Error detected while trying to locate RAS Interface Protocol
    }

    // Callout to get ECC Threshold BIOS Setup option config.
    Status = AmdRasSmmCallout (ECC_THRESH_COUNTER_CALLOUT_ID, &SmmSetupCfg);

    if (EFI_ERROR (Status)) {
      gRasNbCorrMemConfig.ThreshLimit = 0xFFE;   // Load default value = threshold value of 1 (1's complement)
    } else {
      gRasNbCorrMemConfig.ThreshLimit = SmmSetupCfg.DramErrorThreshold;
    }

    // Callout to get RAS Handle Type BIOS Setup option config.
    Status = AmdRasSmmCallout (RAS_HANDLE_CALLOUT_ID, &SmmSetupCfg);

    if (EFI_ERROR (Status)) {
        gRasNbCorrMemConfig.DevCommon.ErrorMonitorType = APIC;    // Default to APIC interrupt = 1 on error
    } else {
      if (SmmSetupCfg.RasHandleType == 1) {
        gRasNbCorrMemConfig.DevCommon.ErrorMonitorType = APIC;    // APIC interrupt = 1
      }
    }

    // Configure Correctable Memory Error Operation
    Status = RasIntProtocol->AmdSetErrorConfigDevice (
                                                      RasIntProtocol,                          // IN *This
                                                      gRasNbCorrMemConfig.DevCommon.DeviceID,  // IN UINTN (enum AMD_RAS_DEVICE_ID)
                                                      (VOID*) &gRasNbCorrMemConfig             // IN OUT VOID* RAS Device Struct
                                                      );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return EFI_SUCCESS;       // Error detected while trying to locate SMM Base Protocol
    }

    // Configure UnCorrectable Memory Error Operation
    Status = RasIntProtocol->AmdSetErrorConfigDevice (
                                                      RasIntProtocol,                            // IN *This
                                                      gRasNbUnCorrMemConfig.DevCommon.DeviceID,  // IN UINTN (enum AMD_RAS_DEVICE_ID)
                                                      (VOID*) &gRasNbUnCorrMemConfig             // IN OUT VOID* RAS Device Struct
                                                      );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return EFI_SUCCESS;       // Error detected while trying to locate SMM Base Protocol
    }
  } // End-- if (!InSmm)

  return Status;
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  InitializeRasRuntime
 *
 *  Description:
 *     Initialize Runtime global variables
 *
 *  Parameters:
 *    @param[in]     ImageHandle
 *    @param[in]     *SystemTable
 *
 *    @retval         EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/

EFI_STATUS
InitializeRasRuntime (
  IN       EFI_HANDLE ImageHandle,
  IN       EFI_SYSTEM_TABLE *SystemTable
  )
{
  UINT64  MsrVal;

  if ((SystemTable != NULL) && (SystemTable->BootServices != NULL)) {
    gST = SystemTable;
    gBS = SystemTable->BootServices;
    gRT = SystemTable->RuntimeServices;
  }

  MsrVal = EfiReadMsr (AMD_MSR_MMIO_CFG_BASE);
  mCfgMmioBase = (UINT32) MsrVal & 0xfff00000;

  return EFI_SUCCESS;
}

