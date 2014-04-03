/* $NoKeywords:$ */
/**
 * @file
 *
 * mnphyml.c
 *
 * Northbridge Phy support for ML
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/ML)
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
#include "Ids.h"
#include "mport.h"
#include "ma.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "mu.h"
#include "OptionMemory.h"       // need def for MEM_FEAT_BLOCK_NB
#include "mnml.h"
#include "PlatformMemoryConfiguration.h"
#include "cpuFamRegisters.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)


#define FILECODE PROC_MEM_NB_ML_MNPHYML_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define UNUSED_CLK 4

/// The structure of TxPrePN tables
typedef struct {
  UINT32 Speed;                          ///< Applied memory speed
  UINT16 TxPrePNVal[6];                  ///< Table values
} TXPREPN_STRUCT;

/// The entry of individual TxPrePN tables
typedef struct {
  UINT8 TxPrePNTblSize;                  ///< Total Table size
  CONST TXPREPN_STRUCT *TxPrePNTblPtr;   ///< Pointer to the table
} TXPREPN_ENTRY;

/// Type of an entry for processing phy init compensation for ML
typedef struct {
  BIT_FIELD_NAME IndexBitField;          ///< Bit field on which the value is decided
  BIT_FIELD_NAME StartTargetBitField;    ///< First bit field to be modified
  BIT_FIELD_NAME EndTargetBitField;      ///< Last bit field to be modified
  UINT16 ExtraValue;                     ///< Extra value needed to be written to bit field
  CONST TXPREPN_ENTRY *TxPrePN;          ///< Pointer to slew rate table
} PHY_COMP_INIT_NB;
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



/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
extern MEM_FEAT_TRAIN_SEQ memTrainSequenceDDR3[];
/* -----------------------------------------------------------------------------*/


/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function initializes the DDR phy compensation logic
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNInitPhyCompML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  //
  // Phy Predriver Calibration Codes for Data/DQS
  //
  CONST STATIC TXPREPN_STRUCT TxPrePNDataDqsV15ML[] = {
    //{TxPreP, TxPreN}[Speed][Drive Strength] at 1.5V
    {DDR667 + DDR800 + DDR1066 + DDR1333 + DDR1600 + DDR1866 + DDR2133, 0xFFF, 0x410, 0x208, 0x104}
  };
  CONST STATIC TXPREPN_STRUCT TxPrePNDataDqsV135ML[] = {
    //{TxPreP, TxPreN}[Speed][Drive Strength] at 1.35V
    {DDR667 + DDR800 + DDR1066 + DDR1333 + DDR1600 + DDR1866 + DDR2133, 0xFFF, 0xFFF, 0x820, 0x410}
  };
  CONST STATIC TXPREPN_STRUCT TxPrePNDataDqsV125ML[] = {
    //{TxPreP, TxPreN}[Speed][Drive Strength] at 1.25V
    {DDR667 + DDR800 + DDR1066 + DDR1333 + DDR1600 + DDR1866 + DDR2133, 0xFFF, 0xFFF, 0xFFF, 0xFFF}
  };
  CONST STATIC TXPREPN_ENTRY TxPrePNDataDqsML[] = {
    {GET_SIZE_OF (TxPrePNDataDqsV15ML), (TXPREPN_STRUCT *)&TxPrePNDataDqsV15ML},
    {GET_SIZE_OF (TxPrePNDataDqsV135ML), (TXPREPN_STRUCT *)&TxPrePNDataDqsV135ML},
    {GET_SIZE_OF (TxPrePNDataDqsV125ML), (TXPREPN_STRUCT *)&TxPrePNDataDqsV125ML}
  };

  //
  // Phy Predriver Calibration Codes for Cmd/Addr
  //
  CONST STATIC TXPREPN_STRUCT TxPrePNCmdAddrV15ML[] = {
    //{TxPreP, TxPreN}[Speed][Drive Strength] at 1.5V
    {DDR667 + DDR800 + DDR1066 + DDR1333 + DDR1600 + DDR1866 + DDR2133, 0x082, 0x041, 0x041, 0x041, 0x000, 0x0C3}
  };
  CONST STATIC TXPREPN_STRUCT TxPrePNCmdAddrV135ML[] = {
    //{TxPreP, TxPreN}[Speed][Drive Strength] at 1.35V
    {DDR667 + DDR800 + DDR1066 + DDR1333 + DDR1600 + DDR1866 + DDR2133, 0x186, 0x104, 0x0C3, 0x082, 0x000, 0x208}
  };
  CONST STATIC TXPREPN_STRUCT TxPrePNCmdAddrV125ML[] = {
    //{TxPreP, TxPreN}[Speed][Drive Strength] at 1.25V
    {DDR667 + DDR800 + DDR1066 + DDR1333 + DDR1600 + DDR1866 + DDR2133, 0x30C, 0x28A, 0x208, 0x186, 0x000, 0x410}
  };
  CONST STATIC TXPREPN_ENTRY TxPrePNCmdAddrML[] = {
    {GET_SIZE_OF (TxPrePNCmdAddrV15ML), (TXPREPN_STRUCT *)&TxPrePNCmdAddrV15ML},
    {GET_SIZE_OF (TxPrePNCmdAddrV135ML), (TXPREPN_STRUCT *)&TxPrePNCmdAddrV135ML},
    {GET_SIZE_OF (TxPrePNCmdAddrV125ML), (TXPREPN_STRUCT *)&TxPrePNCmdAddrV125ML}
  };

  //
  // Phy Predriver Calibration Codes for Clock
  //
  CONST STATIC TXPREPN_STRUCT TxPrePNClockV15ML[] = {
    //{TxPreP, TxPreN}[Speed][Drive Strength] at 1.5V
    {DDR667 + DDR800 + DDR1066 + DDR1333 + DDR1600 + DDR1866 + DDR2133, 0xFFF, 0xFFF, 0xFFF, 0x820, 0x000, 0xFFF}
  };
  CONST STATIC TXPREPN_STRUCT TxPrePNClockV135ML[] = {
    //{TxPreP, TxPreN}[Speed][Drive Strength] at 1.35V
    {DDR667 + DDR800 + DDR1066 + DDR1333 + DDR1600 + DDR1866 + DDR2133, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0x000, 0xFFF}
  };
  CONST STATIC TXPREPN_STRUCT TxPrePNClockV125ML[] = {
    //{TxPreP, TxPreN}[Speed][Drive Strength] at 1.25V
    {DDR667 + DDR800 + DDR1066 + DDR1333 + DDR1600 + DDR1866 + DDR2133, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0x000, 0xFFF}
  };
  CONST STATIC TXPREPN_ENTRY TxPrePNClockML[] = {
    {GET_SIZE_OF (TxPrePNClockV15ML), (TXPREPN_STRUCT *)&TxPrePNClockV15ML},
    {GET_SIZE_OF (TxPrePNClockV135ML), (TXPREPN_STRUCT *)&TxPrePNClockV135ML},
    {GET_SIZE_OF (TxPrePNClockV125ML), (TXPREPN_STRUCT *)&TxPrePNClockV125ML}
  };

  //
  // Tables to describe the relationship between drive strength bit fields, PreDriver Calibration bit fields and also
  // the extra value that needs to be written to specific PreDriver bit fields
  //
  CONST PHY_COMP_INIT_NB PhyCompInitBitFieldML[] = {
    // 3. Program TxPreP/TxPreN for Data and DQS according toTable 25 if VDDIO is 1.5V or Table 26 if 1.35V.
    //    A. Program D18F2x9C_x0D0F_0[F,7:0]0[A,6]_dct[1:0]={0000b, TxPreP, TxPreN}.
    //    B. Program D18F2x9C_x0D0F_0[F,7:0]02_dct[1:0]={0000b, TxPreP, TxPreN}.
    {BFDqsDrvStren, BFDataByteTxPreDriverCal2Pad1, BFDataByteTxPreDriverCal2Pad1, 0, TxPrePNDataDqsML},
    {BFDataDrvStren, BFDataByteTxPreDriverCal2Pad2, BFDataByteTxPreDriverCal2Pad2, 0, TxPrePNDataDqsML},
    {BFDataDrvStren, BFDataByteTxPreDriverCal, BFDataByteTxPreDriverCal, 8, TxPrePNDataDqsML},
    // 4. Program TxPreP/TxPreN for Cmd/Addr according to Table 28 if VDDIO is 1.5V or Table 29 if 1.35V.
    //    A. Program D18F2x9C_x0D0F_[C,8][1:0][12,0E,0A,06]_dct[1:0]={0000b, TxPreP, TxPreN}.
    //    B. Program D18F2x9C_x0D0F_[C,8][1:0]02_dct[1:0]={1000b, TxPreP, TxPreN}.
    {BFCsOdtDrvStren, BFCmdAddr0TxPreDriverCal2Pad1, BFCmdAddr0TxPreDriverCal2Pad2, 0, TxPrePNCmdAddrML},
    {BFAddrCmdDrvStren, BFCmdAddr1TxPreDriverCal2Pad1, BFAddrTxPreDriverCal2Pad4, 0, TxPrePNCmdAddrML},
    {BFCsOdtDrvStren, BFCmdAddr0TxPreDriverCalPad0, BFCmdAddr0TxPreDriverCalPad0, 8, TxPrePNCmdAddrML},
    {BFCkeDrvStren, BFAddrTxPreDriverCalPad0, BFAddrTxPreDriverCalPad0, 8, TxPrePNCmdAddrML},
    {BFAddrCmdDrvStren, BFCmdAddr1TxPreDriverCalPad0, BFCmdAddr1TxPreDriverCalPad0, 8, TxPrePNCmdAddrML},
    // 5. Program TxPreP/TxPreN for Clock according to Table 31 if VDDIO is 1.5V or Table 32 if 1.35V.
    //    A. Program D18F2x9C_x0D0F_2[2:0]02_dct[1:0]={1000b, TxPreP, TxPreN}.
    {BFClkDrvStren, BFClock0TxPreDriverCalPad0, BFClock1TxPreDriverCalPad0, 8, TxPrePNClockML}
  };

  BIT_FIELD_NAME CurrentBitField;
  UINT32 SpeedMask;
  UINT8 SizeOfTable;
  UINT8 Voltage;
  UINT8 i;
  UINT8 j;
  UINT8 k;
  UINT8 Dct;
  CONST TXPREPN_STRUCT *TblPtr;

  Dct = NBPtr->Dct;
  NBPtr->SwitchDCT (NBPtr, 0);
  // 1. Program D18F2x[1,0]9C_x0000_0008[DisAutoComp, DisablePreDriverCal] = {1b, 1b}.
  MemNSetBitFieldNb (NBPtr, BFDisablePredriverCal, 3);
  NBPtr->SwitchDCT (NBPtr, Dct);

  SpeedMask = (UINT32) 1 << (NBPtr->DCTPtr->Timings.Speed / 66);
  Voltage = (UINT8) CONVERT_VDDIO_TO_ENCODED (NBPtr->RefPtr->DDR3Voltage);

  for (j = 0; j < GET_SIZE_OF (PhyCompInitBitFieldML); j ++) {
    i = (UINT8) MemNGetBitFieldNb (NBPtr, PhyCompInitBitFieldML[j].IndexBitField);
    ASSERT (i < 4 || i == 5);
    TblPtr = (PhyCompInitBitFieldML[j].TxPrePN[Voltage]).TxPrePNTblPtr;
    SizeOfTable = (PhyCompInitBitFieldML[j].TxPrePN[Voltage]).TxPrePNTblSize;
    for (k = 0; k < SizeOfTable; k++, TblPtr++) {
      if ((TblPtr->Speed & SpeedMask) != 0) {
        for (CurrentBitField = PhyCompInitBitFieldML[j].StartTargetBitField; CurrentBitField <= PhyCompInitBitFieldML[j].EndTargetBitField; CurrentBitField ++) {
          ASSERT (TblPtr->TxPrePNVal[i] != 0);
          MemNSetBitFieldNb (NBPtr, CurrentBitField, ((PhyCompInitBitFieldML[j].ExtraValue << 12) | TblPtr->TxPrePNVal[i]));
        }
        break;
      }
    }
    // Asserting if no table is found corresponding to current memory speed.
    ASSERT (k < SizeOfTable);
  }
  NBPtr->SwitchDCT (NBPtr, 0);
  // 6. Program D18F2x9C_x0000_0008_dct[1:0]_mp[1:0][DisAutoComp] = 0.
  MemNSetBitFieldNb (NBPtr, BFDisablePredriverCal, 1);
  NBPtr->SwitchDCT (NBPtr, Dct);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This is a general purpose function that executes before DRAM training
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNBeforeDQSTrainingML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;

  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_ML; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      MemNSetBitFieldNb (NBPtr, BFTrNibbleSel, 0);
      MemNSetBitFieldNb (NBPtr, BFPerRankTimingEn, (NBPtr->IsSupported[SelectMotherboardPower] && NBPtr->RefPtr->EnableDllPDBypassMode) ? 0 : 1);
      //
      // 2.10.6.9.2 - BIOS should program D18F2x210_dct[1:0]_nbp[3:0][MaxRdLatency] to 55h.
      //
      MemNSetBitFieldNb (NBPtr, BFMaxLatency, 0x55);
      NBPtr->CsPerDelay = MemNCSPerDelayNb (NBPtr);
    }
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This is a function that executes after DRAM training for ML
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNAfterDQSTrainingML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;
  UINT8 Dimm;
  UINT8 Byte;
  UINT16 Dly;

  MemNBrdcstSetNb (NBPtr, BFMemPhyPllPdMode, 2);
  MemNBrdcstSetNb (NBPtr, BFPllLockTime, 0x190);

  //
  // Synch RdDqs2dDly to RdDqsDly for S3 Save/Restore
  //
  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_ML; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      if (!(NBPtr->DctCachePtr->Is2Dx4)) {
        // Only synch when 1D training has been performed or 2D training with x8 DIMMs
        for (Dimm = 0; Dimm < NBPtr->CsPerChannel / NBPtr->CsPerDelay; Dimm++) {
          for (Byte = 0; Byte < 9; Byte++) {
            Dly = (UINT16) MemNGetTrainDlyNb (NBPtr, AccessRdDqsDly, DIMM_BYTE_ACCESS (Dimm, Byte));
            MemNSetTrainDlyNb (NBPtr, AccessRdDqs2dDly, DIMM_NBBL_ACCESS (Dimm, Byte * 2), Dly);
            MemNSetTrainDlyNb (NBPtr, AccessRdDqs2dDly, DIMM_NBBL_ACCESS (Dimm, (Byte * 2) + 1), Dly);
            NBPtr->ChannelPtr->RdDqs2dDlys[(Dimm * MAX_NUMBER_LANES) + (Byte * 2)] = (UINT8) Dly;
            NBPtr->ChannelPtr->RdDqs2dDlys[(Dimm * MAX_NUMBER_LANES) + (Byte * 2) + 1] = (UINT8) Dly;
          }
        }
      }
    }
  }
}
/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function overrides the seed for hardware based RcvEn training of ML.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *SeedPtr - Pointer to the seed value.
 *
 *     @return    TRUE
 */

BOOLEAN
MemNOverrideRcvEnSeedML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *SeedPtr
  )
{
  UINT16 SeedVal;
  UINT8 MaxSolderedDownDimmPerCh;
  UINT8 *DimmsPerChPtr;

  MaxSolderedDownDimmPerCh = GetMaxSolderedDownDimmsPerChannel (NBPtr->RefPtr->PlatformMemoryConfiguration,
                                                                NBPtr->MCTPtr->SocketId, NBPtr->ChannelPtr->ChannelID);
  DimmsPerChPtr = FindPSOverrideEntry (NBPtr->RefPtr->PlatformMemoryConfiguration,
                                       PSO_SOLDERED_DOWN_SODIMM_TYPE,
                                       NBPtr->MCTPtr->SocketId,
                                       NBPtr->ChannelPtr->ChannelID,
                                       0, NULL, NULL);
  if (MaxSolderedDownDimmPerCh != 0 || DimmsPerChPtr != NULL) {
    // Solder-down DRAM
    SeedVal = 0x20;
  } else if (NBPtr->ChannelPtr->SODimmPresent != 0) {
    // SODIMM
    SeedVal = 0x32;
  } else {
    // Unbuffered dimm
    SeedVal = 0x32;
  }

  *(UINT16 *)SeedPtr = SeedVal - (0x20 * (UINT16) MemNGetBitFieldNb (NBPtr, BFWrDqDqsEarly));

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function choose the correct PllLockTime for ML
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *PllLockTime -  Bit offset of the field to be programmed
 *
 *     @return    TRUE
 */
BOOLEAN
MemNAdjustPllLockTimeML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *PllLockTime
  )
{
  if (MemNGetBitFieldNb (NBPtr, BFMemPhyPllPdMode) == 2) {
    *(UINT16*) PllLockTime = 0x190;
  }

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function overrides the seed for hardware based WL for ML.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *SeedPtr - Pointer to the seed value.
 *
 *     @return    TRUE
 */

BOOLEAN
MemNOverrideWLSeedML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *SeedPtr
  )
{
  UINT8 MaxSolderedDownDimmPerCh;
  UINT8 *DimmsPerChPtr;

  MaxSolderedDownDimmPerCh = GetMaxSolderedDownDimmsPerChannel (NBPtr->RefPtr->PlatformMemoryConfiguration,
                                                                NBPtr->MCTPtr->SocketId, NBPtr->ChannelPtr->ChannelID);
  DimmsPerChPtr = FindPSOverrideEntry (NBPtr->RefPtr->PlatformMemoryConfiguration,
                                       PSO_SOLDERED_DOWN_SODIMM_TYPE,
                                       NBPtr->MCTPtr->SocketId,
                                       NBPtr->ChannelPtr->ChannelID,
                                       0, NULL, NULL);
  if (MaxSolderedDownDimmPerCh != 0 || DimmsPerChPtr != NULL) {
    // Solder-down DRAM
    *(UINT8*) SeedPtr = 0xE;
  } else if (NBPtr->ChannelPtr->SODimmPresent != 0) {
    // SODIMM
    *(UINT8*) SeedPtr = 0xE;
  } else {
    // Unbuffered dimm
    *(UINT8*) SeedPtr = 0x15;
  }

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function adjusts Avg PRE value of Phy fence training for ML.
 *
 *     @param[in,out]   *NBPtr  - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *Value16 - Pointer to the value that we want to adjust
 *
 */
VOID
MemNPFenceAdjustML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   INT16 *Value16
  )
{
  // If FenceTrSel != 00b subtract Char.Temp:8
  if (MemNGetBitFieldNb (NBPtr, BFFenceTrSel) != 0) {
    *Value16 -= 8;
  }

  if (*Value16 < 0) {
    *Value16 = 0;
  }

  // This makes sure the phy fence value will be written to M1 context as well.
  MULTI_MPSTATE_COPY_TSEFO (NBPtr->NBRegTable, BFPhyFence);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function programs Fence2RxDll for ML.
 *
 *     @param[in,out]   *NBPtr  - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *Fence2Data - Pointer to the value of fence2 data
 *
 */
BOOLEAN
MemNProgramFence2RxDllML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Fence2Data
  )
{
  UINT16 Fence2RxDllTxPad;
  UINT16 Fence2Value;
  UINT16 Fence1;

  Fence2Value = (UINT16) MemNGetBitFieldNb (NBPtr, BFFence2);
  Fence2RxDllTxPad = (*(UINT16*) Fence2Data & 0x1F) | (((*(UINT16*) Fence2Data >> 5) & 0x1F) << 10);

  Fence2Value &= ~(UINT16) ((0x1F << 10) | 0x1F);
  Fence2Value |= Fence2RxDllTxPad;
  MemNSetBitFieldNb (NBPtr, BFFence2, Fence2Value);

  if (NBPtr->MemPstateStage == MEMORY_PSTATE_1ST_STAGE) {
    MAKE_TSEFO (NBPtr->NBRegTable, DCT_PHY_ACCESS, 0x0C, 30, 16, BFPhyFence);

    Fence1 = (UINT16) MemNGetBitFieldNb (NBPtr, BFPhyFence);
    MemNSetBitFieldNb (NBPtr, BFChAM1FenceSave, Fence1);
  }

  return TRUE;
}


/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function checks if RdDqsDly needs to be restarted for Kabini
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *Center - Center of the data eye
 *
 *     @return    TRUE
 */

BOOLEAN
MemNRdDqsDlyRestartChkML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Center
  )
{
  INT8 EyeCenter;
  UINT8 ByteLane;
  BOOLEAN RetVal;
  MEM_TECH_BLOCK *TechPtr;
  CH_DEF_STRUCT *ChanPtr;

  TechPtr = NBPtr->TechPtr;
  ChanPtr = NBPtr->ChannelPtr;
  ByteLane = NBPtr->TechPtr->Bytelane;
  RetVal = TRUE;

  // If the average value of passing read DQS delay for the lane is negative, then adjust the input receiver
  // DQ delay in D18F2x9C_x0D0F_0[F,7:0][5F,1F]_dct[1:0] for the lane as follows:

  EyeCenter = ((INT8) ChanPtr->RdDqsMinDlys[ByteLane] + (INT8) ChanPtr->RdDqsMaxDlys[ByteLane] + 1) / 2;

  if ((EyeCenter < 0) && (NBPtr->RdDqsDlyRetrnStat != RDDQSDLY_RTN_SUSPEND)) {
    IDS_HDT_CONSOLE (MEM_FLOW, "        Negative data eye center.\n");

    if (MemNGetBitFieldNb (NBPtr, BFLowPowerRcvrEn) != 0) {
      IDS_HDT_CONSOLE (MEM_FLOW, "                                                   ");
      IDS_HDT_CONSOLE (MEM_FLOW, "Center of data eye is negative. Just use 0 in LowPowerRcvrEn mode.\n");
      IDS_HDT_CONSOLE (MEM_FLOW, "\t\t  ");
      *(UINT8 *) Center = 0;
    } else if (MemNGetBitFieldNb (NBPtr, BFRxDqInsDly) < 3) {
      // IF (RxDqInsDly < 3) THEN increment RxDqInsDly and repeat step 3 above for all ranks and lanes
      IDS_HDT_CONSOLE (MEM_FLOW, "\n\t\tRxDqInsDly < 3, increment it and restart RdDqsDly training on current Dct.\n");

      MemNSetBitFieldNb (NBPtr, BFRxDqInsDly, MemNGetBitFieldNb (NBPtr, BFRxDqInsDly) + 1);
      NBPtr->RdDqsDlyRetrnStat = RDDQSDLY_RTN_ONGOING;

      // When Retrain condition is detected, record the current chipsel at which the retrain starts
      // so we don't need to retrain RcvEnDly and WrDatDly on the chipsels that are already done with these steps.
      if (TechPtr->RestartChipSel < ((INT8) TechPtr->ChipSel)) {
        TechPtr->RestartChipSel = (INT8) TechPtr->ChipSel;
      }

      RetVal = FALSE;
    } else {
      // ELSE program the read DQS delay for the lane with a value of zero
      IDS_HDT_CONSOLE (MEM_FLOW, "                                                   ");
      IDS_HDT_CONSOLE (MEM_FLOW, "Center of data eye is still negative after 2 retires. Do not restart training, just use 0\n");
      IDS_HDT_CONSOLE (MEM_FLOW, "\t\t  ");
      *(UINT8 *) Center = 0;
    }
  }

  return RetVal;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function executes RdDQS training
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *
 *     @return  TRUE - All bytelanes pass
 *     @return  FALSE - Some bytelanes fail
*/
BOOLEAN
MemNRdPosTrnML (
  IN OUT   MEM_TECH_BLOCK *TechPtr
  )
{
  BOOLEAN RetVal;

  if (((INT8) TechPtr->ChipSel) > TechPtr->RestartChipSel) {
    RetVal = MemTRdPosWithRxEnDlySeeds3 (TechPtr);
  } else {
    // Skip RcvEnDly cycle training when current chip select has already gone through that step.
    // Because a retrain condition can only be detected on a chip select after RcvEnDly cycle training
    // So when current chip select is equal to RestartChipSel, we don't need to redo RcvEnDly cycle training.
    // Only redo DQS position training.
    IDS_HDT_CONSOLE (MEM_FLOW, "\n\t\t\tSkip RcvEnDly Cycle Training on Current Chip Select.\n\n");
    RetVal = MemTTrainDQSEdgeDetect (TechPtr);
  }
  return RetVal;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function skips WrDatDly training when a retrain condition is just detected
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *ChipSel - Pointer to ChipSel
 *
 *     @return    TRUE
 */

BOOLEAN
MemNHookBfWrDatTrnML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *ChipSel
  )
{
  BOOLEAN RetVal;

  RetVal = TRUE;
  if (! NBPtr->FamilySpecificHook[DllPDBypassWrDatTraining] (NBPtr, NULL)) {
    // Skip WrDatDly training in DllPDBypassMode
    RetVal = FALSE;
  } else if (NBPtr->RdDqsDlyRetrnStat == RDDQSDLY_RTN_ONGOING) {
    NBPtr->RdDqsDlyRetrnStat = RDDQSDLY_RTN_NEEDED;
    // Clear chipsel value to force a restart of Rd Dqs Training
    if (NBPtr->CsPerDelay == 1) {
      *(UINT8 *) ChipSel = 0xFF;
    } else {
      *(UINT8 *) ChipSel = 0xFE;
    }

    RetVal = FALSE;
  } else if (((INT8) NBPtr->TechPtr->ChipSel) < NBPtr->TechPtr->RestartChipSel) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tSkip WrDatDly Training on Current Chip Select.\n\n");
    // Skip WrDatDly training when current chip select has gone through WrDatDly procedure
    // A retrain is detected during RdDqsDly training, so if RestartChipSel is equal to current
    // chip select, then WrDatDly has not been started for current chip select in the previous cycle.
    // However, RcvEnDly cycle training has been done for current chip select.
    // So we don't need to do RcvEnDly cycle training when current chip select is equal to RestartChipSel
    // but we need to do WrDatDly training for current chip select.
    RetVal = FALSE;
  }

  // when return is FALSE, WrDatDly training will be skipped

  return RetVal;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function sets up output driver and write leveling mode in MR1 during WL
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *Value   - MR1 value
 *
 *     @return    TRUE
 */

BOOLEAN
MemNWLMR1ML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Value
  )
{
  BOOLEAN Target;

  // For the target rank of the target DIMM, enable write leveling mode and enable the output driver.
  // For all other ranks of the target DIMM, enable write leveling mode and disable the output driver.
  Target = (BOOLEAN) (*(UINT16 *) Value >> 7) & 1;

  if (NBPtr->CsPerDelay == 1) {
    // Clear Qoff and reset it based on ML requirement
    *(UINT16 *) Value &= ~((UINT16) 1 << 12);

    if (!Target) {
      *(UINT16 *) Value |= (((UINT16) 1 << 7) | ((UINT16) 1 << 12));
    }
  }

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs POdtOff to disable/enable receiver pad termination
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *Value   - POdtOff value
 *
 *     @return    TRUE
 */

BOOLEAN
MemNProgramPOdtOffML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Value
  )
{
  MemNSetBitFieldNb (NBPtr, BFPOdtOff, (*(BOOLEAN *) Value) ? 0x1000 : 0);

  return TRUE;
}


/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function adjust the SeedGross value for hardware Receiver Enable Training
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *Value   - POdtOff value
 *
 *     @return    TRUE
 */

BOOLEAN
MemNAdjustHwRcvEnSeedGrossML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Value
  )
{
  UINT16 SeedGross;
  UINT32 MemClkSpeed;

  // Program D18F2x9C_x0000_00[2A:10]_dct[0]_mp[1:0][DqsRcvEnGrossDelay] = IF ((NBCOF / DdrRate
  // < 1) && (D18F2x200_dct[0]_mp[1:0][Tcl]=5) && (SeedGross<1) THEN 1 ELSE SeedGross ENDIF.
  MemClkSpeed = NBPtr->DCTPtr->Timings.Speed;
  SeedGross = *(UINT16 *)Value;
  if (NBPtr->NBClkFreq < (UINT32) (MemClkSpeed * 2) && MemNGetBitFieldNb (NBPtr, BFTcl) == 5 && SeedGross < 1) {
    SeedGross = 1;
  }
  *(UINT16 *)Value = SeedGross;

  return TRUE;
}

/*-----------------------------------------------------------------------------
 *
 *
 *     This function calculates the value of WrDqDqsEarly and programs it into
 *       the DCT and adds it to the WrDqsGrossDelay of each byte lane on each
 *       DIMM of the channel.
 *
 *
 *     @param[in,out]  *NBPtr     - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]  OptParam   - Optional parameter
 *
 *     @return    TRUE
 * ----------------------------------------------------------------------------
 */
BOOLEAN
MemNCalcWrDqDqsEarlyML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  )
{
  MEM_TECH_BLOCK *TechPtr;
  DCT_STRUCT *DCTPtr;
  CH_DEF_STRUCT *ChannelPtr;
  UINT8 Dimm;
  UINT8 ByteLane;
  UINT8 *WrDqsDlysPtr;
  UINT8 WrDqDqsEarly;
  UINT8 ChipSel;

  ASSERT ((NBPtr->IsSupported[WLSeedAdjust]) && (NBPtr->IsSupported[WLNegativeDelay]));

  TechPtr = NBPtr->TechPtr;
  ChannelPtr = NBPtr->ChannelPtr;
  DCTPtr = NBPtr->DCTPtr;

  ASSERT (NBPtr != NULL);
  ASSERT (ChannelPtr != NULL);
  ASSERT (DCTPtr != NULL);
  //
  // For each DIMM:
  // - The Critical Gross Delay (CGD) is the minimum GrossDly of all byte lanes and all DIMMs.
  // - If (CGD < 0) Then
  // -   D18F2xA8_dct[1:0][WrDqDqsEarly] = ABS(CGD)
  // -   WrDqsGrossDly = GrossDly + WrDqDqsEarly
  // - Else
  // -   D18F2xA8_dct[1:0][WrDqDqsEarly] = 0.
  // -   WrDqsGrossDly = GrossDly
  //
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tCalculating WrDqDqsEarly, adjusting WrDqs.\n");
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tMin. Critical Delay: %x\n", TechPtr->WLCriticalDelay);

  if (TechPtr->WLCriticalDelay < 0) {
    // We've saved the entire negative delay value, so take the ABS and convert to GrossDly.
    WrDqDqsEarly =  (UINT8) (0x00FF &((((ABS (TechPtr->WLCriticalDelay)) + 0x1F) / 0x20)));
  } else {
    WrDqDqsEarly = 0;
  }
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tWrDqDqsEarly : %02x\n\n", WrDqDqsEarly);
  //
  // Loop through All WrDqsDlys on all DIMMs
  //
  for (ChipSel = 0; ChipSel < NBPtr->CsPerChannel; ChipSel = ChipSel + NBPtr->CsPerDelay) {
    if ((NBPtr->MCTPtr->Status[SbLrdimms]) ? ((NBPtr->ChannelPtr->LrDimmPresent & ((UINT8) 1 << (ChipSel >> 1))) != 0) :
        ((DCTPtr->Timings.CsEnabled & ((UINT16) ((NBPtr->CsPerDelay == 2)? 3 : 1) << ChipSel)) != 0)) {
      //
      // If LRDIMMs, only include the physical dimms, not logical Dimms
      //
      IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tCS %x:", ChipSel);
      Dimm = ChipSel / NBPtr->CsPerDelay;

      WrDqsDlysPtr = &(ChannelPtr->WrDqsDlys[(Dimm * TechPtr->DlyTableWidth ())]);
      for (ByteLane = 0; ByteLane < (NBPtr->MCTPtr->Status[SbEccDimms] ? 9 : 8); ByteLane++) {
        WrDqsDlysPtr[ByteLane] += (WrDqDqsEarly << 5);
        NBPtr->SetTrainDly (NBPtr, AccessWrDqsDly, DIMM_BYTE_ACCESS (Dimm, ByteLane), WrDqsDlysPtr[ByteLane]);
        IDS_HDT_CONSOLE (MEM_FLOW, " %02x", WrDqsDlysPtr[ByteLane]);
      }
      IDS_HDT_CONSOLE (MEM_FLOW, "\n");
    }
  }

  MemNSetBitFieldNb (NBPtr, BFWrDqDqsEarly, WrDqDqsEarly);
  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *     This function sets phy power saving related settings in different MPstate context.
 *
 *
 *     @param[in,out]  *NBPtr     - Pointer to the MEM_NB_BLOCK
 *
 *     @return    none
 * ----------------------------------------------------------------------------
 */
VOID
MemNPhyPowerSavingMPstateML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  STATIC UINT8 RxSequence[] = {8, 4, 3, 5, 2, 6, 1, 7, 0};
  STATIC UINT8 TxSequence[] = {0, 7, 1, 6, 2, 5, 3, 4, 8};
  UINT16 DllPower[9];
  UINT8 NumLanes;
  UINT8 DllWakeTime;
  UINT8 MaxRxStggrDly;
  UINT8 MinRcvEnGrossDly;
  UINT8 MinWrDqsGrossDly;
  UINT8 dRxStggrDly;
  UINT8 dTxStggrDly;
  UINT8 TempStggrDly;
  UINT8 MaxTxStggrDly;
  UINT8 Tcl;
  UINT8 Tcwl;
  UINT8 WrDqDqsEarly;
  UINT8 i;
  UINT8 j;
  UINT16 MemClkSpeed;
  MemClkSpeed = ( (NBPtr->MemPstate == MEMORY_PSTATE0) ? NBPtr->DCTPtr->Timings.Speed : MemNGetMemClkFreqUnb (NBPtr, (UINT8) MemNGetBitFieldNb (NBPtr, BFM1MemClkFreq)) );

  // 3. Program D18F2x9C_x0D0F_0[F,8:0]30_dct[0][PwrDn] to disable the ECC lane if
  // D18F2x90_dct[0][DimmEccEn]==0.
  if (NBPtr->IsSupported[CheckEccDLLPwrDnConfig]) {
    if (!NBPtr->MCTPtr->Status[SbEccDimms]) {
      MemNSetBitFieldNb (NBPtr, BFEccDLLPwrDnConf, 0x0010);
    }
  }

  IDS_HDT_CONSOLE (MEM_FLOW, "Start Phy power saving setting for memory Pstate %d\n", NBPtr->MemPstate);
  // 4. Program D18F2x9C_x0D0F_0[F,8:0]13_dct[1:0][DllDisEarlyU] = 1b.
  // 5. Program D18F2x9C_x0D0F_0[F,8:0]13_dct[1:0][DllDisEarlyL] = 1b.
  MemNSetBitFieldNb (NBPtr, BFPhy0x0D0F0F13, MemNGetBitFieldNb (NBPtr, BFPhy0x0D0F0F13) | 0x3);
  // 7. D18F2x9C_x0D0F_812F_dct[1:0][PARTri] = ~D18F2x90_dct[1:0][ParEn].
  // 8. D18F2x9C_x0D0F_812F_dct[1:0][Add17Tri, Add16Tri] = {1b, 1b}
  if (NBPtr->MemPstate == MEMORY_PSTATE0) {
    MemNSetBitFieldNb (NBPtr, BFAddrCmdTri, MemNGetBitFieldNb (NBPtr, BFAddrCmdTri) | 0xA1);
  }
  // 9. IF (DimmsPopulated == 1)&& ((D18F2x9C_x0000_0000_dct[1:0]_mp[1:0][CkeDrvStren]==010b) ||
  // (D18F2x9C_x0000_0000_dct[1:0]_mp[1:0][CkeDrvStren]==011b)) THEN THEN
  // program D18F2x9C_x0D0F_C0[40,00]_dct[1:0][LowPowerDrvStrengthEn] = 1
  // ELSE program D18F2x9C_x0D0F_C0[40,00]_dct[1:0][LowPowerDrvStrengthEn] = 0 ENDIF.
  if ((NBPtr->ChannelPtr->Dimms == 1) && ((MemNGetBitFieldNb (NBPtr, BFCkeDrvStren) == 2) || (MemNGetBitFieldNb (NBPtr, BFCkeDrvStren) == 3))) {
    MemNSetBitFieldNb (NBPtr, BFLowPowerDrvStrengthEn, 0x100);
  }
  // 11. Program D18F2x9C_x0D0F_0[F,7:0][50,10]_dct[1:0][EnRxPadStandby] = IF
  // (D18F2x94_dct[1:0][MemClkFreq] <= 800 MHz) THEN 1 ELSE 0 ENDIF.
  MemNSetBitFieldNb (NBPtr, BFEnRxPadStandby, (MemClkSpeed <= DDR1600_FREQUENCY) ? 0x1000 : 0);
  // 12. Program D18F2x9C_x0000_000D_dct[1:0]_mp[1:0] as follows:
  // If (DDR rate < = 1600) TxMaxDurDllNoLock = RxMaxDurDllNoLock = 8h
  // else TxMaxDurDllNoLock = RxMaxDurDllNoLock = 7h.
  if (MemClkSpeed <= DDR1600_FREQUENCY) {
    MemNSetBitFieldNb (NBPtr, BFRxMaxDurDllNoLock, 8);
    MemNSetBitFieldNb (NBPtr, BFTxMaxDurDllNoLock, 8);
  } else {
    MemNSetBitFieldNb (NBPtr, BFRxMaxDurDllNoLock, 7);
    MemNSetBitFieldNb (NBPtr, BFTxMaxDurDllNoLock, 7);
  }

  // TxCPUpdPeriod = RxCPUpdPeriod = 011b.
  MemNSetBitFieldNb (NBPtr, BFRxCPUpdPeriod, 3);
  MemNSetBitFieldNb (NBPtr, BFTxCPUpdPeriod, 3);
  // TxDLLWakeupTime = RxDLLWakeupTime = 11b.
  MemNSetBitFieldNb (NBPtr, BFRxDLLWakeupTime, 3);
  MemNSetBitFieldNb (NBPtr, BFTxDLLWakeupTime, 3);

  IDS_SKIP_HOOK (IDS_DLLSTAGGERDLY_OVERRIDE, NULL, &(NBPtr->MemPtr->StdHeader)) {
    // 12. Program D18F2x9C_x0D0F_0[F,7:0][5C,1C]_dct[1:0] as follows.
    // Let Numlanes = 8. = 9 with ECC.
    NumLanes = (NBPtr->MCTPtr->Status[SbEccDimms] && NBPtr->IsSupported[EccByteTraining]) ? 9 : 8;
    // RxDllStggrEn = TxDllStggrEn = 1.
    for (i = 0; i < 9; i ++) {
      DllPower[i] = 0x8080;
    }
    // 13. If (DDR rate > = 1866) DllWakeTime = 1, Else DllWakeTime = 0.
    DllWakeTime = (MemClkSpeed >= DDR1866_FREQUENCY) ? 1 : 0;
    // Let MaxRxStggrDly = ((Tcl-1)*2) + MIN(DqsRcvEnGrossDelay for all byte lanes (see D18F2x9C_x0000_00[2A:10]_dct[1:0]_mp[1:0])) - 6.
    MinRcvEnGrossDly = NBPtr->TechPtr->GetMinMaxGrossDly (NBPtr->TechPtr, AccessRcvEnDly, FALSE);
    Tcl = (UINT8) MemNGetBitFieldNb (NBPtr, BFTcl);
    ASSERT (Tcl >= 1);
    ASSERT (((Tcl - 1) * 2 + MinRcvEnGrossDly) >= 6);
    MaxRxStggrDly = (Tcl - 1) * 2 + MinRcvEnGrossDly - 6;
    // Let (real) dRxStggrDly = (MaxRxStggrDly - DllWakeTime) / (Numlanes - 1).
    ASSERT (MaxRxStggrDly >= DllWakeTime);
    dRxStggrDly = (MaxRxStggrDly - DllWakeTime) / (NumLanes - 1);
    IDS_HDT_CONSOLE (MEM_FLOW, "\tMinimum RcvEnGrossDly: 0x%02x MaxRxStggrDly: 0x%02x dRxStggrDly: 0x%02x\n", MinRcvEnGrossDly, MaxRxStggrDly, dRxStggrDly);
    // For each byte lane in the ordered sequence {8, 4, 3, 5, 2, 6, 1, 7, 0}, program RxDllStggrDly[5:0] = an
    // increasing value, starting with 0 for the first byte lane in the sequence and increasing at a rate of dRxStggrDly
    // for each subsequent byte lane. Convert the real to integer by rounding down or using C (int) typecast after linearization.

    for (i = 9 - NumLanes, j = 0; i < 9; i ++, j ++) {
      TempStggrDly = ((MaxRxStggrDly - DllWakeTime) * j + NumLanes - 2) / (NumLanes - 1);
      DllPower[RxSequence[i]] |= ((TempStggrDly & 0x3F) << 8);
    }

    // Let MaxTxStggrDly = MIN(((Tcwl-1)*2) + MIN(WrDqsGrossDly for all byte lanes) - WrDqDqsEarly -
    // 4, MaxRxStggrDly)
    Tcwl = (UINT8) MemNGetBitFieldNb (NBPtr, BFTcwl);
    WrDqDqsEarly = (UINT8) MemNGetBitFieldNb (NBPtr, BFWrDqDqsEarly);
    MinWrDqsGrossDly = NBPtr->TechPtr->GetMinMaxGrossDly (NBPtr->TechPtr, AccessWrDqsDly, FALSE);
    ASSERT (Tcwl >= 1);
    ASSERT ((Tcwl - 1) * 2 + MinWrDqsGrossDly - WrDqDqsEarly >= 4);
    MaxTxStggrDly = MIN ((Tcwl - 1) * 2 + MinWrDqsGrossDly - WrDqDqsEarly - 4, MaxRxStggrDly);
    // Let dTxStggrDly = (MaxTxStggrDly - DllWakeTime) / (Numlanes - 1).
    ASSERT (MaxTxStggrDly >= DllWakeTime);
    dTxStggrDly = (MaxTxStggrDly - DllWakeTime) / (NumLanes - 1);
    // For each byte lane in the ordered sequence {8, 4, 3, 5, 2, 6, 1, 7, 0}, program TxDllStggrDly[5:0] = an
    // increasing integer value, starting with 0 for the first byte lane in the sequence and increasing at a rate of
    // dTxStggrDly for each subsequent byte lane.
    IDS_HDT_CONSOLE (MEM_FLOW, "\tMinimum WrDqsGrossDly: 0x%02x MaxTxStggrDly: 0x%02x dTxStggrDly: 0x%02x\n", MinWrDqsGrossDly, MaxTxStggrDly, dTxStggrDly);

    for (i = 0, j = 0; i < NumLanes; i ++, j ++) {
      TempStggrDly = ((MaxTxStggrDly - DllWakeTime) * j + NumLanes - 2) / (NumLanes - 1);
      DllPower[TxSequence[i]] |= (TempStggrDly & 0x3F);
    }

    IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\t\tByte Lane    :  ECC   07   06   05   04   03   02   01   00\n");
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\t\tDll Power    : %04x %04x %04x %04x %04x %04x %04x %04x %04x\n",
                     DllPower[8], DllPower[7], DllPower[6], DllPower[5], DllPower[4], DllPower[3], DllPower[2], DllPower[1], DllPower[0]);

    for (i = 0; i < NumLanes; i ++) {
      MemNSetBitFieldNb (NBPtr, BFDataByteDllPowerMgnByte0 + i, MemNGetBitFieldNb (NBPtr, BFDataByteDllPowerMgnByte0 + i) & 0x4040 | DllPower[i]);
    }
  }

  // 14. For M0 & M1 context program RxChMntClkEn=RxSsbMntClkEn=0.
  MemNSetBitFieldNb (NBPtr, BFRxChMntClkEn, 0);
  MemNSetBitFieldNb (NBPtr, BFRxSsbMntClkEn, 0);

  // 15. Program D18F2x9C_x0D0F_0[F,8:0]30_dct[0][TxPclkGateEn,PchgPdPClkGateEn,DataCtlPipePclkGateEn] = {1, 1, 1}
  MemNSetBitFieldNb (NBPtr, BFTxPclkGateEn, 1 << 9);
  MemNSetBitFieldNb (NBPtr, BFPchgPdPclkGateEn, 1 << 7);
  MemNSetBitFieldNb (NBPtr, BFDataCtlPipePclkGateEn, 1 << 6);

  IDS_OPTION_HOOK (IDS_PHY_DLL_STANDBY_CTRL, NBPtr, &NBPtr->MemPtr->StdHeader);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs Vref according to platform requirements
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]  OptParam   - Optional parameter
 *
 *     @return    TRUE
 */
BOOLEAN
MemNPhyInitVrefML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  )
{
  MemNBrdcstSetNb (NBPtr, BFVrefSel, (NBPtr->RefPtr->ExternalVrefCtl ? 0x0002 : 0x0001));

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function finalizes the training pattern.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       Index    - Index of Write Data Delay Value
 *     @param[in,out]   *Value   - Write Data Delay Value
 *     @return      BOOLEAN - TRUE - Use the value returned.
 *                            FALSE - No more values in table.
 */

BOOLEAN
MemNGetApproximateWriteDatDelayML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 Index,
  IN OUT   UINT8 *Value
  )
{
  CONST UINT8 BypassWriteDatDelayValue[] = {0x10};
  CONST UINT8 WriteDatDelayValue[] = {0x10, 0x4, 0x8, 0xC, 0x14, 0x18, 0x1C, 0x1F};

  if (NBPtr->IsSupported[SelectMotherboardPower] && NBPtr->RefPtr->EnableDllPDBypassMode) {
    if (Index < GET_SIZE_OF (BypassWriteDatDelayValue)) {
      *Value = BypassWriteDatDelayValue[Index];
      return TRUE;
    }
  } else {
    if (Index < GET_SIZE_OF (WriteDatDelayValue)) {
      *Value = WriteDatDelayValue[Index];
      return TRUE;
    }
  }
  return FALSE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function determined the settings for the Reliable Read/Write engine
 *       for each specific type of training
 *
 *     @param[in,out]   *NBPtr      - Pointer to the MEM_NB_BLOCK
 *     @param[in]       *OptParam   - Pointer to an Enum of TRAINING_TYPE
 *
 *     @return      TRUE
 */

BOOLEAN
MemNSetupHwTrainingEngineML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *OptParam
  )
{
  TRAINING_TYPE TrnType;
  RRW_SETTINGS *Rrw;

  TrnType = *(TRAINING_TYPE*) OptParam;
  Rrw = &NBPtr->RrwSettings;
  //
  // Common Settings
  //
  Rrw->TgtBankAddressA = CPG_BANK_ADDRESS_A;
  Rrw->TgtRowAddressA = CPG_ROW_ADDRESS_A;
  Rrw->TgtColAddressA = CPG_COL_ADDRESS_A;
  Rrw->TgtBankAddressB = CPG_BANK_ADDRESS_B;
  Rrw->TgtRowAddressB = CPG_ROW_ADDRESS_B;
  Rrw->TgtColAddressB = CPG_COL_ADDRESS_B;
  Rrw->CompareMaskHigh = CPG_COMPARE_MASK_HI;
  Rrw->CompareMaskLow = CPG_COMPARE_MASK_LOW;
  Rrw->CompareMaskEcc = CPG_COMPARE_MASK_ECC;

  switch (TrnType) {
  case TRN_RCVR_ENABLE:
    //
    // Receiver Enable Training
    //
    NBPtr->TechPtr->PatternLength = 192;
    break;
  case TRN_MAX_READ_LATENCY:
    //
    // Max Read Latency Training
    //
    Rrw->CmdTgt = CMD_TGT_A;
    NBPtr->TechPtr->PatternLength = 256;
    Rrw->DataPrbsSeed = PRBS_SEED_256;
    break;
  case TRN_DQS_POSITION:
    //
    // Read/Write DQS Position training
    //
    Rrw->CmdTgt = CMD_TGT_AB;
    NBPtr->TechPtr->PatternLength = 256;
    Rrw->DataPrbsSeed = PRBS_SEED_256;
    break;
  default:
    ASSERT (FALSE);
  }
  return TRUE;
}

/*-----------------------------------------------------------------------------
 *
 *
 *     This function forces phy to M0 state
 *
 *     @param[in,out]  *NBPtr     - Pointer to the MEM_NB_BLOCK
 *
 *     @return  none
 * ----------------------------------------------------------------------------
 */
VOID
MemNForcePhyToM0ML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  IDS_SKIP_HOOK (IDS_FORCE_PHY_TO_M0, NBPtr, &(NBPtr->MemPtr->StdHeader)) {
    // 1. Program D18F2x9C_x0D0F_E013_dct[1:0] = 0118h.
    MemNBrdcstSetUnConditionalNb (NBPtr, BFPllRegWaitTime, 0x118);
    // 2. For each byte lane and each memory P-state: Program D18F2x9C_x0D0F_0[F,7:0][53,13]_dct[1:0][RxSsbMntClkEn] = 0.
    MemNBrdcstSetUnConditionalNb (NBPtr, BFRxSsbMntClkEn, 0);
    // IF (DdrRate >= 1866)
    // D18F2x9C_x0D0F_0[F,8:0]38_dct[0][ReducedLoop] = 10b ENDIF
    if (NBPtr->DCTPtr->Timings.TargetSpeed >= DDR1866_FREQUENCY) {
        MemNSetBitFieldNb (NBPtr, BFReducedLoop, (2 << 13));
    }
    NBPtr->FamilySpecificHook[DllPDBypassClkAddrCmd] (NBPtr, NULL);
    NBPtr->FamilySpecificHook[EnableLowPowerRcvr] (NBPtr, NULL);
    // 3. D18F2xA8_dct[1:0][MemPhyPllPdMode] = 00b.
    MemNBrdcstSetUnConditionalNb (NBPtr, BFMemPhyPllPdMode, 0);
    // 4. Force the phy to M0 with the following sequence:
    // A. Program D18F2x9C_x0D0F_E006_dct[1:0][PllLockTime] = 190h. Restore the default PLL lock time.
    MemNBrdcstSetUnConditionalNb (NBPtr, BFPllLockTime, NBPtr->FreqChangeParam->PllLockTimeDefault);
    // B. For each DCT: Program D18F2x9C_x0000_000B_dct[1:0] = 80800000h.
    MemNBrdcstSetUnConditionalNb (NBPtr, BFDramPhyStatusReg, 0x80800000);
    NBPtr->SwitchDCT (NBPtr, 0);
    // C. Program D18F2x9C_x0D0F_E018_dct[0][PhyPSMasterChannel] = 0.
    MemNSetBitFieldNb (NBPtr, BFPhyPSMasterChannel, 0);
    // D. Program D18F2x9C_x0000_000B_dct[0] = 40000000h. CH0 only;
    MemNSetBitFieldNb (NBPtr, BFDramPhyStatusReg, 0x40000000);
    // E. For each DCT: Program D18F2x9C_x0000_000B_dct[1:0] = 80000000h.
    MemNBrdcstSetUnConditionalNb (NBPtr, BFDramPhyStatusReg, 0x80000000);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function enables low power Rcvr
 *
 *     @param[in,out] *NBPtr    - Pointer to the MEM_NB_BLOCK
 *     @param[in,out] *OptParam - Unused
 *
 *     @return  TRUE - LowPowerRcvr not enabled
 *     @return  FALSE - LowPowerRcvr enabled
*/
BOOLEAN
MemNEnableLowPowerRcvrML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID* OptParam
  )
{
  UINT8 MaxDimmPerCh;
  UINT8 MaxDimmSlotPerCh;
  UINT8 *DimmsPerChPtr;
  UINT32 LowPowerRcvrEn;

  MaxDimmPerCh = GetMaxDimmsPerChannel (NBPtr->RefPtr->PlatformMemoryConfiguration, NBPtr->MCTPtr->SocketId, NBPtr->ChannelPtr->ChannelID);
  MaxDimmSlotPerCh = MaxDimmPerCh - GetMaxSolderedDownDimmsPerChannel (NBPtr->RefPtr->PlatformMemoryConfiguration,
                                                                       NBPtr->MCTPtr->SocketId, NBPtr->ChannelPtr->ChannelID);
  DimmsPerChPtr = FindPSOverrideEntry (NBPtr->RefPtr->PlatformMemoryConfiguration,
                                       PSO_SOLDERED_DOWN_SODIMM_TYPE,
                                       NBPtr->MCTPtr->SocketId,
                                       NBPtr->ChannelPtr->ChannelID,
                                       0, NULL, NULL);

  // D18F2x9C_x0D0F_0[F,8:0]1F_dct[#NUM_DCTS#]_mp[1:0][LowPowerRcvrEn] = ((Solder-down
  // DRAM && DdrRate <= 1066) || DdrRate == 667).
  if ((MaxDimmSlotPerCh == 0) && (DimmsPerChPtr != NULL)) {
    LowPowerRcvrEn = NBPtr->DCTPtr->Timings.Speed <= DDR1066_FREQUENCY ? 1 : 0;
  } else {
    LowPowerRcvrEn = NBPtr->DCTPtr->Timings.Speed <= DDR667_FREQUENCY ? 1 : 0;
  }
  IDS_OPTION_HOOK (IDS_LOW_POWER_RCVR_EN_CTRL, &LowPowerRcvrEn, &NBPtr->MemPtr->StdHeader);
  MemNSetBitFieldNb (NBPtr, BFLowPowerRcvrEn, (LowPowerRcvrEn == 1) ? 0x4000 : 0);

  return TRUE;
}