/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of build option: Memory
 *
 * Contains AMD AGESA install macros and test conditions. Output is the
 * defaults tables reflecting the User's build options selection.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Options
 * @e \$Revision: 282320 $   @e \$Date: 2014-01-09 20:13:34 -0600 (Thu, 09 Jan 2014) $
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
 *
 ***************************************************************************/

#ifndef _OPTION_MEMORY_INSTALL_H_
#define _OPTION_MEMORY_INSTALL_H_

/* Memory Includes */
#include "OptionMemory.h"

/*-------------------------------------------------------------------------------
 *  This option file is designed to be included into the platform solution install
 *  file. The platform solution install file will define the options status.
 *  Check to validate the definition
 */

/*----------------------------------------------------------------------------------
 * FEATURE BLOCK FUNCTIONS
 *
 *  This section defines function names that depend upon options that are selected
 *  in the platform solution install file.
 */
BOOLEAN MemFDefRet (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  return FALSE;
}

BOOLEAN MemMDefRet (
  IN   MEM_MAIN_DATA_BLOCK *MMPtr
  )
{
  return TRUE;
}

BOOLEAN MemMDefRetFalse (
  IN   MEM_MAIN_DATA_BLOCK *MMPtr
  )
{
  return FALSE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function initializes the northbridge block for dimm identification translator
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *MemPtr  - Pointer to the MEM_DATA_STRUCT
 *     @param[in,out]   NodeID   - ID of current node to construct
 *     @return          TRUE     - This is the correct constructor for the targeted node.
 *     @return          FALSE    - This isn't the correct constructor for the targeted node.
 */
BOOLEAN MemNIdentifyDimmConstructorRetDef (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   MEM_DATA_STRUCT *MemPtr,
  IN       UINT8 NodeID
  )
{
  return FALSE;
}

AGESA_STATUS MemNTransSysAddrToCsRetDef (
  IN OUT   AMD_IDENTIFY_DIMM *AmdDimmIdentify,
  IN       MEM_MAIN_DATA_BLOCK *mmPtr
  )
{
  return AGESA_ERROR;
}

/*----------------------------------------------------------------------------------
 * TABLE FEATURE BLOCK FUNCTIONS
 *
 *  This section defines function names that depend upon options that are selected
 *  in the platform solution install file.
 */
UINT8 MemFTableDefRet (
  IN OUT   MEM_TABLE_ALIAS **MTPtr
  )
{
  return 0;
}
/*----------------------------------------------------------------------------------
 * FEATURE S3 BLOCK FUNCTIONS
 *
 *  This section defines function names that depend upon options that are selected
 *  in the platform solution install file.
 */
BOOLEAN MemFS3DefConstructorRet (
  IN OUT   VOID *S3NBPtr,
  IN OUT   MEM_DATA_STRUCT *MemPtr,
  IN       UINT8 NodeID
  )
{
  return TRUE;
}

#if (OPTION_MEMCTLR_ML == TRUE)
  #if ((AGESA_ENTRY_INIT_RESUME == TRUE) || (AGESA_ENTRY_INIT_S3SAVE == TRUE) || (AGESA_ENTRY_INIT_LATE_RESTORE == TRUE) || ((AGESA_ENTRY_INIT_POST) && (OPTION_S3_MEM_SUPPORT == TRUE)))
    #if (OPTION_S3_MEM_SUPPORT == TRUE)
      extern MEM_RESUME_CONSTRUCTOR MemS3ResumeConstructNBBlockML;
      #define MEM_FEATURE_S3_RESUME_CONSTRUCTOR_ML MemS3ResumeConstructNBBlockML
      #if ((AGESA_ENTRY_INIT_RESUME == TRUE) || (AGESA_ENTRY_INIT_S3SAVE == TRUE) || (AGESA_ENTRY_INIT_LATE_RESTORE == TRUE))
        extern OPTION_MEM_FEATURE_MAIN MemMS3Save;
        #define MEM_MAIN_FEATURE_MEM_S3_SAVE MemMS3Save
      #else
        #define MEM_MAIN_FEATURE_MEM_S3_SAVE MemMDefRet
      #endif
    #else
      extern MEM_RESUME_CONSTRUCTOR MemS3ResumeNoS3ConstructNBBlockML;
      #define MEM_FEATURE_S3_RESUME_CONSTRUCTOR_ML MemS3ResumeNoS3ConstructNBBlockML
      #define MEM_MAIN_FEATURE_MEM_S3_SAVE MemMDefRet
    #endif
  #else
    extern MEM_RESUME_CONSTRUCTOR MemS3ResumeNoS3ConstructNBBlockML;
    #define MEM_FEATURE_S3_RESUME_CONSTRUCTOR_ML MemS3ResumeNoS3ConstructNBBlockML
    #define MEM_MAIN_FEATURE_MEM_S3_SAVE MemMDefRet
  #endif
  #if (AGESA_ENTRY_INIT_GENERAL_SERVICES == TRUE)
    extern MEM_IDENDIMM_CONSTRUCTOR MemNIdentifyDimmConstructorML;
    #define MEM_IDENDIMM_ML MemNIdentifyDimmConstructorML
    extern MEM_TRANS_SYS_ADDR_TO_CS MemNTransSysAddrToCsML;
    #define MEM_TRANS_SYS_ADDR_TO_CS_ML MemNTransSysAddrToCsML
  #else
    #define MEM_IDENDIMM_ML MemNIdentifyDimmConstructorRetDef
    #define MEM_TRANS_SYS_ADDR_TO_CS_ML MemNTransSysAddrToCsRetDef
  #endif
#endif

/*----------------------------------------------------------------------------------
 * NORTHBRIDGE BLOCK CONSTRUCTOR AND INITIALIZER FUNCTION DEFAULT ASSIGNMENTS
 *
 *----------------------------------------------------------------------------------
*/
#define MEM_NB_SUPPORT_ML
#define MEM_NB_SUPPORT_END { MEM_NB_SUPPORT_STRUCT_VERSION, 0, 0, 0, 0, 0 }

#if (AGESA_ENTRY_INIT_POST == TRUE)
  /*----------------------------------------------------------------------------------
   * FLOW CONTROL FUNCTION
   *
   *  This section selects the function that controls the memory initialization sequence
   *  based upon the number of processor families that the BIOS will support.
   */
  extern MEM_FLOW_CFG MemMFlowDef;

  #if (OPTION_MEMCTLR_ML == TRUE)
    extern MEM_FLOW_CFG MemMFlowML;
    #define MEM_MAIN_FLOW_CONTROL_PTR_ML MemMFlowML,
  #else
    extern MEM_FLOW_CFG MemMFlowDef;
    #define MEM_MAIN_FLOW_CONTROL_PTR_ML MemMFlowDef,
  #endif

  MEM_FLOW_CFG* memFlowControlInstalled[] = {
    MEM_MAIN_FLOW_CONTROL_PTR_ML
    NULL
  };

  #if (OPTION_ONLINE_SPARE == TRUE)
    extern OPTION_MEM_FEATURE_MAIN MemMOnlineSpare;
    #define MEM_MAIN_FEATURE_ONLINE_SPARE  MemMOnlineSpare
    extern OPTION_MEM_FEATURE_NB MemFOnlineSpare;
    #define MEM_FEATURE_ONLINE_SPARE  MemFOnlineSpare
  #else
    #define MEM_MAIN_FEATURE_ONLINE_SPARE  MemMDefRet
    #define MEM_FEATURE_ONLINE_SPARE  MemFDefRet
  #endif

  #if (OPTION_MEM_RESTORE == TRUE)
    extern OPTION_MEM_FEATURE_MAIN MemMContextSave;
    extern OPTION_MEM_FEATURE_MAIN MemMContextRestore;
    #define MEM_MAIN_FEATURE_MEM_SAVE     MemMContextSave
    #define MEM_MAIN_FEATURE_MEM_RESTORE  MemMContextRestore
  #else
    #define MEM_MAIN_FEATURE_MEM_SAVE     MemMDefRet
    #define MEM_MAIN_FEATURE_MEM_RESTORE  MemMDefRetFalse
  #endif

  #if (OPTION_BANK_INTERLEAVE == TRUE)
    extern OPTION_MEM_FEATURE_NB MemFInterleaveBanks;
    #define MEM_FEATURE_BANK_INTERLEAVE  MemFInterleaveBanks
    extern OPTION_MEM_FEATURE_NB MemFUndoInterleaveBanks;
    #define MEM_FEATURE_UNDO_BANK_INTERLEAVE MemFUndoInterleaveBanks
  #else
    #define MEM_FEATURE_BANK_INTERLEAVE  MemFDefRet
    #define MEM_FEATURE_UNDO_BANK_INTERLEAVE MemFDefRet
  #endif

  #if (OPTION_NODE_INTERLEAVE == TRUE)
    extern OPTION_MEM_FEATURE_MAIN MemMInterleaveNodes;
    #define MEM_MAIN_FEATURE_NODE_INTERLEAVE  MemMInterleaveNodes
    extern OPTION_MEM_FEATURE_NB MemFCheckInterleaveNodes;
    extern OPTION_MEM_FEATURE_NB MemFInterleaveNodes;
    #define MEM_FEATURE_NODE_INTERLEAVE_CHECK  MemFCheckInterleaveNodes
    #define MEM_FEATURE_NODE_INTERLEAVE  MemFInterleaveNodes
  #else
    #define MEM_FEATURE_NODE_INTERLEAVE_CHECK  MemFDefRet
    #define MEM_FEATURE_NODE_INTERLEAVE  MemFDefRet
    #define MEM_MAIN_FEATURE_NODE_INTERLEAVE  MemMDefRet
  #endif

  #if (OPTION_DCT_INTERLEAVE == TRUE)
    extern OPTION_MEM_FEATURE_NB MemFInterleaveChannels;
    #define MEM_FEATURE_CHANNEL_INTERLEAVE  MemFInterleaveChannels
  #else
    #define MEM_FEATURE_CHANNEL_INTERLEAVE  MemFDefRet
  #endif

  #if (OPTION_ECC == TRUE)
    extern OPTION_MEM_FEATURE_MAIN MemMEcc;
    #define MEM_MAIN_FEATURE_ECC  MemMEcc
    extern OPTION_MEM_FEATURE_NB MemFCheckECC;
    extern OPTION_MEM_FEATURE_NB MemFInitECC;
    #define MEM_FEATURE_CK_ECC   MemFCheckECC
    #define MEM_FEATURE_ECC   MemFInitECC
    #define MEM_FEATURE_ECCX8  MemMDefRet
  #else
    #define MEM_MAIN_FEATURE_ECC  MemMDefRet
    #define MEM_FEATURE_CK_ECC   MemFDefRet
    #define MEM_FEATURE_ECC   MemFDefRet
    #define MEM_FEATURE_ECCX8  MemMDefRet
  #endif

  #if (OPTION_EMP == TRUE)
    extern OPTION_MEM_FEATURE_NB MemFInitEMP;
    #define MEM_FEATURE_EMP   MemFInitEMP
  #else
    #define MEM_FEATURE_EMP   MemFDefRet
  #endif

  extern OPTION_MEM_FEATURE_MAIN MemMMctMemClr;
  #define MEM_MAIN_FEATURE_MEM_CLEAR  MemMMctMemClr

  #if (OPTION_AGGRESSOR == TRUE)
    extern OPTION_MEM_FEATURE_MAIN MemMAggressor;
    #define MEM_MAIN_FEATURE_AGGRESSOR  MemMAggressor
    extern OPTION_MEM_FEATURE_NB MemFAggressorInit;
    #define MEM_FEATURE_AGGRESSOR  MemFAggressorInit
  #else //#if (OPTION_AGGRESSOR == FALSE)
    #define MEM_MAIN_FEATURE_AGGRESSOR  MemMDefRet
    #define MEM_FEATURE_AGGRESSOR  MemFDefRet
  #endif

  #if (OPTION_DMI == TRUE)
    #if (OPTION_DDR3 == TRUE)
      extern OPTION_MEM_FEATURE_MAIN MemFDMISupport3;
      #define MEM_MAIN_FEATURE_MEM_DMI MemFDMISupport3
    #else
      #define MEM_MAIN_FEATURE_MEM_DMI MemMDefRet
    #endif
  #else
    #define MEM_MAIN_FEATURE_MEM_DMI MemMDefRet
  #endif

  #if (OPTION_CRAT == TRUE)
    extern OPTION_MEM_FEATURE_MAIN MemFCratSupport;
    #define MEM_MAIN_FEATURE_MEM_CRAT MemFCratSupport
  #else
    #define MEM_MAIN_FEATURE_MEM_CRAT MemMDefRet
  #endif

  #if (OPTION_DDR3 == TRUE)
    extern OPTION_MEM_FEATURE_NB MemFOnDimmThermal;
    extern OPTION_MEM_FEATURE_MAIN MemMLvDdr3;
    extern OPTION_MEM_FEATURE_NB MemFLvDdr3;
    #define MEM_FEATURE_ONDIMMTHERMAL MemFOnDimmThermal
    #define MEM_MAIN_FEATURE_LVDDR3 MemMLvDdr3
    #define MEM_FEATURE_LVDDR3 MemFLvDdr3
  #else
    #define MEM_FEATURE_ONDIMMTHERMAL MemFDefRet
    #define MEM_MAIN_FEATURE_LVDDR3 MemMDefRet
    #define MEM_FEATURE_LVDDR3 MemFDefRet
  #endif

  extern OPTION_MEM_FEATURE_NB MemFInterleaveRegion;
  #define MEM_FEATURE_REGION_INTERLEAVE    MemFInterleaveRegion

  extern OPTION_MEM_FEATURE_MAIN MemMUmaAlloc;
  #define MEM_MAIN_FEATURE_UMAALLOC   MemMUmaAlloc

  #if (OPTION_PARALLEL_TRAINING == TRUE)
    extern OPTION_MEM_FEATURE_MAIN MemMParallelTraining;
    #define MEM_MAIN_FEATURE_TRAINING  MemMParallelTraining
  #else
    extern OPTION_MEM_FEATURE_MAIN MemMStandardTraining;
    #define MEM_MAIN_FEATURE_TRAINING  MemMStandardTraining
  #endif

  #if (OPTION_DIMM_EXCLUDE == TRUE)
    extern OPTION_MEM_FEATURE_MAIN MemMRASExcludeDIMM;
    #define MEM_MAIN_FEATURE_DIMM_EXCLUDE  MemMRASExcludeDIMM
    extern OPTION_MEM_FEATURE_NB MemFRASExcludeDIMM;
    #define MEM_FEATURE_DIMM_EXCLUDE  MemFRASExcludeDIMM
  #else
    #define MEM_FEATURE_DIMM_EXCLUDE  MemFDefRet
    #define MEM_MAIN_FEATURE_DIMM_EXCLUDE  MemMDefRet
  #endif

  #if (OPTION_AMP == TRUE)
    extern OPTION_MEM_FEATURE_NB MemFAMP;
    #define MEM_FEATURE_AMP  MemFAMP
  #else
    #define MEM_FEATURE_AMP  MemFDefRet
  #endif

  #if (OPTION_DATA_EYE == TRUE)
    extern OPTION_MEM_FEATURE_NB MemF2DDataEyeInit;
    #define MEM_FEATURE_DATA_EYE  MemF2DDataEyeInit
  #else //#if (OPTION_DATA_EYE == FALSE)
    #define MEM_FEATURE_DATA_EYE  MemFDefRet
  #endif

  /*----------------------------------------------------------------------------------
   * TECHNOLOGY BLOCK CONSTRUCTOR FUNCTION ASSIGNMENTS
   *
   *----------------------------------------------------------------------------------
  */
  #if OPTION_DDR2 == TRUE
    extern MEM_TECH_CONSTRUCTOR MemConstructTechBlock2;
    #define MEM_TECH_CONSTRUCTOR_DDR2 MemConstructTechBlock2,
    #if (OPTION_HW_DRAM_INIT == TRUE)
      extern MEM_TECH_FEAT MemTDramInitHw;
      #define MEM_TECH_FEATURE_HW_DRAMINIT  MemTDramInitHw
    #else
      #define MEM_TECH_FEATURE_HW_DRAMINIT  MemTFeatDef
    #endif
    #if (OPTION_SW_DRAM_INIT == TRUE)
      #define MEM_TECH_FEATURE_SW_DRAMINIT  MemTFeatDef
    #else
      #define MEM_TECH_FEATURE_SW_DRAMINIT  MemTFeatDef
    #endif
  #else
    #define MEM_TECH_CONSTRUCTOR_DDR2
  #endif
  #if OPTION_DDR3 == TRUE
    extern MEM_TECH_CONSTRUCTOR MemConstructTechBlock3;
    #define MEM_TECH_CONSTRUCTOR_DDR3 MemConstructTechBlock3,
    #if (OPTION_HW_DRAM_INIT == TRUE)
      extern MEM_TECH_FEAT MemTDramInitHw;
      #define MEM_TECH_FEATURE_HW_DRAMINIT  MemTDramInitHw
    #else
      #define  MEM_TECH_FEATURE_HW_DRAMINIT MemTFeatDef
    #endif
    #if (OPTION_SW_DRAM_INIT == TRUE)
      #define MEM_TECH_FEATURE_SW_DRAMINIT  MemTDramInitSw3
    #else
      #define MEM_TECH_FEATURE_SW_DRAMINIT  MemTFeatDef
    #endif
  #else
    #define MEM_TECH_CONSTRUCTOR_DDR3
  #endif

  /*---------------------------------------------------------------------------------------------------
   * FEATURE BLOCKS
   *
   *  This section instantiates a feature block structure for each memory controller installed
   *  by the platform solution install file.
   *---------------------------------------------------------------------------------------------------
   */

  #if (OPTION_HW_DQS_REC_EN_TRAINING == TRUE)
    extern OPTION_MEM_FEATURE_NB MemNInitDqsTrainRcvrEnHwNb;
  #endif
  extern OPTION_MEM_FEATURE_NB MemFStandardTraining;

/*---------------------------------------------------------------------------------------------------
   * MULLINS FEATURE BLOCK
   *---------------------------------------------------------------------------------------------------
   */
  #if (OPTION_MEMCTLR_ML == TRUE)
    #if OPTION_DDR2
      #undef MEM_TECH_FEATURE_DRAMINIT
      #define MEM_TECH_FEATURE_DRAMINIT MEM_TECH_FEATURE_HW_DRAMINIT
    #endif
    #if OPTION_DDR3
      #undef MEM_MAIN_FEATURE_LVDDR3
      extern OPTION_MEM_FEATURE_MAIN MemMLvDdr3PerformanceEnhPre;
      #define MEM_MAIN_FEATURE_LVDDR3 MemMLvDdr3PerformanceEnhPre
      #undef MEM_TECH_FEATURE_DRAMINIT
      #define MEM_TECH_FEATURE_DRAMINIT MEM_TECH_FEATURE_SW_DRAMINIT
    #endif

    #if (OPTION_EARLY_SAMPLES == TRUE)
      extern OPTION_MEM_FEATURE_NB MemNInitEarlySampleSupportML;
      #define MEM_EARLY_SAMPLE_SUPPORT    MemNInitEarlySampleSupportML
    #else
      #define MEM_EARLY_SAMPLE_SUPPORT    MemFDefRet
    #endif

    #if (OPTION_CONTINOUS_PATTERN_GENERATION == TRUE)
      extern OPTION_MEM_FEATURE_NB MemNInitCPGML;
      #undef MEM_TECH_FEATURE_CPG
      #define MEM_TECH_FEATURE_CPG    MemNInitCPGML
    #else
      #undef MEM_TECH_FEATURE_CPG
      #define MEM_TECH_FEATURE_CPG    MemFDefRet
    #endif

    #if (OPTION_HW_DQS_REC_EN_TRAINING == TRUE)
      extern OPTION_MEM_FEATURE_NB MemNInitDqsTrainRcvrEnHwNb;
      #undef MEM_TECH_FEATURE_HWRXEN
      #define MEM_TECH_FEATURE_HWRXEN    MemNInitDqsTrainRcvrEnHwNb
    #else
      extern OPTION_MEM_FEATURE_NB MemNDisableDqsTrainRcvrEnHwNb;
      #undef MEM_TECH_FEATURE_HWRXEN
      #define MEM_TECH_FEATURE_HWRXEN    MemNDisableDqsTrainRcvrEnHwNb
    #endif


    #undef MEM_MAIN_FEATURE_TRAINING
    #undef MEM_FEATURE_TRAINING
    #if (OPTION_RDDQS_2D_TRAINING == TRUE)
      extern OPTION_MEM_FEATURE_MAIN MemMStandardTrainingUsingAdjacentDies;
      #define MEM_MAIN_FEATURE_TRAINING  MemMStandardTrainingUsingAdjacentDies
      extern OPTION_MEM_FEATURE_NB MemFRdWr2DTrainingInitML;
      #define MEM_FEATURE_RDWR_2D_TRAINING MemFRdWr2DTrainingInitML
    #else
      extern OPTION_MEM_FEATURE_MAIN MemMStandardTraining;
      #define MEM_MAIN_FEATURE_TRAINING  MemMStandardTraining
      #define MEM_FEATURE_RDWR_2D_TRAINING MemFDefRet
    #endif
    extern OPTION_MEM_FEATURE_NB MemFStandardTraining;
    #define MEM_FEATURE_TRAINING  MemFStandardTraining

    #undef MEM_FEATURE_DLLPDBYPASS
    #if (OPTION_DLLPDBYPASS == TRUE)
      extern OPTION_MEM_FEATURE_NB MemFDllPDBypassInit;
      #define MEM_FEATURE_DLLPDBYPASS MemFDllPDBypassInit
    #else
      #define MEM_FEATURE_DLLPDBYPASS MemFDefRet
    #endif

    MEM_FEAT_BLOCK_NB  MemFeatBlockML = {
      MEM_FEAT_BLOCK_NB_STRUCT_VERSION,
      MEM_FEATURE_ONLINE_SPARE,
      MEM_FEATURE_BANK_INTERLEAVE,
      MEM_FEATURE_UNDO_BANK_INTERLEAVE,
      MemFDefRet,
      MemFDefRet,
      MemFDefRet,
      MemFDefRet,
      MEM_FEATURE_CK_ECC,
      MEM_FEATURE_ECC,
      MEM_FEATURE_TRAINING,
      MEM_FEATURE_LVDDR3,
      MEM_FEATURE_ONDIMMTHERMAL,
      MEM_TECH_FEATURE_DRAMINIT,
      MEM_FEATURE_DIMM_EXCLUDE,
      MEM_EARLY_SAMPLE_SUPPORT,
      MEM_TECH_FEATURE_CPG,
      MEM_TECH_FEATURE_HWRXEN,
      MemFDefRet,
      MemFDefRet,
      MEM_FEATURE_RDWR_2D_TRAINING,
      MEM_FEATURE_DLLPDBYPASS,
      MemFDefRet,
      MemFDefRet
    };

    #undef MEM_NB_SUPPORT_ML
    extern MEM_NB_CONSTRUCTOR MemConstructNBBlockML;
    extern MEM_INITIALIZER MemNInitDefaultsML;
    #define MEM_NB_SUPPORT_ML { MEM_NB_SUPPORT_STRUCT_VERSION, MemConstructNBBlockML, MemNInitDefaultsML, &MemFeatBlockML, MEM_FEATURE_S3_RESUME_CONSTRUCTOR_ML, MEM_IDENDIMM_ML, MEM_TRANS_SYS_ADDR_TO_CS_ML },
  #endif // OPTION_MEMCTRL_ML

  /*---------------------------------------------------------------------------------------------------
   * MAIN FEATURE BLOCK
   *---------------------------------------------------------------------------------------------------
   */
  MEM_FEAT_BLOCK_MAIN MemFeatMain = {
    MEM_FEAT_BLOCK_MAIN_STRUCT_VERSION,
    MEM_MAIN_FEATURE_TRAINING,
    MEM_MAIN_FEATURE_DIMM_EXCLUDE,
    MEM_MAIN_FEATURE_ONLINE_SPARE,
    MEM_MAIN_FEATURE_NODE_INTERLEAVE,
    MEM_MAIN_FEATURE_ECC,
    MEM_MAIN_FEATURE_MEM_CLEAR,
    MEM_MAIN_FEATURE_MEM_DMI,
    MEM_MAIN_FEATURE_MEM_CRAT,
    MEM_MAIN_FEATURE_LVDDR3,
    MEM_MAIN_FEATURE_UMAALLOC,
    MEM_MAIN_FEATURE_MEM_SAVE,
    MEM_MAIN_FEATURE_MEM_RESTORE,
    MEM_MAIN_FEATURE_MEM_S3_SAVE,
    MEM_MAIN_FEATURE_AGGRESSOR
  };


  /*---------------------------------------------------------------------------------------------------
   * Technology Training SPECIFIC CONFIGURATION
   *
   *
   *---------------------------------------------------------------------------------------------------
   */
  #define MEM_TECH_TRAINING_FEAT_NULL_TERNMIATOR 0

  #if OPTION_MEMCTLR_ML
    extern OPTION_MEM_FEATURE_NB memNEnableTrainSequenceML;
    #define TECH_TRAIN_ENTER_HW_TRN_DDR2 MemTFeatDef
    #define TECH_TRAIN_EXIT_HW_TRN_DDR2 MemTFeatDef
    #define TECH_TRAIN_HW_WL_P1_DDR2 MemTFeatDef
    #define TECH_TRAIN_HW_WL_P2_DDR2 MemTFeatDef
    #define TECH_TRAIN_SW_WL_DDR2 MemTFeatDef
    #define TECH_TRAIN_HW_DQS_REC_EN_P1_DDR2 MemTFeatDef
    #define TECH_TRAIN_HW_DQS_REC_EN_P2_DDR2 MemTFeatDef
    #define TECH_TRAIN_NON_OPT_SW_DQS_REC_EN_P1_DDR2 MemTFeatDef
    #define TECH_TRAIN_OPT_SW_DQS_REC_EN_P1_DDR2 MemTFeatDef
    #define TECH_TRAIN_NON_OPT_SW_RD_WR_POS_DDR2 MemTFeatDef
    #define TECH_TRAIN_OPT_SW_RD_WR_POS_DDR2 MemTFeatDef
    #define TECH_TRAIN_MAX_RD_LAT_DDR2 MemTFeatDef
    #define NB_TRAIN_FLOW_DDR2    (BOOLEAN (*) (MEM_NB_BLOCK*)) memDefTrue
    #define MEM_TECH_ENABLE_TRAINING_SEQUENCE_DDR2_ML  { MEM_TECH_TRAIN_SEQUENCE_STRUCT_VERSION, (BOOLEAN (*) (MEM_NB_BLOCK*)) memDefTrue, (BOOLEAN (*) (MEM_NB_BLOCK*)) memDefFalse, 0 },
    #if OPTION_DDR3
      #undef TECH_TRAIN_ENTER_HW_TRN_DDR3
      #define TECH_TRAIN_ENTER_HW_TRN_DDR3   MemTPreparePhyAssistedTraining
      #undef TECH_TRAIN_EXIT_HW_TRN_DDR3
      #define TECH_TRAIN_EXIT_HW_TRN_DDR3    MemTExitPhyAssistedTraining
      #if (OPTION_HW_WRITE_LEV_TRAINING == TRUE)
        #define TECH_TRAIN_HW_WL_P1_DDR3   MemTWriteLevelizationHw3Pass1
        #define TECH_TRAIN_HW_WL_P2_DDR3   MemTWriteLevelizationHw3Pass2
      #else
        #define TECH_TRAIN_HW_WL_P1_DDR3 MemTFeatDef
        #define TECH_TRAIN_HW_WL_P2_DDR3 MemTFeatDef
      #endif
      #if (OPTION_SW_WRITE_LEV_TRAINING == TRUE)
        #define TECH_TRAIN_SW_WL_DDR3    MemTFeatDef
      #else
        #define TECH_TRAIN_SW_WL_DDR3 MemTFeatDef
      #endif
      #if (OPTION_HW_DQS_REC_EN_TRAINING == TRUE)
        #ifdef TECH_TRAIN_HW_DQS_REC_EN_P1_DDR3
          #undef TECH_TRAIN_HW_DQS_REC_EN_P1_DDR3
        #endif
        #define TECH_TRAIN_HW_DQS_REC_EN_P1_DDR3    MemTDqsTrainRcvrEnHwPass1
        #ifdef TECH_TRAIN_HW_DQS_REC_EN_P2_DDR3
          #undef TECH_TRAIN_HW_DQS_REC_EN_P2_DDR3
        #endif
        #define TECH_TRAIN_HW_DQS_REC_EN_P2_DDR3    MemTDqsTrainRcvrEnHwPass2
        #if (OPTION_HW_DQS_REC_EN_SEED_TRAINING == TRUE)
          #undef TECH_TRAIN_HW_DQS_REC_EN_SEED_DDR3
          extern MEM_TECH_FEAT MemNRdPosTrnML;
          #define TECH_TRAIN_HW_DQS_REC_EN_SEED_DDR3    MemNRdPosTrnML
        #else
          #if (OPTION_OPT_SW_RD_WR_POS_TRAINING == TRUE  || OPTION_NON_OPT_SW_RD_WR_POS_TRAINING == TRUE)
            #define TECH_TRAIN_HW_DQS_REC_EN_SEED_DDR3    MemTTrainDQSEdgeDetect
          #else
            #define TECH_TRAIN_HW_DQS_REC_EN_SEED_DDR3 MemTFeatDef
          #endif
        #endif
      #else
        #define TECH_TRAIN_HW_DQS_REC_EN_P1_DDR3 MemTFeatDef
        #define TECH_TRAIN_HW_DQS_REC_EN_P2_DDR3 MemTFeatDef
        #if (OPTION_OPT_SW_RD_WR_POS_TRAINING == TRUE  || OPTION_NON_OPT_SW_RD_WR_POS_TRAINING == TRUE)
          #define TECH_TRAIN_HW_DQS_REC_EN_SEED_DDR3    MemTTrainDQSEdgeDetect
        #else
          #define TECH_TRAIN_HW_DQS_REC_EN_SEED_DDR3 MemTFeatDef
        #endif
      #endif
      #if (OPTION_NON_OPT_SW_DQS_REC_EN_TRAINING == TRUE)
        #define TECH_TRAIN_NON_OPT_SW_DQS_REC_EN_P1_DDR3    MemTFeatDef
      #else
        #define TECH_TRAIN_NON_OPT_SW_DQS_REC_EN_P1_DDR3 MemTFeatDef
      #endif
      #undef TECH_TRAIN_OPT_SW_DQS_REC_EN_P1_DDR3
      #if (OPTION_OPT_SW_DQS_REC_EN_TRAINING == TRUE)
        #define TECH_TRAIN_OPT_SW_DQS_REC_EN_P1_DDR3    MemTFeatDef
      #else
        #define TECH_TRAIN_OPT_SW_DQS_REC_EN_P1_DDR3 MemTFeatDef
      #endif
      #if (OPTION_NON_OPT_SW_RD_WR_POS_TRAINING == TRUE)
        #define TECH_TRAIN_NON_OPT_SW_RD_WR_POS_DDR3    MemTTrainDQSEdgeDetectSw
      #else
        #define TECH_TRAIN_NON_OPT_SW_RD_WR_POS_DDR3 MemTFeatDef
      #endif
      #undef TECH_TRAIN_OPT_SW_RD_WR_POS_DDR3
      #if (OPTION_OPT_SW_RD_WR_POS_TRAINING == TRUE)
        #define TECH_TRAIN_OPT_SW_RD_WR_POS_DDR3    MemTTrainDQSEdgeDetectSw
      #else
        #define TECH_TRAIN_OPT_SW_RD_WR_POS_DDR3 MemTFeatDef
      #endif
      #if (OPTION_MAX_RD_LAT_TRAINING == TRUE)
        #define TECH_TRAIN_MAX_RD_LAT_DDR3    MemTTrainMaxLatency
      #else
        #define TECH_TRAIN_MAX_RD_LAT_DDR3 MemTFeatDef
      #endif
      #if (OPTION_RDDQS_2D_TRAINING == TRUE)
        extern  MEM_TECH_FEAT MemFAmdRdDqs2DTraining;
        #undef TECH_TRAIN_DQS_2D_DDR3
        #define TECH_TRAIN_DQS_2D_DDR3    MemFAmdRdDqs2DTraining
      #else
        #undef TECH_TRAIN_DQS_2D_DDR3
        #define TECH_TRAIN_DQS_2D_DDR3 MemTFeatDef
      #endif
      MEM_TECH_FEAT_BLOCK  memTechTrainingFeatSequenceDDR3ML = {
        MEM_TECH_FEAT_BLOCK_STRUCT_VERSION,
        TECH_TRAIN_ENTER_HW_TRN_DDR3,
        TECH_TRAIN_SW_WL_DDR3,
        TECH_TRAIN_HW_WL_P1_DDR3,
        TECH_TRAIN_HW_DQS_REC_EN_P1_DDR3,
        TECH_TRAIN_HW_WL_P2_DDR3,
        TECH_TRAIN_HW_DQS_REC_EN_P2_DDR3,
        TECH_TRAIN_EXIT_HW_TRN_DDR3,
        TECH_TRAIN_NON_OPT_SW_DQS_REC_EN_P1_DDR3,
        TECH_TRAIN_OPT_SW_DQS_REC_EN_P1_DDR3,
        TECH_TRAIN_NON_OPT_SW_RD_WR_POS_DDR3,
        TECH_TRAIN_OPT_SW_RD_WR_POS_DDR3,
        TECH_TRAIN_MAX_RD_LAT_DDR3,
        TECH_TRAIN_HW_DQS_REC_EN_SEED_DDR3,
        TECH_TRAIN_DQS_2D_DDR3
      };
      extern OPTION_MEM_FEATURE_NB MemNDQSTiming3Nb;
      #define NB_TRAIN_FLOW_DDR3    MemNDQSTiming3Nb
      extern OPTION_MEM_FEATURE_NB memNSequenceDDR3Nb;
      #define MEM_TECH_ENABLE_TRAINING_SEQUENCE_DDR3_ML {MEM_TECH_TRAIN_SEQUENCE_STRUCT_VERSION,memNSequenceDDR3Nb, memNEnableTrainSequenceML, &memTechTrainingFeatSequenceDDR3ML },
    #else
      #undef TECH_TRAIN_ENTER_HW_TRN_DDR3
      #define TECH_TRAIN_ENTER_HW_TRN_DDR3 MemTFeatDef
      #undef TECH_TRAIN_EXIT_HW_TRN_DDR3
      #define TECH_TRAIN_EXIT_HW_TRN_DDR3 MemTFeatDef
      #define TECH_TRAIN_HW_WL_P1_DDR3 MemTFeatDef
      #define TECH_TRAIN_HW_WL_P2_DDR3 MemTFeatDef
      #define TECH_TRAIN_SW_WL_DDR3 MemTFeatDef
      #define TECH_TRAIN_HW_DQS_REC_EN_P1_DDR3 MemTFeatDef
      #define TECH_TRAIN_HW_DQS_REC_EN_P2_DDR3 MemTFeatDef
      #define TECH_TRAIN_NON_OPT_SW_DQS_REC_EN_P1_DDR3 MemTFeatDef
      #define TECH_TRAIN_OPT_SW_DQS_REC_EN_P1_DDR3 MemTFeatDef
      #define TECH_TRAIN_NON_OPT_SW_RD_WR_POS_DDR3 MemTFeatDef
      #define TECH_TRAIN_OPT_SW_RD_WR_POS_DDR3 MemTFeatDef
      #define TECH_TRAIN_MAX_RD_LAT_DDR3 MemTFeatDef
      #define NB_TRAIN_FLOW_DDR3    (BOOLEAN (*) (MEM_NB_BLOCK*)) memDefTrue
      #define MEM_TECH_ENABLE_TRAINING_SEQUENCE_DDR3_ML  { MEM_TECH_TRAIN_SEQUENCE_STRUCT_VERSION, (BOOLEAN (*) (MEM_NB_BLOCK*)) memDefTrue, (BOOLEAN (*) (MEM_NB_BLOCK*)) memDefFalse, 0 },
    #endif
  #else
    #define MEM_TECH_ENABLE_TRAINING_SEQUENCE_DDR2_ML  { MEM_TECH_TRAIN_SEQUENCE_STRUCT_VERSION, (BOOLEAN (*) (MEM_NB_BLOCK*)) memDefTrue, (BOOLEAN (*) (MEM_NB_BLOCK*)) memDefFalse, 0 },
    #define MEM_TECH_ENABLE_TRAINING_SEQUENCE_DDR3_ML  { MEM_TECH_TRAIN_SEQUENCE_STRUCT_VERSION, (BOOLEAN (*) (MEM_NB_BLOCK*)) memDefTrue, (BOOLEAN (*) (MEM_NB_BLOCK*)) memDefFalse, 0 },
  #endif

  #define MEM_TECH_ENABLE_TRAINING_SEQUENCE_END { MEM_NB_SUPPORT_STRUCT_VERSION, 0, 0, 0 }
  MEM_FEAT_TRAIN_SEQ memTrainSequenceDDR2[] = {
    MEM_TECH_ENABLE_TRAINING_SEQUENCE_DDR2_ML
    MEM_TECH_ENABLE_TRAINING_SEQUENCE_END
  };

  MEM_FEAT_TRAIN_SEQ memTrainSequenceDDR3[] = {
    MEM_TECH_ENABLE_TRAINING_SEQUENCE_DDR3_ML
    MEM_TECH_ENABLE_TRAINING_SEQUENCE_END
  };
  /*---------------------------------------------------------------------------------------------------
   * NB TRAINING FLOW CONTROL
   *
   *
   *---------------------------------------------------------------------------------------------------
   */
  OPTION_MEM_FEATURE_NB* memNTrainFlowControl[] = {    // Training flow control
    NB_TRAIN_FLOW_DDR2,
    NB_TRAIN_FLOW_DDR3,
  };
  /*---------------------------------------------------------------------------------------------------
   * TECHNOLOGY BLOCK
   *
   *
   *---------------------------------------------------------------------------------------------------
   */
  MEM_TECH_CONSTRUCTOR* memTechInstalled[] = {    // Types of technology installed
    MEM_TECH_CONSTRUCTOR_DDR2
    MEM_TECH_CONSTRUCTOR_DDR3
    NULL
  };
   /*---------------------------------------------------------------------------------------------------
   * PLATFORM SPECIFIC BLOCK FORM FACTOR DEFINITION
   *
   *
   *---------------------------------------------------------------------------------------------------
   */

  /*---------------------------------------------------------------------------------------------------
   * PLATFORM-SPECIFIC CONFIGURATION
   *
   *
   *---------------------------------------------------------------------------------------------------
   */

  /*----------------------------------------------------------------------
   * DEFAULT PSCFG DEFINITIONS
   *
   *----------------------------------------------------------------------
   */

  MEM_PLATFORM_CFG* memPlatformTypeInstalled[] = {
    NULL
  };
  CONST UINTN SIZE_OF_PLATFORM = (sizeof (memPlatformTypeInstalled) / sizeof (MEM_PLATFORM_CFG*));
  #if SIZE_OF_PLATFORM > MAX_PLATFORM_TYPES
    #error   Size of memPlatformTypeInstalled array larger than MAX_PLATFORM_TYPES
  #endif

  /*---------------------------------------------------------------------------------------------------
   * EXTRACTABLE PLATFORM SPECIFIC CONFIGURATION
   *
   *
   *---------------------------------------------------------------------------------------------------
   */
  #define MEM_PSC_FLOW_BLOCK_END NULL
  #define PSC_TBL_END NULL
  #define MEM_PSC_FLOW_DEFTRUE (BOOLEAN (*) (MEM_NB_BLOCK*, MEM_PSC_TABLE_BLOCK *)) memDefTrue

  #define PSC_TBL_ML_UDIMM3_S2D_FT3
  #define PSC_TBL_ML_SODIMM3_S2D_FT3
  #if OPTION_MEMCTLR_ML
    #if OPTION_FT3_SOCKET_SUPPORT
      extern PSC_TBL_ENTRY MLClkDisMapEntSOFT3;
      #define PSC_TBL_ML_CLK_DIS_FT3 &MLClkDisMapEntSOFT3,
      extern PSC_TBL_ENTRY MLSODdr3ODTTriEntFT3;
      #define PSC_TBL_ML_ODT_TRI_FT3 &MLSODdr3ODTTriEntFT3,
      extern PSC_TBL_ENTRY MLSODdr3CSTriEntFT3;
      #define PSC_TBL_ML_CS_TRI_FT3 &MLSODdr3CSTriEntFT3,
    #endif
    #if OPTION_UDIMMS
      extern PSC_TBL_ENTRY MLMaxFreqTblEntU6L;
      #define PSC_TBL_ML_UDIMM3_MAX_FREQ_6L  &MLMaxFreqTblEntU6L,
      extern PSC_TBL_ENTRY MLMaxFreqTblEntU4L;
      #define PSC_TBL_ML_UDIMM3_MAX_FREQ_4L  &MLMaxFreqTblEntU4L,
      extern PSC_TBL_ENTRY MLDramTermTblEntU;
      #define PSC_TBL_ML_UDIMM3_DRAM_TERM  &MLDramTermTblEntU,
      extern PSC_TBL_ENTRY MLSAOTblEntU3;
      #define PSC_TBL_ML_UDIMM3_SAO  &MLSAOTblEntU3,
      #undef PSC_TBL_ML_UDIMM3_S2D_FT3
      extern PSC_TBL_ENTRY MLS2DTblEntUFT3;
      #define PSC_TBL_ML_UDIMM3_S2D_FT3  &MLS2DTblEntUFT3,
    #endif
    #if OPTION_SODIMMS
      #if OPTION_FT3_SOCKET_SUPPORT
        extern PSC_TBL_ENTRY MLSAOTblEntSO3;
        #define PSC_TBL_ML_SODIMM3_SAO  &MLSAOTblEntSO3,
        extern PSC_TBL_ENTRY MLSAOTblEntSoDwnPlusSODIMM3;
        #define PSC_TBL_ML_SODWN_PLUS_SODIMM3_SAO  &MLSAOTblEntSoDwnPlusSODIMM3,
        extern PSC_TBL_ENTRY MLSAOTblEntSoDwn3;
        #define PSC_TBL_ML_SODWN_SAO  &MLSAOTblEntSoDwn3,
        extern PSC_TBL_ENTRY MLSAOTblEntSoDwn3LPM;
        #define PSC_TBL_ML_SODWN_SAO_LPM  &MLSAOTblEntSoDwn3LPM,
        extern PSC_TBL_ENTRY MLDramTermTblEntSO3;
        #define PSC_TBL_ML_SODIMM3_DRAM_TERM  &MLDramTermTblEntSO3,
        extern PSC_TBL_ENTRY MLDramTermTblEntSoDwnPlusSODIMM3;
        #define PSC_TBL_ML_SODWN_PLUS_SODIMM3_DRAM_TERM  &MLDramTermTblEntSoDwnPlusSODIMM3,
        extern PSC_TBL_ENTRY MLDramTermTblEntSoDwn3;
        #define PSC_TBL_ML_SODWN_DRAM_TERM  &MLDramTermTblEntSoDwn3,
        extern PSC_TBL_ENTRY MLDramTermTblEntSoDwn3LPM;
        #define PSC_TBL_ML_SODWN_DRAM_TERM_LPM  &MLDramTermTblEntSoDwn3LPM,
        extern PSC_TBL_ENTRY MLMaxFreqTblEntSO6L;
        #define PSC_TBL_ML_SODIMM3_MAX_FREQ_6L  &MLMaxFreqTblEntSO6L,
        extern PSC_TBL_ENTRY MLMaxFreqTblEntSO4L;
        #define PSC_TBL_ML_SODIMM3_MAX_FREQ_4L  &MLMaxFreqTblEntSO4L,
        extern PSC_TBL_ENTRY MLMaxFreqTblEntSoDwnPlusSODIMM6L;
        #define PSC_TBL_ML_SODWN_PLUS_SODIMM3_MAX_FREQ_6L  &MLMaxFreqTblEntSoDwnPlusSODIMM6L,
        extern PSC_TBL_ENTRY MLMaxFreqTblEntSoDwnPlusSODIMM4L;
        #define PSC_TBL_ML_SODWN_PLUS_SODIMM3_MAX_FREQ_4L  &MLMaxFreqTblEntSoDwnPlusSODIMM6L,
        extern PSC_TBL_ENTRY MLMaxFreqTblEntSoDwn;
        #define PSC_TBL_ML_SODWN_MAX_FREQ  &MLMaxFreqTblEntSoDwn,
        extern PSC_TBL_ENTRY MLMaxFreqTblEntSoDwnLPM;
        #define PSC_TBL_ML_SODWN_MAX_FREQ_LPM  &MLMaxFreqTblEntSoDwnLPM,
        #undef PSC_TBL_ML_SODIMM3_S2D_FT3
        extern PSC_TBL_ENTRY MLS2DTblEntUFT3;
        #define PSC_TBL_ML_SODIMM3_S2D_FT3 &MLS2DTblEntUFT3,
      #endif
    #endif
    extern PSC_TBL_ENTRY MLMR0WrTblEntry;
    extern PSC_TBL_ENTRY MLMR0CLTblEntry;
    extern PSC_TBL_ENTRY MLDdr3CKETriEnt;
    extern PSC_TBL_ENTRY ML1DOdtPatTblEnt;
    extern PSC_TBL_ENTRY ML2DOdtPatTblEnt;

    #ifndef PSC_TBL_ML_SODIMM3_MAX_FREQ_6L
      #define PSC_TBL_ML_SODIMM3_MAX_FREQ_6L
    #endif
    #ifndef PSC_TBL_ML_SODIMM3_MAX_FREQ_4L
      #define PSC_TBL_ML_SODIMM3_MAX_FREQ_4L
    #endif
    #ifndef PSC_TBL_ML_SODWN_PLUS_SODIMM3_MAX_FREQ_6L
      #define PSC_TBL_ML_SODWN_PLUS_SODIMM3_MAX_FREQ_6L
    #endif
    #ifndef PSC_TBL_ML_SODWN_PLUS_SODIMM3_MAX_FREQ_4L
      #define PSC_TBL_ML_SODWN_PLUS_SODIMM3_MAX_FREQ_4L
    #endif
    #ifndef PSC_TBL_ML_SODWN_MAX_FREQ
      #define PSC_TBL_ML_SODWN_MAX_FREQ
    #endif
    #ifndef PSC_TBL_ML_SODWN_MAX_FREQ_LPM
      #define PSC_TBL_ML_SODWN_MAX_FREQ_LPM
    #endif
    #ifndef PSC_TBL_ML_UDIMM3_MAX_FREQ_6L
      #define PSC_TBL_ML_UDIMM3_MAX_FREQ_6L
    #endif
    #ifndef PSC_TBL_ML_UDIMM3_MAX_FREQ_4L
      #define PSC_TBL_ML_UDIMM3_MAX_FREQ_4L
    #endif
    #ifndef PSC_TBL_ML_UDIMM3_DRAM_TERM
      #define PSC_TBL_ML_UDIMM3_DRAM_TERM
    #endif
    #ifndef PSC_TBL_ML_SODIMM3_DRAM_TERM
      #define PSC_TBL_ML_SODIMM3_DRAM_TERM
    #endif
    #ifndef PSC_TBL_ML_SODWN_PLUS_SODIMM3_DRAM_TERM
      #define PSC_TBL_ML_SODWN_PLUS_SODIMM3_DRAM_TERM
    #endif
    #ifndef PSC_TBL_ML_SODWN_DRAM_TERM
      #define PSC_TBL_ML_SODWN_DRAM_TERM
    #endif
    #ifndef PSC_TBL_ML_SODWN_DRAM_TERM_LPM
      #define PSC_TBL_ML_SODWN_DRAM_TERM_LPM
    #endif
    #ifndef PSC_TBL_ML_SODIMM3_SAO
      #define PSC_TBL_ML_SODIMM3_SAO
    #endif
    #ifndef PSC_TBL_ML_SODWN_PLUS_SODIMM3_SAO
      #define PSC_TBL_ML_SODWN_PLUS_SODIMM3_SAO
    #endif
    #ifndef PSC_TBL_ML_SODWN_SAO
      #define PSC_TBL_ML_SODWN_SAO
    #endif
    #ifndef PSC_TBL_ML_SODWN_SAO_LPM
      #define PSC_TBL_ML_SODWN_SAO_LPM
    #endif
    #ifndef PSC_TBL_ML_UDIMM3_SAO
      #define PSC_TBL_ML_UDIMM3_SAO
    #endif
    #ifndef PSC_TBL_ML_CLK_DIS_FT3
      #define PSC_TBL_ML_CLK_DIS_FT3
    #endif
    #ifndef PSC_TBL_ML_ODT_TRI_FT3
      #define PSC_TBL_ML_ODT_TRI_FT3
    #endif
    #ifndef PSC_TBL_ML_CS_TRI_FT3
      #define PSC_TBL_ML_CS_TRI_FT3
    #endif

    PSC_TBL_ENTRY* memPSCTblMaxFreqArrayML[] = {
      PSC_TBL_ML_SODIMM3_MAX_FREQ_4L
      PSC_TBL_ML_SODWN_PLUS_SODIMM3_MAX_FREQ_4L
      PSC_TBL_ML_UDIMM3_MAX_FREQ_4L
      PSC_TBL_ML_SODIMM3_MAX_FREQ_6L
      PSC_TBL_ML_SODWN_PLUS_SODIMM3_MAX_FREQ_6L
      PSC_TBL_ML_UDIMM3_MAX_FREQ_6L
      PSC_TBL_ML_SODWN_MAX_FREQ
      PSC_TBL_ML_SODWN_MAX_FREQ_LPM
      PSC_TBL_END
    };

    PSC_TBL_ENTRY* memPSCTblDramTermArrayML[] = {
      PSC_TBL_ML_UDIMM3_DRAM_TERM
      PSC_TBL_ML_SODIMM3_DRAM_TERM
      PSC_TBL_ML_SODWN_PLUS_SODIMM3_DRAM_TERM
      PSC_TBL_ML_SODWN_DRAM_TERM
      PSC_TBL_ML_SODWN_DRAM_TERM_LPM
      PSC_TBL_END
    };

    PSC_TBL_ENTRY* memPSCTblODTPatArrayML[] = {
      &ML1DOdtPatTblEnt,
      &ML2DOdtPatTblEnt,
      PSC_TBL_END
    };

    PSC_TBL_ENTRY* memPSCTblSAOArrayML[] = {
      PSC_TBL_ML_SODIMM3_SAO
      PSC_TBL_ML_SODWN_PLUS_SODIMM3_SAO
      PSC_TBL_ML_SODWN_SAO
      PSC_TBL_ML_SODWN_SAO_LPM
      PSC_TBL_ML_UDIMM3_SAO
      PSC_TBL_END
    };

    PSC_TBL_ENTRY* memPSCTblMR0WRArrayML[] = {
      &MLMR0WrTblEntry,
      PSC_TBL_END
    };

    PSC_TBL_ENTRY* memPSCTblMR0CLArrayML[] = {
      &MLMR0CLTblEntry,
      PSC_TBL_END
    };

    PSC_TBL_ENTRY* memPSCTblGenArrayML[] = {
      &MLDdr3CKETriEnt,
      PSC_TBL_ML_CLK_DIS_FT3
      PSC_TBL_ML_ODT_TRI_FT3
      PSC_TBL_ML_CS_TRI_FT3
      PSC_TBL_END
    };

    PSC_TBL_ENTRY* memPSCTblS2DArrayML[] = {
      PSC_TBL_ML_UDIMM3_S2D_FT3
      PSC_TBL_ML_SODIMM3_S2D_FT3
      PSC_TBL_END
    };

    MEM_PSC_TABLE_BLOCK memPSCTblBlockML = {
      (PSC_TBL_ENTRY **)&memPSCTblMaxFreqArrayML,
      (PSC_TBL_ENTRY **)&memPSCTblDramTermArrayML,
      (PSC_TBL_ENTRY **)&memPSCTblODTPatArrayML,
      (PSC_TBL_ENTRY **)&memPSCTblSAOArrayML,
      (PSC_TBL_ENTRY **)&memPSCTblMR0WRArrayML,
      (PSC_TBL_ENTRY **)&memPSCTblMR0CLArrayML,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      (PSC_TBL_ENTRY **)&memPSCTblGenArrayML,
      (PSC_TBL_ENTRY **)&memPSCTblS2DArrayML,
      NULL,
      NULL
    };

    extern MEM_PSC_FLOW MemPGetMaxFreqSupported;
    extern MEM_PSC_FLOW MemPGetRttNomWr;
    extern MEM_PSC_FLOW MemPGetODTPattern;
    extern MEM_PSC_FLOW MemPGetSAO;
    extern MEM_PSC_FLOW MemPGetMR0WrCL;
    extern MEM_PSC_FLOW MemPGetS2D;

    MEM_PSC_FLOW_BLOCK memPlatSpecFlowML = {
      &memPSCTblBlockML,
      MemPGetMaxFreqSupported,
      MemPGetRttNomWr,
      MemPGetODTPattern,
      MemPGetSAO,
      MemPGetMR0WrCL,
      MEM_PSC_FLOW_DEFTRUE,
      MEM_PSC_FLOW_DEFTRUE,
      MEM_PSC_FLOW_DEFTRUE,
      MEM_PSC_FLOW_DEFTRUE,
      MEM_PSC_FLOW_DEFTRUE,
      MemPGetS2D,
      MEM_PSC_FLOW_DEFTRUE
    };
    #define MEM_PSC_FLOW_BLOCK_ML &memPlatSpecFlowML,
  #else
    #define MEM_PSC_FLOW_BLOCK_ML
  #endif

  MEM_PSC_FLOW_BLOCK* memPlatSpecFlowArray[] = {
    MEM_PSC_FLOW_BLOCK_ML
    MEM_PSC_FLOW_BLOCK_END
  };

  /*---------------------------------------------------------------------------------------------------
  *
  *  LRDIMM CONTROL
  *
  *---------------------------------------------------------------------------------------------------
  */
  #if (OPTION_LRDIMMS == TRUE)
    #define MEM_TECH_FEATURE_LRDIMM_INIT    MemTFeatDef
  #else //#if (OPTION_LRDIMMS == FALSE)
    #define MEM_TECH_FEATURE_LRDIMM_INIT    MemTFeatDef
  #endif
  MEM_TECH_LRDIMM memLrdimmSupported = {
    MEM_TECH_LRDIMM_STRUCT_VERSION,
    MEM_TECH_FEATURE_LRDIMM_INIT
  };
#else
  /*---------------------------------------------------------------------------------------------------
   * MAIN FLOW CONTROL
   *
   *
   *---------------------------------------------------------------------------------------------------
   */
  MEM_FLOW_CFG* memFlowControlInstalled[] = {
    NULL
  };
  /*---------------------------------------------------------------------------------------------------
   * NB TRAINING FLOW CONTROL
   *
   *
   *---------------------------------------------------------------------------------------------------
   */
  OPTION_MEM_FEATURE_NB* memNTrainFlowControl[] = {    // Training flow control
    NULL
  };
  /*---------------------------------------------------------------------------------------------------
   * DEFAULT TECHNOLOGY BLOCK
   *
   *
   *---------------------------------------------------------------------------------------------------
   */
  MEM_TECH_CONSTRUCTOR* memTechInstalled[] = {    // Types of technology installed
    NULL
  };

  /*---------------------------------------------------------------------------------------------------
   * DEFAULT TECHNOLOGY MAP
   *
   *
   *---------------------------------------------------------------------------------------------------
   */
  UINT8 MemoryTechnologyMap[MAX_SOCKETS_SUPPORTED] = {0};

  /*---------------------------------------------------------------------------------------------------
   * DEFAULT MAIN FEATURE BLOCK
   *---------------------------------------------------------------------------------------------------
   */
  MEM_FEAT_BLOCK_MAIN MemFeatMain = {
    NULL
  };

  /*---------------------------------------------------------------------------------------------------
   * DEFAULT NORTHBRIDGE SUPPORT LIST
   *
   *
   *---------------------------------------------------------------------------------------------------
   */
  #if (OPTION_MEMCTLR_ML == TRUE)
    #undef MEM_NB_SUPPORT_ML
    #define MEM_NB_SUPPORT_ML { MEM_NB_SUPPORT_STRUCT_VERSION, NULL, NULL, NULL, MEM_FEATURE_S3_RESUME_CONSTRUCTOR_ML, MEM_IDENDIMM_ML, MEM_TRANS_SYS_ADDR_TO_CS_ML },
  #endif
  /*---------------------------------------------------------------------------------------------------
   * DEFAULT Technology Training
   *
   *
   *---------------------------------------------------------------------------------------------------
   */
  #if OPTION_DDR2
    MEM_TECH_FEAT_BLOCK  memTechTrainingFeatDDR2 = {
      NULL
    };
    MEM_FEAT_TRAIN_SEQ memTrainSequenceDDR2[] = {
      NULL
    };
  #endif
  #if OPTION_DDR3
    MEM_TECH_FEAT_BLOCK  memTechTrainingFeatDDR3 = {
      NULL
    };
    MEM_FEAT_TRAIN_SEQ memTrainSequenceDDR3[] = {
      NULL
    };
  #endif
    /*---------------------------------------------------------------------------------------------------
     * DEFAULT Platform Specific list
     *
     *
     *---------------------------------------------------------------------------------------------------
     */
  /*----------------------------------------------------------------------
   * DEFAULT PSCFG DEFINITIONS
   *
   *----------------------------------------------------------------------
   */
  MEM_PLATFORM_CFG* memPlatformTypeInstalled[] = {
    NULL
  };

  /*----------------------------------------------------------------------
   * EXTRACTABLE PLATFORM SPECIFIC CONFIGURATION
   *
   *----------------------------------------------------------------------
   */
  MEM_PSC_FLOW_BLOCK* memPlatSpecFlowArray[] = {
    NULL
  };

  MEM_TECH_LRDIMM memLrdimmSupported = {
    MEM_TECH_LRDIMM_STRUCT_VERSION,
    NULL
  };
#endif

/*---------------------------------------------------------------------------------------------------
 * NORTHBRIDGE SUPPORT LIST
 *
 *
 *---------------------------------------------------------------------------------------------------
 */
MEM_NB_SUPPORT memNBInstalled[] = {
  MEM_NB_SUPPORT_ML
  MEM_NB_SUPPORT_END
};

UINT8 SizeOfNBInstalledTable = sizeof (memNBInstalled) / sizeof (memNBInstalled[0]);


#endif  // _OPTION_MEMORY_INSTALL_H_
