/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD RAS APEI Protocol
 *
 * AMD Ras Interface Protocol GUID initialization
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI/Addendum/AmdPlatformRas/RAS
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
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
 *
 ***************************************************************************/

#ifndef _AMD_RAS_APEI_PROTOCOL_H_
#define _AMD_RAS_APEI_PROTOCOL_H_

#include "AmdApei.h"

EFI_FORWARD_DECLARATION (AMD_RAS_APEI_PROTOCOL);

//
// GUID definition
//
#define AMD_RAS_APEI_PROTOCOL_GUID \
  { 0xe9dbcc60, 0x8f93, 0x47ed, 0x84, 0x78, 0x46, 0x78, 0xf1, 0x9f, 0x73, 0x4a }
// {E9DBCC60-8F93-47ed-8478-4678F19F734A}

extern EFI_GUID gAmdRasApeiProtocolGuid;

// current PPI revision
#define AMD_RAS_APEI_REV  0x01

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
typedef struct _AMD_RAS_APEI_PROTOCOL AMD_RAS_APEI_PROTOCOL;

//Protocol prototype

typedef
struct _AMD_APEI_INTERFACE {
  APEI_DRIVER_PRIVATE_DATA  *ApeiPrivData;
} AMD_APEI_INTERFACE;


// Protocol prototype
typedef
EFI_STATUS
(EFIAPI *AMD_ADD_BOOT_ERROR_RECORD_ENTRY) (
  IN    UINT8   *ErrorRecord,
  IN    UINT32  RecordLen,
  IN    UINT8   ErrorType,
  IN    UINT8   SeverityType
);

typedef
EFI_STATUS
(EFIAPI *ADD_HEST_ERROR_SOURCE_ENTRY) (
  IN    UINT8   *pErrorRecord,
  IN    UINT32  RecordLen
);


/// RAS APEI Protocol Structure
typedef struct _AMD_RAS_APEI_PROTOCOL {
  AMD_APEI_INTERFACE               *AmdApeiInterface;        /// APEI Interface
  AMD_ADD_BOOT_ERROR_RECORD_ENTRY  AddBootErrorRecordEntry;  /// Boot error record to be added
  ADD_HEST_ERROR_SOURCE_ENTRY      AddHestErrorSourceEntry;  /// HEST error source to be added
} AMD_RAS_APEI_PROTOCOL;


#endif //_AMD_RAS_APEI_PROTOCOL_H_
