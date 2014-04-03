/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP Base Library
 *
 * Contains interface to the PSP library
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 282908 $   @e \$Date: 2014-01-20 00:54:02 -0600 (Mon, 20 Jan 2014) $
 *
 */
/*
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
 **/

#ifndef _PSP_BASE_LIB_H_
#define _PSP_BASE_LIB_H_
#include "Uefi.h"
#include "PspDirectory.h"

#define IOCF8 0xCF8
#define IOCFC 0xCFC

// Reg Values for ReadCpuReg and WriteCpuReg
#define    CR4_REG  0x04
#define    DR0_REG  0x10
#define    DR1_REG  0x11
#define    DR2_REG  0x12
#define    DR3_REG  0x13
#define    DR7_REG  0x17

#define NB_CFG      0xC001001Ful
#define MSR_MMIO_Cfg_Base           0xC0010058ul    // MMIO Configuration Base Address Register

#ifndef BIT0
  #define BIT0        0x0000000000000001ull
#endif
#ifndef BIT1
  #define BIT1        0x0000000000000002ull
#endif
#ifndef BIT2
  #define BIT2        0x0000000000000004ull
#endif
#ifndef BIT3
  #define BIT3        0x0000000000000008ull
#endif
#ifndef BIT4
  #define BIT4        0x0000000000000010ull
#endif
#ifndef BIT5
  #define BIT5        0x0000000000000020ull
#endif
#ifndef BIT6
  #define BIT6        0x0000000000000040ull
#endif
#ifndef BIT7
  #define BIT7        0x0000000000000080ull
#endif
#ifndef BIT8
  #define BIT8        0x0000000000000100ull
#endif
#ifndef BIT9
  #define BIT9        0x0000000000000200ull
#endif
#ifndef BIT10
  #define BIT10       0x0000000000000400ull
#endif
#ifndef BIT11
  #define BIT11       0x0000000000000800ull
#endif
#ifndef BIT12
  #define BIT12       0x0000000000001000ull
#endif
#ifndef BIT13
  #define BIT13       0x0000000000002000ull
#endif
#ifndef BIT14
  #define BIT14       0x0000000000004000ull
#endif
#ifndef BIT15
  #define BIT15       0x0000000000008000ull
#endif
#ifndef BIT16
  #define BIT16       0x0000000000010000ull
#endif
#ifndef BIT17
  #define BIT17       0x0000000000020000ull
#endif
#ifndef BIT18
  #define BIT18       0x0000000000040000ull
#endif
#ifndef BIT19
  #define BIT19       0x0000000000080000ull
#endif
#ifndef BIT20
  #define BIT20       0x0000000000100000ull
#endif
#ifndef BIT21
  #define BIT21       0x0000000000200000ull
#endif
#ifndef BIT22
  #define BIT22       0x0000000000400000ull
#endif
#ifndef BIT23
  #define BIT23       0x0000000000800000ull
#endif
#ifndef BIT24
  #define BIT24       0x0000000001000000ull
#endif
#ifndef BIT25
  #define BIT25       0x0000000002000000ull
#endif
#ifndef BIT26
  #define BIT26       0x0000000004000000ull
#endif
#ifndef BIT27
  #define BIT27       0x0000000008000000ull
#endif
#ifndef BIT28
  #define BIT28       0x0000000010000000ull
#endif
#ifndef BIT29
  #define BIT29       0x0000000020000000ull
#endif
#ifndef BIT30
  #define BIT30       0x0000000040000000ull
#endif
#ifndef BIT31
  #define BIT31       0x0000000080000000ull
#endif
#ifndef BIT32
  #define BIT32       0x0000000100000000ull
#endif
#ifndef BIT33
  #define BIT33       0x0000000200000000ull
#endif
#ifndef BIT34
  #define BIT34       0x0000000400000000ull
#endif
#ifndef BIT35
  #define BIT35       0x0000000800000000ull
#endif
#ifndef BIT36
  #define BIT36       0x0000001000000000ull
#endif
#ifndef BIT37
  #define BIT37       0x0000002000000000ull
#endif
#ifndef BIT38
  #define BIT38       0x0000004000000000ull
#endif
#ifndef BIT39
  #define BIT39       0x0000008000000000ull
#endif
#ifndef BIT40
  #define BIT40       0x0000010000000000ull
#endif
#ifndef BIT41
  #define BIT41       0x0000020000000000ull
#endif
#ifndef BIT42
  #define BIT42       0x0000040000000000ull
#endif
#ifndef BIT43
  #define BIT43       0x0000080000000000ull
#endif
#ifndef BIT44
  #define BIT44       0x0000100000000000ull
#endif
#ifndef BIT45
  #define BIT45       0x0000200000000000ull
#endif
#ifndef BIT46
  #define BIT46       0x0000400000000000ull
#endif
#ifndef BIT47
  #define BIT47       0x0000800000000000ull
#endif
#ifndef BIT48
  #define BIT48       0x0001000000000000ull
#endif
#ifndef BIT49
  #define BIT49       0x0002000000000000ull
#endif
#ifndef BIT50
  #define BIT50       0x0004000000000000ull
#endif
#ifndef BIT51
  #define BIT51       0x0008000000000000ull
#endif
#ifndef BIT52
  #define BIT52       0x0010000000000000ull
#endif
#ifndef BIT53
  #define BIT53       0x0020000000000000ull
#endif
#ifndef BIT54
  #define BIT54       0x0040000000000000ull
#endif
#ifndef BIT55
  #define BIT55       0x0080000000000000ull
#endif
#ifndef BIT56
  #define BIT56       0x0100000000000000ull
#endif
#ifndef BIT57
  #define BIT57       0x0200000000000000ull
#endif
#ifndef BIT58
  #define BIT58       0x0400000000000000ull
#endif
#ifndef BIT59
  #define BIT59       0x0800000000000000ull
#endif
#ifndef BIT60
  #define BIT60       0x1000000000000000ull
#endif
#ifndef BIT61
  #define BIT61       0x2000000000000000ull
#endif
#ifndef BIT62
  #define BIT62       0x4000000000000000ull
#endif
#ifndef BIT63
  #define BIT63       0x8000000000000000ull
#endif

#undef  GET_MASK8
#define GET_MASK8(HighBit, LowBit) ((((UINT8) 1 << (HighBit - LowBit + 1)) - 1) << LowBit)

#undef  GET_MASK16
#define GET_MASK16(HighBit, LowBit) ((((UINT16) 1 << (HighBit - LowBit + 1)) - 1) << LowBit)

#undef  GET_MASK32
#define GET_MASK32(HighBit, LowBit) ((((UINT32) 1 << (HighBit - LowBit + 1)) - 1) << LowBit)

#undef  GET_MASK64
#define GET_MASK64(HighBit, LowBit) ((((UINT64) 1 << (HighBit - LowBit + 1)) - 1) << LowBit)

#define PSP_PCI_SEG        0x00    ///< PSP Seg address
#define PSP_PCI_BUS        0x00    ///< PSP Bus address
#define PSP_PCI_DEV        0x08    ///< PSP Device address
#define PSP_PCI_FN         0x00    ///< PSP Fn address
#define PSP_PCIE_BDA        ((PSP_PCI_DEV << 11) + (PSP_PCI_FN << 8))
#define GET_PSP_PCI_ADDR (Offset)    MAKE_SBDFO (PSP_PCI_SEG, PSP_PCI_BUS, PSP_PCI_DEV, PSP_PCI_FN, Offset)

#define PSP_PCIE_DEVID_REG    0x00    ///< DevId
#define PSP_PCIE_CMD_REG      0x04    ///< CmdReg
#define PSP_PCIE_BAR1_REG     0x18    ///< Pcie Bar

/// These width descriptors are used by the library function, and others, to specify the data size
typedef enum ACCESS_WIDTH {
  AccessWidth8 = 1,                                         ///< Access width is 8 bits.
  AccessWidth16,                                            ///< Access width is 16 bits.
  AccessWidth32,                                            ///< Access width is 32 bits.
  AccessWidth64,                                            ///< Access width is 64 bits.

  AccessS3SaveWidth8 = 0x81,                                ///< Save 8 bits data.
  AccessS3SaveWidth16,                                      ///< Save 16 bits data.
  AccessS3SaveWidth32,                                      ///< Save 32 bits data.
  AccessS3SaveWidth64,                                      ///< Save 64 bits data.
} ACCESS_WIDTH;

/// Extended PCI address format
typedef struct {
  UINT32      Register:12;                ///< Register offset
  UINT32      Function:3;                 ///< Function number
  UINT32      Device:5;                   ///< Device number
  UINT32      Bus:8;                      ///< Bus number
  UINT32      Segment:4;                  ///< Segment
} EXT_PCI_ADDR;

/// Union type for PCI address
typedef union _PCI_ADDR {
  UINT32          AddressValue;               ///< Formal address
  EXT_PCI_ADDR    Address;                    ///< Extended address
} PCI_ADDR;

//   SBDFO - Segment Bus Device Function Offset
//   31:28   Segment (4-bits)
//   27:20   Bus     (8-bits)
//   19:15   Device  (5-bits)
//   14:12   Function(3-bits)
//   11:00   Offset  (12-bits)

#define MAKE_SBDFO(Seg, Bus, Dev, Fun, Off) ((((UINT32) (Seg)) << 28) | (((UINT32) (Bus)) << 20) | \
                   (((UINT32)(Dev)) << 15) | (((UINT32)(Fun)) << 12) | ((UINT32)(Off)))
#define ILLEGAL_SBDFO 0xFFFFFFFFul

#define GET_PCI_BUS(Reg) (((UINT32) Reg >> 16) & 0xFF)
#define GET_PCI_DEV(Reg) (((UINT32) Reg >> 11) & 0x1F)
#define GET_PCI_FUNC(Reg) (((UINT32) Reg >> 8) & 0x7)
#define GET_PCI_OFFSET(Reg) ((UINT32)Reg & 0xFF)

#define PCI_CONFIG_SMU_INDIRECT_INDEX   0xB8          ///<  Gnb Offset index for SMU mbox
#define PCI_CONFIG_SMU_INDIRECT_DATA    0xBC          ///<  Gnb Offset data for SMU mbox

#define SMU_CC_PSP_FUSES_STATUS         0xC00C002Cul  ///< offset in GNB to find PSP fusing
#define SMU_CC_PSP_FUSES_SECURE         BIT1          ///< BIT1
#define SMU_CC_PSP_FUSES_FRA_ENABLE     BIT2          ///< BIT2
#define SMU_CC_PSP_FUSES_PROTO          BIT3          ///< BIT3
#define SMU_CC_PSP_FUSES_PCPU_DIS       BIT5          ///< BIT5

#define PSP_BLANK_PART                  0   ///< Blank part
#define PSP_PROTO_PART                  SMU_CC_PSP_FUSES_PROTO    ///< Proto Part
#define PSP_NON_SECURE_PART             (SMU_CC_PSP_FUSES_PROTO + SMU_CC_PSP_FUSES_PCPU_DIS)    ///< Non Secure Part
#define PSP_SECURE_PART                 (SMU_CC_PSP_FUSES_PROTO + SMU_CC_PSP_FUSES_SECURE)      ///< Secure Part
#define PSP_FRA_MODE                    (SMU_CC_PSP_FUSES_FRA_ENABLE + SMU_CC_PSP_FUSES_PROTO + SMU_CC_PSP_FUSES_SECURE)  ///< FRA Part


UINT8
PspLibReadIo8 (
  IN       UINT16 Address
  );

UINT16
PspLibReadIo16 (
  IN       UINT16 Address
  );

UINT32
PspLibReadIo32 (
  IN       UINT16 Address
  );

VOID
PspLibWriteIo8 (
  IN       UINT16 Address,
  IN       UINT8 Data
  );

VOID
PspLibWriteIo16 (
  IN       UINT16 Address,
  IN       UINT16 Data
  );

VOID
PspLibWriteIo32 (
  IN       UINT16 Address,
  IN       UINT32 Data
  );

VOID
PspLibMemRead (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT64 MemAddress,
     OUT   VOID *Value
  );

VOID
PspLibMemWrite (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT64 MemAddress,
  IN       VOID *Value
  );

UINT64
PspLibReadTSC (
  );

// MSR
VOID
PspLibMsrRead (
  IN       UINT32 MsrAddress,
     OUT   UINT64 *Value
  );

VOID
PspLibMsrWrite (
  IN       UINT32 MsrAddress,
  IN       UINT64 *Value
  );

// IO
VOID
PspLibIoRead (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT16 IoAddress,
     OUT   VOID *Value
  );

VOID
PspLibIoWrite (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT16 IoAddress,
  IN       VOID *Value
  );

VOID
PspLibIoRMW (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT16 IoAddress,
  IN       VOID *Data,
  IN       VOID *DataMask
  );

// PCI
VOID
PspLibPciRead (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR PciAddress,
     OUT   VOID *Value
  );

VOID
PspLibPciWrite (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR PciAddress,
  IN       VOID *Value
  );

VOID
PspLibPciRMW (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR PciAddress,
  IN       VOID *Data,
  IN       VOID *DataMask
  );

VOID
PspLibPciReadBits (
  IN       PCI_ADDR Address,
  IN       UINT8 Highbit,
  IN       UINT8 Lowbit,
     OUT   UINT32 *Value
  );

VOID
PspLibPciWriteBits (
  IN       PCI_ADDR Address,
  IN       UINT8 Highbit,
  IN       UINT8 Lowbit,
  IN       UINT32 *Value
  );

UINT32
PspLibPciReadConfig (
  IN  UINT32 Register
  );

VOID
PspLibPciWriteConfig (
  IN  UINT32 Register,
  IN  UINT32 Value
  );

UINT32
PspLibPciReadPspConfig (
  IN UINT32 Offset
  );

VOID
PspLibPciWritePspConfig (
  IN  UINT32 Offset,
  IN  UINT32 Value
  );

VOID
PspLibReadCpuReg (
  IN       UINT8 RegNum,
     OUT   UINT32 *Value
  );
VOID
PspLibWriteCpuReg (
  IN       UINT8 RegNum,
  IN       UINT32 Value
  );

VOID
PspLibSimNowEnterDebugger ();

EFI_STATUS
GetPspDirBase (
  IN OUT   EFI_PHYSICAL_ADDRESS     *Address
);

EFI_STATUS
PSPEntryInfo (
  IN      PSP_DIRECTORY_ENTRY_TYPE    EntryType,
  IN OUT  EFI_PHYSICAL_ADDRESS        *EntryAddress,
  IN      UINTN                       *EntrySize
  );

EFI_STATUS
CheckPspDevicePresent (
  VOID
  );

EFI_STATUS
GetPspBar1Addr (
  IN OUT EFI_PHYSICAL_ADDRESS *PspMmio
  );

EFI_STATUS
EFIAPI
PspBarInitEarly ();

#endif // _AMD_LIB_H_
