/**
 * @file
 *
 * GNB init tables
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
#include  "Gnb.h"
#include  "GnbPcie.h"
#include  "GnbCommonLib.h"
#include  "GnbTable.h"
#include  "GnbRegistersML.h"
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


/*----------------------------------------------------------------------------------------
 *           T A B L E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

GNB_TABLE ROMDATA GnbEarlierInitTableBeforeSmuML [] = {
  GNB_ENTRY_RMW (
    D0F0x98_x07_TYPE,
    D0F0x98_x07_ADDRESS,
    D0F0x98_x07_SMUCsrIsocEn_MASK,
    (1 << D0F0x98_x07_SMUCsrIsocEn_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x1E_TYPE,
    D0F0x98_x1E_ADDRESS,
    D0F0x98_x1E_HiPriEn_MASK,
    (1 << D0F0x98_x1E_HiPriEn_OFFSET)
    ),
  GNB_ENTRY_TERMINATE
};

GNB_TABLE ROMDATA GnbEarlyInitTableML [] = {
  // Set SVI2
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_SVI2,
    D0F0xBC_x3F9D8_TYPE,
    D0F0xBC_x3F9D8_ADDRESS,
    D0F0xBC_x3F9D8_SviMode_MASK,
    (1 << D0F0xBC_x3F9D8_SviMode_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F0x64_x16_TYPE,
    D0F0x64_x16_ADDRESS,
    D0F0x64_x16_AerUrMsgEn_MASK,
    0x0 << D0F0x64_x16_AerUrMsgEn_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x07_TYPE,
    D0F0x98_x07_ADDRESS,
    D0F0x98_x07_IocBwOptEn_MASK | D0F0x98_x07_DropZeroMaskWrEn_MASK,
    (1 << D0F0x98_x07_IocBwOptEn_OFFSET) | (1 << D0F0x98_x07_DropZeroMaskWrEn_OFFSET)
    ),

  GNB_ENTRY_RMW (
    D0F0x98_x0C_TYPE,
    D0F0x98_x0C_ADDRESS,
    D0F0x98_x0C_GcmWrrLenA_MASK | D0F0x98_x0C_GcmWrrLenB_MASK,
    (0x8 << D0F0x98_x0C_GcmWrrLenA_OFFSET) | (0x8 << D0F0x98_x0C_GcmWrrLenB_OFFSET)
    ),

//---------------------------------------------------------------------------
// Set SB location for IOMMU
  GNB_ENTRY_COPY (
    D0F2xFC_x09_L1_TYPE,
    D0F2xFC_x09_L1_ADDRESS (L1_SEL_SBG),
    0,
    15,
    D0F0x64_x1F_TYPE,
    D0F0x64_x1F_ADDRESS,
    0,
    15
    ),
  GNB_ENTRY_COPY (
    D0F2xF4_x49_TYPE,
    D0F2xF4_x49_ADDRESS,
    0,
    32,
    D0F0x64_x1F_TYPE,
    D0F0x64_x1F_ADDRESS,
    0,
    32
    ),
  //  CSR_GNB_1.SviTrimValueVdd = Vdd Trim
  GNB_ENTRY_COPY (
    D0F0xBC_x3F9F4_TYPE,
    D0F0xBC_x3F9F4_ADDRESS,
    D0F0xBC_x3F9F4_SviTrimValueVdd_OFFSET, D0F0xBC_x3F9F4_SviTrimValueVdd_WIDTH,
    D18F5x12C_TYPE,
    D18F5x12C_ADDRESS,
    D18F5x12C_CoreLoadLineTrim_OFFSET, D18F5x12C_CoreLoadLineTrim_WIDTH
  ),
  //  CSR_GNB_1.SviTrimValueVddNB = VddNB Trim
  GNB_ENTRY_COPY (
      D0F0xBC_x3F9F4_TYPE,
      D0F0xBC_x3F9F4_ADDRESS,
      D0F0xBC_x3F9F4_SviTrimValueVddNB_OFFSET, D0F0xBC_x3F9F4_SviTrimValueVddNB_WIDTH,
      D18F5x188_TYPE,
      D18F5x188_ADDRESS,
      D18F5x188_NbLoadLineTrim_OFFSET, D18F5x188_NbLoadLineTrim_WIDTH
  ),
  //  CSR_GNB_3.SviLoadLineOffsetVdd = Vdd Offset
  GNB_ENTRY_COPY (
    D0F0xBC_x3F9F8_TYPE,
    D0F0xBC_x3F9F8_ADDRESS,
    D0F0xBC_x3F9F8_SviLoadLineOffsetVdd_OFFSET, D0F0xBC_x3F9F8_SviLoadLineOffsetVdd_WIDTH,
    D18F5x12C_TYPE,
    D18F5x12C_ADDRESS,
    D18F5x12C_CoreOffsetTrim_OFFSET, D18F5x12C_CoreOffsetTrim_WIDTH
  ),
  //  CSR_GNB_3.SviLoadLineOffsetVddNB = VddNB Offset
  GNB_ENTRY_COPY (
    D0F0xBC_x3F9F8_TYPE,
    D0F0xBC_x3F9F8_ADDRESS,
    D0F0xBC_x3F9F8_SviLoadLineOffsetVddNB_OFFSET, D0F0xBC_x3F9F8_SviLoadLineOffsetVddNB_WIDTH,
    D18F5x188_TYPE,
    D18F5x188_ADDRESS,
    D18F5x188_NbOffsetTrim_OFFSET, D18F5x188_NbOffsetTrim_WIDTH
  ),
  GNB_ENTRY_TERMINATE
};

GNB_TABLE ROMDATA GnbEnvInitTableML [] = {
//---------------------------------------------------------------------------
// ORB Init
  GNB_ENTRY_RMW (
    D0F0x98_x07_TYPE,
    D0F0x98_x07_ADDRESS,
    D0F0x98_x07_IocBwOptEn_MASK | D0F0x98_x07_DropZeroMaskWrEn_MASK,
    (0x1 << D0F0x98_x07_IocBwOptEn_OFFSET) | (0x1 << D0F0x98_x07_DropZeroMaskWrEn_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x07_TYPE,
    D0F0x98_x07_ADDRESS,
    D0F0x98_x07_IommuBwOptEn_MASK | D0F0x98_x07_IommuIsocPassPWMode_MASK | D0F0x98_x07_DmaReqRespPassPWMode_MASK,
    (0x1 << D0F0x98_x07_IommuBwOptEn_OFFSET) | (0x1 << D0F0x98_x07_IommuIsocPassPWMode_OFFSET) | (0 << D0F0x98_x07_DmaReqRespPassPWMode_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x08_TYPE,
    D0F0x98_x08_ADDRESS,
    D0F0x98_x08_NpWrrLenC_MASK,
    0x1 << D0F0x98_x08_NpWrrLenC_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x28_TYPE,
    D0F0x98_x28_ADDRESS,
    D0F0x98_x28_ForceCoherentIntr_MASK,
    0x1 << D0F0x98_x28_ForceCoherentIntr_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x2C_TYPE,
    D0F0x98_x2C_ADDRESS,
    D0F0x98_x2C_SBDmaActiveMask_MASK | D0F0x98_x2C_CgttLclkOverride_MASK,
    (1 << D0F0x98_x2C_SBDmaActiveMask_OFFSET) | (1 << D0F0x98_x2C_CgttLclkOverride_OFFSET)
    ),
  // Enable gBIF UID Clumping [BIT 23:21 = 011b]
  GNB_ENTRY_RMW (
    D0F0x98_x3A_TYPE,
    D0F0x98_x3A_ADDRESS,
    0x00E00000,
    0x00600000
    ),
  GNB_ENTRY_RMW (
    D18F0x110_TYPE,
    D18F0x110_ADDRESS,
    0x00E00000,
    0x00600000
    ),
  //NB P-state Configuration for Runtime
  GNB_ENTRY_RMW (
    D0F0xBC_x3F9E8_TYPE,
    D0F0xBC_x3F9E8_ADDRESS,
    D0F0xBC_x3F9E8_Dpm0PgNbPsLo_MASK | D0F0xBC_x3F9E8_Dpm0PgNbPsHi_MASK |
    D0F0xBC_x3F9E8_DpmXNbPsLo_MASK | D0F0xBC_x3F9E8_DpmXNbPsHi_MASK,
    (3 << D0F0xBC_x3F9E8_Dpm0PgNbPsLo_OFFSET) | (0 << D0F0xBC_x3F9E8_Dpm0PgNbPsHi_OFFSET) |
    (2 << D0F0xBC_x3F9E8_DpmXNbPsLo_OFFSET) | (1 << D0F0xBC_x3F9E8_DpmXNbPsHi_OFFSET)
    ),
  // Programming a conservative watermark for NBP states
  // [INST0]
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_GMC_STUTTER_ENABLED,
    GMMx6CD8_TYPE,
    GMMx6CD8_ADDRESS,
    GMMx6CD8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_MASK |
    GMMx6CD8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_MASK |
    GMMx6CD8_NB_PSTATE_CHANGE_WATERMARK_MASK,
    (1 << GMMx6CD8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_OFFSET) |
    (1 << GMMx6CD8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_OFFSET) |
    (0x7FFF << GMMx6CD8_NB_PSTATE_CHANGE_WATERMARK_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_GMC_STUTTER_ENABLED,
    GMMx6CC8_TYPE,
    GMMx6CC8_ADDRESS,
    GMMx6CC8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_MASK |
    GMMx6CC8_NB_PSTATE_CHANGE_WATERMARK_MASK_MASK,
    (0x3 << GMMx6CC8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_OFFSET) |
    (0x3 << GMMx6CC8_NB_PSTATE_CHANGE_WATERMARK_MASK_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_GMC_STUTTER_ENABLED,
    GMMx6CD4_TYPE,
    GMMx6CD4_ADDRESS,
    GMMx6CD4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK,
    (0x7FFF << GMMx6CD4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_GMC_STUTTER_ENABLED,
    GMMx6CD4_TYPE,
    GMMx6CD4_ADDRESS,
    GMMx6CD4_STUTTER_ENABLE_MASK,
    (1 << GMMx6CD4_STUTTER_ENABLE_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_GMC_STUTTER_ENABLED,
    GMMx6CD8_TYPE,
    GMMx6CD8_ADDRESS,
    GMMx6CD8_NB_PSTATE_CHANGE_ENABLE_MASK,
    (1 << GMMx6CD8_NB_PSTATE_CHANGE_ENABLE_OFFSET)
    ),
  // [INST1]
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_GMC_STUTTER_ENABLED,
    GMMx78D8_TYPE,
    GMMx78D8_ADDRESS,
    GMMx78D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_MASK |
    GMMx78D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_MASK |
    GMMx78D8_NB_PSTATE_CHANGE_WATERMARK_MASK,
    (1 << GMMx78D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_OFFSET) |
    (1 << GMMx78D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_OFFSET) |
    (0x7FFF << GMMx78D8_NB_PSTATE_CHANGE_WATERMARK_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_GMC_STUTTER_ENABLED,
    GMMx78C8_TYPE,
    GMMx78C8_ADDRESS,
    GMMx78C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_MASK |
    GMMx78C8_NB_PSTATE_CHANGE_WATERMARK_MASK_MASK,
    (0x3 << GMMx78C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_OFFSET) |
    (0x3 << GMMx78C8_NB_PSTATE_CHANGE_WATERMARK_MASK_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_GMC_STUTTER_ENABLED,
    GMMx78D4_TYPE,
    GMMx78D4_ADDRESS,
    GMMx78D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK,
    (0x7FFF << GMMx78D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_GMC_STUTTER_ENABLED,
    GMMx78D4_TYPE,
    GMMx78D4_ADDRESS,
    GMMx78D4_STUTTER_ENABLE_MASK,
    (1 << GMMx78D4_STUTTER_ENABLE_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_GMC_STUTTER_ENABLED,
    GMMx78D8_TYPE,
    GMMx78D8_ADDRESS,
    GMMx78D8_NB_PSTATE_CHANGE_ENABLE_MASK,
    (1 << GMMx78D8_NB_PSTATE_CHANGE_ENABLE_OFFSET)
    ),

  GNB_ENTRY_TERMINATE
};

GNB_TABLE ROMDATA GnbMidInitTableML [] = {
//---------------------------------------------------------------------------
// ORB clock gating
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_ORB_CLK_GATING,
    D0F0x98_x49_TYPE,
    D0F0x98_x49_ADDRESS,
    D0F0x98_x49_SoftOverrideClk6_MASK | D0F0x98_x49_SoftOverrideClk5_MASK | D0F0x98_x49_SoftOverrideClk4_MASK | D0F0x98_x49_SoftOverrideClk3_MASK | D0F0x98_x49_SoftOverrideClk2_MASK | D0F0x98_x49_SoftOverrideClk1_MASK | D0F0x98_x49_SoftOverrideClk0_MASK,
    0x0
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_ORB_CLK_GATING,
    D0F0x98_x4A_TYPE,
    D0F0x98_x4A_ADDRESS,
    D0F0x98_x4A_SoftOverrideClk6_MASK | D0F0x98_x4A_SoftOverrideClk5_MASK | D0F0x98_x4A_SoftOverrideClk4_MASK | D0F0x98_x4A_SoftOverrideClk3_MASK | D0F0x98_x4A_SoftOverrideClk2_MASK | D0F0x98_x4A_SoftOverrideClk1_MASK | D0F0x98_x4A_SoftOverrideClk0_MASK,
    0x0
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_ORB_CLK_GATING,
    D0F0x98_x2C_TYPE,
    D0F0x98_x2C_ADDRESS,
    D0F0x98_x2C_CgttLclkOverride_MASK,
    0x0
    ),
//---------------------------------------------------------------------------
// IOC clock gating
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOC_LCLK_CLOCK_GATING,
    D0F0x64_x22_TYPE,
    D0F0x64_x22_ADDRESS,
    D0F0x64_x22_SoftOverrideClk6_MASK | D0F0x64_x22_SoftOverrideClk4_MASK | D0F0x64_x22_SoftOverrideClk3_MASK | D0F0x64_x22_SoftOverrideClk2_MASK | D0F0x64_x22_SoftOverrideClk1_MASK | D0F0x64_x22_SoftOverrideClk0_MASK,
    0x0
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOC_LCLK_CLOCK_GATING,
    D0F0x64_x23_TYPE,
    D0F0x64_x23_ADDRESS,
    D0F0x64_x23_SoftOverrideClk4_MASK | D0F0x64_x23_SoftOverrideClk3_MASK | D0F0x64_x23_SoftOverrideClk2_MASK | D0F0x64_x23_SoftOverrideClk1_MASK | D0F0x64_x23_SoftOverrideClk0_MASK,
    0x0
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOC_LCLK_CLOCK_GATING,
    D0F0x64_x46_TYPE,
    D0F0x64_x46_ADDRESS,
    D0F0x64_x46_CgttLclkOverride_MASK,
    0x0
    ),
//---------------------------------------------------------------------------
// SPG clock gating
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_SPG_CLOCK_GATING,
    D8F0xEC_x4A_TYPE,
    D8F0xEC_x4A_ADDRESS,
    D8F0xEC_x4A_Reserved_30_24_MASK,
    0x0
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_SPG_CLOCK_GATING,
    D8F0xEC_x4C_TYPE,
    D8F0xEC_x4C_ADDRESS,
    D8F0xEC_x4C_Reserved_30_24_MASK,
    0x0
    ),
  //---------------------------------------------------------------------------
  // LCLK Activity Monitor init
    GNB_ENTRY_RMW (
      D0F0xBC_xC0200110_TYPE,
      D0F0xBC_xC0200110_ADDRESS,
      D0F0xBC_xC0200110_BusyCntSel_MASK | D0F0xBC_xC0200110_ActivityCntRst_MASK |
      D0F0xBC_xC0200110_PeriodCntRst_MASK | D0F0xBC_xC0200110_EnOrbUsCnt_MASK |
      D0F0xBC_xC0200110_EnOrbDsCnt_MASK,
      (0x3 << D0F0xBC_xC0200110_BusyCntSel_OFFSET)   |
      (0 << D0F0xBC_xC0200110_ActivityCntRst_OFFSET) |
      (0x0 << D0F0xBC_xC0200110_PeriodCntRst_OFFSET) |
      (0x1 << D0F0xBC_xC0200110_EnOrbUsCnt_OFFSET)   |
      (0x1 << D0F0xBC_xC0200110_EnOrbDsCnt_OFFSET)
      ),
//---------------------------------------------------------------------------
// Enable LCLK Deep Sleep
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_LCLK_DEEP_SLEEP,
    D0F0xBC_xC0200310_TYPE,
    D0F0xBC_xC0200310_ADDRESS,
    D0F0xBC_xC0200310_SMU_BUSY_MASK_MASK |
    D0F0xBC_xC0200310_PCIE_LCLK_IDLE1_MASK_MASK |
    D0F0xBC_xC0200310_PCIE_LCLK_IDLE2_MASK_MASK |
    D0F0xBC_xC0200310_PCIE_LCLK_IDLE3_MASK_MASK |
    D0F0xBC_xC0200310_PCIE_LCLK_IDLE4_MASK_MASK |
    D0F0xBC_xC0200310_L1IMUGPP_IDLE_MASK_MASK |
    D0F0xBC_xC0200310_L1IMUGPPSB_IDLE_MASK_MASK |
    D0F0xBC_xC0200310_L1IMUBIF_IDLE_MASK_MASK |
    D0F0xBC_xC0200310_L1IMUINTGEN_IDLE_MASK_MASK |
    D0F0xBC_xC0200310_L2IMU_IDLE_MASK_MASK |
    D0F0xBC_xC0200310_ORB_IDLE_MASK_MASK |
    D0F0xBC_xC0200310_ON_INB_WAKE_MASK_MASK |
    D0F0xBC_xC0200310_ON_INB_WAKE_ACK_MASK_MASK |
    D0F0xBC_xC0200310_ON_OUTB_WAKE_MASK_MASK |
    D0F0xBC_xC0200310_ON_OUTB_WAKE_ACK_MASK_MASK |
    D0F0xBC_xC0200310_DMAACTIVE_MASK_MASK |
    D0F0xBC_xC0200310_RFE_BUSY_MASK_MASK |
    D0F0xBC_xC0200310_BIF_CG_LCLK_BUSY_MASK_MASK |
    D0F0xBC_xC0200310_L1IMU_SMU_IDLE_MASK_MASK,
    ( 1 << D0F0xBC_xC0200310_SMU_BUSY_MASK_OFFSET) |
    ( 0 << D0F0xBC_xC0200310_PCIE_LCLK_IDLE1_MASK_OFFSET) |
    ( 0 << D0F0xBC_xC0200310_PCIE_LCLK_IDLE2_MASK_OFFSET) |
    ( 0 << D0F0xBC_xC0200310_PCIE_LCLK_IDLE3_MASK_OFFSET) |
    ( 0 << D0F0xBC_xC0200310_PCIE_LCLK_IDLE4_MASK_OFFSET) |
    ( 1 << D0F0xBC_xC0200310_L1IMUGPP_IDLE_MASK_OFFSET) |
    ( 1 << D0F0xBC_xC0200310_L1IMUGPPSB_IDLE_MASK_OFFSET) |
    ( 1 << D0F0xBC_xC0200310_L1IMUBIF_IDLE_MASK_OFFSET) |
    ( 1 << D0F0xBC_xC0200310_L1IMUINTGEN_IDLE_MASK_OFFSET) |
    ( 1 << D0F0xBC_xC0200310_L2IMU_IDLE_MASK_OFFSET) |
    ( 1 << D0F0xBC_xC0200310_ORB_IDLE_MASK_OFFSET) |
    ( 1 << D0F0xBC_xC0200310_ON_INB_WAKE_MASK_OFFSET) |
    ( 1 << D0F0xBC_xC0200310_ON_INB_WAKE_ACK_MASK_OFFSET) |
    ( 1 << D0F0xBC_xC0200310_ON_OUTB_WAKE_MASK_OFFSET) |
    ( 1 << D0F0xBC_xC0200310_ON_OUTB_WAKE_ACK_MASK_OFFSET) |
    ( 1 << D0F0xBC_xC0200310_DMAACTIVE_MASK_OFFSET) |
    ( 1 << D0F0xBC_xC0200310_RFE_BUSY_MASK_OFFSET) |
    ( 1 << D0F0xBC_xC0200310_BIF_CG_LCLK_BUSY_MASK_OFFSET) |
    ( 1 << D0F0xBC_xC0200310_L1IMU_SMU_IDLE_MASK_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_LCLK_DEEP_SLEEP,
    D0F0xBC_xC020008C_TYPE,
    D0F0xBC_xC020008C_ADDRESS,
    D0F0xBC_xC020008C_DIV_ID_MASK |
    D0F0xBC_xC020008C_RAMP_DIS_MASK |
    D0F0xBC_xC020008C_HYSTERESIS_MASK,
    ( 4 << D0F0xBC_xC020008C_DIV_ID_OFFSET) |
    ( 0 << D0F0xBC_xC020008C_RAMP_DIS_OFFSET) |
    ( 0xF << D0F0xBC_xC020008C_HYSTERESIS_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IGFX_DISABLED,
    D0F0xBC_xC0200310_TYPE,
    D0F0xBC_xC0200310_ADDRESS,
    D0F0xBC_xC0200310_RLC_SMU_GFXCLK_OFF_MASK_MASK,
    0x0
    ),
// Reset : 0, Enable : 1
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_LCLK_DEEP_SLEEP,
    D0F0xBC_xC020008C_TYPE,
    D0F0xBC_xC020008C_ADDRESS,
    D0F0xBC_xC020008C_ENABLE_DS_MASK,
    (0x1 << D0F0xBC_xC020008C_ENABLE_DS_OFFSET)
    ),
//---------------------------------------------------------------------------
  GNB_ENTRY_TERMINATE
};
