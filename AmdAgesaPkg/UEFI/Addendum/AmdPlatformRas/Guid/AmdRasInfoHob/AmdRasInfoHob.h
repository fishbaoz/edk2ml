/* $NoKeywords: $ */
 /**
 * @file
 *
 * Amd Ras Info Hob GUID
 *
 * Contains GUID Declaration for Memory Info Hob
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
 * Description: AmdRasInfoHob.h - RAS Info Hob GUID
 *
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
 */

#ifndef _AMD_RAS_INFO_HOB_H_
#define _AMD_RAS_INFO_HOB_H_

#ifdef __cplusplus
  extern "C" {
  #endif

#define MAX_RAS_SOCKETS   4           // Max Sockets per platform
#define MAX_RAS_CORES     20          // Max 20 cores per socket

// {BA00F2B6-3D87-458e-B544-7D6B041DDFC9}
#define AMD_RAS_INFO_HOB_GUID \
  { 0xba00f2b6, 0x3d87, 0x458e, 0xb5, 0x44, 0x7d, 0x6b, 0x4, 0x1d, 0xdf, 0xc9 }

  extern EFI_GUID gAmdRasInfoHobGuid;

  #pragma pack (push, 1)
/// MCA Status Descriptor structure for each Core
  typedef enum _ERROR_CODE {
    UNCORR_MC4_MEM = 0,  ///< Uncorrectable MC4 Memory
    UNCORR_MC4_L3,       ///< Uncorrectable MC4 L3
    UNCORR_MC4_LINKS,    ///< Uncorrectable MC4 links
    UNCORR_CORE,         ///< Uncorrectable core
    UNCORR_GNB_PARITY,   ///< Uncorrectable GNB parity
    UNCORR_SB_PARITY,    ///< Uncorrectable SB parity
    UNCORR_SATA,         ///< Uncorrectable SATA
    UNCORR_USB           ///< Uncorrectable USB
  } ERROR_CODE;

/// MCA Status Descriptor structure for each Core
  typedef struct _AMD_MCA_STAT_DESCRIPTOR {
    UINT8       CoreNum;                    ///< Core Number associated with this MCA descriptor
    UINT8       NodeNum;                    ///< Node Number associated with this MCA descriptor
    UINT32      MC_Control;                 ///< MC Control MSR
    UINT32      MC_StatusHi;                ///< MC Status MSR High 32=bits
    UINT32      MC_StatusLow;               ///< MC Status MSR Low 32-bits
    UINT32      MC_AddressHi;               ///< MC Address MSR High 32=bits
    UINT32      MC_AddressLo;               ///< MC Address MSR Low 32=bits
  } AMD_MCA_STAT_DESCRIPTOR;

/// Processor SocketMCA Status Descriptor structure for each Core
  typedef struct _AMD_CPU_RAS_SOCKET {
    UINT8                     CpuSocketNum; ///< CPU Socket Number associated with this socket data
    AMD_MCA_STAT_DESCRIPTOR   McaStatDesc;  ///< RAS HOB Descirptor Array
  } AMD_CPU_RAS_SOCKET;

/// RAS info HOB structure
  typedef struct _AMD_RAS_INFO_HOB {
    UINT32              Version;            ///< Version of HOB structure
    BOOLEAN             ErrorPresent;       ///< Is a valide Error Present?
    ERROR_CODE          ErrorCode;          ///< Enumerated Error Code associated with this error
    BOOLEAN             ClearError;         ///< Should the error be cleared?
    AMD_CPU_RAS_SOCKET  RasCpuSocket;       ///< RAS HOB Descirptor Array
  } AMD_RAS_INFO_HOB;

  #pragma pack (pop)

#define AMD_RAS_INFO_HOB_VERISION                 0x00000100ul  // Ver: 00.00.01.00

#define MAX_SIZEOF_AMD_RAS_INFO_HOB_BUFFER        sizeof (AMD_RAS_INFO_HOB)

/****** DO NOT WRITE BELOW THIS LINE *******/
  #ifdef __cplusplus
  }
#endif
#endif // _AMD_RAS_INFO_HOB_H_

