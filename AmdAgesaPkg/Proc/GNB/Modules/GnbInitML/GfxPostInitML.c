/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe late post initialization.
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
#include  "GnbPcieConfig.h"
#include  "GnbCommonLib.h"
#include  "GnbGfx.h"
#include  "GnbGfxConfig.h"
#include  "GnbFuseTable.h"
#include  "GnbGfxInitLibV1.h"
#include  "GnbRegisterAccML.h"
#include  "GnbRegistersML.h"
#include  "GnbHandleLib.h"
#include  "GfxLibV3.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITML_GFXPOSTINITML_FILECODE
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
 * Init GFX at Post.
 *
 *
 *
 * @param[in]     StdHeader     Standard configuration header
 * @retval        AGESA_STATUS
 */


AGESA_STATUS
GfxPostInterfaceML (
  IN       AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AMD_POST_PARAMS           *PostParamsPtr;
  AGESA_STATUS              Status;
  GFX_PLATFORM_CONFIG       *Gfx;
  D0F0xBC_xC00C0000_STRUCT  D0F0xBC_xC00C0000;
  D0F0xD4_x013014AC_STRUCT  D0F0xD4_x13014AC;
  D0F0xD4_x013014B6_STRUCT  D0F0xD4_x13014B6;
  GNB_HANDLE                *GnbHandle;

  PostParamsPtr = (AMD_POST_PARAMS *)StdHeader;
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxPostInterfaceML Enter\n");
  GnbHandle = GnbGetHandle (StdHeader);
  ASSERT (GnbHandle != NULL);
  GnbRegisterReadML (GnbHandle, D0F0xBC_xC00C0000_TYPE,
      D0F0xBC_xC00C0000_ADDRESS, &D0F0xBC_xC00C0000.Value, 0, StdHeader);
  Status = GfxLocateConfigData (StdHeader, &Gfx);
  ASSERT (Status == AGESA_SUCCESS);
  if (Status == AGESA_SUCCESS) {
    if (D0F0xBC_xC00C0000.Field.GPU_DIS == 1) {
      PostParamsPtr->MemConfig.UmaMode = UMA_NONE;
      Gfx->GfxFusedOff = TRUE;
    }
  } else {
    PostParamsPtr->MemConfig.UmaMode = UMA_NONE;
  }
  if (PostParamsPtr->MemConfig.UmaMode == UMA_NONE) {
    GnbRegisterReadML (GnbHandle, D0F0xD4_x013014AC_TYPE, D0F0xD4_x013014AC_ADDRESS, &D0F0xD4_x13014AC.Value, 0, StdHeader);
    GnbRegisterReadML (GnbHandle, D0F0xD4_x013014B6_TYPE, D0F0xD4_x013014B6_ADDRESS, &D0F0xD4_x13014B6.Value, 0, StdHeader);
    D0F0xD4_x13014AC.Field.StrapBifAudioEnPin = FALSE;
    D0F0xD4_x13014B6.Field.StrapBifAudioEn = FALSE;
    GnbRegisterWriteML (GnbHandle, D0F0xD4_x013014AC_TYPE, D0F0xD4_x013014AC_ADDRESS, &D0F0xD4_x13014AC.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
    GnbRegisterWriteML (GnbHandle, D0F0xD4_x013014B6_TYPE, D0F0xD4_x013014B6_ADDRESS, &D0F0xD4_x13014B6.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
  }

  if (D0F0xBC_xC00C0000.Field.GPU_DIS != 1) {
    // Power down iGPU
    GfxRequestGPUPowerV3 (Gfx, 0);
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxPostInterfaceML Exit [0x%x]\n", Status);
  return  Status;
}
