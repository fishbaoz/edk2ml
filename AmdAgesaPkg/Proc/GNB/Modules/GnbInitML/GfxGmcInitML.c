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
#include  "Gnb.h"
#include  "GnbGfx.h"
#include  "GnbCommonLib.h"
#include  "GnbTable.h"
#include  "GnbPcieConfig.h"
#include  "GnbRegisterAccML.h"
#include  "cpuFamilyTranslation.h"
#include  "GnbRegistersML.h"
#include  "GfxLibML.h"
#include  "GfxGmcInitML.h"
#include  "Filecode.h"

#define FILECODE PROC_GNB_MODULES_GNBINITML_GFXGMCINITML_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern GNB_TABLE ROMDATA GfxGmcClockGatingDisableML [];
extern GNB_TABLE ROMDATA GfxGmcInitTableML [];
extern GNB_TABLE ROMDATA GfxGmcClockGatingEnableML [];


#define GNB_GFX_DRAM_CH_0_PRESENT 1
#define GNB_GFX_DRAM_CH_1_PRESENT 2

#define DRAMTYPE_GDDR5            0
#define DRAMTYPE_DDR3             1

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
 * Initialize Fb location
 *
 *
 *
 * @param[in]   GnbHandle         Pointer to GNB_HANDLE
 * @param[in]   Gfx               Pointer to global GFX configuration
 *
 */
VOID
GfxGmcInitializeFbLocationML (
  IN      GNB_HANDLE            *GnbHandle,
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  GMMx2024_STRUCT   GMMx2024;
  UINT32            GMMx2068;
  UINT32            GMMx2C04;
  UINT32            GMMx5428;
  UINT64            FBBase;
  UINT64            FBTop;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGmcInitializeFbLocationML Enter\n");

  FBBase = 0x0F400000000;
  FBTop = FBBase + Gfx->UmaInfo.UmaSize - 1;
  GMMx2024.Value = 0;
  GMMx2C04 = 0;
  GMMx2024.Field.FbBase = (UINT16) (FBBase >> 24);
  GMMx2024.Field.FbTop = (UINT16) (FBTop >> 24);
  GMMx2068 = (UINT32) (Gfx->UmaInfo.UmaBase >> 22);
  GMMx2C04 = (UINT32) (FBBase >> 8);
  GMMx5428 = Gfx->UmaInfo.UmaSize >> 20;
  GnbRegisterWriteML (GnbHandle, GMMx2024_TYPE, GMMx2024_ADDRESS, &GMMx2024.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteML (GnbHandle, GMMx2068_TYPE, GMMx2068_ADDRESS, &GMMx2068, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteML (GnbHandle, GMMx2C04_TYPE, GMMx2C04_ADDRESS, &GMMx2C04, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteML (GnbHandle, GMMx5428_TYPE, GMMx5428_ADDRESS, &GMMx5428, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGmcInitializeFbLocationML Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize sequencer model
 *
 *
 *
 * @param[in]   GnbHandle         Pointer to GNB_HANDLE
 * @param[in]   Gfx               Pointer to global GFX configuration
 *
 */
VOID
GfxGmcInitializeHubAndCitfSteeringML (
  IN      GNB_HANDLE            *GnbHandle,
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  GMMx2004_STRUCT           GMMx2004;
  GMMx2008_STRUCT           GMMx2008;

  GMMx2004.Value = 0x2210;
  GMMx2008.Value = 0;

  GnbRegisterWriteML (GnbHandle, GMMx2004_TYPE, GMMx2004_ADDRESS, &GMMx2004.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteML (GnbHandle, GMMx2008_TYPE, GMMx2008_ADDRESS, &GMMx2008.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));

}

/*----------------------------------------------------------------------------------------*/
/**
 * Enable Stutter Mode with/without power-gating
 *
 *
 *
 * @param[in]   GnbHandle         Pointer to GNB_HANDLE
 * @param[in]   Gfx               Pointer to global GFX configuration
 *
 */
VOID
GfxGmcEnableStutterModePowerGatingML (
  IN      GNB_HANDLE            *GnbHandle,
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  GMMx3508_STRUCT   GMMx3508;
  GMMx350C_STRUCT   GMMx350C;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGmcEnableStutterModePowerGatingML Enter\n");

  GnbRegisterReadML (GnbHandle, GMMx3508_TYPE, GMMx3508_ADDRESS, &GMMx3508.Value, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadML (GnbHandle, GMMx350C_TYPE, GMMx350C_ADDRESS, &GMMx350C.Value, 0, GnbLibGetHeader (Gfx));
  if (Gfx->GmcPowerGating != GmcPowerGatingDisabled) {
    // Enabling power gating
    if (Gfx->GmcPowerGating == GmcPowerGatingWithStutter) {
      GMMx3508.Field.RENG_EXECUTE_ON_PWR_UP = 1;
      GMMx350C.Field.RENG_EXECUTE_ON_REG_UPDATE = 1;
      GMMx350C.Field.STCTRL_STUTTER_EN = 0;
    } else {
      GMMx3508.Field.RENG_EXECUTE_ON_PWR_UP = 0;
      GMMx350C.Field.RENG_EXECUTE_ON_REG_UPDATE = 0;
      GMMx350C.Field.STCTRL_STUTTER_EN = 1;
    }
    GnbRegisterWriteML (GnbHandle, GMMx3508_TYPE, GMMx3508_ADDRESS, &GMMx3508.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
    GnbRegisterWriteML (GnbHandle, GMMx350C_TYPE, GMMx350C_ADDRESS, &GMMx350C.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  } else {
    // Disabling power gating
    GMMx3508.Field.RENG_EXECUTE_ON_PWR_UP = 0;
    GMMx350C.Field.RENG_EXECUTE_ON_REG_UPDATE = 0;
    GMMx350C.Field.STCTRL_STUTTER_EN = 0;
    GnbRegisterWriteML (GnbHandle, GMMx3508_TYPE, GMMx3508_ADDRESS, &GMMx3508.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
    GnbRegisterWriteML (GnbHandle, GMMx350C_TYPE, GMMx350C_ADDRESS, &GMMx350C.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGmcEnableStutterModePowerGatingML Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Secure Garlic Access
 *
 *
 *
 * @param[in]   GnbHandle         Pointer to GNB_HANDLE
 * @param[in] Gfx         Pointer to global GFX configuration
 */

VOID
GfxGmcSecureGarlicAccessML (
  IN      GNB_HANDLE            *GnbHandle,
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  UINT32   GMMx2868;
  UINT32   GMMx286C;
  UINT32   GMMx2878;

  GMMx2868 = (UINT32) (Gfx->UmaInfo.UmaBase >> 20);
  GnbRegisterWriteML (GnbHandle, GMMx2868_TYPE, GMMx2868_ADDRESS, &GMMx2868, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GMMx286C = (UINT32) (((Gfx->UmaInfo.UmaBase + Gfx->UmaInfo.UmaSize) >> 20) - 1);
  GnbRegisterWriteML (GnbHandle, GMMx286C_TYPE, GMMx286C_ADDRESS, &GMMx286C, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  // Area FB - 32K reserved by VBIOS for SBIOS to use
  GMMx2878 = (UINT32) ((Gfx->UmaInfo.UmaBase + Gfx->UmaInfo.UmaSize - 32 * 1024) >> 12);
  GnbRegisterWriteML (GnbHandle, GMMx2878_TYPE, GMMx2878_ADDRESS, &GMMx2878, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize C6 aperture location
 *
 *
 *
 * @param[in]   GnbHandle         Pointer to GNB_HANDLE
 * @param[in]   Gfx               Pointer to global GFX configuration
 *
 */
VOID
GfxGmcInitializeC6LocationML (
  IN      GNB_HANDLE            *GnbHandle,
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  D18F2x118_STRUCT    D18F2x118;
  D18F1x44_STRUCT     D18F1x44;
  UINT32     GMMx2870;
  UINT32     GMMx2874;

  // Check C6 enable,  D18F2x118[CC6SaveEn]
  GnbRegisterReadML (GnbHandle, D18F2x118_TYPE, D18F2x118_ADDRESS, &D18F2x118.Value, 0, GnbLibGetHeader (Gfx));

  if (D18F2x118.Field.CC6SaveEn) {
    // From D18F1x[144:140,44:40] DRAM Base/Limit,
    // {DramBase[47:24], 00_0000h} <= address[47:0] <= {DramLimit[47:24], FF_FFFFh}.
    GnbRegisterReadML (GnbHandle, D18F1x44_TYPE, D18F1x44_ADDRESS, &D18F1x44.Value, 0, GnbLibGetHeader (Gfx));
    //
    // base 39:20, base = Dram Limit + 1
    // ex: system 256 MB on Node 0, D18F1x44.Field.DramLimit_39_24_ = 0xE (240MB -1)
    // Node DRAM     D18F1x[144:140,44:40]    CC6DRAMRange         D18F4x128   D18F1x120 D18F1x124
    //    0 256MB    0MB ~ 240 MB - 1        240 MB ~ 256 MB - 1     0           0 MB,    256 MB - 1
    //

    // base 39:20
    GMMx2870 = ((D18F1x44.Field.DramLimit_39_24 + 1) << 4);
    // top 39:20
    GMMx2874 = (((D18F1x44.Field.DramLimit_39_24 + 1) << 24) + (16 * 0x100000) - 1) >> 20;

    GnbRegisterWriteML (GnbHandle, GMMx2870_TYPE, GMMx2870_ADDRESS, &GMMx2870, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
    GnbRegisterWriteML (GnbHandle, GMMx2874_TYPE, GMMx2874_ADDRESS, &GMMx2874, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize GMC
 *
 *
 *
 * @param[in]   Gfx               Pointer to global GFX configuration
 *
 */

AGESA_STATUS
GfxGmcInitML (
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  UINT32            GMMx206C;
  UINT32            GMMx2878;
  UINT32            GMMx25E8;
  D18F3x44_STRUCT   D18F3x44;
  GNB_HANDLE        *GnbHandle;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGmcInitML Enter\n");
//2.1 Disable clock-gating
  GnbHandle = GnbGetHandle (GnbLibGetHeader (Gfx));
  ASSERT (GnbHandle != NULL);
  GnbProcessTable (
    GnbHandle,
    GfxGmcClockGatingDisableML,
    0,
    GNB_TABLE_FLAGS_FORCE_S3_SAVE,
    GnbLibGetHeader (Gfx)
    );
//2.3 HUB & CITF channel steering
  GfxGmcInitializeHubAndCitfSteeringML (GnbHandle, Gfx);
//2.6 Frame buffer location
  GfxGmcInitializeFbLocationML (GnbHandle, Gfx);
//2.8 Securing GARLIC access
//2.8.1 GARLIC request disable
  GfxGmcSecureGarlicAccessML (GnbHandle, Gfx);
//2.8.2 C6 save aperture
  GfxGmcInitializeC6LocationML (GnbHandle, Gfx);
//2.2 System memory address translation
//2.4 Sequencer model programming
//2.5 Power Gating - PGFSM and RENG init
//2.7 Performance tuning
//2.8 Garlic Credits
//2.10 Display latency
//2.11 Remove blackout
  GnbProcessTable (
    GnbHandle,
    GfxGmcInitTableML,
    0,
    GNB_TABLE_FLAGS_FORCE_S3_SAVE,
    GnbLibGetHeader (Gfx)
    );
//4. Power management
//4.1 Enabling clock-gating
  if (Gfx->GmcClockGating) {
    GnbProcessTable (
      GnbHandle,
      GfxGmcClockGatingEnableML,
      0,
      GNB_TABLE_FLAGS_FORCE_S3_SAVE,
      GnbLibGetHeader (Gfx)
      );
  }

//5. Steering all requests to Garlic for non-ECC, Onion for ECC.
//   SETUP or BLDCFG may override.
  switch (Gfx->UmaSteering) {
  case DefaultGarlic:
    GMMx206C = DefaultGarlic;
    GnbRegisterReadML (GnbHandle, D18F3x44_TYPE, D18F3x44_ADDRESS, &D18F3x44.Value, 0, GnbLibGetHeader (Gfx));
    if (D18F3x44.Field.DramEccEn == 1) {
      // If ECC enabled, do not use DefaultGarlic
      GMMx206C = SystemTrafficOnion;
    }
    break;
  case SystemTrafficOnion:
    GMMx206C = SystemTrafficOnion;
    break;
  case Onion:
    GMMx206C = Onion;
    break;
  case Garlic:
    GMMx206C = Garlic;
    break;
  default:
    GMMx206C = DefaultGarlic;
    ASSERT (FALSE);
    break;
  }
  GnbRegisterWriteML (GnbHandle, GMMx206C_TYPE, GMMx206C_ADDRESS, &GMMx206C, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_GMM_REGISTER_OVERRIDE, Gfx, GnbLibGetHeader (Gfx));
//3.7 GMC Not Allow Locking of Critical Registers
  if (Gfx->GmcLockRegisters) {
    GnbRegisterReadML (GnbHandle, GMMx2878_TYPE, GMMx2878_ADDRESS, &GMMx2878, 0, GnbLibGetHeader (Gfx));
    GMMx2878 |= BIT28;
    GnbRegisterWriteML (GnbHandle, GMMx2878_TYPE, GMMx2878_ADDRESS, &GMMx2878, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  }

  GnbRegisterReadML (GnbHandle, GMMx25E8_TYPE, GMMx25E8_ADDRESS, &GMMx25E8, 0, GnbLibGetHeader (Gfx));
  GMMx25E8 |= BIT14;
  GnbRegisterWriteML (GnbHandle, GMMx25E8_TYPE, GMMx25E8_ADDRESS, &GMMx25E8, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));

  //4.2 Enabling stutter mode with or without power-gating
  GfxGmcEnableStutterModePowerGatingML (GnbHandle, Gfx);

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGmcInitML Exit\n");
  return AGESA_SUCCESS;
}
