/* $NoKeywords:$ */
/**
 * @file
 *
 * Family specific PCIe complex initialization services
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
#include  "GnbPcieConfig.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieInitLibV5.h"
#include  "GnbRegistersCommonV2.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBPCIEINITLIBV5_PCIESILICONSERVICESV5_FILECODE
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

VOID
STATIC
PcieSiliconControlPortsV5 (
  IN      PCIE_PORT_VISIBILITY    Control,
  IN      PCIe_SILICON_CONFIG     *Silicon,
  IN      PCIe_PLATFORM_CONFIG    *Pcie
  );

VOID
STATIC
PcieSiliconEnablePortsV5 (
  IN      PCIe_SILICON_CONFIG   *Silicon,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Control port visibility in PCI config space
 *
 *
 * @param[in]  Control         Control Hide/Unhide ports
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */
VOID
PciePortsVisibilityControlV5 (
  IN      PCIE_PORT_VISIBILITY    Control,
  IN      PCIe_PLATFORM_CONFIG    *Pcie
  )
{
  PCIe_SILICON_CONFIG *SiliconList;
  SiliconList = (PCIe_SILICON_CONFIG *) PcieConfigGetChild (DESCRIPTOR_SILICON, &Pcie->Header);
  while (SiliconList != NULL) {
    switch (Control) {
    case UnhidePorts:
      PcieSiliconControlPortsV5 (UnhidePorts, SiliconList, Pcie);
      break;
    case HidePorts:
      PcieSiliconControlPortsV5 (HidePorts, SiliconList, Pcie);
      PcieSiliconEnablePortsV5 (SiliconList, Pcie);
      break;
    default:
      ASSERT (FALSE);
    }
    SiliconList = (PCIe_SILICON_CONFIG *) PcieConfigGetNextTopologyDescriptor (SiliconList, DESCRIPTOR_TERMINATE_TOPOLOGY);
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * Hide/Unhide all ports
 *
 *
 * @param[in]  Control             Control Hide/Unhide ports
 * @param[in]  Silicon             Pointer to silicon configuration descriptor
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */

VOID
STATIC
PcieSiliconControlPortsV5 (
  IN      PCIE_PORT_VISIBILITY    Control,
  IN      PCIe_SILICON_CONFIG     *Silicon,
  IN      PCIe_PLATFORM_CONFIG    *Pcie
  )
{
  PCIe_ENGINE_CONFIG  *EngineList;
  UINT32              Value;
  Value = (Control == HidePorts) ? ((1 << D0F0xCC_x01_BridgeDis_OFFSET) | (1 << D0F0xCC_x01_CfgDis_OFFSET)) : 0;
  Value |= (1 << D0F0xCC_x01_CsrEnable_OFFSET) | (1 << D0F0xCC_x01_SetPowEn_OFFSET);
  EngineList = PcieConfigGetChildEngine (Silicon);
  while (EngineList != NULL) {
    if (PcieConfigIsPcieEngine (EngineList)) {
      GnbLibPciIndirectRMW (
        Silicon->Address.AddressValue | D0F0xC8_ADDRESS,
        D0F0xCC_x01_ADDRESS | ((EngineList->Type.Port.NativeDevNumber << 3 | EngineList->Type.Port.NativeFunNumber) << D0F0xC8_NB_DEV_IND_SEL_OFFSET),
        AccessS3SaveWidth32,
        (UINT32)~(D0F0xCC_x01_BridgeDis_MASK | D0F0xCC_x01_CfgDis_MASK | D0F0xCC_x01_CsrEnable_MASK | D0F0xCC_x01_SetPowEn_MASK),
        Value,
        GnbLibGetHeader (Pcie)
        );
    }
    EngineList = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor (EngineList, DESCRIPTOR_TERMINATE_GNB);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Hide unused ports
 *
 *
 *
 * @param[in]  Silicon             Pointer to silicon configuration data area
 * @param[in]  Pcie                Pointer to data area up to 256 byte
 */

VOID
STATIC
PcieSiliconEnablePortsV5 (
  IN      PCIe_SILICON_CONFIG   *Silicon,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_ENGINE_CONFIG  *EngineList;
  EngineList = PcieConfigGetChildEngine (Silicon);
  while (EngineList != NULL) {
    if (PcieConfigIsPcieEngine (EngineList)) {
      if (!PcieConfigIsSbPcieEngine (EngineList) &&
        (PcieConfigCheckPortStatus (EngineList, INIT_STATUS_PCIE_TRAINING_SUCCESS) ||
        ((EngineList->Type.Port.PortData.LinkHotplug != HotplugDisabled) &&
        (EngineList->Type.Port.PortData.LinkHotplug != HotplugInboard)))) {
        GnbLibPciIndirectRMW (
          Silicon->Address.AddressValue | D0F0xC8_ADDRESS,
          D0F0xCC_x01_ADDRESS | ((EngineList->Type.Port.PortData.DeviceNumber << 3 | EngineList->Type.Port.PortData.FunctionNumber)  << D0F0xC8_NB_DEV_IND_SEL_OFFSET),
          AccessS3SaveWidth32,
          (UINT32)~(D0F0xCC_x01_BridgeDis_MASK | D0F0xCC_x01_CfgDis_MASK | D0F0xCC_x01_CsrEnable_MASK | D0F0xCC_x01_SetPowEn_MASK),
          ((1 << D0F0xCC_x01_CsrEnable_OFFSET) | (1 << D0F0xCC_x01_SetPowEn_OFFSET)),
          GnbLibGetHeader (Pcie)
        );
      }
    }
    EngineList = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor (EngineList, DESCRIPTOR_TERMINATE_GNB);
  }
}
