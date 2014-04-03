/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD APEI Error Injection for RAS
 *
 * AMD APEI DXE Driver to perform Error Injection tasks
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
 * Description: AmdApeiEinj.c - AMD APEI DXE Driver to perform Error Injection tasks.
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
#include "AmdApeiEinj.h"
#include "AmdApei.h"
#include "EfiDriverLib.h"
#include "CpuFuncs.h"

#include EFI_PROTOCOL_DEFINITION (SmmBase)

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
APEI_EINJ_REGISTERS               *mEinjActMemRegAddr;
UINT8                             *mTrigErrorAct;
UINT32                            mCfgMmioBase;         // Extended Config Space Base address


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


APEI_EINJ_ACPI_TABLE mEinjTable = {

  //AcpiHeader
  { EINJ_SIG, sizeof (APEI_EINJ_ACPI_TABLE), ACPI_REV1, 0, "AMD", AMD_EINJ, 0, 0, 0},

  //EINJ Serialization Header
  {
    sizeof (APEI_SERIALIZATION_HEADER),    // Injection Header Size
    APEI_RESERVED,                          // Injection Flags = RESERVED
    {
      APEI_RESERVED,                        // RESERVED
      APEI_RESERVED,                        // RESERVED
      APEI_RESERVED                         // RESERVED
    },
    APEI_EINJ_INSTR_ENTRIES                 // APEI_EINJ_INSTR_ENTRIES
  },
  {
    //EinjBeginInject
    {
      BEGIN_INJECT_OPERATION,               // Injection Action
      APEI_WRITE_REG_VALUE,                 // Instruction
      APEI_CLEAR_FLAG,                      // Flags
      APEI_RESERVED,                        // Reserved
      { // Generic Address Region
        APEI_ENTRY_ADDRID_SYSMEM,           // AddressSpaceID = System Memory
        REG_BIT_WIDTH_BYTE,                 // Register Bit Width
        REG_BIT_OFFSET0,                    // Register Bit Offset
        APEI_ACCESS_BYTE,                   // Access Size
        0                                   // Address
      },
      0,                                    // Value
      APEI_BYTE_MASK                        // Mask
    },

    //EinjGetTriggerErrActionTable
    {
      GET_TRIGGER_ERROR_ACTION_TABLE,       // Injection Action
      APEI_READ_REG,                        // Instruction
      APEI_CLEAR_FLAG,                      // Flags
      APEI_RESERVED,                        // Reserved
      { // Generic Address Region
        APEI_ENTRY_ADDRID_SYSMEM,           // AddressSpaceID = System Memory
        REG_BIT_WIDTH_QWORD,                // Register Bit Width
        REG_BIT_OFFSET0,                    // Register Bit Offset
        APEI_ACCESS_QWORD,                  // Access Size
        0                                   // Address
      },
      0,                                    // Value
      APEI_QWORD_MASK                       // Mask
    },

    //EinjSetErrorType
    {
      SET_ERROR_TYPE,                       // Injection Action
      APEI_WRITE_REG,                       // Instruction
      APEI_CLEAR_FLAG,                      // Flags
      APEI_RESERVED,                        // Reserved
      { // Generic Address Region
        APEI_ENTRY_ADDRID_SYSMEM,           // AddressSpaceID = System Memory
        REG_BIT_WIDTH_DWORD,                // Register Bit Width
        REG_BIT_OFFSET0,                    // Register Bit Offset
        APEI_ACCESS_DWORD,                  // Access Size
        0                                   // Address
      },
      0,                                    // Value
      APEI_DWORD_MASK                       // Mask
    },

    //EinjGetErrorType
    {
      GET_ERROR_TYPE,                       // Injection Action
      APEI_READ_REG,                        // Instruction
      APEI_CLEAR_FLAG,                      // Flags
      APEI_RESERVED,                        // Reserved
      { // Generic Address Region
        APEI_ENTRY_ADDRID_SYSMEM,           // AddressSpaceID = System Memory
        REG_BIT_WIDTH_DWORD,                // Register Bit Width
        REG_BIT_OFFSET0,                    // Register Bit Offset
        APEI_ACCESS_DWORD,                  // Access Size
        0                                   // Address
      },
      0,                                    // Value
      APEI_DWORD_MASK                       // Mask
    },

    //EinjEndOperation
    {
      END_OPERATION,                       // Injection Action
      APEI_WRITE_REG_VALUE,                // Instruction
      APEI_CLEAR_FLAG,                     // Flags
      APEI_RESERVED,                       // Reserved
      { // Generic Address Region
        APEI_ENTRY_ADDRID_SYSMEM,          // AddressSpaceID = System Memory
        REG_BIT_WIDTH_WORD,                // Register Bit Width
        REG_BIT_OFFSET0,                   // Register Bit Offset
        APEI_ACCESS_WORD,                  // Access Size
        0                                  // Address
      },
      0,                                   // Value
      APEI_WORD_MASK                       // Mask
    },

    //EinjExecOperation
    {
      EXECUTE_OPERATION,                    // Injection Action
      APEI_WRITE_REG_VALUE,                 // Instruction
      APEI_CLEAR_FLAG,                      // Flags
      APEI_RESERVED,                        // Reserved
      { // Generic Address Region
        APEI_ENTRY_ADDRID_SYSIO,            // AddressSpaceID = System Memory
        REG_BIT_WIDTH_BYTE,                 // Register Bit Width
        REG_BIT_OFFSET0,                    // Register Bit Offset
        APEI_ACCESS_BYTE,                   // Access Size
        SMIPORT                             // Address
      },
      EINJ_BEGIN_INJ_CMD,                   // Value
      APEI_BYTE_MASK                        // Mask
    },

    //EinjCheckBusyStatus
    {
      CHECK_BUSY_STATUS,                    // Injection Action
      APEI_READ_REG,                        // Instruction
      APEI_CLEAR_FLAG,                      // Flags
      APEI_RESERVED,                        // Reserved
      { // Generic Address Region
        APEI_ENTRY_ADDRID_SYSMEM,           // AddressSpaceID = System Memory
        REG_BIT_WIDTH_BYTE,                 // Register Bit Width
        REG_BIT_OFFSET0,                    // Register Bit Offset
        APEI_ACCESS_BYTE,                   // Access Size
        0                                   // Address
      },
      0x01,                                 // Value
      APEI_CHK_BUSY_STS_MASK                // Mask
    },

    //EinjGetCommandStatus
    {
      GET_COMMAND_STATUS,                   // UINT8 SerializationAction - Injection Action
      APEI_READ_REG,                        // UINT8 Instruction
      APEI_CLEAR_FLAG,                      // UINT8 Flags
      APEI_RESERVED,                        // UINT8 Reserved
      { // Generic Address Region
        APEI_ENTRY_ADDRID_SYSMEM,           // AddressSpaceID = System Memory
        REG_BIT_WIDTH_WORD,                 // Register Bit Width
        REG_BIT_OFFSET0,                    // Register Bit Offset
        APEI_ACCESS_WORD,                   // Access Size
        0                                   // UINT64 Address
      },
      0,                                    // UINT64 Value
      APEI_GET_CMD_STS_MASK                 // UINT64 Mask
    }
  }
};

// Trigger Error Action Table
TRIG_ERROR_ACTION mTrigErrActTable = \
{
  {
    sizeof (TRIG_ERROR_ACTION_HEADER),      // UINT32 HeaderSize
    0,                                      // UINT32 Revsion
    sizeof (TRIG_ERROR_ACTION),             // UINT32 TableSize
    1                                       // UINT32 Instruction EntryCount
  },
  {
    { //APEI_SERIALIZATION_INST_ENTRY[0]
      EXECUTE_OPERATION,                    // UINT8 SerializationAction - Injection Action
      EINJ_NOOP,                            // UINT8 Instruction - No Operation
      APEI_CLEAR_FLAG,                      // UINT8 Flags
      APEI_RESERVED,                        // UINT8 Reserved
      { // Generic Address Region
        APEI_ENTRY_ADDRID_SYSMEM,           // UINT8 AddressSpaceID = System Memory
        REG_BIT_WIDTH_DWORD,                // UINT8 Register Bit Width IGNORED due to NOOP Instruction
        REG_BIT_OFFSET0,                    // UINT8 Register Bit Offset
        APEI_ACCESS_DWORD,                  // UINT8 Access Size
        0                                   // UINT64 Address
      },
      0,                                    // UINT64 Value
      APEI_DWORD_MASK                       // UINT64 Mask
    }
  }
};

/*
// Trigger Error Action Table
CORR_MEM_TRIG_ERROR_ACTION  CorrMemTrigErrActTable = \
{
  {
    sizeof (TRIG_ERROR_ACTION_HEADER),      // UINT32 HeaderSize
    0,                                      // UINT32 Revsion
    sizeof (CORR_MEM_TRIG_ERROR_ACTION),    // UINT32 TableSize
    1                                       // UINT32 Instruction EntryCount
  },
  {
    { //APEI_SERIALIZATION_INST_ENTRY[0]
      EXECUTE_OPERATION,                    // UINT8 SerializationAction - Injection Action
      EINJ_WRITE_REGISTER_VALUE,            // UINT8 Instruction
      APEI_CLEAR_FLAG,                      // UINT8 Flags
      APEI_RESERVED,                        // UINT8 Reserved
      { // Generic Address Region
        APEI_ENTRY_ADDRID_SYSMEM,           // UINT8 AddressSpaceID = System Memory
        REG_BIT_WIDTH_DWORD,                // UINT8 Register Bit Width
        REG_BIT_OFFSET0,                    // UINT8 Register Bit Offset
        APEI_ACCESS_DWORD,                  // UINT8 Access Size
        0                                   // UINT64 Address
      },
      0x80000002,                           // UINT64 Value
      APEI_DWORD_MASK                       // UINT64 Mask
    },
    { //APEI_SERIALIZATION_INST_ENTRY[1]
      EXECUTE_OPERATION,                    // UINT8 SerializationAction - Injection Action
      EINJ_WRITE_REGISTER_VALUE,            // UINT8 Instruction
      APEI_CLEAR_FLAG,                      // UINT8 Flags
      APEI_RESERVED,                        // UINT8 Reserved
      { // Generic Address Region
        APEI_ENTRY_ADDRID_SYSMEM,           // UINT8 AddressSpaceID = System Memory
        REG_BIT_WIDTH_DWORD,                // UINT8 Register Bit Width
        REG_BIT_OFFSET0,                    // UINT8 Register Bit Offset
        APEI_ACCESS_DWORD,                  // UINT8 Access Size
        0                                   // UINT64 Address
      },
      0x00160001,                           // UINT64 Value
      APEI_DWORD_MASK                       // UINT64 Mask
    }
  }
};
*/

/**
 *---------------------------------------------------------------------------------------
 *
 *  ApeiEinjInit
 *
 *  Description:
 *     Initialize ACPI APEI EINJ table.
 *
 *  Parameters:
 *    @retval        EFI_SUCCESS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
ApeiEinjInit ( VOID )
{
  EFI_STATUS Status = EFI_SUCCESS;
  APEI_EINJ_ACPI_TABLE *EinjTable;

  //
  //Reserve EFI memory buffer for Einj serial instruction
  //
  Status = gBS->AllocatePool (EfiReservedMemoryType, sizeof (APEI_EINJ_REGISTERS), &mEinjActMemRegAddr);
  if (EFI_ERROR (Status)) {
    return EFI_OUT_OF_RESOURCES;
  }

  mEinjTable.InjectionInstEntry[0].RegisterRegion.Address  = (UINT64) ((UINTN) &mEinjActMemRegAddr->EinjBeginInjReg);
  mEinjTable.InjectionInstEntry[1].RegisterRegion.Address  = (UINT64) ((UINTN) &mEinjActMemRegAddr->EinjGetTriActTblReg);
  mEinjTable.InjectionInstEntry[2].RegisterRegion.Address  = (UINT64) ((UINTN) &mEinjActMemRegAddr->EinjSetErrTypeReg);
  mEinjTable.InjectionInstEntry[3].RegisterRegion.Address  = (UINT64) ((UINTN) &mEinjActMemRegAddr->EinjGetErrTypeReg);
  mEinjTable.InjectionInstEntry[4].RegisterRegion.Address  = (UINT64) ((UINTN) &mEinjActMemRegAddr->EinjEndOperReg);
  mEinjTable.InjectionInstEntry[6].RegisterRegion.Address  = (UINT64) ((UINTN) &mEinjActMemRegAddr->EinjChkBusyStsReg);
  mEinjTable.InjectionInstEntry[7].RegisterRegion.Address  = (UINT64) ((UINTN) &mEinjActMemRegAddr->EinjCmdStsReg);

  // Set error injection capabilities bitmask for this platform
  mEinjActMemRegAddr->EinjGetErrTypeReg = MEM_CORRECTABLE | MEM_UNCORRECTABLE_FATAL;

  //Allocate EFI Boot Services memory space for EINJ table
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (APEI_EINJ_ACPI_TABLE), &EinjTable);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Copy local EINJ table to EFI Boot Services memory buffer
  gBS->CopyMem (EinjTable, &mEinjTable, sizeof (APEI_EINJ_ACPI_TABLE));

  // Assign APEI Private Data EINJ table pointer
  mApeiPrivData->ApeiEinjTbl = EinjTable;

  // Allocate ACPI NVS memory for Trigger Error Action Table
  Status = gBS->AllocatePool (EfiACPIMemoryNVS, sizeof (TRIG_ERROR_ACTION), &mTrigErrorAct);
  if (EFI_ERROR (Status)) {
    return EFI_OUT_OF_RESOURCES;
  }

  // Copy local Trigger Error Action table to ACPI NVS memory space
  //  gBS->CopyMem (mCorrMemTrigErrorAct ,&CorrMemTrigErrActTable, sizeof(CORR_MEM_TRIG_ERROR_ACTION));
  gBS->CopyMem (mTrigErrorAct, &mTrigErrActTable, sizeof (TRIG_ERROR_ACTION));

  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  ApeiEinjSwSmiHandler
 *
 *  Description:
 *     Software SMM handler for EINJ error injection.
 *
 *  Parameters:
 *    @param[in]     DispatchHandle
 *    @param[in]     *DispatchContext
 *
 *---------------------------------------------------------------------------------------
 **/
VOID ApeiEinjSwSmiHandler (
  IN  EFI_HANDLE                  DispatchHandle,
  IN  EFI_SMM_SW_DISPATCH_CONTEXT *DispatchContext
  )
{
  UINT64  CurrAddr;
  UINT64  MsrVal;
  UINT32  ErrorType;
  UINT16  OperationStatus;
  UINTN   CurrNode;

  // Initialize Local Variables
  CurrAddr = 0;
  MsrVal = 0;
  ErrorType = 0;
  OperationStatus = EINJ_CMD_UNKNOWN_FAIL;
  CurrNode = 0;

  // Set Busy Status bit
  mEinjActMemRegAddr->EinjChkBusyStsReg = APEI_SET_BUSY_BIT;   // APEI_SET_BUSY_BIT = 0x01

  // Get Injection error type
  ErrorType = mEinjActMemRegAddr->EinjSetErrTypeReg;

  //
  // Execute error injection based upon requested Error Type
  //
  switch (ErrorType)
  {
  case MEM_CORRECTABLE:  // Correctable Memory Error
    MsrVal = EfiReadMsr (AMD_MSR_MMIO_CFG_BASE);
    mCfgMmioBase = (UINT32) MsrVal & 0xfff00000;
    CurrAddr = mCfgMmioBase | ((0x18 + CurrNode) << 15) | (3 << 12) | 0xB8; //F3:B8
    // Sit Bit[31] - DRAM Error - Error Address Implicit, Bit[1] - 16 Byte Quadrant 0 in 64 Byte Cache Line
    RasSmmWriteMem32 (CurrAddr, 0x80000002);
    CurrAddr = mCfgMmioBase | ((0x18 + CurrNode) << 15) | (3 << 12) | 0xBC; //F3:BC
    // Sit Bit[20] - ErrInjEn: enable error injection to word Data[15:0], Bit[17] - EccWrReq, Bit[0] - EccVector = 0x0001
    RasSmmWriteMem32 (CurrAddr, 0x00120001);
    OperationStatus = EINJ_CMD_SUCCEESS;
    mEinjActMemRegAddr->EinjGetTriActTblReg = (UINT64) ((UINTN) mTrigErrorAct);
    break;
  case MEM_UNCORRECTABLE_FATAL:  // Uncorrectable Memory Error
    MsrVal = EfiReadMsr (AMD_MSR_MMIO_CFG_BASE);
    mCfgMmioBase = (UINT32) MsrVal & 0xfff00000;
    CurrAddr = mCfgMmioBase | ((0x18 + CurrNode) << 15) | (3 << 12) | 0xB8; //F3:B8
    // Sit Bit[31] - DRAM Error - Error Address Implicit, Bit[1] - 16 Byte Quadrant 0 in 64 Byte Cache Line
    RasSmmWriteMem32 (CurrAddr, 0x80000002);
    CurrAddr = mCfgMmioBase | ((0x18 + CurrNode) << 15) | (3 << 12) | 0xBC; //F3:BC
    // Sit Bit[21:20] - ErrInjEn: enable error injection to word Data[15:0]&Data[31:16], Bit[18] - DramErrEn, Bit[1:0] - EccVector = 0x0003
    RasSmmWriteMem32 (CurrAddr, 0x00340003);
    OperationStatus = EINJ_CMD_SUCCEESS;
    mEinjActMemRegAddr->EinjGetTriActTblReg = (UINT64) ((UINTN) mTrigErrorAct);
    break;
  default:
    OperationStatus = EINJ_CMD_INVALID_ACCESS;
  }

  //Set action command status
  mEinjActMemRegAddr->EinjCmdStsReg = OperationStatus;

  // Clear busy status bit
  mEinjActMemRegAddr->EinjChkBusyStsReg = APEI_RESET_BUSY_BIT;
}