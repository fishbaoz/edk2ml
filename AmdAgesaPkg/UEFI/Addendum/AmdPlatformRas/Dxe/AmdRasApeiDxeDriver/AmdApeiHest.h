/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD APEI HEST for RAS
 *
 * Contains structures for APEI (WHEA) related ACPI HEST Hardware
 * Error Source Table definitions and support structures
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI/Addendum/AmdPlatformRas
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ****************************************************************************
 * AMD Generic Encapsulated Software Architecture
 *
 * Description: AmdApeiHest.h - Contains structures for APEI (WHEA) related ACPI
 *                          HEST Hardware Error Source Table definitions and
 *                          support structures.
 *
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

#ifndef _AMD_APEI_HEST_H_
#define _AMD_APEI_HEST_H_

#ifdef __cplusplus
  extern "C" {
  #endif

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
  #include "AmdApei.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define HEST_SIG 0x54534548ul           // "HEST" backwards
#define AMD_HEST 0x5453454820444D41ull  // "AMD HEST" backwards

#define REVISION_HEST                 0x201
#define SOURCE_ID0                    0
#define SOURCE_ID1                    1
#define RECORD_SECTION_ENABLE         0x03
#define ERR_SOURCE_ENABLED            0x01
#define RECORDS_PREALLOCATE1          0x01
#define SECTIONS_PER_RECORD1          0x01
#define NOTIFY_TYPE_NMI               0x04
#define NOTIFY_TYPE_SCI               0x03
#define NOTIFY_TYPE_POLLED            0
#define INT_VECTOR_NMI                2


//
//UEFI defined Mmeory Error type for Platform Memory Error
//
#define PLATFORM_MEM_ERROR_TYPE_SINGLE_BIT_ECC  0x02
#define PLATFORM_MEM_ERROR_TYPE_MULTI_BIT_ECC   0x03

  #pragma pack(1)



/// Uncorrectable Memory Error Status Block
  typedef struct _UNCORR_MEM_ERR_STS_BLK {
    UINT64                ErrorStatusPtr;             ///< Error Status Pointer
    GENERIC_ERR_STS_BLK   UnCorrectGenErrStsBlk;      ///< Uncorrectable Generic Error Status Block
    GEN_ERR_DATA_ENTRY    UnCorrectGenDataEntry;      ///< Uncorrectable Generic Error Data Entry
    PLATFORM_MEM_ERR_SEC  PlatformMemErrSection;      ///< Platform Memory Error Section
  } UNCORR_MEM_ERR_STS_BLK;

/// Correctable Memory Error Status Block
  typedef struct _CORR_MEM_ERR_STS_BLK {
    UINT64                 ErrorStatusPtr;             ///< Error Status Pointer
    GENERIC_ERR_STS_BLK    CorrectGenErrStsBlk;        ///< Correctable Generic Error Status Block
    GEN_ERR_DATA_ENTRY     CorrectGenDataEntry;        ///< Correctable Generic Error Data Entry
    PLATFORM_MEM_ERR_SEC   PlatformMemErrSection;      ///< Platform Memory Error Section
  } CORR_MEM_ERR_STS_BLK;


  #pragma pack()

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
  EFI_STATUS
  ApeiHestInit ( VOID );

/****** DO NOT WRITE BELOW THIS LINE *******/
  #ifdef __cplusplus
  }
#endif
#endif