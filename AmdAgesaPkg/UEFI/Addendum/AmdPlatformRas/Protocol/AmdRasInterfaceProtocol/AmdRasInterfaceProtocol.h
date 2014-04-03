/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD RAS Interface Protocol
 *
 * AMD RAS Interface Protocol GUID initialization
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

#ifndef _AMD_RAS_INTERFACE_PROTOCOL_H_
#define _AMD_RAS_INTERFACE_PROTOCOL_H_

EFI_FORWARD_DECLARATION (AMD_RAS_DXE_INTERFACE_PROTOCOL);
//
// GUID definition
//
#define AMD_RAS_DXE_INTERFACE_PROTOCOL_GUID \
 { 0xa732cd62, 0x894f, 0x460f, 0x96, 0x12, 0x8b, 0xd, 0xac, 0x42, 0x17, 0x65 }
// {A732CD62-894F-460f-9612-8B0DAC421765}

extern EFI_GUID gAmdRasDxeInterfaceProtocolGuid;

// current PPI revision
#define AMD_RAS_INTERFACE_REV  0x01

EFI_FORWARD_DECLARATION (RAS_DEVICE_COMMON);

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
/// RAS Device IDs
typedef enum _AMD_RAS_DEVICE_ID {
  RAS_DXE_ID_INVALID = 0,///< / 0  = Invalid RAS DXE ID
  CPU_CORE,              ///< /  1  = CPU core
  NB_CORR_MEM,           ///< /  2  = NB correctable memory
  NB_UNCORR_MEM,         ///< /  3  = NB uncorrectable memory
  NB_ONLINE_SPARE,       ///< / 4  = NB online spare
  NB_CORR_L3_CACHE,      ///< / 5  = NB correctable L3 cache
  NB_UNCORR_L3_CACHE,    ///< / 6  = NB uncorrectable L3 cache
  NB_CORR_LINKS,         ///< / 7  = NB correctable links
  NB_UNCORR_LINKS,       ///< / 8  = NB uncorrectable links
  UNB_PCIE_AER,          ///< / 9  = UNB PCIE AER
  UNB_ONION,             ///< / 10 = UNB Onion
  UNB_PARITY,            ///< / 11 = UNB Parity
  SB_PCIE_AER,           ///< / 12 = SB PCIE AER
  SB_PARITY              ///< / 13 = SB Parity
} AMD_RAS_DEVICE_ID;

/// Error monitor type
typedef enum _ERROR_MONITOR_TYPE {
  POLLING = 0,           ///< / 0 = polling
  APIC,                  ///< / 1 = APIC
  SMI,                   ///< / 2 = SMI
  NMI,                   ///< / 3 = NMI
  SCI,                   ///< / 4 = SCI
  SYNC_FLOOD,            ///< / 5 = Sync Flood
  PCIE_LINK_DISABLE      ///< / 6 = PCIE link disabled
} ERROR_MONITOR_TYPE;

/// Common Correctable RAS Device Parameters
typedef struct _RAS_DEVICE_COMMON {
  AMD_RAS_DEVICE_ID   DeviceID;         ///< / Numeric RAS Device ID
  BOOLEAN             Enable;           ///< / Enable?
  ERROR_MONITOR_TYPE  ErrorMonitorType; ///< / Enum Monitor type, e.g APIC, SMI, NMI, Sync-Flood, etc.
  UINT32              Size;             ///< / Size of this struct
} RAS_DEVICE_COMMON;

/// Correctable Memory Config Structure
typedef struct _RAS_NB_CORR_MEM_CONFIG {
  RAS_DEVICE_COMMON   DevCommon;        ///< / RAS Device Common
  BOOLEAN             EnThreshCounter;  ///< / Enable Threshold Counter?
  BOOLEAN             SetLockedBit;     ///< / Set Lock Bit?
  UINT32              ThreshLimit;      ///< / Must not be zero or 0xFFF
  BOOLEAN             EnMemParity;      ///< / Enable Memory Parity?
  BOOLEAN             EnCorrECC;        ///< / Enable Correctable ECC?
} RAS_NB_CORR_MEM_CONFIG;

/// Uncorrectable Memory Config Structure
typedef struct _RAS_NB_UNCORR_MEM_CONFIG {
  RAS_DEVICE_COMMON   DevCommon;        ///< / RAS Device Common
  BOOLEAN             SetLockedBit;     ///< / Set Lock Bit?
  BOOLEAN             EnMemParity;      ///< / Enable Memory Parity?
  BOOLEAN             EnUnCorrECC;      ///< / Enable Uncorrectable ECC?
} RAS_NB_UNCORR_MEM_CONFIG;

/// Correctable L3 Cache Config Structure
typedef struct _RAS_NB_CORR_L3CACHE_CONFIG {
  RAS_DEVICE_COMMON   DevCommon;        ///< / RAS Device Common
  BOOLEAN             EnThreshCounter;  ///< / Enable Threshold Counter?
  BOOLEAN             SetLockedBit;     ///< / Set Lock Bit?
  UINT32              ThreshLimit;      ///< / Must not be zero or 0xFFF
} RAS_NB_CORR_L3CACHE_CONFIG;

/// Error Config Devices Stucture
typedef struct _AMD_ERROR_CONFIG_DEVICE {
  RAS_NB_CORR_MEM_CONFIG      RasMemoryConfig;  ///< / RAS NB correctable memory config
  RAS_NB_CORR_L3CACHE_CONFIG  RasL3CacheConfig; ///< / RAS NB correctable L3 Cache config
} AMD_ERROR_CONFIG_DEVICE;

/// All Error Config Structure
typedef struct _AMD_ERROR_CONFIG_ALL {
  BOOLEAN                     Enable;         ///< / Enable?
  AMD_ERROR_CONFIG_DEVICE     ErrorDeviceCfg; ///< / Error Device to be configured
} AMD_ERROR_CONFIG_ALL;

/// Error Device Location
typedef struct _AMD_ERROR_LOCATION {
  UINT32    CpuSocket;   ///< / CPU socket number
  UINT32    Node;        ///< / Node number
} AMD_ERROR_LOCATION;


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
typedef struct _AMD_RAS_DXE_INTERFACE_PROTOCOL AMD_RAS_DXE_INTERFACE_PROTOCOL;

// Protocol prototype
typedef
EFI_STATUS
(EFIAPI *AMD_GET_ERROR_CONFIG_ALL) (
  IN      AMD_RAS_DXE_INTERFACE_PROTOCOL   *This,
  IN OUT  VOID                             *ConfigBuffer,
  IN OUT  UINTN                            SourceSize
  );

// Protocol prototype
typedef
EFI_STATUS
(EFIAPI *AMD_GET_ERROR_CONFIG_DEVICE) (
  IN      AMD_RAS_DXE_INTERFACE_PROTOCOL  *This,
  IN      UINTN                           DeviceID,
  IN OUT  VOID                            *ConfigBuffer
  );

// Protocol prototype
typedef
EFI_STATUS
(EFIAPI *AMD_SET_ERROR_CONFIG_ALL) (
  IN      AMD_RAS_DXE_INTERFACE_PROTOCOL  *This,
  IN OUT  VOID                            *ConfigBuffer,
  IN      UINTN                           SourceSize
  );

// Protocol prototype
typedef
EFI_STATUS
(EFIAPI *AMD_SET_ERROR_CONFIG_DEVICE) (
  IN      AMD_RAS_DXE_INTERFACE_PROTOCOL  *This,
  IN      UINTN                           DeviceID,
  IN OUT  VOID                            *ConfigBuffer
  );

/// Protocol Structure
typedef
struct _AMD_RAS_DXE_INTERFACE_PROTOCOL {
  AMD_GET_ERROR_CONFIG_ALL      AmdGetErrorConfigAll;     ///< / Get all error config
  AMD_GET_ERROR_CONFIG_DEVICE   AmdGetErrorConfigDevice;  ///< / Get device error config
  AMD_SET_ERROR_CONFIG_ALL      AmdSetErrorConfigAll;     ///< / Set all error config
  AMD_SET_ERROR_CONFIG_DEVICE   AmdSetErrorConfigDevice;  ///< / set device error config
} AMD_RAS_DXE_INTERFACE_PROTOCOL;

#endif //_AMD_RAS_INTERFACE_PROTOCOL_H_
