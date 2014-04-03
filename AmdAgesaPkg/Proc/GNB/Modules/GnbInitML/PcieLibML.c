/* $NoKeywords:$ */
/**
 * @file
 *
 * ML specific PCIe services
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
#include  "heapManager.h"
#include  "Gnb.h"
#include  "GnbPcie.h"
#include  "GnbSbLib.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbPcieInitLibV5.h"
#include  "GnbPcieConfig.h"
#include  "GnbPcieTrainingV2.h"
#include  "GnbNbInitLibV4.h"
#include  "GnbNbInitLibV1.h"
#include  "GnbNbInitLibV5.h"
#include  "PcieComplexDataML.h"
#include  "PcieLibML.h"
#include  "GnbRegistersML.h"
#include  "GnbRegisterAccML.h"
#include  "GnbFuseTable.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITML_PCIELIBML_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


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
 * PLL powerdown
 *
 *
 * @param[in]  Wrapper     Pointer to Wrapper config descriptor
 * @param[in]  Pcie        Pointer to PICe configuration data area
 */

VOID
PciePifPllConfigureML (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  D0F0xE4_PIF_0012_STRUCT D0F0xE4_PIF_0012;

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePifPllConfigureML Enter\n");
  D0F0xE4_PIF_0012.Value = PcieRegisterRead (
                             Wrapper,
                             PIF_SPACE (Wrapper->WrapId, 0, D0F0xE4_PIF_0012_ADDRESS),
                             Pcie
                             );

  D0F0xE4_PIF_0012.Field.PllRampUpTime = 0x0;

  if (Wrapper->Features.PowerOffUnusedPlls != 0) {
    D0F0xE4_PIF_0012.Field.PllPowerStateInOff = PifPowerStateOff;
    D0F0xE4_PIF_0012.Field.PllPowerStateInTxs2 = PifPowerStateOff;
  } else {
    D0F0xE4_PIF_0012.Field.PllPowerStateInOff = PifPowerStateL0;
    D0F0xE4_PIF_0012.Field.PllPowerStateInTxs2 = PifPowerStateL0;
  }

  if (Wrapper->Features.PllOffInL1 != 0) {
    D0F0xE4_PIF_0012.Field.TxPowerStateInTxs2 = PifPowerStateLS2;
    D0F0xE4_PIF_0012.Field.RxPowerStateInRxs2 = PifPowerStateLS2;
  } else {
    D0F0xE4_PIF_0012.Field.TxPowerStateInTxs2 = PifPowerStateL0;
    D0F0xE4_PIF_0012.Field.RxPowerStateInRxs2 = PifPowerStateL0;
  }

  PcieRegisterWrite (
    Wrapper,
    PIF_SPACE (Wrapper->WrapId, 0, D0F0xE4_PIF_0012_ADDRESS),
    D0F0xE4_PIF_0012.Value,
    TRUE,
    Pcie
  );
  PcieRegisterWrite (
    Wrapper,
    PIF_SPACE (Wrapper->WrapId, 0, D0F0xE4_PIF_0012_ADDRESS + 1),
    D0F0xE4_PIF_0012.Value,
    TRUE,
    Pcie
  );
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePifPllConfigureML Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Power down unused lanes and plls
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper config descriptor
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */

VOID
PciePwrPowerDownUnusedLanesML (
  IN       PCIe_WRAPPER_CONFIG    *Wrapper,
  IN       PCIe_PLATFORM_CONFIG   *Pcie
  )
{
  UINT32              UnusedLanes;
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePwrPowerDownUnusedLanesML Enter\n");
  if (Wrapper->Features.PowerOffUnusedLanes != 0) {
    UnusedLanes = PcieUtilGetWrapperLaneBitMap (LANE_TYPE_CORE_ALL, LANE_TYPE_PCIE_CORE_ALLOC_ACTIVE, Wrapper);
    PcieTopologyLaneControlV5 (
      DisableLanes,
      UnusedLanes,
      Wrapper,
      Pcie
      );
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePwrPowerDownUnusedLanesML Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Request boot up voltage
 *
 *
 *
 * @param[in]  LinkCap             Global GEN capability
 * @param[in]  Pcie                Pointer to PCIe configuration data area
 */
VOID
PcieSetVoltageML (
  IN      PCIE_LINK_SPEED_CAP   LinkCap,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                      TargetVid;
  UINT8                      MinVidIndex;
  UINT8                      VddNbVid[5];
  UINT8                      Index;
  PP_FUSE_ARRAY_V2           *PpFuseArray;
  UINT32                     Millivolt;
  D0F0xBC_xC0104007_STRUCT   D0F0xBC_xC0104007;
  D0F0xBC_xC0104008_STRUCT   D0F0xBC_xC0104008;
  D0F0xBC_xC010407C_STRUCT   D0F0xBC_xC010407C;
  D0F0xBC_xC0107064_STRUCT   D0F0xBC_xC0107064;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieSetVoltageML Enter\n");
  PpFuseArray = GnbLocateHeapBuffer (AMD_PP_FUSE_TABLE_HANDLE, GnbLibGetHeader (Pcie));
  if (PpFuseArray == NULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "  PpFuseArray is NULL\n");
    GnbRegisterReadML (GnbGetHandle (GnbLibGetHeader (Pcie)), D0F0xBC_xC0104007_TYPE, D0F0xBC_xC0104007_ADDRESS, &D0F0xBC_xC0104007, 0, GnbLibGetHeader (Pcie));
    GnbRegisterReadML (GnbGetHandle (GnbLibGetHeader (Pcie)), D0F0xBC_xC0104008_TYPE, D0F0xBC_xC0104008_ADDRESS, &D0F0xBC_xC0104008, 0, GnbLibGetHeader (Pcie));
    GnbRegisterReadML (GnbGetHandle (GnbLibGetHeader (Pcie)), D0F0xBC_xC010407C_TYPE, D0F0xBC_xC010407C_ADDRESS, &D0F0xBC_xC010407C, 0, GnbLibGetHeader (Pcie));
    GnbRegisterReadML (GnbGetHandle (GnbLibGetHeader (Pcie)), D0F0xBC_xC0107064_TYPE, D0F0xBC_xC0107064_ADDRESS, &D0F0xBC_xC0107064, 0, GnbLibGetHeader (Pcie));
    VddNbVid[0] = (UINT8) D0F0xBC_xC0104007.Field.VddNbVid0;
    VddNbVid[1] = (UINT8) D0F0xBC_xC0104008.Field.VddNbVid1;
    VddNbVid[2] = (UINT8) D0F0xBC_xC0104008.Field.VddNbVid2;
    VddNbVid[3] = (UINT8) D0F0xBC_xC0104008.Field.VddNbVid3;
    VddNbVid[4] = (UINT8) D0F0xBC_xC010407C.Field.VddNbVid4;
    Index = (UINT8) D0F0xBC_xC0107064.Field.PcieGen2Vid;
  } else {
    IDS_HDT_CONSOLE (GNB_TRACE, "  PpFuseArray is Active\n");
    VddNbVid[0] = PpFuseArray->VddNbVid[0];
    VddNbVid[1] = PpFuseArray->VddNbVid[1];
    VddNbVid[2] = PpFuseArray->VddNbVid[2];
    VddNbVid[3] = PpFuseArray->VddNbVid[3];
    VddNbVid[4] = PpFuseArray->VddNbVid[4];
    Index = PpFuseArray->PcieGen2Vid;
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "  VddNbVid 0 - %x\n", VddNbVid[0]);
  IDS_HDT_CONSOLE (GNB_TRACE, "  VddNbVid 1 - %x\n", VddNbVid[1]);
  IDS_HDT_CONSOLE (GNB_TRACE, "  VddNbVid 2 - %x\n", VddNbVid[2]);
  IDS_HDT_CONSOLE (GNB_TRACE, "  VddNbVid 3 - %x\n", VddNbVid[3]);
  IDS_HDT_CONSOLE (GNB_TRACE, "  VddNbVid 4 - %x\n", VddNbVid[4]);
  IDS_HDT_CONSOLE (GNB_TRACE, "  PcieGen2Vid Index - %x\n", Index);

  if (LinkCap > PcieGen1) {
    ASSERT (VddNbVid[Index] != 0);
    TargetVid = VddNbVid[Index];
    IDS_HDT_CONSOLE (GNB_TRACE, "  Gen2 TargetVid - %x\n", TargetVid);
  } else {

    MinVidIndex = 0;
    for (Index = 0; Index < 5; Index++) {
      if (VddNbVid[Index] > VddNbVid[MinVidIndex]) {
        MinVidIndex = (UINT8) Index;
      }
    }
    IDS_HDT_CONSOLE (GNB_TRACE, "  MinVidIndex - %x\n", MinVidIndex);
    ASSERT (VddNbVid[MinVidIndex] != 0);
    TargetVid = VddNbVid[MinVidIndex];
    IDS_HDT_CONSOLE (GNB_TRACE, "  Gen1 TargetVid - %x\n", TargetVid);
  }

  IDS_HDT_CONSOLE (PCIE_MISC, "  Set Voltage for Gen %d, Vid code %d\n", LinkCap, TargetVid);
  Millivolt = GnbTranslateVidCodeToMillivoltV5 (TargetVid, GnbLibGetHeader (Pcie)) * 4 / 100;
  GnbRegisterWriteML (GnbGetHandle (GnbLibGetHeader (Pcie)), TYPE_SMU_MSG, SMC_MSG_VDDNB_REQUEST, &Millivolt, 0, GnbLibGetHeader (Pcie));
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieSetVoltageML Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * PLL power up latency
 *
 *
 * @param[in]  Wrapper     Pointer to Wrapper config descriptor
 * @param[in]  Pcie        Pointer to PICe configuration data area
 * @retval                 Pll wake up latency in us
 */
UINT8
PciePifGetPllPowerUpLatencyML (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  return 35;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Get max link speed capability supported by this port
 *
 *
 *
 * @param[in]  Flags              See Flags PCIE_PORT_GEN_CAP_BOOT / PCIE_PORT_GEN_CAP_MAX
 * @param[in]  Engine             Pointer to engine config descriptor
 * @retval     PcieGen1/PcieGen2  Max supported link gen capability
 */
PCIE_LINK_SPEED_CAP
PcieGetLinkSpeedCapML (
  IN       UINT32                Flags,
  IN       PCIe_ENGINE_CONFIG    *Engine
  )
{
  PCIE_LINK_SPEED_CAP   LinkSpeedCapability;
  PCIe_WRAPPER_CONFIG   *Wrapper;
  PCIe_PLATFORM_CONFIG  *Pcie;

  Wrapper = PcieConfigGetParentWrapper (Engine);
  Pcie = PcieConfigGetPlatform (Wrapper);

  LinkSpeedCapability = PcieGen2;

  if (Engine->Type.Port.PortData.LinkSpeedCapability == PcieGenMaxSupported) {
    Engine->Type.Port.PortData.LinkSpeedCapability = (UINT8) LinkSpeedCapability;
  }
  if (Pcie->PsppPolicy == PsppPowerSaving) {
    LinkSpeedCapability = PcieGen1;
  }
  if (Engine->Type.Port.PortData.LinkSpeedCapability < LinkSpeedCapability) {
    LinkSpeedCapability = Engine->Type.Port.PortData.LinkSpeedCapability;
  }
  if ((Flags & PCIE_PORT_GEN_CAP_BOOT) != 0) {

    if (( Pcie->PsppPolicy == PsppBalanceLow ||
          Engine->Type.Port.PortData.MiscControls.LinkSafeMode == PcieGen1)
        && !PcieConfigIsSbPcieEngine (Engine)) {

      LinkSpeedCapability = PcieGen1;
    }
  }
  return LinkSpeedCapability;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Limit MaxPayload to 256 for x1 ports
 *
 *
 *
 * @param[in]  Engine             Pointer to engine config descriptor
 * @retval     MaxPayload         MaxPayloadSupport
 */
UINT8
PcieMaxPayloadML (
  IN       PCIe_ENGINE_CONFIG    *Engine
  )
{
  UINT8    MaxPayload;

  MaxPayload = MAX_PAYLOAD_512;
  if (Engine->EngineData.StartLane == Engine->EngineData.EndLane) {
    MaxPayload = MAX_PAYLOAD_256;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieMaxPayloadML Exit with MaxPayload = %d for StartLane = %d and EndLane = %d\n", MaxPayload, Engine->EngineData.StartLane, Engine->EngineData.EndLane);
  return MaxPayload;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get timestamp
 *
 *
 *
 * @param[in]  StdHeader             Standard header
 * @retval     TimeStamp             Count
 */

UINT32
GnbTimeStampML (
  IN       AMD_CONFIG_PARAMS          *StdHeader
  )
{
  UINT32 TimeStamp;

  TimeStamp = 0;

  GnbLibPciIndirectRead (
    MAKE_SBDFO (0, 0, 0, 0, 0xE0),
    0x13080F0,
    AccessWidth32,
    &TimeStamp,
    StdHeader
    );

  return TimeStamp;

}

/*----------------------------------------------------------------------------------------*/
/**
 * Select master PLL
 *
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[out] ConfigChanged       Pointer to boolean indicator that configuration was changed
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */

VOID
PcieTopologySelectMasterPllML (
  IN       PCIe_WRAPPER_CONFIG   *Wrapper,
     OUT   BOOLEAN               *ConfigChanged,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_ENGINE_CONFIG        *EngineList;
  UINT16                    MasterLane;
  UINT16                    MasterHotplugLane;
  UINT16                    EngineMasterLane;
  D0F0xE4_WRAP_8013_STRUCT  D0F0xE4_WRAP_8013;
  D0F0xE4_WRAP_8013_STRUCT  D0F0xE4_WRAP_8013_BASE;
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologySelectMasterPllML Enter\n");
  MasterLane = 0xFFFF;
  MasterHotplugLane = 0xFFFF;
  EngineList = PcieConfigGetChildEngine (Wrapper);
  while (EngineList != NULL) {
    if (PcieConfigIsEngineAllocated (EngineList) && EngineList->Type.Port.PortData.PortPresent != PortDisabled && PcieConfigIsPcieEngine (EngineList)) {
      EngineMasterLane = PcieConfigGetPcieEngineMasterLane (EngineList);
      if (EngineList->Type.Port.PortData.LinkHotplug != HotplugDisabled) {
        MasterHotplugLane = (EngineMasterLane < MasterHotplugLane) ? EngineMasterLane : MasterHotplugLane;
      } else {
        MasterLane = (EngineMasterLane < MasterLane) ? EngineMasterLane : MasterLane;
        if (PcieConfigIsSbPcieEngine (EngineList)) {
          break;
        }
      }
    }
    EngineList = PcieLibGetNextDescriptor (EngineList);
  }

  if (MasterLane == 0xffff) {
    if (MasterHotplugLane != 0xffff) {
      MasterLane = MasterHotplugLane;
    } else {
      MasterLane = 0x0;
    }
  }

  D0F0xE4_WRAP_8013.Value = PcieRegisterRead (
                              Wrapper,
                              WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8013_ADDRESS),
                              Pcie
                              );
  D0F0xE4_WRAP_8013_BASE.Value = D0F0xE4_WRAP_8013.Value;

  if (MasterLane <= 3 ) {
    Wrapper->MasterPll = GNB_PCIE_MASTERPLL_A;
  } else {
    Wrapper->MasterPll = GNB_PCIE_MASTERPLL_B;
  }

  IDS_OPTION_HOOK (IDS_GNB_PCIE_MASTERPLL_SELECTION, &(Wrapper->MasterPll), GnbLibGetHeader (Pcie));

  if (Wrapper->MasterPll == GNB_PCIE_MASTERPLL_A) {
    D0F0xE4_WRAP_8013.Field.MasterPciePllA = 0x1;
    D0F0xE4_WRAP_8013.Field.MasterPciePllB = 0x0;
  } else {
    D0F0xE4_WRAP_8013.Field.MasterPciePllA = 0x0;
    D0F0xE4_WRAP_8013.Field.MasterPciePllB = 0x1;
  }

  if (ConfigChanged != NULL) {
    *ConfigChanged = (D0F0xE4_WRAP_8013.Value == D0F0xE4_WRAP_8013_BASE.Value) ? FALSE : TRUE;
  }
  PcieRegisterWrite (
    Wrapper,
    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8013_ADDRESS),
    D0F0xE4_WRAP_8013.Value,
    FALSE,
    Pcie
    );

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologySelectMasterPllML Exit\n");
}

