/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of family 16h support
 *
 * This file generates the defaults tables for family 16h processors.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Core
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
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

#ifndef _OPTION_FAMILY_16H_INSTALL_H_
#define _OPTION_FAMILY_16H_INSTALL_H_

#include "OptionFamily16hEarlySample.h"
#include "cpuFamilyTranslation.h"


/*
 * Pull in family specific services based on entry point
 */

/*
 * Common Family 16h routines
 */

/*
 * Install family 16h model 30h - 3Fh support
 */
#ifdef OPTION_FAMILY16H_ML
  #if OPTION_FAMILY16H_ML == TRUE
    extern CONST REGISTER_TABLE ROMDATA F16MlPciRegisterTableBeforeApLaunch;
    extern CONST REGISTER_TABLE ROMDATA F16MlPciRegisterTableAfterApLaunch;
    extern CONST REGISTER_TABLE ROMDATA F16MlPciWorkaroundTable;
    extern CONST REGISTER_TABLE ROMDATA F16MlMsrRegisterTable;
    extern CONST REGISTER_TABLE ROMDATA F16MlMsrWorkaroundTable;
    extern CONST REGISTER_TABLE ROMDATA F16MlSharedMsrRegisterTable;
    extern CONST REGISTER_TABLE ROMDATA F16MlSharedMsrWorkaroundTable;

    /**
     * Compute unit and Compute unit primary determination table.
     *
     * The four fields from the compute unit status hardware register can be used to determine whether
     * even number cores are primary or all cores are primary.  It can be extended if it is
     * decided to have other configs as well.  The other logically possible value sets are BitMapMapping,
     * but they are currently not supported by the processor.
     */
    CONST COMPUTE_UNIT_MAP ROMDATA TopologyFam16MlComputeUnitMapping[] =
    {
      {1, 'x', 'x', 1, QuadCoresMapping},                            ///< 1 Compute Unit with 4 Cores
      {1, 'x',  1,  0, TripleCoresMapping},                          ///< 1 Compute Unit with 3 Cores
      {1,  1,   0,  0, EvenCoresMapping},                            ///< 1 Compute Unit with 2 cores
      {1,  0,   0,  0, AllCoresMapping},                             ///< 1 Compute Unit with 1 Cores
      {TOPOLOGY_LIST_TERMINAL, TOPOLOGY_LIST_TERMINAL, TOPOLOGY_LIST_TERMINAL, TOPOLOGY_LIST_TERMINAL, MaxComputeUnitMapping}   ///< End
    };


    #if USES_REGISTER_TABLES == TRUE
      CONST REGISTER_TABLE ROMDATA *F16MlRegisterTablesAfterApLaunch[] =
      {
        #if MODEL_SPECIFIC_PCI == TRUE
          &F16MlPciRegisterTableAfterApLaunch,
          &F16MlPciWorkaroundTable,
        #endif
        #if MODEL_SPECIFIC_MSR == TRUE
          &F16MlMsrRegisterTable,
          &F16MlMsrWorkaroundTable,
          &F16MlSharedMsrRegisterTable,
          &F16MlSharedMsrWorkaroundTable,
        #endif
        // the end.
        NULL
      };
      CONST REGISTER_TABLE ROMDATA *F16MlRegisterTablesAfterApLaunchSecureS3[] =
      {
        #if MODEL_SPECIFIC_PCI == TRUE
          &F16MlPciRegisterTableAfterApLaunch,
          &F16MlPciWorkaroundTable,
        #endif
        // the end.
        NULL
      };
      CONST REGISTER_TABLE ROMDATA *F16MlRegisterTablesBeforeApLaunch[] =
      {
        #if MODEL_SPECIFIC_PCI == TRUE
          &F16MlPciRegisterTableBeforeApLaunch,
        #endif
        // the end.
        NULL
      };
      CONST REGISTER_TABLE ROMDATA *F16MlRegisterTablesBeforeApLaunchSecureS3[] =
      {
        #if MODEL_SPECIFIC_PCI == TRUE
          &F16MlPciRegisterTableBeforeApLaunch,
        #endif
        // the end.
        NULL
      };
    #endif

    #include "OptionCpuSpecificServicesInstallReset.h"
    #define CpuSrvcTableName                            cpuF16MlServices

    #define CpuSrvcDisablePstate                        F16DisablePstate
    #define CpuSrvcTransitionPstate                     F16TransitionPstate
    #define CpuSrvcGetProcIddMax                        F16MlGetProcIddMax
    #define CpuSrvcGetTscRate                           F16GetTscRate
    #define CpuSrvcGetCurrentNbFrequency                F16MlGetCurrentNbFrequency
    #define CpuSrvcGetMinMaxNbFrequency                 F16MlGetMinMaxNbFrequency
    #define CpuSrvcGetNbPstateInfo                      F16MlGetNbPstateInfo
    #define CpuSrvcIsNbCofInitNeeded                    F16GetNbCofVidUpdate
    #define CpuSrvcGetNbIddMax                          F16MlGetNbIddMax
    #define CpuSrvcLaunchApCore                         F16LaunchApCore
    #define CpuSrvcGetNumberOfPhysicalCores             F16MlGetNumberOfPhysicalCores
    #define CpuSrvcGetApCoreNumber                      F16MlGetApCoreNumber
    #define CpuSrvcCoreIdPositionInInitialApicId        F16CpuAmdCoreIdPositionInInitialApicId
    #define CpuSrvcSetWarmResetFlag                     F16SetAgesaWarmResetFlag
    #define CpuSrvcGetWarmResetFlag                     F16GetAgesaWarmResetFlag
    #define CpuSrvcGetMicroCodePatchesStruct            GetF16MlMicroCodePatchesStruct
    #define CpuSrvcGetMicrocodeEquivalenceTable         GetF16MlMicrocodeEquivalenceTable
    #define CpuSrvcGetCacheInfo                         GetF16CacheInfo
    #define CpuSrvcGetSysPmTableStruct                  GetF16MlSysPmTable
    #define CpuSrvcGetWheaInitData                      GetF16WheaInitData
    #define CpuSrvcIsNbPstateEnabled                    F16MlIsNbPstateEnabled
    #define CpuSrvcIsPspDevicePresent                   F16MlIsPspDevicePresent
    #define CpuSrvcIsSecureS3                           F16MlIsSecureS3
    #define CpuSrvcDoEntryTypeMsrRegister               SetRegisterForMsrEntry
    #define CpuSrvcDoEntryTypePciRegister               SetRegisterForPciEntry
    #define CpuSrvcDoEntryTypeFamSpecificWorkaround     SetRegisterForFamSpecificWorkaroundEntry
    #define CpuSrvcDoEntryTypeProfileFixup              SetRegisterForPerformanceProfileEntry
    #define CpuSrvcDoEntryTypeCoreCountsPciRegister     SetRegisterForCoreCountsPerformanceEntry
    #define CpuSrvcRegisterTableListAfterApLaunch       (REGISTER_TABLE **) F16MlRegisterTablesAfterApLaunch
    #define CpuSrvcRegisterTableListAfterApLaunchSecureS3 (REGISTER_TABLE **) F16MlRegisterTablesAfterApLaunchSecureS3
    #define CpuSrvcRegisterTableListBeforeApLaunch      (REGISTER_TABLE **) F16MlRegisterTablesBeforeApLaunch
    #define CpuSrvcRegisterTableListBeforeApLaunchSecureS3 (REGISTER_TABLE **) F16MlRegisterTablesBeforeApLaunchSecureS3
    #define CpuSrvcComputeUnitMap                       (COMPUTE_UNIT_MAP *) &TopologyFam16MlComputeUnitMapping
    #define CpuSrvcInitCacheDisabled                    InitCacheEnabled
    #define CpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable GetF16MlEarlyInitBeforeApLaunchOnCoreTable
    #define CpuSrvcGetEarlyInitAfterApLaunchOnCoreTable GetF16MlEarlyInitAfterApLaunchOnCoreTable
    #define CpuSrvcPatchLoaderIsSharedByCU              FALSE

    #include "OptionCpuSpecificServicesInstall.h"
    INSTALL_CPU_SPECIFIC_SERVICES_TABLE (CpuSrvcTableName);

    #define ML_SOCKETS 1
    #define ML_MODULES 1
    #define ML_RECOVERY_SOCKETS 1
    #define ML_RECOVERY_MODULES 1
    extern F_CPU_GET_SUBFAMILY_ID_ARRAY GetF16MlLogicalIdAndRev;
    #define OPT_F16_ML_ID (PF_CPU_GET_SUBFAMILY_ID_ARRAY) GetF16MlLogicalIdAndRev,
    #ifndef ADVCFG_PLATFORM_SOCKETS
      #define ADVCFG_PLATFORM_SOCKETS ML_SOCKETS
    #else
      #if ADVCFG_PLATFORM_SOCKETS < ML_SOCKETS
        #undef ADVCFG_PLATFORM_SOCKETS
        #define ADVCFG_PLATFORM_SOCKETS ML_SOCKETS
      #endif
    #endif
    #ifndef ADVCFG_PLATFORM_MODULES
      #define ADVCFG_PLATFORM_MODULES ML_MODULES
    #else
      #if ADVCFG_PLATFORM_MODULES < ML_MODULES
        #undef ADVCFG_PLATFORM_MODULES
        #define ADVCFG_PLATFORM_MODULES ML_MODULES
      #endif
    #endif

    #if (AGESA_ENTRY_INIT_RECOVERY == TRUE) || (AGESA_ENTRY_INIT_EARLY == TRUE)
      #define F16_ML_UCODE_7301
      #define F16_ML_UCODE_7300

      #if AGESA_ENTRY_INIT_EARLY == TRUE
        #if OPTION_EARLY_SAMPLES == TRUE
        #endif

        extern  CONST UINT8 ROMDATA  CpuF16MlId7300MicrocodePatch[];
        #undef F16_ML_UCODE_7300
        #define F16_ML_UCODE_7300 CpuF16MlId7300MicrocodePatch,

        extern  CONST UINT8 ROMDATA  CpuF16MlId7301MicrocodePatch[];
        #undef F16_ML_UCODE_7301
        #define F16_ML_UCODE_7301 CpuF16MlId7301MicrocodePatch,
      #endif

      CONST UINT8 ROMDATA *CpuF16MlMicroCodePatchArray[] =
      {
        F16_ML_UCODE_7301
        F16_ML_UCODE_7300
        NULL
      };

      CONST UINT8 ROMDATA CpuF16MlNumberOfMicrocodePatches = (UINT8) ((sizeof (CpuF16MlMicroCodePatchArray) / sizeof (CpuF16MlMicroCodePatchArray[0])) - 1);
    #endif

    CONST PF_CPU_GET_SUBFAMILY_ID_ARRAY ROMDATA F16MlLogicalIdTable[] =
    {
      OPT_F16_ML_ID
    };

    #define OPT_F16_ML_ID_TABLE {0x00730F00ul, {AMD_FAMILY_16_ML, AMD_F16_UNKNOWN}, F16MlLogicalIdTable, (sizeof (F16MlLogicalIdTable) / sizeof (F16MlLogicalIdTable[0]))},
    #define OPT_F16_ML_CPU {AMD_FAMILY_16_ML, &cpuF16MlServices},

  #else  //  OPTION_FAMILY16H_ML == TRUE
    #define OPT_F16_ML_CPU
    #define OPT_F16_ML_ID
    #define OPT_F16_ML_ID_TABLE
  #endif  //  OPTION_FAMILY16H_ML == TRUE
#else  //  defined (OPTION_FAMILY16H_ML)
  #define OPT_F16_ML_CPU
  #define OPT_F16_ML_ID
  #define OPT_F16_ML_ID_TABLE
#endif  //  defined (OPTION_FAMILY16H_ML)


// Family 16h maximum base address is 40 bits. Limit BLDCFG to 40 bits, if appropriate.

#if (FAMILY_MMIO_BASE_MASK < 0xFFFFFF0000000000ull)

  #undef  FAMILY_MMIO_BASE_MASK

  #define FAMILY_MMIO_BASE_MASK (0xFFFFFF0000000000ull)

#endif

#undef OPT_F16_ID_TABLE
#define OPT_F16_ID_TABLE  OPT_F16_ML_ID_TABLE

#undef OPT_F16_TABLE
#define OPT_F16_TABLE  OPT_F16_ML_CPU



CONST PF_CPU_GET_SUBFAMILY_ID_ARRAY ROMDATA F16LogicalIdTable[] =

{

  OPT_F16_ML_ID

};


#endif  // _OPTION_FAMILY_16H_INSTALL_H_
