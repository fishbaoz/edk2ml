/* $NoKeywords:$ */
/**
 * @file
 *
 * Config Fch Imc controller
 *
 * Init Imc Controller features.
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
#include "Filecode.h"
#define FILECODE PROC_FCH_AVALON_AVALONIMC_AVALONIMCSERVICE_FILECODE

//
// Declaration of local functions
//


/**
 * SoftwareToggleImcStrapping - Software Toggle IMC Firmware Strapping.
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
SoftwareToggleImcStrapping (
  IN  VOID     *FchDataPtr
  )
{
  UINT8    ValueByte;
  UINT8    PortStatusByte;
  UINT32   AbValue;
  UINT32   ABStrapOverrideReg;
  AMD_CONFIG_PARAMS     *StdHeader;

  StdHeader = ((FCH_DATA_BLOCK *) FchDataPtr)->StdHeader;
  GetChipSysMode (&PortStatusByte, StdHeader);
  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGC8 + 3, AccessWidth8, 0x7F, BIT7, StdHeader);
  ReadPmio (FCH_PMIOA_REGBF, AccessWidth8, &ValueByte, StdHeader);

  ReadMem ((ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG80), AccessWidth32, &AbValue);
  ABStrapOverrideReg = AbValue;

  ABStrapOverrideReg |= BIT31;                             /// Overwrite enable

  if ((PortStatusByte & ChipSysEcEnable) == 0) {
    ABStrapOverrideReg |= BIT2;                            /// bit2- EcEnableStrap
  } else {
    ABStrapOverrideReg &= ~BIT2;                           /// bit2=0 EcEnableStrap
  }

  WriteMem ((ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG84), AccessWidth32, &ABStrapOverrideReg);
  ValueByte |= (BIT6 + BIT7);                    /// PwrGoodOut =1, PwrGoodEnB=1
  WritePmio (FCH_PMIOA_REGBF, AccessWidth8, &ValueByte, StdHeader);

  ReadPmio (FCH_PMIOA_REGC8, AccessWidth8, &ValueByte, StdHeader);
  ValueByte |= BIT4;        // Set UseAcpiStraps to 1, this bit is to be reset in early BIOS
  WritePmio (FCH_PMIOA_REGC8, AccessWidth8, &ValueByte, StdHeader);

  ValueByte = 06;
  LibAmdIoWrite (AccessWidth8, 0xcf9, &ValueByte, StdHeader);
  FchStall (0xffffffff, StdHeader);
}


