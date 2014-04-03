/* $NoKeywords:$ */
/**
 * @file
 *
 * GNB mid post initialization.
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
#include  "Gnb.h"
#include  "GnbLib.h"
#include  "GnbPcieConfig.h"
#include  "GnbFuseTable.h"
#include  "heapManager.h"
#include  "GnbGfxFamServices.h"
#include  "GnbCommonLib.h"
#include  "GnbNbInitLibV1.h"
#include  "GnbNbInitLibV4.h"
#include  "GnbNbInitLibV5.h"
#include  "GnbGfxInitLibV1.h"
#include  "GnbSmuInitLibV7.h"
#include  "GnbTable.h"
#include  "GnbRegisterAccML.h"
#include  "GnbRegistersML.h"
#include  "GnbFamServices.h"
#include  "OptionGnb.h"
#include  "GnbSmuBiosTableML.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITML_GNBMIDINITML_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern GNB_BUILD_OPTIONS_ML   GnbBuildOptionsML;
extern GNB_TABLE ROMDATA      GnbMidInitTableML[];

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
STATIC
GnbIommuMidInitPcieCoreUsageML (
  IN       GNB_HANDLE            *GnbHandle
  )
{
  PCIe_ENGINE_CONFIG    *EngineList;
  D0F2xF4_x57_STRUCT    D0F2xF4_x57;
  UINT32                AllCoreDis;
  AMD_CONFIG_PARAMS     *StdHeader;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitPcieCoreUsage Enter\n");
  //
  // Initial asusmption that no wrapper/cores are booting used
  //
  AllCoreDis = 1;
  EngineList = PcieConfigGetChildEngine (GnbHandle);
  while (EngineList != NULL) {
    if (PcieConfigIsPcieEngine (EngineList)) {
      if (PcieConfigCheckPortStatus (EngineList, INIT_STATUS_PCIE_TRAINING_SUCCESS) ||
        ((EngineList->Type.Port.PortData.LinkHotplug != HotplugDisabled) && (EngineList->Type.Port.PortData.LinkHotplug != HotplugInboard))) {
          //Wrap ID correspoing to bit in D0F2xF4_x57
        AllCoreDis = 0;
        break;
      }
    }
    EngineList = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor (EngineList, DESCRIPTOR_TERMINATE_GNB);
  }

  if (AllCoreDis == 1) {
    StdHeader = PcieConfigGetStdHeader (GnbHandle);
    GnbRegisterReadML (GnbHandle, D0F2xF4_x57_TYPE, D0F2xF4_x57_ADDRESS, &D0F2xF4_x57.Value, 0, StdHeader);
    D0F2xF4_x57.Value |= BIT0;
    GnbRegisterWriteML (GnbHandle, D0F2xF4_x57_TYPE, D0F2xF4_x57_ADDRESS, &D0F2xF4_x57.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitPcieCoreUsage Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to for each PCIe port
 *
 *
 *
 *
 * @param[in]       CoreId        CoreId
 *
 */

UINT8
GnbIommuCoreIdToL1SelML (
  IN       UINT8                 CoreId
  )
{
  UINT8 Sel;
  Sel = 0;
  switch (CoreId) {
  case 0x0:
    Sel = L1_SEL_PPD;
    break;
  case 0x1:
    Sel = L1_SEL_SBG;
    break;
  case 0x2:
    Sel = L1_SEL_BIF;
    break;
  default:
    ASSERT (FALSE);
  }
  return Sel;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to for each PCIe port
 *
 *
 *
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 */

VOID
STATIC
GnbIommuMidInitOnPortCallbackML (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  GNB_TOPOLOGY_INFO       TopologyInfo;
  GNB_HANDLE              *GnbHandle;
  D0F2xFC_x07_L1_STRUCT   D0F2xFC_x07_L1;
  D0F2xFC_x0D_L1_STRUCT   D0F2xFC_x0D_L1;

  TopologyInfo.PhantomFunction = FALSE;
  TopologyInfo.PcieToPciexBridge = FALSE;
  GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon (Engine);
  if (Engine->Type.Port.PortData.LinkHotplug != HotplugDisabled) {
    TopologyInfo.PhantomFunction = TRUE;
    TopologyInfo.PcieToPciexBridge = TRUE;
  } else {
    GnbGetTopologyInfoV4 (Engine->Type.Port.Address, Engine->Type.Port.Address, &TopologyInfo, GnbLibGetHeader (Pcie));
  }
  GnbRegisterReadML (
    GnbHandle,
    D0F2xFC_x07_L1_TYPE,
    D0F2xFC_x07_L1_ADDRESS (GnbIommuCoreIdToL1SelML (Engine->Type.Port.CoreId)),
    &D0F2xFC_x07_L1.Value,
    0,
    GnbLibGetHeader (Pcie)
    );
  if (TopologyInfo.PhantomFunction) {
    D0F2xFC_x07_L1.Value &= ~BIT0;
  } else {
    D0F2xFC_x07_L1.Value |= BIT0;
  }
  GnbRegisterWriteML (
    GnbHandle,
    D0F2xFC_x07_L1_TYPE,
    D0F2xFC_x07_L1_ADDRESS (GnbIommuCoreIdToL1SelML (Engine->Type.Port.CoreId)),
    &D0F2xFC_x07_L1.Value,
    GNB_REG_ACC_FLAG_S3SAVE,
    GnbLibGetHeader (Pcie)
    );
  if (TopologyInfo.PcieToPciexBridge) {
    GnbRegisterReadML (
      GnbHandle,
      D0F2xFC_x0D_L1_TYPE,
      D0F2xFC_x0D_L1_ADDRESS (GnbIommuCoreIdToL1SelML (Engine->Type.Port.CoreId)),
      &D0F2xFC_x0D_L1.Value,
      0,
      GnbLibGetHeader (Pcie)
      );
    D0F2xFC_x0D_L1.Field.VOQPortBits = 0x7;
    GnbRegisterWriteML (
      GnbHandle,
      D0F2xFC_x0D_L1_TYPE,
      D0F2xFC_x0D_L1_ADDRESS (GnbIommuCoreIdToL1SelML (Engine->Type.Port.CoreId)),
      &D0F2xFC_x0D_L1.Value,
      GNB_REG_ACC_FLAG_S3SAVE,
      GnbLibGetHeader (Pcie)
      );
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * IOMMU Mid Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbIommuMidInitML (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS          Status;
  PCIe_PLATFORM_CONFIG  *Pcie;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitML Enter\n");
  Status = PcieLocateConfigurationData (StdHeader, &Pcie);
  if (Status == AGESA_SUCCESS) {
    PcieConfigRunProcForAllEngines (
      DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
      GnbIommuMidInitOnPortCallbackML,
      NULL,
      Pcie
      );
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitML Exit [0x%x]\n", Status);
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Gnb Mid Post Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbMidInterfaceML (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS            Status;
  UINT32                  Property;
  AGESA_STATUS            AgesaStatus;
  GNB_HANDLE              *GnbHandle;
  PCI_ADDR                GnbPciAddress;
  DEV_OBJECT              DevObject;
  GNB_BUILD_OPTIONS_ML    *GnbBuildOptionData;
  UINT32                  EnablementMask;
  D18F5x170_STRUCT        D18F5x170;
  PP_FUSE_ARRAY_V2        *PpFuseArray;

  AgesaStatus = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbMidInterfaceML Enter\n");

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  ASSERT (GnbBuildOptionData != NULL);
  Status = GnbIommuMidInitML (StdHeader);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  Property = TABLE_PROPERTY_DEFAULT;
  Property |= GfxLibIsControllerPresent (StdHeader) ? 0 : TABLE_PROPERTY_IGFX_DISABLED;
  Property |= GnbBuildOptionsML.GnbCommonOptions.LclkDeepSleepEn ? TABLE_PROPERTY_LCLK_DEEP_SLEEP : 0;
  Property |= GnbBuildOptionsML.GnbCommonOptions.CfgOrbClockGatingEnable ? TABLE_PROPERTY_ORB_CLK_GATING : 0;
  Property |= GnbBuildOptionsML.GnbCommonOptions.CfgIocLclkClockGatingEnable ? TABLE_PROPERTY_IOC_LCLK_CLOCK_GATING : 0;
  Property |= GnbBuildOptionsML.CfgSpgClockGatingEnable ? TABLE_PROPERTY_SPG_CLOCK_GATING : 0;

  GnbHandle = GnbGetHandle (StdHeader);
  if (GnbFmCheckIommuPresent (GnbHandle, StdHeader)) {
    Status = GnbEnableIommuMmioV4 (GnbHandle, StdHeader);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    GnbIommuMidInitPcieCoreUsageML (GnbHandle);
  }

  IDS_OPTION_HOOK (IDS_GNB_PROPERTY, &Property, StdHeader);
  Status = GnbProcessTable (
             GnbHandle,
             GnbMidInitTableML,
             Property,
             GNB_TABLE_FLAGS_FORCE_S3_SAVE,
             StdHeader
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  GnbPciAddress = GnbGetHostPciAddress (GnbHandle);
  GnbLibPciIndirectWriteField (
    GnbPciAddress.AddressValue | D0F0xF8_ADDRESS,
    D0F0xFC_x0F_ADDRESS,
    D0F0xFC_x0F_GBIFExtIntrGrp_OFFSET,
    D0F0xFC_x0F_GBIFExtIntrGrp_WIDTH,
    5,
    AccessS3SaveWidth32,
    StdHeader
    );

  GnbLibPciIndirectWriteField (
    GnbPciAddress.AddressValue | D0F0xF8_ADDRESS,
    D0F0xFC_x0F_ADDRESS,
    D0F0xFC_x0F_GBIFExtIntrSwz_OFFSET,
    D0F0xFC_x0F_GBIFExtIntrSwz_WIDTH,
    0,
    AccessS3SaveWidth32,
    StdHeader
    );

  GnbWriteSmuBiosTableML (StdHeader);

  EnablementMask = GnbBuildOptionData->CfgSMUServiceEnablementBitMap;
  // Check if NbPstate disable
  GnbRegisterReadML (GnbHandle, TYPE_D18F5, D18F5x170_ADDRESS, &D18F5x170.Value, 0, StdHeader);
  if ((D18F5x170.Field.SwNbPstateLoDis == 1) || (D18F5x170.Field.NbPstateMaxVal == 0)) {
    // If NbPstate disabled, then disable NBDPM for SMU
    EnablementMask &= ~NB_DPM_MASK;
  }
  // Check if BAPM is supported.  If not, disable feature in SMU
  PpFuseArray = GnbLocateHeapBuffer (AMD_PP_FUSE_TABLE_HANDLE, StdHeader);
  if (PpFuseArray != NULL) {
    if (PpFuseArray->BapmDisable == 1) {
      EnablementMask &= ~(BAPM_MASK | POWER_ESTIMATOR_MASK | TDC_LIMITING_MASK |
                          PKG_PWR_LIMITING_MASK | LOCAL_EAPM_MASK | STAPM_MASK | BBB_MASK);
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "BAPM %s\n", ((EnablementMask & BAPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "POWER_ESTIMATOR %s\n", ((EnablementMask & POWER_ESTIMATOR_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "THERMAL_CONTROLLER %s\n", ((EnablementMask & THERMAL_CONTROLLER_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "TDC_LIMITING %s\n", ((EnablementMask & TDC_LIMITING_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "PKG_PWR_LIMITING %s\n", ((EnablementMask & PKG_PWR_LIMITING_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "LCLK_DPM %s\n", ((EnablementMask & LCLK_DPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "VOLTAGE_CONTROLLER %s\n", ((EnablementMask & VOLTAGE_CONTROLLER_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "GLOBAL EAPM %s\n", ((EnablementMask & GLOBAL_EAPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "VPC_ACCUMULATOR %s\n", ((EnablementMask & VPC_ACCUMULATOR_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "IBS %s\n", ((EnablementMask & IBS_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "PSTATE_ARBITER %s\n", ((EnablementMask & PSTATE_ARBITER_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "NB_DPM %s\n", ((EnablementMask & NB_DPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "LPMX %s\n", ((EnablementMask & LPMX_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "WHISPER_MODE %s\n", ((EnablementMask & WHISPER_MODE_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "GFX_CU_PG %s\n", ((EnablementMask & GFX_CU_PG_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "CONN_STDBY %s\n", ((EnablementMask & CONN_STDBY_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "MULTI_VID %s\n", ((EnablementMask & MULTI_VID_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "CHTC %s\n", ((EnablementMask & CHTC_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "ALLCLK_MON_MASK %s\n", ((EnablementMask & ALLCLK_MON_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "SCLK_DPM_MASK %s\n", ((EnablementMask & SCLK_DPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "PRE_FETCH_DATA_MASK %s\n", ((EnablementMask & PRE_FETCH_DATA_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "LCLK DEEP SLEEP %s\n", ((EnablementMask & LCLK_DEEP_SLEEP_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "STAPM_MASK %s\n", ((EnablementMask & STAPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "PTC_MASK %s\n", ((EnablementMask & PTC_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "BBB_MASK %s\n", ((EnablementMask & BBB_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "DCTDP_MASK %s\n", ((EnablementMask & DCTDP_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "APU_PWR_CTRL_MASK %s\n", ((EnablementMask & APU_PWR_CTRL_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "LOCAL_EAPM_MASK %s\n", ((EnablementMask & LOCAL_EAPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "CORE_RRT_MASK %s\n", ((EnablementMask & CORE_RRT_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "GFX_RRT_MASK %s\n", ((EnablementMask & GFX_RRT_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "STAPM_TSKIN_CALC_MASK %s\n", ((EnablementMask & STAPM_TSKIN_CALC_MASK) != 0) ? "Enable" : "Disable");

  // Voltage controller has been turned on in InitEarly
  EnablementMask &= (~VOLTAGE_CONTROLLER_MASK);

  DevObject.DevPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
  DevObject.GnbHandle = GnbHandle;
  DevObject.StdHeader = StdHeader;

  if (EnablementMask != 0) {
    GnbSmuServiceRequestV7 (
        &DevObject,
        SMC_MSG_ENABLE_ALL_SMU_FEATURES,
        EnablementMask,
        GNB_REG_ACC_FLAG_S3SAVE
        );
  }

  if (GnbBuildOptionData->CfgPspDpmEn == TRUE) {
    GnbSmuServiceRequestV7 (
        &DevObject,
        SMC_MSG_PSPCLKDPM_ENABLE,
        NULL,
        GNB_REG_ACC_FLAG_S3SAVE
        );
  } else {
    GnbSmuServiceRequestV7 (
        &DevObject,
        SMC_MSG_PSPCLKDPM_DISABLE,
        NULL,
        GNB_REG_ACC_FLAG_S3SAVE
        );
  }

  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbMidInterfaceML Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}
