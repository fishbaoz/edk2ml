/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD RAS Reset DXE Driver
 *
 * AGESA DXE Driver to perform Mid and Late
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI/Addendum/AmdPlatformRas
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ****************************************************************************
 * AMD Generic Encapsulated Software Architecture
 *
 * Description: AmdRasIntDxeDriver.c - AGESA DXE Driver to perform Mid and Late
 * Init.
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

/*
 **********************************************************************
 *
 * Name:  AMDRASRESETDXEDRIVER.c
 *
 * Description:
 *    UEFI DXE driver to log errors that were caused prior to a sync-flood
 *    reset, such as a multi-bit DRAM error.  This DXE driver is used in
 *    conjunction with AmdRasResetPeim PEI module.
 *
 * **********************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AmdRasResetDxeDriver.h"
#include "CpuFuncs.h"
#include "EfiDriverLib.h"
#include "EfiHobLib.h"
#include "AmdApeiBert.h"
#include "AmdApei.h"
#include "AcpiCommon.h"
#include "DimmMapping.h"
#include "AGESA.h"                  // Make sure this is listed last

//GUID Definitions
#include EFI_GUID_DEFINITION (Hob)
#include EFI_GUID_DEFINITION (AmdRasInfoHob)

// Protocol Consumer Definitions
#include EFI_PROTOCOL_CONSUMER (CpuIo)
#include EFI_PROTOCOL_CONSUMER (AmdRasApeiProtocol)
#include EFI_PROTOCOL_CONSUMER (AmdAgesaMiscServicesProtocol)

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
EFI_GUID    gAmdRasInfoHobGuid        = AMD_RAS_INFO_HOB_GUID;
EFI_GUID    gAmdRasApeiProtocolGuid   = AMD_RAS_APEI_PROTOCOL_GUID;
EFI_GUID    gEfiCpuIoProtocolGuid     = EFI_CPU_IO_PROTOCOL_GUID;
EFI_GUID    gAmdAgesaDxeMiscServicesProtocolGuid = AMD_AGESA_MISC_SERVICES_PROTOCOL_GUID;


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
STATIC EFI_RUNTIME_SERVICES   *m_pRT;
UINT32                        mCfgMmioBase;  // Extended Config Space Base address
UINT8                         mNumNodes;
EFI_CPU_IO_PROTOCOL           *mCpuIoProtocol;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

EFI_DRIVER_ENTRY_POINT (AmdRasResetDxeDriverEntryPoint);

// Generic Memory Error for BERT entries
BERT_GENERIC_MEM_ERR_ENTRY gBertGenMemErrEntry = {{
  {0xA5BC1114, 0x6F64, 0x4EDE, 0xB8, 0x63, 0x3E, 0x83, 0xED, 0x7C, 0x83, 0xB1},   // UEFI 2.3.1 Section N 2.5 Memory Error Section Type 16-byte GUID
  ERROR_SEVERITY_FATAL,                                       // UINT32 WHEA Platform Design Guide Table 4-14 Error Severity
  REVISION_BERT,                                              // UINT16 UEFI 2.3.1 Section N Revision Field
  FRU_STRING_VALID,                                           // UINT8 UEFI 2.3.1 Section N Section Descriptor Validation Bits field
  0x1,                                                        // UINT8 UEFI 2.3.1 Section N Section Descriptor Flags
  sizeof (PLATFORM_MEM_ERR_SEC),                              // UINT32 Generic Error Data Block Length
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                            // UINT8[16] FRU ID - UEFI 2.3.1 Section N
  'U','n','c','o','r','r','e','c','t','e','d','E','r','r'},   // UINT8 [20] FRU Text[20] - UEFI 2.3.1 Section N
  {MEM_VALID_BIT_MAP,                                         // 0x4009 UEFI 2.3.1 Section N Table 245 Bit[14]-Memory Error Type Valid, Bit[3]-Node Valid, Bit[0]-ErrSts Valid (0xC34D)
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                // Card,Node, Module, Bank, etc.
  MEM_TYPE_MULTI_BIT_ECC}                                     // Memory Error Type = 3 = Multi-bit ECC
};


/**
 *---------------------------------------------------------------------------------------
 *
 *  EfiInitializeRasIntLib
 *
 *  Description:
 *     Entry point for the cpu driver.
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
EfiInitializeRasResetLib (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS Status;
  gST = SystemTable;

  ASSERT (gST != NULL);

  gBS = gST->BootServices;
  m_pRT = gST->RuntimeServices;

  ASSERT (gBS != NULL);
  ASSERT (m_pRT != NULL);

  Status = gBS->LocateProtocol (
                                &gEfiCpuIoProtocolGuid,
                                NULL,
                                (VOID**)&mCpuIoProtocol
                                );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  UpdateGlobalVars
 *
 *  Description:
 *     Update global variables.
 *
 *  Parameters:
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
UpdateGlobalVars ( VOID )
{
  EFI_STATUS Status;
  UINT64    MsrVal;
  UINT32    MmioAddr;
  UINT32    MmioVal;

  MsrVal = 0;
  MmioAddr = 0;
  MmioVal = 0;

  // Initialize Mmio Config Base Address global variable
  MsrVal = EfiReadMsr (AMD_MSR_MMIO_CFG_BASE);
  mCfgMmioBase = (UINT32) MsrVal & 0xfff00000;

  // Extended Config Space (Bus 0, D0x18,Func 0, Offset 0x60) [0x?00C0060]
  MmioAddr = mCfgMmioBase | (DEVICE_24 << 15) | (FUNCTION_0 << 12) | 0x60; //F0:60

  Status = mCpuIoProtocol->Mem.Read (
                                    mCpuIoProtocol,
                                    EfiCpuIoWidthUint32,
                                    MmioAddr,
                                    1,
                                    &MmioVal
                                    );

  // Get number of nodes in the system from Bits[6:4]
  mNumNodes = (UINT8) (((MmioVal >> 4) & 0x7) + 1);

  return;
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  AgesaDxeDriverEntryPoint
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
AmdRasResetDxeDriverEntryPoint (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS                        Status;
  AMD_RAS_INFO_HOB                  *RasInfoHob;
  AMD_CPU_RAS_SOCKET                *RasCpuSocket;
  AMD_MCA_STAT_DESCRIPTOR           *McaStatDesc;
  APEI_BOOT_ERROR_REGION_TABLE      *BertErrRgnPtr;
  BERT_GENERIC_MEM_ERR_ENTRY        *GenericMemErrEntry;
  APEI_DRIVER_PRIVATE_DATA          *ApeiPrivData;
  AMD_RAS_APEI_PROTOCOL             *AmdRasApeiProtocol;
  AMD_AGESA_MISC_SERVICES_PROTOCOL  *AmdAgesaMiscServProtocol = NULL;
  AMD_IDENTIFY_DIMM                 AmdLocateDimm;
  APEI_BERT_ACPI_TABLE              *ApeiBertAcpiTable;
  UINT8                             *RasInfoHobBuffer;
  UINTN                             HobBufferSize;
  VOID                              *HobList;
  UINT32                            McCtl;
  UINT32                            McStsHi;
  UINT32                            McStsLow;
  UINT32                            NbAddrLo;
  UINT32                            NbAddrHi;

  // Initialize local variables
  Status = EFI_SUCCESS;
  RasInfoHob = NULL;
  RasCpuSocket = NULL;
  McaStatDesc = NULL;
  BertErrRgnPtr = NULL;
  GenericMemErrEntry = NULL;
  ApeiPrivData = NULL;
  AmdRasApeiProtocol = NULL;
  RasInfoHobBuffer = NULL;

  //
  // Initialize Global Variable
  //
  EfiInitializeRasResetLib (ImageHandle, SystemTable);

  UpdateGlobalVars(); // Update global variables

  EfiZeroMem (&AmdLocateDimm, sizeof (AMD_IDENTIFY_DIMM));

  Status = gBS->AllocatePool (
                              EfiBootServicesData,
                              MAX_SIZEOF_AMD_RAS_INFO_HOB_BUFFER,
                              &RasInfoHobBuffer
                              );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return EFI_OUT_OF_RESOURCES;  // Error detected while trying to Allocate memory
  }

  Status = EfiLibGetSystemConfigurationTable (&gEfiHobListGuid, (VOID**) &HobList);

  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return EFI_OUT_OF_RESOURCES;  // Error detected while trying to locate HobList
  }

  // Check for a valid HOB list pointer
  if (HobList == NULL) {
    return EFI_NOT_FOUND;
  }

  // Search the HOB list for the AMD_RAS_INFO_HOB
  Status = GetNextGuidHob (&HobList, &gAmdRasInfoHobGuid, (VOID **) &RasInfoHobBuffer, &HobBufferSize);

  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return EFI_NOT_FOUND;   // Error detected while trying to locate requested HOB
  }

  RasInfoHob = (AMD_RAS_INFO_HOB *) &RasInfoHobBuffer[0];
  // A valid error has been flagged so continue with decoding and updating BERT.
  RasCpuSocket = (AMD_CPU_RAS_SOCKET *) &RasInfoHob->RasCpuSocket;        // RasCpuSocket points to first index of the RasCpuSocket array
  McaStatDesc = (AMD_MCA_STAT_DESCRIPTOR *) &RasCpuSocket->McaStatDesc;   // McaStatDesc points to first index of the McaStatDesc array

  McCtl = (UINT32) McaStatDesc->MC_Control;
  McStsHi = (UINT32) McaStatDesc->MC_StatusHi;
  McStsLow = (UINT32) McaStatDesc->MC_StatusLow;
  NbAddrLo = (UINT32) McaStatDesc->MC_AddressLo;
  NbAddrHi = (UINT32) McaStatDesc->MC_AddressHi;
  AmdLocateDimm.MemoryAddress = (UINT64) NbAddrHi << 32 | NbAddrLo;

  //
  // Check to see if a valid error is even present before going any further
  //
  if (RasInfoHob->ErrorPresent == FALSE) {
    return EFI_SUCCESS;
  }

  // We obviously have an error present since we made it to here so get the
  // pointer to BERT table via the APEI protocol interface so we can log this
  // error properly.
  Status = gBS->LocateProtocol (
                                &gAmdRasApeiProtocolGuid,
                                NULL,
                                (VOID**)&AmdRasApeiProtocol
                                );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ApeiPrivData = AmdRasApeiProtocol->AmdApeiInterface->ApeiPrivData;

  ApeiBertAcpiTable = ApeiPrivData->ApeiBertTbl;  // Assign global BERT table pointer

  if (ApeiBertAcpiTable == NULL) {
    return EFI_SUCCESS;
  }

  BertErrRgnPtr = (APEI_BOOT_ERROR_REGION_TABLE*) ApeiBertAcpiTable->BertContext.BootErrRegPtr;

  if (BertErrRgnPtr == NULL) {
    return EFI_SUCCESS;
  }

  // Use the error code to determine how to dispatch the logging
  switch (RasInfoHob->ErrorCode) {
  case UNCORR_MC4_MEM:
    // MC Status Valid bit (Bit[63]) set?
    if (McStsHi & (1 << 31)) {
        // Check to see if error is uncorrectable (Bit[61] is set ? [Bit[29] in 32-bit variable]
        if (McStsHi & (1 << 29)) {
          // Check to see if error is uncorrectable ECC UECC (Bit[45] is set ? [Bit[13] in 32-bit variable]
          if (McStsHi & (1 << 13)) {
            // Error Address != 0?
            if (AmdLocateDimm.MemoryAddress) {
              Status = gBS->LocateProtocol (
                                            &gAmdAgesaDxeMiscServicesProtocolGuid,
                                            NULL,
                                            (VOID**)&AmdAgesaMiscServProtocol
                                            );
              if (EFI_ERROR (Status)) {
                return Status;
              }

              Status = AmdAgesaMiscServProtocol->GetDimmInfo (
                                                              AmdAgesaMiscServProtocol,
                                                              &AmdLocateDimm
                                                              );

              BertErrRgnPtr->BlockStatus = 0x11; //Error Data Entry count = 1, Uncorrected Error = 1
              BertErrRgnPtr->ErrorSeverity = ERROR_SEVERITY_FATAL;
              GenericMemErrEntry = (BERT_GENERIC_MEM_ERR_ENTRY*) &gBertGenMemErrEntry;   // Local ptr to BERT Mem Error Table
              GenericMemErrEntry->GenErrorDataEntry.ErrorSeverity = ERROR_SEVERITY_FATAL;  // 0x01;
              GenericMemErrEntry->MemErrorSection.PhyAddr = AmdLocateDimm.MemoryAddress;
              GenericMemErrEntry->MemErrorSection.Node = (UINT16) AmdLocateDimm.SocketId;
              GenericMemErrEntry->MemErrorSection.Bank = (UINT16) AmdLocateDimm.Bank;
              GenericMemErrEntry->MemErrorSection.Row = (UINT16) AmdLocateDimm.Row;
              GenericMemErrEntry->MemErrorSection.Column = (UINT16) AmdLocateDimm.Column;
              GenericMemErrEntry->MemErrorSection.Module = (UINT16) (UINT16) DimmNum[AmdLocateDimm.SocketId][AmdLocateDimm.MemChannelId][AmdLocateDimm.DimmId];
              GenericMemErrEntry->MemErrorSection.Device = (UINT16) AmdLocateDimm.ChipSelect;
            } // End-- Error Address != 0?

            if (Status == EFI_NOT_FOUND) {
              EfiAsciiStrCpy(GenericMemErrEntry->GenErrorDataEntry.FRU_Text, "DIMM# Not Sourced");  // Error Fru Text String
            } else {
              EfiAsciiStrCpy(GenericMemErrEntry->GenErrorDataEntry.FRU_Text, "DIMM# Sourced"); // New Fru Text String
            }
            GenericMemErrEntry->MemErrorSection.ErrStatus = ((1 << 18) | (4 << 8));   // Error Detected on Data Transaction | Internal DRAM Error (0x40400)
            // Also remember to update MEM_VALID_BITMAP
            GenericMemErrEntry->MemErrorSection.MemErrType = MEM_TYPE_MULTI_BIT_ECC;  // Multi-bit ECC Error type = 3

            // Add a new record to the BERT table
            Status = AmdRasApeiProtocol->AddBootErrorRecordEntry((UINT8*)GenericMemErrEntry,          // IN UINT8* pErrRecord
                                                                 sizeof(BERT_GENERIC_MEM_ERR_ENTRY),  // IN UINT nSize
                                                                 ERROR_TYPE_GENERIC,                  // IN UINT8 ErrorType - GENERIC error type
                                                                 ERROR_SEVERITY_FATAL                 // IN UINT8 SeverityType - NON-CORRECTABLE
                                                                 );
            if (EFI_ERROR (Status)) {
              ASSERT_EFI_ERROR (Status);
              return EFI_SUCCESS;   // Error detected while trying to locate SMM Base Protocol
            }
          } // End-- if (McStsHi & (1 << 13))
        } // end-- if (McStsHi & (1 << 29))
      } // End-- if(McStsHi & 0x80000000)
      break;
    case UNCORR_MC4_L3:
      return EFI_UNSUPPORTED;
    case UNCORR_MC4_LINKS:
      return EFI_UNSUPPORTED;
    case UNCORR_CORE:
      return EFI_UNSUPPORTED;
    case UNCORR_GNB_PARITY:
      return EFI_UNSUPPORTED;
    case UNCORR_SB_PARITY:
      return EFI_UNSUPPORTED;
    case UNCORR_SATA:
      return EFI_UNSUPPORTED;
    case UNCORR_USB:
      return EFI_UNSUPPORTED;
    default:
      return EFI_UNSUPPORTED;
  } //End-- switch(RasInfoHob->ErrorCode)

  return EFI_SUCCESS;
}

