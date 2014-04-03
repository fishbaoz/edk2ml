/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD APEI Error Injection for RAS
 *
 * Contains structures for APEI (WHEA) EINJ related ACPI definitions and support structures
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
 * Description: AmdApeiEinj.h - Contains structures for APEI (WHEA) EINJ related
 *                              ACPI definitions and support structures.
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

#ifndef _AMD_APEI_EINJ_H_
#define _AMD_APEI_EINJ_H_

#ifdef __cplusplus
  extern "C" {
  #endif

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
  #include "AmdApei.h"
  #include "AmdRasSmmCpuIoLib.h"
  #include "SmmSwDispatch.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define EINJ_SIG 0x4A4E4945ul            // "EINJ" backwards
#define AMD_EINJ 0x4A4E494520444D41ull   // "AMD EINJ" backwards

#define REVISION_EINJ                   0x201

//
//  Injection Actions Definition
//
#define BEGIN_INJECT_OPERATION          0x0
#define GET_TRIGGER_ERROR_ACTION_TABLE  0x01
#define SET_ERROR_TYPE                  0x02
#define GET_ERROR_TYPE                  0x03
#define END_OPERATION                   0x04
#define EXECUTE_OPERATION               0x05
#define CHECK_BUSY_STATUS               0x06
#define GET_COMMAND_STATUS              0x07
#define TRIGGER_ERROR                   0xFF

//
// EINJ Command Status Definition
//
#define EINJ_CMD_SUCCEESS               0x0
#define EINJ_CMD_UNKNOWN_FAIL           0x01
#define EINJ_CMD_INVALID_ACCESS         0x02

//
// Injection Instructions
//
#define EINJ_READ_REGISTER              0x0   // Returns a register value
#define EINJ_READ_REGISTER_VALUE        0x01  // Reads register value and compares results to value in VALUE field
#define EINJ_WRITE_REGISTER             0x02  // Write register (ignores value in register field)
#define EINJ_WRITE_REGISTER_VALUE       0x03  // Write register using data in value field
#define EINJ_NOOP                       0x04  // No operation


//
//Busy bits
//
#define APEI_RESET_BUSY_BIT           0X0
#define APEI_SET_BUSY_BIT             0x01


//
//EINJ Injection Action Reg of memory location definition
//
#define EINJ_BEGIN_INJ_OFFSET         0
#define EINJ_GET_TRI_ACT_TBL          2
#define EINJ_SET_ERR_TYPE_OFFSET      10
#define EINJ_CHK_BUSY_STATUS_OFFSET   14
#define EINJ_GET_CMD_STATUS_OFFSET    14
#define EINJ_GET_ERR_TYPE_OFFSET      16 + 0
#define EINJ_END_OPER_OFFSET          16 + 4


//
// Error Type Definition (Bits 12:31 are RESERVED)
//
#define PROC_CORRECTABLE              1 << 0
#define PROC_UNCORRECT_NON_FATAL      1 << 1
#define PROC_UNCORRECT_FATAL          1 << 2
#define MEM_CORRECTABLE               1 << 3
#define MEM_UNCORRECTABLE_NON_FATAL   1 << 4
#define MEM_UNCORRECTABLE_FATAL       1 << 5
#define PCIE_CORRECTABLE              1 << 6
#define PCIE_UNCORRECTABLE_NON_FATAL  1 << 7
#define PCIE_UNCORRECTABLE_FATAL      1 << 8
#define PLATFORM_CORRECTABLE          1 << 9
#define PLATFORM_UNCORRECT_NON_FATAL  1 << 10
#define PLATFORM_UNCORRECT_FATAL      1 << 11

  #ifndef AMD_MSR_MMIO_CFG_BASE
    #define AMD_MSR_MMIO_CFG_BASE   0xc0010058ul
  #endif


  #pragma pack(1)


///Trigger Error Action Header
  typedef struct _TRIG_ERROR_ACTION_HEADER {
    UINT32  HeaderSize;           ///< Trigger Error Action Header Size
    UINT32  Revsion;              ///< Header Structure Revision
    UINT32  TableSize;            ///< Trigger Error Action Table Size
    UINT32  EntryCount;           ///< Trigger Error Action Entry count
  } TRIG_ERROR_ACTION_HEADER;

  /// Correctable Memory Trigger Action Table
  typedef struct _TRIG_ERROR_ACTION {
    TRIG_ERROR_ACTION_HEADER  TrigErrActionHeader;          ///< Trigger Error Action Header
    APEI_SERIALIZATION_INST_ENTRY TrigErrorInstruction[1];  ///< Trigger Error Instruction Array
  } TRIG_ERROR_ACTION;

  /// APEI EINJ Registers
  typedef struct _APEI_EINJ_REGISTERS {
    UINT16  EinjBeginInjReg;              ///< EINJ Begin Injection Register
    UINT64  EinjGetTriActTblReg;          ///< EINJ Get Trigger Action Table Register
    UINT32  EinjSetErrTypeReg;            ///< EINJ Set Error Type
    UINT16  EinjChkBusyStsReg;            ///< EINJ Check Busy Status Register
    UINT16  EinjCmdStsReg;                ///< EINJ Command Status Register
    UINT32  EinjGetErrTypeReg;            ///< EINJ Get Error Type Register
    UINT16  EinjEndOperReg;               ///< EINJ End Operation Register
  } APEI_EINJ_REGISTERS;

  #pragma pack()

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
  EFI_STATUS
  ApeiEinjInit ( VOID );

  VOID
  ApeiEinjSwSmiHandler (
    IN  EFI_HANDLE                  DispatchHandle,
    IN  EFI_SMM_SW_DISPATCH_CONTEXT *DispatchContext
    );

/****** DO NOT WRITE BELOW THIS LINE *******/
  #ifdef __cplusplus
  }
#endif
#endif