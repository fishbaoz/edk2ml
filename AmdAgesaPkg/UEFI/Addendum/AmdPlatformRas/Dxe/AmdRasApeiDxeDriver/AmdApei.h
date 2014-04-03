/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD APEI for RAS
 *
 * Contains structures for APEI (WHEA) related ACPI definitions and support structures.
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
 * Description: AmdApei.h - Contains structures for APEI (WHEA) related ACPI
 *                          definitions and support structures.
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

#ifndef _AMD_APEI_H_
#define _AMD_APEI_H_

#ifdef __cplusplus
  extern "C" {
  #endif

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
  #include "AcpiCommon.h"


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define ACPI_REV1                  0x01


/// APEI Instruction definition
#define APEI_READ_REG             0x0
#define APEI_READ_REG_VALUE       0x1
#define APEI_WRITE_REG            0x2
#define APEI_WRITE_REG_VALUE      0x3
#define APEI_NOOP                 0x4

#define APEI_ACCESS_BYTE          0x1
#define APEI_ACCESS_WORD          0x2
#define APEI_ACCESS_DWORD         0x3
#define APEI_ACCESS_QWORD         0x4

#define APEI_ENTRY_ADDRID_SYSMEM  0x0
#define APEI_ENTRY_ADDRID_SYSIO   0x1
#define APEI_ENTRY_ADDRID_RES     0x0


/// Register bit width
#define REG_BIT_WIDTH_BYTE        8
#define REG_BIT_WIDTH_WORD        16
#define REG_BIT_WIDTH_DWORD       32
#define REG_BIT_WIDTH_QWORD       64


/// Register bit offset
#define REG_BIT_OFFSET0           0

/// FLAGS
#define APEI_CLEAR_FLAG           0
#define APEI_SET_FLAG             1

/// Reserved
#define APEI_RESERVED             0

/// Mask
#define APEI_BYTE_MASK            0xFF
#define APEI_WORD_MASK            0xFFFF
#define APEI_DWORD_MASK           0xFFFFFFFFul
#define APEI_QWORD_MASK           0xFFFFFFFFFFFFFFFFull
#define APEI_CHK_BUSY_STS_MASK    0x01
#define APEI_GET_CMD_STS_MASK     0xFFFE

/// WHEA Platform Design Guide Table 4-13 Generic Error Status Block
#define ERROR_STS_BLK_UNCORR_VALID  (1 << 0) // Uncorrected Error Valid
#define ERROR_STS_BLK_CORR_VALID    (1 << 1) // Corrected Error Valid
#define ERROR_STS_BLK_UNCORR_MULTI  (1 << 2) // Multiple Uncorrected Errors
#define ERROR_STS_BLK_CORR_MULTI    (1 << 3) // Multiple Corrected Errors

/// WHEA Platform Design Guide Table 4-13 Generic Error Severity
#define ERROR_RECOVERABLE           0
#define ERROR_SEVERITY_FATAL        1
#define ERROR_SEVERITY_CORRECTED    2
#define ERROR_NONE                  3

#define GEN_HW_ERR_TYPE             0x09
#define SOURCE_ID0                  0
#define SOURCE_ID1                  1
#define RELATED_SOURCE_ID_NO_ALT    0xFFFF
#define RECORD_SECTION_ENABLE       0x03
#define ERR_SOURCE_ENABLED          0x01
#define RECORDS_PREALLOCATE1        0x01
#define SECTIONS_PER_RECORD1        0x01
#define NOTIFY_TYPE_SCI             0x03
#define NOTIFY_TYPE_NMI             0x04
#define NOTIFY_TYPE_POLLED          0
#define INT_VECTOR_NMI              2
#define INT_VECTOR_SCI              0

/// UEFI 2.3.1 Section N Table 245 *SJK1
/// Bit[15]-Rank Num Valid, Bit[14]-Memory Error Type Valid, Bit[9]-Column Valid, Bit[8]-Row Valid, Bit[6]-Bank Valid, Bit[3]-Node Valid, Bit[2]-Error Address Valid, Bit[0]-ErrSts Valid
#define MEM_VALID_BIT_MAP ((1 << 15) | (1 << 14) | (1 << 9) | (1 << 8) | (1 << 6) | (1 << 3) | (1 << 2) | (1 << 0))

/// UEFI  2.3.1. Section N Table 234 Error Section Descriptor GUID Definitions
/// Processor Generic Error Section GUID
#define PROCESSOR_GENERIC_SECT_GUID \
  { 0x9876CCAD, 0x47B4, 0x4BDB, 0xB6, 0x5E, 0x16, 0xF1, 0x93, 0xC4, 0xF3, 0xDB }
  // 9876CCAD-47B4-4bdb-B65E16F193C4F3DB

//Processor Specific (IA32/X64) Error Section GUID
#define PROCESSOR_SPECIFIC_X86_SECT_GUID \
  { 0xDC3EA0B0, 0xA144, 0x4797, 0xB9, 0x5B, 0x53, 0xFA, 0x24, 0x2B, 0x6E, 0x1D }
  // DC3EA0B0-A144-4797-B95B53FA242B6E1D

/// Platform Memory Error Section GUID
#define PLATFORM_MEMORY_SECT_GUID \
  { 0xA5BC1114, 0x6F64, 0x4EDE, 0xB8, 0x63, 0x3E, 0x83, 0xED, 0x7C, 0x83, 0xB1 }
  // A5BC1114-6F64-4EDE-B8633E83ED7C83B1

/// PCI Express Error Section GUID
#define PCIE_SECT_GUID \
  { 0xD995E954, 0xBBC1, 0x430F, 0xAD, 0x91, 0xB4, 0x4D, 0xCB, 0x3C, 0x6F, 0x35 }
  //D995E954-BBC1-430F-AD91B44DCB3C6F35

/// Firmware Error Record Section GUID
#define FIRMWARE_ERROR_RECORD_SECT_GUID \
  { 0x81212A96, 0x09ED, 0x4996, 0x94, 0x71, 0x8D, 0x72, 0x9C, 0x8E, 0x69, 0xED }
  //81212A96-09ED-4996-94718D729C869ED

/// PCI/PCIX Error Section GUID
#define PCI_PCIX_SECT_GUID \
  { 0xC5753963, 0x3B84, 0x4095, 0xBF, 0x78, 0xED, 0xDA, 0xD3, 0xF9, 0xC9, 0xDD }
  //C5753963-3B84-4095-BF78EDDAD3F9C9DD

/// PCI Component Device GUID
#define PCI_COMPONENT_DEVICE_SECT_GUID \
  { 0xEB5E4685, 0xCA66, 0x4769, 0xB6, 0xA2, 0x26, 0x06, 0x8B, 0x00, 0x13, 0x26 }
  //EB5E4685-CA66-4769-B6A226068B001326

/// DMAr Generic Section GUID
#define DMAR_GENERIC_SECT_GUID \
  { 0x5B51FEF7, 0xC79D, 0x4434, 0x8F, 0x1B, 0xAA, 0x62, 0xDE, 0x3E, 0x2C, 0x64 }
  //5B51FEF7-C79D-4434-8F1BAA62DE3E2C64

/// IOMMU Specific DMAr Section GUID
#define IOMMU_SPECIFIC_DMAR_SECT_GUID \
  { 0x036F84E1, 0x7F37, 0x428c, 0xA7, 0x9E, 0x57, 0x5F, 0xDF, 0xAA, 0x84, 0xEC }
  //036F84E1-7F37-428C-A79E575FDFAA84EC


/// Memory Error Types UEFI 2.3.1 Table 245
#define MEM_TYPE_UNKNOWN                    0
#define MEM_TYPE_NO_ERROR                   1
#define MEM_TYPE_SINGLE_BIT_ECC             2
#define MEM_TYPE_MULTI_BIT_ECC              3
#define MEM_TYPE_SINGLE_SYM_CHIP_KILL_ECC   4
#define MEM_TYPE_MULTI_SYM_CHIP_KILL_ECC    5
#define MEM_TYPE_MASTER_ABORT               6
#define MEM_TYPE_TARGET_ABORT               7
#define MEM_TYPE_PARITY_ERROR               8
#define MEM_TYPE_WATCHDOG_TIMEOUT           9
#define MEM_TYPE_INVALID_ADDRESS            10
#define MEM_TYPE_MIRROR_BROKEN              11
#define MEM_TYPE_MEMORY_SPARING             12
#define MEM_TYPE_SCRUB_CORR_ERROR           13
#define MEM_TYPE_SCRUB_UNCORR_ERROR         14
#define MEM_TYPE_PHYSICAL_MEM_MAPOUT        15


/// Validation bits UEFI spec2.1
#define FRU_ID_VALID                        0x01
#define FRU_STRING_VALID                    0X02
#define FRU_STRING_ID_VALID                 0x03

/// Pack structures on 1-byte boundary, otherwise structures will be
/// normalized to the compiler's boundary (64-bit boundary in this case)
  #pragma pack(1)


/// UEFI 2.1 Appendix N Table 245 Spec platform memory error
  typedef struct _PLATFORM_MEM_ERR_SEC {
    INT64   ValidBits;                ///< Valid bits Bitmp
    UINT64  ErrStatus;                ///< Error Status
    UINT64  PhyAddr;                  ///< Physical memory address of detected error
    UINT64  PhyAddrMask;              ///< Physical Error Address mask
    UINT16  Node;                     ///< Node Number
    UINT16  Card;                     ///< Card Number
    UINT16  Module;                   ///< Module Number
    UINT16  Bank;                     ///< Bank Number
    UINT16  Device;                   ///< Device Number
    UINT16  Row;                      ///< Row Number
    UINT16  Column;                   ///< Column Number
    UINT16  BitPosition;              ///< Bit Position
    UINT64  RequestorID;              ///< Requestor ID
    UINT64  ResponderID;              ///< Responder ID
    UINT64  TargetID;                 ///< Target ID
    UINT8   MemErrType;               ///< Memory Error Type
  } PLATFORM_MEM_ERR_SEC;

/// UEFI 2.1 Appendix N PCIX Bus Error
  typedef struct _PCIX_BUS_ERR_SEC {
    UINT64 ValidBits;                 ///< Valid bits Bitmp
    UINT64  ErrSts;                   ///< Error Status
    UINT16  ErrType;                  ///< Error Type
    UINT16  BusId;                    ///< PCIx Bus ID
    UINT32  Reserved;                 ///< Reserved
    UINT64  BusAddr;                  ///< Bus Address
    UINT64  BusData;                  ///< Bus Data
    UINT64  BusCmd;                   ///< Bus Command
    UINT64  BusReqId;                 ///< Bus Requestor ID
    UINT64  BusComId;                 ///< Bus Command ID
    UINT64  TargetId;                 ///< Target ID
  } PCIX_BUS_ERR_SEC;

/// Generic Error Status Block - WHEA Platform Design Guide V2.2 Table 4-13
  typedef struct GENERIC_ERR_STS_BLK {
    UINT32  BlockStatus;              ///< Generic Error Block Status
    UINT32  RawDataOffset;            ///< Raw Data Offset
    UINT32  RawDataLen;               ///< Raw Data Length
    UINT32  DataLen;                  ///< Data Length
    UINT32  ErrSeverity;              ///< Error Severity
  } GENERIC_ERR_STS_BLK;


/// Error Data Type struct for Boot Error Region table
  typedef struct _ERR_DATA_TYPE_DEF {
    UINT32  ErrTypeGUIDDword0;        ///< Error Type GUID DWord
    UINT16  ErrTypeGUIDWord0;         ///< Error Type GUID Word0
    UINT16  ErrTypeGUIDWord1;         ///< Error Type GUID Word1
    UINT8   ErrTypeGUIDByte0;         ///< Error Type GUID Byte0
    UINT8   ErrTypeGUIDByte1;         ///< Error Type GUID Byte1
    UINT8   ErrTypeGUIDByte2;         ///< Error Type GUID Byte2
    UINT8   ErrTypeGUIDByte3;         ///< Error Type GUID Byte3
    UINT8   ErrTypeGUIDByte4;         ///< Error Type GUID Byte4
    UINT8   ErrTypeGUIDByte5;         ///< Error Type GUID Byte5
    UINT8   ErrTypeGUIDByte6;         ///< Error Type GUID Byte6
    UINT8   ErrTypeGUIDByte7;         ///< Error Type GUID Byte7
  } ERR_DATA_TYPE_DEF;


/// Generic Error Data Entry
  typedef struct _GEN_ERR_DATA_ENTRY {
    ERR_DATA_TYPE_DEF SectionType;    ///< Error Data Type Definition
    UINT32            ErrorSeverity;  ///< Generic Error Severity
    UINT16            Revison;        ///< Structure Revision
    UINT8             ValidBits;      ///< Valid bits Bitmap
    UINT8             Flags;          ///< Flags
    UINT32            ErrDataLen;     ///< Error Data Length
    UINT8             FRU_ID[16];     ///< Field Replaceable Unit ID string
    UINT8             FRU_Text[20];   ///< Field Replaceable Unit Text string
  } GEN_ERR_DATA_ENTRY;


/// ACPI 5.0 Table 5-26 Generic Address Space definition
  typedef struct _EFI_ACPI_5_0_GAS {
    UINT8   AddressSpaceId;           ///< Generic Address Space ID
    UINT8   RegisterBitWidth;         ///< Generic Address Space Register Bit Width
    UINT8   RegisterBitOffset;        ///< Generic Address Space Register Bit Offset
    UINT8   AccessSize;               ///< Generic Address Space Access Size
    UINT64  Address;                  ///< Generic Address Space Address
  } EFI_ACPI_5_0_GAS;


/// Hardware Error Notification - WHEA Platform Design Guide V2.2 Table 4-15
  typedef struct _HW_ERROR_NOTIFY {
    UINT8     NotifiyType;                ///< Error Notification Type
    UINT8     ErrNotifyLen;               ///< Error Notification Length
    UINT16    ConfigWrite;                ///< Config Write
    UINT32    PollInterval;               ///< Polling Interval
    UINT32    Vector;                     ///< Interrupt Vector
    UINT32    SwitchPollingThreshVal;     ///< Switch Polling Threshold Value
    UINT32    SwitchPollingThreshWindow;  ///< Switch Polling Threshold Window
    UINT32    ErrThresholdVal;            ///< Error Threshold Value
    UINT32    ErrThresholdWindow;         ///< Error Threshold Window
  } HW_ERROR_NOTIFY;

/// Generic Hardware Error Source WHEA Platform Design Guide V2.2 Table 4-12
  typedef struct _GENERIC_HW_ERR_SOURCE {
    UINT16            GenSourceType;       ///< Error type = 0x09 Generic Error
    UINT16            SourceID;            ///< Unique Source ID
    UINT16            RelatedSrcID;        ///< Related Source ID
    UINT8             Flags;               ///< Flags = Reserved
    UINT8             Enabled;             ///< Enabled = 1 / Disabled = 0
    UINT32            NumRecPreAlloc;      ///< Number of records to preallocate for this error source
    UINT32            MaxSection;          ///< Indicate to OS how many error records could be created due to the this error source
    UINT32            MaxRawDataLen;       ///< Syze of he error data by this error source
    EFI_ACPI_5_0_GAS  RegisterRegion;      ///< Generic Address Structure
    HW_ERROR_NOTIFY   NotificationStruct;  ///< Notification Structure
    UINT32            ErrStsBlockLen;      ///< Size of error status block
  } GENERIC_HW_ERR_SOURCE;


/// Error source structure
  typedef struct _ERROR_SOURCE_STRUCT {
    GENERIC_HW_ERR_SOURCE   UncorrGenErrSource;   ///< Uncorrectable Generic Error Source
    GENERIC_HW_ERR_SOURCE   CorrGenErrSource;     ///< Correctable Generic Error Source
  } ERROR_SOURCE_STRUCT;

/// HEST ACPI Table
  typedef struct _APEI_HEST_ACPI_TABLE {
    EFI_ACPI_DESCRIPTION_HEADER   Header;             ///< ACPI Table Header
    UINT32                        HestErrSourceCount; ///< HEST Error Source Count
    ERROR_SOURCE_STRUCT           ErrorSourceStruct;  ///< Error Source Structure
  } APEI_HEST_ACPI_TABLE;


/// APEI BERT Error Context Table
  typedef struct _APEI_BERT_CONTEXT {
    UINT32  BootErrRegLen;                        ///< Boot Error Region Length
    UINT64  BootErrRegPtr;                        ///< Boot Error Region Pointer
  } APEI_BERT_CONTEXT;

/// BERT ACPI Table
  typedef struct _APEI_BERT_ACPI_TABLE {
    EFI_ACPI_DESCRIPTION_HEADER   Header;         ///< ACPI Table Header
    APEI_BERT_CONTEXT             BertContext;    ///< APEI BERT Context
  } APEI_BERT_ACPI_TABLE;


/// EINJ Table Injection Header Struct
  typedef struct _APEI_SERIALIZATION_HEADER {
    UINT32  InjectionHeaderSize;                  ///< Error Injection Header Size
    UINT8   Reserved;                             ///< Reserved
    UINT8   Reserved2[3];                         ///< Reserved2
    UINT32  InjectionInstEntryCount;              ///< Error Injection Instance Entry Count
  } APEI_SERIALIZATION_HEADER;

//// APEI Serialization Instructure Entry
  typedef struct _APEI_SERIALIZATION_INST_ENTRY {
    UINT8     SerializationAction;                ///< Serialization Action
    UINT8     Instruction;                        ///< Instruction
    UINT8     Flags;                              ///< Flags
    UINT8     Reserved;                           ///< Reserved
    EFI_ACPI_5_0_GAS  RegisterRegion;             ///< ACPI 5.0 Generic Address Space
    UINT64    Value;                              ///< Serialization Entry Value
    UINT64    Mask;                               ///< Serialization Entry Mask
  } APEI_SERIALIZATION_INST_ENTRY;

#define APEI_EINJ_INSTR_ENTRIES   8

/// EINJ ACPI Table
  typedef struct _APEI_EINJ_ACPI_TABLE {
    EFI_ACPI_DESCRIPTION_HEADER     Header;                 ///< ACPI Table Header
    APEI_SERIALIZATION_HEADER       InjectionHeader;        ///< APEI Serialization Header
    APEI_SERIALIZATION_INST_ENTRY   InjectionInstEntry[8];  ///< APEI Serialization Instance Entry
  } APEI_EINJ_ACPI_TABLE;

/// APEI ERST ACPI Table
#define APEI_ERST_INSTR_ENTRIES   15

/// ERST ACPI Table
  typedef struct _APEI_ERST_ACPI_TABLE {
    EFI_ACPI_DESCRIPTION_HEADER     Header;                                           ///< ACPI Table Header
    APEI_SERIALIZATION_HEADER       SerializationHeader;                              ///< APEI Serialization Header
    APEI_SERIALIZATION_INST_ENTRY   SerializationInstEntry[APEI_ERST_INSTR_ENTRIES];  ///< APEI Serialization Instance Entry
  } APEI_ERST_ACPI_TABLE;


/// APEI Private Data
  typedef struct _APEI_DRIVER_PRIVATE_DATA {
    VOID                    *ErrorLogAddressRange;    ///< Pointer to Error Log Address Range
    VOID                    *PersistentStoreBuffer;   ///< Pointer to Peristent Store Buffer
    APEI_BERT_ACPI_TABLE    *ApeiBertTbl;             ///< Pointer to APEI BERT Table
    APEI_HEST_ACPI_TABLE    *ApeiHestTbl;             ///< Pointer to APEI HEST Table
    APEI_EINJ_ACPI_TABLE    *ApeiEinjTbl;             ///< Pointer to APEI EINJ Table
    APEI_ERST_ACPI_TABLE    *ApeiErstTbl;             ///< Pointer to APEI ERST Table
  } APEI_DRIVER_PRIVATE_DATA;

  #pragma pack ()

/****** DO NOT WRITE BELOW THIS LINE *******/
  #ifdef __cplusplus
  }
#endif
#endif

