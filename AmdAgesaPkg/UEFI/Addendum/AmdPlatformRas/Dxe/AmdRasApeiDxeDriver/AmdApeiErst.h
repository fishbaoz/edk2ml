
/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD APEI Erst for RAS
 *
 * Contains structures for APEI (WHEA) ERST related ACPI definitions and support structures
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
 * Description: AmdApeiErst.h - Contains table definitions and structures for APEI
 *                          (WHEA) related ACPI ERST Error Record Serializaton
 *                          Table.
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

#ifndef _AMD_APEI_ERST_H_
  #define _AMD_APEI_ERST_H_

#ifdef __cplusplus
  extern "C" {
  #endif

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
  #include "AmdApei.h"
  #include "SmmSwDispatch.h"


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define ERST_SIG 0x54535245ul           ///< "ERST" backwards
#define AMD_ERST 0x5453524520444D41ull  ///< "AMD ERST" backwards

//
// Error Record Serailization Action Definition
//
#define ERST_BEGIN_WRITE_OPERATION        0x0
#define ERST_BEGIN_READ_OPERATION         0x1
#define ERST_BEGIN_CLEAR_OPERATION        0x2
#define ERST_END_OPERATION                0x3
#define ERST_SET_RECORD_OFFSET            0x4
#define ERST_EXECUTE_OPERATION            0x5
#define ERST_CHECK_BUSY_STATUS            0x6
#define ERST_GET_OPERATION_STATUS         0x7
#define ERST_GET_RECORD_IDENTIFIER        0x8
#define ERST_SET_RECORD_IDENTIFIER        0x9
#define ERST_GET_RECORD_COUNT             0xA
#define ERST_BEGIN_DUMMY_WRITE_OPERATION  0xB
#define ERST_GET_NEXT_RECORD_ID           0xC
#define ERST_GET_ERROR_LOG_ADDRESS_RANGE  0xD
#define ERST_GET_ERROR_LOG_ADDRESS_LENGTH 0xE
#define ERST_GET_ERROR_LOG_ADDRESS_ATTR   0xF

//
// ERST Command Status Definition
//
#define ERST_SUCCESS                      0x0
#define ERST_NOT_ENOUGH_SPACE             0x1
#define ERST_HARDWARE_NOT_AVAILABLE       0x2
#define ERST_FAILED                       0x3
#define ERST_RECORD_STORE_EMPTY           0x4
#define ERST_RECORD_NOT_FOUND             0x5

//
//Serialization Instruction Definition
//
#define ERST_READ_REGISTER                0x0
#define ERST_READ_REGISTER_VALUE          0x1
#define ERST_WRITE_REGISTER               0x2
#define ERST_WRITE_REGISTER_VALUE         0x3
#define ERST_NOOP                         0x4

//
//ERST Serialization Action Register Region of memory location definition
//
#define ERST_END_OPER_OFFSET              1
#define ERST_SET_REC_OFFSET               2
#define ERST_CHK_BUSY_OFFSET              6
#define ERST_GET_OPER_STATUS_OFFSET       7
#define ERST_GET_REC_IDENT_OFFSET         8
#define ERST_SET_REC_IDENT_OFFSET         8
#define ERST_GET_REC_COUNT_OFFSET         16
#define ERST_GET_MEM_OFFSET               18
#define ERST_GET_MEM_SIZE_OFFSET          26
#define ERST_GET_MEM_ATTRIBUTES_OFFSET    30

#define ERST_PERSISTENT_STORE_GUID \
  { 0x193A3779, 0xA2E3, 0x4f08, 0xB4, 0xAB, 0x43, 0x99, 0x8E, 0x55, 0x02, 0xFF }

  enum {
    APEI_PREPEND_MAGIC_NUMBER = 0x44332211,   ///< APEI Prepend Magic Number
    APEI_APPEND_MAGIC_NUMBER = 0x11223344     ///< APEI Append Magic Number
  };

  #pragma pack(1)


///< Defined in UEFI spec
  typedef struct _UEFI_ERROR_RECORD_HEADER {
    UINT32    SignatureStart;         ///<  Record signature etart
    UINT16    Revision;               ///<  Header revision
    UINT32    SignatureEnd;           ///<  Record signature end
    UINT16    SectionCount;           ///<  Section count
    UINT32    ErrSeverity;            ///<  Error severity
    UINT32    ValidBits;              ///<  Valid bitmap
    UINT32    RecLength;              ///<  Record length
    UINT64    Timestamp;              ///<  Timestamp
    EFI_GUID  PlatformID;             ///<  Platform ID GUID
    EFI_GUID  PartitionID;            ///<  Partition ID GUID
    EFI_GUID  CreatorID;              ///<  Creator ID GUID
    EFI_GUID  NotifiType;             ///<  Notification type GUID
    UINT64    RecordID;               ///<  Record ID
    UINT32    Flags;                  ///<  Flags
    UINT64    OSReserved;             ///<  OS Reserved
    UINT8     Reserved[12];           ///<  Reserved
  } UEFI_ERROR_RECORD_HEADER;


/// Header of Error Record in GPNV area
  typedef struct _NV_PREPEND_DATA {
    UINT32      MagicNum;             ///< Magic Number
    UINT8       RecordValidity;       ///< if status = ERASE_VALUE, then valid record
  } NV_PREPEND_DATA;


/// Footer of Error Record in GPNV area
  typedef struct _NV_APPEND_DATA {
    UINT32      MagicNum;             ///< Magic Number
  } NV_APPEND_DATA;

/// Error Record Serialization Info
  typedef struct _ERROR_RECORD_SERIALIZATION_INFO {
    UINT16      Signature;            ///< Signature
    UINT32      Reserved1;            ///< Reserved1
    UINT8       Reserved2;            ///< Reserved2
    UINT8       Attributes;           ///< Attributes
  } ERROR_RECORD_SERIALIZATION_INFO;

/// Persistent Error Record
  typedef struct _PERSISTENT_ERROR_RECORD {
    NV_PREPEND_DATA NvPrependData;              ///< Persistent Prepend Data
    UEFI_ERROR_RECORD_HEADER UefiErrorRecord;   ///< UEFI Error Record
  } PERSISTENT_ERROR_RECORD;

#define PS_PREPEND_DATA_SIZE sizeof (NV_PREPEND_DATA)
#define PS_APPEND_DATA_SIZE sizeof (NV_APPEND_DATA)

  #pragma pack()

/// These are the registers that will be read/written by windows
  typedef struct _APEI_ERST_REGISTERS {
    UINT8         CmdReg;                 ///< Command Registers
    UINT8         EndOperation;           ///< End Operation Register
    UINT32        RecordOffset;           ///< Record Offset Register
    UINT8         BusyStatus;             ///< Busy Status Register
    UINT8         OperationStatus;        ///< Operation Status Register
    UINT64        ValidRecordIdentifier;  ///< Firmware stores a record identifier which is available in persistent store
    UINT64        OSRecordIdentifier;     ///< OS tells to firmware which record identifier to be read
    UINT16        RecordCount;            ///< Record Count Register
    UINT64        ElarAddress;            ///< Elar Address Register
    UINT32        ElarLength;             ///< Elar Length Register
    UINT8         ElarAttributes;         ///< Elar Attributes
  } APEI_ERST_REGISTERS;


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
  EFI_STATUS
  ApeiErstInit (
    IN       VOID
    );

  VOID
  ApeiErstSwSmiHandler (
    IN       EFI_HANDLE                  DispatchHandle,
    IN       EFI_SMM_SW_DISPATCH_CONTEXT *DispatchContext
    );

/****** DO NOT WRITE BELOW THIS LINE *******/
  #ifdef __cplusplus
  }
#endif
#endif