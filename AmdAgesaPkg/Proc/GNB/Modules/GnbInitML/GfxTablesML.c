/* $NoKeywords:$ */
/**
 * @file
 *
 * GFx tables
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 284868 $   @e \$Date: 2014-02-14 17:24:48 -0600 (Fri, 14 Feb 2014) $
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
#include  "cpuFamilyTranslation.h"

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

GNB_TABLE ROMDATA GfxGmcClockGatingDisableML [] = {
  //2.1 Disable clock-gating
  GNB_ENTRY_WR (GMMx20C0_TYPE, GMMx20C0_ADDRESS, 0x00000C80),
  GNB_ENTRY_WR (GMMx2478_TYPE, GMMx2478_ADDRESS, 0x00000400),
  GNB_ENTRY_WR (GMMx20B8_TYPE, GMMx20B8_ADDRESS, 0x00000400),
  GNB_ENTRY_WR (GMMx20BC_TYPE, GMMx20BC_ADDRESS, 0x00000400),
  GNB_ENTRY_WR (GMMx2648_TYPE, GMMx2648_ADDRESS, 0x00000400),
  GNB_ENTRY_WR (GMMx264C_TYPE, GMMx264C_ADDRESS, 0x00000400),
  GNB_ENTRY_WR (GMMx2650_TYPE, GMMx2650_ADDRESS, 0x00000400),
  GNB_ENTRY_WR (GMMx15C0_TYPE, GMMx15C0_ADDRESS, 0x00000400),
  GNB_ENTRY_TERMINATE
};

GNB_TABLE ROMDATA GfxGmcInitTableML [] = {
  // Enable Garlic interface on UNB Through F5x178[19] = 0
  GNB_ENTRY_RMW (D18F5x178_TYPE, D18F5x178_ADDRESS, D18F5x178_SwGfxDis_MASK, 0 << D18F5x178_SwGfxDis_OFFSET),
  //2.2 System memory address translation
  GNB_ENTRY_COPY (GMMx2814_TYPE, GMMx2814_ADDRESS,  0, 32, D18F2x40_dct0_TYPE, D18F2x40_dct0_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx281C_TYPE, GMMx281C_ADDRESS,  0, 32, D18F2x44_dct0_TYPE, D18F2x44_dct0_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx2824_TYPE, GMMx2824_ADDRESS,  0, 32, D18F2x48_dct0_TYPE, D18F2x48_dct0_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx282C_TYPE, GMMx282C_ADDRESS,  0, 32, D18F2x4C_dct0_TYPE, D18F2x4C_dct0_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx2834_TYPE, GMMx2834_ADDRESS,  0, 32, D18F2x60_dct0_TYPE, D18F2x60_dct0_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx283C_TYPE, GMMx283C_ADDRESS,  0, 32, D18F2x64_dct0_TYPE, D18F2x64_dct0_ADDRESS,  0, 32),
  // MC_FUS_DRAM0_BANK_ADDR_MAPPING
  GNB_ENTRY_COPY (GMMx2844_TYPE, GMMx2844_ADDRESS,  0,  8, D18F2x80_dct0_TYPE, D18F2x80_dct0_ADDRESS,  0,  8),
  GNB_ENTRY_COPY (GMMx2844_TYPE, GMMx2844_ADDRESS,  8,  1, D18F2x94_dct0_TYPE, D18F2x94_dct0_ADDRESS, 22,  1),
  GNB_ENTRY_COPY (GMMx2844_TYPE, GMMx2844_ADDRESS,  9,  1, D18F2xA8_dct0_TYPE, D18F2xA8_dct0_ADDRESS, 20,  1),
  // MC_FUS_DRAM0_CTL_BASE
  GNB_ENTRY_COPY (GMMx284C_TYPE, GMMx284C_ADDRESS,  0,  3, D18F1x200_TYPE, D18F1x200_ADDRESS,  4,  3),
  GNB_ENTRY_COPY (GMMx284C_TYPE, GMMx284C_ADDRESS,  3,  4, D18F1x204_TYPE, D18F1x204_ADDRESS,  0,  4),
  GNB_ENTRY_COPY (GMMx284C_TYPE, GMMx284C_ADDRESS,  7, 21, D18F1x200_TYPE, D18F1x200_ADDRESS, 11, 21),
  GNB_ENTRY_COPY (GMMx284C_TYPE, GMMx284C_ADDRESS, 28,  1, D18F1x200_TYPE, D18F1x200_ADDRESS,  3,  1),
  GNB_ENTRY_COPY (GMMx284C_TYPE, GMMx284C_ADDRESS, 29,  1, D18F1x200_TYPE, D18F1x200_ADDRESS,  0,  1),
  // MC_FUS_DRAM0_CTL_LIMIT
  GNB_ENTRY_COPY (GMMx2854_TYPE, GMMx2854_ADDRESS,  0, 21, D18F1x204_TYPE, D18F1x204_ADDRESS, 11, 21),
  GNB_ENTRY_COPY (GMMx2854_TYPE, GMMx2854_ADDRESS, 21,  1, D18F1xF0_TYPE,  D18F1xF0_ADDRESS,   1,  1),
  // MC_FUS_DRAM_MODE
  GNB_ENTRY_COPY (GMMx2864_TYPE, GMMx2864_ADDRESS,  3,  1, D18F2x78_dct0_TYPE, D18F2x78_dct0_ADDRESS,  8,  1),
  GNB_ENTRY_COPY (GMMx2864_TYPE, GMMx2864_ADDRESS,  4,  9, D18F1xF0_TYPE,      D18F1xF0_ADDRESS,       7,  9),
  GNB_ENTRY_COPY (GMMx2864_TYPE, GMMx2864_ADDRESS,  0,  2, D18F2x110_TYPE,     D18F2x110_ADDRESS,      6,  2),
  GNB_ENTRY_COPY (GMMx2864_TYPE, GMMx2864_ADDRESS,  2,  1, D18F2x114_TYPE,     D18F2x114_ADDRESS,      9,  1),
  // MC_FUS_DRAM_CTL_HIGH_01
  GNB_ENTRY_COPY (GMMx285C_TYPE, GMMx285C_ADDRESS,  0, 12, D18F1x240_TYPE, D18F1x240_ADDRESS, 11, 12),

  // 2.4 Sequencer model programming
  GNB_ENTRY_WR (GMMx276C_TYPE, GMMx276C_ADDRESS, 0x000003ff),

  //---------------------------------------------------------------------------
  // 2.5 Power gating init
  // Initializing PGFSMs
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0x200010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0x300010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x210000),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0xa00010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x21003),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0xb00010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x2b00),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0xc00010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0xd00010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x420000),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0x100010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x120202),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0x500010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x3e3e36),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0x600010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x373f3e),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0x700010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x3e1332),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0xe00010ff),
//---------------------------------------------------------------------------
// Initializing register engine
// opcode=1, body_cnt=0, mask=0xf, const=0xf: MC_CONFIG (0x800) MC_CONFIG (0x800)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10000800),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xf),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xf),
// opcode=1, body_cnt=0, mask=0x3f, const=0x3f: MC_CONFIG_MCD (0x828) MC_CONFIG_MCD (0x828)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x4),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10000828),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x3f),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x3f),
// opcode=1, body_cnt=0, mask=0xffff, const=0xffff: VM_INVALIDATE_REQUEST (0x51e) VM_INVALIDATE_REQUEST (0x51e)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x8),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x1000051e),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xffff),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xffff),
// opcode=0, body_cnt=2: VM_L2_CNTL (0x500) VM_L2_CNTL3 (0x502)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0xc),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x20500),
// opcode=0, body_cnt=4: VM_CONTEXT0_CNTL (0x504) VM_SECURE_FAULT_CNTL (0x508)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x10),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x40504),
// opcode=0, body_cnt=9: VM_CONTEXT0_CNTL2 (0x50c) VM_CONTEXT15_PAGE_TABLE_BASE_ADDR (0x515)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x16),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x9050c),
// opcode=0, body_cnt=9: VM_PRT_APERTURE0_LOW_ADDR (0x52c) VM_CONTEXTS_DISABLE (0x535)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x21),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x9052c),
// opcode=0, body_cnt=1: VM_CONTEXT0_PROTECTION_FAULT_DEFAULT_ADDR (0x546) VM_CONTEXT1_PROTECTION_FAULT_DEFAULT_ADDR (0x547)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x2c),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10546),
// opcode=0, body_cnt=10: VM_FAULT_CLIENT_ID (0x54e) VM_CONTEXT1_PAGE_TABLE_START_ADDR (0x558)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x2f),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xa054e),
// opcode=0, body_cnt=1: VM_CONTEXT0_PAGE_TABLE_END_ADDR (0x55f) VM_CONTEXT1_PAGE_TABLE_END_ADDR (0x560)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x3b),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x1055f),
// opcode=0, body_cnt=1: VM_DEBUG (0x56f) VM_L2_CG (0x570)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x3e),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x1056f),
// opcode=0, body_cnt=1: VM_L2_BANK_SELECT_MASKA (0x572) VM_L2_BANK_SELECT_MASML (0x573)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x41),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10572),
// opcode=0, body_cnt=2: VM_L2_CONTEXT1_IDENTITY_APERTURE_LOW_ADDR (0x575) VM_L2_CONTEXT_IDENTITY_PHYSICAL_OFFSET (0x577)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x44),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x20575),
// opcode=0, body_cnt=53: MC_CITF_PERFCOUNTER_LO (0x7a0) ATC_PERFCOUNTER_RSLT_CNTL (0x7d5)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x48),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x3507a0),
// opcode=0, body_cnt=0: MC_ARB_PERF_MON_CNTL0_ECC (0x7db) MC_ARB_PERF_MON_CNTL0_ECC (0x7db)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x7f),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x7db),
// opcode=0, body_cnt=26: MC_SHARED_CHMAP (0x801) MC_VM_STEERING (0x81b)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x81),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x1a0801),
// opcode=0, body_cnt=2: MC_CG_CONFIG_MCD (0x829) MC_SHARED_BLACKOUT_CNTL (0x82b)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x9d),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x20829),
// opcode=0, body_cnt=4: MC_HUB_MISC_POWER (0x82d) MC_HUB_MISC_DBG (0x831)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0xa1),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x4082d),
// opcode=0, body_cnt=4: MC_HUB_MISC_OVERRIDE (0x833) MC_HUB_WDP_BP (0x837)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0xa7),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x40833),
// opcode=0, body_cnt=11: MC_HUB_RDREQ_CNTL (0x83b) MC_HUB_SHARED_DAGB_DLY (0x846)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0xad),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xb083b),
// opcode=0, body_cnt=1: MC_HUB_RDREQ_DMIF_LIMIT (0x848) MC_HUB_RDREQ_ACPG_LIMIT (0x849)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0xba),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10848),
// opcode=0, body_cnt=63: MC_HUB_WDP_SH2 (0x84d) MC_HUB_WDP_SAM (0x88c)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0xbd),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x3f084d),
// opcode=0, body_cnt=0: MC_VM_MB_L1_TLB0_DEBUG (0x891) MC_VM_MB_L1_TLB0_DEBUG (0x891)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0xfe),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x891),
// opcode=0, body_cnt=0: MC_VM_MB_L1_TLB2_DEBUG (0x893) MC_VM_MB_L1_TLB2_DEBUG (0x893)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x100),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x893),
// opcode=0, body_cnt=0: MC_VM_MB_L2ARBITER_L2_CREDITS (0x8a1) MC_VM_MB_L2ARBITER_L2_CREDITS (0x8a1)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x102),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x8a1),
// opcode=0, body_cnt=0: MC_VM_MB_L1_TLB3_DEBUG (0x8a5) MC_VM_MB_L1_TLB3_DEBUG (0x8a5)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x104),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x8a5),
// opcode=0, body_cnt=51: MC_XPB_RTR_SRC_APRTR0 (0x8cd) MC_XPB_UNC_THRESH_SID (0x900)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x106),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x3308cd),
// opcode=0, body_cnt=1: MC_XPB_WCB_CFG (0x902) MC_XPB_P2P_BAR_CFG (0x903)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x13b),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10902),
// opcode=0, body_cnt=19: MC_XPB_P2P_BAR_SETUP (0x90c) MC_XPB_INTF_CFG (0x91f)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x13e),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x13090c),
// opcode=0, body_cnt=0: MC_XPB_SUB_CTRL (0x922) MC_XPB_SUB_CTRL (0x922)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x153),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x922),
// opcode=0, body_cnt=0: MC_XPB_PERF_KNOBS (0x924) MC_XPB_PERF_KNOBS (0x924)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x155),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x924),
// opcode=0, body_cnt=20: MC_XPB_STICKY_W1C (0x926) MC_XPB_CLG_CFG36 (0x93a)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x157),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x140926),
// opcode=2, body_cnt=0: MC_RPB_CID_QUEUE_EX (0x95a)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x16d),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x2000095a),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x1),
// opcode=3, body_cnt=31: MC_RPB_CID_QUEUE_EX_DATA (0x95b)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x16f),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x301f095b),
// opcode=0, body_cnt=12: MC_RPB_CONF (0x94d) MC_RPB_PERF_COUNTER_STATUS (0x959)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x190),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xc094d),
// opcode=0, body_cnt=16: MC_CITF_XTRA_ENABLE (0x96d) MC_CITF_INT_CREDITS_WR (0x97d)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x19e),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10096d),
// opcode=0, body_cnt=12: MC_CITF_WTM_RD_CNTL (0x97f) MC_WR_GRP_LCL (0x98b)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x1b0),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xc097f),
// opcode=0, body_cnt=0: MC_CITF_PERF_MON_CNTL2 (0x98e) MC_CITF_PERF_MON_CNTL2 (0x98e)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x1be),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x98e),
// opcode=0, body_cnt=2: MC_CITF_MISC_RD_CG (0x992) MC_CITF_MISC_VM_CG (0x994)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x1c0),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x20992),
// opcode=0, body_cnt=2: MC_VM_MD_L1_TLB0_DEBUG (0x998) MC_VM_MD_L1_TLB2_DEBUG (0x99a)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x1c4),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x20998),
// opcode=0, body_cnt=0: MC_VM_MD_L2ARBITER_L2_CREDITS (0x9a4) MC_VM_MD_L2ARBITER_L2_CREDITS (0x9a4)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x1c8),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x9a4),
// opcode=0, body_cnt=0: MC_VM_MD_L1_TLB3_DEBUG (0x9a7) MC_VM_MD_L1_TLB3_DEBUG (0x9a7)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x1ca),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x9a7),
// opcode=0, body_cnt=6: MC_ARB_AGE_CNTL (0x9bf) MC_ARB_GECC2_DEBUG2 (0x9c5)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x1cc),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x609bf),
// opcode=0, body_cnt=45: MC_ARB_GECC2 (0x9c9) MC_ARB_MAX_LAT_CID (0x9f6)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x1d4),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x2d09c9),
// opcode=0, body_cnt=3: MC_ARB_SSM (0x9f9) MC_ARB_DRAM_TIMING_1 (0x9fc)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x203),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x309f9),
// opcode=0, body_cnt=0: MC_ARB_DRAM_TIMING2_1 (0x9ff) MC_ARB_DRAM_TIMING2_1 (0x9ff)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x208),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x9ff),
// opcode=0, body_cnt=2: MC_ARB_BURST_TIME (0xa02) MC_ARB_SCRAMBLE_KEY1 (0xa04)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x20a),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x20a02),
// opcode=0, body_cnt=6: MC_FUS_DRAM0_CTL_BASE (0xa13) MC_FUS_DRAM_MODE (0xa19)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x20e),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x60a13),
// opcode=0, body_cnt=3: MC_FUS_ARB_GARLIC_ISOC_PRI (0xa1f) MC_FUS_ARB_GARLIC_WR_PRI2 (0xa22)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x216),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x30a1f),
// opcode=1, body_cnt=0, mask=0x3f, const=0x0: ATC_ATS_FAULT_CNTL (0xccd) ATC_ATS_FAULT_CNTL (0xccd)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x21b),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10000ccd),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x3f),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x0),
// opcode=0, body_cnt=7: ATC_VM_APERTURE0_LOW_ADDR (0xcc0) ATC_VM_APERTURE1_CNTL2 (0xcc7)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x21f),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x70cc0),
// opcode=0, body_cnt=2: ATC_ATS_CNTL (0xcc9) ATC_ATS_FAULT_DEBUG (0xccb)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x228),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x20cc9),
// opcode=0, body_cnt=1: ATC_ATS_DEFAULT_PAGE_LOW (0xcd0) ATC_ATS_DEFAULT_PAGE_CNTL (0xcd1)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x22c),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10cd0),
// opcode=0, body_cnt=4: ATC_MISC_CG (0xcd4) ATC_L2_DEBUG2 (0xcd8)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x22f),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x40cd4),
// opcode=0, body_cnt=5: ATC_L1_CNTL (0xcdc) ATC_L1WR_STATUS (0xce1)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x235),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x50cdc),
// opcode=0, body_cnt=16: ATC_VMID_PASID_MAPPING_UPDATE_STATUS (0xce6) ATC_VMID15_PASID_MAPPING (0xcf6)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x23c),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x100ce6),
// opcode=0, body_cnt=0: ATC_ATS_FAULT_CNTL (0xccd) ATC_ATS_FAULT_CNTL (0xccd)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x24e),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xccd),
// opcode=0, body_cnt=23: MC_ARB_HARSH_EN_RD (0xdc0) MC_ARB_HARSH_CTL_WR (0xdd7)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x250),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x170dc0),
// opcode=0, body_cnt=0: MC_CONFIG (0x800) MC_CONFIG (0x800)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x269),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x800),
// opcode=0, body_cnt=0: MC_CONFIG_MCD (0x828) MC_CONFIG_MCD (0x828)
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x26b),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x828),
//---------------------------------------------------------------------------
// Setting up end pointers
  GNB_ENTRY_RMW (GMMx3508_TYPE, GMMx3508_ADDRESS, 0xfffff000, 0x9b26c000),
//---------------------------------------------------------------------------
// Disabling light sleep
  GNB_ENTRY_RMW (GMMx3544_TYPE, GMMx3544_ADDRESS, 0x6000000, 0x0000000),

  // 2.7 Performance tuning
  GNB_ENTRY_WR (GMMx27D0_TYPE, GMMx27D0_ADDRESS, 0x10724847),
  GNB_ENTRY_WR (GMMx27C0_TYPE, GMMx27C0_ADDRESS, 0x00C32008),
  GNB_ENTRY_WR (GMMx27C4_TYPE, GMMx27C4_ADDRESS, 0x00C32006),
  GNB_ENTRY_WR (GMMx277C_TYPE, GMMx277C_ADDRESS, 0x00000007),
  GNB_ENTRY_WR (GMMx218C_TYPE, GMMx218C_ADDRESS, 0x000021b1),
  GNB_ENTRY_WR (GMMx201C_TYPE, GMMx201C_ADDRESS, 0x47773337),
  GNB_ENTRY_WR (GMMx2020_TYPE, GMMx2020_ADDRESS, 0x73773337),
  GNB_ENTRY_WR (GMMx2018_TYPE, GMMx2018_ADDRESS, 0x66334303),
  GNB_ENTRY_WR (GMMx2014_TYPE, GMMx2014_ADDRESS, 0x66300333),
  GNB_ENTRY_WR (GMMx2794_TYPE, GMMx2794_ADDRESS, 0xfcfcfdfc),
  GNB_ENTRY_WR (GMMx2798_TYPE, GMMx2798_ADDRESS, 0xfcfcfdfc),
  GNB_ENTRY_WR (GMMx27A4_TYPE, GMMx27A4_ADDRESS, 0x00ffffff),
  GNB_ENTRY_WR (GMMx27A8_TYPE, GMMx27A8_ADDRESS, 0x00ffffff),
  GNB_ENTRY_WR (GMMx278C_TYPE, GMMx278C_ADDRESS, 0x00000008),
  GNB_ENTRY_WR (GMMx2790_TYPE, GMMx2790_ADDRESS, 0x00000008),
  GNB_ENTRY_WR (GMMx2628_TYPE, GMMx2628_ADDRESS, 0x55111000),
  GNB_ENTRY_WR (GMMx25E0_TYPE, GMMx25E0_ADDRESS, 0x00000025),
  GNB_ENTRY_WR (GMMx262C_TYPE, GMMx262C_ADDRESS, 0x10555111),
  GNB_ENTRY_WR (GMMx25E4_TYPE, GMMx25E4_ADDRESS, 0x00000025),
  GNB_ENTRY_WR (GMMx25C8_TYPE, GMMx25C8_ADDRESS, 0x0080685F),
  GNB_ENTRY_WR (GMMx25CC_TYPE, GMMx25CC_ADDRESS, 0x0000807F),
  GNB_ENTRY_WR (GMMx2144_TYPE, GMMx2144_ADDRESS, 0x50A1421D),
  GNB_ENTRY_WR (GMMx20EC_TYPE, GMMx20EC_ADDRESS, 0x0000001C),
  GNB_ENTRY_WR (GMMx2184_TYPE, GMMx2184_ADDRESS, 0x0000A1F1),
  GNB_ENTRY_WR (GMMx21E0_TYPE, GMMx21E0_ADDRESS, 0x0000A1F1),
  GNB_ENTRY_WR (GMMx217C_TYPE, GMMx217C_ADDRESS, 0x0000A1F1),
  GNB_ENTRY_WR (GMMx21C0_TYPE, GMMx21C0_ADDRESS, 0x0000A1F1),
  GNB_ENTRY_WR (GMMx2214_TYPE, GMMx2214_ADDRESS, 0x000021B1),
  GNB_ENTRY_WR (GMMx2220_TYPE, GMMx2220_ADDRESS, 0x000021B1),
  GNB_ENTRY_WR (GMMx3758_TYPE, GMMx3758_ADDRESS, 0x00000800),
  GNB_ENTRY_WR (GMMx375C_TYPE, GMMx375C_ADDRESS, 0x00000800),
  GNB_ENTRY_WR (GMMx3700_TYPE, GMMx3700_ADDRESS, 0xD1000000),
  GNB_ENTRY_WR (GMMx3704_TYPE, GMMx3704_ADDRESS, 0xD0000000),
  GNB_ENTRY_WR (GMMx3748_TYPE, GMMx3748_ADDRESS, 0x0000007F),
  GNB_ENTRY_WR (GMMx3750_TYPE, GMMx3750_ADDRESS, 0x18201015),
  GNB_ENTRY_WR (GMMx3754_TYPE, GMMx3754_ADDRESS, 0x18201015),
  GNB_ENTRY_WR (GMMx253C_TYPE, GMMx253C_ADDRESS, 0x000000B4),
  GNB_ENTRY_WR (GMMx2550_TYPE, GMMx2550_ADDRESS, 0x000000B3),
  GNB_ENTRY_WR (GMMx2558_TYPE, GMMx2558_ADDRESS, 0x00002077),
  GNB_ENTRY_WR (GMMx2558_TYPE, GMMx2558_ADDRESS, 0x000020B7),
  GNB_ENTRY_WR (GMMx2558_TYPE, GMMx2558_ADDRESS, 0x00002A3D),
  GNB_ENTRY_WR (GMMx2558_TYPE, GMMx2558_ADDRESS, 0x00002010),
  GNB_ENTRY_WR (GMMx2558_TYPE, GMMx2558_ADDRESS, 0x00002A20),
  GNB_ENTRY_WR (GMMx2544_TYPE, GMMx2544_ADDRESS, 0x00040108),

  //2.11 Garlic credits
  GNB_ENTRY_WR (GMMx287C_TYPE, GMMx287C_ADDRESS, 0x0c000911),

  //2.9 Display latency
  GNB_ENTRY_WR (GMMx2114_TYPE, GMMx2114_ADDRESS, 0x00000015),

  //2.10 Garlic EDC disable
  GNB_ENTRY_WR (GMMx2880_TYPE, GMMx2880_ADDRESS, 0xFCFE8000),

  //2.12 Remove blackout
  GNB_ENTRY_WR (GMMx20AC_TYPE, GMMx20AC_ADDRESS, 0x00000000),

  // VCE Optimization
  GNB_ENTRY_WR (GMMx2768_TYPE, GMMx2768_ADDRESS, 0x0000F0F0),
  GNB_ENTRY_RMW (GMMx2108_TYPE, GMMx2108_ADDRESS, GMMx2108_STALL_THRESHOLD_MASK, (1 << GMMx2108_STALL_THRESHOLD_OFFSET)),
  GNB_ENTRY_WR (GMMx21FC_TYPE, GMMx21FC_ADDRESS, 0xA1F1),
  GNB_ENTRY_WR (GMMx2198_TYPE, GMMx2198_ADDRESS, 0xA1F1),

  // STCTRL_IGNORE_PROTECTION_FAULT
  GNB_ENTRY_RMW (GMMx350C_TYPE, GMMx350C_ADDRESS, GMMx350C_STCTRL_IGNORE_PROTECTION_FAULT_MASK, (1 << GMMx350C_STCTRL_IGNORE_PROTECTION_FAULT_OFFSET)),

  GNB_ENTRY_RMW (GMMx5490_TYPE, GMMx5490_ADDRESS, GMMx5490_FB_WRITE_EN_MASK | GMMx5490_FB_READ_EN_MASK, (1 << GMMx5490_FB_READ_EN_OFFSET) | (1 << GMMx5490_FB_WRITE_EN_OFFSET)),

  //Sets the threshold.
  GNB_ENTRY_RMW (GMMx3510_TYPE, GMMx3510_ADDRESS, GMMx3510_STCTRL_NONDISP_IDLE_THRESHOLD_MASK, 2 << GMMx3510_STCTRL_NONDISP_IDLE_THRESHOLD_OFFSET),

  GNB_ENTRY_TERMINATE
};

GNB_TABLE ROMDATA GfxGmcClockGatingEnableML [] = {
  GNB_ENTRY_WR (GMMx20C0_TYPE, GMMx20C0_ADDRESS, 0x000c0c80),
  GNB_ENTRY_WR (GMMx2478_TYPE, GMMx2478_ADDRESS, 0x000c0400),
  GNB_ENTRY_WR (GMMx20B8_TYPE, GMMx20B8_ADDRESS, 0x000c0400),
  GNB_ENTRY_WR (GMMx20BC_TYPE, GMMx20BC_ADDRESS, 0x000c0400),
  GNB_ENTRY_WR (GMMx2648_TYPE, GMMx2648_ADDRESS, 0x00080400),
  GNB_ENTRY_WR (GMMx264C_TYPE, GMMx264C_ADDRESS, 0x000c0400),
  GNB_ENTRY_WR (GMMx2650_TYPE, GMMx2650_ADDRESS, 0x000c0400),
  GNB_ENTRY_WR (GMMx15C0_TYPE, GMMx15C0_ADDRESS, 0x000c0400),
  GNB_ENTRY_TERMINATE
};

GNB_TABLE ROMDATA GfxEnvInitTableML [] = {
  GNB_ENTRY_TERMINATE
};
