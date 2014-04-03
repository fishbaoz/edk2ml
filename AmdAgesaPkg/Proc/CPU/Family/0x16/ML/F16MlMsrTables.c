/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_16 ML MSR tables with values as defined in BKDG
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/FAMILY/0x16/ML
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
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
 *
 ******************************************************************************
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "amdlib.h"
#include "Ids.h"
#include "cpuRegisters.h"
#include "F16MlPowerMgmt.h"
#include "Table.h"
#include "Filecode.h"
CODE_GROUP  (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X16_ML_F16MLMSRTABLES_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
F16MlNbMcaLock (
  IN       UINT32            Data,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
F16MlFPThrottle (
  IN       UINT32            Data,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );
/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

//   M S R   T a b l e s
// ----------------------

STATIC CONST MSR_TYPE_ENTRY_INITIALIZER ROMDATA F16MlMsrRegisters[] =
{
// MSR_TOM2 (0xC001001D)
// bits[39:23]   TOP_MEM2            = 0x0
  {
    MsrRegister,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ML_ALL                        // CpuRevision
    },
    AMD_PF_ALL,                             // PlatformFeatures
    {
      MSR_TOM2,                             // Address
      0x0000000000000000,                   // RegData
      0xFFFFFFFFFF800000,                   // RegMask
    }
  },

// MSR_SYS_CFG (0xC0010010)
// bits[21]      MtrrTom2En          = 0x1
  {
    MsrRegister,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ML_ALL                        // CpuRevision
    },
    AMD_PF_ALL,                             // PlatformFeatures
    {
      MSR_SYS_CFG,                          // Address
      (1 << 21),                            // RegData
      (1 << 21),                            // RegMask
    }
  },
// MC4_MISC_1 (0xC0000408)
// Clear to 0
  {
    MsrRegister,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ML_ALL                        // CpuRevision
    },
    AMD_PF_ALL,                             // PlatformFeatures
    {
      0xC0000408,                           // Address
      0x0000000000000000,                   // RegData
      0xFF0FFFFFFFFFFFFF,                   // RegMask
    }
  },
// MSR_LS_CFG (C0011020)
// bits[52]      LsWcPlusFastStringEn = 0x1
// bits[51]      LsWcFastStringEn    = 0x1
// bits[24]      WcPlusEn            = 0x1
  {
    MsrRegister,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ML_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MSR_LS_CFG,                           // Address
      0x0018000001000000,                   // RegData
      0x0018000001000000,                   // RegMask
    }}
  },
// Processor Feedback Constants 0 (C0011090)
// bits[15:8]                        = 0x64
// bits[7:0]                         = 0xA5
  {
    MsrRegister,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ML_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {
      0xC0011090,                           // Address
      0x00000000000064A5,                   // RegData
      0x000000000000FFFF,                   // RegMask
    }
  },
// MSR_L2I_CFG (C00110A0)
// bits[56:45]   L2ScrubberInterval  = 0x100
// bits[44]      PbDisObeysThrottleNb = 0x1
// bits[43:40]   ThrottleNbInterface = 0x0
// bits[19]      McaToMstCoreEn      = 0x1
  {
    MsrRegister,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ML_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {
      MSR_L2I_CFG,                          // Address
      0x0020100000080000,                   // RegData
      0x01FFFF0000080000,                   // RegMask
    }
  },
};

//   MSR with Special Programming Requirements Table
// ----------------------

STATIC CONST FAM_SPECIFIC_WORKAROUND_TYPE_ENTRY_INITIALIZER ROMDATA F16MlMsrWorkarounds[] =
{
// MSR_0000_0413
  {
    FamSpecificWorkaround,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ML_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      F16MlNbMcaLock,                       // Function call
      0x00000000,                           // Data
    }}
  },
// MSR_0000_0413
  {
    FamSpecificWorkaround,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ML_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      F16MlFPThrottle,                      // Function call
      0x00000000,                           // Data
    }}
  },
};

CONST REGISTER_TABLE ROMDATA F16MlMsrRegisterTable = {
  AllCores,
  (sizeof (F16MlMsrRegisters) / sizeof (TABLE_ENTRY_FIELDS)),
  (TABLE_ENTRY_FIELDS *) &F16MlMsrRegisters,
};

CONST REGISTER_TABLE ROMDATA F16MlMsrWorkaroundTable = {
  AllCores,
  (sizeof (F16MlMsrWorkarounds) / sizeof (TABLE_ENTRY_FIELDS)),
  (TABLE_ENTRY_FIELDS *) F16MlMsrWorkarounds,
};


/*---------------------------------------------------------------------------------------*/
/**
 *  MSR special programming requirements for MSR_0000_0413
 *
 *  AGESA should program MSR_0000_0413[61] with the fused value from MSR_0000_0413[50:49]
 *
 * @param[in]   Data         The table data value, for example to indicate which CPU and Platform types matched.
 * @param[in]   StdHeader    Config handle for library and services.
 *
 */
VOID
F16MlNbMcaLock (
  IN       UINT32            Data,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  MC4_MISC0_MSR Mc4Misc0;

  LibAmdMsrRead (MSR_MC4_MISC0, (UINT64 *) &Mc4Misc0, StdHeader);

  if (Mc4Misc0.IntType == 0x2) {
    Mc4Misc0.Locked = 1;
  } else {
    Mc4Misc0.Locked = 0;
  }
  LibAmdMsrWrite (MSR_MC4_MISC0, (UINT64 *) &Mc4Misc0, StdHeader);

  return;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  MSR special programming requirements for MSR_C001_1028[15:8]
 *
 *  AGESA should program MSR_C001_1028[15:8] with the fused value from D18F3x1FC[12:5]
 *
 * @param[in]   Data         The table data value, for example to indicate which CPU and Platform types matched.
 * @param[in]   StdHeader    Config handle for library and services.
 *
 */
VOID
F16MlFPThrottle (
  IN       UINT32            Data,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  PRODUCT_INFO_REGISTER PciData;
  FP_CFG_MSR MsrData;
  PCI_ADDR PciAddress;

  PciAddress.AddressValue = PRCT_INFO_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &PciData, StdHeader);
  LibAmdMsrRead (MSR_FP_CFG, (UINT64 *) &MsrData, StdHeader);
  MsrData.Pipe0ThrottlePacked = PciData.Pipe0ThrottlePacked;
  MsrData.Pipe0ThrottleSelect = PciData.Pipe0ThrottleSelect;
  MsrData.Pipe1ThrottlePacked = PciData.Pipe1ThrottlePacked;
  MsrData.Pipe1ThrottleSelect = PciData.Pipe1ThrottleSelect;
  IDS_OPTION_HOOK (IDS_PIPE_THROTTLE, (VOID *) (&MsrData), StdHeader);
  LibAmdMsrWrite (MSR_FP_CFG, (UINT64 *) &MsrData, StdHeader);
  return;
}
