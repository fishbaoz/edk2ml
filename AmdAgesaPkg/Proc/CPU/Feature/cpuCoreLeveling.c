/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU Core Leveling Function.
 *
 * Contains code to Level the number of core in a multi-socket system
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ******************************************************************************
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
 *----------------------------------------------------------------------------
 */


/*
 *----------------------------------------------------------------------------
 *                                MODULES USED
 *
 *----------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "AMD.h"
#include "Topology.h"
#include "cpuRegisters.h"
#include "GeneralServices.h"
#include "cpuServices.h"
#include "cpuFamilyTranslation.h"
#include "cpuFeatures.h"
#include "cpuEarlyInit.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)
#define FILECODE PROC_CPU_FEATURE_CPUCORELEVELING_FILECODE
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

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
extern CPU_FAMILY_SUPPORT_TABLE CoreLevelingFamilyServiceTable;

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
AGESA_STATUS
CoreLevelingAtEarly (
  IN       UINT64                 EntryPoint,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

/*----------------------------------------------------------------------------------------
 *                          P U B L I C     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 *  Should core leveling be enabled
 *
 * @param[in]    PlatformConfig     Contains the runtime modifiable feature input data.
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @retval       TRUE               core leveling is supported.
 * @retval       FALSE              core leveling cannot be enabled.
 *
 */
BOOLEAN
STATIC
IsCoreLevelingEnabled (
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  CORE_LEVELING_TYPE  CoreLevelMode;

  CoreLevelMode = (CORE_LEVELING_TYPE) PlatformConfig->CoreLevelingMode;
  if (CoreLevelMode != CORE_LEVEL_NONE) {
    return (TRUE);
  } else {
    return (FALSE);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Performs core leveling for the system.
 *
 * This function implements the AMD_CPU_EARLY_PARAMS.CoreLevelingMode parameter.
 * The possible modes are:
 *    -0    CORE_LEVEL_LOWEST            Level to lowest common denominator
 *    -1    CORE_LEVEL_TWO               Level to 2 cores
 *    -2    CORE_LEVEL_POWER_OF_TWO      Level to 1,2,4 or 8
 *    -3    CORE_LEVEL_NONE              Do no leveling
 *    -4    CORE_LEVEL_COMPUTE_UNIT      Level cores to one core per compute unit
 *
 * @param[in]  EntryPoint        Timepoint designator.
 * @param[in]  PlatformConfig    Contains the leveling mode parameter
 * @param[in]  StdHeader         Config handle for library and services
 *
 * @return       The most severe status of any family specific service.
 *
 */
AGESA_STATUS
CoreLevelingAtEarly (
  IN       UINT64                 EntryPoint,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32    CoreNumPerComputeUnit;
  UINT32    EnabledComputeUnit;
  UINT32    SocketAndModule;
  UINT32    LowCore;
  UINT32    HighCore;
  UINT32    LeveledCores;
  UINT32    RequestedCores;
  UINT32    TotalEnabledCoresOnNode;
  BOOLEAN   RegUpdated;
  CORE_LEVELING_TYPE  CoreLevelMode;
  CPU_CORE_LEVELING_FAMILY_SERVICES  *FamilySpecificServices;
  WARM_RESET_REQUEST Request;

  IDS_HDT_CONSOLE (CPU_TRACE, "CoreLevelingAtEarly\n  CoreLevelMode: %d\n", PlatformConfig->CoreLevelingMode);

  LeveledCores = 0;
  SocketAndModule = 0;

  ASSERT (PlatformConfig->CoreLevelingMode < CoreLevelModeMax);

  // Get OEM IO core level mode
  CoreLevelMode = (CORE_LEVELING_TYPE) PlatformConfig->CoreLevelingMode;

  // Collect cpu core info
  GetGivenModuleCoreRange (0, 0, &LowCore, &HighCore, StdHeader);
  // Get the highest and lowest core count in all nodes
  TotalEnabledCoresOnNode = HighCore - LowCore + 1;
  switch (GetComputeUnitMapping (StdHeader)) {
  case AllCoresMapping:
    // All cores are in their own compute unit.
    CoreNumPerComputeUnit = 1;
    EnabledComputeUnit = TotalEnabledCoresOnNode;
    break;
  case EvenCoresMapping:
    // Cores are paired in compute units.
    CoreNumPerComputeUnit = 2;
    EnabledComputeUnit = (TotalEnabledCoresOnNode / 2);
    break;
  case TripleCoresMapping:
    // Three cores are grouped in compute units.
    CoreNumPerComputeUnit = 3;
    EnabledComputeUnit = (TotalEnabledCoresOnNode / 3);
    break;
  case QuadCoresMapping:
    // Four cores are grouped in compute units.
    CoreNumPerComputeUnit = 4;
    EnabledComputeUnit = (TotalEnabledCoresOnNode / 4);
    break;
  default:
    CoreNumPerComputeUnit = 1;
    EnabledComputeUnit = TotalEnabledCoresOnNode;
    ASSERT (FALSE);
  }

  IDS_HDT_CONSOLE (CPU_TRACE, "  TotalEnabledCoresOnNode %d EnabledComputeUnit %d\n", \
                                TotalEnabledCoresOnNode, EnabledComputeUnit);
  // Get LeveledCores
  switch (CoreLevelMode) {
  case CORE_LEVEL_LOWEST:
    return (AGESA_SUCCESS);
    break;
  case CORE_LEVEL_TWO:
    LeveledCores = 2;
    LeveledCores = (LeveledCores <= TotalEnabledCoresOnNode) ? LeveledCores : TotalEnabledCoresOnNode;
    if (LeveledCores != 2) {
      PutEventLog (
      AGESA_WARNING,
      CPU_WARNING_ADJUSTED_LEVELING_MODE,
      2, LeveledCores, 0, 0, StdHeader
      );
    }
    break;
  case CORE_LEVEL_POWER_OF_TWO:
    // Level to power of 2 (1, 2, 4, 8...)
    LeveledCores = 1;
    while (TotalEnabledCoresOnNode >= (LeveledCores * 2)) {
      LeveledCores = LeveledCores * 2;
    }
    break;
  case CORE_LEVEL_COMPUTE_UNIT:
  case CORE_LEVEL_COMPUTE_UNIT_TWO:
  case CORE_LEVEL_COMPUTE_UNIT_THREE:
    // Level cores to 1~3 core(s) per compute unit, with additional reduction to level
    // all processors to match the processor with the minimum number of cores.
    if (CoreNumPerComputeUnit == 1) {
      // If there is one core per compute unit, this is the same as CORE_LEVEL_LOWEST.
      return (AGESA_SUCCESS);
    } else {
      // If there are more than one core per compute unit, level to the number of compute units * cores per compute unit.
      LeveledCores = EnabledComputeUnit * (CoreLevelMode - CORE_LEVEL_COMPUTE_UNIT + 1);
    }
    break;
  case CORE_LEVEL_ONE:
    LeveledCores = 1;
    break;
  case CORE_LEVEL_THREE:
  case CORE_LEVEL_FOUR:
  case CORE_LEVEL_FIVE:
  case CORE_LEVEL_SIX:
  case CORE_LEVEL_SEVEN:
  case CORE_LEVEL_EIGHT:
  case CORE_LEVEL_NINE:
  case CORE_LEVEL_TEN:
  case CORE_LEVEL_ELEVEN:
  case CORE_LEVEL_TWELVE:
  case CORE_LEVEL_THIRTEEN:
  case CORE_LEVEL_FOURTEEN:
  case CORE_LEVEL_FIFTEEN:
    // Processors with compute units disable all cores in an entire compute unit at a time
    // For example, on a processor with two cores per compute unit, the effective
    // explicit levels are CORE_LEVEL_ONE, CORE_LEVEL_TWO, CORE_LEVEL_FOUR, CORE_LEVEL_SIX, and
    // CORE_LEVEL_EIGHT.
    RequestedCores = CoreLevelMode - CORE_LEVEL_THREE + 3;
    LeveledCores = RequestedCores;
    LeveledCores = (LeveledCores / CoreNumPerComputeUnit) * CoreNumPerComputeUnit;
    LeveledCores = (LeveledCores <= TotalEnabledCoresOnNode) ? LeveledCores : TotalEnabledCoresOnNode;
    if (LeveledCores != 1) {
      LeveledCores = (LeveledCores / CoreNumPerComputeUnit) * CoreNumPerComputeUnit;
    }
    if (LeveledCores != RequestedCores) {
      PutEventLog (
      AGESA_WARNING,
      CPU_WARNING_ADJUSTED_LEVELING_MODE,
      RequestedCores, LeveledCores, 0, 0, StdHeader
      );
    }
    break;
  default:
    ASSERT (FALSE);
  }

  // Set down core register
  GetFeatureServicesOfSocket (&CoreLevelingFamilyServiceTable, 0, (CONST VOID **)&FamilySpecificServices, StdHeader);
  if (FamilySpecificServices != NULL) {
    IDS_HDT_CONSOLE (CPU_TRACE, "  SetDownCoreRegister: LeveledCores %d CoreLevelMode %d\n", LeveledCores, CoreLevelMode);
    RegUpdated = FamilySpecificServices->SetDownCoreRegister (FamilySpecificServices, &SocketAndModule, &SocketAndModule, &LeveledCores, CoreLevelMode, StdHeader);
    // If the down core register is updated, trigger a warm reset.
    if (RegUpdated) {
      GetWarmResetFlag (StdHeader, &Request);
      Request.RequestBit = TRUE;
      Request.StateBits = Request.PostStage - 1;
      IDS_HDT_CONSOLE (CPU_TRACE, "  Request a warm reset.\n");
      SetWarmResetFlag (StdHeader, &Request);
    }
  }

  return (AGESA_SUCCESS);
}


CONST CPU_FEATURE_DESCRIPTOR ROMDATA CpuFeatureCoreLeveling =
{
  CoreLeveling,
  (CPU_FEAT_AFTER_PM_INIT),
  IsCoreLevelingEnabled,
  CoreLevelingAtEarly
};

/*----------------------------------------------------------------------------------------
 *                          L O C A L      F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

