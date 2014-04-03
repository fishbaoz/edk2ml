/* $NoKeywords:$ */
/**
 * @file
 *
 * SMU Bios Table
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
#include  "amdlib.h"
#include  "heapManager.h"
#include  "Ids.h"
#include  "Gnb.h"
#include  "OptionGnb.h"
#include  "GnbFuseTable.h"
#include  "GnbPcieConfig.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbCommonLib.h"
#include  "GnbGfxFamServices.h"
#include  "GnbRegisterAccML.h"
#include  "GnbRegistersML.h"
#include  "GnbSmuInitLibV7.h"
#include  "GnbSmuBiosTableML.h"
#include  "GnbNbInitLibV5.h"
#include  "mm.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITML_GNBSMUBIOSTABLEML_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern BUILD_OPT_CFG UserOptions;

#define NUM_DPM_STATES   8
#define NUM_FUSED_STATES 5

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Init SMU Bios Table
 *
 * @param[in]  StdHeader  Standard configuration header
 */

VOID
GnbInitSmuBiosTableML (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AMD_EARLY_PARAMS        *EarlyParams;
  GNB_HANDLE              *GnbHandle;
  SMU7_BIOSTABLE          *BiosTable;
  UINT32                  *Ptr;
  D0F0xBC_x20054_STRUCT   D0F0xBC_x20054;
  UINT32                  Addr;
  UINT32                  Value;
  UINT8                   Index;

  EarlyParams = (AMD_EARLY_PARAMS *) StdHeader;
  GnbHandle = GnbGetHandle (StdHeader);

  BiosTable = (SMU7_BIOSTABLE *) GnbAllocateHeapBuffer (AMD_GNB_SMU_TABLE_HANDLE, sizeof (SMU7_BIOSTABLE), StdHeader);
  ASSERT (BiosTable != NULL);

  if (BiosTable != NULL) {

    GnbRegisterReadML (GnbHandle, TYPE_D0F0xBC, D0F0xBC_x20054_ADDRESS,
                        &D0F0xBC_x20054.Value, 0, StdHeader);
    Addr = D0F0xBC_x20054.Value;
    ASSERT (Addr != NULL);

    IDS_HDT_CONSOLE (GNB_TRACE, "Read BiosTable = %08x with %08x bytes\n",
        BiosTable, sizeof (SMU7_BIOSTABLE));
    IDS_HDT_CONSOLE (GNB_TRACE, "  Start addr: %08x\n", Addr);

    Ptr = (UINT32 *) BiosTable;
    for (Index = 0; Index < (sizeof (SMU7_BIOSTABLE) / 4); Index++) {
      GnbRegisterReadML (GnbHandle, TYPE_D0F0xBC, Addr, &Value,
          0, StdHeader);
      *(Ptr + Index) = Value;
      IDS_HDT_CONSOLE (GNB_TRACE, "  Addr: %08x Value: %08x Dest: %08x\n",
          Addr, Value, Ptr + Index);
      Addr += 4;
    }

    BiosTable->TdcVrmLimit_Idd = (UINT16) (EarlyParams->PlatformConfig.VrmProperties[CoreVrm].CurrentLimit / 10);
    BiosTable->TdcVrmLimit_IddNb = (UINT16) (EarlyParams->PlatformConfig.VrmProperties[NbVrm].CurrentLimit / 10);
    BiosTable->Iddspike_OCP = (UINT16) (EarlyParams->PlatformConfig.VrmProperties[CoreVrm].SviOcpLevel / 10);
    BiosTable->IddNbspike_OCP = (UINT16) (EarlyParams->PlatformConfig.VrmProperties[NbVrm].SviOcpLevel / 10);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * SMU BIOS table
 *
 *
 * @param[in]  StdHeader       Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbWriteSmuBiosTableML (
  IN       AMD_CONFIG_PARAMS        *StdHeader
  )
{
  SMU7_BIOSTABLE          *BiosTable;
  GNB_HANDLE              *GnbHandle;
  D0F0xBC_x20054_STRUCT   D0F0xBC_x20054;
  UINT32                  Addr;
  UINT32                  Index;
  UINT32                  *Ptr;

  GnbHandle = GnbGetHandle (StdHeader);

  GnbRegisterReadML (GnbHandle, TYPE_D0F0xBC, D0F0xBC_x20054_ADDRESS,
                      &D0F0xBC_x20054.Value, 0, StdHeader);
  Addr = D0F0xBC_x20054.Value;
  ASSERT (Addr != NULL);

  BiosTable = (SMU7_BIOSTABLE *) GnbLocateHeapBuffer (AMD_GNB_SMU_TABLE_HANDLE, StdHeader);
  ASSERT (BiosTable != NULL);

  IDS_HDT_CONSOLE (GNB_TRACE, "Write BiosTable = %08x with %08x bytes\n",
      BiosTable, sizeof (SMU7_BIOSTABLE));
  IDS_HDT_CONSOLE (GNB_TRACE, "  BiosTable[0] =   %02X\n", *((UINT8 *) BiosTable));

  if (BiosTable != NULL) {

    Ptr = (UINT32 *) BiosTable;
    for (Index = 0; Index < (sizeof (SMU7_BIOSTABLE) / 4); Index++) {
      GnbRegisterWriteML (GnbHandle, TYPE_D0F0xBC, Addr, Ptr + Index, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
      IDS_HDT_CONSOLE (GNB_TRACE, "  Addr: %08x Value: %08x Dest: %08x\n", Addr, *(Ptr + Index), Ptr + Index);
      Addr += 4;
    }

  }

  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get system configuration information for SMU BIOS table
 *
 * @param[in]  StdHeader  Standard configuration header
 */
VOID
GnbSmuBiosTableGetSystemConfigInfoML (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  SMU7_BIOSTABLE          *BiosTable;
  GNB_BUILD_OPTIONS_ML    *GnbBuildOptionData;
  AMD_POST_PARAMS         *PostParamsPtr;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuBiosTableGetSystemConfigInfoML Enter\n");

  BiosTable = (SMU7_BIOSTABLE *) GnbLocateHeapBuffer (AMD_GNB_SMU_TABLE_HANDLE, StdHeader);
  ASSERT (BiosTable != NULL);
  PostParamsPtr = (AMD_POST_PARAMS *) StdHeader;

  IDS_HDT_CONSOLE (GNB_TRACE, "  TdpDcUndocked: %x\n", UserOptions.CfgTdpDcUnDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  TdpAcUndocked: %x\n", UserOptions.CfgTdpAcUnDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  TdpDcDocked: %x\n", UserOptions.CfgTdpDcDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  TdpAcDocked: %x\n", UserOptions.CfgTdpAcDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  StapmEnableDcUndocked: %x\n", UserOptions.CfgStapmEnDcUnDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  StapmEnableAcUndocked: %x\n", UserOptions.CfgStapmEnAcUnDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  StapmEnableDcDocked: %x\n", UserOptions.CfgStapmEnDcDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  StapmEnableAcDocked: %x\n", UserOptions.CfgStapmEnAcDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  StapmPowerDcUndocked: %x\n", UserOptions.CfgStapmPowerDcUnDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  StapmPowerAcUndocked: %x\n", UserOptions.CfgStapmPowerAcUnDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  StapmPowerDcDocked: %x\n", UserOptions.CfgStapmPowerDcDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  StapmPowerAcDocked: %x\n", UserOptions.CfgStapmPowerAcDocked);

  BiosTable->TdpDcUndocked = UserOptions.CfgTdpDcUnDocked;
  BiosTable->TdpAcUndocked = UserOptions.CfgTdpAcUnDocked;
  BiosTable->TdpDcDocked = UserOptions.CfgTdpDcDocked;
  BiosTable->TdpAcDocked = UserOptions.CfgTdpAcDocked;

  BiosTable->NumGfxCUsEnabled = UserOptions.CfgNumGfxCoresEnabled;
  BiosTable->BatteryBoostTune = UserOptions.CfgBatteryBoostTune;
  BiosTable->TspAcDocked = UserOptions.CfgTspAcDocked;
  BiosTable->TspDcDocked = UserOptions.CfgTspDcDocked;
  BiosTable->TspAcUndocked = UserOptions.CfgTspAcUnDocked;
  BiosTable->TspDcUndocked = UserOptions.CfgTspDcUnDocked;

  BiosTable->StapmEnableDcUndocked = UserOptions.CfgStapmEnDcUnDocked;
  BiosTable->StapmEnableAcUndocked = UserOptions.CfgStapmEnAcUnDocked;
  BiosTable->StapmEnableDcDocked = UserOptions.CfgStapmEnDcDocked;
  BiosTable->StapmEnableAcDocked = UserOptions.CfgStapmEnAcDocked;

  BiosTable->StapmPowerDcUndocked = UserOptions.CfgStapmPowerDcUnDocked;
  BiosTable->StapmPowerAcUndocked = UserOptions.CfgStapmPowerAcUnDocked;
  BiosTable->StapmPowerDcDocked = UserOptions.CfgStapmPowerDcDocked;
  BiosTable->StapmPowerAcDocked = UserOptions.CfgStapmPowerAcDocked;
  // SMU expects TemperatureLimit with 8 fractional bits
  // BLDCFG is defined in tenths of degrees C
  BiosTable->TemperatureLimit = (UINT16) ((((UINT32) PostParamsPtr->PlatformConfig.HtcTemperatureLimit) * 256) / 10);

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  // Set SVI mode
  if (GnbBuildOptionData->CfgSviRevision == 2) {
    BiosTable->SVI1 = 0;
  } else {
    BiosTable->SVI1 = 1;
  }

  IDS_OPTION_HOOK (IDS_GNB_SMU_BIOS_TABLE, BiosTable, StdHeader);
  IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_SMUBIOSTABLE_OVERRIDE, BiosTable, StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "  After IDS HOOK - TdpDcUndocked: %x\n", BiosTable->TdpDcUndocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  After IDS HOOK - TdpAcUndocked: %x\n", BiosTable->TdpAcUndocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  After IDS HOOK - TdpDcDocked: %x\n", BiosTable->TdpDcDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  After IDS HOOK - TdpAcDocked: %x\n", BiosTable->TdpAcDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  After IDS HOOK - StapmEnableDcUndocked: %x\n", BiosTable->StapmEnableDcUndocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  After IDS HOOK - StapmEnableAcUndocked: %x\n", BiosTable->StapmEnableAcUndocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  After IDS HOOK - StapmEnableDcDocked: %x\n", BiosTable->StapmEnableDcDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  After IDS HOOK - StapmEnableAcDocked: %x\n", BiosTable->StapmEnableAcDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  After IDS HOOK - StapmPowerDcUndocked: %x\n", BiosTable->StapmPowerDcUndocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  After IDS HOOK - StapmPowerAcUndocked: %x\n", BiosTable->StapmPowerAcUndocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  After IDS HOOK - StapmPowerDcDocked: %x\n", BiosTable->StapmPowerDcDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  After IDS HOOK - StapmPowerAcDocked: %x\n", BiosTable->StapmPowerAcDocked);
  IDS_HDT_CONSOLE (GNB_TRACE, "  SMU HtcTemperatureLimit = %08x\n", BiosTable->TemperatureLimit);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuBiosTableGetSystemConfigInfoML Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get Pcie lane info.
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper configuration descriptor
 * @param[in]  Buffer          Pointer buffer
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */
AGESA_STATUS
STATIC
GetPcieLaneInfoCallbackML (
  IN       PCIe_WRAPPER_CONFIG           *Wrapper,
  IN OUT   VOID                          *Buffer,
  IN       PCIe_PLATFORM_CONFIG          *Pcie
  )
{
  UINT32      ActiveLaneBitmap;
  UINT32      *DataBuffer;

  DataBuffer = (UINT32 *) Buffer;
  ActiveLaneBitmap = PcieUtilGetWrapperLaneBitMap (LANE_TYPE_PCIE_PHY_NATIVE_ALLOC_ACTIVE, 0, Wrapper);
  *DataBuffer = ActiveLaneBitmap;
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get Pcie information for BIOS table
 *
 * @param[in]  StdHeader  Standard configuration header
 * @param[in]  Pcie       Pointer to global PCIe configuration
 */
VOID
GnbSmuBiosTableGetPcieInfoML (
  IN      AMD_CONFIG_PARAMS               *StdHeader,
  IN      PCIe_PLATFORM_CONFIG            *Pcie
  )
{
  AGESA_STATUS          AgesaStatus;
  AGESA_STATUS          Status;
  SMU7_BIOSTABLE        *BiosTable;
  UINT32                ActiveLaneBitMap;

  AgesaStatus = AGESA_SUCCESS;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuBiosTableGetPcieInfoML Enter\n");
  BiosTable = (SMU7_BIOSTABLE *) GnbLocateHeapBuffer (AMD_GNB_SMU_TABLE_HANDLE, StdHeader);
  ASSERT (BiosTable != NULL);

  Status = PcieConfigRunProcForAllWrappers (DESCRIPTOR_ALL_WRAPPERS, GetPcieLaneInfoCallbackML, &ActiveLaneBitMap, Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  BiosTable->PcieLanesActiveMap = (UINT8) ActiveLaneBitMap;
  IDS_HDT_CONSOLE (GNB_TRACE, "BiosTable - PcieLanesActiveMap = %08x\n", BiosTable->PcieLanesActiveMap);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuBiosTableGetPcieInfoML Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get memory information for BIOS table
 *
 * @param[in]  StdHeader  Standard configuration header
 */
VOID
GnbSmuBiosTableGetDimmInfoML (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  SMU7_BIOSTABLE          *BiosTable;
  DIE_STRUCT              *MCTPtr;
  MEM_DATA_STRUCT         *MemDataPtr;
  UINT32                  VddIox100;
  UINT32                  Vddrx100;
  UINT8                   offset;
  GNB_HANDLE              *GnbHandle;
  UINT32                  CSEnable;
  UINT32                  Index;
  UINT32                  DllBypassMode;
  UINT32                  LowPowerRecMode;

  IDS_HDT_CONSOLE (GNB_TRACE, "Get Dimm information for BIOS table for SMU\n");

  GnbHandle = GnbGetHandle (StdHeader);
  BiosTable = (SMU7_BIOSTABLE *) GnbLocateHeapBuffer (AMD_GNB_SMU_TABLE_HANDLE, StdHeader);
  ASSERT (BiosTable != NULL);

  MCTPtr = (DIE_STRUCT *) GnbLocateHeapBuffer (GENERATE_MEM_HANDLE (ALLOC_DIE_STRUCT_HANDLE, 0, 0, 0), StdHeader);
  ASSERT (MCTPtr != NULL);

  MemDataPtr = (MEM_DATA_STRUCT *) GnbLocateHeapBuffer (AMD_MEM_DATA_HANDLE, StdHeader);
  ASSERT (MemDataPtr != NULL);

  IDS_HDT_CONSOLE (GNB_TRACE, "Valid Dimm Mask: %04x\n", MCTPtr->DimmValid);
  BiosTable->Dimms = 0;

  // Get number of dimms
  for (offset = 0; offset < 16; offset ++) {
    if ((MCTPtr->DimmValid & ((UINT32)1 << offset)) != 0) {
      BiosTable->Dimms ++;
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "Number of Dimms: %d\n", BiosTable->Dimms);

  // Get memory frequency
  BiosTable->MemorySpeedLow = MCTPtr->DctData[0].Timings.Speed & 0xFFFF;
  BiosTable->MemorySpeedHigh = MCTPtr->DctData[0].TimingsMemPs1->Speed & 0xFFFF;
  IDS_HDT_CONSOLE (GNB_TRACE, "Memory Speed Low: %dMHz\n", BiosTable->MemorySpeedLow);
  IDS_HDT_CONSOLE (GNB_TRACE, "Memory Speed High: %dMHz\n", BiosTable->MemorySpeedHigh);

  IDS_HDT_CONSOLE (GNB_TRACE, "Ddr3 Voltage: %08x\n", MemDataPtr->ParameterListPtr->DDR3Voltage);

  // Get VDDIO
  // Take the voltage in volt and multiply it by 2^15
  VddIox100 = 150;
  switch (MemDataPtr->ParameterListPtr->DDR3Voltage) {
  case VOLT1_5:
    VddIox100 = 150;
    break;
  case VOLT1_35:
    VddIox100 = 135;
    break;
  case VOLT1_25:
    VddIox100 = 125;
    break;
  default:
    ASSERT (FALSE);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "VDDIO: %d ", VddIox100);
  VddIox100 = VddIox100 << 15;
  // Need to round to nearest
  BiosTable->Vddio = (UINT16) (VddIox100 / 100);
  if ((VddIox100 % 100) >= 50) {
    BiosTable->Vddio ++;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%d\n", BiosTable->Vddio);

  // Get VDDR
  // Format is the same as VDDIO
  Vddrx100 = 95;
  if (UserOptions.CfgMemoryPhyVoltage == VOLT0_95) {
    Vddrx100 = 95;
  }
  ASSERT (UserOptions.CfgMemoryPhyVoltage < MAX_VDDR);
  IDS_HDT_CONSOLE (GNB_TRACE, "VDDR: %d ", Vddrx100);
  Vddrx100 <<= 15;
  BiosTable->Vddr = (UINT16) (Vddrx100 / 100);
  if ((Vddrx100 % 100) >= 50) {
    BiosTable->Vddr ++;
  }

  BiosTable->DimmsConfig = 0;
  for (Index = 0; Index < 4; Index++) {
    GnbRegisterReadML (GnbHandle, TYPE_D18F2, 0x40 + Index * 4, &CSEnable, 0, StdHeader);
    if (CSEnable & BIT0) {
      BiosTable->DimmsConfig |= 1 << Index;
    }
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "%d\n", BiosTable->Vddr);

  GnbRegisterReadML (GnbHandle, D18F2x9C_x0D0F_001E_dct0_mp0_TYPE, D18F2x9C_x0D0F_001E_dct0_mp0_ADDRESS, &DllBypassMode, 0, StdHeader);
  if (DllBypassMode & BIT7) {
    BiosTable->DllBypassMode = TRUE;
  }

  GnbRegisterReadML (GnbHandle, D18F2x9C_x0D0F_001F_dct0_mp0_TYPE, D18F2x9C_x0D0F_001F_dct0_mp0_ADDRESS, &LowPowerRecMode, 0, StdHeader);
  if (LowPowerRecMode & BIT14) {
    BiosTable->LowPowerRecMode_mp0 = TRUE;
  }

  GnbRegisterReadML (GnbHandle, D18F2x9C_x0D0F_001F_dct0_mp1_TYPE, D18F2x9C_x0D0F_001F_dct0_mp1_ADDRESS, &LowPowerRecMode, 0, StdHeader);
  if (LowPowerRecMode & BIT14) {
    BiosTable->LowPowerRecMode_mp1 = TRUE;
  }


  IDS_HDT_CONSOLE (GNB_TRACE, "Low Power DDR Phy Mode: %d ", BiosTable->Reserved[0]);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Check if fused state valid
 *
 *
 * @param[out]      Index             State index
 * @param[in]       PpFuses           Pointer to fuse table
 * @retval          TRUE              State is valid
 */
BOOLEAN
GnbIsFusedStateValidML (
  IN      UINT8                         Index,
  IN      PP_FUSE_ARRAY_V2              *PpFuses
  )
{
  BOOLEAN Result;
  Result = FALSE;

  if (Index >= NUM_FUSED_STATES) {
    Result = FALSE;
  } else if ((PpFuses->PerfFlag & (1 << Index)) ||
             (PpFuses->PowerSavingsFlag & (1 << Index))) {
    Result = TRUE;
  }
  return Result;
}


