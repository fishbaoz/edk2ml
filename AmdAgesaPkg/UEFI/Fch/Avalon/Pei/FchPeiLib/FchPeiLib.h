/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH PEI LIBRARY
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project  FCH PEI LIBRARY
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
#ifndef _FCH_PEI_LIB_H_
#define _FCH_PEI_LIB_H_

#include "FchPlatform.h"

#define FCH_PCICFG_ADDRESS(Src32)  (                \
      (UINT64) ((UINT32) (Src32 & 0xff0000ff) +     \
               ((UINT32) (Src32 & 0xf80000) >> 3) + \
               ((UINT32) (Src32 & 0x70000) >> 8)) & 0x00000000ffffffff)

UINT8
LibFchMemRead8 (
  IN       EFI_PEI_SERVICES  **PeiServices,
  IN       UINT64            Address
  );

VOID
LibFchMemWrite8 (
  IN       EFI_PEI_SERVICES  **PeiServices,
  IN       UINT64            Address,
  IN       UINT8             *Value
  );

VOID
LibFchMemRMW8 (
  IN       EFI_PEI_SERVICES  **PeiServices,
  IN       UINT64            Address,
  IN       UINT8             AndMask,
  IN       UINT8             OrMask
  );

UINT8 LibFchPmIoRead8 (
  IN       EFI_PEI_SERVICES  **PeiServices,
  IN       UINT8             RegIndex
  );

UINT16
LibFchPmIoRead16 (
  IN       EFI_PEI_SERVICES  **PeiServices,
  IN       UINT8             RegIndex
  );

UINT32
LibFchGetAcpiMmio (
  IN       EFI_PEI_SERVICES  **PeiServices
  );

VOID
LibFchPciCfgRead (
  IN       EFI_PEI_SERVICES         **PeiServices,
  IN       PEI_PCI_CFG_PPI_WIDTH    Width,
  IN       UINT64                   Address,
  IN OUT   VOID                     *Value
  );

VOID
LibFchPciCfgWrite (
  IN       EFI_PEI_SERVICES         **PeiServices,
  IN       PEI_PCI_CFG_PPI_WIDTH    Width,
  IN       UINT64                   Address,
  IN OUT   VOID                     *Value
  );

VOID
LibFchPciCfgModify (
  IN       EFI_PEI_SERVICES         **PeiServices,
  IN       PEI_PCI_CFG_PPI_WIDTH    Width,
  IN       UINT64                   Address,
  IN       UINTN                    SetBits,
  IN       UINTN                    ClearBits
  );

VOID
LibFchPeiAuxInitialization (
  IN       EFI_PEI_SERVICES         **PeiServices
  );
#endif
