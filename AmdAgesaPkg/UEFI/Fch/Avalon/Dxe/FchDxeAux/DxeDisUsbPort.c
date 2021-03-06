/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH DXE AUX Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH Disable USB Ports Service
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
#include "Tiano.h"

#include "FchDxeAux.h"
#include "FchDxeLib.h"
#include "Filecode.h"
#define FILECODE UEFI_FCH_AVALON_DXE_FCHDXEAUX_DXEDISUSBPORT_FILECODE

VOID
FchUsbSetPortDisable (
  IN       UINT16              PciDevice,
  IN       UINT8               UsbPortBitMap
  )
{
  UINT8               ErData;
  UINT32              PciAddress;

  PciAddress = (UINT32) (PciDevice << 16) + FCH_OHCI_REG51;
  LibFchPciRead (EfiPciWidthUint8, PciAddress, &ErData);
  ErData = ((ErData & 0xBF) | 0x20);
  LibFchPciWrite (EfiPciWidthUint8, PciAddress, &ErData);

  PciAddress = (UINT32) (PciDevice << 16) + FCH_OHCI_REG42;
  LibFchPciRead (EfiPciWidthUint8, PciAddress, &ErData);
  ErData = ((ErData & 0xE0) | UsbPortBitMap);
  LibFchPciWrite (EfiPciWidthUint8, PciAddress, &ErData);

  PciAddress = (UINT32) (PciDevice << 16) + FCH_OHCI_REG51;
  LibFchPciRead (EfiPciWidthUint8, PciAddress, &ErData);
  ErData &= 0xBF;
  LibFchPciWrite (EfiPciWidthUint8, PciAddress, &ErData);
}

VOID
FchXhciSetPortDisable (
  IN       UINT16             PciDevice,
  IN       UINT32             XhciPortBitMap
  )
{
  UINT32              dInderData;
  UINT32              PciAddress;

  dInderData = FCH_OHCI_REG0C;
  PciAddress = (UINT32) (PciDevice << 16) + FCH_XHCI_REG48;
  LibFchPciWrite (EfiPciWidthUint32, PciAddress, &dInderData);
  dInderData = 0x03;
  PciAddress = (UINT32) (PciDevice << 16) + FCH_XHCI_REG4C;
  LibFchPciWrite (EfiPciWidthUint32, PciAddress, &dInderData);
  dInderData = FCH_OHCI_REG08;
  PciAddress = (UINT32) (PciDevice << 16) + FCH_XHCI_REG48;
  LibFchPciWrite (EfiPciWidthUint32, PciAddress, &dInderData);
  dInderData = XhciPortBitMap;
  PciAddress = (UINT32) (PciDevice << 16) + FCH_XHCI_REG4C;
  LibFchPciWrite (EfiPciWidthUint32, PciAddress, &dInderData);
  dInderData = FCH_OHCI_REG0C;
  PciAddress = (UINT32) (PciDevice << 16) + FCH_XHCI_REG48;
  LibFchPciWrite (EfiPciWidthUint32, PciAddress, &dInderData);
  dInderData = 0x00;
  PciAddress = (UINT32) (PciDevice << 16) + FCH_XHCI_REG4C;
  LibFchPciWrite (EfiPciWidthUint32, PciAddress, &dInderData);
}

/**
 * FchDxeDisUsbPort - Disable USB Ports Service
 *
 * @param[in]  UsbBitMapPr           Usb each controler's port bitmap pointer
 *
 */
VOID
FchDxeDisUsbPort (
  IN       USB_PORT_BITMAP      *UsbBitMapPr
  )
{
  UINT32                      PciAddress;
  UINT32                      dDword;
  UINT32                      dXhciData;
  UINT8                       UsbPortBitMap;
  UINT16                      PciDev;

  if (UsbBitMapPr->Usb1PortBM != 0 ) {
    PciDev = USB1_OHCI_BUS_DEV_FUN;
    UsbPortBitMap = UsbBitMapPr->Usb1PortBM;
    FchUsbSetPortDisable ( PciDev, UsbPortBitMap);
  }
  if (UsbBitMapPr->Usb2PortBM != 0 ) {
    PciDev = USB2_OHCI_BUS_DEV_FUN;
    UsbPortBitMap = UsbBitMapPr->Usb2PortBM;
    FchUsbSetPortDisable ( PciDev, UsbPortBitMap);
  }
  if (UsbBitMapPr->Usb4PortBM != 0 ) {
    PciDev = USB4_OHCI_BUS_DEV_FUN;
    UsbPortBitMap = UsbBitMapPr->Usb4PortBM;
    FchUsbSetPortDisable ( PciDev, UsbPortBitMap);
  }
  if (UsbBitMapPr->Usb3PortBM != 0 ) {
    PciAddress = (USB_XHCI_BUS_DEV_FUN << 16);
    LibFchPciRead (EfiPciWidthUint32, PciAddress, &dDword);
    if ( dDword == (AVALON_USB_XHCI_DID << 16 | FCH_USB_XHCI_VID) ) {
      PciDev = USB_XHCI_BUS_DEV_FUN;
      dXhciData = (UINT32) UsbBitMapPr->Usb3PortBM;
      dXhciData &= 0x03;
      FchXhciSetPortDisable ( PciDev, dXhciData);
      PciDev = USB_XHCI1_BUS_DEV_FUN;
      dXhciData = (UINT32) UsbBitMapPr->Usb3PortBM;
      dXhciData = (dXhciData & 0x0C) >> 2;
      FchXhciSetPortDisable ( PciDev, dXhciData);
    } else {
      PciDev = USB3_OHCI_BUS_DEV_FUN;
      UsbPortBitMap = UsbBitMapPr->Usb3PortBM;
      FchUsbSetPortDisable ( PciDev, UsbPortBitMap);
    }
  }
}


