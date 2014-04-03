/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD Bit Definitions for RAS
 *
 * Contains the bit field definitions for RAS
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI/Addendum/AmdPlatformRas/RAS
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

#ifndef _BIT_DEFINE_H_
#define _BIT_DEFINE_H_

#ifdef __cplusplus
  extern "C" {
  #endif


/*
 *--------------------------------------------------------------
 *
 *                      D E F I N I T I O N S  /  M A C R O S
 *
 *---------------------------------------------------------------
 */

#define BIT0        0x0000000000000001ull
#define BIT1        0x0000000000000002ull
#define BIT2        0x0000000000000004ull
#define BIT3        0x0000000000000008ull
#define BIT4        0x0000000000000010ull
#define BIT5        0x0000000000000020ull
#define BIT6        0x0000000000000040ull
#define BIT7        0x0000000000000080ull
#define BIT8        0x0000000000000100ull
#define BIT9        0x0000000000000200ull
#define BIT10       0x0000000000000400ull
#define BIT11       0x0000000000000800ull
#define BIT12       0x0000000000001000ull
#define BIT13       0x0000000000002000ull
#define BIT14       0x0000000000004000ull
#define BIT15       0x0000000000008000ull
#define BIT16       0x0000000000010000ull
#define BIT17       0x0000000000020000ull
#define BIT18       0x0000000000040000ull
#define BIT19       0x0000000000080000ull
#define BIT20       0x0000000000100000ull
#define BIT21       0x0000000000200000ull
#define BIT22       0x0000000000400000ull
#define BIT23       0x0000000000800000ull
#define BIT24       0x0000000001000000ull
#define BIT25       0x0000000002000000ull
#define BIT26       0x0000000004000000ull
#define BIT27       0x0000000008000000ull
#define BIT28       0x0000000010000000ull
#define BIT29       0x0000000020000000ull
#define BIT30       0x0000000040000000ull
#define BIT31       0x0000000080000000ull
#define BIT32       0x0000000100000000ull
#define BIT33       0x0000000200000000ull
#define BIT34       0x0000000400000000ull
#define BIT35       0x0000000800000000ull
#define BIT36       0x0000001000000000ull
#define BIT37       0x0000002000000000ull
#define BIT38       0x0000004000000000ull
#define BIT39       0x0000008000000000ull
#define BIT40       0x0000010000000000ull
#define BIT41       0x0000020000000000ull
#define BIT42       0x0000040000000000ull
#define BIT43       0x0000080000000000ull
#define BIT44       0x0000100000000000ull
#define BIT45       0x0000200000000000ull
#define BIT46       0x0000400000000000ull
#define BIT47       0x0000800000000000ull
#define BIT48       0x0001000000000000ull
#define BIT49       0x0002000000000000ull
#define BIT50       0x0004000000000000ull
#define BIT51       0x0008000000000000ull
#define BIT52       0x0010000000000000ull
#define BIT53       0x0020000000000000ull
#define BIT54       0x0040000000000000ull
#define BIT55       0x0080000000000000ull
#define BIT56       0x0100000000000000ull
#define BIT57       0x0200000000000000ull
#define BIT58       0x0400000000000000ull
#define BIT59       0x0800000000000000ull
#define BIT60       0x1000000000000000ull
#define BIT61       0x2000000000000000ull
#define BIT62       0x4000000000000000ull
#define BIT63       0x8000000000000000ull

/****** DO NOT WRITE BELOW THIS LINE *******/
  #ifdef __cplusplus
  }
#endif
#endif      // _BIT_DEFINE_H_

