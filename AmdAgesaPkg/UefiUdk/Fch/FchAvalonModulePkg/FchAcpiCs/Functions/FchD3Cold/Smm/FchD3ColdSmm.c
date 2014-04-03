/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH DXE Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH DXE Driver
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
#include "FchD3ColdSmm.h"

FCH_INIT_PROTOCOL                   gFchInitInSmm;
/*********************************************************************************
 * Name: FchD3ColdSmmInit
 *
 * Description
 *   FchD3ColdSmm Entry point
 *
 * Input
 *   ImageHandle : EFI Image Handle for the DXE driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchD3ColdSmmInit (
  IN    EFI_HANDLE                  ImageHandle,
  IN    EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS                        Status;
  FCH_INIT_PROTOCOL                 *pAmdFchInitProtocol;
  PSP_RESUME_SERVICE_PROTOCOL       *pAmdPspResumeServiceProtocol;
  EFI_HANDLE                        DispatchHandle;

  Status = gSmst->SmmLocateProtocol (
                    &gFchInitProtocolGuid,
                    NULL,
                    &pAmdFchInitProtocol
                    );

  if (!EFI_ERROR (Status)) {
    CopyMem (
      &gFchInitInSmm,
      pAmdFchInitProtocol,
      sizeof (FCH_INIT_PROTOCOL)
      );
  } else {
    return EFI_ABORTED;
  }

  Status = gSmst->SmmLocateProtocol (
                    &gPspResumeServiceProtocolGuid,
                    NULL,
                    &pAmdPspResumeServiceProtocol
                    );

  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = pAmdPspResumeServiceProtocol->Register (
                                           pAmdPspResumeServiceProtocol,
                                           &FchCsResumeCallBack,
                                           &gFchInitInSmm,
                                           0xFF,
                                           &DispatchHandle
                                           );
  return Status;
}

EFI_STATUS
EFIAPI
FchCsResumeCallBack (
  IN  RESUME_TYPE   ResumeType,
  IN  VOID          *Context
   )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) Context;
  StdHeader = LocalCfgPtr->StdHeader;
  if (ResumeType == ResumeFromConnectedStandby) {

    RwAlink (FCH_ABCFG_REG80 | (UINT32) (ABCFG << 29), ~(UINT32) BIT0, BIT0, StdHeader);
    RwAlink (FCH_ABCFG_REG80 | (UINT32) (ABCFG << 29), ~(UINT32) (0x3 << 17), (UINT32) (0x3 << 17), StdHeader);
    RwAlink (FCH_ABCFG_REG94 | (UINT32) (ABCFG << 29), 0xFFE00000, BIT20 + 0x00FEE, StdHeader);
    RwAlink (FCH_ABCFG_REG10054 | (UINT32) (ABCFG << 29), 0xFFFFF000, 0x7FF, StdHeader);
    RwAlink (FCH_ABCFG_REG90 | (UINT32) (ABCFG << 29), ~(UINT32) BIT21, BIT21, StdHeader);
    RwAlink (FCH_ABCFG_REG10090 | (UINT32) (ABCFG << 29), ~(UINT32) BIT16, BIT16, StdHeader);
    RwAlink (FCH_ABCFG_REG58 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT29 + BIT31), (UINT32) (BIT29 + BIT31), StdHeader);
    RwAlink (FCH_ABCFG_REG10056 | (UINT32) (ABCFG << 29), 0xFFFFC00, 0x310, StdHeader);
    RwAlink (FCH_ABCFG_REG54 | (UINT32) (ABCFG << 29), 0xFE00FFEA, 0x01100014, StdHeader);
//Add SATA PHY STAGGERING SEQUENCE to fix SSD disk detection issue
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40), AccessWidth32, (UINT32) (~ (0x1 << 16)), (UINT32) (0x1 << 16), StdHeader);
    FchStall (1, StdHeader);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40), AccessWidth32, (UINT32) (~ (0x1 << 16)), (UINT32) (0x0 << 16), StdHeader);

    return EFI_SUCCESS;
  } else {
    return EFI_UNSUPPORTED;
  }
}

