/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU Register Table Related Functions
 *
 * Contains the definition of the CPU CPUID MSRs and PCI registers with BKDG recommended values
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU
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

#ifndef _CPU_FAM_REGISTERS_H_
#define _CPU_FAM_REGISTERS_H_

/*
 *--------------------------------------------------------------
 *
 *                      M O D U L E S    U S E D
 *
 *---------------------------------------------------------------
 */

/*
 *--------------------------------------------------------------
 *
 *                      D E F I N I T I O N S  /  M A C R O S
 *
 *---------------------------------------------------------------
 */

// This define should be equal to the total number of families
// in the cpuFamily enum.
#define MAX_CPU_FAMILIES    64
#define MAX_CPU_REVISIONS   63    // Max Cpu Revisions Per Family

// CPU_LOGICAL_ID.Family equates
// Family 15h equates
#define AMD_FAMILY_15_TN  0x0000000000000200ull
#define AMD_FAMILY_TN     (AMD_FAMILY_15_TN)
#define AMD_FAMILY_15_RL  (AMD_FAMILY_15_TN)
#define AMD_FAMILY_RL     (AMD_FAMILY_15_TN)
#define AMD_FAMILY_15_KV  0x0000000000000400ull
#define AMD_FAMILY_KV     (AMD_FAMILY_15_KV)
#define AMD_FAMILY_15_CZ  0x0000000000000800ull
#define AMD_FAMILY_CZ     (AMD_FAMILY_15_CZ)
#define AMD_FAMILY_15     (AMD_FAMILY_15_TN | AMD_FAMILY_15_KV | AMD_FAMILY_15_CZ)

// Family 16h equates
#define AMD_FAMILY_16_KB  0x0000000000010000ull
#define AMD_FAMILY_KB     AMD_FAMILY_16_KB
#define AMD_FAMILY_16_ML  0x0000000000020000ull
#define AMD_FAMILY_ML     AMD_FAMILY_16_ML
#define AMD_FAMILY_16     (AMD_FAMILY_KB | AMD_FAMILY_ML)

// Family Unknown
#define AMD_FAMILY_UNKNOWN 0x8000000000000000ull


// Family 15h CPU_LOGICAL_ID.Revision equates
// -------------------------------------

  // Family 15h TN steppings
#define AMD_F15_TN_A0    0x0000000000000100ull
#define AMD_F15_TN_A1    0x0000000000000200ull
#define AMD_F15_RL_A1    0x0000000000000400ull
  // Family 15h KV steppings
#define AMD_F15_KV_A0    0x0000000000100000ull
#define AMD_F15_KV_A1    0x0000000000200000ull
  // Family 15h CZ steppings
#define AMD_F15_CZ_A0    0x0000000001000000ull
  // Family 15h Unknown stepping
#define AMD_F15_UNKNOWN  0x8000000000000000ull

#define AMD_F15_RL_Ax    (AMD_F15_RL_A1)
#define AMD_F15_RL_ALL   (AMD_F15_RL_Ax)

#define AMD_F15_TN_Ax    (AMD_F15_TN_A0 | AMD_F15_TN_A1)
#define AMD_F15_TN_GT_A0 (AMD_F15_TN_ALL & ~AMD_F15_TN_A0)
#define AMD_F15_TN_ONLY  (AMD_F15_TN_Ax)
#define AMD_F15_TN_ALL   (AMD_F15_TN_Ax | AMD_F15_RL_ALL)

#define AMD_F15_KV_Ax    (AMD_F15_KV_A0 | AMD_F15_KV_A1)
#define AMD_F15_KV_ALL   (AMD_F15_KV_Ax)

#define AMD_F15_CZ_Ax    (AMD_F15_CZ_A0)
#define AMD_F15_CZ_ALL   (AMD_F15_CZ_Ax)

#define AMD_F15_ALL      (AMD_F15_TN_ALL | AMD_F15_KV_ALL | AMD_F15_CZ_ALL)

// Family 16h CPU_LOGICAL_ID.Revision equates
// -------------------------------------

  // Family 16h KB steppings
#define AMD_F16_KB_A0    0x0000000000000001ull
#define AMD_F16_KB_A1    0x0000000000000002ull
  // Family 16h ML steppings
#define AMD_F16_ML_A0    0x0000000000000100ull
#define AMD_F16_ML_A1    0x0000000000000200ull
  // Family 16h Unknown stepping
#define AMD_F16_UNKNOWN  0x8000000000000000ull

#define AMD_F16_KB_Ax    (AMD_F16_KB_A0 | AMD_F16_KB_A1)
#define AMD_F16_KB_ALL   (AMD_F16_KB_Ax)

#define AMD_F16_ML_Ax    (AMD_F16_ML_A0 | AMD_F16_ML_A1)
#define AMD_F16_ML_ALL   (AMD_F16_ML_Ax)
#define AMD_F16_ML_GT_A0 (AMD_F16_ML_ALL & ~AMD_F16_ML_A0)

#define AMD_F16_ALL      (AMD_F16_KB_ALL | AMD_F16_ML_ALL)

#endif      // _CPU_FAM_REGISTERS_H_

