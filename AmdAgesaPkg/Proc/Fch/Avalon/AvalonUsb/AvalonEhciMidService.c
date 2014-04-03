/* $NoKeywords:$ */
/**
 * @file
 *
 * Config AVALON FCH USB EHCI controller
 *
 * Init USB EHCI features.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 284224 $   @e \$Date: 2014-02-06 09:46:15 -0600 (Thu, 06 Feb 2014) $
 *
 */
/*;********************************************************************************
;
; Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
;
; AMD is granting you permission to use this software (the Materials)
; pursuant to the terms and conditions of your Software License Agreement
; with AMD.  This header does *NOT* give you permission to use the Materials
; or any rights under AMD's intellectual property.  Your use of any portion
; of these Materials shall constitute your acceptance of those terms and
; conditions.  If you do not agree to the terms and conditions of the Software
; License Agreement, please do not use any portion of these Materials.
;
; CONFIDENTIALITY:  The Materials and all other information, identified as
; confidential and provided to you by AMD shall be kept confidential in
; accordance with the terms and conditions of the Software License Agreement.
;
; LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
; PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
; WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
; MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
; OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
; IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
; (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
; INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
; GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
; RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
; THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
; EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
; THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
;
; AMD does not assume any responsibility for any errors which may appear in
; the Materials or any other related information provided to you by AMD, or
; result from use of the Materials or any related information.
;
; You agree that you will not reverse engineer or decompile the Materials.
;
; NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
; further information, software, technical information, know-how, or show-how
; available to you.  Additionally, AMD retains the right to modify the
; Materials at any time, without notice, and is not obligated to provide such
; modified Materials to you.
;
; U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
; "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
; subject to the restrictions as set forth in FAR 52.227-14 and
; DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
; Government constitutes acknowledgement of AMD's proprietary rights in them.
;
; EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
; direct product thereof will be exported directly or indirectly, into any
; country prohibited by the United States Export Administration Act and the
; regulations thereunder, without the required authorization from the U.S.
; government nor will be used for any purpose prohibited by the same.
;*********************************************************************************/
#include "FchPlatform.h"
#include "Filecode.h"
#include "AvalonFch.h"
#define FILECODE PROC_FCH_AVALON_AVALONUSB_AVALONEHCIMIDSERVICE_FILECODE
//
// Declaration of local functions
//

/**
 * FchEhciInitAfterPciInit - Config USB controller after PCI emulation
 *
 * @param[in] Value Controller PCI config address (bus# + device# + function#)
 * @param[in] FchDataPtr Fch configuration structure pointer.
 */
VOID
FchEhciInitAfterPciInit (
  IN  UINT32           Value,
  IN  FCH_DATA_BLOCK   *FchDataPtr
  )
{
  UINT8                  Index;
  UINT32                 BarAddress;
  UINT32                 Var;
  UINT8                  UsbS3WakeResumeOnlyDisable;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;
  UINT32                 PortNum;
  UINT32                 DrivingStrength;
  UINT8                  RetEfuseValue;
  UINT32                 UsbFuseCommonCalibrationValue;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  //
  //Get BAR address
  //
  ReadPci ((UINT32) Value + FCH_EHCI_REG10, AccessWidth32, &BarAddress, StdHeader);
  if ( (BarAddress != - 1) && (BarAddress != 0) ) {
    //
    //Clear Share Register Enable
    //
    RwPci ((UINT32) Value + FCH_EHCI_REG64, AccessWidth8, 0xFD, 0, StdHeader);
    //
    //Enable Memory access
    //
    RwPci ((UINT32) Value + FCH_EHCI_REG04, AccessWidth8, 0, BIT1, StdHeader);
    if (FchDataPtr->Usb.EhciSsid != NULL ) {
      RwPci ((UINT32) Value + FCH_EHCI_REG2C, AccessWidth32, 0x00, FchDataPtr->Usb.EhciSsid, StdHeader);
    }
    //
    // IN AND OUT DATA PACKET FIFO THRESHOLD
    // EHCI BAR 0xA4 //IN threshold bits[7:0]=0x40 //OUT threshold bits[23:16]=0x40
    //
    RwMem (BarAddress + FCH_EHCI_BAR_REGA4, AccessWidth32, 0xFF00FF00, 0x00400040);
    //
    // EHCI Dynamic Clock Gating Feature
    // Enable Global Clock Gating (BIT14)
    //
    RwMem (BarAddress + FCH_EHCI_BAR_REGBC, AccessWidth32, ~(UINT32) ( BIT12 + BIT14), BIT12 + BIT14);
    RwMem (BarAddress + FCH_EHCI_BAR_REGB0, AccessWidth32, ~(UINT32) BIT5, BIT5);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGF0, AccessWidth16, ~(UINT32) BIT12, BIT12);
    //
    // Enable adding extra flops to PHY rsync path
    // Step 1:
    //  EHCI_BAR 0xB4 [6] = 1
    //  EHCI_BAR 0xB4 [7] = 0
    //  EHCI_BAR 0xB4 [12] = 0 ("VLoad")
    //  All other bit field untouched
    // Step 2:
    //  EHCI_BAR 0xB4[12] = 1
    //
    // USB 2.0 Ports Driving Strength
    // Step1 is done by default
    // Step2
    for (PortNum = 0; PortNum < 4; PortNum ++) {
      if (Value == (AVALON_EHCI1_BUS_DEV_FUN << 16)) {
        DrivingStrength = (UINT32) (LocalCfgPtr->Usb.Ehci1Phy[PortNum]);
      } else if (Value == (AVALON_EHCI2_BUS_DEV_FUN << 16)) {
        DrivingStrength = (UINT32) (LocalCfgPtr->Usb.Ehci2Phy[PortNum]);
      } else if ((Value == (AVALON_EHCI3_BUS_DEV_FUN << 16)) && (PortNum < 4)) {
        DrivingStrength = (UINT32) (LocalCfgPtr->Usb.Ehci3Phy[PortNum]);
      } else {
        break;
      }
      RwMem (BarAddress + FCH_EHCI_BAR_REGB4, AccessWidth32, 0xFFFE0000, (PortNum << 13) + DrivingStrength);
      RwMem (BarAddress + FCH_EHCI_BAR_REGB4, AccessWidth32, ~(UINT32) BIT12, BIT12);
      // HSSLEW=2  >>
      DrivingStrength = 0x302;
      RwMem (BarAddress + FCH_EHCI_BAR_REGB4, AccessWidth32, 0xFFFFC000, (PortNum << 13) + BIT12 + DrivingStrength);
      Index = 0;
      do {
        ReadMem ( BarAddress + FCH_EHCI_BAR_REGB4, AccessWidth32, &Var);
        Index++;
        FchStall (10, StdHeader);
      } while (( Var & BIT17) && (Index < 10 ));
      Index = 0;
      RwMem (BarAddress + FCH_EHCI_BAR_REGB4, AccessWidth32, 0xFFFFC000, (PortNum << 13) + DrivingStrength);
      do {
        ReadMem ( BarAddress + FCH_EHCI_BAR_REGB4, AccessWidth32, &Var);
        Index++;
        FchStall (10, StdHeader);
      } while (( Var & BIT17) && (Index < 10 ));
      RwMem (BarAddress + FCH_EHCI_BAR_REGB4, AccessWidth32, 0xFFFFC000, (PortNum << 13) + BIT12 + DrivingStrength);
      // HSSLEW = 2 <<
    }
    // Step3
    RwMem (BarAddress + FCH_EHCI_BAR_REGD0, AccessWidth32, ~((UINT32) (0x0F)), (UINT32) (0x6));
    RwMem (BarAddress + FCH_EHCI_BAR_REGC4, AccessWidth32, (UINT32) (~ 0xff00ffff), 0x90001221);
    RwMem (BarAddress + FCH_EHCI_BAR_REGD4, AccessWidth32, ~((UINT32) (0xC2)), (UINT32) (0x40));
    FchStall (200, StdHeader);
    RwMem (BarAddress + FCH_EHCI_BAR_REGD4, AccessWidth32, ~((UINT32) (0x02)), (UINT32) (0x02));
    FchStall (400, StdHeader);
    RwMem (BarAddress + FCH_EHCI_BAR_REGD4, AccessWidth32, ~((UINT32) (0x02)), (UINT32) (0x0));
    RetEfuseValue = FchUsbCommonPhyCalibration ( FchDataPtr );
    if (CheckAvalonRevisionA0 (StdHeader)) {
      if ( RetEfuseValue == 0 ) {
        RwMem (BarAddress + FCH_EHCI_BAR_REGC0, AccessWidth32, (UINT32) (~ 0x00030000), BIT16);
      } else {
        UsbFuseCommonCalibrationValue = RetEfuseValue << 11;
        RwMem (BarAddress + FCH_EHCI_BAR_REGC0, AccessWidth32, (UINT32) (~ 0x0003FF00), UsbFuseCommonCalibrationValue);
      }
    } else {
      RwMem (BarAddress + FCH_EHCI_BAR_REGC0, AccessWidth32, (UINT32) (~ 0x0003FF00), 0x00020300);
    }
    //Set EHCI_pci_configx50[6]='1' to disable EHCI MSI support
    RwPci ((UINT32) Value + FCH_EHCI_REG50, AccessWidth32, ~ ((UINT32) (0x01 << 6)), (UINT32) (0x01 << 6), StdHeader);
    // EHCI Async Park Mode
    //Set EHCI_pci_configx50[11:8]=0x1
    //Set EHCI_pci_configx50[15:12]=0x1
    //Set EHCI_pci_configx50[17]=0x1
    RwPci ((UINT32) Value + FCH_EHCI_REG50, AccessWidth32, ~ ((UINT32) (0x0F << 8)), (UINT32) (0x01 << 8), StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG50, AccessWidth32, ~ ((UINT32) (0x0F << 12)), (UINT32) (0x01 << 12), StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG50, AccessWidth32, ~ ((UINT32) (0x01 << 17)), (UINT32) (0x01 << 17), StdHeader);
    // Extend InterPacket Gap
    RwPci ((UINT32) Value + FCH_EHCI_REG50, AccessWidth32, ~ ((UINT32) (0x01 << 21)), (UINT32) (0x01 << 21), StdHeader);

    // Enabling EHCI Async Stop Enhancement
    // Set EHCI_pci_configx50[29]='1' to disableEnabling EHCI Async Stop Enhancement
    //
    RwPci ((UINT32) Value + FCH_EHCI_REG50, AccessWidth32, ~ ((UINT32) (0x01 << 29)), (UINT32) (0x01 << 29), StdHeader);
    //
    // recommended setting "EHCI Advance PHY Power Savings"
    // Set EHCI_pci_configx50[31]='1'
    // Fix for EHCI controller driver  yellow sign issue under device manager
    // when used in conjunction with HSET tool driver. EHCI PCI config 0x50[20]=1
    // Disable USB data cache to resolve USB controller hang issue with Lan adaptor.
    // EHCI PCI config register 50h bit 26 to `1'.
    //
    RwPci ((UINT32) Value + FCH_EHCI_REG50 + 2, AccessWidth16, (UINT16)0xFFFF, BIT15 + BIT10, StdHeader);
    //
    // USB Delay A-Link Express L1 State
    // PING Response Fix Enable EHCI_PCI_Config  x54[1] = 1
    // Enable empty list mode. x54[3]
    // Enable "L1 Early Exit" functionality. 0x54 [6:5] = 0x3 0x54 [9:7] = 0x4
    // EHCI PING Response Fix Enable 0x54 [1] = 0x1
    // EHCI Controller Micro-Frame Counter Sync Enhancement 0x54 [4] = 0x1
    //
    RwPci ((UINT32) Value + FCH_EHCI_REG54, AccessWidth16, 0xCC04, 0x0000027b, StdHeader);
    RwAlink ((FCH_ABCFG_REG90 | (UINT32) (ABCFG << 29)), 0xFFFEFFFF, BIT16, StdHeader);
    if ( FchDataPtr->Usb.UsbMsiEnable) {
      RwPci ((UINT32) Value + FCH_EHCI_REG50, AccessWidth32, ~(UINT32) BIT6, 0x00, StdHeader);
    }
    // Long Delay on Framelist Read Causing EHCI DMA to Address 0 - Fix
    // RWPCI ((UINT32) Value + FCH_EHCI_REG54, AccWidthUint32 | S3_SAVE, ~(UINT32) BIT13, BIT13);
    // LS connection can't wake up system from S3/S4/S5 when EHCI owns the port - Fix
    RwPci ((UINT32) Value + FCH_EHCI_REG54, AccessWidth32, ~(UINT32) BIT4, BIT4, StdHeader);
    // EHCI lMU Hangs when Run/Stop is Set First and PDC is Enabled Near End uFrame 7 - Fix Enable
    RwPci ((UINT32) Value + FCH_EHCI_REG54, AccessWidth32, ~(UINT32) BIT11, BIT11, StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG54, AccessWidth16, (UINT16)0x5FFF, BIT15, StdHeader);
    // CsplitDatatglResoEnhance: Csplit_datatgl_reso_fix_enable. Read-write. Reset: 0. BIOS: 1.
    // Chicken bit for UBTS449357, the fix is to retry SSPLIT if CSPLIT detects data toggle error response.
    // 0=Disable the fix. 1=Enable the fix
    RwPci ((UINT32) Value + FCH_EHCI_REG54, AccessWidth32, ~(UINT32) BIT14, BIT14, StdHeader);

    RwPci ((UINT32) Value + FCH_EHCI_REG50 + 2, AccessWidth16, ~(UINT32) BIT3, BIT3, StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG54 + 2, AccessWidth16, (UINT16)0xFFFC, BIT0 + BIT1, StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG54 + 2, AccessWidth16, (UINT16)0xFFFB, BIT2, StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG54 + 2, AccessWidth16, (UINT16)0xFFF7, BIT3, StdHeader);
    // UBTS378549 (EHCI_REG54 BIT21) EHCI Frame list processing enhancement
    RwPci ((UINT32) Value + FCH_EHCI_REG54 + 2, AccessWidth16, ~(UINT32) BIT5, BIT5, StdHeader);
    // UBTS378553 (EHCI_REG54 BIT24) Speed field enhancement
    RwPci ((UINT32) Value + FCH_EHCI_REG54 + 2, AccessWidth16, ~(UINT32) BIT8, BIT8, StdHeader);
    // UBTS378555 (EHCI_REG54 BIT20) EHCI data cache using wrong buffer pointer on page-cross
    RwPci ((UINT32) Value + FCH_EHCI_REG54 + 2, AccessWidth16, ~(UINT32) BIT4, BIT4, StdHeader);
    // UBTS378550 (EHCI_REG54 BIT22) Unexpected OS driver TD setup causing EHCI to hang
    RwPci ((UINT32) Value + FCH_EHCI_REG54 + 2, AccessWidth16, ~(UINT32) BIT6, BIT6, StdHeader);
    // UBTS378544 (EHCI_REG54 BIT25) Reset Connect timer when disconnecting
    RwPci ((UINT32) Value + FCH_EHCI_REG54 + 2, AccessWidth16, ~(UINT32) BIT9, BIT9, StdHeader);
    // UBTS376608 (EHCI_REG54 BIT27, EHCI_REG50 BIT0) EHCI_PME should be gated by PME enable bit
    RwPci ((UINT32) Value + FCH_EHCI_REG54 + 2, AccessWidth16, ~(UINT32) BIT11, BIT11, StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG50, AccessWidth16, ~(UINT32) BIT0, BIT0, StdHeader);
    // UBTS377802 (EHCI_REG54 BIT28) EHCI need to support de-linking async active QH
    RwPci ((UINT32) Value + FCH_EHCI_REG54 + 2, AccessWidth16, ~(UINT32) BIT12, BIT12, StdHeader);
    // UBTS382275 (EHCI_REG54 BIT29) EHCI QTD with SOF
    RwPci ((UINT32) Value + FCH_EHCI_REG54 + 2, AccessWidth16, ~(UINT32) BIT13, BIT13, StdHeader);
    // (EHCI_REG54 BIT12) Enable cycle-based EHCI PIE Handshake Ready
    RwPci ((UINT32) Value + FCH_EHCI_REG54, AccessWidth16, ~(UINT32) BIT12, BIT12, StdHeader);
    // Enhance EHCI/OHCI resume/disconnect detection timer (EHCI_REG54 BIT30)
    RwPci ((UINT32) Value + FCH_EHCI_REG54 + 2, AccessWidth16, ~(UINT32) BIT14, BIT14, StdHeader);
    // Enable EHCI hold resume enhancement (EHCI_REG54 BIT31)
    RwPci ((UINT32) Value + FCH_EHCI_REG54 + 2, AccessWidth16, ~(UINT32) BIT15, BIT15, StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG74, AccessWidth8, ~(UINT32) (BIT2 + BIT5), BIT2 + BIT5, StdHeader);
    if (CheckAvalonRevisionA0 (StdHeader)) {
      RwPci ((UINT32) Value + FCH_EHCI_REG74, AccessWidth8, ~(UINT32) (BIT3), 0, StdHeader);
    } else {
      RwPci ((UINT32) Value + FCH_EHCI_REG74, AccessWidth8, ~(UINT32) (BIT3), BIT3, StdHeader);
    }
    // Set HUB_CFG_OC_DEBOUNCE to 0
    RwPci ((UINT32) Value + FCH_EHCI_REG84, AccessWidth32, ~(UINT32) (BIT24 + BIT25), 0, StdHeader);
    // ISO device CRC false error detection
    for ( PortNum = 0; PortNum < 4; PortNum++ ) {
      // 1, read current value
      RwMem (BarAddress + FCH_EHCI_BAR_REGB4, AccessWidth32, 0xFFFE0000, (UINT32) ((PortNum << 13) + BIT12 + (0x7 << 7)));
      ReadMem (BarAddress + FCH_EHCI_BAR_REGA8, AccessWidth32, &Var);
      Var = (Var >> (PortNum * 8)) & 0x000000FF;
      // 2, set BIT0
      Var &= 0xF8;
      Var |= BIT0 + BIT2;
      RwMem (BarAddress + FCH_EHCI_BAR_REGB4, AccessWidth32, 0xFFFE0000, (UINT32) ((PortNum << 13) + BIT12 + (0x7 << 7) + Var));
      RwMem (BarAddress + FCH_EHCI_BAR_REGB4, AccessWidth32, ~((UINT32) (1 << 12)), 0);
      RwMem (BarAddress + FCH_EHCI_BAR_REGB4, AccessWidth32, ~((UINT32) (1 << 12)), (UINT32) (0x1 << 12));
    }
  }

  ReadPmio (FCH_PMIOA_REGF0, AccessWidth8, &UsbS3WakeResumeOnlyDisable, StdHeader);
  if ( (UsbS3WakeResumeOnlyDisable &= BIT6) == BIT6 ) {
    RwPmio (FCH_PMIOA_REGF4, AccessWidth8, ~(UINT32) BIT2, 0, StdHeader);
  } else {
    RwPmio (FCH_PMIOA_REGF4, AccessWidth8, ~(UINT32) BIT2, BIT2, StdHeader);
  }
  RwMem (BarAddress + FCH_EHCI_BAR_REGDC, AccessWidth32, ~((UINT32) (0x0f)), (UINT32) (0x00));
}

