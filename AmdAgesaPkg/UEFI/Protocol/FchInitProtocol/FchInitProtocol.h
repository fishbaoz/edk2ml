/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH DXE
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH DXE
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

#ifndef _FCH_INIT_PROTOCOL_H_
#define _FCH_INIT_PROTOCOL_H_

#include "FchPlatform.h"
//
// GUID definition
//
#define FCH_INIT_PROTOCOL_GUID \
        { 0xdb4a79ac, 0x5bbb, 0x4625, 0xa6, 0x9e, 0xfe, 0xbf, 0x9d, 0x6d, 0x95, 0xeb }

extern EFI_GUID gFchInitProtocolGuid;


/// FCH USB Over Current Data Block
typedef struct {
  UINT32       ChipType;                           ///< Bitfield: Applied chip types
  UINT8        Usb1OcPinMap[NUM_USB1_PORTS];       ///< USB1 over current pin mapping
  UINT8        Usb2OcPinMap[NUM_USB2_PORTS];       ///< USB2 over current pin mapping
  UINT8        Usb3OcPinMap[NUM_USB3_PORTS];       ///< USB3 over current pin mapping
  UINT8        Usb4OcPinMap[NUM_USB4_PORTS];       ///< USB4 over current pin mapping
  UINT8        Xhci0OcPinMap[NUM_XHC0_PORTS];      ///< XHCI0 over current pin mapping
  UINT8        Xhci1OcPinMap[NUM_XHC1_PORTS];      ///< XHCI1 over current pin mapping
} USB_OC;


//
// Protocol prototypes
//
typedef
VOID
(EFIAPI *FP_FCH_USB_OC) (
  USB_OC      *UsbOc
  );


/// FCH INIT Protocol
typedef struct _FCH_INIT_PROTOCOL {
  UINTN                     Revision;                 ///< Protocol Revision
  UINTN                     FchRev;                   ///< FCH Revision
  AMD_CONFIG_PARAMS         StdHeader;                ///< AMD Standard Header
  FCH_DATA_BLOCK            FchPolicy;                ///< Fch Config Data Block
  FP_FCH_USB_OC             FpUsbOverCurrent;         ///< Service: USB Over Current
} FCH_INIT_PROTOCOL;

// current PPI revision
#define FCH_INIT_REV  0x01

#endif // _FCH_INIT_PROTOCOL_H_
