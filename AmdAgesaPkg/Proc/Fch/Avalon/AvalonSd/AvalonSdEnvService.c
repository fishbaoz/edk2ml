/* $NoKeywords:$ */
/**
 * @file
 *
 * Config AVALON SD
 *
 * Init SD Controller.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 283199 $   @e \$Date: 2014-01-21 15:27:13 -0600 (Tue, 21 Jan 2014) $
 *
 */
/*
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
****************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE PROC_FCH_AVALON_AVALONSD_AVALONSDENVSERVICE_FILECODE
/**
 * FchInitEnvSdProgram - Config SD controller before PCI
 * emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvSdProgram (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //
  // SD Configuration
  //
  if ( (LocalCfgPtr->Sd.SdConfig != SdDisable) && (LocalCfgPtr->Sd.SdConfig != SdDump)) {
    ACPIMMIO32(FCH_MISCx68_MEMORY_POWER_SAVING_CONTROL) &= ~ BIT18; //SdBypassMemDsd

    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD3, AccessWidth8, 0xBF, 0x40);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGE8, AccessWidth8, 0xFE, BIT0);

    if (LocalCfgPtr->Sd.SdSsid != NULL ) {
      RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REG2C, AccessWidth32, 0, LocalCfgPtr->Sd.SdSsid, StdHeader);
    }

//Auto
//Bu0/De14/F7 0xA4 [31:0] = 21DE_32B2h
//Bu0/De14/F7 0xA8 [31:0] = 0000_2570h
//Bu0/De14/F7 0xB0 [31:0] = 0118_0C1Dh
//Bu0/De14/F7 0xD0 [31:0] = 0008_17CFh

//SD2.0
//Bu0/De14/F7 0xA4 [31:0] = 21FE_32B2h
//Bu0/De14/F7 0xA8 [31:0] = 0000_2570h
//Bu0/De14/F7 0xB0 [31:0] = 0118_0C1Dh
//Bu0/De14/F7 0xD0 [31:0] = 0008_17CFh

//SD3.0
//Bu0/De14/F7 0xA4 [31:0] = 21FE_C8B2h
//Bu0/De14/F7 0xA8 [31:0] = 0000_2573h
//Bu0/De14/F7 0xB0 [31:0] = 0218_0C1Dh
//Bu0/De14/F7 0xD0 [31:0] = 0008_17CFh

//SD 3.0
//A. Program D14F#SD_FUN#xA4=21FE_C8B2h. (UBTS470952)
//B. Program D14F#SD_FUN#xA8=0000_2573h. (UBTS470952)
//C. Program D14F#SD_FUN#xB0=0218_0C19h.
//D. Program D14F#SD_FUN#xD0=0000_078Bh.
//2.20.5.2 SD 2.0 Mode
//The following programming sequence sets up SD controller to operate in SD 2.0 mode.
//A. Program D14F#SD_FUN#xA4=21FE_3FB2h.
//B. Program D14F#SD_FUN#xA8=0000_0570h. (UBTS467336, UBTS470952)
//C. Program D14F#SD_FUN#xB0=0118_0C19h.
//D. Program D14F#SD_FUN#xD0=0000_078Bh.

    RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4, AccessWidth32, 0x3FFFFFFF, ((UINT32) (LocalCfgPtr->Sd.SdSlotType) << 30), StdHeader);

    if ( LocalCfgPtr->Sd.SdHostControllerVersion == 1) {
      // SD 2.0
      //RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4, AccessWidth32, 0xC0000000, 0x21FE32B2, StdHeader);
      //RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA8, AccessWidth32, 0, 0x00002570, StdHeader);
      //RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGB0, AccessWidth32, 0, 0x01180C1D, StdHeader);
      //RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGD0, AccessWidth32, 0, 0x000817CF, StdHeader);
      RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4, AccessWidth32, 0xC0000000, 0x21FE3FB2, StdHeader);
      RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA8, AccessWidth32, 0, 0x00000570, StdHeader);
      RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGB0, AccessWidth32, 0, 0x01180C19, StdHeader);
      RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGD0, AccessWidth32, 0, 0x0000078B, StdHeader);
    } else {
      if ( LocalCfgPtr->Sd.SdHostControllerVersion == 2) {
        // SD 3.0
        //RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4, AccessWidth32, 0xC0000000, 0x21FEC8B2, StdHeader);
        //RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA8, AccessWidth32, 0, 0x00002573, StdHeader);
        //RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGB0, AccessWidth32, 0, 0x02180C1D, StdHeader);
        //RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGD0, AccessWidth32, 0, 0x000817CF, StdHeader);
        RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4, AccessWidth32, 0xC0000000, 0x21FEC8B2, StdHeader);
        RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA8, AccessWidth32, 0, 0x00002573, StdHeader);
        RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGB0, AccessWidth32, 0, 0x02180C19, StdHeader);
        RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGD0, AccessWidth32, 0, 0x0000078B, StdHeader);
      } else {
        if ( LocalCfgPtr->Sd.SdHostControllerVersion == 15 ) {
          // SD AUTO 2.0 + Low speed
          RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4, AccessWidth32, 0xC0000000, 0x21DE32B2, StdHeader);
          RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA8, AccessWidth32, 0, 0x00002570, StdHeader);
          RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGB0, AccessWidth32, 0, 0x01180C1D, StdHeader);
          RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGD0, AccessWidth32, 0, 0x000817CF, StdHeader);
        }
      }
    }

  } else {
    if (LocalCfgPtr->Sd.SdConfig == SdDisable) {
      RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD3, AccessWidth8, 0xBF, 0x00);
      RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGE8, AccessWidth8, 0xFE, 0x00);
    }
  }
}
