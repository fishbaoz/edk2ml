/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe early post initialization.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 282985 $   @e \$Date: 2014-01-20 12:02:23 -0600 (Mon, 20 Jan 2014) $
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
#include  "Gnb.h"
#include  "GnbPcie.h"
#include  "GnbUra.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieConfig.h"
#include  "GnbPcieTrainingV2.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbPcieInitLibV4.h"
#include  "GnbPcieInitLibV5.h"
#include  "PcieLibML.h"
#include  "PcieComplexDataML.h"
#include  "GnbRegistersML.h"
#include  "GnbRegisterAccML.h"
#include  "OptionGnb.h"
#include  "GnbSmuInitLibV7.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITML_PCIEEARLYINITML_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern GNB_BUILD_OPTIONS_ML GnbBuildOptionsML;
extern BUILD_OPT_CFG UserOptions;
extern CONST PCIE_HOST_REGISTER_TABLE_HEADER ROMDATA PcieInitEarlyTableML;
extern CONST PCIE_HOST_REGISTER_TABLE_HEADER ROMDATA CoreInitTableML;
extern CONST PCIE_PORT_REGISTER_TABLE_HEADER ROMDATA PortInitEarlyTableML;

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
 * Set port device/function mapping
 *
 *
 *
 * @param[in]  Descriptor          Silicon descriptor
 * @param[in]  Buffer              Pointer to buffer
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
AGESA_STATUS
PciePortMapInitCallbackML (
  IN       PCIe_DESCRIPTOR_HEADER            *Descriptor,
  IN OUT   VOID                              *Buffer,
  IN       PCIe_PLATFORM_CONFIG              *Pcie
  )
{
  PcieSetPortPciAddressMapML ((PCIe_SILICON_CONFIG *) Descriptor);
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Static init for various registers.
 *
 *
 *
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */
VOID
STATIC
PcieEarlyStaticInitML (
  IN       PCIe_PLATFORM_CONFIG          *Pcie
  )
{
  UINTN Index;

  for (Index = 0; Index < PcieInitEarlyTableML.Length; Index++) {
    GnbLibPciIndirectRMW (
      MAKE_SBDFO (0,0,0,0, D0F0xE0_ADDRESS),
      PcieInitEarlyTableML.Table[Index].Reg,
      AccessS3SaveWidth32,
      (UINT32)~PcieInitEarlyTableML.Table[Index].Mask,
      PcieInitEarlyTableML.Table[Index].Data,
      GnbLibGetHeader (Pcie)
      );
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Init core registers.
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper configuration descriptor
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */
VOID
STATIC
PcieEarlyCoreInitML (
  IN       PCIe_WRAPPER_CONFIG           *Wrapper,
  IN       PCIe_PLATFORM_CONFIG          *Pcie
  )
{
  UINT8   CoreId;
  UINTN   Index;
  if (PcieLibIsPcieWrapper (Wrapper)) {
    IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyCoreInitML Enter\n");
    for (CoreId = Wrapper->StartPcieCoreId; CoreId <= Wrapper->EndPcieCoreId; CoreId++) {
      for (Index = 0; Index < CoreInitTableML.Length; Index++) {
        UINT32 Value;
        Value = PcieRegisterRead (
                  Wrapper,
                  CORE_SPACE (CoreId, CoreInitTableML.Table[Index].Reg),
                  Pcie
                  );
        Value &= (~CoreInitTableML.Table[Index].Mask);
        Value |= CoreInitTableML.Table[Index].Data;
        PcieRegisterWrite (
          Wrapper,
          CORE_SPACE (CoreId, CoreInitTableML.Table[Index].Reg),
          Value,
          FALSE,
          Pcie
          );
      }
    }
    IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyCoreInitML Exit\n");
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Set Pcie Phy Isolation
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper configuration descriptor
 * @param[in]  Pcie            Pointer to PCIe configuration data area
 */
VOID
STATIC
PciePhyIsolationML (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT32  ActiveLaneBitmap;
  UINT32  PhyRxIsoDis;
  D0F0xE4_WRAP_8013_STRUCT  D0F0xE4_WRAP_8013;
  UINT32  D0F0xE4_WRAP_8021;
  UINT32  D0F0xE4_WRAP_8022;
  UINT32  D0F0xE4_WRAP_8025;
  UINT32  D0F0xE4_WRAP_8026;

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePhyIsolationML Enter\n");

  // Apply lane mux
  D0F0xE4_WRAP_8021 = 0x07060504;
  D0F0xE4_WRAP_8022 = 0x03020100;
  D0F0xE4_WRAP_8025 = 0x07060504;
  D0F0xE4_WRAP_8026 = 0x03020100;

  PcieRegisterWrite (
    Wrapper,
    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8021_ADDRESS),
    D0F0xE4_WRAP_8021,
    FALSE,
    Pcie
    );
  PcieRegisterWrite (
    Wrapper,
    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8022_ADDRESS),
    D0F0xE4_WRAP_8022,
    FALSE,
    Pcie
    );
  PcieRegisterWrite (
    Wrapper,
    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8025_ADDRESS),
    D0F0xE4_WRAP_8025,
    FALSE,
    Pcie
    );
  PcieRegisterWrite (
    Wrapper,
    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8026_ADDRESS),
    D0F0xE4_WRAP_8026,
    FALSE,
    Pcie
    );

  PhyRxIsoDis = GnbBuildOptionsML.CfgPciePhyIsolationEnable ? 0 : 3;
  ActiveLaneBitmap = PcieUtilGetWrapperLaneBitMap (LANE_TYPE_PCIE_PHY_NATIVE, 0, Wrapper);
  if ((ActiveLaneBitmap & 0xF0) != 0) {
    PhyRxIsoDis = 3;
  }

  IDS_OPTION_HOOK (IDS_GNB_PCIE_PHY_ISOLATION, &PhyRxIsoDis, GnbLibGetHeader (Pcie));

  D0F0xE4_WRAP_8013.Value = PcieRegisterRead (
                              Wrapper,
                              WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8013_ADDRESS),
                              Pcie
                              );
  D0F0xE4_WRAP_8013.Field.PhyRxIsoDis = PhyRxIsoDis;
  PcieRegisterWrite (
    Wrapper,
    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8013_ADDRESS),
    D0F0xE4_WRAP_8013.Value,
    FALSE,
    Pcie
    );
  IDS_HDT_CONSOLE (GNB_TRACE, "    ActiveLaneBitmap = 0x%x, PhyRxIsoDis = %d\n", ActiveLaneBitmap, PhyRxIsoDis);
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePhyIsolationML Exit\n");
}

UINT8 LaneMuxSelectorArrayML[] = { 7, 6, 5, 4, 3, 2, 1, 0 };

/*----------------------------------------------------------------------------------------*/
/**
 * Locate mux array index
 *
 *
 *
 * @param[in, out]  LaneMuxSelectorArrayPtr     Pointer to mux selector array
 * @param[in]       LaneMuxValue                The value that match to array
 * @retval          Index                       Index successfully mapped
 */
UINT8
PcieTopologyLocateMuxIndexML (
  IN OUT   UINT8                *LaneMuxSelectorArrayPtr,
  IN       UINT8                LaneMuxValue
  )
{
  UINT8  Index;
  for (Index = 0; Index < sizeof (LaneMuxSelectorArrayML); Index++ ) {
    if (LaneMuxSelectorArrayPtr [Index] ==  LaneMuxValue) {
      return Index;
    }
  }
  return 0;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Apply lane mux
 *
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */

VOID
PcieTopologyApplyLaneMuxML (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_ENGINE_CONFIG  *EngineList;
  UINT32              Index;
  UINT8               RxLaneMuxSelectorArray [sizeof (LaneMuxSelectorArrayML)];
  UINT8               TxLaneMuxSelectorArray [sizeof (LaneMuxSelectorArrayML)];

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyApplyLaneMuxML Enter\n");
  if (PcieLibIsPcieWrapper (Wrapper)) {
    LibAmdMemCopy (
      &TxLaneMuxSelectorArray[0],
      &LaneMuxSelectorArrayML[0],
      sizeof (LaneMuxSelectorArrayML),
      GnbLibGetHeader (Pcie)
      );
    LibAmdMemCopy (
      &RxLaneMuxSelectorArray[0],
      &LaneMuxSelectorArrayML[0],
      sizeof (LaneMuxSelectorArrayML),
      GnbLibGetHeader (Pcie)
      );
    EngineList = PcieConfigGetChildEngine (Wrapper);
    while (EngineList != NULL) {
      if (PcieLibIsPcieEngine (EngineList) && PcieLibIsEngineAllocated (EngineList)) {
        UINT32  CoreLaneBitmap;
        UINT32  PifLaneBitmap;
        UINT8   CurrentCoreLane;
        UINT8   CurrentPifLane;

        CoreLaneBitmap = PcieUtilGetEngineLaneBitMap (LANE_TYPE_PCIE_CORE_ALLOC, 0, EngineList);
        PifLaneBitmap = PcieUtilGetEngineLaneBitMap (LANE_TYPE_PCIE_PHY_NATIVE, 0, EngineList);

        while (CoreLaneBitmap != 0) {
          CurrentCoreLane = LibAmdBitScanForward (CoreLaneBitmap);
          CurrentPifLane = LibAmdBitScanForward (PifLaneBitmap);

          if (TxLaneMuxSelectorArray[CurrentPifLane] != CurrentCoreLane) {
            TxLaneMuxSelectorArray[PcieTopologyLocateMuxIndexML (TxLaneMuxSelectorArray, CurrentCoreLane)] = TxLaneMuxSelectorArray[CurrentPifLane];
            TxLaneMuxSelectorArray[CurrentPifLane] = CurrentCoreLane;
          }

          if (RxLaneMuxSelectorArray[CurrentCoreLane] != CurrentPifLane) {
            RxLaneMuxSelectorArray[PcieTopologyLocateMuxIndexML (RxLaneMuxSelectorArray, CurrentPifLane)] = RxLaneMuxSelectorArray[CurrentCoreLane];
            RxLaneMuxSelectorArray[CurrentCoreLane] = CurrentPifLane;
          }

          CoreLaneBitmap &= (~ (1 << CurrentCoreLane));
          PifLaneBitmap &= (~ (1 << CurrentPifLane));
        }
      }
      EngineList = PcieLibGetNextDescriptor (EngineList);
    }
    for (Index = 0; Index < 2; ++Index) {
      PcieRegisterWrite (
        Wrapper,
        WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8021_ADDRESS + Index),
        ((UINT32 *) TxLaneMuxSelectorArray) [Index],
        FALSE,
        Pcie
        );
      PcieRegisterWrite (
        Wrapper,
        WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8025_ADDRESS + Index),
        ((UINT32 *) RxLaneMuxSelectorArray) [Index],
        FALSE,
        Pcie
        );
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyApplyLaneMuxML Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Execute/clean up reconfiguration
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
VOID
PcieTopologyExecuteReconfigML (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  D0F0xE4_WRAP_8062_STRUCT  D0F0xE4_WRAP_8062;
  D0F0xE4_WRAP_8060_STRUCT  D0F0xE4_WRAP_8060;
  PCIe_SILICON_CONFIG       *Silicon;
  DEV_OBJECT                DevObject;
  GNB_BUILD_OPTIONS_ML  *GnbBuildOptionData;

  if (PcieLibIsPcieWrapper (Wrapper)) {
    IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyExecuteReconfigML Enter\n");

    GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, GnbLibGetHeader (Pcie));
    ASSERT (GnbBuildOptionData != NULL);

    D0F0xE4_WRAP_8062.Value = PcieRegisterRead (
                                Wrapper,
                                WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8062_ADDRESS),
                                Pcie
                                );
    D0F0xE4_WRAP_8062.Field.ReconfigureEn = 0x1;
    D0F0xE4_WRAP_8062.Field.ResetPeriod = 0x2;
    PcieRegisterWrite (
      Wrapper,
      WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8062_ADDRESS),
      D0F0xE4_WRAP_8062.Value,
      FALSE,
      Pcie
      );

    if (GnbBuildOptionData->CfgUseSMUServices == TRUE) {
      Silicon = PcieConfigGetParentSilicon (Wrapper);

      GnbLibPciIndirectRMW (
        Silicon->Address.AddressValue | D0F0xB8_ADDRESS,
        D0F0xBC_x3F81C_ADDRESS,
        AccessWidth32,
        (UINT32) ~D0F0xBC_x3F81C_RECONF_WRAPPER_MASK,
        Wrapper->WrapId << D0F0xBC_x3F81C_RECONF_WRAPPER_OFFSET,
        GnbLibGetHeader (Pcie)
        );

      DevObject.StdHeader = GnbLibGetHeader (Pcie);
      DevObject.GnbHandle = GnbGetHandle (GnbLibGetHeader (Pcie));
      DevObject.DevPciAddress.AddressValue = Silicon->Address.AddressValue;
      GnbSmuServiceRequestV7 (
        &DevObject,
        SMC_MSG_RECONFIGURE,
        0,
        0
        );
    } else {
      // Do manual reconfig for recovery mode
      D0F0xE4_WRAP_8060.Value = PcieRegisterRead (
                                  Wrapper,
                                  WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8060_ADDRESS),
                                  Pcie
                                  );
      D0F0xE4_WRAP_8060.Field.Reconfigure = 0x1;
      PcieRegisterWrite (
        Wrapper,
        WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8060_ADDRESS),
        D0F0xE4_WRAP_8060.Value,
        FALSE,
        Pcie
        );
      do {
        D0F0xE4_WRAP_8060.Value = PcieRegisterRead (
                                    Wrapper,
                                    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8060_ADDRESS),
                                    Pcie
                                    );
      } while (D0F0xE4_WRAP_8060.Field.Reconfigure == 0x1);
    }

    D0F0xE4_WRAP_8062.Field.ConfigXferMode = 0x1;
    D0F0xE4_WRAP_8062.Field.ReconfigureEn = 0x0;
    PcieRegisterWrite (
      Wrapper,
      WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8062_ADDRESS),
      D0F0xE4_WRAP_8062.Value,
      FALSE,
      Pcie
      );

    IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyExecuteReconfigML Exit\n");
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Apply Misc settings for given wrapper
 *
 *
 *
 * @param[in]  Wrapper     Pointer to Wrapper config descriptor
 * @param[in]  Pcie        Pointer to PICe configuration data area
 */
VOID
PcieMiscInitML (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  D0F0xE4_WRAP_8011_STRUCT  D0F0xE4_WRAP_8011;
  D0F0xE4_WRAP_0805_STRUCT  D0F0xE4_WRAP_0805;
  D0F0xE4_WRAP_0905_STRUCT  D0F0xE4_WRAP_0905;
  D0F0xE4_WRAP_0A05_STRUCT  D0F0xE4_WRAP_0A05;
  D0F0xE4_WRAP_0B05_STRUCT  D0F0xE4_WRAP_0B05;
  D0F0xE4_WRAP_0C05_STRUCT  D0F0xE4_WRAP_0C05;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieMiscInitML Enter\n");
  D0F0xE4_WRAP_8011.Value = PcieRegisterRead (
                              Wrapper,
                              WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8011_ADDRESS),
                              Pcie
                              );

  D0F0xE4_WRAP_8011.Field.Bitfield_23_23 = 0;

  PcieRegisterWrite (
    Wrapper,
    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8011_ADDRESS),
    D0F0xE4_WRAP_8011.Value,
    TRUE,
    Pcie
    );

  D0F0xE4_WRAP_0805.Value = PcieRegisterRead (
                              Wrapper,
                              WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0805_ADDRESS),
                              Pcie
                              );

  D0F0xE4_WRAP_0905.Value = PcieRegisterRead (
                              Wrapper,
                              WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0905_ADDRESS),
                              Pcie
                              );

  D0F0xE4_WRAP_0A05.Value = PcieRegisterRead (
                              Wrapper,
                              WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0A05_ADDRESS),
                              Pcie
                              );
  D0F0xE4_WRAP_0B05.Value = PcieRegisterRead (
                              Wrapper,
                              WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0B05_ADDRESS),
                              Pcie
                              );

  D0F0xE4_WRAP_0C05.Value = PcieRegisterRead (
                              Wrapper,
                              WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0C05_ADDRESS),
                              Pcie
                              );

  D0F0xE4_WRAP_0805.Field.STRAP_BIF_INITIAL_N_FTS_A = 0x40;
  D0F0xE4_WRAP_0905.Field.STRAP_BIF_INITIAL_N_FTS_B = 0x40;
  D0F0xE4_WRAP_0A05.Field.STRAP_BIF_INITIAL_N_FTS_C = 0x40;
  D0F0xE4_WRAP_0B05.Field.STRAP_BIF_INITIAL_N_FTS_D = 0x40;
  D0F0xE4_WRAP_0C05.Field.STRAP_BIF_INITIAL_N_FTS_E = 0x40;

  PcieRegisterWrite (
    Wrapper,
    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0805_ADDRESS),
    D0F0xE4_WRAP_0805.Value,
    TRUE,
    Pcie
    );

  PcieRegisterWrite (
    Wrapper,
    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0905_ADDRESS),
    D0F0xE4_WRAP_0905.Value,
    TRUE,
    Pcie
    );
  PcieRegisterWrite (
    Wrapper,
    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0A05_ADDRESS),
    D0F0xE4_WRAP_0A05.Value,
    TRUE,
    Pcie
    );
  PcieRegisterWrite (
    Wrapper,
    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0B05_ADDRESS),
    D0F0xE4_WRAP_0B05.Value,
    TRUE,
    Pcie
    );
  PcieRegisterWrite (
    Wrapper,
    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0C05_ADDRESS),
    D0F0xE4_WRAP_0C05.Value,
    TRUE,
    Pcie
    );
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieMiscInitML Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Switch to PCIe Native Gen1 PLL.
 *
 *
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */
AGESA_STATUS
STATIC
PcieNativeGen1PLLSwitchML (
  IN       PCIe_PLATFORM_CONFIG          *Pcie
  )
{
  PCIE_LINK_SPEED_CAP  GlobalSpeedCap;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieNativeGen1PLLSwitchML Enter\n");

  GlobalSpeedCap = PcieUtilGlobalGenCapability (
                     PCIE_PORT_GEN_CAP_MAX | PCIE_GLOBAL_GEN_CAP_TRAINED_PORTS | PCIE_GLOBAL_GEN_CAP_HOTPLUG_PORTS,
                     Pcie
                     );

  IDS_HDT_CONSOLE (GNB_TRACE, "  GlobalSpeedCap - %s\n", (GlobalSpeedCap == PcieGen1) ? "PcieGen1" : ((GlobalSpeedCap == PcieGen2) ? "PcieGen2" : "Unknown"));
  if (GlobalSpeedCap == PcieGen1) {
    GnbRegisterWriteML (GnbGetHandle (GnbLibGetHeader (Pcie)), TYPE_SMU_MSG, SMC_MSG_PCIE_NATIVE_GEN1_PLLSWITCH, 0, 0, GnbLibGetHeader (Pcie));
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieNativeGen1PLLSwitchML Exit\n");
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Per wrapper Pcie Init prior training.
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper configuration descriptor
 * @param[in]  Buffer          Pointer buffer
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */
AGESA_STATUS
STATIC
PcieEarlyInitCallbackML (
  IN       PCIe_WRAPPER_CONFIG           *Wrapper,
  IN OUT   VOID                          *Buffer,
  IN       PCIe_PLATFORM_CONFIG          *Pcie
  )
{
  AGESA_STATUS  Status;
  BOOLEAN       CoreConfigChanged;
  BOOLEAN       PllConfigChanged;
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInitCallbackML Enter\n");
  CoreConfigChanged = FALSE;
  PllConfigChanged = FALSE;
  IDS_OPTION_HOOK (IDS_BEFORE_RECONFIGURATION, Pcie, GnbLibGetHeader (Pcie));
  PcieTopologyPrepareForReconfig (Wrapper, Pcie);
  Status = PcieTopologySetCoreConfig (Wrapper, &CoreConfigChanged, Pcie);
  ASSERT (Status == AGESA_SUCCESS);
  PciePhyIsolationML (Wrapper, Pcie);
  PcieTopologyApplyLaneMuxML (Wrapper, Pcie);
  PciePifSetRxDetectPowerMode (Wrapper, Pcie);
  PciePifSetLs2ExitTimeV5 (Wrapper, Pcie);
  PciePifApplyGanging (Wrapper, Pcie);
  PcieTopologySelectMasterPllML (Wrapper, &PllConfigChanged, Pcie);
  PcieMiscInitML (Wrapper, Pcie);
  PcieTopologyExecuteReconfigML (Wrapper, Pcie);
  PcieTopologyCleanUpReconfig (Wrapper, Pcie);
  PcieTopologySetLinkReversalV4 (Wrapper, Pcie);
  PciePifPllConfigureML (Wrapper, Pcie);
  PcieTopologyLaneControlV5 (
    DisableLanes,
    PcieUtilGetWrapperLaneBitMap (LANE_TYPE_CORE_ALL, LANE_TYPE_PCIE_CORE_ALLOC, Wrapper),
    Wrapper,
    Pcie
    );

  PciePollPifForCompeletion (Wrapper, Pcie);
  PcieEarlyCoreInitML (Wrapper, Pcie);
  PcieSetSsidV4 (UserOptions.CfgGnbPcieSSID, Wrapper, Pcie);
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInitCallbackML Exit [%x]\n", Status);
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Pcie Init
 *
 *
 *
 * @param[in]  Pcie                Pointer to global PCIe configuration
 * @retval     AGESA_SUCCESS       Topology successfully mapped
 * @retval     AGESA_ERROR         Topology can not be mapped
 */

AGESA_STATUS
STATIC
PcieEarlyInitML (
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  AGESA_STATUS        Status;
  AGESA_STATUS        AgesaStatus;
  BOOLEAN             NativeGen1PLL;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInitML Enter\n");
  AgesaStatus = AGESA_SUCCESS;
  NativeGen1PLL = GnbBuildOptionsML.CfgNativeGen1PLL;

  Status = PcieConfigRunProcForAllDescriptors (DESCRIPTOR_SILICON, 0, DESCRIPTOR_TERMINATE_TOPOLOGY, PciePortMapInitCallbackML, NULL, Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  IDS_OPTION_HOOK (IDS_GNB_PMM_NATIVEGEN1PLL, &NativeGen1PLL, GnbLibGetHeader (Pcie));

  if (NativeGen1PLL == TRUE) {
    Status = PcieNativeGen1PLLSwitchML (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
  }

  Status = PcieConfigRunProcForAllWrappers (DESCRIPTOR_ALL_WRAPPERS, PcieEarlyInitCallbackML, NULL, Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  PcieEarlyStaticInitML (Pcie);
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInitML Exit [%x]\n", AgesaStatus);
  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Set misc slot capability
 *
 *
 *
 * @param[in]  Engine          Pointer to engine config descriptor
 * @param[in]  Pcie            Pointer to global PCIe configuration
 *
 */
VOID
STATIC
PcieLinkSetSlotCapML (
  IN      PCIe_ENGINE_CONFIG    *Engine,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT32 IntPin;
  GnbLibPciRMW (
    Engine->Type.Port.Address.AddressValue | DxF0x58_ADDRESS,
    AccessWidth32,
    0xffffffff,
    1 << DxF0x58_SlotImplemented_OFFSET,
    GnbLibGetHeader (Pcie)
    );

  if (Engine->Type.Port.Address.Address.Function < 5) {
    IntPin = Engine->Type.Port.Address.Address.Function;
  } else {
    IntPin = Engine->Type.Port.Address.Address.Function - 4;
  }

  GnbLibPciRMW (
    Engine->Type.Port.Address.AddressValue | DxF0x3C_ADDRESS,
    AccessWidth32,
    0xffffffff,
    IntPin << DxF0x3C_IntPin_OFFSET,
    GnbLibGetHeader (Pcie)
    );
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to init various features on all active ports
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
PcieEarlyPortInitCallbackML (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyPortInitCallbackML Enter\n");
  ASSERT (Engine->EngineData.EngineType == PciePortEngine);
  PciePortProgramRegisterTable (PortInitEarlyTableML.Table, PortInitEarlyTableML.Length, Engine, FALSE, Pcie);
  PcieSetLinkSpeedCapV4 (PcieGen1, Engine, Pcie);
  PcieSetLinkWidthCap (Engine, Pcie);
  PcieCompletionTimeout (Engine, Pcie);
  PcieLinkSetSlotCapML (Engine, Pcie);
  PcieLinkInitHotplugV5 (Engine, Pcie);
  if (Engine->Type.Port.PortData.PortPresent == PortDisabled ||
      (Engine->Type.Port.PortData.EndpointStatus == EndpointNotPresent &&
       Engine->Type.Port.PortData.LinkHotplug != HotplugEnhanced &&
       Engine->Type.Port.PortData.LinkHotplug != HotplugServer)) {
    ASSERT (!PcieConfigIsSbPcieEngine (Engine));
    //
    //  Pass endpoint status in scratch
    //
    PciePortRegisterRMW (
      Engine,
      DxFxxE4_x01_ADDRESS,
      0x1,
      0x1,
      FALSE,
      Pcie
      );
    PcieTrainingSetPortStateV2 (Engine, LinkStateDeviceNotPresent, FALSE, Pcie);
  }
  if (PcieConfigIsSbPcieEngine (Engine)) {
    PcieTrainingSetPortStateV2 (Engine, LinkStateTrainingSuccess, FALSE, Pcie);
  }
  if (Engine->Type.Port.PortData.MiscControls.LinkComplianceMode == 0x1) {
    PcieTrainingSetPortStateV2 (Engine, LinkStateTrainingCompleted, FALSE, Pcie);
  }

  IDS_OPTION_HOOK (IDS_GNB_GEN1_LOOPBACK, Engine, (AMD_CONFIG_PARAMS *)Pcie->StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyPortInitCallbackML Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Master procedure to init various features on all active ports
 *
 *
 * @param[in]   Pcie            Pointer to global PCIe configuration
 * @retval      AGESA_STATUS
 *
 */
AGESA_STATUS
STATIC
PcieEarlyPortInitML (
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  AGESA_STATUS  Status;
  Status = AGESA_SUCCESS;
  // Leave all device in Presence Detect Presence state for distributed training will be completed at PciePortPostEarlyInit
  if (Pcie->TrainingAlgorithm == PcieTrainingDistributed) {
    Pcie->TrainingExitState = LinkStateResetExit;
  }

  PcieConfigRunProcForAllEngines (
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    PcieEarlyPortInitCallbackML,
    NULL,
    Pcie
    );

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * PCIe Early Post Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
PcieEarlyInterfaceML (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS          Status;
  AGESA_STATUS          AgesaStatus;
  PCIe_PLATFORM_CONFIG  *Pcie;
  AgesaStatus = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInterfaceML Enter\n");
  Status = PcieLocateConfigurationData (StdHeader, &Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  if (Status == AGESA_SUCCESS) {
    PciePortsVisibilityControlV5 (UnhidePorts, Pcie);

    Status = PcieEarlyInitML (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    Status = PcieEarlyPortInitML (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    Status = PcieTrainingV2 (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_PCIE_PHY_CONFIG, Pcie, StdHeader);
    PciePortsVisibilityControlV5 (HidePorts, Pcie);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInterfaceML Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}

