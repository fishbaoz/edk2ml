/* $NoKeywords:$ */
/**
 * @file
 *
 * mfecc.c
 *
 * Feature ECC initialization functions
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Feat/ECC)
 * @e \$Revision: 281178 $ @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 **/
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
  * ***************************************************************************
  *
 */

#include "AGESA.h"
#include "amdlib.h"
#include "Ids.h"
#include "mm.h"
#include "mn.h"
#include "mfecc.h"
#include "Filecode.h"
#include "mfmemclr.h"
#include "GeneralServices.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_MEM_FEAT_ECC_MFECC_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

VOID
STATIC
InitECCOverriedeStruct (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   ECC_OVERRIDE_STRUCT *pecc_override_struct
  );

BOOLEAN
MemFCheckECC (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

/*
 *-----------------------------------------------------------------------------
 *                                EXPORTED FUNCTIONS
 *
 *-----------------------------------------------------------------------------
 */

extern BUILD_OPT_CFG UserOptions;

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function checks to see if ECC can be enabled on all nodes
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE -  This feature is enabled.
 *     @return          FALSE - This feature is not enabled.
 */

BOOLEAN
MemFCheckECC (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  DIE_STRUCT *MCTPtr;
  MEM_SHARED_DATA *SharedPtr;
  BOOLEAN ErrorRecovery;
  UINT32   EccEnable;

  ASSERT (NBPtr != NULL);

  MCTPtr = NBPtr->MCTPtr;
  SharedPtr = NBPtr->SharedPtr;

  ErrorRecovery = TRUE;
  IDS_OPTION_HOOK (IDS_MEM_ERROR_RECOVERY, &ErrorRecovery, &NBPtr->MemPtr->StdHeader);

  //
  // Check if respective silicon supports Ecc by Querying EccDis.
  //
  EccEnable = NBPtr->GetBitField (NBPtr, BFEccDis);
  IDS_HDT_CONSOLE (MEM_FLOW, "EccDis %d\n", EccEnable);

  if (MCTPtr->NodeMemSize != 0) {
    if (SharedPtr->AllECC && MCTPtr->Status[SbEccDimms] && (ErrorRecovery || (MCTPtr->ErrCode < AGESA_ERROR)) && EccEnable) {
      // Clear all MCA reports before using scrubber
      // to initialize ECC check bits
      //
      NBPtr->McaNbCtlReg = NBPtr->GetBitField (NBPtr, BFMcaNbCtlReg);
      NBPtr->SetBitField (NBPtr, BFMcaNbCtlReg, 0);
      NBPtr->SetBitField (NBPtr, BFSyncOnUcEccEn, 0);
      // In unganged mode, set DctDctIntlv
      if (!NBPtr->Ganged) {
        NBPtr->SetBitField (NBPtr, BFDctDatIntLv, 1);
      }
      //
      // Set Ecc Symbol Size
      //
      NBPtr->SetEccSymbolSize (NBPtr);
      // If ECC can be enabled on this node,
      // set the master ECCen bit (according to setup)
      //
      NBPtr->SetBitField (NBPtr, BFDramEccEn, 1);
      // Do mem clear on current node
      MemFMctMemClr_Init (NBPtr);
      return TRUE;
    } else {
      if (SharedPtr->AllECC) {
        SharedPtr->AllECC = FALSE;
      }
      // ECC requested but cannot be enabled
      MCTPtr->Status[SbEccDimms] = FALSE;
      MCTPtr->ErrStatus[EsbDramECCDis] = TRUE;
      PutEventLog (AGESA_WARNING, MEM_WARNING_ECC_DIS, NBPtr->Node, 0, 0, 0, &NBPtr->MemPtr->StdHeader);
      SetMemError (AGESA_WARNING, MCTPtr);
    }
  }
  return FALSE;
}

  /* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function initializes the ECC on all nodes
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE -  This feature is enabled.
 *     @return          FALSE - This feature is not enabled.
 */

BOOLEAN
MemFInitECC (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Node;
  UINT32 ScrubAddrRJ16;
  DIE_STRUCT *MCTPtr;
  MEM_SHARED_DATA *SharedPtr;
  ECC_OVERRIDE_STRUCT  ecc_override_struct;
  BOOLEAN Flag;

  InitECCOverriedeStruct (NBPtr, &ecc_override_struct);
  IDS_OPTION_HOOK (IDS_ECC, &ecc_override_struct, &(NBPtr->MemPtr->StdHeader));

  ASSERT (NBPtr != NULL);

  MCTPtr = NBPtr->MCTPtr;
  Node = MCTPtr->NodeId;
  SharedPtr = NBPtr->SharedPtr;
  Flag = TRUE;

  NBPtr->FamilySpecificHook[ScrubberErratum] (NBPtr, (VOID *) &Flag);

  if ((MCTPtr->Status[SbEccDimms]) && (SharedPtr->AllECC)) {
    // Check if the input dram scrub rate is supported or not
    ASSERT (ecc_override_struct.CfgScrubDramRate <= 0x16);
    if (ecc_override_struct.CfgScrubDramRate != 0) {
    // Program scrub address,
    // let the scrub Addr be the Base of this Node
    // Only enable Dram scrubber when there is memory on current node
    //
      NBPtr->SetBitField (NBPtr, BFScrubReDirEn, 0);
      ScrubAddrRJ16 = (NBPtr->GetBitField (NBPtr, BFDramBaseReg0 + Node) & 0xFFFF0000) >> 8;
      ScrubAddrRJ16 |= NBPtr->GetBitField (NBPtr, BFDramBaseHiReg0 + Node) << 24;
      NBPtr->SetBitField (NBPtr, BFScrubAddrLoReg, ScrubAddrRJ16 << 16);
      NBPtr->SetBitField (NBPtr, BFScrubAddrHiReg, ScrubAddrRJ16 >> 16);
      NBPtr->SetBitField (NBPtr, BFDramScrub, ecc_override_struct.CfgScrubDramRate);
      NBPtr->IsSupported[ScrubberEn] = TRUE;
    }
  }
  // Scrub CTL for Dcache, L2, L3
  // Check if the input L2 scrub rate is supported or not
  ASSERT (ecc_override_struct.CfgScrubL2Rate <= 0x16);
  NBPtr->SetBitField (NBPtr, BFL2Scrub, ecc_override_struct.CfgScrubL2Rate);
  // Check if the input Dcache scrub rate is supported or not
  ASSERT (ecc_override_struct.CfgScrubDcRate <= 0x16);
  NBPtr->SetBitField (NBPtr, BFDcacheScrub, ecc_override_struct.CfgScrubDcRate);
  // Do not enable L3 Scrub if F3xE8[L3Capable] is 0 or F3x188[DisableL3] is 1
  if ((NBPtr->GetBitField (NBPtr, BFL3Capable) == 1) && (NBPtr->GetBitField (NBPtr, BFDisableL3) == 0)) {
    // Check if input L3 scrub rate is supported or not
    ASSERT (ecc_override_struct.CfgScrubL3Rate <= 0x16);
    NBPtr->SetBitField (NBPtr, BFL3Scrub, ecc_override_struct.CfgScrubL3Rate);
  }

  // Check if Dcache scrubber or L2 scrubber is enabled
  if ((ecc_override_struct.CfgScrubL2Rate != 0) || (ecc_override_struct.CfgScrubDcRate!= 0)) {
    // If ClkDivisor is deeper than divide-by-16
    if (NBPtr->GetBitField (NBPtr, BFC1ClkDivisor) > 4) {
      // Set it to divide-by-16
      NBPtr->SetBitField (NBPtr, BFC1ClkDivisor, 4);
    }
  }

  NBPtr->SetBitField (NBPtr, BFScrubReDirEn, ecc_override_struct.CfgEccRedirection);
  NBPtr->SetBitField (NBPtr, BFSyncOnUcEccEn, ecc_override_struct.CfgEccSyncFlood);
  // Restore MCA reports after scrubber is done
  // with initializing ECC check bits
  NBPtr->SetBitField (NBPtr, BFMcaNbCtlReg, NBPtr->McaNbCtlReg);

  Flag = FALSE;
  NBPtr->FamilySpecificHook[ScrubberErratum] (NBPtr, (VOID *) &Flag);

  return TRUE;
}

VOID
STATIC
InitECCOverriedeStruct (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   ECC_OVERRIDE_STRUCT *pecc_override_struct
  )
{
  //
  // If (D18F3x44[DramEccEn]==1) THEN 1 ELSE 0 ENDIF
  //
  if (NBPtr->GetBitField (NBPtr, BFDramEccEn) == 1) {
    pecc_override_struct->CfgEccRedirection = 1;
  } else {
    pecc_override_struct->CfgEccRedirection = 0;
  }

  pecc_override_struct->CfgEccSyncFlood = UserOptions.CfgEccSyncFlood;
  pecc_override_struct->CfgScrubDcRate = UserOptions.CfgScrubDcRate;

  if (UserOptions.CfgScrubDramRate != 0xFF) {
    pecc_override_struct->CfgScrubDramRate = UserOptions.CfgScrubDramRate;
  } else {
    if (NBPtr->MCTPtr->NodeMemSize <= 0x4000) {
      pecc_override_struct->CfgScrubDramRate = 0x12; // 1 ~ 1 GB
    } else if (NBPtr->MCTPtr->NodeMemSize <= 0x8000) {
      pecc_override_struct->CfgScrubDramRate = 0x11; // 1 GB + 1 ~ 2 GB
    } else if (NBPtr->MCTPtr->NodeMemSize <= 0x10000) {
      pecc_override_struct->CfgScrubDramRate = 0x10; // 2 GB + 1 ~ 4 GB
    } else if (NBPtr->MCTPtr->NodeMemSize <= 0x20000) {
      pecc_override_struct->CfgScrubDramRate = 0x0F; // 4 GB + 1 ~ 8 GB
    } else if (NBPtr->MCTPtr->NodeMemSize <= 0x40000) {
      pecc_override_struct->CfgScrubDramRate = 0x0E; // 8 GB + 1 ~ 16 GB
    } else {
      pecc_override_struct->CfgScrubDramRate = 0x0D; //16 GB + 1 above
    }
  }

  pecc_override_struct->CfgScrubL2Rate = UserOptions.CfgScrubL2Rate;
  pecc_override_struct->CfgScrubL3Rate = UserOptions.CfgScrubL3Rate;
}
