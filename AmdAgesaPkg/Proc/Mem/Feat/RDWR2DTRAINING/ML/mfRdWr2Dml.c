/* $NoKeywords:$ */
/**
 * @file
 *
 * mfRdWr2Dml.c
 *
 * ML - Specific funtion for 2D Read and write training feature
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Feat/RdWr2DTraining/Ml
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
/*
 *----------------------------------------------------------------------------
 *                                MODULES USED
 *
 *----------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "amdlib.h"
#include "AdvancedApi.h"
#include "GeneralServices.h"
#include "Ids.h"
#include "heapManager.h"
#include "mm.h"
#include "mn.h"
#include "mu.h"
#include "mt.h"
#include "mport.h"
#include "merrhdl.h"
#include "OptionMemory.h"
#include "mfRdWr2DTraining.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_MEM_FEAT_RDWR2DTRAINING_ML_MFRDWR2DML_FILECODE
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

BOOLEAN
STATIC
MemFRdWr2DProgramVrefML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *VrefPtr
  );

BOOLEAN
STATIC
MemFRdWr2DScaleVrefML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Vref
  );

BOOLEAN
STATIC
MemFRdWr2DProgramIntExtVrefSelectML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
STATIC
MemFRdWr2DProgramDataPatternML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID* PatternIndexPtr
  );

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function initializes the 2D Read/Write Training Feature Hooks for ML
 *
 *     @param[in,out] *NBPtr - Pointer to the MEM_NB_BLOCK
 *
 *     @return         BOOLEAN
 *                             TRUE - Function was implemented
 *
 */

BOOLEAN
MemFRdWr2DTrainingInitML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  ASSERT (NBPtr != NULL);
  NBPtr->FamilySpecificHook[RdWr2DTraining] = MemFAmdRdWr2DTraining;
  NBPtr->FamilySpecificHook[CheckRdWr2DTrainingPerConfig] = MemFCheckRdWr2DTrainingPerConfig;
  NBPtr->FamilySpecificHook[RdWr2DSelectIntExtVref] = MemFRdWr2DProgramIntExtVrefSelectML;
  NBPtr->FamilySpecificHook[RdWr2DProgramVref] = MemFRdWr2DProgramVrefML;
  NBPtr->FamilySpecificHook[RdWr2DScaleVref] = MemFRdWr2DScaleVrefML;
  NBPtr->FamilySpecificHook[RdWr2DProgramDelays] = MemFRdWr2DProgramDelays;
  NBPtr->FamilySpecificHook[RdWr2DDataCollection] = MemFRdWr2DEyeRimSearch;
  NBPtr->FamilySpecificHook[RdWr2DInitVictim] = MemFRdWr2DInitVictim;
  NBPtr->FamilySpecificHook[RdWr2DInitVictimChipSel] = MemFRdWr2DInitVictimChipSel;
  NBPtr->FamilySpecificHook[RdWr2DStartVictim] = MemFRdWr2DStartVictim;
  NBPtr->FamilySpecificHook[RdWr2DFinalizeVictim] = MemFRdWr2DFinalizeVictim;
  NBPtr->FamilySpecificHook[RdWr2DCompareInPhase] = MemFRdWr2DCompareInPhase;
  NBPtr->FamilySpecificHook[RdWr2DCompare180Phase] = MemFRdWr2DCompare180Phase;
  NBPtr->FamilySpecificHook[RdWr2DProgramDataPattern] = MemFRdWr2DProgramDataPatternML;
  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs Vref for 2D Read/Write Training
 *
 *     @param[in,out] *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]     *VrefPtr - Pointer to Vref value
 *
 *     @return         BOOLEAN
 *                               TRUE - Success
 *                               FAIL (External Callout only)
 *
 */
BOOLEAN
STATIC
MemFRdWr2DProgramVrefML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *VrefPtr
  )
{
  AGESA_STATUS Status;
  MEM_DATA_STRUCT *MemPtr;
  ID_INFO CallOutIdInfo;
  VOLTAGE_ADJUST Va;
  UINT8 Vref;

  ASSERT (NBPtr != NULL);
  ASSERT (VrefPtr != NULL);
  MemPtr = NBPtr->MemPtr;
  Vref = *(UINT8*)VrefPtr;
  CallOutIdInfo.IdField.SocketId = NBPtr->MCTPtr->SocketId;
  CallOutIdInfo.IdField.ModuleId = NBPtr->MCTPtr->DieId;
  LibAmdMemCopy ((VOID *)&Va, (VOID *)MemPtr, (UINTN)sizeof (Va.StdHeader), &MemPtr->StdHeader);
  Va.MemData = MemPtr;
  Status = AGESA_SUCCESS;
  if (NBPtr->TechPtr->Direction == DQS_READ_DIR) {
    if (NBPtr->RefPtr->ExternalVrefCtl == FALSE) {
      //
      // Internal vref control
      //
      ASSERT (Vref < 61);
      if (Vref < 30) {
        Vref = (62 - Vref);
      } else {
        Vref = (Vref - 30);
      }
      NBPtr->SetBitField (NBPtr, BFVrefDAC, Vref << 3);
    } else {
      //
      // External vref control
      //
      AGESA_TESTPOINT (TpProcMemBefore2dTrainExtVrefChange, &(NBPtr->MemPtr->StdHeader));
      NBPtr->MemPtr->ParameterListPtr->ExternalVrefValue = Vref;
      IDS_HDT_CONSOLE (MEM_FLOW, "\n2D Read Training External CPU Vref Callout \n");
      Va.VoltageType = VTYPE_CPU_VREF;
      Va.AdjustValue = Vref = (Vref - 15) << 1;
      Status = AgesaExternalVoltageAdjust ((UINTN)CallOutIdInfo.IdInformation, &Va);
      AGESA_TESTPOINT (TpProcMemAfter2dTrainExtVrefChange, &(NBPtr->MemPtr->StdHeader));
    }
  } else {
    //
    // DIMM Vref Control
    //
    Va.VoltageType = VTYPE_DIMM_VREF;
    //
    // Offset by 15 and multiply by 2.
    //
    Va.AdjustValue = Vref = (Vref - 15) << 1;
    Status = AgesaExternalVoltageAdjust ((UINTN)CallOutIdInfo.IdInformation, &Va);
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tDimm Vref = %c%d%    ", (Va.AdjustValue < 0) ? '-':'+', (Va.AdjustValue < 0) ? (0 - Va.AdjustValue) : Va.AdjustValue );
    if (Status != AGESA_SUCCESS) {
      IDS_HDT_CONSOLE (MEM_FLOW, "* Dimm Vref Callout Failed *");
    }
    IDS_HDT_CONSOLE (MEM_FLOW, "\n");
  }
  return (Status == AGESA_SUCCESS) ? TRUE : FALSE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function scales Vref from the range used in Data Collection to
 *     the range that is programmed into the register.
 *
 *     @param[in,out] *NBPtr - Pointer to the MEM_NB_BLOCK
 *     @param[in,out] *Vref  - Pointer to UINT8 Vref Value to scale.
 *
 *     @return       BOOLEAN
 *                             TRUE Function was implemented
 *
 */
BOOLEAN
STATIC
MemFRdWr2DScaleVrefML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Vref
  )
{
  *(UINT8*)Vref = *(UINT8*)Vref * 2;
  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs Vref to internal or external control for 2D Read
 *     or Write Training
 *
 *     @param[in,out] *NBPtr    - Pointer to the MEM_NB_BLOCK
 *     @param[in,out] *OptParam - Unused
 *
 *     @return          BOOLEAN
 *                                TRUE -  External Vref was selected
 *                                FALSE - Internal Vref was selected
 *
 */
BOOLEAN
STATIC
MemFRdWr2DProgramIntExtVrefSelectML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  )
{
  if (NBPtr->TechPtr->Direction == DQS_READ_DIR) {
    NBPtr->SetBitField (NBPtr, BFVrefSel, (NBPtr->RefPtr->ExternalVrefCtl ? 0x0002 : 0x0001));
  }
  return NBPtr->RefPtr->ExternalVrefCtl;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs the Data Pattern that will be sent and compared
 *     against.
 *
 *     @param[in,out] *NBPtr           - Pointer to the MEM_NB_BLOCK
 *     @param[in]     *PatternIndexPtr - Pointer to a generic index used to
 *                                         determine which pattern to program.
 *
 *     @return                 BOOLEAN
 *                                       TRUE
 *
 */
BOOLEAN
STATIC
MemFRdWr2DProgramDataPatternML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID* PatternIndexPtr
  )
{
  UINT8 SeedCount;
  UINT32 PrbsSeed;
  CONST STATIC UINT32 CmdStreamLenTbl[4] = {13, 61, 127, 251};

  ASSERT (NBPtr != 0);
  ASSERT (PatternIndexPtr != NULL);
  SeedCount = *(UINT8*)PatternIndexPtr;
  ASSERT (SeedCount <= (NBPtr->MaxSeedCount - 1));
  MemNSetBitFieldNb (NBPtr, BFCmdStreamLen, CmdStreamLenTbl[SeedCount]);
  PrbsSeed = 0x7EA05;
  switch (SeedCount) {
  case 0:
    MemNSetBitFieldNb (NBPtr, BFDataPatGenSel, 0x01);
    PrbsSeed = 0x7FFFF;
    break;
  case 1:
    MemNSetBitFieldNb (NBPtr, BFDataPatGenSel, 0x04);
    MemNSetBitFieldNb (NBPtr, BFXorPatOvr, 0xFF);
    PrbsSeed = 0x7EA05;
    break;
  case 2:
    MemNSetBitFieldNb (NBPtr, BFDataPatGenSel, 0x03);
    MemNSetBitFieldNb (NBPtr, BFDramUserDataPattern0, 0x55555549);
    MemNSetBitFieldNb (NBPtr, BFDramUserDataPattern1, 0x55555555);
    MemNSetBitFieldNb (NBPtr, BFDramUserDataPattern2, 0x55555555);
    MemNSetBitFieldNb (NBPtr, BFDramUserDataPattern3, 0x55555555);
    break;
  case 3:
    MemNSetBitFieldNb (NBPtr, BFDataPatGenSel, 0x03);
    MemNSetBitFieldNb (NBPtr, BFDramUserDataPattern0, 0xA5A5A55A);
    MemNSetBitFieldNb (NBPtr, BFDramUserDataPattern1, 0xA5A5A5A5);
    MemNSetBitFieldNb (NBPtr, BFDramUserDataPattern2, 0xA5A5A5A5);
    MemNSetBitFieldNb (NBPtr, BFDramUserDataPattern3, 0xA5A5A5A5);
    break;
  default:
    ASSERT (FALSE);
  }
  ASSERT (PrbsSeed != 0);
  //
  // Program the PRBS Seed
  //
  NBPtr->SetBitField (NBPtr, BFDataPrbsSeed, PrbsSeed);
  return TRUE;
}