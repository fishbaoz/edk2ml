/* $NoKeywords:$ */
/**
 * @file
 *
 * Integrated table info init
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 282216 $   @e \$Date: 2014-01-08 09:42:15 -0600 (Wed, 08 Jan 2014) $
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
#include  "amdlib.h"
#include  "Ids.h"
#include  "heapManager.h"
#include  "GeneralServices.h"
#include  "Gnb.h"
#include  "GnbFuseTable.h"
#include  "GnbPcie.h"
#include  "GnbGfx.h"
#include  "GnbSbLib.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieConfig.h"
#include  "GnbGfxConfig.h"
#include  "GnbGfxInitLibV1.h"
#include  "GnbGfxFamServices.h"
#include  "GnbRegistersML.h"
#include  "GnbRegisterAccML.h"
#include  "GnbNbInitLibV1.h"
#include  "GfxConfigLib.h"
#include  "GfxIntegratedInfoTable.h"
#include  "GfxPwrPlayTable.h"
#include  "GfxLibML.h"
#include  "GfxLibV3.h"
#include  "OptionGnb.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITML_GFXINTEGRATEDINFOTABLEML_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define GFX_REFCLK                        100     // (in MHz) Reference clock is 100 MHz
#define GFX_NCLK_MIN                      700     // (in MHz) Minimum value for NCLK is 700 MHz
extern BUILD_OPT_CFG UserOptions;
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

CONST UINT8  DdiLaneConfigArrayML [][4] = {
  {8,  11, 0, 0},
  {12, 15, 1, 1},
  {11, 8,  0, 0},
  {15, 12, 1, 1},
  {16, 19, 6, 6},
  {19, 16, 6, 6}
};

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize display path for given engine
 *
 *
 *
 * @param[in]   Engine            Engine configuration info
 * @param[out]  DisplayPathList   Display path list
 * @param[in]   Gfx               Pointer to global GFX configuration
 */

AGESA_STATUS
GfxMapEngineToDisplayPathML (
  IN       PCIe_ENGINE_CONFIG          *Engine,
     OUT   EXT_DISPLAY_PATH            *DisplayPathList,
  IN       GFX_PLATFORM_CONFIG         *Gfx
  )
{
  AGESA_STATUS      Status;
  UINT8             PrimaryDisplayPathId;
  UINT8             SecondaryDisplayPathId;
  UINTN             DisplayPathIndex;
  PrimaryDisplayPathId = 0xff;
  SecondaryDisplayPathId = 0xff;
  for (DisplayPathIndex = 0; DisplayPathIndex <  (sizeof (DdiLaneConfigArrayML) / 4); DisplayPathIndex++) {
    if (DdiLaneConfigArrayML[DisplayPathIndex][0] == Engine->EngineData.StartLane &&
        DdiLaneConfigArrayML[DisplayPathIndex][1] == Engine->EngineData.EndLane) {
      PrimaryDisplayPathId = DdiLaneConfigArrayML[DisplayPathIndex][2];
      SecondaryDisplayPathId = DdiLaneConfigArrayML[DisplayPathIndex][3];
      break;
    }
  }

  if (Engine->Type.Ddi.DdiData.ConnectorType == ConnectorTypeDualLinkDVI ||
     (Engine->Type.Ddi.DdiData.ConnectorType == ConnectorTypeLvds && PrimaryDisplayPathId != 0)) {
    // Display config invalid for ML
    PrimaryDisplayPathId = 0xff;
  }

  if (PrimaryDisplayPathId != 0xff) {
    ASSERT (Engine->Type.Ddi.DdiData.AuxIndex <= Aux3);
    IDS_HDT_CONSOLE (GFX_MISC, "  Allocate Display Connector at Primary sPath[%d]\n", PrimaryDisplayPathId);
    Engine->InitStatus |= INIT_STATUS_DDI_ACTIVE;
    GfxIntegratedCopyDisplayInfo (
      Engine,
      &DisplayPathList[PrimaryDisplayPathId],
      (PrimaryDisplayPathId != SecondaryDisplayPathId) ? &DisplayPathList[SecondaryDisplayPathId] : NULL,
      Gfx
      );

    if (Engine->Type.Ddi.DdiData.ConnectorType == ConnectorTypeSingleLinkDviI) {
      LibAmdMemCopy (&DisplayPathList[6], &DisplayPathList[PrimaryDisplayPathId], sizeof (EXT_DISPLAY_PATH), GnbLibGetHeader (Gfx));
      DisplayPathList[6].usDeviceACPIEnum = 0x100;
      DisplayPathList[6].usDeviceTag = ATOM_DEVICE_CRT1_SUPPORT;
    }

    Status = AGESA_SUCCESS;
  } else {
    IDS_HDT_CONSOLE (GFX_MISC, "  Error!!! Map DDI lanes %d - %d to display path failed\n",
      Engine->EngineData.StartLane,
      Engine->EngineData.EndLane
      );
    PutEventLog (
      AGESA_ERROR,
      GNB_EVENT_INVALID_DDI_LINK_CONFIGURATION,
      Engine->EngineData.StartLane,
      Engine->EngineData.EndLane,
      0,
      0,
      GnbLibGetHeader (Gfx)
      );
    Status = AGESA_ERROR;
  }
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 *Init ML Nb p-State MemclkFreq
 *
 *
 * @param[in] IntegratedInfoTable Integrated info table pointer
 * @param[in] Gfx                 Gfx configuration info
 */

VOID
GfxFillNbPstateMemclkFreqML (
  IN OUT   ATOM_INTEGRATED_SYSTEM_INFO_V1_8 *IntegratedInfoTable,
  IN       GFX_PLATFORM_CONFIG              *Gfx
  )
{
  D18F2x94_dct0_STRUCT            D18F2x94;
  D18F2x2E0_dct0_STRUCT           D18F2x2E0;
  D18F5x160_STRUCT                NbPstate;
  UINT8                           i;
  ULONG                           memps0_freq;
  ULONG                           memps1_freq;
  UINT8                           last_valid_pstate;
  GNB_HANDLE        *GnbHandle;

  GnbHandle = GnbGetHandle (GnbLibGetHeader (Gfx));
  ASSERT (GnbHandle != NULL);

  GnbRegisterReadML (
    GnbHandle,
    D18F2x94_dct0_TYPE,
    D18F2x94_dct0_ADDRESS,
    &D18F2x94.Value,
    0,
    GnbLibGetHeader (Gfx)
    );

  GnbRegisterReadML (
    GnbHandle,
    D18F2x2E0_dct0_TYPE,
    D18F2x2E0_dct0_ADDRESS,
    &D18F2x2E0.Value,
    0,
    GnbLibGetHeader (Gfx)
    );

  memps0_freq = 100 * GfxLibExtractDramFrequencyV3 ((UINT8) D18F2x94.Field.MemClkFreq, GnbLibGetHeader (Gfx));
  memps1_freq = 100 * GfxLibExtractDramFrequencyV3 ((UINT8) D18F2x2E0.Field.M1MemClkFreq, GnbLibGetHeader (Gfx));

  last_valid_pstate = 0;
  for (i = 0; i < 4; i++) {
    NbPstate.Value = 0;
    GnbRegisterReadML (
      GnbHandle,
      TYPE_D18F5,
      (D18F5x160_ADDRESS + (i * 4)),
      &NbPstate.Value,
      0,
      GnbLibGetHeader (Gfx)
      );
    if (NbPstate.Field.NbPstateEn == 1) {
      last_valid_pstate = i;
      IntegratedInfoTable->ulNbpStateMemclkFreq[i] = (NbPstate.Field.MemPstate == 0) ? memps0_freq : memps1_freq;
    } else {
      IntegratedInfoTable->ulNbpStateMemclkFreq[i] =
          IntegratedInfoTable->ulNbpStateMemclkFreq[last_valid_pstate];
    }
  }

  for (i = 0; i < 4; i++) {
    IDS_HDT_CONSOLE (GNB_TRACE, "  Nclk[%d] = %08x\n", i, IntegratedInfoTable->ulNbpStateNClkFreq[i]);
    IDS_HDT_CONSOLE (GNB_TRACE, "  Mclk[%d] = %08x\n", i, IntegratedInfoTable->ulNbpStateMemclkFreq[i]);
  }

}

/*----------------------------------------------------------------------------------------*/
/**
 *Calculate ulGMCRestoreResetTime
 *
 *
 * @param[in] IntegratedInfoTable Integrated info table pointer
 * @param[in] Gfx                 Gfx configuration info
 * @param[in] PpFuseArray         Fuse array pointer
 * @retval    AGESA_STATUS
 */
AGESA_STATUS
GfxCalculateRestoreResetTimeML (
  IN       ATOM_INTEGRATED_SYSTEM_INFO_V1_8 *IntegratedInfoTable,
  IN       GFX_PLATFORM_CONFIG              *Gfx,
  IN       PP_FUSE_ARRAY_V2                  *PpFuseArray
  )
{
  UINT8      MaxDid;
  ULONG      FreqSclk;
  UINTN      Index;
  UINT32     TSclk;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxCalculateRestoreResetTimeML Enter\n");
  // Find FreqSclk = MIN of frequencies SclkDpmDid (0 to 4) and SclkThermDid
  // First find the highest Did
  MaxDid = PpFuseArray->SclkThermDid;
  for (Index = 0; Index < 4; Index++) {
    // Compare with SclkDpmDid[x] - These are stored in:
    // IntegratedInfoTable-> sDISPCLK_Voltage[Index].ulMaximumSupportedCLK
    MaxDid = MAX (MaxDid, PpFuseArray->SclkDid[Index]);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "MaxDid = %d\n", MaxDid);
  FreqSclk = GfxFmCalculateClock (MaxDid, GnbLibGetHeader (Gfx));
  // FreqSclk is in 10KHz units - need calculations in nS
  // For accuracy, do calculations in .01nS, then convert at the end
  TSclk = (100 * (1000000000 / 10000)) / FreqSclk;

  IntegratedInfoTable->ulGMCRestoreResetTime = ((TSclk * 662) + 99) / 100;
  IDS_HDT_CONSOLE (GNB_TRACE, "ulGMCRestoreResetTime = %d\n", IntegratedInfoTable->ulGMCRestoreResetTime);
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxCalculateRestoreResetTimeTN Exit\n");

  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 *Init TN HTC Data
 *
 *
 * @param[in] IntegratedInfoTable Integrated info table pointer
 * @param[in] Gfx                 Gfx configuration info
 */

VOID
GfxFillHtcDataML (
  IN OUT   ATOM_INTEGRATED_SYSTEM_INFO_V1_8 *IntegratedInfoTable,
  IN       GFX_PLATFORM_CONFIG              *Gfx
  )
{
  D18F3x64_STRUCT  D18F3x64;
  GNB_HANDLE        *GnbHandle;

  GnbHandle = GnbGetHandle (GnbLibGetHeader (Gfx));
  ASSERT (GnbHandle != NULL);

  GnbRegisterReadML (
    GnbHandle,
    D18F3x64_TYPE,
    D18F3x64_ADDRESS,
    &D18F3x64.Value,
    0,
    GnbLibGetHeader (Gfx)
    );

  if (D18F3x64.Field.HtcEn == 1) {
    IntegratedInfoTable->ucHtcTmpLmt = (UCHAR) (D18F3x64.Field.HtcTmpLmt / 2 + 52);
    IntegratedInfoTable->ucHtcHystLmt = (UCHAR) (D18F3x64.Field.HtcHystLmt / 2);
  } else {
    IntegratedInfoTable->ucHtcTmpLmt = 0;
    IntegratedInfoTable->ucHtcHystLmt = 0;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 *Init Sclk <-> VID table
 *
 *
 * @param[in] PpFuseArray         Fuse array pointer
 * @param[in] IntegratedInfoTable Integrated info table pointer
 * @param[in] Gfx                 Gfx configuration info
 */

VOID
GfxIntInfoTableInitSclkTableML (
  IN       PP_FUSE_ARRAY_V2                  *PpFuseArray,
  IN       ATOM_INTEGRATED_SYSTEM_INFO_V1_8 *IntegratedInfoTable,
  IN       GFX_PLATFORM_CONFIG              *Gfx
  )
{
  UINT8                       Index;
  UINTN                       AvailSclkIndex;
  ATOM_AVAILABLE_SCLK_LIST    *AvailSclkList;
  BOOLEAN                     Sorting;
  AvailSclkList = &IntegratedInfoTable->sAvail_SCLK[0];

  AvailSclkIndex = 0;
  for (Index = 0; Index < MAX_NUM_OF_FUSED_DPM_STATES; Index++) {
    if (PpFuseArray->SclkDid[Index] != 0) {
      AvailSclkList[AvailSclkIndex].ulSupportedSCLK = GfxFmCalculateClock (PpFuseArray->SclkDid[Index], GnbLibGetHeader (Gfx));
      AvailSclkList[AvailSclkIndex].usVoltageIndex = Index;
      AvailSclkList[AvailSclkIndex].usVoltageID = PpFuseArray->VddNbVid[Index];
      AvailSclkIndex++;
    }
  }
  //Sort by VoltageIndex & ulSupportedSCLK
  if (AvailSclkIndex > 1) {
    do {
      Sorting = FALSE;
      for (Index = 0; Index < (AvailSclkIndex - 1); Index++) {
        ATOM_AVAILABLE_SCLK_LIST  Temp;
        BOOLEAN                   Exchange;
        Exchange = FALSE;
        if (AvailSclkList[Index].usVoltageIndex > AvailSclkList[Index + 1].usVoltageIndex) {
          Exchange = TRUE;
        }
        if ((AvailSclkList[Index].usVoltageIndex == AvailSclkList[Index + 1].usVoltageIndex) &&
            (AvailSclkList[Index].ulSupportedSCLK > AvailSclkList[Index + 1].ulSupportedSCLK)) {
          Exchange = TRUE;
        }
        if (Exchange) {
          Sorting = TRUE;
          LibAmdMemCopy (&Temp, &AvailSclkList[Index], sizeof (ATOM_AVAILABLE_SCLK_LIST), GnbLibGetHeader (Gfx));
          LibAmdMemCopy (&AvailSclkList[Index], &AvailSclkList[Index + 1], sizeof (ATOM_AVAILABLE_SCLK_LIST), GnbLibGetHeader (Gfx));
          LibAmdMemCopy (&AvailSclkList[Index + 1], &Temp, sizeof (ATOM_AVAILABLE_SCLK_LIST), GnbLibGetHeader (Gfx));
        }
      }
    } while (Sorting);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Build integrated info table
 *
 *
 *
 * @param[in] Gfx             Gfx configuration info
 * @retval    AGESA_STATUS
 */
AGESA_STATUS
STATIC
GfxIntInfoTableInitML (
  IN      GFX_PLATFORM_CONFIG     *Gfx
  )
{
  AGESA_STATUS                    Status;
  AGESA_STATUS                    AgesaStatus;
  ATOM_FUSION_SYSTEM_INFO_V3      SystemInfoTableV3;
  PP_FUSE_ARRAY_V2                *PpFuseArray;
  ATOM_PPLIB_POWERPLAYTABLE4      *PpTable;
  D18F5x170_STRUCT                D18F5x170;
  GNB_HANDLE                      *GnbHandle;
  GNB_BUILD_OPTIONS_ML            *GnbBuildOptionData;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntInfoTableInitML Enter\n");

  AgesaStatus = AGESA_SUCCESS;
  GnbHandle = GnbGetHandle (GnbLibGetHeader (Gfx));
  ASSERT (GnbHandle != NULL);
  PpFuseArray = GnbLocateHeapBuffer (AMD_PP_FUSE_TABLE_HANDLE, GnbLibGetHeader (Gfx));
  if (PpFuseArray != NULL) {
    LibAmdMemFill (&SystemInfoTableV3, 0x00, sizeof (ATOM_FUSION_SYSTEM_INFO_V3), GnbLibGetHeader (Gfx));

    // Use common initialization first
    Status = GfxIntInfoTableInitV3 (Gfx, &SystemInfoTableV3, PpFuseArray);
    // Complete table with ML-specific fields

    // Build PP table
    PpTable = (ATOM_PPLIB_POWERPLAYTABLE4*) &SystemInfoTableV3.ulPowerplayTable;
    Status = GfxPwrPlayBuildTable (PpTable,  Gfx);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);

    IDS_HDT_CONSOLE (GNB_TRACE, "ML Pplay done\n");

    // Assign usFormatID to 0x0013 to represent Kabini
    PpTable->usFormatID = 0x13;

    // Build Sclk info table
    GfxIntInfoTableInitSclkTableML (PpFuseArray, &SystemInfoTableV3.sIntegratedSysInfo, Gfx);

    // Fill in Nb P-state MemclkFreq Data
    GfxFillNbPstateMemclkFreqML (&SystemInfoTableV3.sIntegratedSysInfo, Gfx);
    // Fill in HTC Data
    GfxFillHtcDataML (&SystemInfoTableV3.sIntegratedSysInfo, Gfx);

    // Family specific data update
    // Determine ulGMCRestoreResetTime
    Status = GfxCalculateRestoreResetTimeML (&SystemInfoTableV3.sIntegratedSysInfo, Gfx, PpFuseArray);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);

    //GfxFmIntegratedInfoTableInit (&SystemInfoV1Table.sIntegratedSysInfo, Gfx);
    SystemInfoTableV3.sIntegratedSysInfo.ulDDR_DLL_PowerUpTime = 4940;
    SystemInfoTableV3.sIntegratedSysInfo.ulDDR_PLL_PowerUpTime = 2000;

    SystemInfoTableV3.sIntegratedSysInfo.ulGPUCapInfo =
      GPUCAPINFO_TMDS_HDMI_USE_SINGLE_PLL_MODE |
      GPUCAPINFO_DP_USE_SINGLE_PLL_MODE |
      GPUCAPINFO_DFS_BYPASS_DISABLE;

    GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, GnbLibGetHeader (Gfx));
    if (GnbBuildOptionData != NULL) {
      if ((GnbBuildOptionData->CfgSMUServiceEnablementBitMap & ALLCLK_MON_MASK) != 0) {
        SystemInfoTableV3.sIntegratedSysInfo.ulGPUCapInfo |= GPUCAPINFO_DFS_BYPASS_ENABLE;
      }
      if (GnbBuildOptionData->CfgBatteryBoostEn == TRUE) {
        SystemInfoTableV3.sIntegratedSysInfo.ulGPUCapInfo |= GPUCAPINFO_BATTERY_BOOST_ENABLE;
      }
    }
    // Check if NbPstate enable
    GnbRegisterReadML (GnbHandle, TYPE_D18F5, D18F5x170_ADDRESS, &D18F5x170.Value, 0, GnbLibGetHeader (Gfx));
    if ((D18F5x170.Field.SwNbPstateLoDis != 1) && (D18F5x170.Field.NbPstateMaxVal != 0)) {
      // If NbPstate enable, then enable NBDPM for driver
      SystemInfoTableV3.sIntegratedSysInfo.ulSystemConfig |= BIT3;
    }

    IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_INTEGRATED_TABLE_CONFIG, &SystemInfoTableV3.sIntegratedSysInfo, GnbLibGetHeader (Gfx));
    //Copy integrated info table to Frame Buffer. (Do not use LibAmdMemCopy, routine not guaranteed access to above 4G memory in 32 bit mode.)
    GfxIntInfoTablePostToFbV3 (&SystemInfoTableV3, Gfx);

    IDS_HDT_CONSOLE (GNB_TRACE, "  sIntegratedSysInfo ulGPUCapInfo - %08x\n", SystemInfoTableV3.sIntegratedSysInfo.ulGPUCapInfo);

    GNB_DEBUG_CODE (
      GfxIntInfoTableDebugDumpV3 (&SystemInfoTableV3, Gfx);
    );
  } else {
    IDS_HDT_CONSOLE(GNB_TRACE, "  ERROR! Cannot locate fuse table\n");
    Status = AGESA_ERROR;
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntInfoTableInitML Exit [0x%x]\n", Status);
  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Build integrated info table
 *  GMC FB access requred
 *
 *
 * @param[in]   StdHeader     Standard configuration header
 * @retval      AGESA_STATUS
 */
AGESA_STATUS
GfxIntInfoTableInterfaceML (
  IN      AMD_CONFIG_PARAMS       *StdHeader
  )
{
  AGESA_STATUS                    AgesaStatus;
  AGESA_STATUS                    Status;
  GFX_PLATFORM_CONFIG             *Gfx;
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntInfoTableInterfaceML Enter\n");
  AgesaStatus = AGESA_SUCCESS;
  if (GfxLibIsControllerPresent (StdHeader)) {
    Status = GfxLocateConfigData (StdHeader, &Gfx);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    if (Status != AGESA_FATAL) {
      Status = GfxIntInfoTableInitML (Gfx);
      AGESA_STATUS_UPDATE (Status, AgesaStatus);
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntInfoTableInterfaceML Exit[0x%x]\n", AgesaStatus);
  return AgesaStatus;
}
