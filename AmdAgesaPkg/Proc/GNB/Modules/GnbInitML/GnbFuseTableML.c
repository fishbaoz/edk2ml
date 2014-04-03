/* $NoKeywords:$ */
/**
 * @file
 *
 * Gnb fuse table
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
*****************************************************************************
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
* ***************************************************************************
*
*/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include  "AGESA.h"
#include  "Ids.h"
#include  "amdlib.h"
#include  "heapManager.h"
#include  "Gnb.h"
#include  "GnbGfxFamServices.h"
#include  "GnbCommonLib.h"
#include  "GnbFuseTable.h"
#include  "GnbFuseTableML.h"
#include  "GnbRegistersML.h"
#include  "GnbRegisterAccML.h"
#include  "GnbHandleLib.h"
#include  "OptionGnb.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITML_GNBFUSETABLEML_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern GNB_BUILD_OPTIONS GnbBuildOptions;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */



/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
GnbFuseTableDebugDumpML (
  IN       PP_FUSE_ARRAY_V2    *PpFuseArray,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  );


PP_FUSE_ARRAY_V2  DefaultPpFuseArrayML = {
  0,                              // PP table revision
  {0x40, 0,  0,  0, 0},           // VCLK DID
  {0x40, 0,  0,  0, 0},           // DCLK DID
  8,                              // Thermal SCLK
  {0,    0,  0,  0,  0,  0},      // Vclk/Dclk selector
  {0x40, 0x40, 0x40, 0, 0},       // Lclk DID
  {0,    0,  0,  0,  0},          // Displclk DID
  3,                              // Pcie Gen 2 VID
  0x10,                           // Main PLL id for 3200 VCO
  0,                              // WRCK SMU clock Divisor
  0,                              // GPU boost cap
  0,                              // Boost margin
  0,                              // Throttle margin
  TRUE,                           // Support VCE in PP table
  {0x3, 0xC, 0x30, 0xC0},         // VCE Flags
  0,                              // MCLK for VCE
  {0, 0, 0, 0},                   // SCLK selector for VCE
  {0x40, 0x40, 0x40, 0x40, 0},    // Eclk DID
  0,                              // HTC Enabled
  0,                              // HTC Temperature Limit
  0,                              // HTC Hysteresis Limit
  0,                              // GCK_PLL_FUSES
  { 1, 1, 1, 1},                  // DRAM Configuration Low [per DCT]
  { 0, 0, 0, 0},                  // DRAM Configuration High [per DCT]
  { 2, 2, 2, 2},                  // DRAM Controller Miscellaneous 2 [per DCT]
  { 0, 0, 0, 0},                  // Memory PState Control and Status [per DCT]
  { 0, 0, 0, 0},                  // NB PState Enable
  { 0, 0, 0, 0},                  // NB VID [6:0]
  { 0, 0, 0, 0},                  // NB VID [7]
  { 0, 0, 0, 0},                  // Memory P State
  { 0, 0, 0, 0},                  // NB FID
  { 1, 1, 1, 1},                   // NB DID
  { 0x24, 0x24, 0x24, 0x24, 0x24 }, // VddNbVid
  { 0x17, 0x18, 0x20, 0x22, 0x24 }, // SclkDid
  { 0x17, 0x18, 0x20, 0x22, 0x24 }, // SamclkDid
  { 0x1, 0x2, 0x3, 0x4, 0x5 },    // VddNbVidOffset
  0x1E,                           // PowerplayPerf
  0x3                             // PowerplayPS
};


FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0104007_TABLE_ML [] = {
  {
    D0F0xBC_xC0104007_VddNbVid0_OFFSET,
    D0F0xBC_xC0104007_VddNbVid0_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VddNbVid[0])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0104008_TABLE_ML [] = {
  {
    D0F0xBC_xC0104008_VddNbVid1_OFFSET,
    D0F0xBC_xC0104008_VddNbVid1_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VddNbVid[1])
  },
  {
    D0F0xBC_xC0104008_VddNbVid2_OFFSET,
    D0F0xBC_xC0104008_VddNbVid2_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VddNbVid[2])
  },
  {
    D0F0xBC_xC0104008_VddNbVid3_OFFSET,
    D0F0xBC_xC0104008_VddNbVid3_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VddNbVid[3])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC010400C_TABLE_ML [] = {
  {
    D0F0xBC_xC010400C_MainPllOpFreqIdStartup_OFFSET,
    D0F0xBC_xC010400C_MainPllOpFreqIdStartup_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, MainPllOpFreqIdStartup)
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC010407C_TABLE_ML [] = {
  {
    D0F0xBC_xC010407C_VddNbVid4_OFFSET,
    D0F0xBC_xC010407C_VddNbVid4_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VddNbVid[4])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0104080_TABLE_ML [] = {
  {
    D0F0xBC_xC0104080_VCEFlag0_OFFSET,
    D0F0xBC_xC0104080_VCEFlag0_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VceFlags[0])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0104083_TABLE_ML [] = {
  {
    D0F0xBC_xC0104083_VCEFlag1_OFFSET,
    D0F0xBC_xC0104083_VCEFlag1_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VceFlags[1])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0104084_TABLE_ML [] = {
  {
    D0F0xBC_xC0104084_VCEFlag2_OFFSET,
    D0F0xBC_xC0104084_VCEFlag2_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VceFlags[2])
  },
  {
    D0F0xBC_xC0104084_VCEFlag3_OFFSET,
    D0F0xBC_xC0104084_VCEFlag3_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VceFlags[3])
  },
  {
    D0F0xBC_xC0104084_VCEFlag4_OFFSET,
    D0F0xBC_xC0104084_VCEFlag4_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VceFlags[4])
  },
  {
    D0F0xBC_xC0104084_ReqSclkSel0_OFFSET,
    D0F0xBC_xC0104084_ReqSclkSel0_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VceReqSclkSel[0])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0104088_TABLE_ML [] = {
  {
    D0F0xBC_xC0104088_ReqSclkSel1_OFFSET,
    D0F0xBC_xC0104088_ReqSclkSel1_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VceReqSclkSel[1])
  },
  {
    D0F0xBC_xC0104088_ReqSclkSel2_OFFSET,
    D0F0xBC_xC0104088_ReqSclkSel2_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VceReqSclkSel[2])
  },
  {
    D0F0xBC_xC0104088_ReqSclkSel3_OFFSET,
    D0F0xBC_xC0104088_ReqSclkSel3_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VceReqSclkSel[3])
  },
  {
    D0F0xBC_xC0104088_ReqSclkSel4_OFFSET,
    D0F0xBC_xC0104088_ReqSclkSel4_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VceReqSclkSel[4])
  },
  {
    D0F0xBC_xC0104088_VCEMclk_OFFSET,
    D0F0xBC_xC0104088_VCEMclk_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VceMclk)
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC010408C_TABLE_ML [] = {
  {
    D0F0xBC_xC010408C_BapmDisable_OFFSET,
    D0F0xBC_xC010408C_BapmDisable_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, BapmDisable)
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC01040A8_TABLE_ML [] = {
  {
    D0F0xBC_xC01040A8_VddNbVidOffset0_OFFSET,
    D0F0xBC_xC01040A8_VddNbVidOffset0_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VddNbVidOffset[0])
  },
  {
    D0F0xBC_xC01040A8_VddNbVidOffset1_OFFSET,
    D0F0xBC_xC01040A8_VddNbVidOffset1_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VddNbVidOffset[1])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC01040AC_TABLE_ML [] = {
  {
    D0F0xBC_xC01040AC_VddNbVidOffset2_OFFSET,
    D0F0xBC_xC01040AC_VddNbVidOffset2_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VddNbVidOffset[2])
  },
  {
    D0F0xBC_xC01040AC_VddNbVidOffset3_OFFSET,
    D0F0xBC_xC01040AC_VddNbVidOffset3_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VddNbVidOffset[3])
  },
  {
    D0F0xBC_xC01040AC_VddNbVidOffset4_OFFSET,
    D0F0xBC_xC01040AC_VddNbVidOffset4_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VddNbVidOffset[4])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0107044_TABLE [] = {
  {
    D0F0xBC_xC0107044_NumBoostStates_OFFSET,
    D0F0xBC_xC0107044_NumBoostStates_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NumBoostStates)
  },
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0107064_TABLE_ML [] = {
  {
    D0F0xBC_xC0107064_PowerplayTableRev_OFFSET,
    D0F0xBC_xC0107064_PowerplayTableRev_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, PPlayTableRev)
  },
  {
    D0F0xBC_xC0107064_SClkThermDid_OFFSET,
    D0F0xBC_xC0107064_SClkThermDid_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, SclkThermDid)
  },
  {
    D0F0xBC_xC0107064_PcieGen2Vid_OFFSET,
    D0F0xBC_xC0107064_PcieGen2Vid_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, PcieGen2Vid)
  },
  {
    D0F0xBC_xC0107064_SClkDid0_OFFSET,
    D0F0xBC_xC0107064_SClkDid0_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, SclkDid[0])
  },
  {
    D0F0xBC_xC0107064_SClkDid1_OFFSET,
    D0F0xBC_xC0107064_SClkDid1_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, SclkDid[1])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0107067_TABLE_ML [] = {
  {
    D0F0xBC_xC0107067_SClkDid2_OFFSET,
    D0F0xBC_xC0107067_SClkDid2_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, SclkDid[2])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0107068_TABLE_ML [] = {
  {
    D0F0xBC_xC0107068_SClkDid3_OFFSET,
    D0F0xBC_xC0107068_SClkDid3_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, SclkDid[3])
  },
  {
    D0F0xBC_xC0107068_SClkDid4_OFFSET,
    D0F0xBC_xC0107068_SClkDid4_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, SclkDid[4])
  },
  {
    D0F0xBC_xC0107068_DispClkDid0_OFFSET,
    D0F0xBC_xC0107068_DispClkDid0_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, DispClkDid[0])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC010706B_TABLE_ML [] = {
  {
    D0F0xBC_xC010706B_DispClkDid1_OFFSET,
    D0F0xBC_xC010706B_DispClkDid1_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, DispClkDid[1])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC010706C_TABLE_ML [] = {
  {
    D0F0xBC_xC010706C_DispClkDid2_OFFSET,
    D0F0xBC_xC010706C_DispClkDid2_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, DispClkDid[2])
  },
  {
    D0F0xBC_xC010706C_DispClkDid3_OFFSET,
    D0F0xBC_xC010706C_DispClkDid3_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, DispClkDid[3])
  },
  {
    D0F0xBC_xC010706C_DispClkDid4_OFFSET,
    D0F0xBC_xC010706C_DispClkDid4_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, DispClkDid[4])
  },
  {
    D0F0xBC_xC010706C_LClkDid0_OFFSET,
    D0F0xBC_xC010706C_LClkDid0_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, LclkDid[0])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC010706F_TABLE_ML [] = {
  {
    D0F0xBC_xC010706F_LClkDid1_OFFSET,
    D0F0xBC_xC010706F_LClkDid1_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, LclkDid[1])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0107070_TABLE_ML [] = {
  {
    D0F0xBC_xC0107070_LClkDid2_OFFSET,
    D0F0xBC_xC0107070_LClkDid2_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, LclkDid[2])
  },
  {
    D0F0xBC_xC0107070_LClkDid3_OFFSET,
    D0F0xBC_xC0107070_LClkDid3_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, LclkDid[3])
  },
  {
    D0F0xBC_xC0107070_LClkDid4_OFFSET,
    D0F0xBC_xC0107070_LClkDid4_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, LclkDid[4])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0107073_TABLE_ML [] = {
  {
    D0F0xBC_xC0107073_DClkDid0_OFFSET,
    D0F0xBC_xC0107073_DClkDid0_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, DclkDid[0])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0107074_TABLE_ML [] = {
  {
    D0F0xBC_xC0107074_DClkDid1_OFFSET,
    D0F0xBC_xC0107074_DClkDid1_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, DclkDid[1])
  },
  {
    D0F0xBC_xC0107074_DClkDid2_OFFSET,
    D0F0xBC_xC0107074_DClkDid2_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, DclkDid[2])
  },
  {
    D0F0xBC_xC0107074_DClkDid3_OFFSET,
    D0F0xBC_xC0107074_DClkDid3_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, DclkDid[3])
  },
  {
    D0F0xBC_xC0107074_DClkDid4_OFFSET,
    D0F0xBC_xC0107074_DClkDid4_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, DclkDid[4])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0107077_TABLE_ML [] = {
  {
    D0F0xBC_xC0107077_VClkDid0_OFFSET,
    D0F0xBC_xC0107077_VClkDid0_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VclkDid[0])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0107078_TABLE_ML [] = {
  {
    D0F0xBC_xC0107078_VClkDid1_OFFSET,
    D0F0xBC_xC0107078_VClkDid1_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VclkDid[1])
  },
  {
    D0F0xBC_xC0107078_VClkDid2_OFFSET,
    D0F0xBC_xC0107078_VClkDid2_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VclkDid[2])
  },
  {
    D0F0xBC_xC0107078_VClkDid3_OFFSET,
    D0F0xBC_xC0107078_VClkDid3_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VclkDid[3])
  },
  {
    D0F0xBC_xC0107078_VClkDid4_OFFSET,
    D0F0xBC_xC0107078_VClkDid4_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, VclkDid[4])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC010707C_TABLE_ML [] = {
  {
    D0F0xBC_xC010707C_EClkDid0_OFFSET,
    D0F0xBC_xC010707C_EClkDid0_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, EclkDid[0])
  },
  {
    D0F0xBC_xC010707C_EClkDid1_OFFSET,
    D0F0xBC_xC010707C_EClkDid1_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, EclkDid[1])
  },
  {
    D0F0xBC_xC010707C_EClkDid2_OFFSET,
    D0F0xBC_xC010707C_EClkDid2_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, EclkDid[2])
  },
  {
    D0F0xBC_xC010707C_EClkDid3_OFFSET,
    D0F0xBC_xC010707C_EClkDid3_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, EclkDid[3])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC010707F_TABLE_ML [] = {
  {
    D0F0xBC_xC010707F_EClkDid4_OFFSET,
    D0F0xBC_xC010707F_EClkDid4_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, EclkDid[4])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0107080_TABLE_ML [] = {
  {
    D0F0xBC_xC0107080_PowerplayPerfFlag_OFFSET,
    D0F0xBC_xC0107080_PowerplayPerfFlag_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, PerfFlag)
  },
  {
    D0F0xBC_xC0107080_PowerplayPowerSavingsFlag_OFFSET,
    D0F0xBC_xC0107080_PowerplayPowerSavingsFlag_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, PowerSavingsFlag)
  },
  {
    D0F0xBC_xC0107080_SamClkDid0_OFFSET,
    D0F0xBC_xC0107080_SamClkDid0_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, SamClkDid[0])
  },
  {
    D0F0xBC_xC0107080_SamClkDid1_OFFSET,
    D0F0xBC_xC0107080_SamClkDid1_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, SamClkDid[1])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0107083_TABLE_ML [] = {
  {
    D0F0xBC_xC0107083_SamClkDid2_OFFSET,
    D0F0xBC_xC0107083_SamClkDid2_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, SamClkDid[2])
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0107084_TABLE_ML [] = {
  {
    D0F0xBC_xC0107084_SamClkDid3_OFFSET,
    D0F0xBC_xC0107084_SamClkDid3_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, SamClkDid[3])
  },
  {
    D0F0xBC_xC0107084_SamClkDid4_OFFSET,
    D0F0xBC_xC0107084_SamClkDid4_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, SamClkDid[4])
  }
};

FUSE_REGISTER_ENTRY_ML D18F3x64_TABLE_ML [] = {
  {
    D18F3x64_HtcEn_OFFSET,
    D18F3x64_HtcEn_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, HtcEn)
  },
  {
    D18F3x64_HtcTmpLmt_OFFSET,
    D18F3x64_HtcTmpLmt_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, HtcTmpLmt)
  },
  {
    D18F3x64_HtcHystLmt_OFFSET,
    D18F3x64_HtcHystLmt_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, HtcHystLmt)
  }
};

FUSE_REGISTER_ENTRY_ML D0F0xBC_xC0500000_TABLE_ML [] = {
  {
    D0F0xBC_xC0500000_MainPllOpFreqIdStartup_OFFSET,
    D0F0xBC_xC0500000_MainPllOpFreqIdStartup_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, MainPllOpFreqIdStartup)
  }
};

FUSE_REGISTER_ENTRY_ML D18F2x90_dct0_TABLE_ML [] = {
  {
    D18F2x90_dct0_DisDllShutdownSR_OFFSET,
    D18F2x90_dct0_DisDllShutdownSR_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, DisDllShutdownSR[0])
  }
};

FUSE_REGISTER_ENTRY_ML D18F2x94_dct0_TABLE_ML [] = {
  {
    D18F2x94_dct0_MemClkFreq_OFFSET,
    D18F2x94_dct0_MemClkFreq_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, MemClkFreq[0])
  }
};

FUSE_REGISTER_ENTRY_ML D18F2xA8_dct0_TABLE_ML [] = {
  {
    D18F2xA8_dct0_MemPhyPllPdMode_OFFSET,
    D18F2xA8_dct0_MemPhyPllPdMode_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, MemPhyPllPdMode[0])
  }
};

FUSE_REGISTER_ENTRY_ML D18F2x2E0_dct0_TABLE_ML [] = {
  {
    D18F2x2E0_dct0_M1MemClkFreq_OFFSET,
    D18F2x2E0_dct0_M1MemClkFreq_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, M1MemClkFreq[0])
  }
};

FUSE_REGISTER_ENTRY_ML D18F5x160_TABLE_ML [] = {
  {
    D18F5x160_NbPstateEn_OFFSET,
    D18F5x160_NbPstateEn_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbPstateEn[0])
  },
  {
    D18F5x160_MemPstate_OFFSET,
    D18F5x160_MemPstate_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, MemPstate[0])
  },
  {
    D18F5x160_NbFid_OFFSET,
    D18F5x160_NbFid_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbFid[0])
  },
  {
    D18F5x160_NbDid_OFFSET,
    D18F5x160_NbDid_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbDid[0])
  },
  {
    D18F5x160_NbVid_6_0_OFFSET,
    D18F5x160_NbVid_6_0_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbVid_6_0[0])
  },
  {
    D18F5x160_NbVid_7_OFFSET,
    D18F5x160_NbVid_7_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbVid_7[0])
  }
};

FUSE_REGISTER_ENTRY_ML D18F5x164_TABLE_ML [] = {
  {
    D18F5x164_NbPstateEn_OFFSET,
    D18F5x164_NbPstateEn_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbPstateEn[1])
  },
  {
    D18F5x164_MemPstate_OFFSET,
    D18F5x164_MemPstate_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, MemPstate[1])
  },
  {
    D18F5x164_NbFid_OFFSET,
    D18F5x164_NbFid_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbFid[1])
  },
  {
    D18F5x164_NbDid_OFFSET,
    D18F5x164_NbDid_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbDid[1])
  },
  {
    D18F5x164_NbVid_6_0_OFFSET,
    D18F5x164_NbVid_6_0_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbVid_6_0[1])
  },
  {
    D18F5x164_NbVid_7_OFFSET,
    D18F5x164_NbVid_7_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbVid_7[1])
  }
};

FUSE_REGISTER_ENTRY_ML D18F5x168_TABLE_ML [] = {
  {
    D18F5x168_NbPstateEn_OFFSET,
    D18F5x168_NbPstateEn_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbPstateEn[2])
  },
  {
    D18F5x168_MemPstate_OFFSET,
    D18F5x168_MemPstate_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, MemPstate[2])
  },
  {
    D18F5x168_NbFid_OFFSET,
    D18F5x168_NbFid_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbFid[2])
  },
  {
    D18F5x168_NbDid_OFFSET,
    D18F5x168_NbDid_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbDid[2])
  },
  {
    D18F5x168_NbVid_6_0_OFFSET,
    D18F5x168_NbVid_6_0_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbVid_6_0[2])
  },
  {
    D18F5x168_NbVid_7_OFFSET,
    D18F5x168_NbVid_7_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbVid_7[2])
  }
};

FUSE_REGISTER_ENTRY_ML D18F5x16C_TABLE_ML [] = {
  {
    D18F5x16C_NbPstateEn_OFFSET,
    D18F5x16C_NbPstateEn_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbPstateEn[3])
  },
  {
    D18F5x16C_MemPstate_OFFSET,
    D18F5x16C_MemPstate_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, MemPstate[3])
  },
  {
    D18F5x16C_NbFid_OFFSET,
    D18F5x16C_NbFid_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbFid[3])
  },
  {
    D18F5x16C_NbDid_OFFSET,
    D18F5x16C_NbDid_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbDid[3])
  },
  {
    D18F5x16C_NbVid_6_0_OFFSET,
    D18F5x16C_NbVid_6_0_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbVid_6_0[3])
  },
  {
    D18F5x16C_NbVid_7_OFFSET,
    D18F5x16C_NbVid_7_WIDTH,
    (UINT16) offsetof (PP_FUSE_ARRAY_V2, NbVid_7[3])
  }
};


FUSE_TABLE_ENTRY_ML FuseRegisterTableML [] = {

  {
    D0F0xBC_xC0104007_TYPE,
    D0F0xBC_xC0104007_ADDRESS,
    sizeof (D0F0xBC_xC0104007_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0104007_TABLE_ML
  },
  {
    D0F0xBC_xC0104008_TYPE,
    D0F0xBC_xC0104008_ADDRESS,
    sizeof (D0F0xBC_xC0104008_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0104008_TABLE_ML
  },
  {
    D0F0xBC_xC010400C_TYPE,
    D0F0xBC_xC010400C_ADDRESS,
    sizeof (D0F0xBC_xC010400C_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC010400C_TABLE_ML
  },
  {
    D0F0xBC_xC010407C_TYPE,
    D0F0xBC_xC010407C_ADDRESS,
    sizeof (D0F0xBC_xC010407C_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC010407C_TABLE_ML
  },
  {
    D0F0xBC_xC0104080_TYPE,
    D0F0xBC_xC0104080_ADDRESS,
    sizeof (D0F0xBC_xC0104080_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0104080_TABLE_ML
  },
  {
    D0F0xBC_xC0104083_TYPE,
    D0F0xBC_xC0104083_ADDRESS,
    sizeof (D0F0xBC_xC0104083_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0104083_TABLE_ML
  },
  {
    D0F0xBC_xC0104084_TYPE,
    D0F0xBC_xC0104084_ADDRESS,
    sizeof (D0F0xBC_xC0104084_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0104084_TABLE_ML
  },
  {
    D0F0xBC_xC0104088_TYPE,
    D0F0xBC_xC0104088_ADDRESS,
    sizeof (D0F0xBC_xC0104088_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0104088_TABLE_ML
  },
  {
    D0F0xBC_xC010408C_TYPE,
    D0F0xBC_xC010408C_ADDRESS,
    sizeof (D0F0xBC_xC010408C_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC010408C_TABLE_ML
  },
  {
    D0F0xBC_xC01040A8_TYPE,
    D0F0xBC_xC01040A8_ADDRESS,
    sizeof (D0F0xBC_xC01040A8_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC01040A8_TABLE_ML
  },
  {
    D0F0xBC_xC01040AC_TYPE,
    D0F0xBC_xC01040AC_ADDRESS,
    sizeof (D0F0xBC_xC01040AC_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC01040AC_TABLE_ML
  },
  {
    D0F0xBC_xC0107044_TYPE,
    D0F0xBC_xC0107044_ADDRESS,
    sizeof (D0F0xBC_xC0107044_TABLE) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0107044_TABLE
  },
  {
    D0F0xBC_xC0107064_TYPE,
    D0F0xBC_xC0107064_ADDRESS,
    sizeof (D0F0xBC_xC0107064_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0107064_TABLE_ML
  },
  {
    D0F0xBC_xC0107067_TYPE,
    D0F0xBC_xC0107067_ADDRESS,
    sizeof (D0F0xBC_xC0107067_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0107067_TABLE_ML
  },
  {
    D0F0xBC_xC0107068_TYPE,
    D0F0xBC_xC0107068_ADDRESS,
    sizeof (D0F0xBC_xC0107068_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0107068_TABLE_ML
  },
  {
    D0F0xBC_xC010706B_TYPE,
    D0F0xBC_xC010706B_ADDRESS,
    sizeof (D0F0xBC_xC010706B_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC010706B_TABLE_ML
  },
  {
    D0F0xBC_xC010706C_TYPE,
    D0F0xBC_xC010706C_ADDRESS,
    sizeof (D0F0xBC_xC010706C_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC010706C_TABLE_ML
  },
  {
    D0F0xBC_xC010706F_TYPE,
    D0F0xBC_xC010706F_ADDRESS,
    sizeof (D0F0xBC_xC010706F_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC010706F_TABLE_ML
  },
  {
    D0F0xBC_xC0107070_TYPE,
    D0F0xBC_xC0107070_ADDRESS,
    sizeof (D0F0xBC_xC0107070_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0107070_TABLE_ML
  },
  {
    D0F0xBC_xC0107073_TYPE,
    D0F0xBC_xC0107073_ADDRESS,
    sizeof (D0F0xBC_xC0107073_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0107073_TABLE_ML
  },
  {
    D0F0xBC_xC0107074_TYPE,
    D0F0xBC_xC0107074_ADDRESS,
    sizeof (D0F0xBC_xC0107074_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0107074_TABLE_ML
  },
  {
    D0F0xBC_xC0107077_TYPE,
    D0F0xBC_xC0107077_ADDRESS,
    sizeof (D0F0xBC_xC0107077_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0107077_TABLE_ML
  },
  {
    D0F0xBC_xC0107078_TYPE,
    D0F0xBC_xC0107078_ADDRESS,
    sizeof (D0F0xBC_xC0107078_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0107078_TABLE_ML
  },
  {
    D0F0xBC_xC010707C_TYPE,
    D0F0xBC_xC010707C_ADDRESS,
    sizeof (D0F0xBC_xC010707C_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC010707C_TABLE_ML
  },
  {
    D0F0xBC_xC010707F_TYPE,
    D0F0xBC_xC010707F_ADDRESS,
    sizeof (D0F0xBC_xC010707F_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC010707F_TABLE_ML
  },
  {
    D0F0xBC_xC0107080_TYPE,
    D0F0xBC_xC0107080_ADDRESS,
    sizeof (D0F0xBC_xC0107080_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0107080_TABLE_ML
  },
  {
    D0F0xBC_xC0107083_TYPE,
    D0F0xBC_xC0107083_ADDRESS,
    sizeof (D0F0xBC_xC0107083_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0107083_TABLE_ML
  },
  {
    D0F0xBC_xC0107084_TYPE,
    D0F0xBC_xC0107084_ADDRESS,
    sizeof (D0F0xBC_xC0107084_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0107084_TABLE_ML
  }
};

FUSE_TABLE_ENTRY_ML  PPRegisterTableML [] = {
  {
    D18F3x64_TYPE,
    D18F3x64_ADDRESS,
    sizeof (D18F3x64_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D18F3x64_TABLE_ML
  },
  {
    D0F0xBC_xC0500000_TYPE,
    D0F0xBC_xC0500000_ADDRESS,
    sizeof (D0F0xBC_xC0500000_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D0F0xBC_xC0500000_TABLE_ML
  },
  {
    D18F2x90_dct0_TYPE,
    D18F2x90_dct0_ADDRESS,
    sizeof (D18F2x90_dct0_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D18F2x90_dct0_TABLE_ML
  },
  {
    D18F2x94_dct0_TYPE,
    D18F2x94_dct0_ADDRESS,
    sizeof (D18F2x94_dct0_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D18F2x94_dct0_TABLE_ML
  },
  {
    D18F2xA8_dct0_TYPE,
    D18F2xA8_dct0_ADDRESS,
    sizeof (D18F2xA8_dct0_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D18F2xA8_dct0_TABLE_ML
  },
  {
    D18F5x160_TYPE,
    D18F5x160_ADDRESS,
    sizeof (D18F5x160_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D18F5x160_TABLE_ML
  },
  {
    D18F5x164_TYPE,
    D18F5x164_ADDRESS,
    sizeof (D18F5x164_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D18F5x164_TABLE_ML
  },
  {
    D18F5x168_TYPE,
    D18F5x168_ADDRESS,
    sizeof (D18F5x168_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D18F5x168_TABLE_ML
  },
  {
    D18F5x16C_TYPE,
    D18F5x16C_ADDRESS,
    sizeof (D18F5x16C_TABLE_ML) / sizeof (FUSE_REGISTER_ENTRY_ML),
    D18F5x16C_TABLE_ML
  }
};


FUSE_TABLE_ML  FuseTableML = {
  sizeof (FuseRegisterTableML) / sizeof (FUSE_TABLE_ENTRY_ML),
  FuseRegisterTableML
};

FUSE_TABLE_ML  PPTableML = {
  sizeof (PPRegisterTableML) / sizeof (FUSE_TABLE_ENTRY_ML),
  PPRegisterTableML
};


/*----------------------------------------------------------------------------------------*/
/**
 * Load Fuse Table ML
 *
 *
 * param[out] PpFuseArray      Pointer to save fuse table
 * param[in]  StdHeader        Pointer to Standard configuration
 * retval     AGESA_STATUS
 */

VOID
NbFuseLoadFuseTableML (
  IN       FUSE_TABLE_ML       *FuseTable,
     OUT   PP_FUSE_ARRAY_V2    *PpFuseArray,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINTN                     RegisterIndex;

  for (RegisterIndex = 0; RegisterIndex < FuseTable->FuseTableLength; RegisterIndex++ ) {
    UINTN   FieldIndex;
    UINTN   FuseRegisterTableLength;
    UINT32  FuseValue;
    FuseRegisterTableLength = FuseTable->FuseTable[RegisterIndex].FuseRegisterTableLength;

    GnbRegisterReadML (
      GnbGetHandle (StdHeader),
      FuseTable->FuseTable[RegisterIndex].RegisterSpaceType,
      FuseTable->FuseTable[RegisterIndex].Register,
      &FuseValue,
      0,
      StdHeader
    );
    for (FieldIndex = 0; FieldIndex < FuseRegisterTableLength;  FieldIndex++) {
      FUSE_REGISTER_ENTRY_ML   RegisterEntry;
      RegisterEntry = FuseTable->FuseTable[RegisterIndex].FuseRegisterTable[FieldIndex];
      *((UINT8 *) PpFuseArray + RegisterEntry.FuseOffset) = (UINT8) ((FuseValue >> RegisterEntry.FieldOffset) &
                                                            ((1 << RegisterEntry.FieldWidth) - 1));
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Gnb load fuse table
 *
 *
 *
 * @param[in] StdHeader        Pointer to Standard configuration
 * @retval    AGESA_STATUS
 */

AGESA_STATUS
GnbLoadFuseTableML (
  IN      AMD_CONFIG_PARAMS   *StdHeader
  )
{
  PP_FUSE_ARRAY_V2       *PpFuseArray;
  AGESA_STATUS           Status;
  D18F3xA0_STRUCT        D18F3xA0;

  Status = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbLoadFuseTableML Enter\n");

  PpFuseArray = (PP_FUSE_ARRAY_V2 *) GnbAllocateHeapBuffer (AMD_PP_FUSE_TABLE_HANDLE, sizeof (PP_FUSE_ARRAY_V2), StdHeader);
  ASSERT (PpFuseArray != NULL);
  if (PpFuseArray != NULL) {
    //Support for real fuse table
    GnbRegisterReadML (GnbGetHandle (StdHeader), D18F3xA0_TYPE, D18F3xA0_ADDRESS, &D18F3xA0.Value, 0, StdHeader);

    if ((D18F3xA0.Field.CofVidProg) && (GnbBuildOptions.GnbLoadRealFuseTable)) {
      NbFuseLoadFuseTableML (&FuseTableML, PpFuseArray, StdHeader);
      PpFuseArray->VceStateTableSupport = TRUE;
      IDS_HDT_CONSOLE (NB_MISC, "  Processor Fused\n");
    } else {
      LibAmdMemCopy (PpFuseArray, &DefaultPpFuseArrayML, sizeof (PP_FUSE_ARRAY_V2), StdHeader);
      IDS_HDT_CONSOLE (NB_MISC, "  Processor Unfused\n");
    }
    NbFuseLoadFuseTableML (&PPTableML, PpFuseArray, StdHeader);
    IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_PPFUSE_OVERRIDE, PpFuseArray, StdHeader);
    GnbFuseTableDebugDumpML (PpFuseArray, StdHeader);
  } else {
    Status = AGESA_ERROR;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbLoadFuseTableML Exit [0x%x]\n", Status);
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Override fuses based on AGESA params
 *
 *
 * @param[in]   StdHeader     Standard configuration header
 * @retval      AGESA_STATUS
 */
AGESA_STATUS
GnbOverrideFuseTableML (
  IN      AMD_CONFIG_PARAMS       *StdHeader
  )
{
  AGESA_STATUS          AgesaStatus;
  PP_FUSE_ARRAY_V2      *PpFuseArray;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbOverrideFuseTableML Enter\n");

  AgesaStatus = AGESA_SUCCESS;
  PpFuseArray = GnbLocateHeapBuffer (AMD_PP_FUSE_TABLE_HANDLE, StdHeader);

  ASSERT (PpFuseArray != NULL);

  if (PpFuseArray == NULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "  ERROR! Cannot locate fuse table\n");
    AgesaStatus = AGESA_ERROR;
    return AgesaStatus;
  }

  if (PpFuseArray->BapmDisable == 1) {
    IDS_HDT_CONSOLE (GNB_TRACE, "  Skip override\n");
    return AgesaStatus;
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "  PerfFlag => %02x\n", PpFuseArray->PerfFlag);
  IDS_HDT_CONSOLE (GNB_TRACE, "  PsFlag => %02x\n", PpFuseArray->PowerSavingsFlag);
  IDS_HDT_CONSOLE (GNB_TRACE, "  VceReqSclkSel[2] => %02x\n", PpFuseArray->VceReqSclkSel[2]);

  PpFuseArray->VceFlags[0] = 0;
  PpFuseArray->VceFlags[1] = 0;
  PpFuseArray->VceFlags[2] = 0xFF;
  PpFuseArray->VceFlags[3] = 0;
  PpFuseArray->VceFlags[4] = 0;
  PpFuseArray->PerfFlag = 0x7;
  PpFuseArray->VceReqSclkSel[2] = 2;

  if (PpFuseArray->PowerSavingsFlag > PpFuseArray->PerfFlag) {
    PpFuseArray->PowerSavingsFlag = PpFuseArray->PerfFlag;
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "  Override PerfFlag => %02x\n", PpFuseArray->PerfFlag);
  IDS_HDT_CONSOLE (GNB_TRACE, "  Override PsFlag => %02x\n", PpFuseArray->PowerSavingsFlag);
  IDS_HDT_CONSOLE (GNB_TRACE, "  VceReqSclkSel[2] => %02x\n", PpFuseArray->VceReqSclkSel[2]);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbOverrideFuseTableML Enter\n");
  return AgesaStatus;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Debug dump fuse table
 *
 *
 * @param[out] PpFuseArray      Pointer to save fuse table
 * @param[in]  StdHeader        Pointer to Standard configuration
 */

VOID
GnbFuseTableDebugDumpML (
  IN       PP_FUSE_ARRAY_V2    *PpFuseArray,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINTN   Index;

  IDS_HDT_CONSOLE (NB_MISC, "<------------ GNB FUSE TABLE------------>\n");

  for (Index = 0; Index < 5; Index++) {
    IDS_HDT_CONSOLE (
      NB_MISC,
      "  VCLK DID[%d] - 0x%02x (%dMHz)\n",
      Index,
      PpFuseArray->VclkDid[Index],
      (PpFuseArray->VclkDid[Index] != 0) ? (GfxFmCalculateClock (PpFuseArray->VclkDid[Index], StdHeader) / 100) : 0
      );
    IDS_HDT_CONSOLE (
      NB_MISC,
      "  DCLK DID[%d] - 0x%02x (%dMHz)\n",
      Index,
      PpFuseArray->DclkDid[Index],
      (PpFuseArray->DclkDid[Index] != 0) ? (GfxFmCalculateClock (PpFuseArray->DclkDid[Index], StdHeader) / 100) : 0
    );
  }
  for (Index = 0; Index < 5; Index++) {
    IDS_HDT_CONSOLE (
      NB_MISC,
      "  DISPCLK DID[%d] - 0x%02x (%dMHz)\n",
      Index,
      PpFuseArray->DispClkDid[Index],
      (PpFuseArray->DispClkDid[Index] != 0) ? (GfxFmCalculateClock (PpFuseArray->DispClkDid[Index], StdHeader) / 100) : 0
      );
  }
  for (Index = 0; Index < 5; Index++) {
    IDS_HDT_CONSOLE (
      NB_MISC,
      "  ECLK DID[%d] - 0x%02x (%dMHz)\n",
      Index,
      PpFuseArray->EclkDid[Index],
      (PpFuseArray->EclkDid[Index] != 0) ?
          (GfxFmCalculateClock (PpFuseArray->EclkDid[Index], StdHeader) / 100) : 0
      );
  }
  for (Index = 0; Index < 5; Index++) {
    IDS_HDT_CONSOLE (
      NB_MISC,
      "  SamuCLK DID[%d] - 0x%02x (%dMHz)\n",
      Index,
      PpFuseArray->SamClkDid[Index],
      (PpFuseArray->SamClkDid[Index] != 0) ?
          (GfxFmCalculateClock (PpFuseArray->SamClkDid[Index], StdHeader) / 100) : 0
      );
  }
  for (Index = 0; Index < 5; Index++) {
    IDS_HDT_CONSOLE (
      NB_MISC,
      "  VceReqSclkSel[%d] - 0x%02x\n",
      Index,
      PpFuseArray->VceReqSclkSel[Index]
      );
    IDS_HDT_CONSOLE (
      NB_MISC,
      "  VCE Flags[%d] - 0x%02x\n",
      Index,
      PpFuseArray->VceFlags[Index]
      );
  }
  for (Index = 0; Index < 5; Index++) {
    IDS_HDT_CONSOLE (
      NB_MISC,
      "  SCLK DID[%d] - 0x%02x (%dMHz)\n",
      Index,
      PpFuseArray->SclkDid[Index],
      (PpFuseArray->SclkDid[Index] != 0) ?
          (GfxFmCalculateClock (PpFuseArray->SclkDid[Index], StdHeader) / 100) : 0
    );
    IDS_HDT_CONSOLE (NB_MISC, "  SCLK VID[%d] - 0x%02x\n", Index, PpFuseArray->VddNbVid[Index]);
  }
  for (Index = 0; Index < 6; Index++) {
    IDS_HDT_CONSOLE (NB_MISC, "  State #%d\n", Index);
    IDS_HDT_CONSOLE (NB_MISC, "    Vclk/Dclk Index - 0x%x\n", PpFuseArray->VclkDclkSel[Index]);
  }
  IDS_HDT_CONSOLE (NB_MISC, "  GEN2 VID - 0x%x\n", PpFuseArray->PcieGen2Vid);
  IDS_HDT_CONSOLE (NB_MISC, "  Main PLL Id - 0x%x\n", PpFuseArray->MainPllId);
  IDS_HDT_CONSOLE (NB_MISC, "  GpuBoostCap  - %x\n", PpFuseArray->GpuBoostCap);
  IDS_HDT_CONSOLE (NB_MISC, "  SclkDpmBoostMargin    - %x\n", PpFuseArray->SclkDpmBoostMargin);
  IDS_HDT_CONSOLE (NB_MISC, "  SclkDpmThrottleMargin - %x\n", PpFuseArray->SclkDpmThrottleMargin);
  IDS_HDT_CONSOLE (
    NB_MISC, "  SclkThermDid          - %x(%dMHz)\n",
    PpFuseArray->SclkThermDid,
    (PpFuseArray->SclkThermDid != 0) ?
        (GfxFmCalculateClock (PpFuseArray->SclkThermDid, StdHeader) / 100) : 0
    );
  if (PpFuseArray->HtcEn == 0) {
    IDS_HDT_CONSOLE (NB_MISC, "  HtcEn                 - Disabled\n");
  } else {
    IDS_HDT_CONSOLE (NB_MISC, "  HtcEn                 - Enabled\n    HtcTmpLmt = %d - HtcHystLmt = %d", PpFuseArray->HtcTmpLmt, PpFuseArray->HtcHystLmt);
  }
  for (Index = 0; Index < 4; Index++) {
    if (PpFuseArray->NbPstateEn[Index] == 0) {
      IDS_HDT_CONSOLE (NB_MISC, "  NbPstate[%d]          - Disabled\n", Index);
    } else {
      IDS_HDT_CONSOLE (NB_MISC, "  NbPstate[%d]          - Enabled\n", Index);
      IDS_HDT_CONSOLE (NB_MISC, "    NbDid = %d\n", PpFuseArray->NbDid[Index]);
      IDS_HDT_CONSOLE (NB_MISC, "    NbFid = %d\n", PpFuseArray->NbFid[Index]);
      IDS_HDT_CONSOLE (NB_MISC, "    MemPstate = %d\n", PpFuseArray->MemPstate[Index]);
      IDS_HDT_CONSOLE (NB_MISC, "    NbVid = %d\n", PpFuseArray->NbVid_6_0[Index] + (PpFuseArray->NbVid_7[Index] << 7));
    }
  }
  IDS_HDT_CONSOLE (NB_MISC, "  MainPllOpFreqIdStartup - %d\n", PpFuseArray->MainPllOpFreqIdStartup);
  for (Index = 0; Index < 4; Index++) {
    IDS_HDT_CONSOLE (NB_MISC, "  DisDllShutdownSR[%d]  - %d \n", Index, PpFuseArray->DisDllShutdownSR[Index]);
  }
  for (Index = 0; Index < 4; Index++) {
    IDS_HDT_CONSOLE (NB_MISC, "  MemPhyPllPdMode[%d]   - %d \n", Index, PpFuseArray->MemPhyPllPdMode[Index]);
  }
  for (Index = 0; Index < 4; Index++) {
    IDS_HDT_CONSOLE (NB_MISC, "  MemClkFreq[%d]        - %d \n", Index, PpFuseArray->MemClkFreq[Index]);
    IDS_HDT_CONSOLE (NB_MISC, "  M1MemClkFreq[%d]      - %d \n", Index, PpFuseArray->M1MemClkFreq[Index]);
  }
  IDS_HDT_CONSOLE (NB_MISC, "  PerfFlag        - %02x \n", PpFuseArray->PerfFlag);
  IDS_HDT_CONSOLE (NB_MISC, "  PSFlag          - %02x \n", PpFuseArray->PowerSavingsFlag);
  IDS_HDT_CONSOLE (NB_MISC, "  BapmDisable     - %02x \n", PpFuseArray->BapmDisable);
  IDS_HDT_CONSOLE (NB_MISC, "<------------ GNB FUSE END-------------->\n");
}
