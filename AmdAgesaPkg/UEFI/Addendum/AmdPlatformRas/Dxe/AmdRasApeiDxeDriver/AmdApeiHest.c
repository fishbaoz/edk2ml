/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD APEI HEST for RAS
 *
 * AGESA DXE Driver to perform APEI tasks
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
 * Description: AmdRasApeiDxeDriver.c - AGESA DXE Driver to perform APEI tasks.
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
#include "EfiDriverLib.h"
#include "AmdApei.h"
#include "AmdApeiHest.h"
#include "AmdApeiEinj.h"


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
extern APEI_DRIVER_PRIVATE_DATA   *mApeiPrivData;
UNCORR_MEM_ERR_STS_BLK            *mUncrErrorBlk;
CORR_MEM_ERR_STS_BLK              *mCrErrorBlk;


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */



/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */



/*----------------------------------------------------------------------------------------
 *                          T A B L E    D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */
APEI_HEST_ACPI_TABLE mApeiHestTable = {

  //EFI_ACPI_DESCRIPTION_HEADER
  {HEST_SIG, sizeof (APEI_HEST_ACPI_TABLE), ACPI_REV1, 0, "AMD", AMD_HEST, 0, 0, 0},
  0x02,                           //ErrorSourceCount - Number of error records defined in this table

  //
  // HEST Source ID #0 - Uncorrectable Generic Error Source
  //
  {{  // GENERIC_HW_ERR_SOURCE
    GEN_HW_ERR_TYPE,              // UINT16 GenSourceType   = 0x0009
    SOURCE_ID0,                   // UINT16 SourceID Number = 0x0000
    RELATED_SOURCE_ID_NO_ALT,     // UINT16 RelatedSrcID   No alternate source = 0xFFFF
    RECORD_SECTION_ENABLE,        // UINT8  Flags           = 0x03
    ERR_SOURCE_ENABLED,           // UINT8  Enabled         = 0x01
    RECORDS_PREALLOCATE1,         // UINT32 NumRecPreAlloc  = 0x00000001
    SECTIONS_PER_RECORD1,         // UINT32 MaxSection      = 0x00000001
    sizeof (UNCORR_MEM_ERR_STS_BLK) - sizeof (UINT64), // UINT32  MaxRawDataLen subtract sizeof (UINT64) address
    { // EFI_ACPI_5_0_GAS - Generic Address Structure
      APEI_ENTRY_ADDRID_SYSMEM,   // UINT8  AddressSpaceId SysMem AddressID = 0x00
      REG_BIT_WIDTH_QWORD,        // UINT8  RegisterBitWidth = 0x40 = 64d
      REG_BIT_OFFSET0,            // UINT8  RegisterBitOffset = 0x00
      APEI_ACCESS_QWORD,          // UINT8  AccessSize = 0x04 = QWORD
      0                           // UINT64 Address
    },
    {  // HW_ERROR_NOTIFY
      NOTIFY_TYPE_NMI,            // UINT8  NotifiyType NMI = 0x04
      sizeof (HW_ERROR_NOTIFY),   // UINT8  ErrNotifyLen
      0,                          // UINT16 ConfigWrite
      0,                          // UINT32 PollInterval
      INT_VECTOR_NMI,
      0,                          // UINT32 SwitchPollingThreshVal
      0,                          // UINT32 SwitchPollingThreshWindow
      0,                          // UINT32 ErrThresholdVal;
      0                           // UINT32 ErrThresholdWindow
    },
    sizeof (UNCORR_MEM_ERR_STS_BLK) - sizeof (UINT64) // UINT32 ErrorStatusBlockLen subtract sizeof (UINT64) address
  },
  //
  // HEST Source ID #1 - Correctable Generic Error Source
  //
  {  // GENERIC_HW_ERR_SOURCE
    GEN_HW_ERR_TYPE,              // UINT16 GenSourceType   = 0x0009
    SOURCE_ID1,                   // UINT16 SourceID Number = 0x0000
    RELATED_SOURCE_ID_NO_ALT,     // UINT16 RelatedSrcID   No alternate source = 0xFFFF
    RECORD_SECTION_ENABLE,        // UINT8  Flags           = 0x03
    ERR_SOURCE_ENABLED,           // UINT8  Enabled         = 0x01
    RECORDS_PREALLOCATE1,         // UINT32 NumRecPreAlloc  = 0x00000001
    SECTIONS_PER_RECORD1,         // UINT32 MaxSection      = 0x00000001
    sizeof (CORR_MEM_ERR_STS_BLK) - sizeof(UINT64), // UINT32 MaxRawDataLen subtract sizeof (UINT64) address
    {  // EFI_ACPI_5_0_GAS - Generic Address Structure
      APEI_ENTRY_ADDRID_SYSMEM,   // UINT8  AddressSpaceId  SysMem AddressID = 0x00
      REG_BIT_WIDTH_QWORD,        // UINT8  RegisterBitWidth = 0x40 = 64d
      REG_BIT_OFFSET0,            // UINT8  RegisterBitOffset = 0x00
      APEI_ACCESS_QWORD,          // UINT8  AccessSize  QWORD = 0x04
      0                           // UINT64 Address
    },
    {  // HW_ERROR_NOTIFY
      NOTIFY_TYPE_POLLED,         // UINT8  NotifiyType Polling = 0x00
      sizeof (HW_ERROR_NOTIFY),   // UINT8  ErrNotifyLen
      0x3e,                       // UINT16 ConfigWrite
      1000,                       // UINT32 PollInterval
      INT_VECTOR_NMI,
      0,                          // UINT32 SwitchPollingThreshVal
      0,                          // UINT32 SwitchPollingThreshWindow
      0,                          // UINT32 ErrThresholdVal;
    0                             // UINT32 ErrThresholdWindow
  },
  sizeof (CORR_MEM_ERR_STS_BLK) - sizeof (UINT64) // UINT32 ErrorStatusBlockLen subtract sizeof (UINT64) address
  }}
};


//
// Uncorrectable Error Status Block
//
UNCORR_MEM_ERR_STS_BLK UncorrMemErrStsBlk = {
  0,                                                          // UINT64 ErrStsPtr
  {  // Generic Error Status Block
    0,                                                        // WHEA Platform Design Guide Table 4-13 Block Status
    0,                                                        // WHEA Platform Design Guide Table 4-13 Raw Data Offset
    0,                                                        // WHEA Platform Design Guide Table 4-13 Raw Data Length
    sizeof (GEN_ERR_DATA_ENTRY) + sizeof (PLATFORM_MEM_ERR_SEC), // WHEA Platform Design Guide Table 4-13 Data Length
    ERROR_SEVERITY_FATAL                                      // WHEA Platform Design Guide Table 4-13 Error Severity
  },
  {  // Uncorrectable Generic Data Entry
    PLATFORM_MEMORY_SECT_GUID,                                // UEFI 2.3.1 Section N 2.5 Memory Error Section Type 16-byte GUID
    ERROR_SEVERITY_FATAL,                                     // WHEA Platform Design Guide Table 4-14 Error Severity
    REVISION_HEST,                                            // UEFI 2.3.1 Section N Revision Field
    FRU_STRING_VALID,                                         // UEFI 2.3.1 Section N Section Descriptor Validation Bits field
    0x1,                                                      // UEFI 2.3.1 Section N Section Descriptor Flags
    sizeof (PLATFORM_MEM_ERR_SEC),                            // Generic Error Data Block Length
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                          // FRU ID - UEFI 2.3.1 Section N
    'U','n','c','o','r','r','e','c','t','e','d','E','r','r'   // FRU Text - UEFI 2.3.1 Section N
  },
  {  // Platform Memory Error Secion
    MEM_VALID_BIT_MAP,                                        // UEFI 2.3.1 Section N Table 245 Bit[0]-ErrSts Valid, Bit[3]-Node Valid, Bit[14]-Memory Error Type Valid
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,                              // Card,Node, Module, etc. Data filled in during error detection/logging
    PLATFORM_MEM_ERROR_TYPE_MULTI_BIT_ECC                     // UEFI 2.3.1 Section N 2.5 Table 245 Offset 72, Memory Error Type = 3 = Multi-bit ECC
  }
};

//
// Correctable Error Status Block
//
CORR_MEM_ERR_STS_BLK CorrMemErrStsBlk = {
  0,                                                          // UINT64 ErrStsPtr
  {  // Generic Error Status Block
    0,                                                        // WHEA Platform Design Guide Table 4-13 Block Status
    0,                                                        // WHEA Platform Design Guide Table 4-13 Raw Data Offset
    0,                                                        // WHEA Platform Design Guide Table 4-13 Raw Data Length
    sizeof (GEN_ERR_DATA_ENTRY) + sizeof (PLATFORM_MEM_ERR_SEC), // WHEA Platform Design Guide Table 4-13 Data Length
    ERROR_SEVERITY_CORRECTED                                  // WHEA Platform Design Guide Table 4-13 Error Severity
  },
  {  // Uncorrectable Generic Data Entry
    PLATFORM_MEMORY_SECT_GUID,                                // UEFI 2.3.1 Section N 2.5 Memory Error Section Type 16-byte GUID
    ERROR_SEVERITY_CORRECTED,                                 // WHEA Platform Design Guide Table 4-14 Error Severity
    REVISION_HEST,                                            // UEFI 2.3.1 Section N Revision Field
    FRU_STRING_VALID,                                         // UEFI 2.3.1 Section N Section Descriptor Validation Bits field
    0x1,                                                      // UEFI 2.3.1 Section N Section Descriptor Flags
    sizeof (PLATFORM_MEM_ERR_SEC),                            // Generic Error Data Block Length
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                          // FRU ID - UEFI 2.3.1 Section N
    'C','o','r','r','e','c','t','e','d','E','r','r'           // FRU Text - UEFI 2.3.1 Section N
  },
  {  // Platform Memory Error Secion
    MEM_VALID_BIT_MAP,                                        // UEFI 2.3.1 Section N Table 245 Bit[0]-ErrSts Valid, Bit[3]-Node Valid, Bit[14]-Memory Error Type Valid
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,                              // Card,Node, Module, etc. Data filled in during error detection/logging
    PLATFORM_MEM_ERROR_TYPE_SINGLE_BIT_ECC                    // UEFI 2.3.1 Section N 2.5 Table 245 Offset 72, Memory Error Type = 2  Single-bit ECC
  }
};

/**
 *---------------------------------------------------------------------------------------
 *
 *  ApeiHestInit
 *
 *  Description:
 *     Initialize ACPI APEI HEST table.
 *
 *  Parameters:
 *    @retval        EFI_SUCCESS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
ApeiHestInit ( VOID )
{
  EFI_STATUS Status;
  UINTN ErrorBlockLen;
  UINT8 *ErrorBlock;
  APEI_HEST_ACPI_TABLE *HestTable;

  // Reserve 2048-bytes to store error status block data
  ErrorBlockLen = 0x800;

  // Allocate reserverd memory for Error Block data so we can get its address
  Status = gBS->AllocatePool (EfiReservedMemoryType, ErrorBlockLen, &ErrorBlock);
  if (EFI_ERROR (Status)) {
    // Not enough memory avaialable, so bail...
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // Error Status address for Uncorrected error
  //
  mApeiHestTable.ErrorSourceStruct.UncorrGenErrSource.RegisterRegion.Address = (UINT64) ((UINTN) ErrorBlock);
  // Assign global Uncorrectable error pointer to local error block
  mUncrErrorBlk = (UNCORR_MEM_ERR_STS_BLK*) ErrorBlock;

  // Copy Uncorrectable Error Stutus Block entry into the EFI Reserved Memory
  gBS->CopyMem (ErrorBlock, &UncorrMemErrStsBlk, sizeof (UNCORR_MEM_ERR_STS_BLK));

  // Get the address of the Uncorrectable Error Status Block and store it in the ErrorStatusPtr
  ((UNCORR_MEM_ERR_STS_BLK*) ErrorBlock)->ErrorStatusPtr = (UINT64) (UINTN) &(((UNCORR_MEM_ERR_STS_BLK*) ErrorBlock)->UnCorrectGenErrStsBlk);

  //
  // Move the pErrorBlock pointer to the next Error Status Block Status record
  //
  ErrorBlock = ErrorBlock + sizeof (UNCORR_MEM_ERR_STS_BLK);
  // Put the Error block on a 16-bit boundary
  ErrorBlock = (UINT8 *) (((UINTN) ErrorBlock + 0x10) & ~0x0f);

  //
  // Error Status address for Corrected error - The address holds the error status block
  //
  mApeiHestTable.ErrorSourceStruct.CorrGenErrSource.RegisterRegion.Address = (UINT64) ((UINTN) ErrorBlock);
  // Assign global Correctable error pointer to error block pointer
  mCrErrorBlk = (CORR_MEM_ERR_STS_BLK*) ErrorBlock;

  // Copy Correctable Error Status Block into the EFI Reserved Memory
  gBS->CopyMem (ErrorBlock, &CorrMemErrStsBlk, sizeof (CORR_MEM_ERR_STS_BLK));
  // Get the address of the Uncorrectable Error Status Block and store it in the ErrorStatusPtr
  ((CORR_MEM_ERR_STS_BLK*) ErrorBlock)->ErrorStatusPtr = (UINT64) (UINTN) &(((CORR_MEM_ERR_STS_BLK*) ErrorBlock)->CorrectGenErrStsBlk);

  // Allocate Boot Services memory for APEI HEST table
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (APEI_HEST_ACPI_TABLE), &HestTable);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Copy APEI HEST ACPI table to boot services memory space
  gBS->CopyMem (HestTable, &mApeiHestTable, sizeof (APEI_HEST_ACPI_TABLE));

  // Assign the address to the APEI_DRIVER_PRIVATE_DATA HEST table parameter
  mApeiPrivData->ApeiHestTbl = HestTable;

  return EFI_SUCCESS;
}