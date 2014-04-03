/* $NoKeywords:$ */
/**
 * @file
 *
 * GNB early post initialization.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 283510 $   @e \$Date: 2014-01-25 19:47:29 -0600 (Sat, 25 Jan 2014) $
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
#include  "OptionGnb.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieConfig.h"
#include  "GnbTable.h"
#include  "GnbFamServices.h"
#include  "GnbSmuInitLibV7.h"
#include  "GnbRegisterAccML.h"
#include  "GnbRegistersML.h"
#include  "cpuF16PowerMgmt.h"
#include  "cpuFamilyTranslation.h"
#include  "GnbSmuScsML.h"
#include  "GnbPspML.h"
#include  "GnbSmuBiosTableML.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITML_GNBEARLYINITML_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern GNB_TABLE ROMDATA    GnbEarlierInitTableBeforeSmuML [];
extern GNB_TABLE ROMDATA    GnbEarlyInitTableML [];
extern GNB_BUILD_OPTIONS_ML GnbBuildOptionsML;
extern BUILD_OPT_CFG        UserOptions;

#define CFG_DISP_PHY_TDP_LIMIT                      0
#define CFG_FCH_PWR_CREDIT                          0


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------*/
/**
 * Request VDDB Pmin
 *
 *
 *
 * @param[in]  GnbHandle  GNB_HANDLE
 * @param[in]  StdHeader  Standard configuration header
 */

VOID
GnbRequestVddNbPminML (
  IN      GNB_HANDLE                      *GnbHandle,
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  D18F5x170_STRUCT        D18F5x170;
  D18F5x160_STRUCT        D18F5x160;
  UINT32                  Millivolt;
  UINT32                  NbVid;
  DEV_OBJECT              DevObject;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbRequestVddNbPminML Enter\n");
  // 1) Read F5x170[NbPstateMaxVal] to find NB Pmin.
  GnbRegisterReadML (GnbHandle, D18F5x170_TYPE, D18F5x170_ADDRESS, &D18F5x170, 0, StdHeader);
  // 2) Determine voltage associated with NB Pmin.  Read F5x(160+4*NbPstateMaxVal)[NbVid].  (For example, NBP0 is F5x160, NBP1 is F5x164, etc)
  GnbRegisterReadML (GnbHandle, D18F5x160_TYPE, (D18F5x160_ADDRESS + (4 * D18F5x170.Field.NbPstateMaxVal)), &D18F5x160, 0, StdHeader);
  NbVid = (D18F5x160.Field.NbVid_7_ << 7) | (D18F5x160.Field.NbVid_6_0_);
  // 3) Make voltage request (via BIOSSMC_MSG_VDDNB_REQUEST) for voltage determined by Step 2.  Note that message requires encoding in voltage, not VID.
  //    Use the following equation to decode SVI2 VIDs:
  //      Voltage = 1.55 - (0.00625*VID)
  //    Furthermore, VDDNB requests to SMU should be encoded in 0.25mV steps.
  //    Therefore, voltage should be translated into mV and then multiplied by 4, to be encoded as 0.25mV steps.
  //    Or... ( 1.55 * 1000 * 4) - (.00625 * 1000 * 4) * VID) = (1550 * 4) - (25 * VID)
  Millivolt = (1550 * 4) - (25 * NbVid);

  IDS_HDT_CONSOLE (GNB_TRACE, "  Set Voltage for NbPstateMaxVal = 0x%x, Vid code 0x%x = %d mV\n", D18F5x170.Field.NbPstateMaxVal, NbVid, Millivolt);
  DevObject.StdHeader = StdHeader;
  DevObject.GnbHandle = GnbHandle;
  DevObject.DevPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
  GnbSmuServiceRequestV7 (
    &DevObject,
    SMC_MSG_ENABLE_ALL_SMU_FEATURES,
    VOLTAGE_CONTROLLER_MASK,
    0
    );
  GnbSmuServiceRequestV7 (
    &DevObject,
    SMC_MSG_VDDNB_REQUEST,
    Millivolt,
    0
    );
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbRequestVddNbPminML Exit\n");
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize GFX straps.
 *
 *
 * @param[in] StdHeader  Standard configuration header
 */
VOID
GfxStrapsEarlyInitML (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AMD_EARLY_PARAMS            *EarlyParamsPtr;
  D0F0xD4_x013014AC_STRUCT    D0F0xD4_x13014AC;
  D0F0xD4_x013014B6_STRUCT    D0F0xD4_x13014B6;
  D0F0xD4_x013014B8_STRUCT    D0F0xD4_x013014B8;
  D0F0xD4_x013014BA_STRUCT    D0F0xD4_x013014BA;
  D0F0xD4_x013014BE_STRUCT    D0F0xD4_x13014BE;
  GNB_HANDLE                 *GnbHandle;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxStrapsEarlyInitML Enter\n");

  EarlyParamsPtr = (AMD_EARLY_PARAMS *) StdHeader;
  GnbHandle = GnbGetHandle (StdHeader);

  GnbRegisterReadML (GnbHandle, D0F0xD4_x013014AC_TYPE, D0F0xD4_x013014AC_ADDRESS, &D0F0xD4_x13014AC.Value, 0, StdHeader);
  GnbRegisterReadML (GnbHandle, D0F0xD4_x013014B6_TYPE, D0F0xD4_x013014B6_ADDRESS, &D0F0xD4_x13014B6.Value, 0, StdHeader);
  GnbRegisterReadML (GnbHandle, D0F0xD4_x013014B8_TYPE, D0F0xD4_x013014B8_ADDRESS, &D0F0xD4_x013014B8.Value, 0, StdHeader);
  GnbRegisterReadML (GnbHandle, D0F0xD4_x013014BA_TYPE, D0F0xD4_x013014BA_ADDRESS, &D0F0xD4_x013014BA.Value, 0, StdHeader);
  GnbRegisterReadML (GnbHandle, D0F0xD4_x013014BE_TYPE, D0F0xD4_x013014BE_ADDRESS, &D0F0xD4_x13014BE.Value, 0, StdHeader);

  D0F0xD4_x13014B6.Field.StrapBifAzLegacyDeviceTypeDis = 0x0;
  D0F0xD4_x13014B6.Field.StrapBifF0LegacyDeviceTypeDis = 0x0;

  D0F0xD4_x13014AC.Field.StrapBifAudioEnPin = EarlyParamsPtr->PlatformConfig.GnbHdAudio;
  D0F0xD4_x13014B6.Field.StrapBifAudioEn = EarlyParamsPtr->PlatformConfig.GnbHdAudio;

  // Enable PCI Vendor Specific Capabilities
  D0F0xD4_x13014BE.Field.StrapBifSymalignHwDebug = 1;

  D0F0xD4_x013014B8.Field.STRAP_BIF_F0_ATS_EN = 1;
  D0F0xD4_x013014B8.Field.STRAP_BIF_MAX_PASID_WIDTH = 0x10;
  D0F0xD4_x013014B8.Field.STRAP_BIF_PASID_PREFIX_SUPPORTED = 1;
  D0F0xD4_x013014B8.Field.STRAP_BIF_EXTENDED_FMT_SUPPORTED_A = 1;
  D0F0xD4_x013014B8.Field.STRAP_BIF_E2E_PREFIX_EN_A = 1;

  D0F0xD4_x013014BA.Field.STRAP_BIF_F0_PAGE_REQ_EN = 1;
  D0F0xD4_x013014BA.Field.STRAP_BIF_F0_PASID_EN = 1;
  D0F0xD4_x013014BA.Field.STRAP_BIF_PASID_EXE_PERMISSION_SUP = 1;
  D0F0xD4_x013014BA.Field.STRAP_BIF_PASID_PRIV_MODE_SUP = 1;
  D0F0xD4_x013014BA.Field.STRAP_BIF_PASID_GLOBAL_INVALID_SUP = 1;

  GnbRegisterWriteML (GnbHandle, D0F0xD4_x013014AC_TYPE, D0F0xD4_x013014AC_ADDRESS, &D0F0xD4_x13014AC.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
  GnbRegisterWriteML (GnbHandle, D0F0xD4_x013014B6_TYPE, D0F0xD4_x013014B6_ADDRESS, &D0F0xD4_x13014B6.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
  GnbRegisterWriteML (GnbHandle, D0F0xD4_x013014B8_TYPE, D0F0xD4_x013014B8_ADDRESS, &D0F0xD4_x013014B8.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
  GnbRegisterWriteML (GnbHandle, D0F0xD4_x013014BA_TYPE, D0F0xD4_x013014BA_ADDRESS, &D0F0xD4_x013014BA.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
  GnbRegisterWriteML (GnbHandle, D0F0xD4_x013014BE_TYPE, D0F0xD4_x013014BE_ADDRESS, &D0F0xD4_x13014BE.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxStrapsEarlyInitML Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Enable Gfx gBIF
 *
 *
 * @param[in] StdHeader  Standard configuration header
 * @retval    AGESA_STATUS
 */

AGESA_STATUS
GfxGBifEnableML (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  D0F0xD4_x01301486_STRUCT    D0F0xD4_x1301486;
  D0F0xD4_x01091518_STRUCT    D0F0xD4_x1091518;
  D0F0xBC_xC00C0000_STRUCT    D0F0xBC_xC00C0000;
  GNB_HANDLE                  *GnbHandle;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGBifEnableML Enter\n");
  GnbHandle = GnbGetHandle (StdHeader);

  GnbRegisterReadML (GnbHandle, D0F0xBC_xC00C0000_TYPE, D0F0xBC_xC00C0000_ADDRESS, &D0F0xBC_xC00C0000.Value, 0, StdHeader);
  if (D0F0xBC_xC00C0000.Field.GPU_DIS != 1) {
    GfxStrapsEarlyInitML (StdHeader);
  }

  GnbRegisterReadML (GnbHandle, D0F0xD4_x01301486_TYPE, D0F0xD4_x01301486_ADDRESS, &D0F0xD4_x1301486.Value, 0, StdHeader);
  GnbRegisterReadML (GnbHandle, D0F0xD4_x01091518_TYPE, D0F0xD4_x01091518_ADDRESS, &D0F0xD4_x1091518.Value, 0, StdHeader);
  D0F0xD4_x1301486.Field.LinkTrainEn = 0x1;
  D0F0xD4_x1091518.Field.LinkTrainEn = 0x1;
  GnbRegisterWriteML (GnbHandle, D0F0xD4_x01091518_TYPE, D0F0xD4_x01091518_ADDRESS, &D0F0xD4_x1091518.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
  GnbRegisterWriteML (GnbHandle, D0F0xD4_x01301486_TYPE, D0F0xD4_x01301486_ADDRESS, &D0F0xD4_x1301486.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGBifEnableML Exit\n");

  return  AGESA_SUCCESS;
}
/*----------------------------------------------------------------------------------------*/
/**
 * GNB init at early post
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbEarlyInterfaceML (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS     Status;
  AGESA_STATUS     AgesaStatus;
  GNB_HANDLE       *GnbHandle;
  UINT32           Property;
  GNB_BUILD_OPTIONS_ML  *GnbBuildOptionData;

  AgesaStatus = AGESA_SUCCESS;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEarlyInterfaceML Enter\n");
  GnbHandle = GnbGetHandle (StdHeader);
  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  ASSERT (GnbBuildOptionData != NULL);

  Property = TABLE_PROPERTY_DEFAULT;
  Property |= UserOptions.CfgGnbSyncFloodPinAsNmi ? TABLE_PROPERTY_NMI_SYNCFLOOD : 0;

  IDS_OPTION_HOOK (IDS_GNB_PROPERTY, &Property, StdHeader);
  IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_PACKAGE_POWER_CONFIG, GnbHandle, StdHeader);

  GnbInitSmuBiosTableML (StdHeader);
  Status = GnbProcessTable (
             GnbHandle,
             GnbEarlyInitTableML,
             Property,
             0,
             StdHeader
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  if (GnbBuildOptionData->CfgUseSMUServices == TRUE) {
    GnbRequestVddNbPminML (GnbHandle, StdHeader);
  }
  Status = GfxGBifEnableML (StdHeader);
  ASSERT (Status == AGESA_SUCCESS);
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEarlyInterfaceML Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Helper function to dump gnb build option data
 *
 *
 * @param[in]  GnbBuildOptionData       gnb build option data
 */
VOID
GnbDumpBuildOptionDataML (
  IN      GNB_BUILD_OPTIONS_ML    *GnbBuildOptionData
  )
{

  IDS_HDT_CONSOLE (GNB_TRACE, "  GmcClockGating - %s\n", (GnbBuildOptionData->GnbCommonOptions.CfgGmcClockGating == TRUE) ? "Enabled" : "Disabled");
  IDS_HDT_CONSOLE (GNB_TRACE, "  IocLclkClockGating - %s\n", (GnbBuildOptionData->GnbCommonOptions.CfgIocLclkClockGatingEnable == TRUE) ? "Enabled" : "Disabled");
  IDS_HDT_CONSOLE (GNB_TRACE, "  PCIePhyIsolation - %s\n", (GnbBuildOptionData->CfgPciePhyIsolationEnable == TRUE) ? "Enabled" : "Disabled");
  IDS_HDT_CONSOLE (GNB_TRACE, "  PCIeNativeGen1PLL - %s\n", (GnbBuildOptionData->CfgNativeGen1PLL == TRUE) ? "Enabled" : "Disabled");
  IDS_HDT_CONSOLE (GNB_TRACE, "  BatteryBoostEn - %s\n", (GnbBuildOptionData->CfgBatteryBoostEn == TRUE) ? "Enabled" : "Disabled");
  IDS_HDT_CONSOLE (GNB_TRACE, "  Enablement - %08x\n", GnbBuildOptionData->CfgSMUServiceEnablementBitMap);
}

/*----------------------------------------------------------------------------------------*/
/**
 * GNB load build options data
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbLoadBuildOptionDataML (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS            Status;
  GNB_HANDLE              *GnbHandle;
  GNB_BUILD_OPTIONS_ML    *GnbBuildOptionData;
  UINT32                  D0F0xBC_xC01040D0;
  CPU_LOGICAL_ID          LogicalCpuid;
  D18F3xA0_STRUCT         D18F3xA0;

  Status = AGESA_SUCCESS;

  GnbBuildOptionData = (GNB_BUILD_OPTIONS_ML *) GnbAllocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, sizeof (GNB_BUILD_OPTIONS_ML), StdHeader);
  ASSERT (GnbBuildOptionData != NULL);

  GnbHandle = GnbGetHandle (StdHeader);
  GnbBuildOptionData->GnbCommonOptions.CfgScsSupport = GnbBuildOptionsML.GnbCommonOptions.CfgScsSupport;
  GnbBuildOptionData->GnbCommonOptions.CfgUmaSteering = GnbBuildOptionsML.GnbCommonOptions.CfgUmaSteering;
  GnbBuildOptionData->GnbCommonOptions.GmcPowerGating = GnbBuildOptionsML.GnbCommonOptions.GmcPowerGating;
  GnbBuildOptionData->GnbCommonOptions.CfgGmcClockGating = GnbBuildOptionsML.GnbCommonOptions.CfgGmcClockGating;
  GnbBuildOptionData->GnbCommonOptions.CfgOrbDynWakeEnable = GnbBuildOptionsML.GnbCommonOptions.CfgOrbDynWakeEnable;
  GnbBuildOptionData->GnbCommonOptions.CfgOrbClockGatingEnable = GnbBuildOptionsML.GnbCommonOptions.CfgOrbClockGatingEnable;
  GnbBuildOptionData->GnbCommonOptions.CfgIommuL1ClockGatingEnable = GnbBuildOptionsML.GnbCommonOptions.CfgIommuL1ClockGatingEnable;
  GnbBuildOptionData->GnbCommonOptions.CfgIommuL2ClockGatingEnable = GnbBuildOptionsML.GnbCommonOptions.CfgIommuL2ClockGatingEnable;
  GnbBuildOptionData->GnbCommonOptions.LclkDeepSleepEn = GnbBuildOptionsML.GnbCommonOptions.LclkDeepSleepEn;
  GnbBuildOptionData->GnbCommonOptions.LclkDpmEn = GnbBuildOptionsML.GnbCommonOptions.LclkDpmEn;
  GnbBuildOptionData->GnbCommonOptions.CfgIocLclkClockGatingEnable = GnbBuildOptionsML.GnbCommonOptions.CfgIocLclkClockGatingEnable;
  GnbBuildOptionData->GnbCommonOptions.CfgBapmSupport = GnbBuildOptionsML.GnbCommonOptions.CfgBapmSupport;
  GnbBuildOptionData->GnbCommonOptions.CfgDcTdpEnable = GnbBuildOptionsML.GnbCommonOptions.CfgDcTdpEnable;

  GnbBuildOptionData->CfgLhtcSupport = GnbBuildOptionsML.CfgLhtcSupport;
  GnbBuildOptionData->CfgSviRevision = GnbBuildOptionsML.CfgSviRevision;
  GnbBuildOptionData->CfgSamuPatchEnabled = GnbBuildOptionsML.CfgSamuPatchEnabled;
  GnbBuildOptionData->CfgTdcSupport = GnbBuildOptionsML.CfgTdcSupport;
  GnbBuildOptionData->CfgNativeGen1PLL = GnbBuildOptionsML.CfgNativeGen1PLL;
  GnbBuildOptionData->CfgPciePhyIsolationEnable = GnbBuildOptionsML.CfgPciePhyIsolationEnable;
  GnbBuildOptionData->CfgLinkBwNotificationEn = GnbBuildOptionsML.CfgLinkBwNotificationEn;
  GnbBuildOptionData->CfgBatteryBoostEn = UserOptions.CfgBatteryBoostEn;
  GnbBuildOptionData->CfgSpgClockGatingEnable = GnbBuildOptionsML.CfgSpgClockGatingEnable;
  GnbBuildOptionData->CfgPspDpmEn = GnbBuildOptionsML.CfgPspDpmEn;
  GnbBuildOptionData->CfgSMUServiceEnablementBitMap = GnbBuildOptionsML.CfgSMUServiceEnablementBitMap;
  GnbBuildOptionData->CfgUseSMUServices = GnbBuildOptionsML.CfgUseSMUServices;

  // Check for BAPM capability
  GnbRegisterReadML (GnbHandle, D18F3xA0_TYPE, D18F3xA0_ADDRESS, &D18F3xA0, 0, StdHeader);
  GetLogicalIdOfCurrentCore (&LogicalCpuid, StdHeader);
  if (((LogicalCpuid.Family & AMD_FAMILY_16_ML) != 0)
      && ((LogicalCpuid.Revision & AMD_F16_ML_A0) != 0)
      && (D18F3xA0.Field.ConfigId < 35)) {
    // BAPM is not supported on A0 ES1 config IDs
    // Reduce SMU service enablement to supported services
    GnbBuildOptionData->CfgSMUServiceEnablementBitMap = 0x003C0040ul;
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "SMUServiceEnablementBitMap = 0x%x\n", GnbBuildOptionData->CfgSMUServiceEnablementBitMap);

  IDS_OPTION_HOOK (IDS_GNB_LOAD_BUILD_OPTIONS, GnbBuildOptionData, StdHeader);

  // Check fuse state of BBB feature.
  GnbRegisterReadML (GnbHandle, D0F0xBC_xC01040D0_TYPE, D0F0xBC_xC01040D0_ADDRESS, &D0F0xBC_xC01040D0, 0, StdHeader);
  if ((D0F0xBC_xC01040D0 & BIT15) == 0) {
    GnbBuildOptionData->CfgBatteryBoostEn = FALSE;
  }

  GnbDumpBuildOptionDataML (GnbBuildOptionData);
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * GNB init at earlier post
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbEarlierInterfaceML (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS          Status;
  AGESA_STATUS          AgesaStatus;
  GNB_HANDLE            *GnbHandle;
  UINT32                D0F0xBC_xC00C002C;
  FIRMWARE_HEADER_V7    *SmuFwHeader;
  UINTN                 SmuFwSize;
  GNB_BUILD_OPTIONS_ML  *GnbBuildOptionData;
  UINT32                PspMBox;

  AgesaStatus = AGESA_SUCCESS;
  Status = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEarlierInterfaceML Enter\n");
  GnbHandle = GnbGetHandle (StdHeader);
  Status = GnbProcessTable (
             GnbHandle,
             GnbEarlierInitTableBeforeSmuML,
             0,
             0,
             StdHeader
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  SmuFwHeader = NULL;
  GnbRegisterReadML (GnbHandle, D0F0xBC_xC00C002C_TYPE, D0F0xBC_xC00C002C_ADDRESS, &D0F0xBC_xC00C002C, 0, StdHeader);
  D0F0xBC_xC00C002C &= (BIT1 + BIT2 + BIT3 + BIT4 + BIT5);
  IDS_HDT_CONSOLE (GNB_TRACE, "  D0F0xBC_xC00C002C = 0x%x\n", D0F0xBC_xC00C002C);

  Status = GnbLoadBuildOptionDataML (StdHeader);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  ASSERT (GnbBuildOptionData != NULL);

  //Load the SMU for non secure part & blank part
  if ((D0F0xBC_xC00C002C == 0) || (D0F0xBC_xC00C002C == BIT5 + BIT3)) {
    Status = GnbGetSmuFirmwareML ((UINTN *)&SmuFwHeader, &SmuFwSize);
    ASSERT (SmuFwHeader != NULL);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    Status = GnbSmuFirmwareLoadV7 (GnbHandle, SmuFwHeader, StdHeader);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
  } else {
    Status = GnbPspMBoxRead (&PspMBox, GnbHandle, StdHeader);
    IDS_HDT_CONSOLE (GNB_TRACE, "  PSP Mailbox Status = 0x%x\n", PspMBox);
    if (Status == AGESA_SUCCESS) {
      if ((PspMBox & MBOX_STATUS_RECOVERY) == MBOX_STATUS_RECOVERY) {
        /// SET CfgUseSMUServices flag for recovery mode
        GnbBuildOptionData->CfgUseSMUServices = FALSE;
      }
    }
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEarlierInterfaceML Exit [0x%x]\n", Status);
  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * GNB Scs interface
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */
AGESA_STATUS
GnbScsInterfaceML (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS      Status;
  AGESA_STATUS      AgesaStatus;
  GNB_HANDLE        *GnbHandle;
  UINTN             ScsDataAddress;
  UINTN             ScsDataSize;
  GNB_BUILD_OPTIONS_ML      *GnbBuildOptionData;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbScsInterfaceML Enter\n");
  AgesaStatus = AGESA_SUCCESS;
  Status = AGESA_SUCCESS;

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  ASSERT (GnbBuildOptionData != NULL);

  if ((GnbBuildOptionData->CfgUseSMUServices == TRUE) &&
      (GnbBuildOptionData->GnbCommonOptions.CfgScsSupport == TRUE)) {
    IDS_HDT_CONSOLE (GNB_TRACE, "GnbScsInterfaceML Supported\n");
    GnbHandle = GnbGetHandle (StdHeader);

    Status = GnbGetScsDataML (&ScsDataAddress, &ScsDataSize);
    ASSERT (ScsDataAddress != NULL);
    Status = GnbSmuLoadScsDataV7 (GnbHandle, (UINT8 *)ScsDataAddress, StdHeader);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);

  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbScsInterfaceML Exit [0x%x]\n", Status);
  return  AgesaStatus;
}
