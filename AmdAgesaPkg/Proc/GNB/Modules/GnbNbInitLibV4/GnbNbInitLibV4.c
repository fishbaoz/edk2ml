/* $NoKeywords:$ */
/**
 * @file
 *
 * NB services
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
#include  "S3SaveState.h"
#include  "Gnb.h"
#include  "GnbPcieConfig.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbNbInitLibV4.h"
#include  "GnbRegistersCommonV2.h"
#include  "heapManager.h"
#include  "GnbFamServices.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBNBINITLIBV4_GNBNBINITLIBV4_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define SMC_RAM_START_ADDR 0x10000ul

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

typedef struct {
  GNB_PCI_SCAN_DATA     ScanData;
  GNB_TOPOLOGY_INFO     *TopologyInfo;
} GNB_TOPOLOGY_INFO_DATA;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Check a PCIE device to see if it supports phantom functions
 *
 * @param[in] Device      Device pci address
 * @param[in] StdHeader   Standard configuration header
 * @return    TRUE        Current device supports phantom functions
 */
BOOLEAN
GnbCheckPhantomFuncSupport (
  IN       PCI_ADDR                 Device,
  IN       AMD_CONFIG_PARAMS        *StdHeader
  )
{
  UINT8   PcieCapPtr;
  UINT32  Value;
  Value = 0;

  PcieCapPtr = GnbLibFindPciCapability (Device.AddressValue, PCIE_CAP_ID, StdHeader);
  if (PcieCapPtr != 0) {
    GnbLibPciRead (Device.AddressValue | (PcieCapPtr + 4), AccessWidth32, &Value, StdHeader);
  }
  return ((Value & (BIT3 | BIT4)) != 0) ? TRUE : FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Evaluate device
 *
 *
 *
 * @param[in]     Device          PCI Address
 * @param[in,out] ScanData        Scan configuration data
 * @retval                        Scan Status
 */

SCAN_STATUS
STATIC
GnbTopologyInfoScanCallback (
  IN       PCI_ADDR                 Device,
  IN OUT   GNB_PCI_SCAN_DATA        *ScanData
  )
{
  SCAN_STATUS             ScanStatus;
  GNB_TOPOLOGY_INFO_DATA  *GnbTopologyInfo;
  PCIE_DEVICE_TYPE        DeviceType;
  ScanStatus = SCAN_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "  GnbIommuInfoScanCallback for Device = %d:%d:%d\n",
    Device.Address.Bus,
    Device.Address.Device,
    Device.Address.Function
    );
  GnbTopologyInfo = (GNB_TOPOLOGY_INFO_DATA *)ScanData;
  ScanStatus = SCAN_SUCCESS;
  DeviceType = GnbLibGetPcieDeviceType (Device, ScanData->StdHeader);
  switch (DeviceType) {
  case  PcieDeviceRootComplex:
  case  PcieDeviceDownstreamPort:
    GnbLibPciScanSecondaryBus (Device, &GnbTopologyInfo->ScanData);
    break;
  case  PcieDeviceUpstreamPort:
    GnbLibPciScanSecondaryBus (Device, &GnbTopologyInfo->ScanData);
    ScanStatus = SCAN_SKIP_FUNCTIONS | SCAN_SKIP_DEVICES | SCAN_SKIP_BUSES;
    break;
  case  PcieDevicePcieToPcix:
    GnbTopologyInfo->TopologyInfo->PcieToPciexBridge = TRUE;
    ScanStatus = SCAN_SKIP_FUNCTIONS | SCAN_SKIP_DEVICES | SCAN_SKIP_BUSES;
    break;
  case  PcieDeviceEndPoint:
  case  PcieDeviceLegacyEndPoint:
    if (GnbCheckPhantomFuncSupport (Device, ScanData->StdHeader)) {
      GnbTopologyInfo->TopologyInfo->PhantomFunction = TRUE;
    }
    ScanStatus = SCAN_SKIP_DEVICES | SCAN_SKIP_BUSES;
    break;
  default:
    break;
  }
  return ScanStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get IOMMU topology info
 *
 *
 *
 * @param[in] StartPciAddress   Start PCI address
 * @param[in] EndPciAddress     End PCI address
 * @param[in] TopologyInfo      Topology info structure
 * @param[in] StdHeader         Standard Configuration Header
 */

AGESA_STATUS
GnbGetTopologyInfoV4 (
  IN       PCI_ADDR                  StartPciAddress,
  IN       PCI_ADDR                  EndPciAddress,
     OUT   GNB_TOPOLOGY_INFO         *TopologyInfo,
  IN       AMD_CONFIG_PARAMS         *StdHeader
  )
{
  GNB_TOPOLOGY_INFO_DATA  GnbTopologyInfo;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbGetTopologyInfoV4 Enter\n");
  GnbTopologyInfo.ScanData.GnbScanCallback = GnbTopologyInfoScanCallback;
  GnbTopologyInfo.ScanData.StdHeader = StdHeader;
  GnbTopologyInfo.TopologyInfo = TopologyInfo;
  GnbLibPciScan (StartPciAddress, EndPciAddress, &GnbTopologyInfo.ScanData);
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbGetTopologyInfoV4 Exit\n");
  return AGESA_SUCCESS;
}



/*----------------------------------------------------------------------------------------*/
/**
 * SMU service request
 *
 *
 * @param[in]  GnbPciAddress   GNB PCI address
 * @param[in]  RequestId       Request ID
 * @param[in]  AccessFlags     See GNB_ACCESS_FLAGS_* definitions
 * @param[in]  StdHeader       Standard configuration header
 */

VOID
GnbSmuServiceRequestV4 (
  IN       PCI_ADDR                 GnbPciAddress,
  IN       UINT8                    RequestId,
  IN       UINT32                   AccessFlags,
  IN       AMD_CONFIG_PARAMS        *StdHeader
  )
{
  D0F0xBC_xE0003004_STRUCT  D0F0xBC_xE0003004;
  D0F0xBC_xE0003000_STRUCT  D0F0xBC_xE0003000;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuServiceRequestV4 Enter\n");
  IDS_HDT_CONSOLE (NB_MISC, "  Service Request %d\n", RequestId);

  if ((AccessFlags & GNB_REG_ACC_FLAG_S3SAVE) != 0) {
    SMU_MSG_CONTEXT SmuMsgContext;
    SmuMsgContext.GnbPciAddress.AddressValue = GnbPciAddress.AddressValue;
    SmuMsgContext.RequestId = RequestId;
    S3_SAVE_DISPATCH (StdHeader, GnbSmuServiceRequestV4S3Script_ID, sizeof (SmuMsgContext), &SmuMsgContext);
  }
  do {
    GnbLibPciIndirectRead (GnbPciAddress.AddressValue | D0F0xB8_ADDRESS, D0F0xBC_xE0003004_ADDRESS, AccessWidth32, &D0F0xBC_xE0003004.Value, StdHeader);
  } while (D0F0xBC_xE0003004.Field.IntDone == 0x0);
  GnbLibPciIndirectRead (GnbPciAddress.AddressValue | D0F0xB8_ADDRESS, D0F0xBC_xE0003000_ADDRESS, AccessWidth32, &D0F0xBC_xE0003000.Value, StdHeader);
  D0F0xBC_xE0003000.Field.IntToggle = ~D0F0xBC_xE0003000.Field.IntToggle;
  D0F0xBC_xE0003000.Field.ServiceIndex = RequestId;
  GnbLibPciIndirectWrite (GnbPciAddress.AddressValue | D0F0xB8_ADDRESS, D0F0xBC_xE0003000_ADDRESS, AccessWidth32, &D0F0xBC_xE0003000.Value, StdHeader);
  do {
    GnbLibPciIndirectRead (GnbPciAddress.AddressValue | D0F0xB8_ADDRESS, D0F0xBC_xE0003004_ADDRESS, AccessWidth32, &D0F0xBC_xE0003004.Value, StdHeader);
  } while (D0F0xBC_xE0003004.Field.IntAck == 0x0);
  do {
    GnbLibPciIndirectRead (GnbPciAddress.AddressValue | D0F0xB8_ADDRESS, D0F0xBC_xE0003004_ADDRESS, AccessWidth32, &D0F0xBC_xE0003004.Value, StdHeader);
  } while (D0F0xBC_xE0003004.Field.IntDone == 0x0);
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuServiceRequestV4 Exit\n");
}
/*----------------------------------------------------------------------------------------*/
/**
 * SMU service request for S3 script
 *
 *
 * @param[in]  StdHeader       Standard configuration header
 * @param[in]  ContextLength   Context length
 * @param[in]  Context         Pointer to Context
 */

VOID
GnbSmuServiceRequestV4S3Script (
  IN      AMD_CONFIG_PARAMS     *StdHeader,
  IN      UINT16                ContextLength,
  IN      VOID                  *Context
  )
{
  SMU_MSG_CONTEXT   *SmuMsgContext;
  SmuMsgContext =  (SMU_MSG_CONTEXT *) Context;
  GnbSmuServiceRequestV4 (SmuMsgContext->GnbPciAddress, SmuMsgContext->RequestId, 0, StdHeader);
}

/*----------------------------------------------------------------------------------------*/
/**
 * SMU firmware download
 *
 *
 * @param[in]  GnbPciAddress   GNB Pci Address
 * @param[in]  Firmware        Pointer tp firmware
 * @param[in]  StdHeader       Standard configuration header
 */

AGESA_STATUS
GnbSmuFirmwareLoadV4 (
  IN       PCI_ADDR                 GnbPciAddress,
  IN       FIRMWARE_HEADER_V4       *Firmware,
  IN       AMD_CONFIG_PARAMS        *StdHeader
  )
{

  UINT32                     Index;
  D0F0xBC_xE00030A4_STRUCT  D0F0xBC_xE00030A4;
  D0F0xBC_xE0000004_STRUCT  D0F0xBC_xE0000004;
  D0F0xBC_xE0003088_STRUCT  D0F0xBC_xE0003088;
  D0F0xBC_x80010000_STRUCT  D0F0xBC_x80010000;
  D0F0xBC_x1F380_STRUCT      D0F0xBC_x1F380;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuFirmwareLoadV4 Enter\n");
  IDS_HDT_CONSOLE (NB_MISC, "  Firmware version 0x%x\n", Firmware->Version);
  IDS_OPTION_HOOK (IDS_REPORT_SMU_FW_VERSION, &(Firmware->Version), StdHeader);
  // Step 2, 10, make sure Rom firmware sequence is done
  do {
    GnbLibPciIndirectRead (GnbPciAddress.AddressValue | D0F0xB8_ADDRESS, D0F0xBC_xE0000004_ADDRESS, AccessWidth32, &D0F0xBC_xE0000004.Value, StdHeader);
  } while (D0F0xBC_xE0000004.Field.boot_seq_done == 0);
  // Step 1, check if firmware running in protected mode
  GnbLibPciIndirectRead (GnbPciAddress.AddressValue | D0F0xB8_ADDRESS, D0F0xBC_xE00030A4_ADDRESS, AccessWidth32, &D0F0xBC_xE00030A4.Value, StdHeader);
  if (D0F0xBC_xE00030A4.Field.SmuProtectedMode == 0) {
    // Step3, Clear firmware interrupt flags
    GnbLibPciIndirectRMW (
      GnbPciAddress.AddressValue | D0F0xB8_ADDRESS,
      D0F0xBC_x1F380_ADDRESS,
      AccessWidth32,
      0x0,
      0x0,
      StdHeader
      );
  }
  //Step 4, 11, Assert LM32 reset
  GnbLibPciIndirectRMW (
    GnbPciAddress.AddressValue | D0F0xB8_ADDRESS,
    D0F0xBC_x80000000_ADDRESS,
    AccessWidth32,
    (UINT32) ~(D0F0xBC_x80000000_lm32_rst_reg_MASK),
    1 << D0F0xBC_x80000000_lm32_rst_reg_OFFSET,
    StdHeader
    );
  // Step5, 12, Load firmware
  for (Index = 0; Index < (Firmware->FirmwareLength + Firmware->HeaderLength); Index++) {
    GnbLibPciIndirectWrite (GnbPciAddress.AddressValue | D0F0xB8_ADDRESS, SMC_RAM_START_ADDR + (Index * 4), AccessWidth32, &((UINT32 *) Firmware)[Index], StdHeader);
  }
  if (D0F0xBC_xE00030A4.Field.SmuProtectedMode == 0) {
    //Step 6, Write jmp to RAM firmware
    GnbLibPciIndirectRMW (
      GnbPciAddress.AddressValue | D0F0xB8_ADDRESS,
      0x0,
      AccessWidth32,
      0x0,
      0xE0000000 + ((SMC_RAM_START_ADDR + Firmware->HeaderLength * 4) >> 2),
      StdHeader
      );
  } else {
    //Step 13, Clear authentication done
    GnbLibPciIndirectRMW (
      GnbPciAddress.AddressValue | D0F0xB8_ADDRESS,
      D0F0xBC_xE0003088_ADDRESS,
      AccessWidth32,
      0x0,
      0x0,
      StdHeader
      );
  }
  // Step 7, 14 Enable LM32 clock
  GnbLibPciIndirectRMW (
    GnbPciAddress.AddressValue | D0F0xB8_ADDRESS,
    D0F0xBC_x80000004_ADDRESS,
    AccessWidth32,
    (UINT32) ~(D0F0xBC_x80000004_lm32_ck_disable_MASK),
    0 << D0F0xBC_x80000004_lm32_ck_disable_OFFSET,
    StdHeader
    );

  //Step 8, 15, Deassert LM32 reset
  GnbLibPciIndirectRMW (
    GnbPciAddress.AddressValue | D0F0xB8_ADDRESS,
    D0F0xBC_x80000000_ADDRESS,
    AccessWidth32,
    (UINT32) ~(D0F0xBC_x80000000_lm32_rst_reg_MASK),
    0 << D0F0xBC_x80000000_lm32_rst_reg_OFFSET,
    StdHeader
    );

  if (D0F0xBC_xE00030A4.Field.SmuProtectedMode == 1) {
    IDS_HDT_CONSOLE (NB_MISC, "  Protected mode: poll init authentication vector\n");
    // Step 16, Wait for rom firmware init authentication vector
    do {
      GnbLibPciIndirectRead (GnbPciAddress.AddressValue | D0F0xB8_ADDRESS, D0F0xBC_x80010000_ADDRESS, AccessWidth32, &D0F0xBC_x80010000.Value, StdHeader);
    } while (D0F0xBC_x80010000.Value != 0x400);
    // Call Authentication service
    GnbSmuServiceRequestV4 (GnbPciAddress, SMC_MSG_FIRMWARE_AUTH, 0, StdHeader);
    IDS_HDT_CONSOLE (NB_MISC, "  Protected mode: poll init authentication done\n");
    // Wait for authentication done
    do {
      GnbLibPciIndirectRead (GnbPciAddress.AddressValue | D0F0xB8_ADDRESS, D0F0xBC_xE0003088_ADDRESS, AccessWidth32, &D0F0xBC_xE0003088.Value, StdHeader);
    } while (D0F0xBC_xE0003088.Field.SmuAuthDone == 0x0);
    //Step 17, Check Authentication results
    if (D0F0xBC_xE0003088.Field.SmuAuthPass == 0) {
      IDS_HDT_CONSOLE (NB_MISC, "  ERROR!!!Authentication fail!!!\n");
      ASSERT (FALSE);
      return AGESA_FATAL;
    }
    // Step 18, Clear firmware interrupt enable flag
    GnbLibPciIndirectRMW (
      GnbPciAddress.AddressValue | D0F0xB8_ADDRESS,
      D0F0xBC_x1F380_ADDRESS,
      AccessWidth32,
      0x0,
      0x0,
      StdHeader
      );
    //Step 19, Assert LM32 reset
    GnbLibPciIndirectRMW (
      GnbPciAddress.AddressValue | D0F0xB8_ADDRESS,
      D0F0xBC_x80000000_ADDRESS,
      AccessWidth32,
      (UINT32) ~(D0F0xBC_x80000000_lm32_rst_reg_MASK),
      1 << D0F0xBC_x80000000_lm32_rst_reg_OFFSET,
      StdHeader
      );
    //Step 20, Deassert LM32 reset
    GnbLibPciIndirectRMW (
      GnbPciAddress.AddressValue | D0F0xB8_ADDRESS,
      D0F0xBC_x80000000_ADDRESS,
      AccessWidth32,
      (UINT32) ~(D0F0xBC_x80000000_lm32_rst_reg_MASK),
      0 << D0F0xBC_x80000000_lm32_rst_reg_OFFSET,
      StdHeader
      );
  }
//Step 9, 21 Wait firmware to initialize
  do {
    GnbLibPciIndirectRead (GnbPciAddress.AddressValue | D0F0xB8_ADDRESS, D0F0xBC_x1F380_ADDRESS, AccessWidth32, &D0F0xBC_x1F380.Value, StdHeader);
  } while (D0F0xBC_x1F380.Field.InterruptsEnabled == 0);
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuFirmwareLoadV4 Exit\n");
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get IOMMU PCI address
 *
 *
 * @param[in]  GnbHandle       GNB handle
 * @param[in]  StdHeader       Standard configuration header
 */

PCI_ADDR
GnbGetIommuPciAddressV4 (
  IN       GNB_HANDLE               *GnbHandle,
  IN       AMD_CONFIG_PARAMS        *StdHeader
  )
{
  PCI_ADDR  GnbIommuPciAddress;
  GnbIommuPciAddress = GnbGetHostPciAddress (GnbHandle);
  GnbIommuPciAddress.Address.Function = 0x2;
  return  GnbIommuPciAddress;
}


/*----------------------------------------------------------------------------------------*/
/**
 * UnitID Clumping
 *
 *
 * @param[in]  GnbHandle       GNB handle
 * @param[in]  StdHeader       Standard configuration header
 */

VOID
GnbClumpUnitIdV4 (
  IN      GNB_HANDLE                *GnbHandle,
  IN      AMD_CONFIG_PARAMS         *StdHeader
  )
{

  PCIe_ENGINE_CONFIG  *EngineList;
  UINT32              Value;

  Value = 0;
  EngineList = (PCIe_ENGINE_CONFIG *) PcieConfigGetChild (DESCRIPTOR_PCIE_ENGINE, &GnbHandle->Header);
  while (EngineList != NULL) {
    if (EngineList->Type.Port.NumberOfUnitId != 0) {
      if (!PcieConfigIsActivePcieEngine (EngineList)) {
        Value |= (((1 << EngineList->Type.Port.NumberOfUnitId) - 1) << EngineList->Type.Port.UnitId);
      } else {
        if (EngineList->Type.Port.NumberOfUnitId > 1) {
          Value |= (((1 << (EngineList->Type.Port.NumberOfUnitId - 1)) - 1) << (EngineList->Type.Port.UnitId + 1));
        }
      }
    }
    EngineList = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor (EngineList, DESCRIPTOR_TERMINATE_GNB);
  }
  // Set GNB
  GnbLibPciIndirectRMW (
    GnbHandle->Address.AddressValue | D0F0x94_ADDRESS,
    D0F0x98_x3A_ADDRESS | (1 << D0F0x94_OrbIndWrEn_OFFSET),
    AccessS3SaveWidth32,
    (UINT32) ~Value,
    Value,
    StdHeader
    );
  //Set UNB
  GnbLibPciRMW (
    MAKE_SBDFO (0, 0, GnbHandle->NodeId + 0x18, 0, D18F0x110_ADDRESS + GnbHandle->LinkId * 4),
    AccessS3SaveWidth32,
    (UINT32) ~Value,
    Value,
    StdHeader
    );
}



/*----------------------------------------------------------------------------------------*/
/**
 * Config GNB to prevent LPC deadlock scenario
 *
 *
 * @param[in]  GnbHandle       GNB handle
 * @param[in]  StdHeader       Standard configuration header
 */

VOID
GnbLpcDmaDeadlockPreventionV4 (
  IN       GNB_HANDLE               *GnbHandle,
  IN       AMD_CONFIG_PARAMS        *StdHeader
  )
{
  PCIe_PLATFORM_CONFIG  *Pcie;
  PCIe_ENGINE_CONFIG    *EngineList;

  Pcie = (PCIe_PLATFORM_CONFIG *) PcieConfigGetParent (DESCRIPTOR_PLATFORM, &GnbHandle->Header);
  EngineList = (PCIe_ENGINE_CONFIG *) PcieConfigGetChild (DESCRIPTOR_ALL_ENGINES, &GnbHandle->Header);
  while (EngineList != NULL) {
    if (PcieConfigIsPcieEngine (EngineList) && PcieConfigIsSbPcieEngine (EngineList)) {
      PcieRegisterRMW (
        PcieConfigGetParentWrapper (EngineList),
        CORE_SPACE (EngineList->Type.Port.CoreId, D0F0xE4_CORE_0010_ADDRESS),
        D0F0xE4_CORE_0010_UmiNpMemWrite_MASK,
        1 << D0F0xE4_CORE_0010_UmiNpMemWrite_OFFSET,
        TRUE,
        Pcie
        );
      //Enable special NP memory write protocol in ORB
      GnbLibPciIndirectRMW (
        GnbHandle->Address.AddressValue | D0F0x94_ADDRESS,
        D0F0x98_x06_ADDRESS | (1 << D0F0x94_OrbIndWrEn_OFFSET),
        AccessS3SaveWidth32,
        0xFFFFFFFF,
        1 << D0F0x98_x06_UmiNpMemWrEn_OFFSET,
        StdHeader
        );
      break;
    }
    EngineList = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor (EngineList, DESCRIPTOR_TERMINATE_GNB);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Enable IOMMU base address. (MMIO space )
 *
 *
 * @param[in]     GnbHandle       GNB handle
 * @param[in]     StdHeader       Standard Configuration Header
 * @retval        AGESA_SUCCESS
 * @retval        AGESA_ERROR
 */

AGESA_STATUS
GnbEnableIommuMmioV4 (
  IN       GNB_HANDLE           *GnbHandle,
  IN       AMD_CONFIG_PARAMS    *StdHeader
  )
{
  AGESA_STATUS            Status;
  UINT16                  CapabilityOffset;
  UINT64                  BaseAddress;
  UINT32                  Value;
  PCI_ADDR                GnbIommuPciAddress;

  Status = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEnableIommuMmio Enter\n");

  if (GnbFmCheckIommuPresent (GnbHandle, StdHeader)) {
    GnbIommuPciAddress = GnbGetIommuPciAddressV4 (GnbHandle, StdHeader);
    CapabilityOffset = GnbLibFindPciCapability (GnbIommuPciAddress.AddressValue, IOMMU_CAP_ID, StdHeader);

    GnbLibPciRead (GnbIommuPciAddress.AddressValue | (CapabilityOffset + 0x4), AccessWidth32, &Value, StdHeader);
    BaseAddress = (UINT64) Value << 32;
    GnbLibPciRead (GnbIommuPciAddress.AddressValue | (CapabilityOffset + 0x8), AccessWidth32, &Value, StdHeader);
    BaseAddress |= Value;

    if ((BaseAddress & 0xfffffffffffffffe) != 0x0) {
      IDS_HDT_CONSOLE (GNB_TRACE, "  Enable IOMMU MMIO at address %x for Socket %d Silicon %d\n", BaseAddress, GnbGetSocketId (GnbHandle) , GnbGetSiliconId (GnbHandle));
      GnbLibPciRMW (GnbIommuPciAddress.AddressValue | (CapabilityOffset + 0x8), AccessS3SaveWidth32, 0xFFFFFFFF, 0x0, StdHeader);
      GnbLibPciRMW (GnbIommuPciAddress.AddressValue | (CapabilityOffset + 0x4), AccessS3SaveWidth32, 0xFFFFFFFE, 0x1, StdHeader);
    } else {
      ASSERT (FALSE);
      Status = AGESA_ERROR;
    }
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEnableIommuMmio Exit\n");
  return Status;
}

