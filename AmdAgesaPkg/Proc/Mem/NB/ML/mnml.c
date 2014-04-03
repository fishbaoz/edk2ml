/* $NoKeywords:$ */
/**
 * @file
 *
 * mnml.c
 *
 * Common Northbridge  functions for ML
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/ML)
 * @e \$Revision: 283087 $ @e \$Date: 2014-01-20 21:05:41 -0600 (Mon, 20 Jan 2014) $
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
#include "AdvancedApi.h"
#include "amdlib.h"
#include "Ids.h"
#include "OptionMemory.h"
#include "PlatformMemoryConfiguration.h"
#include "mm.h"
#include "mn.h"
#include "mnml.h"
#include "mu.h"
#include "S3.h"
#include "cpuRegisters.h"
#include "cpuFamRegisters.h"
#include "cpuFamilyTranslation.h"
#include "heapManager.h"
#include "GeneralServices.h"
#include "IdsF16MlAllService.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_MEM_NB_ML_MNML_FILECODE
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
MemNRegAccessFenceML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemNInitCPGML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

/**
 * Array for frequency change related parameters.
 */
CONST MEM_FREQ_CHANGE_PARAM FreqChangeParamML = {0x0190, 0, 0, 0, 0, 0, 0, 0};

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

extern BUILD_OPT_CFG UserOptions;
extern PSO_ENTRY DefaultPlatformMemoryConfiguration[];
extern OPTION_MEM_FEATURE_NB* memNTrainFlowControl[];

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function initializes the northbridge block
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *MemPtr  - Pointer to the MEM_DATA_STRUCT
 *     @param[in]        *FeatPtr  - Pointer to the MEM_FEAT_BLOCK_NB
 *     @param[in]        *SharedPtr - Pointer to the MEM_SHARED_DATA
 *     @param[in]        NodeID  - UINT8 indicating node ID of the NB object.
 *
 *     @return     Boolean indicating that this is the correct memory
 *                 controller type for the node number that was passed in.
 */

BOOLEAN
MemConstructNBBlockML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   MEM_DATA_STRUCT *MemPtr,
  IN       MEM_FEAT_BLOCK_NB *FeatPtr,
  IN       MEM_SHARED_DATA *SharedPtr,
  IN       UINT8 NodeID
  )
{
  UINT8 Dct;
  UINT8 Channel;
  UINT8 SpdSocketIndex;
  UINT8 SpdChannelIndex;
  DIE_STRUCT *MCTPtr;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;

  //
  // Determine if this is the expected NB Type
  //
  GetLogicalIdOfSocket (MemPtr->DiesPerSystem->SocketId, &(MemPtr->DiesPerSystem->LogicalCpuid), &(MemPtr->StdHeader));
  if (!MemNIsIdSupportedML (&(MemPtr->DiesPerSystem->LogicalCpuid))) {
    return FALSE;
  }

  NBPtr->MemPtr = MemPtr;
  NBPtr->RefPtr = MemPtr->ParameterListPtr;
  NBPtr->SharedPtr = SharedPtr;

  MCTPtr = MemPtr->DiesPerSystem;
  NBPtr->MCTPtr = MCTPtr;
  NBPtr->MCTPtr->NodeId = NodeID;
  NBPtr->PciAddr.AddressValue = MCTPtr->PciAddr.AddressValue;
  NBPtr->VarMtrrHiMsk = GetVarMtrrHiMsk (&(MemPtr->DiesPerSystem->LogicalCpuid), &(MemPtr->StdHeader));

  //
  // Allocate buffer for DCT_STRUCTs and CH_DEF_STRUCTs
  //
  AllocHeapParams.RequestedBufferSize = MAX_DCTS_PER_NODE_ML * (
                                          sizeof (DCT_STRUCT) + (
                                            MAX_CHANNELS_PER_DCT_ML * (sizeof (CH_DEF_STRUCT) + sizeof (MEM_PS_BLOCK) + sizeof (CH_TIMING_STRUCT))
                                          )
                                        );
  AllocHeapParams.BufferHandle = GENERATE_MEM_HANDLE (ALLOC_DCT_STRUCT_HANDLE, NodeID, 0, 0);
  AllocHeapParams.Persist = HEAP_LOCAL_CACHE;
  if (HeapAllocateBuffer (&AllocHeapParams, &MemPtr->StdHeader) != AGESA_SUCCESS) {
    PutEventLog (AGESA_FATAL, MEM_ERROR_HEAP_ALLOCATE_FOR_DCT_STRUCT_AND_CH_DEF_STRUCTs, NBPtr->Node, 0, 0, 0, &MemPtr->StdHeader);
    SetMemError (AGESA_FATAL, MCTPtr);
    ASSERT(FALSE); // Could not allocate buffer for DCT_STRUCTs and CH_DEF_STRUCTs
    return FALSE;
  }

  MCTPtr->DctCount = MAX_DCTS_PER_NODE_ML;
  MCTPtr->DctData = (DCT_STRUCT *) AllocHeapParams.BufferPtr;
  AllocHeapParams.BufferPtr += MAX_DCTS_PER_NODE_ML * sizeof (DCT_STRUCT);
  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_ML; Dct++) {
    MCTPtr->DctData[Dct].Dct = Dct;
    MCTPtr->DctData[Dct].ChannelCount = MAX_CHANNELS_PER_DCT_ML;
    MCTPtr->DctData[Dct].ChData = (CH_DEF_STRUCT *) AllocHeapParams.BufferPtr;
    MCTPtr->DctData[Dct].ChData[0].Dct = Dct;
    AllocHeapParams.BufferPtr += MAX_CHANNELS_PER_DCT_ML * sizeof (CH_DEF_STRUCT);
    MCTPtr->DctData[Dct].TimingsMemPs1 = (CH_TIMING_STRUCT *) AllocHeapParams.BufferPtr;
    AllocHeapParams.BufferPtr += MAX_CHANNELS_PER_DCT_ML * sizeof (CH_TIMING_STRUCT);
  }
  NBPtr->PSBlock = (MEM_PS_BLOCK *) AllocHeapParams.BufferPtr;

  //
  // Initialize Socket List
  //
  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_ML; Dct++) {
    MemPtr->SocketList[MCTPtr->SocketId].ChannelPtr[(MCTPtr->DieId * 2) + Dct] = &(MCTPtr->DctData[Dct].ChData[0]);
    MemPtr->SocketList[MCTPtr->SocketId].TimingsPtr[(MCTPtr->DieId * 2) + Dct] = &(MCTPtr->DctData[Dct].Timings);
    MCTPtr->DctData[Dct].ChData[0].ChannelID = (MCTPtr->DieId * 2) + Dct;
  }

  MemNInitNBDataML (NBPtr);

  FeatPtr->InitCPG (NBPtr);
  FeatPtr->InitHwRxEn (NBPtr);
  FeatPtr->InitEarlySampleSupport (NBPtr);
  FeatPtr->InitRdWr2DTraining (NBPtr);
  FeatPtr->InitDllPDBypass (NBPtr);

  NBPtr->FeatPtr = FeatPtr;

  //
  // Calculate SPD Offsets per channel and assign pointers to the data.  At this point, we calculate the Node-Dct-Channel
  // centric offsets and store the pointers to the first DIMM of each channel in the Channel Definition struct for that
  // channel.  This pointer is then used later to calculate the offsets to be used for each logical dimm once the
  // dimm types(QR or not) are known. This is done in the Technology block constructor.
  //
  // Calculate the SpdSocketIndex separately from the SpdChannelIndex.
  // This will facilitate modifications due to some processors that might
  // map the DCT-CHANNEL differently.
  //
  SpdSocketIndex = GetSpdSocketIndex (NBPtr->RefPtr->PlatformMemoryConfiguration, NBPtr->MCTPtr->SocketId, &MemPtr->StdHeader);
  //
  // Traverse the Dct/Channel structures
  //
  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_ML; Dct++) {
    for (Channel = 0; Channel < MAX_CHANNELS_PER_DCT_ML; Channel++) {
      //
      // Calculate the number of Dimms on this channel using the
      //   die/dct/channel to Socket/channel conversion.
      //
      SpdChannelIndex = GetSpdChannelIndex (NBPtr->RefPtr->PlatformMemoryConfiguration,
                                            NBPtr->MCTPtr->SocketId,
                                            MemNGetSocketRelativeChannelNb (NBPtr, Dct, Channel),
                                            &MemPtr->StdHeader);
      NBPtr->MCTPtr->DctData[Dct].ChData[Channel].SpdPtr = &(MemPtr->SpdDataStructure[SpdSocketIndex + SpdChannelIndex]);
    }
  }

  //
  // Initialize Dct and DctCfgSel bit
  //
  MemNSetBitFieldNb (NBPtr, BFDctCfgSel, 0);
  MemNSwitchDCTNb (NBPtr, 0);

  if (MemNGetBitFieldNb (NBPtr, BFMemPstateDis) == 1) {
    // MemPstate is disabled
    NBPtr->MemPstateStage = 0;
  }

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function initializes member functions and variables of NB block.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNInitNBDataML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  NBPtr->DctCachePtr = NBPtr->DctCache;
  NBPtr->PsPtr = NBPtr->PSBlock;

  MemNInitNBRegTableML (NBPtr, NBPtr->NBRegTable);
  NBPtr->Node = ((UINT8) NBPtr->PciAddr.Address.Device) - 24;
  NBPtr->Dct = 0;
  NBPtr->Channel = 0;
  NBPtr->DctCount = MAX_DCTS_PER_NODE_ML;
  NBPtr->ChannelCount = MAX_CHANNELS_PER_DCT_ML;
  NBPtr->NodeCount = MAX_NODES_SUPPORTED_ML;
  NBPtr->Ganged = FALSE;
  NBPtr->PosTrnPattern = POS_PATTERN_256B;
  NBPtr->MemCleared = FALSE;
  NBPtr->StartupSpeed = DDR667_FREQUENCY;
  NBPtr->RcvrEnDlyLimit = 0x1FF;
  NBPtr->DefDctSelIntLvAddr = 4;
  NBPtr->NbFreqChgState = 0;
  NBPtr->FreqChangeParam = (MEM_FREQ_CHANGE_PARAM *) &FreqChangeParamML;
  NBPtr->MaxRxEnSeedTotal = 0x1FF;
  NBPtr->MinRxEnSeedGross = 0;
  NBPtr->CsRegMsk = 0x7FF8FFE0;
  NBPtr->RdDqsDlyRetrnStat = RDDQSDLY_RTN_NEEDED;
  NBPtr->MemPstate = MEMORY_PSTATE0;
  NBPtr->MemPstateStage = MEMORY_PSTATE_1ST_STAGE;
  NBPtr->CsPerChannel = MAX_CS_PER_CHANNEL_ML;
  NBPtr->CsPerDelay = 1;
  NBPtr->RdDqsDlyForMaxRdLat = 0x1F;
  NBPtr->TotalMaxVrefRange = 0x20;
  NBPtr->TotalRdDQSDlyRange = 0x40;
  NBPtr->MaxSeedCount = MAX_2D_DQS_SEED_COUNT;
  NBPtr->TotalBitTimes2DRdTraining = TOTAL_BIT_TIMES_2D_RD_TRAINING_ML;
  NBPtr->TotalBitTimes2DWrTraining = TOTAL_BIT_TIMES_2D_WR_TRAINING_ML;
  NBPtr->PhaseLaneMask = 0x3FFFF;
  NBPtr->MaxDiamondStep = 3;
  NBPtr->DiamondWidthRd = 9;
  NBPtr->DiamondWidthWr = 9;
  NBPtr->RdRolloverMultiple = 8;


  LibAmdMemFill (NBPtr->DctCache, 0, sizeof (NBPtr->DctCache), &NBPtr->MemPtr->StdHeader);
  MemNInitNBDataNb (NBPtr);

  NBPtr->SetMaxLatency = MemNSetMaxLatencyML;
  NBPtr->getMaxLatParams = MemNGetMaxLatParamsML;
  NBPtr->InitializeMCT = MemNInitializeMctML;
  NBPtr->FinalizeMCT = MemNFinalizeMctML;
  NBPtr->SendMrsCmd = MemNSendMrsCmdUnb;
  NBPtr->sendZQCmd = MemNSendZQCmdNb;
  NBPtr->WritePattern = MemNWritePatternML;
  NBPtr->ReadPattern = MemNReadPatternML;
  NBPtr->GenHwRcvEnReads = (VOID (*) (MEM_NB_BLOCK *, UINT32)) memDefRet;
  NBPtr->CompareTestPattern = MemNCompareTestPatternNb;
  NBPtr->InsDlyCompareTestPattern = MemNInsDlyCompareTestPatternNb;
  NBPtr->StitchMemory = MemNStitchMemoryNb;
  NBPtr->AutoConfig = MemNAutoConfigML;
  NBPtr->PlatformSpec = MemNPlatformSpecUnb;
  NBPtr->InitMCT = MemNInitMCTNb;
  NBPtr->DisableDCT = MemNDisableDCTUnb;
  NBPtr->StartupDCT = MemNStartupDCTUnb;
  NBPtr->SyncTargetSpeed = MemNSyncTargetSpeedNb;
  NBPtr->ChangeFrequency = MemNChangeFrequencyUnb;
  NBPtr->RampUpFrequency = MemNRampUpFrequencyUnb;
  NBPtr->ChangeNbFrequency = MemNChangeNbFrequencyUnb;
  NBPtr->ChangeNbFrequencyWrap = MemNChangeNbFrequencyWrapUnb;
  NBPtr->GetMemClkFreqInCurrentContext = MemNGetMemClkFreqInCurrentContextML;
  NBPtr->ProgramNbPsDependentRegs = MemNProgramNbPstateDependentRegistersML;
  NBPtr->ProgramCycTimings = MemNProgramCycTimingsML;
  NBPtr->SyncDctsReady = (BOOLEAN (*) (MEM_NB_BLOCK *)) memDefTrue;
  NBPtr->HtMemMapInit = MemNHtMemMapInitML;
  NBPtr->SyncAddrMapToAllNodes = (BOOLEAN (*) (MEM_NB_BLOCK *)) memDefTrue;
  NBPtr->CpuMemTyping = MemNCPUMemTypingNb;
  NBPtr->BeforeDqsTraining = MemNBeforeDQSTrainingML;
  NBPtr->AfterDqsTraining = MemNAfterDQSTrainingML;
  NBPtr->OtherTiming = MemNOtherTimingML;
  NBPtr->UMAMemTyping = MemNUMAMemTypingNb;
  NBPtr->GetSocketRelativeChannel = MemNGetSocketRelativeChannelNb;
  NBPtr->TechBlockSwitch = MemNTechBlockSwitchML;
  NBPtr->MemNCmnGetSetFieldNb = MemNCmnGetSetFieldML;
  NBPtr->SetEccSymbolSize = MemNSetEccSymbolSizeNb;
  NBPtr->TrainingFlow = MemNTrainingFlowUnb;
  NBPtr->PollBitField = MemNPollBitFieldNb;
  NBPtr->BrdcstCheck = MemNBrdcstCheckNb;
  NBPtr->BrdcstSet = MemNBrdcstSetNb;
  NBPtr->GetTrainDly = MemNGetTrainDlyNb;
  NBPtr->SetTrainDly = MemNSetTrainDlyNb;
  NBPtr->PhyFenceTraining = MemNPhyFenceTrainingUnb;
  NBPtr->GetSysAddr = MemNGetMCTSysAddrNb;
  NBPtr->RankEnabled = MemNRankEnabledNb;
  NBPtr->MemNBeforeDramInitNb = MemNBeforeDramInitML;
  NBPtr->MemNcmnGetSetTrainDly = MemNcmnGetSetTrainDlyUnb;
  NBPtr->MemPPhyFenceTrainingNb = (VOID (*) (MEM_NB_BLOCK *)) memDefRet;
  NBPtr->MemNInitPhyComp = MemNInitPhyCompML;
  NBPtr->MemNBeforePlatformSpecNb = (VOID (*) (MEM_NB_BLOCK *)) memDefRet;
  NBPtr->MemNPlatformSpecificFormFactorInitNb = MemNPlatformSpecificFormFactorInitTblDrvNb;
  NBPtr->MemNPFenceAdjustNb = MemNPFenceAdjustML;
  NBPtr->GetTrainDlyParms = MemNGetTrainDlyParmsUnb;
  NBPtr->TrainingPatternInit = MemNTrainingPatternInitNb;
  NBPtr->TrainingPatternFinalize = MemNTrainingPatternFinalizeNb;
  NBPtr->GetApproximateWriteDatDelay = MemNGetApproximateWriteDatDelayML;
  NBPtr->FlushPattern = MemNFlushPatternNb;
  NBPtr->MinDataEyeWidth = MemNMinDataEyeWidthNb;
  NBPtr->MemNCapSpeedBatteryLife = MemNCapSpeedBatteryLifeML;
  NBPtr->GetUmaSize = MemNGetUmaSizeML;
  NBPtr->GetMemClkFreqId = MemNGetMemClkFreqIdUnb;
  NBPtr->EnableSwapIntlvRgn = (VOID (*) (MEM_NB_BLOCK *, UINT32, UINT32)) memDefRet;
  NBPtr->WaitXMemClks = MemNWaitXMemClksNb;
  NBPtr->MemNGetDramTerm = MemNGetDramTermTblDrvNb;
  NBPtr->MemNGetDynDramTerm = MemNGetDynDramTermTblDrvNb;
  NBPtr->MemNGetMR0CL = MemNGetMR0CLTblDrvNb;
  NBPtr->MemNGetMR0WR = MemNGetMR0WRTblDrvNb;
  NBPtr->MemNSaveMR0 = (VOID (*) (MEM_NB_BLOCK *, UINT32)) memDefRet;
  NBPtr->MemNGetMR2CWL = MemNGetMR2CWLUnb;
  NBPtr->AllocateC6Storage = MemNAllocateC6StorageML;
  NBPtr->MemS3PspDetect = MemS3PspDetectionML;
  NBPtr->MemRunningOnPsp = MemRunningOnPspML;
  NBPtr->MemNBeforePlatformSpecNb = MemNBeforePlatformSpecML;
  NBPtr->AgressorContinuousWrites = MemNAgressorContinuousWritesUnb;
  NBPtr->GetPrbs2dRdDqsSeed = (UINT32 (*) (struct _MEM_NB_BLOCK *NBPtr , UINT8 SeedCount)) memDefRet;
  NBPtr->InPhaseCompareRdDqs2DPattern = (UINT32 (*) (struct _MEM_NB_BLOCK *NBPtr, UINT8 Buffer[], UINT8 Pattern[], UINT16 ByteCount)) memDefRet;
  NBPtr->Phase180CompareRdDqs2DPattern = (UINT32 (*) (struct _MEM_NB_BLOCK *NBPtr, UINT8 Buffer[], UINT8 Pattern[], UINT16 ByteCount)) memDefRet;
  NBPtr->InitializeRdDqs2dVictimContinuousWrites = (VOID (*) (MEM_NB_BLOCK *)) memDefRet;
  NBPtr->FinalizeRdDqs2dVictimContinuousWrites = (VOID (*) (MEM_NB_BLOCK *)) memDefRet;
  NBPtr->InitializeRdDqs2dVictimChipSelContinuousWrites = (VOID (*) (MEM_NB_BLOCK *)) memDefRet;
  NBPtr->StartRdDqs2dVictimContinuousWrites = (VOID (*) (struct _MEM_NB_BLOCK *NBPtr , UINT8 SeedCount)) memDefRet;
  NBPtr->MemN2DRdDQSDataCollection = (BOOLEAN (*) (MEM_NB_BLOCK *)) memDefRet;
  NBPtr->MemNGetMemoryWidth = MemNGetMemoryWidthUnb;
  NBPtr->MemNDisableScrubber = MemNDisableScrubberML;
  NBPtr->MemNRestoreScrubber = MemNRestoreScrubberML;
  NBPtr->MemNSetEccExclusionRange = MemNSetEccExclusionRangeUnb;
  NBPtr->MemNDisableMemClk = MemNDisableMemClkUnb;
  NBPtr->MemNTristateUnusedChipSelects = MemNTristateUnusedChipSelectsNb;

  NBPtr->IsSupported[SetDllShutDown] = TRUE;
  NBPtr->IsSupported[CheckEccDLLPwrDnConfig] = TRUE;
  NBPtr->IsSupported[CheckMaxDramRate] = TRUE;
  NBPtr->IsSupported[CheckPhyFenceTraining] = TRUE;
  NBPtr->IsSupported[CheckSendAllMRCmds] = TRUE;
  NBPtr->IsSupported[CheckFindPSOverideWithSocket] = TRUE;
  NBPtr->IsSupported[FenceTrnBeforeDramInit] = TRUE;
  NBPtr->IsSupported[UnifiedNbFence] = TRUE;
  NBPtr->IsSupported[CheckODTControls] = TRUE;
  NBPtr->IsSupported[EccByteTraining] = TRUE;
  NBPtr->IsSupported[CheckDramTerm] = TRUE;
  NBPtr->IsSupported[CheckDramTermDyn] = TRUE;
  NBPtr->IsSupported[CheckQoff] = TRUE;
  NBPtr->IsSupported[CheckDrvImpCtrl] = TRUE;
  NBPtr->IsSupported[CheckSetSameDctODTsEn] = TRUE;
  NBPtr->IsSupported[WLSeedAdjust] = TRUE;
  NBPtr->IsSupported[WLNegativeDelay] = TRUE;
  NBPtr->IsSupported[TwoStageDramInit] = TRUE;
  NBPtr->IsSupported[ForceEnMemHoleRemapping] = TRUE;
  NBPtr->IsSupported[ProgramCsrComparator] = TRUE;
  NBPtr->IsSupported[AdjustTrp] = TRUE; // erratum 638
  NBPtr->IsSupported[ForcePhyToM0] = TRUE;
  NBPtr->IsSupported[SwitchRdDqsDlyForMaxRdLatency] = TRUE;
  NBPtr->IsSupported[OptimizedPatternWrite2D] = TRUE;
  NBPtr->IsSupported[SelectMotherboardLayer] = TRUE;
  NBPtr->IsSupported[SelectMotherboardPower] = TRUE;

  NBPtr->FamilySpecificHook[ExitPhyAssistedTraining] = MemNExitPhyAssistedTrainingML;
  NBPtr->FamilySpecificHook[DCTSelectSwitch] = MemNDctCfgSelectUnb;
  NBPtr->FamilySpecificHook[AfterSaveRestore] = MemNAfterSaveRestoreUnb;
  NBPtr->FamilySpecificHook[OverrideRcvEnSeed] = MemNOverrideRcvEnSeedML;
  NBPtr->FamilySpecificHook[OverrideWLSeed] = MemNOverrideWLSeedML;
  NBPtr->FamilySpecificHook[CalcWrDqDqsEarly] = MemNCalcWrDqDqsEarlyML;
  NBPtr->FamilySpecificHook[AdjustRdDqsDlyOffset] = MemNAdjustRdDqsDlyOffsetUnb;
  NBPtr->FamilySpecificHook[GetDdrMaxRate] = MemNGetMaxDdrRateUnb;
  NBPtr->FamilySpecificHook[AfterMemClkFreqVal] = MemNAdjustPllLockTimeML;
  NBPtr->FamilySpecificHook[AdjustCSIntLvLowAddr] = MemNCSIntLvLowAddrAdjML;
  NBPtr->FamilySpecificHook[ReleaseNbPstate] = MemNReleaseNbPstateML;
  NBPtr->FamilySpecificHook[InitializeRxEnSeedlessTraining] = MemNInitializeRxEnSeedlessTrainingUnb;
  NBPtr->FamilySpecificHook[TrackRxEnSeedlessRdWrNoWindBLError] = MemNTrackRxEnSeedlessRdWrNoWindBLErrorUnb;
  NBPtr->FamilySpecificHook[TrackRxEnSeedlessRdWrSmallWindBLError] = MemNTrackRxEnSeedlessRdWrSmallWindBLErrorUnb;
  NBPtr->FamilySpecificHook[InitialzeRxEnSeedlessByteLaneError] = MemNInitialzeRxEnSeedlessByteLaneErrorUnb;
  NBPtr->FamilySpecificHook[MemPstateStageChange] = MemNMemPstateStageChangeML;
  NBPtr->FamilySpecificHook[ProgramFence2RxDll] = MemNProgramFence2RxDllML;
  NBPtr->FamilySpecificHook[RdDqsDlyRestartChk] = MemNRdDqsDlyRestartChkML;
  NBPtr->FamilySpecificHook[BeforeWrDatTrn] = MemNHookBfWrDatTrnML;
  NBPtr->FamilySpecificHook[RegAccessFence] = MemNRegAccessFenceML;
  NBPtr->FamilySpecificHook[AdjustWrDqsBeforeSeedScaling] = MemNAdjustWrDqsBeforeSeedScalingUnb;
  NBPtr->FamilySpecificHook[WLMR1] = MemNWLMR1ML;
  NBPtr->FamilySpecificHook[Adjust2DPhaseMaskBasedOnEcc] = MemNAdjust2DPhaseMaskBasedOnEccUnb;
  NBPtr->FamilySpecificHook[ProgramPOdtOff] = MemNProgramPOdtOffML;
  NBPtr->FamilySpecificHook[AdjustHwRcvEnSeedGross] = MemNAdjustHwRcvEnSeedGrossML;
  NBPtr->FamilySpecificHook[ForceEccSymbolSize] = MemNForceEccSymbolSizeML;
  NBPtr->FamilySpecificHook[AdjustRdDqsDlyForMaxRdLat] = MemNAdjustRdDqsDlyForMaxRdLatUnb;
  NBPtr->FamilySpecificHook[SetMaxRdLatBasedOnSeededRxEnDly] = MemNSetMaxRdLatBasedOnSeededRxEnDlyML;
  NBPtr->FamilySpecificHook[DisableMemHoleMapping] = MemNDisableMemHoleMappingML;
  NBPtr->FamilySpecificHook[RestoreMemHoleMapping] = MemNRestoreMemHoleMappingML;
  NBPtr->FamilySpecificHook[PhyInitVref] = MemNPhyInitVrefML;
  NBPtr->FamilySpecificHook[IntermediateMemclkFreqVal] = MemNIntermediateMemclkFreqValUnb;
  NBPtr->FamilySpecificHook[EnableLowPowerRcvr] = MemNEnableLowPowerRcvrML;

  IEM_INSERT_CODE (IEM_NBBLOCK_INIT, IemInitNBDataOverrideML, (NBPtr));
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function initializes the default values in the MEM_DATA_STRUCT
 *
 *   NOTE: This function must first verify the NB Family.
 *
 *     @param[in,out]   *MemPtr  - Pointer to the MEM_DATA_STRUCT
 *
 */
VOID
MemNInitDefaultsML (
  IN OUT   MEM_DATA_STRUCT *MemPtr
  )
{
  MEM_PARAMETER_STRUCT *RefPtr;
  CPU_LOGICAL_ID LogicalCpuid;
  ASSERT (MemPtr != NULL);
  //
  // Determine if this is the expected NB Type
  //
  GetLogicalIdOfCurrentCore (&LogicalCpuid, &(MemPtr->StdHeader));
  if (MemNIsIdSupportedML (&LogicalCpuid)) {
    RefPtr = MemPtr->ParameterListPtr;
    RefPtr->EnableNodeIntlv = FALSE;
    RefPtr->EnableChannelIntlv = FALSE;
    RefPtr->EnableParity = FALSE;
    RefPtr->EnableOnLineSpareCtl = FALSE;
  }
}

/*-----------------------------------------------------------------------------*/
/**
 *
 *      This function writes training pattern
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in] Pattern[] - Pattern to write
 *     @param[in] Address - System Address [47:16]
 *     @param[in] ClCount - Number of cache lines
 *
 */

VOID
MemNWritePatternML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT32 Address,
  IN       UINT8 Pattern[],
  IN       UINT16 ClCount
  )
{
  Address = MemUSetUpperFSbase (Address, NBPtr->MemPtr);
  MemUWriteCachelines (Address, Pattern, ClCount);
}

/*-----------------------------------------------------------------------------*/
/**
 *
 *      This function reads training pattern
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in] Buffer[] - Buffer to fill
 *     @param[in] Address - System Address [47:16]
 *     @param[in] ClCount - Number of cache lines
 *
 */

VOID
MemNReadPatternML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 Buffer[],
  IN       UINT32 Address,
  IN       UINT16 ClCount
  )
{
  Address = MemUSetUpperFSbase (Address, NBPtr->MemPtr);
  MemUReadCachelines (Buffer, Address, ClCount);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function initiates DQS training for ML
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

BOOLEAN
memNEnableTrainSequenceML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  BOOLEAN Retval;
  Retval = TRUE;
  if (!MemNIsIdSupportedML (&(NBPtr->MemPtr->DiesPerSystem[NBPtr->MCTPtr->NodeId].LogicalCpuid))) {
    Retval = FALSE;
  }
  return Retval;
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function makes sure that previous phy register writes are done.
 *
 *     @param[in,out]  *NBPtr     - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]  OptParam   - Optional parameter
 *
 *     @return    TRUE
 *
 */

BOOLEAN
STATIC
MemNRegAccessFenceML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  )
{
  // If subsequent writes to this array are scheduled, such as when writing several byte lanes during dram
  // training, then it is recommended to issue a dummy register read to ensure the last write.
  NBPtr->GetTrainDly (NBPtr, AccessRcvEnDly, DIMM_BYTE_ACCESS (0, 0));

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function assigns read/write function pointers to CPG read/write modules.
 *
 *     @param[in,out] NBPtr  - Pointer to the MEM_NB_BLOCK
 *
 */
BOOLEAN
MemNInitCPGML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  NBPtr->WritePattern = MemNContWritePatternUnb;
  NBPtr->ReadPattern = MemNContReadPatternUnb;
  NBPtr->GenHwRcvEnReads = MemNGenHwRcvEnReadsUnb;
  NBPtr->FlushPattern = (VOID (*) (MEM_NB_BLOCK *, UINT32, UINT16)) memDefRet;
  NBPtr->TrainingPatternInit = (AGESA_STATUS (*) (MEM_NB_BLOCK *)) memDefRetSuccess;
  NBPtr->TrainingPatternFinalize = (AGESA_STATUS (*) (MEM_NB_BLOCK *)) memDefRetSuccess;
  NBPtr->CompareTestPattern = MemNCompareTestPatternUnb;
  NBPtr->InsDlyCompareTestPattern = MemNInsDlyCompareTestPatternUnb;
  NBPtr->FamilySpecificHook[SetupHwTrainingEngine] = MemNSetupHwTrainingEngineML;
  NBPtr->EnableInfiniteWritePattern = MemNEnableInfiniteWritePatternUnb;
  NBPtr->DisableInfiniteWritePattern = MemNDisableInfiniteWritePatternUnb;
  NBPtr->CPGInit = 0;

  return TRUE;
}