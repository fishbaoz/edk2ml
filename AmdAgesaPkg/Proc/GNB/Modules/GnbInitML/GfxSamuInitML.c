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
#include  "heapManager.h"
#include  "Gnb.h"
#include  "GnbGfx.h"
#include  "GnbCommonLib.h"
#include  "GnbTable.h"
#include  "GnbPcieConfig.h"
#include  "GnbRegisterAccML.h"
#include  "cpuFamilyTranslation.h"
#include  "GnbRegistersML.h"
#include  "GfxLibML.h"
#include  "GfxSamuInitML.h"
#include  "GnbSamuPatchML.h"
#include  "OptionGnb.h"
#include  "Filecode.h"

#define FILECODE PROC_GNB_MODULES_GNBINITML_GFXSAMUINITML_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern GNB_BUILD_OPTIONS_ML  GnbBuildOptionsML;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
#define LENGTH_1MBYTE             0x0100000ul
#define MASK_1MBYTE               0x0FFFFFul

#define SAM_IND_INDEX             0x22000ul
#define SAM_IND_DATA              0x22004ul

#define SAM_SAB_IND_INDEX         0x22008ul
#define SAM_SAB_IND_DATA          0x2200Cul

#define SMU_TOOLS_INDEX           0x238ul
#define SMU_TOOLS_DATA            0x23Cul

typedef struct {
  UINT32  BootControl;     ///<
  UINT32  KeySelect;       ///<
  UINT32  KernelAddrLo;    ///<
  UINT32  KernelAddrHi;    ///<
  UINT32  TweakSelect;     ///<
} SAMU_BOOT_CONTROL;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


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
GfxSamuInit (
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  D0F0xBC_xC00C0000_STRUCT  D0F0xBC_xC00C0000;
  GNB_HANDLE                *GnbHandle;
  VOID                      *ControlXBuffer;
  VOID                      *AlignedControlXBuffer;
  VOID                      *PatchYBuffer;
  VOID                      *AlignedPatchYBuffer;
  SAMU_BOOT_CONTROL         *SamuBootControl;

  UINT32                    D0F0xBC_x800000A4;
  UINT32                    GMMx22000;
  UINT32                    GMMx22004;
  UINT32                    GMMx22008;
  UINT32                    GMMx2200C;
  UINT32                    LoopCount;
  BOOLEAN                   SamuUseFusedPatch;
  BOOLEAN                   SamuPatchEnabled;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSamuInit Enter\n");

  GnbHandle = GnbGetHandle (GnbLibGetHeader (Gfx));
  ASSERT (GnbHandle != NULL);
  GnbRegisterReadML (GnbHandle, D0F0xBC_xC00C0000_TYPE, D0F0xBC_xC00C0000_ADDRESS,
      &D0F0xBC_xC00C0000.Value, 0, GnbLibGetHeader (Gfx));

  SamuPatchEnabled = GnbBuildOptionsML.CfgSamuPatchEnabled;
  IDS_OPTION_HOOK (IDS_GNB_LOAD_SAMU_PATCH, &SamuPatchEnabled, GnbLibGetHeader (Gfx));

  if ((((D0F0xBC_xC00C0000.Value) & BIT24) == 0) &&
      (SamuPatchEnabled == TRUE)) {

    // Decide which version of the patch to use
    SamuUseFusedPatch = TRUE;

    // read efuse status from SAM_SAB
    GMMx22008 = 0x29;
    GnbRegisterWriteML (GnbHandle, GMMx22008_TYPE, GMMx22008_ADDRESS,
        &GMMx22008, 0, GnbLibGetHeader (Gfx));
    GnbRegisterReadML (GnbHandle, GMMx2200C_TYPE, GMMx2200C_ADDRESS,
        &GMMx2200C, 0, GnbLibGetHeader (Gfx));
    IDS_HDT_CONSOLE (GNB_TRACE, " SAMSAB:29=%08x\n", GMMx2200C);

    if (GMMx2200C == 0x80000001) {
      SamuUseFusedPatch = FALSE;
    }

    ControlXBuffer = GnbAllocateHeapBufferAndClear (AMD_GNB_SAMU_BOOT_CONTROL_HANDLE, 2 * LENGTH_1MBYTE, GnbLibGetHeader (Gfx));
    AlignedControlXBuffer = (VOID *) (((UINTN)ControlXBuffer + LENGTH_1MBYTE) & (~MASK_1MBYTE));
    PatchYBuffer = GnbAllocateHeapBuffer (AMD_GNB_SAMU_PATCH_HANDLE, 2 * LENGTH_1MBYTE, GnbLibGetHeader (Gfx));
    AlignedPatchYBuffer = (VOID *) (((UINTN)PatchYBuffer + LENGTH_1MBYTE) & (~MASK_1MBYTE));

    // Copy samu firmware patch to PatchYBuffer
    if (SamuUseFusedPatch == TRUE) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Samu: load normal patch\n");
      LibAmdMemCopy (AlignedPatchYBuffer, &SamuPatchML[0],
          SamuPatchMLHeader[1], GnbLibGetHeader (Gfx));
    } else {
      IDS_HDT_CONSOLE (GNB_TRACE, "Samu: load unfused patch\n");
      LibAmdMemCopy (AlignedPatchYBuffer, &SamuPatchMLUnfused[0],
          SamuPatchMLHeaderUnfused[1], GnbLibGetHeader (Gfx));
    }

    // WBINVD
    LibAmdWriteBackInvalidateCache ();

    // Load boot control structure
    SamuBootControl = (SAMU_BOOT_CONTROL *)AlignedControlXBuffer;
    SamuBootControl->BootControl = 0x3;
    SamuBootControl->KernelAddrLo = (UINT32) ((UINT64) AlignedPatchYBuffer);
    SamuBootControl->KernelAddrHi = (UINT32) ((((UINT64) AlignedPatchYBuffer) >> 32) & 0xFF);
    if (SamuUseFusedPatch == TRUE) {
      SamuBootControl->TweakSelect = 0xBB027E1F;
      SamuBootControl->KeySelect = 0x8E174F83;
    } else {
      SamuBootControl->TweakSelect = 0x0;
      SamuBootControl->KeySelect = 0x0;
    }

    IDS_HDT_CONSOLE (GNB_TRACE, "Address of ControlXBuffer: 0x%X\n", (UINT64)ControlXBuffer);
    IDS_HDT_CONSOLE (GNB_TRACE, "Address of AlignedControlXBuffer: 0x%X\n", (UINT64)AlignedControlXBuffer);
    IDS_HDT_CONSOLE (GNB_TRACE, "Address of PatchYBuffer: 0x%X\n", (UINT64)PatchYBuffer);
    IDS_HDT_CONSOLE (GNB_TRACE, "Address of AlignedPatchYBuffer: 0x%X\n", (UINT64)AlignedPatchYBuffer);

    IDS_HDT_CONSOLE (GNB_TRACE, "Address of SamuBootControl: 0x%X\n", (UINT64)SamuBootControl);
    IDS_HDT_CONSOLE (GNB_TRACE, "SamuBootControl->KernelAddrLo: 0x%X\n", SamuBootControl->KernelAddrLo);
    IDS_HDT_CONSOLE (GNB_TRACE, "SamuBootControl->KernelAddrHi: 0x%X\n", SamuBootControl->KernelAddrHi);

    // Write 0x0 to SAM_CGC_HOST_CTRL to release the clock-gating of SAMU
    GMMx22000 = 0x3;
    GnbRegisterWriteML (GnbHandle, GMMx22000_TYPE, GMMx22000_ADDRESS, &GMMx22000, 0, GnbLibGetHeader (Gfx));
    GMMx22004 = 0x0;
    GnbRegisterWriteML (GnbHandle, GMMx22004_TYPE, GMMx22004_ADDRESS, &GMMx22004, 0, GnbLibGetHeader (Gfx));

    // Write (physical address of boot control structure)>>8 into SAM_SAB_INIT_TLB_CONFIG (Location X >> 8)
    GMMx22008 = 0x4;
    GnbRegisterWriteML (GnbHandle, GMMx22008_TYPE, GMMx22008_ADDRESS, &GMMx22008, 0, GnbLibGetHeader (Gfx));
    GMMx2200C = ((UINT32) ((UINT64) AlignedControlXBuffer)) >> 8;
    GnbRegisterWriteML (GnbHandle, GMMx2200C_TYPE, GMMx2200C_ADDRESS, &GMMx2200C, 0, GnbLibGetHeader (Gfx));

    // Write 0x0 to SAM_RST_HOST_SOFT_RESET
    GMMx22000 = 0x1;
    GnbRegisterWriteML (GnbHandle, GMMx22000_TYPE, GMMx22000_ADDRESS, &GMMx22000, 0, GnbLibGetHeader (Gfx));
    GMMx22004 = 0x0;
    GnbRegisterWriteML (GnbHandle, GMMx22004_TYPE, GMMx22004_ADDRESS, &GMMx22004, 0, GnbLibGetHeader (Gfx));

    // Write 0x2 to SAM_SCRATCH_0 to start the firmware boot
    GMMx22000 = 0x38;
    GnbRegisterWriteML (GnbHandle, GMMx22000_TYPE, GMMx22000_ADDRESS, &GMMx22000, 0, GnbLibGetHeader (Gfx));
    GMMx22004 = 0x2;
    GnbRegisterWriteML (GnbHandle, GMMx22004_TYPE, GMMx22004_ADDRESS, &GMMx22004, 0, GnbLibGetHeader (Gfx));

    // Poll SAM_RST_HOST_SOFT_RST_RDY and wait for HOST_RDY
    do {
      // Write 0x2 to SAM_SCRATCH_0 to start the firmware boot
      GMMx22000 = 0x51;
      GnbRegisterWriteML (GnbHandle, GMMx22000_TYPE, GMMx22000_ADDRESS, &GMMx22000, 0, GnbLibGetHeader (Gfx));
      GnbRegisterReadML (GnbHandle, GMMx22004_TYPE, GMMx22004_ADDRESS, &GMMx22004, 0, GnbLibGetHeader (Gfx));
    } while ((GMMx22004 & BIT0) == 0);

    // Clear the allocated memory ranges, locations X and Y (write 0), issue WBINVD
    LibAmdMemFill (ControlXBuffer, 0, 2 * LENGTH_1MBYTE, GnbLibGetHeader (Gfx));
    LibAmdMemFill (PatchYBuffer, 0, 2 * LENGTH_1MBYTE, GnbLibGetHeader (Gfx));
    LibAmdWriteBackInvalidateCache ();

    // Confirm read of SMC_DRAM_ACCESS_CNTL is 0x1
    D0F0xBC_x800000A4 = 0;
    for (LoopCount = 0; LoopCount < 0x00FFFFFF; LoopCount++) {
      GnbRegisterReadML (GnbHandle, D0F0xBC_x800000A4_TYPE, D0F0xBC_x800000A4_ADDRESS, &D0F0xBC_x800000A4, 0, GnbLibGetHeader (Gfx));
      if ((D0F0xBC_x800000A4 & BIT0) != 0)  {
        break;
      }
    }
    ASSERT ((D0F0xBC_x800000A4 & BIT0) != 0);
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSamuInit Exit\n");
  return AGESA_SUCCESS;
}
