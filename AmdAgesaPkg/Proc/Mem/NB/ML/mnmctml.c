/* $NoKeywords:$ */
/**
 * @file
 *
 * mnmctml.c
 *
 * Northbridge ML MCT supporting functions
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/ML)
 * @e \$Revision: 282272 $ @e \$Date: 2014-01-08 23:41:43 -0600 (Wed, 08 Jan 2014) $
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
#include "cpuRegisters.h"
#include "cpuFamRegisters.h"
#include "cpuFamilyTranslation.h"
#include "Gnb.h"
#include "GnbPcie.h"
#include "GnbHandleLib.h"
#include "GnbRegistersML.h"
#include "GnbRegisterAccML.h"
#include "mm.h"
#include "mn.h"
#include "OptionMemory.h"
#include "mnml.h"
#include "cpuFeatures.h"
#include "Filecode.h"
#include "mftds.h"
#include "mu.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)


#define FILECODE PROC_MEM_NB_ML_MNMCTML_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define _16MB_RJ16  0x0100
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
extern BUILD_OPT_CFG UserOptions;

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function force memory Pstate to M0
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE -  No fatal error occurs.
 *     @return          FALSE - Fatal error occurs.
 */

BOOLEAN
MemNInitializeMctML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNSetBitFieldNb (NBPtr, BFMemPsSel, 0);
  MemNSetBitFieldNb (NBPtr, BFEnSplitMctDatBuffers, 1);

  MemNBrdcstSetUnConditionalNb (NBPtr, BFPStateToAccess, 0);

  MemNForcePhyToM0ML (NBPtr);

  return (BOOLEAN) (NBPtr->MCTPtr->ErrCode < AGESA_FATAL);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function sets final values for specific registers.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE -  No fatal error occurs.
 *     @return          FALSE - Fatal error occurs.
 */

BOOLEAN
MemNFinalizeMctML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MEM_DATA_STRUCT *MemPtr;
  MEM_PARAMETER_STRUCT *RefPtr;
  DRAM_PREFETCH_MODE DramPrefetchMode;
  UINT16 Speed;
  UINT32 Value32;
  UINT8 DcqBwThrotWm1;
  UINT8 DcqBwThrotWm2;

  MemPtr = NBPtr->MemPtr;
  RefPtr = MemPtr->ParameterListPtr;
  DramPrefetchMode = MemPtr->PlatFormConfig->PlatformProfile.AdvancedPerformanceProfile.DramPrefetchMode;
  Speed = NBPtr->DCTPtr->Timings.Speed;

  //
  // F2x11C
  //
  MemNSetBitFieldNb (NBPtr, BFMctCfgHiReg, 0x0CE00F30);
  if (DramPrefetchMode == DISABLE_DRAM_PREFETCH_FOR_IO || DramPrefetchMode == DISABLE_DRAM_PREFETCHER) {
    MemNSetBitFieldNb (NBPtr, BFPrefIoDis, 1);
  }

  if (DramPrefetchMode == DISABLE_DRAM_PREFETCH_FOR_CPU || DramPrefetchMode == DISABLE_DRAM_PREFETCHER) {
    MemNSetBitFieldNb (NBPtr, BFPrefCpuDis, 1);
  }


  if (Speed == DDR667_FREQUENCY) {
    DcqBwThrotWm1 = 3;
    DcqBwThrotWm2 = 4;
  } else if (Speed == DDR800_FREQUENCY) {
    DcqBwThrotWm1 = 3;
    DcqBwThrotWm2 = 5;
  } else if (Speed == DDR1066_FREQUENCY) {
    DcqBwThrotWm1 = 4;
    DcqBwThrotWm2 = 6;
  } else if (Speed == DDR1333_FREQUENCY) {
    DcqBwThrotWm1 = 5;
    DcqBwThrotWm2 = 8;
  } else if  (Speed == DDR1600_FREQUENCY) {
    DcqBwThrotWm1 = 6;
    DcqBwThrotWm2 = 9;
  } else if  (Speed == DDR1866_FREQUENCY) {
    DcqBwThrotWm1 = 7;
    DcqBwThrotWm2 = 10;
  } else {
    DcqBwThrotWm1 = 8;
    DcqBwThrotWm2 = 12;
  }
  //
  // F2x1B0
  //
  Value32 = MemNGetBitFieldNb (NBPtr, BFExtMctCfgLoReg);
  Value32 &= 0x003FE8C0;
  Value32 |= 0x0FC01001;
  MemNSetBitFieldNb (NBPtr, BFExtMctCfgLoReg, Value32);

  //
  // F2x1B4
  //
  Value32 = MemNGetBitFieldNb (NBPtr, BFExtMctCfgHiReg);
  Value32 &= 0xFFFFFC00;
  Value32 |= ((0x00800000 | (UINT32) DcqBwThrotWm2 << 5) | (UINT32) DcqBwThrotWm1);
  MemNSetBitFieldNb (NBPtr, BFExtMctCfgHiReg, Value32);

  // Set LockDramCfg
  if (IsFeatureEnabled (C6Cstate, NBPtr->MemPtr->PlatFormConfig, &(NBPtr->MemPtr->StdHeader)) || MemS3PspDetectionML (NBPtr)) {
    IDS_SKIP_HOOK (IDS_LOCK_DRAM_CFG, NBPtr, &NBPtr->MemPtr->StdHeader) {
      MemNSetBitFieldNb (NBPtr, BFLockDramCfg, 1);
    }
  }

  return (BOOLEAN) (NBPtr->MCTPtr->ErrCode < AGESA_FATAL);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function create the HT memory map for ML
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE -  No fatal error occurs.
 *     @return          FALSE - Fatal error occurs.
 */

BOOLEAN
MemNHtMemMapInitML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 WeReMask;
  UINT8 LgcyMmioHoleEn;
  UINT32 BottomIo;
  UINT32 HoleOffset;
  UINT32 DctLimitAddr;
  UINT32 NodeSysBase;
  UINT32 NodeSysLimit;
  MEM_PARAMETER_STRUCT *RefPtr;
  DIE_STRUCT *MCTPtr;

  RefPtr = NBPtr->RefPtr;
  MCTPtr = NBPtr->MCTPtr;
  //
  // Physical addresses in this function are right adjusted by 16 bits ([47:16])
  // They are BottomIO, HoleOffset, DctLimitAddr, NodeSysBase, NodeSysLimit.
  //

  // Enforce bottom of IO be be 128MB aligned
  BottomIo = (RefPtr->BottomIo & 0xF8) << 8;

  if (MCTPtr->NodeMemSize != 0) {
    NodeSysBase = 0;
    NodeSysLimit = MCTPtr->NodeMemSize - 1;
    DctLimitAddr = MCTPtr->DctData[0].Timings.DctMemSize;
    LgcyMmioHoleEn = 0;

    if (NodeSysLimit >= BottomIo) {
      // HW Dram Remap
      MCTPtr->Status[SbHWHole] = TRUE;
      RefPtr->GStatus[GsbHWHole] = TRUE;
      MCTPtr->NodeHoleBase = BottomIo;
      RefPtr->HoleBase = BottomIo;

      HoleOffset = _4GB_RJ16 - BottomIo;

      NodeSysLimit += HoleOffset;

      if ((DctLimitAddr > 0) && (DctLimitAddr < BottomIo)) {
        HoleOffset += DctLimitAddr;
      } else {
        if (DctLimitAddr >= BottomIo) {
          DctLimitAddr += HoleOffset;
        }
        HoleOffset += NodeSysBase;
        LgcyMmioHoleEn = 1;
      }

      MemNSetBitFieldNb (NBPtr, BFDramHoleBase, BottomIo >> 8);
      MemNSetBitFieldNb (NBPtr, BFDramHoleOffset, HoleOffset >> 7);
      MemNSetBitFieldNb (NBPtr, BFDramHoleValid, 1);
      MemNSetBitFieldNb (NBPtr, BFDramMemHoistValid, 1);
    } else {
      // No Remapping.  Normal Contiguous mapping
    }
    MCTPtr->NodeSysBase = NodeSysBase;
    MCTPtr->NodeSysLimit = NodeSysLimit;
    RefPtr->SysLimit = MCTPtr->NodeSysLimit;

    WeReMask = 3;
    // Set the Dram base and set the WE and RE flags in the base.
    MemNSetBitFieldNb (NBPtr, BFDramBaseReg0, (NodeSysBase << 8) | WeReMask);
    // Set the Dram limit and set DstNode.
    MemNSetBitFieldNb (NBPtr, BFDramLimitReg0, ((NodeSysLimit << 8) & 0xFFFF0000));

    MemNSetBitFieldNb (NBPtr, BFDramBaseAddr, NodeSysBase >> (27 - 16));
    MemNSetBitFieldNb (NBPtr, BFDramLimitAddr, NodeSysLimit >> (27 - 16));

    MemNSetBitFieldNb (NBPtr, BFDramCtrlBaseReg0, 1);
    MemNSetBitFieldNb (NBPtr, BFLgcyMmioHoleEn0, LgcyMmioHoleEn);
    MemNSetBitFieldNb (NBPtr, BFDramCtrlLimitReg0, ((DctLimitAddr - 1) >> (27 - 16 - 11)) & 0xFFFFF800);
  }
  return (BOOLEAN) (NBPtr->MCTPtr->ErrCode < AGESA_FATAL);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *     Report the Uma size that is going to be allocated.
 *     Total system memory   UMASize
 *     >= 6G                 1024M
 *     >= 4G but < 6G         512M
 *     < 4G                   256M
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          Uma size [31:0] = Addr [47:16]
 */
UINT32
MemNGetUmaSizeML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT32 SysMemSize;
  UINT32 SizeOfUma;

  SysMemSize = NBPtr->RefPtr->SysLimit + 1;
  SysMemSize = (SysMemSize + 0x100) & 0xFFFFF000;   // Ignore 16MB allocated for C6 when finding UMA size

  if (SysMemSize > 0x10000) {
    //
    // If SysLimit is > 4GB then calculate the SysMemSize by Adjusting the BottomIo
    //
    SysMemSize = (SysMemSize - 0x10000) + (NBPtr->RefPtr->BottomIo << 8);
    if (SysMemSize >= 0x10000 && SysMemSize < 0x18000) {
      SizeOfUma = 512 << (20 - 16);       // If SystemMemSize is >= 4GB but < 6 GB
    } else {
      SizeOfUma = 1024 << (20 - 16);      // If SystemMemSize is >= 6 GB
    }
  } else {
    SizeOfUma = 256 << (20 - 16);         // If SystemMemSize is < 4 GB
  }

  return SizeOfUma;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function allocates 16MB of memory for C6 storage when it is requested to be enabled
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */
VOID
MemNAllocateC6StorageML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT32 SysLimit;
  UINT32 DramLimitReg;
  CPU_LOGICAL_ID LogicalCpuid;

  if (NBPtr->SharedPtr->C6Enabled || IsFeatureEnabled (C6Cstate, NBPtr->MemPtr->PlatFormConfig, &(NBPtr->MemPtr->StdHeader)) || MemS3PspDetectionML (NBPtr)) {

    SysLimit = NBPtr->RefPtr->SysLimit;

    // Calculate new SysLimit
    if (!NBPtr->SharedPtr->C6Enabled) {
      // System memory available is reduced by 16MB
      SysLimit -= _16MB_RJ16;

      NBPtr->MCTPtr->NodeSysLimit = SysLimit;
      NBPtr->RefPtr->SysLimit = SysLimit;
      NBPtr->SharedPtr->C6Enabled = TRUE;

      // Set TOPMEM and MTRRs (only need to be done once for BSC)
      MemNC6AdjustMSRs (NBPtr);
    }

    GetLogicalIdOfCurrentCore (&LogicalCpuid, &(NBPtr->MemPtr->StdHeader));
    if (((LogicalCpuid.Family & AMD_FAMILY_16_ML) != 0)
        && ((LogicalCpuid.Revision & AMD_F16_ML_GT_A0) != 0)) {
      // Set Dram Limit
      DramLimitReg = MemNGetBitFieldNb (NBPtr, BFDramLimitReg0) & 0x0000FFFF;
      MemNSetBitFieldNb (NBPtr, BFDramLimitReg0, ((SysLimit << 8) & 0xFFFF0000) | DramLimitReg);
      MemNSetBitFieldNb (NBPtr, BFDramLimitHiReg0, SysLimit >> 24);
    }

    // Set BFCC6SaveEn and LockDramCfg
    MemNSetBitFieldNb (NBPtr, BFCC6SaveEn, 1);
    IDS_SKIP_HOOK (IDS_LOCK_DRAM_CFG, NBPtr, &NBPtr->MemPtr->StdHeader) {
      MemNSetBitFieldNb (NBPtr, BFLockDramCfg, 1);
    }
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function programs Powerdown and other power saving features for ML
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE -  No fatal error occurs.
 *     @return          FALSE - Fatal error occurs.
 */

BOOLEAN
MemNPowerSavingML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;

  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_ML; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);

    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      //
      // Phy Power Saving
      //
      MemNPhyPowerSavingMPstateML (NBPtr);
      if (NBPtr->MemPstateStage == MEMORY_PSTATE_3RD_STAGE) {
        MemNChangeMemPStateContextNb (NBPtr, 1);
        MemNPhyPowerSavingMPstateML (NBPtr);
        MemFInitTableDrive (NBPtr, MTAfterSettingMemoryPstate1);
        MemNChangeMemPStateContextNb (NBPtr, 0);
      }
      //
      // Power Down Enable
      //
      if (NBPtr->RefPtr->EnablePowerDown) {
        MemNSetBitFieldNb (NBPtr, BFPowerDownEn, 1);
      }
    }
  }

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function force the ECC symbol size
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *Size    - ECC symbol size
 *
 *     @return    TRUE
 */

BOOLEAN
MemNForceEccSymbolSizeML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Size
  )
{
  *(BOOLEAN *)Size = FALSE;

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function disables scrubber temporarily for write access to
 *     DCT additional address space
 *
 *     @param[in,out]   *NBPtr            - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNDisableScrubberML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  NBPtr->DisDramScrub = NBPtr->GetBitField (NBPtr, BFDisDramScrub);
  NBPtr->SetBitField (NBPtr, BFDisDramScrub, 1);
  NBPtr->GetBitField (NBPtr, BFDisDramScrub);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function restores scrubber settings
 *
 *     @param[in,out]   *NBPtr            - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNRestoreScrubberML (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  NBPtr->SetBitField (NBPtr, BFDisDramScrub, NBPtr->DisDramScrub);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function disable memory hole mappings
 *
 *     @param[in,out]   *NBPtr             - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *OptParam          - Optional parameter
 *
 *     @return    TRUE
 */

BOOLEAN
MemNDisableMemHoleMappingML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  )
{
  UINT8 Range;
  CONST BIT_FIELD_NAME LgcyMmioHoleEnBF[] = {BFLgcyMmioHoleEn0};

  for (Range = 0; Range < sizeof (LgcyMmioHoleEnBF) / sizeof (BIT_FIELD_NAME); Range++) {
    NBPtr->LgcyMmioHoleEnMap &= ~(1 << Range);
    NBPtr->LgcyMmioHoleEnMap |= (NBPtr->GetBitField (NBPtr, LgcyMmioHoleEnBF[Range]) << Range);
    NBPtr->SetBitField (NBPtr, LgcyMmioHoleEnBF[Range], 0);
  }
  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function restore memory hole mappings
 *
 *     @param[in,out]   *NBPtr             - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *OptParam          - Optional parameter
 *
 *     @return    TRUE
 */

BOOLEAN
MemNRestoreMemHoleMappingML (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  )
{
  UINT8 Range;
  CONST BIT_FIELD_NAME LgcyMmioHoleEnBF[] = {BFLgcyMmioHoleEn0};

  for (Range = 0; Range < sizeof (LgcyMmioHoleEnBF) / sizeof (BIT_FIELD_NAME); Range++) {
    NBPtr->SetBitField (NBPtr, LgcyMmioHoleEnBF[Range], (NBPtr->LgcyMmioHoleEnMap >> Range) & 1);
  }
  return TRUE;
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
