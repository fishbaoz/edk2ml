/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD RAS APEI DXE Driver
 *
 * DXE Driver to perform APEI (WHEA) tasks.
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
 * Description: AmdRasApeiDxeDriver.c - DXE Driver to perform APEI (WHEA) tasks.
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
#include "Tiano.h"
#include "AmdRasApeiDxeDriver.h"
#include "EfiDriverLib.h"
#include "Acpi.h"
#include "CpuFuncs.h"
#include "AmdApei.h"
#include "AmdApeiErst.h"
#include "AmdApeiEinj.h"
#include "AmdApeiHest.h"
#include "AmdApeiBert.h"
#include "AmdRasSmmCpuIoLib.h"
#include EFI_PROTOCOL_CONSUMER (AcpiSupport)
#include EFI_PROTOCOL_CONSUMER (SmmBase)
#include EFI_PROTOCOL_CONSUMER (LoadedImage)
#include EFI_PROTOCOL_CONSUMER (AmdRasApeiProtocol)
#include EFI_PROTOCOL_CONSUMER (PciRootBridgeIo)


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
EFI_GUID gAcpiSupportGuid           = EFI_ACPI_SUPPORT_GUID;
EFI_GUID gAmdRasApeiProtocolGuid    = AMD_RAS_APEI_PROTOCOL_GUID;
EFI_GUID gEfiPciRootBridgeIoProtocolGuid = EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_GUID;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
extern EFI_RUNTIME_SERVICES   *gRT;
APEI_DRIVER_PRIVATE_DATA      *mApeiPrivData;
AMD_APEI_INTERFACE            *mApeiInterface;
EFI_EVENT                     mEvtApeiReadyToBoot;
UINT32                        mCfgMmioBase;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
AddBertErrorRecord (
  IN UINT8       *ErrorRecord,
  IN UINT32      RecordLen,
  IN UINT8       ErrorType,
  IN UINT8       SeverityType
  );

EFI_STATUS
AddHestErrorRecord (
  IN UINT8       *ErrorRecord,
  IN UINT32      RecordLen
  );

VOID
AddApeiTables (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

EFI_DRIVER_ENTRY_POINT (AmdRasApeiDxeDriverEntryPoint);


/**
 *---------------------------------------------------------------------------------------
 *
 *  EfiInitializeApei
 *
 *  Description:
 *     Initialize global variables.
 *
 *  Parameters:
 *    @param[in]     ImageHandle
 *    @param[in]     *SystemTable
 *
 *    @retval         EFI_SUCCESS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
EfiInitializeApei (
  IN EFI_HANDLE          ImageHandle,
  IN EFI_SYSTEM_TABLE    *SystemTable
  )
{
  UINT64 MsrVal;
  gST = SystemTable;

  if ((SystemTable != NULL) && (SystemTable->BootServices != NULL)) {
    gST = SystemTable;
    gBS = SystemTable->BootServices;
    gRT = SystemTable->RuntimeServices;
  }

  ASSERT (gST != NULL);
  ASSERT (gBS != NULL);
  ASSERT (gRT != NULL);

  MsrVal = EfiReadMsr (AMD_MSR_MMIO_CFG_BASE);
  mCfgMmioBase = (UINT32)MsrVal & 0xfff00000;

  return EFI_SUCCESS;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  AmdRasApeiDxeDriverEntryPoint
 *
 *  Description:
 *     Entry point for the cpu driver.
 *
 *  Parameters:
 *    @param[in]     ImageHandle
 *    @param[in]     *SystemTable
 *
 *    @retval         AGESA_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/

EFI_STATUS
EFIAPI
AmdRasApeiDxeDriverEntryPoint (
  IN   EFI_HANDLE        ImageHandle,
  IN   EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                      Status = EFI_SUCCESS;
  EFI_HANDLE                      Handle = NULL;
  EFI_HANDLE                      hEinj = NULL;
  AMD_RAS_APEI_PROTOCOL           *AmdRasApeiProtocol;
  BOOLEAN                         InSmm;
  EFI_SMM_BASE_PROTOCOL           *SmmBase = NULL;
  EFI_DEVICE_PATH_PROTOCOL        *ImageDevicePath = NULL;
  EFI_DEVICE_PATH_PROTOCOL        *NewFilePath = NULL;
  EFI_LOADED_IMAGE_PROTOCOL       *LoadedImage = NULL;
  EFI_SMM_SW_DISPATCH_PROTOCOL    *SwDispatch = NULL;
  EFI_SMM_SW_DISPATCH_CONTEXT     SwContext;
  EFI_ACPI_SUPPORT_PROTOCOL       *AcpiSupportProtocol = NULL;
  EFI_HANDLE                      SwHandle = NULL;
  EFI_HANDLE                      hNewHandle = NULL;
  UINTN                           BufferSize;
  UINTN                           HestTblHandle;
  UINT64                          Address64;
  UINT32                          Value32;
  //
  // Initialize Local Variables
  //
  InSmm = FALSE;
  BufferSize = 0;
  HestTblHandle = 0;

  //
  // Initialize Global Variables
  //
  EfiInitializeApei (ImageHandle, SystemTable);

  // Locate SMM Base Protocol
  Status  = gBS->LocateProtocol (
                                 &gEfiSmmBaseProtocolGuid,  // IN EFI_GUID Published unique identifier of requested protocol GUID to locate
                                 NULL,                      // IN VOID* Published unique identifier of requested protocol GUID
                                 &SmmBase                   // OUT VOID** Returns address of pointer for SMM Base protocol interface
                                 );

  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;     // Error detected while trying to locate SMM Base Protocol
  }

  // Save the SMM Base system table pointer
  SmmBase->GetSmstLocation (SmmBase, &gSmst);

  // Determine if we are in SMM or not
  SmmBase->InSmm (SmmBase, &InSmm);

  if (!InSmm) {
    // Query image handle to see if it supports a requested protocol and then get an address of the protocol image to be loaded
    Status = gBS->HandleProtocol (
                                  ImageHandle,                  // Handle of requested protocol
                                  &gEfiLoadedImageProtocolGuid, // Published unique identifier of requested protocol GUID
                                  (VOID*)&LoadedImage           // Returns address of pointer for requested protocol interface of the image
                                  );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;            // Error detected while trying to query LoadImage protocol
    }

    // Query device handle of loaded image to see if it supports a requested protocol and then get an address of the prototocl device path
    Status = gBS->HandleProtocol (
                                  LoadedImage->DeviceHandle,   // Handle of requested protocol
                                  &gEfiDevicePathProtocolGuid, // Published unique identifier of requested protocol GUID
                                  (VOID*) &ImageDevicePath     // Returns address of pointer for requested protocol interface
                                  );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }

    // Add this image's device path to the loaded image device path image and return
    // a pointer to the new file path.
    NewFilePath = AddDevicePath (
                                 ImageDevicePath,         // IN EFI_DEVICE_PATH_PROTOCOL*
                                 LoadedImage->FilePath    // IN EFI_DEVICE_PATH_PROTOCOL* Device Path Image to add
                                 );

    //Load the image in System Management RAM; it will automatically generate an SMI.
    Status = SmmBase->Register (
                                SmmBase,      // IN *This Addreess of pointer for SMM Base protocol interface)
                                NewFilePath,  // IN EFI_DEVICE_PATH_PROTOCOL
                                NULL,         // IN SourceBuffer OPTIONAL
                                0,            // IN SourceSize OPTIONAL
                                &hEinj,       // OUT *ImageHandle
                                FALSE         // IN LegacyIA32Binary OPTIONAL
                                );

    // Free up residual heap memory if not equal to NULL
    if (NewFilePath) {
      gBS->FreePool (NewFilePath);
    }

    if (EFI_ERROR (Status)) {
      switch (EFIERR (Status))
      {
      case EFI_LOAD_ERROR:
        break;
      case EFI_INVALID_PARAMETER:
        break;
      case EFI_UNSUPPORTED:
        break;
      default:
        ;
      }
      return Status;
    }
  } else {
    // We are in SMM...
    // Allocate Memory for type Runtime Services Data - APEI Module Private Data
    Status = gBS->AllocatePool (EfiRuntimeServicesData, sizeof (APEI_DRIVER_PRIVATE_DATA), &mApeiPrivData);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    // Allocate Memory for type Runtime Services Data - APEI Interface
    Status = gBS->AllocatePool (EfiRuntimeServicesData, sizeof (AMD_APEI_INTERFACE), &mApeiInterface);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    // Locate SW SMM Dispatch Protocol
    Status  = gBS->LocateProtocol (
                                   &gEfiSmmSwDispatchProtocolGuid, // IN EFI_GUID Published unique identifier of requested protocol GUID to locate
                                   NULL,                           // IN VOID* Published unique identifier of requested protocol GUID
                                   &SwDispatch                     // OUT VOID** Returns address of pointer for SMM Base protocol interface
                                   );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }

    SwContext.SwSmiInputValue = EINJ_BEGIN_INJ_CMD;
    // Register EINJ SMM callback handler.
    Status = SwDispatch->Register (
                                   SwDispatch,           // IN *This
                                   ApeiEinjSwSmiHandler, // IN EFI SW SMM Dispatcher Callback Entry Point Address
                                   &SwContext,           // IN SwContext
                                   &SwHandle             // IN OUT SwHandle
                                   );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }

    SwContext.SwSmiInputValue = ERST_EXECUTE_OPERATION_CMD;
    // Register ERST SMM callback handler.
    Status = SwDispatch->Register (
                                   SwDispatch,
                                   ApeiErstSwSmiHandler,
                                   &SwContext,
                                   &SwHandle
                                   );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }

    // Allocate Memory for the the AMD_RAS_APEI_PROTOCOL protocol.
    Status = gBS->AllocatePool (
                                EfiBootServicesData,              // IN EFI_MEMORY_TYPE PoolType
                                sizeof (AMD_RAS_APEI_PROTOCOL),   // IN UINTN Size
                                &AmdRasApeiProtocol               // OUT VOID **Buffer
                                );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return EFI_OUT_OF_RESOURCES;    // Error detected while trying to locate SMM Base Protocol
    }

    EfiCommonLibZeroMem (AmdRasApeiProtocol, sizeof (AMD_RAS_APEI_PROTOCOL));

    // Initialize BERT
    ApeiBertInit ();
    // Initialize HEST
    ApeiHestInit ();
    // Determine if we have ECC error enable bits set
    Address64 = mCfgMmioBase | ((0x18 << 15) | (3 << 12) | 0x44); //F3:44 MC4_NB_CFG
    Value32 = RasSmmReadMem32 (Address64);
    if (Value32 & (1 << 22)) {
      //Initialize EINJ if ECC is Enabled
      ApeiEinjInit ();
    }
    // Initialize ERST
    ApeiErstInit ();

    AmdRasApeiProtocol->AmdApeiInterface = mApeiInterface;
    mApeiInterface->ApeiPrivData = mApeiPrivData;

    AmdRasApeiProtocol->AddBootErrorRecordEntry = 0;
    AmdRasApeiProtocol->AddHestErrorSourceEntry = 0;

    Status = gBS->InstallProtocolInterface (
                                            &Handle,                  // IN OUT EFI_HANDLE
                                            &gAmdRasApeiProtocolGuid, // IN EFI_GUID
                                            EFI_NATIVE_INTERFACE,     // IN EFI_INITERFACE_TYPE
                                            AmdRasApeiProtocol        // IN VOID* Interface
                                            );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return EFI_PROTOCOL_ERROR;    // Error detected while trying to locate SMM Base Protocol
    }
  } // End-- if(!InSmm)

  //
  // Do more non-SMM configuration
  //
  if (!InSmm) {
    Status = gBS->LocateProtocol (
                                  &gAmdRasApeiProtocolGuid,
                                  NULL,
                                  &AmdRasApeiProtocol
                                  );

    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return EFI_PROTOCOL_ERROR;   // Error detected while trying to locate SMM Base Protocol
    }

    mApeiInterface = AmdRasApeiProtocol->AmdApeiInterface;
    mApeiPrivData = mApeiInterface->ApeiPrivData;

    // Initialize function pointers to protocol interfaces
    AmdRasApeiProtocol->AddBootErrorRecordEntry = &AddBertErrorRecord;
    AmdRasApeiProtocol->AddHestErrorSourceEntry = &AddHestErrorRecord;

    //
    //  Find the protocol that was installed during SMM phase and reinstall it during
    //  Non-SMM phase as well.
    //

    // Get the buffer size for the EFI_HANDLE
    BufferSize = sizeof (EFI_HANDLE);

    // Returns an array of handles that support a specified protocol.
    Status = gBS->LocateHandle (
                                ByProtocol,               // IN EFI_LOCATE_SEARCH_TYPE SearchType
                                &gAmdRasApeiProtocolGuid, // IN EFI_GUID *Protocol OPTIONAL
                                NULL,                     // IN VOID *SearchKey OPTIONAL
                                &BufferSize,              // IN OUT UINTN BufferSize
                                &hNewHandle               // OUT EFI_HANDLE *Buffer
                                );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return EFI_PROTOCOL_ERROR;   // Error detected while trying to locate SMM Base Protocol
    }

    // Queries a handle to determine if it supports a specified protocol.
    Status = gBS->HandleProtocol (
                                  hNewHandle,                     // IN EFI_HANDLE Handle
                                  &gAmdRasApeiProtocolGuid,       // IN EFI_GUID *Protocol
                                  (VOID **) &AmdRasApeiProtocol   // OUT VOID *NewInterface
                                  );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return EFI_PROTOCOL_ERROR;    // Error detected while trying to locate SMM Base Protocol
    }

    // Local ACPI Protocol
    Status = gBS->LocateProtocol (
                                  &gAcpiSupportGuid,
                                  NULL,
                                  (VOID **) &AcpiSupportProtocol
                                  );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return EFI_PROTOCOL_ERROR;    // Error detected while trying to locate ACPI Support Protocol
    }

    //Add HEST table to the ACPI aware OS
    Status = AcpiSupportProtocol->SetAcpiTable (
                                                AcpiSupportProtocol,                        // IN EFI_ACPI_SUPPORT_PROTOCOL *This
                                                mApeiInterface->ApeiPrivData->ApeiHestTbl,  // IN VOID  *Table OPTIONAL
                                                TRUE,                                       // IN BOOLEAN Checksum
                                                EFI_ACPI_TABLE_VERSION_ALL,                 // IN EFI_ACPI_TABLE_VERSION Version
                                                &HestTblHandle                              // IN OUT UINTN *TableHandle
                                                );
    ASSERT_EFI_ERROR (Status);

    // Reinstalls a protocol interface on a device handle.
    Status = gBS->ReinstallProtocolInterface (
                                              hNewHandle,               // IN EFI_HANDLE Handle
                                              &gAmdRasApeiProtocolGuid, // IN EFI_GUID *Protocol
                                              AmdRasApeiProtocol,       // IN VOID *OldInterface
                                              AmdRasApeiProtocol        // IN VOID *NewInterface
                                              );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return EFI_PROTOCOL_ERROR;    // Error detected while trying to locate SMM Base Protocol
    }

    // Create a callback event to be executed at ReadyToBoot to publish APEI tables to OS.
    Status = gBS->CreateEventEx (
                                 EFI_EVENT_NOTIFY_SIGNAL,
                                 EFI_TPL_NOTIFY,
                                 AddApeiTables,
                                 NULL,
                                 &gEfiEventReadyToBootGuid,
                                 &mEvtApeiReadyToBoot
                                 );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return EFI_OUT_OF_RESOURCES;    // Error detected while trying to locate SMM Base Protocol
    }
  }

  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  AddBertErrorRecord
 *
 *  Description:
 *     Adds a new Boot Error Record Entry to the APEI Boot Error Record Table (BERT).
 *
 *  Parameters:
 *    @param[in]     *ErrorRecord
 *    @param[in]     RecordLen
 *    @param[in]     ErrorType
 *    @param[in]     SeverityType
 *
 *    @retval         EFI_SUCCESS   Error record has been added to BERT table
 *                    EFI_UNSUPPORTED ErrorType passed in is unsupported
 *                    EFI_OUT_OF_RESOURCES  Could not allocate memory
 *                    EFI_VOLUME_FULL   cannot add one more error record
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
AddBertErrorRecord (
  IN UINT8  *ErrorRecord,
  IN UINT32 RecordLen,
  IN UINT8  ErrorType,
  IN UINT8  SeverityType
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  APEI_BERT_ACPI_TABLE          *BertTable;             // Local helper pointer
  APEI_BOOT_ERROR_REGION_TABLE  *NewBertErrRgnTable;
  APEI_BOOT_ERROR_REGION_TABLE *OrgBertErrRgnTable;

  if (mApeiInterface == NULL) {
    Status = EFI_NOT_FOUND;
    return Status;
  } // End-- if (mApeiInterface)

  BertTable = mApeiInterface->ApeiPrivData->ApeiBertTbl;

  // Get pointer to original Boot Error Region Ptr
  OrgBertErrRgnTable = (APEI_BOOT_ERROR_REGION_TABLE*) BertTable->BertContext.BootErrRegPtr;

  // First check to see if Error Data Entry Count has reached the maximum allowed (error count is stored in Bit[13:4])
  if ((OrgBertErrRgnTable->BlockStatus & 0x3ff0) == 0x3ff0) {
    return EFI_VOLUME_FULL;
  }

  // Reserve ACPI system memory for new BERT Error Region Table + add-on error entries (Raw or Generic)
  Status = gBS->AllocatePool (EfiACPIMemoryNVS, BertTable->BertContext.BootErrRegLen + RecordLen, &NewBertErrRgnTable);
  if (EFI_ERROR (Status))  {
    return Status;
  }

  switch (ErrorType)
  {
  case ERROR_TYPE_RAW:    // Raw data Error
    // Copy Error Region Data from Original BERT table Error Region into New BERT table
    gBS->CopyMem (NewBertErrRgnTable, OrgBertErrRgnTable, BertTable->BertContext.BootErrRegLen);
    // Copy new Error Record data at the end of the current BERT table
    gBS->CopyMem ((UINT8*) NewBertErrRgnTable + BertTable->BertContext.BootErrRegLen, ErrorRecord, RecordLen);
    // Update the RAW data length parameter with the new data length
    NewBertErrRgnTable->RawDataLen = OrgBertErrRgnTable->RawDataLen + RecordLen;
      break;
  case ERROR_TYPE_GENERIC:     // Generic Error (Note: **According to WHEA, RAW data must follow any Generic error data entries**)
    // Copy Original Error Region raw data into New BERT Error Region data
    gBS->CopyMem ( NewBertErrRgnTable, OrgBertErrRgnTable, OrgBertErrRgnTable->RawDataOffset);
    // Copy new Generic Error Record data at the end of the current BERT table Error Region Offset but before the Raw data
    gBS->CopyMem ((UINT8*) NewBertErrRgnTable + OrgBertErrRgnTable->RawDataOffset, ErrorRecord, RecordLen);
    // Copy Original Raw Data after the new Generic Error Record Data
    gBS->CopyMem ((UINT8*) (NewBertErrRgnTable + OrgBertErrRgnTable->RawDataOffset + RecordLen),  \
                 (UINT8*) (OrgBertErrRgnTable + OrgBertErrRgnTable->RawDataOffset), OrgBertErrRgnTable->RawDataLen);
    // Update the Raw data offset value
    NewBertErrRgnTable->RawDataOffset = OrgBertErrRgnTable->RawDataOffset + RecordLen;
    // Update the Generic data offset value
    NewBertErrRgnTable->GenericDataLen = OrgBertErrRgnTable->GenericDataLen + RecordLen;
    break;
  default:
    Status = EFI_UNSUPPORTED;
      return Status;
  }

  // If error type Correctable AND multiple corrected errors
  if ((SeverityType == ERROR_SEVERITY_CORRECTED) && (NewBertErrRgnTable->BlockStatus & MULT_CORR_ERROR_VALID) != 0x8) {
    // If block status correctable error valid
    if ((NewBertErrRgnTable->BlockStatus & ERROR_CORR_VALID) == 0x2) {
      NewBertErrRgnTable->BlockStatus = NewBertErrRgnTable->BlockStatus | MULT_CORR_ERROR_VALID;
      // Mask off Uncorrectable Status Block Bit[2] & Bit[0]
      NewBertErrRgnTable->BlockStatus = NewBertErrRgnTable->BlockStatus & 0xfffffffa;
      } else {
        NewBertErrRgnTable->BlockStatus = NewBertErrRgnTable->BlockStatus | ERROR_CORR_VALID;
      }
   }

  // If error type Uncorrectable AND multiple uncorrectable errors
  if (SeverityType == ERROR_SEVERITY_FATAL && (NewBertErrRgnTable->BlockStatus & MULT_UNCORR_ERROR_VALID) != 0x4) {
    // If block status uncorrectable error valid
    if ((NewBertErrRgnTable->BlockStatus & ERROR_UNCORR_VALID) == 1) {
      NewBertErrRgnTable->BlockStatus = NewBertErrRgnTable->BlockStatus | MULT_UNCORR_ERROR_VALID;
      // Mask off Correctable Status Block Bit[3] & Bit[1]
      NewBertErrRgnTable->BlockStatus = NewBertErrRgnTable->BlockStatus & 0xfffffff5;
    } else {
      NewBertErrRgnTable->BlockStatus = NewBertErrRgnTable->BlockStatus | ERROR_UNCORR_VALID;
    }
  }

  // Update Error Data Entry Count to 1 in upper nibble
  NewBertErrRgnTable->BlockStatus = NewBertErrRgnTable->BlockStatus + 0x10;

  // Set Boot Error Region Pointer to new Bert Boot Error Region memory location
  BertTable->BertContext.BootErrRegPtr = (UINT64) ((UINTN) NewBertErrRgnTable);
  // Set Boot Error Region length to new Bert Boot Error Region length
  BertTable->BertContext.BootErrRegLen = BertTable->BertContext.BootErrRegLen + RecordLen;
  // Free memory allocated for original BERT Table
  gBS->FreePool (OrgBertErrRgnTable);

  return EFI_SUCCESS;
}



/**
 *---------------------------------------------------------------------------------------
 *
 *  AddHestErrorRecord
 *
 *  Description:
 *     Adds a new Hardware Error Source Record Entry to the APEI Hardware Error Source Table (HEST).
 *
 *  Parameters:
 *    @param[in]     *ErrorRecord
 *    @param[in]     RecordLen
 *
 *    @retval         EFI_SUCCESS  Error record has been added to BERT table
 *                    EFI_UNSUPPORTED  ErrorType passed in is unsupported
 *                    EFI_OUT_OF_RESOURCES  Could not allocate memory
 *                    EFI_VOLUME_FULL  Cannot add one more error record
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
AddHestErrorRecord (
  IN UINT8  *ErrorRecord,
  IN UINT32 RecordLen
  )
{
  EFI_STATUS Status   = EFI_SUCCESS;
  APEI_HEST_ACPI_TABLE  *NewHestAcpiTable;
  APEI_HEST_ACPI_TABLE  *OrgHestAcpiTable;

  // Assign local original Hest pointer to global HEST table pointer
  OrgHestAcpiTable = mApeiInterface->ApeiPrivData->ApeiHestTbl;

  // Allocate EFI Boot Services Data Memory for a new "working" HEST table
  Status = gBS->AllocatePool (EfiBootServicesData, OrgHestAcpiTable->Header.Length + RecordLen, &NewHestAcpiTable);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Copy the entire orginial HEST table into the working table memory buffer
  gBS->CopyMem ((UINT8*) NewHestAcpiTable, OrgHestAcpiTable, OrgHestAcpiTable->Header.Length);
  // Copy the new HEST record entry to the end of working table
  gBS->CopyMem ((UINT8*) NewHestAcpiTable + OrgHestAcpiTable->Header.Length, ErrorRecord, RecordLen);

  // Increment the table record count by 1.
  NewHestAcpiTable->HestErrSourceCount = NewHestAcpiTable->HestErrSourceCount + 1;
  // Update the size of the table with the new record added
  NewHestAcpiTable->Header.Length = NewHestAcpiTable->Header.Length + RecordLen;

  // Point the global HEST table to the new HEST table pointer that has the new HEST record added to it
  mApeiInterface->ApeiPrivData->ApeiHestTbl = NewHestAcpiTable;

  // Release the memory used by the original HEST table
  gBS->FreePool (OrgHestAcpiTable);

  return EFI_SUCCESS;
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  AddApeiTables
 *
 *  Description:
 *     Event Callback that adds the APEI Tables to OS.
 *
 *  Parameters:
 *    @param[in]     Event
 *    @param[in]     *Context
 *
 *    @retval         EFI_SUCCESS   Error record has been added to BERT table
 *                    EFI_UNSUPPORTED   ErrorType passed in is unsupported
 *                    EFI_OUT_OF_RESOURCES  Could not allocate memory
 *                    EFI_VOLUME_FULL   Cannot add one more error record
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
AddApeiTables (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_ACPI_SUPPORT_PROTOCOL *AcpiSupportProtocol = NULL;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo = NULL;
  UINTN BertTblHandle;
  UINTN EinjTblHandle;
  UINT32  Value32;
  // UINTN  ErstTblHandle;

  BertTblHandle = 0;
  EinjTblHandle = 0;
  //ErstTblHandle = 0;


  // Local ACPI Protocol
  Status = gBS->LocateProtocol (
                                &gAcpiSupportGuid,
                                NULL,
                                &AcpiSupportProtocol
                                );
  if (EFI_ERROR (Status)) {
    return;
  }

  //Add BERT table to the ACPI aware OS
  Status = AcpiSupportProtocol->SetAcpiTable (
                                              AcpiSupportProtocol,                       // IN EFI_ACPI_SUPPORT_PROTOCOL *This
                                              mApeiInterface->ApeiPrivData->ApeiBertTbl, // IN VOID *Table OPTIONAL
                                              TRUE,                                      // IN BOOLEAN Checksum
                                              EFI_ACPI_TABLE_VERSION_ALL,                // IN EFI_ACPI_TABLE_VERSION Version
                                              &BertTblHandle                             // IN OUT UINTN *TableHandle
                                              );
  ASSERT_EFI_ERROR (Status);

  // Locate PCI Root Bridge Protocol
  Status = gBS->LocateProtocol (
                                &gEfiPciRootBridgeIoProtocolGuid,
                                NULL,
                                (VOID**) &PciRootBridgeIo
                                );
  if (EFI_ERROR (Status)) {
    return;
  }

  // Check to see if ECC is enabled before adding EINJ table
  PciRootBridgeIo->Pci.Read (
                             PciRootBridgeIo,
                             EfiPciWidthUint32,
                             EFI_PCI_ADDRESS (0, 0x18, 0x3, 0x44),
                             1,
                             &Value32
                             );

  if (Value32 & (1 << 22)) {
    //
    //Add EINJ table to the ACPI aware OS
    //
    Status = AcpiSupportProtocol->SetAcpiTable (
                                                AcpiSupportProtocol,                        // IN EFI_ACPI_SUPPORT_PROTOCOL *This
                                                mApeiInterface->ApeiPrivData->ApeiEinjTbl,  // IN VOID  *Table OPTIONAL
                                                TRUE,                                       // IN BOOLEAN Checksum
                                                EFI_ACPI_TABLE_VERSION_ALL,                 // IN EFI_ACPI_TABLE_VERSION Version
                                                &EinjTblHandle                              // IN OUT UINTN *TableHandle
                                                );
    ASSERT_EFI_ERROR (Status);
  }

  // Uncomment code below to publish ERST Table to OS
  //Add ERST table
  //Status = AcpiSupportProtocol->SetAcpiTable (
  //                                            AcpiSupportProtocol,                       // IN EFI_ACPI_SUPPORT_PROTOCOL *This
  //                                            mApeiInterface->ApeiPrivData->ApeiErstTbl, // IN VOID *Table OPTIONAL
  //                                            TRUE,                                      // IN BOOLEAN Checksum
  //                                            EFI_ACPI_TABLE_VERSION_ALL,                // IN EFI_ACPI_TABLE_VERSION Version
  //                                            &ErstTblHandle                             // IN OUT UINTN *TableHandle
  //                                            );
  //
  //ASSERT_EFI_ERROR (Status);

  // Close the APEI ready2boot event
  gBS->CloseEvent (mEvtApeiReadyToBoot);
  return;
}
