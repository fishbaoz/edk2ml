/* $NoKeywords:$ */
/**
 * @file
 *
 * Initialize GFX configuration data structure.
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
#include  "Ids.h"
#include  "heapManager.h"
#include  "Gnb.h"
#include  "GnbGfx.h"
#include  "GnbCommonLib.h"
#include  "GfxConfigLib.h"
#include  "OptionGnb.h"
#include  "GnbFamServices.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBGFXCONFIG_GFXCONFIGPOST_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern BUILD_OPT_CFG      UserOptions;
extern GNB_BUILD_OPTIONS  GnbBuildOptions;

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
 * Allocate UMA configuration data
 *
 *
 *
 * @param[in]     StdHeader     Standard configuration header
 * @retval        AGESA_STATUS  Always succeeds
 */

AGESA_STATUS
GfxConfigPostInterface (
  IN       AMD_CONFIG_PARAMS        *StdHeader
  )
{
  GFX_PLATFORM_CONFIG       *Gfx;
  AMD_POST_PARAMS           *PostParamsPtr;
  AGESA_STATUS              Status;
  GNB_BUILD_OPTIONS_COMMON  *GnbCommonOptions;
  PostParamsPtr = (AMD_POST_PARAMS *)StdHeader;
  Status = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxConfigPostInterface Enter\n");
  Gfx = GnbAllocateHeapBuffer (AMD_GFX_PLATFORM_CONFIG_HANDLE, sizeof (GFX_PLATFORM_CONFIG), StdHeader);
  ASSERT (Gfx != NULL);
  if (Gfx != NULL) {
    LibAmdMemFill (Gfx, 0x00, sizeof (GFX_PLATFORM_CONFIG), StdHeader);
    GnbCommonOptions = (GNB_BUILD_OPTIONS_COMMON*) GnbFmGnbBuildOptions (StdHeader);
    if (GnbBuildOptions.IgfxModeAsPcieEp) {
      Gfx->GfxControllerMode = GfxControllerPcieEndpointMode;
      Gfx->GfxPciAddress.AddressValue = MAKE_SBDFO (0, 0, 1, 0, 0);
    } else {
      Gfx->GfxControllerMode = GfxControllerLegacyBridgeMode;
      Gfx->GfxPciAddress.AddressValue = MAKE_SBDFO (0, 1, 5, 0, 0);
    }
    Gfx->StdHeader = (PVOID) StdHeader;
    Gfx->GnbHdAudio = PostParamsPtr->PlatformConfig.GnbHdAudio;
    Gfx->AbmSupport = PostParamsPtr->PlatformConfig.AbmSupport;
    Gfx->DynamicRefreshRate = PostParamsPtr->PlatformConfig.DynamicRefreshRate;
    Gfx->LcdBackLightControl = PostParamsPtr->PlatformConfig.LcdBackLightControl;
    Gfx->AmdPlatformType = UserOptions.CfgAmdPlatformType;
    Gfx->GmcClockGating = GnbCommonOptions->CfgGmcClockGating;
    Gfx->GmcPowerGating = GnbCommonOptions->GmcPowerGating;
    Gfx->UmaSteering = GnbCommonOptions->CfgUmaSteering;
    GNB_DEBUG_CODE (
      GfxConfigDebugDump (Gfx);
      );
  } else {
    Status = AGESA_ERROR;
  }
  IDS_OPTION_HOOK (IDS_GNB_PLATFORMCFG_OVERRIDE, Gfx, StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxConfigPostInterface Exit [0x%x]\n", Status);
  return  Status;
}


