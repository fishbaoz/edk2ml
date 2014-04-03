/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_16 PCI tables for unknown processor
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x16
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
#include "Ids.h"
#include "cpuRegisters.h"
#include "Table.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X16_CPUF16PCIUNKNOWNTABLES_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

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

//  P C I    T a b l e s
// ----------------------

STATIC CONST TABLE_ENTRY_FIELDS ROMDATA F16PciUnknownRegisters[] =
{
// D18F0x68 - Link Transaction Control
// bits[22:21]   DsNpReqLmt          = 0x2
// bits[19]      ApicExtSpur         = 0x1
// bits[18]      ApicExtId           = 0x1
// bits[17]      ApicExtBrdCst       = 0x1
// bits[15]      LimitCldtCfg        = 0x1
// bits[11]      RespPassPW          = 0x1
// bits[10]      DisFillP            = 0x0
  {
    PciRegister,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ALL                           // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_0, 0x68),  // Address
      0x004E8800,                           // RegData
      0x006E8C00,                           // RegMask
    }}
  },
// D18F0x6C - Link Initialization Control
// bits[0]       RouteTblDis         = 0x0
  {
    PciRegister,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ALL                           // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_0, 0x6C),    // Address
      0x00000000,                           // RegData
      0x00000001,                           // RegMask
    }}
  },
// D18F0x90 - Upstream Base Channel Buffer Count
// bits[27:25]   FreeData            = 0x0
// bits[24:20]   FreeCmd             = 0x0
// bits[19:18]   RspData             = 0x1
// bits[17:16]   NpReqData           = 0x1
// bits[15:12]   ProbeCmd            = 0x0
// bits[11:8]    RspCmd              = 0x2
// bits[7:5]     PReq                = 0x5
// bits[4:0]     NpReqCmd            = 0x8
  {
    PciRegister,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ALL                           // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_0, 0x90),    // Address
      0x000502A8,                           // RegData
      0x0FFFFFFF,                           // RegMask
    }}
  },
// D18F0x94 - Link Isochronous Channel Buffer Count
// bits[28:27]   IsocRspData         = 0x0
// bits[26:25]   IsocNpReqData       = 0x1
// bits[24:22]   IsocRspCmd          = 0x0
// bits[21:19]   IsocPReq            = 0x0
// bits[18:16]   IsocNpReqCmd        = 0x1
  {
    PciRegister,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ALL                           // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_0, 0x94),    // Address
      0x02010000,                           // RegData
      0x1FFF0000,                           // RegMask
    }}
  },
// D18F3x6C - Data Buffer Count
// bits[30:28]   IsocRspDBC          = 0x1
// bits[18:16]   UpRspDBC            = 0x1
// bits[7:6]     DnRspDBC            = 0x1
// bits[5:4]     DnReqDBC            = 0x1
// bits[2:0]     UpReqDBC            = 0x2
  {
    PciRegister,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ALL                           // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x6C),    // Address
      0x10010052,                           // RegData
      0x700700F7,                           // RegMask
    }}
  },
// D18F3xA0 - Power Control Miscellaneous
// bits[13:11]   PllLockTime         = 0x1
  {
    PciRegister,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ALL                           // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0xA0),    // Address
      0x00000800,                           // RegData
      0x00003800,                           // RegMask
    }}
  },
// D18F3xA4 - Reported Temperature Control
// bits[20]      TcenPwrDnCc6En      = 0x1
// bits[12:8]    PerStepTimeDn       = 0xF
// bits[7]       TmpSlewDnEn         = 0x1
// bits[6:5]     TmpMaxDiffUp        = 0x3
// bits[4:0]     PerStepTimeUp       = 0xF
  {
    PciRegister,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ALL                           // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0xA4),    // Address
      0x00100FEF,                           // RegData
      0x00101FFF,                           // RegMask
    }}
  },
// D18F3x1CC - IBS Control
// bits[8]       LvtOffsetVal        = 0x1
// bits[3:0]     LvtOffset           = 0x0
  {
    PciRegister,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ALL                           // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x1CC),   // Address
      0x00000100,                           // RegData
      0x0000010F,                           // RegMask
    }}
  },
// D18F4x15C - Core Performance Boost Control
// bits[1:0]     BoostSrc            = 0
  {
    PciRegister,
    {
      AMD_FAMILY_16,                        // CpuFamily
      AMD_F16_ALL                           // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_4, 0x15C),   // Address
      0x00000000,                           // RegData
      0x00000003,                           // RegMask
    }}
  },
};

CONST REGISTER_TABLE ROMDATA F16PciUnknownRegisterTable = {
  PrimaryCores,
  (sizeof (F16PciUnknownRegisters) / sizeof (TABLE_ENTRY_FIELDS)),
  F16PciUnknownRegisters,
};
