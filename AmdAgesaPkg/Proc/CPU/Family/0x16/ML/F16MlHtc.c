/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_16 Mullins thermal initialization
 *
 * Performs processor thermal initialization.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x16/ML
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ******************************************************************************
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
 ******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "amdlib.h"
#include "Ids.h"
#include "cpuRegisters.h"
#include "GeneralServices.h"
#include "CommonReturns.h"
#include "cpuFeatures.h"
#include "cpuHtc.h"
#include "cpuF16PowerMgmt.h"
#include "F16MlPowerMgmt.h"
#include "Gnb.h"
#include "GnbPcie.h"
#include "GnbRegistersML.h"
#include "GnbRegisterAccML.h"
#include "GnbHandleLib.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_CPU_FAMILY_0X16_ML_F16MLHTC_FILECODE


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
AGESA_STATUS
STATIC
F16MlInitializeHtc (
  IN       HTC_FAMILY_SERVICES    *HtcServices,
  IN       UINT64                  EntryPoint,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

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
 *    Main entry point for initializing the Thermal Control
 *    safety net feature.
 *
 *    This must be run by all Family 16h Mullins core 0s in the system.
 *
 * @param[in]  HtcServices             The current CPU's family services.
 * @param[in]  EntryPoint              Timepoint designator.
 * @param[in]  PlatformConfig          Platform profile/build option config structure.
 * @param[in]  StdHeader               Config handle for library and services.
 *
 * @retval     AGESA_SUCCESS           Always succeeds.
 *
 */
AGESA_STATUS
STATIC
F16MlInitializeHtc (
  IN       HTC_FAMILY_SERVICES    *HtcServices,
  IN       UINT64                  EntryPoint,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  NB_CAPS_REGISTER               NbCaps;
  HTC_REGISTER                   HtcReg;
  CLK_PWR_TIMING_CTRL2_REGISTER  Cptc2;
  POPUP_PSTATE_REGISTER          PopUpPstate;
  PCI_ADDR                       PciAddress;
  D0F0xBC_xC0107097_STRUCT       D0F0xBC_xC0107097;

  if ((EntryPoint & (CPU_FEAT_AFTER_POST_MTRR_SYNC | CPU_FEAT_AFTER_RESUME_MTRR_SYNC)) != 0) {
    PciAddress.AddressValue = NB_CAPS_PCI_ADDR;
    LibAmdPciRead (AccessWidth32, PciAddress, &NbCaps, StdHeader);
    if (NbCaps.HtcCapable == 1) {
      // Enable HTC
      PciAddress.Address.Register = HTC_REG;
      LibAmdPciRead (AccessWidth32, PciAddress, &HtcReg, StdHeader);
      GnbRegisterReadML (GnbGetHandle (StdHeader), D0F0xBC_xC0107097_TYPE, D0F0xBC_xC0107097_ADDRESS, &D0F0xBC_xC0107097, 0, StdHeader);
      HtcReg.HtcTmpLmt = D0F0xBC_xC0107097.Field.HtcTmpLmt;
      if (HtcReg.HtcTmpLmt != 0) {
        // Enable HTC
        HtcReg.HtcEn = 1;
        PciAddress.Address.Register = CPTC2_REG;
        LibAmdPciRead (AccessWidth32, PciAddress, &Cptc2, StdHeader);
        if (HtcReg.HtcPstateLimit > Cptc2.HwPstateMaxVal) {
          // F3xDC[HwPstateMaxVal] = F3x64[HtcPstateLimit]
          Cptc2.HwPstateMaxVal = HtcReg.HtcPstateLimit;
          LibAmdPciWrite (AccessWidth32, PciAddress, &Cptc2, StdHeader);
          // F3xA8[PopDownPstate] = F3xDC[HwPstateMaxVal]
          PciAddress.Address.Register = POPUP_PSTATE_REG;
          LibAmdPciRead (AccessWidth32, PciAddress, &PopUpPstate, StdHeader);
          PopUpPstate.PopDownPstate = Cptc2.HwPstateMaxVal;
          LibAmdPciWrite (AccessWidth32, PciAddress, &PopUpPstate, StdHeader);
        }
      } else {
        // Disable HTC
        HtcReg.HtcEn = 0;
      }
      PciAddress.Address.Register = HTC_REG;
      IDS_OPTION_HOOK (IDS_HTC_CTRL, &HtcReg, StdHeader);
      LibAmdPciWrite (AccessWidth32, PciAddress, &HtcReg, StdHeader);
    }
  }
  return AGESA_SUCCESS;
}

CONST HTC_FAMILY_SERVICES ROMDATA F16MlHtcSupport =
{
  0,
  (PF_HTC_IS_SUPPORTED) CommonReturnTrue,
  F16MlInitializeHtc
};
