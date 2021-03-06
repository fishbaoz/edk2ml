/* $NoKeywords:$ */
/**
 * @file
 *
 * Config Fch HwAcpi controller
 *
 * Init Spread Spectrum features.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
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
#include "amdlib.h"
#include "cpuServices.h"
#include "Filecode.h"
#define FILECODE PROC_FCH_AVALON_AVALONHWACPI_AVALONSSSERVICE_FILECODE

/**
 * FchInitResetAcpiMmioTable - Fch ACPI MMIO initial
 * during the power on stage.
 *
 *
 *
 *
 */
ACPI_REG_WRITE FchInitResetAcpiMmioTable[] =
{
  {00, 00, 0xB0, 0xAC},                                         /// Signature
  //
  // Supported Yuba RPR / KR BKDG spread-spectrum setting.
  //
  {MISC_BASE >> 8,  FCH_MISC_REG40 + 1, 0x0F, 0x40},
  //
  // USB 3.0 Reference Clock MISC_REG 0x40 [4] = 0 Enable spread-spectrum reference clock.
  //
  {MISC_BASE >> 8,  FCH_MISC_REG40, 0xEF, 0x00},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG5D, 0xFC, BIT1},
  {PMIO_BASE >> 8,  FCH_PMIOA_REGD2, 0xCF, 0x00},
  {SMBUS_BASE >> 8, FCH_SMBUS_REG12, 0x00, BIT0},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG44 + 3, 0x67, 0},            /// Stop Boot timer
  {PMIO_BASE >> 8,  FCH_PMIOA_REG00, 0xFF, BIT7},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG00, 0xFF, 0xE7},
  {PMIO_BASE >> 8,  FCH_PMIOA_REGEC, 0xFB, BIT2},      /// For BTS remove setting LPCA20EN = 1 to fix 0x004E hang at restart from power off
  {PMIO_BASE >> 8,  FCH_PMIOA_REG08, 0xFE, BIT2 + BIT4},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG08 + 1, 0xFF, BIT0},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG54, 0x00, BIT4 + BIT6 + BIT7},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG74, 0xF6, BIT0 + BIT3},
  {PMIO_BASE >> 8,  FCH_PMIOA_REGF0, 0xFB, 0x00},

  {PMIO_BASE >> 8, FCH_PMIOA_REGC4, 0xEE, 0x04},                 /// Release NB_PCIE_RST
  {PMIO_BASE >> 8, FCH_PMIOA_REGC0 + 2, 0xBF, 0x40},
  {PMIO_BASE >> 8, FCH_PMIOA_REGBE, 0xDF, BIT5},
  {PMIO_BASE >> 8, FCH_PMIOA_REGC4, 0xFB, 0},
  //
  // Enabling ClkRun Function
  //
  {PMIO_BASE >> 8,  FCH_PMIOA_REGBB, 0xFF, BIT2},
  {PMIO_BASE >> 8,  FCH_PMIOA_REGD0, 0xFB, 0},
  {PMIO_BASE >> 8,  FCH_PMIOA_REGD7, 0xFD, 0},                   // PMIO 0xD4 BIT25 clear 0
  {MISC_BASE >> 8,  FCH_MISC_REG6C + 2, 0x7F, BIT7},             // MISC 0x6C BIT23
  {MISC_BASE >> 8,  FCH_MISC_REG6C + 3, 0xF7, BIT3},             // MISC 0x6C BIT27
  //
  // CG PLL CMOS Clock Driver Setting for power saving
  //
  {MISC_BASE >> 8, FCH_MISC_REG1C + 0x03, 0xDF, 0x00},
  {MISC_BASE >> 8, FCH_MISC_REG1C + 0x02, 0x9F, 0x60},
  {MISC_BASE >> 8, FCH_MISC_REG1C + 0x03, 0xFA, 0x05},
  {0xFF, 0xFF, 0xFF, 0xFF},
};

/**
 * ProgramFchHwAcpiResetP  - Config SpreadSpectrum before PCI
 * emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramFchHwAcpiResetP (
  IN VOID  *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;
  AMD_CONFIG_PARAMS         *StdHeader;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  StdHeader = &((AMD_RESET_PARAMS *)FchDataPtr)->StdHeader;

  //
  // Clear UseAcpiStraps, PMIO_C8[4]
  //
  RwPmio (FCH_PMIOA_REGC8, AccessWidth8, 0xEF, 0x0, StdHeader);
  RwPmio (FCH_PMIOA_REGD3, AccessWidth8, ~(UINT32) BIT4, 0, StdHeader);
  RwPmio (FCH_PMIOA_REGD3, AccessWidth8, ~(UINT32) BIT4, BIT4, StdHeader);
  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGC8 + 3, AccessWidth8, 0x7F, BIT7, StdHeader);
}


