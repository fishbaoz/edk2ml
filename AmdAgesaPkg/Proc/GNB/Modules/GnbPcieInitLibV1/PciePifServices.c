/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe PIF initialization routine
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
#include  "GnbPcie.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieConfig.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbRegistersCommon.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBPCIEINITLIBV1_PCIEPIFSERVICES_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define PIF_GANG_0to1  0x1
#define PIF_GANG_2to3  (0x1 << 1)
#define PIF_GANG_4to5  (0x1 << 2)
#define PIF_GANG_6to7  (0x1 << 3)
#define PIF_GANG_0to3  (0x1 << 4)
#define PIF_GANG_4to7  (0x1 << 8)
#define PIF_GANG_0to7  (0x1 << 9)
#define PIF_GANG_ALL   (0x1 << 25)

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------*/
/**
 * Apply PIF ganging for all lanes for given wrapper
 *
 *
 *
 * @param[in]  Wrapper     Pointer to Wrapper config descriptor
 * @param[in]  Pcie        Pointer to PICe configuration data area
 */


VOID
PciePifApplyGanging (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_ENGINE_CONFIG      *EngineList;
  UINT32                  LaneBitmap;
  UINT8                   Pif;
  D0F0xE4_PIF_0011_STRUCT D0F0xE4_PIF_0011[2];
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePifApplyGanging Enter\n");
  LibAmdMemFill (&D0F0xE4_PIF_0011, 0, sizeof (D0F0xE4_PIF_0011), GnbLibGetHeader (Pcie));
  EngineList = PcieConfigGetChildEngine (Wrapper);
  while (EngineList != NULL) {
    if (PcieLibIsEngineAllocated (EngineList)) {
      LaneBitmap = PcieUtilGetEngineLaneBitMap (LANE_TYPE_PCIE_PHY_NATIVE | LANE_TYPE_DDI_PHY_NATIVE, 0, EngineList);
      switch (LaneBitmap) {
      case  0x0003:
        D0F0xE4_PIF_0011[0].Field.X2Lane10 = 0x1;
        break;
      case  0x000c:
        D0F0xE4_PIF_0011[0].Field.X2Lane32 = 0x1;
        break;
      case  0x0030:
        D0F0xE4_PIF_0011[0].Field.X2Lane54 = 0x1;
        break;
      case  0x00c0:
        D0F0xE4_PIF_0011[0].Field.X2Lane76 = 0x1;
        break;
      case  0x000f:
        D0F0xE4_PIF_0011[0].Field.X4Lane30 = 0x1;
        break;
      case  0x00f0:
        D0F0xE4_PIF_0011[0].Field.X4Lane74 = 0x1;
        break;
      case  0x00ff:
        D0F0xE4_PIF_0011[0].Field.X8Lane70 = 0x1;
        break;
      case  0x0300:
        D0F0xE4_PIF_0011[1].Field.X2Lane10 = 1;
        break;
      case  0x0c00:
        D0F0xE4_PIF_0011[1].Field.X2Lane32 = 0x1;
        break;
      case  0x3000:
        D0F0xE4_PIF_0011[1].Field.X2Lane54 = 0x1;
        break;
      case  0xc000:
        D0F0xE4_PIF_0011[1].Field.X2Lane76 = 0x1;
        break;
      case  0x0f00:
        D0F0xE4_PIF_0011[1].Field.X4Lane30 = 0x1;
        break;
      case  0xf000:
        D0F0xE4_PIF_0011[1].Field.X4Lane74 = 0x1;
        break;
      case  0xff00:
        D0F0xE4_PIF_0011[1].Field.X8Lane70 = 0x1;
        break;
      case  0xffff:
        D0F0xE4_PIF_0011[0].Field.MultiPif = 0x1;
        D0F0xE4_PIF_0011[1].Field.MultiPif = 0x1;
        break;
      default:
        break;
      }
    }
    EngineList = PcieLibGetNextDescriptor (EngineList);
  }
  for (Pif = 0; Pif < Wrapper->NumberOfPIFs; Pif++) {
    PcieRegisterWrite (
        Wrapper,
        PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0011_ADDRESS),
        D0F0xE4_PIF_0011[Pif].Value,
        FALSE,
        Pcie
        );
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePifApplyGanging Exit\n");
}


/*----------------------------------------------------------------------------------------*/
/**
 * PLL powerdown
 *
 *
 * @param[in]  LaneBitmap  Power down PLL for these lanes
 * @param[in]  Wrapper     Pointer to Wrapper config descriptor
 * @param[in]  Pcie        Pointer to PICe configuration data area
 */

VOID
PciePifPllPowerDown (
  IN      UINT32                LaneBitmap,
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                   Nibble;
  UINT16                  NibbleBitmap;
  D0F0xE4_PIF_0012_STRUCT D0F0xE4_PIF_0012;
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePifPllPowerDown Enter\n");
  for (Nibble = 0; Nibble < 4; Nibble++) {
    NibbleBitmap = (0xF << (Nibble * 4));
    if ((LaneBitmap & NibbleBitmap) == NibbleBitmap) {
      D0F0xE4_PIF_0012.Value = PcieRegisterRead (
                                 Wrapper,
                                 PIF_SPACE (Wrapper->WrapId, (Nibble >> 1), D0F0xE4_PIF_0012_ADDRESS + (Nibble & 0x1)),
                                 Pcie
                                 );

      D0F0xE4_PIF_0012.Field.PllPowerStateInTxs2 = PifPowerStateOff;
      D0F0xE4_PIF_0012.Field.PllPowerStateInOff = PifPowerStateOff;
      D0F0xE4_PIF_0012.Field.TxPowerStateInTxs2 = PifPowerStateOff;
      D0F0xE4_PIF_0012.Field.RxPowerStateInRxs2 = PifPowerStateOff;
      PcieRegisterWrite (
        Wrapper,
        PIF_SPACE (Wrapper->WrapId, (Nibble >> 1), D0F0xE4_PIF_0012_ADDRESS + (Nibble & 0x1)),
        D0F0xE4_PIF_0012.Value,
        TRUE,
        Pcie
        );
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePifPllPowerDown Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * PLL init for DDI
 *
 *
 *
 * @param[in]  Wrapper     Pointer to Wrapper config descriptor
 * @param[in]  Pcie        Pointer to PICe configuration data area
 */

VOID
PciePifPllInitForDdi (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                   Nibble;
  UINT32                  LaneBitmap;
  D0F0xE4_PIF_0012_STRUCT D0F0xE4_PIF_0012;
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePifPllInitForDdi Enter\n");
  LaneBitmap = PcieUtilGetWrapperLaneBitMap (LANE_TYPE_DDI_PHY_NATIVE, 0, Wrapper);
  for (Nibble = 0; Nibble < 4; Nibble++) {
    if (LaneBitmap & (0xF << (Nibble * 4))) {
      D0F0xE4_PIF_0012.Value = PcieRegisterRead (
                                 Wrapper,
                                 PIF_SPACE (Wrapper->WrapId, (Nibble >> 1), D0F0xE4_PIF_0012_ADDRESS + (Nibble & 0x1)),
                                 Pcie
                                 );

      D0F0xE4_PIF_0012.Field.PllPowerStateInTxs2 = PifPowerStateOff;
      D0F0xE4_PIF_0012.Field.PllPowerStateInOff = PifPowerStateOff;
      D0F0xE4_PIF_0012.Field.PllRampUpTime = 0x2;
      PcieRegisterWrite (
        Wrapper,
        PIF_SPACE (Wrapper->WrapId, (Nibble >> 1), D0F0xE4_PIF_0012_ADDRESS + (Nibble & 0x1)),
        D0F0xE4_PIF_0012.Value,
        FALSE,
        Pcie
        );
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePifPllInitForDdi Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Poll for on PIF to indicate action completion
 *
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
VOID
PciePollPifForCompeletion (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                   Pif;
  D0F0xE4_PIF_0015_STRUCT D0F0xE4_PIF_0015;
  for (Pif = 0; Pif < Wrapper->NumberOfPIFs; Pif++) {
    do {
      D0F0xE4_PIF_0015.Value = PcieRegisterRead (
                                 Wrapper,
                                 PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0015_ADDRESS),
                                 Pcie
                                 );
    } while ((D0F0xE4_PIF_0015.Value & 0xff) != 0xff);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Disable fifo reset
 *
 *
 *
 * @param[in]  Wrapper     Pointer to Wrapper config descriptor
 * @param[in]  Pcie        Pointer to PICe configuration data area
 */


VOID
PciePifDisableFifoReset (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8 Pif;
  for (Pif = 0; Pif < Wrapper->NumberOfPIFs; Pif++) {
    PcieRegisterWriteField (
      Wrapper,
      PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0010_ADDRESS),
      D0F0xE4_PIF_0010_RxDetectFifoResetMode_OFFSET,
      D0F0xE4_PIF_0010_RxDetectFifoResetMode_WIDTH,
      0,
      FALSE,
      Pcie
      );
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Program LS2 exit time
 *
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */

VOID
PciePifSetLs2ExitTime (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8 Pif;
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePifSetLs2ExitTime Enter\n");
  for (Pif = 0; Pif < Wrapper->NumberOfPIFs; Pif++) {
    PcieRegisterWriteField (
        Wrapper,
        PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0010_ADDRESS),
        D0F0xE4_PIF_0010_Ls2ExitTime_OFFSET,
        D0F0xE4_PIF_0010_Ls2ExitTime_WIDTH,
        0x0,
        FALSE,
        Pcie
        );
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePifSetLs2ExitTime Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Set PLL mode for L1
 *
 *
 * @param[in]  LaneBitmap  Power down PLL for these lanes
 * @param[in]  Wrapper     Pointer to Wrapper config descriptor
 * @param[in]  Pcie        Pointer to PICe configuration data area
 */

VOID
PciePifSetPllModeForL1 (
  IN      UINT32                LaneBitmap,
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                   Nibble;
  D0F0xE4_PIF_0012_STRUCT D0F0xE4_PIF_0012;
  for (Nibble = 0; Nibble < 4; Nibble++) {
    if (LaneBitmap & (0xF << (Nibble * 4))) {
      D0F0xE4_PIF_0012.Value = PcieRegisterRead (
                                 Wrapper,
                                 PIF_SPACE (Wrapper->WrapId, (Nibble >> 1), D0F0xE4_PIF_0012_ADDRESS + (Nibble & 0x1)),
                                 Pcie
                                 );
      D0F0xE4_PIF_0012.Field.RxPowerStateInRxs2 = PifPowerStateLS2;
      D0F0xE4_PIF_0012.Field.TxPowerStateInTxs2 = PifPowerStateLS2;
      D0F0xE4_PIF_0012.Field.PllPowerStateInTxs2 = PifPowerStateOff;
      D0F0xE4_PIF_0012.Field.PllPowerStateInOff = PifPowerStateOff;
      D0F0xE4_PIF_0012.Field.PllRampUpTime = 0x1;
      PcieRegisterWrite (
        Wrapper,
        PIF_SPACE (Wrapper->WrapId, (Nibble >> 1), D0F0xE4_PIF_0012_ADDRESS + (Nibble & 0x1)),
        D0F0xE4_PIF_0012.Value,
        TRUE,
        Pcie
        );
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Program receiver detection power mode
 *
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */

VOID
PciePifSetRxDetectPowerMode (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8 Pif;
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePifSetRxDetectPowerMode Enter\n");
  for (Pif = 0; Pif < Wrapper->NumberOfPIFs; Pif++) {
    PcieRegisterWriteField (
        Wrapper,
        PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0010_ADDRESS),
        D0F0xE4_PIF_0010_RxDetectTxPwrMode_OFFSET,
        D0F0xE4_PIF_0010_RxDetectTxPwrMode_WIDTH,
        0x1,
        FALSE,
        Pcie
        );
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePifSetRxDetectPowerMode Enter\n");
}


/*----------------------------------------------------------------------------------------*/
/**
 * Pll ramp up time
 *
 *
 *
 * @param[in]  Rampup              Ramp up time
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
VOID
PciePifSetPllRampTime (
  IN      PCIE_PLL_RAMPUP_TIME  Rampup,
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                   Pif;
  D0F0xE4_PIF_0012_STRUCT D0F0xE4_PIF_0012;
  D0F0xE4_PIF_0013_STRUCT D0F0xE4_PIF_0013;
  D0F0xE4_PIF_0010_STRUCT D0F0xE4_PIF_0010;
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePifSetPllRampTime Enter\n");
  for (Pif = 0; Pif < Wrapper->NumberOfPIFs; Pif++) {
    D0F0xE4_PIF_0012.Value = PcieRegisterRead (
                               Wrapper,
                               PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0012_ADDRESS),
                               Pcie
                               );
    D0F0xE4_PIF_0013.Value = PcieRegisterRead (
                               Wrapper,
                               PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0013_ADDRESS),
                               Pcie
                               );
    D0F0xE4_PIF_0010.Value = PcieRegisterRead (
                               Wrapper,
                               PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0010_ADDRESS),
                               Pcie
                               );
    if (Rampup == NormalRampup) {
      D0F0xE4_PIF_0012.Field.PllRampUpTime = 0x1;
      D0F0xE4_PIF_0013.Field.PllRampUpTime = 0x1;
      D0F0xE4_PIF_0010.Field.Ls2ExitTime = 0x0;
    } else {
      D0F0xE4_PIF_0012.Field.PllRampUpTime = 0x3;
      D0F0xE4_PIF_0013.Field.PllRampUpTime = 0x3;
      D0F0xE4_PIF_0010.Field.Ls2ExitTime = 0x6;
    }
    PcieRegisterWrite (
      Wrapper,
      PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0012_ADDRESS),
      D0F0xE4_PIF_0012.Value,
      FALSE,
      Pcie
      );
    PcieRegisterWrite (
      Wrapper,
      PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0013_ADDRESS),
      D0F0xE4_PIF_0013.Value,
      FALSE,
      Pcie
      );
    PcieRegisterWrite (
      Wrapper,
      PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0010_ADDRESS),
      D0F0xE4_PIF_0010.Value,
      FALSE,
      Pcie
      );
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePifSetPllRampTime Exit\n");
}


/*----------------------------------------------------------------------------------------*/
/**
 * Power down PIFs
 *
 *
 *
 * @param[in]  Control             Power up or Power down control
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
VOID
PciePifPllPowerControl (
  IN      PCIE_PIF_POWER_CONTROL  Control,
  IN      PCIe_WRAPPER_CONFIG     *Wrapper,
  IN      PCIe_PLATFORM_CONFIG    *Pcie
  )
{
  UINT8   Pif;
  UINT8   PllPowerStateInOff;
  PllPowerStateInOff = (Control == PowerDownPifs) ? PifPowerStateOff : PifPowerStateL0;
  for (Pif = 0; Pif < Wrapper->NumberOfPIFs; Pif++) {
    PcieRegisterWriteField (
      Wrapper,
      PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0012_ADDRESS),
      D0F0xE4_PIF_0012_PllPowerStateInOff_OFFSET,
      D0F0xE4_PIF_0012_PllPowerStateInOff_WIDTH,
      PllPowerStateInOff,
      FALSE,
      Pcie
      );
    PcieRegisterWriteField (
      Wrapper,
      PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0013_ADDRESS),
      D0F0xE4_PIF_0013_PllPowerStateInOff_OFFSET,
      D0F0xE4_PIF_0013_PllPowerStateInOff_WIDTH,
      PllPowerStateInOff,
      FALSE,
      Pcie
      );
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * Power down PIFs
 *
 *
 *
 * @param[in]  Control             Power up/Down control
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
VOID
PciePifFullPowerStateControl (
  IN      PCIE_PIF_POWER_CONTROL  Control,
  IN      PCIe_WRAPPER_CONFIG     *Wrapper,
  IN      PCIe_PLATFORM_CONFIG    *Pcie
  )
{
  UINT8                   Pif;
  D0F0xE4_PIF_0012_STRUCT D0F0xE4_PIF_0012;
  D0F0xE4_PIF_0013_STRUCT D0F0xE4_PIF_0013;
  for (Pif = 0; Pif < Wrapper->NumberOfPIFs; Pif++) {
    D0F0xE4_PIF_0012.Value = PcieRegisterRead (
                               Wrapper,
                               PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0012_ADDRESS),
                               Pcie
                               );
    D0F0xE4_PIF_0013.Value = PcieRegisterRead (
                               Wrapper,
                               PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0013_ADDRESS),
                               Pcie
                               );
    if (Control == PowerDownPifs) {
      D0F0xE4_PIF_0012.Field.PllPowerStateInOff = PifPowerStateOff;
      D0F0xE4_PIF_0012.Field.PllPowerStateInTxs2 = PifPowerStateOff;
      D0F0xE4_PIF_0012.Field.TxPowerStateInTxs2 = PifPowerStateOff;
      D0F0xE4_PIF_0012.Field.RxPowerStateInRxs2 = PifPowerStateOff;
      D0F0xE4_PIF_0013.Field.PllPowerStateInOff = PifPowerStateOff;
      D0F0xE4_PIF_0013.Field.PllPowerStateInTxs2 = PifPowerStateOff;
      D0F0xE4_PIF_0013.Field.TxPowerStateInTxs2 = PifPowerStateOff;
      D0F0xE4_PIF_0013.Field.RxPowerStateInRxs2 = PifPowerStateOff;
    } else {
      D0F0xE4_PIF_0012.Field.PllPowerStateInOff = PifPowerStateLS2;
      D0F0xE4_PIF_0012.Field.PllPowerStateInTxs2 = PifPowerStateLS2;
      D0F0xE4_PIF_0012.Field.TxPowerStateInTxs2 = PifPowerStateL0;
      D0F0xE4_PIF_0012.Field.RxPowerStateInRxs2 = PifPowerStateL0;
      D0F0xE4_PIF_0013.Field.PllPowerStateInOff = PifPowerStateLS2;
      D0F0xE4_PIF_0013.Field.PllPowerStateInTxs2 = PifPowerStateLS2;
      D0F0xE4_PIF_0013.Field.TxPowerStateInTxs2 = PifPowerStateL0;
      D0F0xE4_PIF_0013.Field.RxPowerStateInRxs2 = PifPowerStateL0;
    }
    PcieRegisterWrite (
      Wrapper,
      PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0012_ADDRESS),
      D0F0xE4_PIF_0012.Value,
      FALSE,
      Pcie
      );
    PcieRegisterWrite (
      Wrapper,
      PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0013_ADDRESS),
      D0F0xE4_PIF_0013.Value,
      FALSE,
      Pcie
      );
  }
}
