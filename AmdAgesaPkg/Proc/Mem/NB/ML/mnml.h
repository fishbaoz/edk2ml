/* $NoKeywords:$ */
/**
 * @file
 *
 * mnml.h
 *
 * Northbridge ML
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/ML)
 * @e \$Revision: 281945 $ @e \$Date: 2014-01-03 01:12:25 -0600 (Fri, 03 Jan 2014) $
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

#ifndef _MNML_H_
#define _MNML_H_

/*----------------------------------------------------------------------------
 *   Mixed (DEFINITIONS AND MACROS / TYPEDEFS, STRUCTURES, ENUMS)
 *
 *----------------------------------------------------------------------------
 */
#define MAX_DCTS_PER_NODE_ML          1
#define MAX_CHANNELS_PER_DCT_ML       1
#define MAX_NODES_SUPPORTED_ML        1
#define MAX_CS_PER_CHANNEL_ML         4

#define DEFAULT_WR_ODT_ML 6
#define DEFAULT_RD_ODT_ML 6
#define DEFAULT_RD_ODT_TRNONDLY_ML 0

#define TOTAL_BIT_TIMES_2D_RD_TRAINING_ML      (64 * 1024)
#define TOTAL_BIT_TIMES_2D_WR_TRAINING_ML      (64 * 1024)

#define PSP_SMCIND_INDEX_DATA   0xC00C002Cul
#define PSP_STATUS_CCP_CYPH_DIS_BIT_POS  0x00000010ul
#define PSP_STATUS_CCP_CYPH_DIS_MASK  0x00000010ul
#define PCI_CONFIG_SMU_INDIRECT_INDEX   0xB8          ///<  Gnb Offset index for SMU mbox
#define PCI_CONFIG_SMU_INDIRECT_DATA    0xBC          ///<  Gnb Offset data for SMU mbox

#define SMU_CC_PSP_FUSES_STATUS         0xC0108000ul //0xC00C002Cul    ///< offset in GNB to find PSP fusing
#define         STATUS_PCPU_EN         0x02          ///< BIT1

#define STATUS_PSP_DIS         0x20          ///< BIT5
#define STATUS_PSP_SECURE      0x01          //< BiT 0
#define PSP_ALIGN_TO_4_BYTES
/*-----------------------------------------------------------------------------
 *                         DEFINITIONS AND MACROS
 *
 *-----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                         TYPEDEFS, STRUCTURES, ENUMS
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                           FUNCTIONS PROTOTYPE
 *
 *----------------------------------------------------------------------------
 */
BOOLEAN
MemConstructNBBlockML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   MEM_DATA_STRUCT *MemPtr,
  IN       MEM_FEAT_BLOCK_NB *FeatPtr,
  IN       MEM_SHARED_DATA *SharedPtr,
  IN       UINT8 NodeID
  );

VOID
MemNInitNBDataML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNInitDefaultsML (
  IN OUT   MEM_DATA_STRUCT *MemPtr
  );

BOOLEAN
MemNInitializeMctML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNAutoConfigML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNOtherTimingML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNInitPhyCompML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNWritePatternML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT32 Address,
  IN       UINT8 Pattern[],
  IN       UINT16 ClCount
  );

VOID
MemNReadPatternML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 Buffer[],
  IN       UINT32 Address,
  IN       UINT16 ClCount
  );

VOID
MemNInitNBRegTableML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   TSEFO NBRegTable[]
  );

BOOLEAN
MemNIsIdSupportedML (
  IN       CPU_LOGICAL_ID *LogicalIdPtr
  );

UINT32
MemNCmnGetSetFieldML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 IsSet,
  IN       BIT_FIELD_NAME FieldName,
  IN       UINT32 Field
  );

BOOLEAN
memNEnableTrainSequenceML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNTechBlockSwitchML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNAfterDQSTrainingML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNCapSpeedBatteryLifeML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNGetMaxLatParamsML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT16 MaxRcvEnDly,
  IN OUT   UINT16 *MinDlyPtr,
  IN OUT   UINT16 *MaxDlyPtr,
  IN OUT   UINT16 *DlyBiasPtr
  );

VOID
MemNSetMaxLatencyML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT16 MaxRcvEnDly
  );

BOOLEAN
MemNExitPhyAssistedTrainingML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemNOverrideRcvEnSeedML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *SeedPtr
  );

VOID
MemNBeforeDQSTrainingML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

UINT16
MemNGetMemClkFreqInCurrentContextML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNProgramNbPstateDependentRegistersML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNAdjustPllLockTimeML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *PllLockTime
  );

BOOLEAN
MemNOverrideWLSeedML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *SeedPtr
  );

BOOLEAN
MemNFinalizeMctML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNHtMemMapInitML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

UINT32
MemNGetUmaSizeML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNBeforeDramInitML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNCSIntLvLowAddrAdjML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *LowBit
  );

VOID
MemNAllocateC6StorageML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPFenceAdjustML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   INT16 *Value16
  );

BOOLEAN
MemNReleaseNbPstateML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemNMemPstateStageChangeML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemNProgramFence2RxDllML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Fence2Data
  );

BOOLEAN
MemNRdDqsDlyRestartChkML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Center
  );

BOOLEAN
MemNHookBfWrDatTrnML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *ChipSel
  );

VOID
MemNSetOtherTimingML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPowerDownCtlML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNBeforePlatformSpecML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNWLMR1ML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Value
  );

BOOLEAN
MemNProgramPOdtOffML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Value
  );

BOOLEAN
MemNAdjustHwRcvEnSeedGrossML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Value
  );

BOOLEAN
MemNPowerSavingML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNForceEccSymbolSizeML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Size
  );

BOOLEAN
MemNCalcWrDqDqsEarlyML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

VOID
MemNConfigureDctTrainingML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNProgramNonSeqDependentRegistersML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNConfigureDctNormalML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNSetMaxRdLatBasedOnSeededRxEnDlyML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

VOID
MemNPhyPowerSavingMPstateML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNDisableScrubberML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNRestoreScrubberML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNProgramCycTimingsML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNDisableMemHoleMappingML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemNRestoreMemHoleMappingML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemNPhyInitVrefML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemS3PspDetectionML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemPspDetectionMechanismML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemS3RunningOnPspML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemRunningOnPspML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNSetupHwTrainingEngineML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *OptParam
  );

VOID
MemNForcePhyToM0ML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNEnableLowPowerRcvrML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID* OptParam
  );
#endif  /* _MNML_H_ */
