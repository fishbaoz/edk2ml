/* $NoKeywords:$ */
/**
 * @file
 *
 * iTpmSmm Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD PSP UEFI Driver
 * @e sub-project   iTpmSmm Driver
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*****************************************************************************
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

#ifndef _PSP_SMM_H_
#define _PSP_SMM_H_

#include "Tiano.h"
#include "EfiDebug.h"
#include "EfiCombinationLib.h"
#include "EfiSmmDriverLib.h"
#include "CpuIa32.h"
#include "PspBaseLib.h"
#include "PspMbox.h"
#include "PspResumeMemInfo.h"
#include "PspPlatformProtocol.h"

#include EFI_PROTOCOL_DEPENDENCY (SmmBase)
#include EFI_PROTOCOL_CONSUMER   (FchSmmSxDispatch)
#include EFI_PROTOCOL_CONSUMER   (FchSmmSwDispatch)

#define   SMM_SAVE_SIZE   0x200

#define MAX_AP_COUNT                  3
#define SMM_RESUME_HEAP_SIZE          (1024 * 64 * 4)
#define SMM_RESUME_TEMP_REGION_SIZE   (1024 * 1)


typedef struct _SMM_SAVE_AREA_CONTEXT SMM_SAVE_AREA_CONTEXT;

/// Used to keep SMM context
typedef struct _SMM_SAVE_AREA_CONTEXT {
  VOID     *SaveAreaOffset;  ///< Point to SMM Context
  UINT8    SaveAreaBuffer[SMM_SAVE_SIZE]; ///< SMM Context buffer
} SMM_SAVE_AREA_CONTEXT;

extern EFI_SMM_SYSTEM_TABLE                  *mSmst;

extern SMM_SAVE_AREA_CONTEXT *mSmmSaveAreaCopy;

extern PSP_PLATFORM_PROTOCOL  mPspPlatformProtocolCopy;
extern PSP_SMM_RSM_MEM_INFO   *mPspSmmRsmMemInfoPtr;


EFI_STATUS
CopySmmSaveAreaToLocal (
  IN OUT   VOID **SmmSaveStart
  );

#endif  //_PSP_SMM_H_
