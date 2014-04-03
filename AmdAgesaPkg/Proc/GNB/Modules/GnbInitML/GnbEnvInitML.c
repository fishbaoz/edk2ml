/* $NoKeywords:$ */
/**
 * @file
 *
 * GNB env post initialization.
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
#include  "heapManager.h"
#include  "Gnb.h"
#include  "GnbCommonLib.h"
#include  "GnbTable.h"
#include  "GnbPcieConfig.h"
#include  "GnbNbInitLibV1.h"
#include  "GnbNbInitLibV4.h"
#include  "GnbNbInitLibV5.h"
#include  "GnbFuseTableML.h"
#include  "GnbFuseTable.h"
#include  "GnbRegistersML.h"
#include  "GnbRegisterAccML.h"
#include  "GnbGfx.h"
#include  "PcieLibML.h"
#include  "OptionGnb.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITML_GNBENVINITML_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern GNB_BUILD_OPTIONS_ML   GnbBuildOptionsML;
extern GNB_TABLE ROMDATA      GnbEnvInitTableML [];
extern GNB_TABLE ROMDATA      GnbIommuInitTableML [];

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
 * HTC Data
 *
 * @param[in] GnbHandle       Gnb Header
 * @param[in] StdHeader       Standard Configuration Header
 */

VOID
STATIC
GnbFillHtcData (
  IN      GNB_HANDLE          *GnbHandle,
  IN      AMD_CONFIG_PARAMS   *StdHeader
  )
{

  D0F0xBC_x3FDD4_STRUCT D0F0xBC_x3FDD4;
  D18F3x64_STRUCT  D18F3x64;

  GnbRegisterReadML (GnbHandle, D18F3x64_TYPE, D18F3x64_ADDRESS,
    &D18F3x64.Value, 0, StdHeader);

  GnbRegisterReadML (GnbHandle, D0F0xBC_x3FDD4_TYPE, D0F0xBC_x3FDD4_ADDRESS,
    &D0F0xBC_x3FDD4.Value, 0, StdHeader);

  if (D18F3x64.Field.HtcEn == 1) {

    D0F0xBC_x3FDD4.Field.HighThreshold = D18F3x64.Field.HtcTmpLmt / 2 + 52;
    D0F0xBC_x3FDD4.Field.LowThreshold =
        D0F0xBC_x3FDD4.Field.HighThreshold - (D18F3x64.Field.HtcHystLmt / 2);

    D0F0xBC_x3FDD4.Field.HighThreshold =
        (D0F0xBC_x3FDD4.Field.HighThreshold + 49) << 3;
    D0F0xBC_x3FDD4.Field.LowThreshold =
        (D0F0xBC_x3FDD4.Field.LowThreshold + 49) << 3;

  } else {

    D0F0xBC_x3FDD4.Field.HighThreshold = 0;
    D0F0xBC_x3FDD4.Field.LowThreshold = 0;

  }

  IDS_HDT_CONSOLE (GNB_TRACE, "Htc High: %08x\n", D0F0xBC_x3FDD4.Field.HighThreshold);
  IDS_HDT_CONSOLE (GNB_TRACE, "Htc Low:  %08x\n", D0F0xBC_x3FDD4.Field.LowThreshold);

  GnbRegisterWriteML (GnbHandle, D0F0xBC_x3FDD4_TYPE,
        D0F0xBC_x3FDD4_ADDRESS, &D0F0xBC_x3FDD4.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);

}

/*----------------------------------------------------------------------------------------*/
/**
 * GNB init at env
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbEnvInterfaceML (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS      Status;
  AGESA_STATUS      AgesaStatus;
  AMD_ENV_PARAMS    *EnvParamsPtr;
  UINT32            Property;
  GNB_HANDLE        *GnbHandle;
  PCI_ADDR          GnbPciAddress;
  D0F0x7C_STRUCT    D0F0x7C;
  GNB_BUILD_OPTIONS_ML  *GnbBuildOptionData;
  UINT32            EnablementMask;
  GFX_PLATFORM_CONFIG   *Gfx;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEnvInterfaceML Enter\n");
  AgesaStatus = AGESA_SUCCESS;
  EnvParamsPtr = (AMD_ENV_PARAMS *) StdHeader;
  Property = TABLE_PROPERTY_DEFAULT;
  Property |= GnbBuildOptionsML.GnbCommonOptions.CfgIommuL1ClockGatingEnable ? TABLE_PROPERTY_IOMMU_L1_CLOCK_GATING : 0;
  Property |= GnbBuildOptionsML.GnbCommonOptions.CfgIommuL2ClockGatingEnable ? TABLE_PROPERTY_IOMMU_L2_CLOCK_GATING : 0;

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  ASSERT (GnbBuildOptionData != NULL);

  Gfx = GnbLocateHeapBuffer (AMD_GFX_PLATFORM_CONFIG_HANDLE, StdHeader);
  ASSERT (Gfx != NULL);
  Property |= (Gfx->GmcPowerGating != GmcPowerGatingDisabled) ? TABLE_PROPERTY_GMC_STUTTER_ENABLED : 0;

  if (!EnvParamsPtr->GnbEnvConfiguration.IommuSupport) {
    Property |= TABLE_PROPERTY_IOMMU_DISABLED;
  }

  GnbLibPciRead (MAKE_SBDFO (0, 0, 0, 0, D0F0x7C_ADDRESS), AccessWidth32, &D0F0x7C.Value, StdHeader);
  Property |= (D0F0x7C.Field.ForceIntGfxDisable != 0) ? TABLE_PROPERTY_IGFX_DISABLED : 0;

  GnbLoadFuseTableML (StdHeader);
  GnbHandle = GnbGetHandle (StdHeader);

  EnablementMask = GnbBuildOptionData->CfgSMUServiceEnablementBitMap;
  IDS_HDT_CONSOLE (GNB_TRACE, " Mask = %08x\n", EnablementMask);

  if ((EnablementMask & BAPM_MASK) == 0) {
    GnbOverrideFuseTableML (StdHeader);
  }

  GnbPciAddress = GnbGetHostPciAddress (GnbHandle);
  Status = GnbSetTomML (GnbPciAddress, StdHeader);
  GnbOrbDynamicWakeV5 (GnbPciAddress, StdHeader);
  GnbFillHtcData (GnbHandle, StdHeader);
  if (GnbIsGnbConnectedToSb (GnbHandle)) {
    GnbLpcDmaDeadlockPreventionV4 (GnbHandle, StdHeader);
  }

  IDS_OPTION_HOOK (IDS_GNB_PROPERTY, &Property, StdHeader);

  Status = GnbProcessTable (
             GnbHandle,
             GnbEnvInitTableML,
             Property,
             GNB_TABLE_FLAGS_FORCE_S3_SAVE,
             StdHeader
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  Status = GnbProcessTable (
             GnbHandle,
             GnbIommuInitTableML,
             Property,
             GNB_TABLE_FLAGS_FORCE_S3_SAVE,
             StdHeader
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEnvInterfaceML Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}
