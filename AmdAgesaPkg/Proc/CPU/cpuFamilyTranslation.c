/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU Family Translation functions.
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Interface
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
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "amdlib.h"
#include "Ids.h"
#include "cpuRegisters.h"
#include "CommonReturns.h"
#include "GeneralServices.h"
#include "cpuFamilyTranslation.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_CPU_CPUFAMILYTRANSLATION_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define RAW_TO_XLAT_MASK (UINT32) (CPUID_EXT_FAMILY_MASK | CPUID_EXT_MODEL_MASK | CPUID_BASE_FAMILY_MASK)
#define RAW_TO_LOGICAL_MASK (UINT32) (CPUID_EXT_FAMILY_MASK | CPUID_EXT_MODEL_MASK | CPUID_BASE_MODEL_MASK | CPUID_STEPPING_MASK)

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

CONST CPU_SPECIFIC_SERVICES ROMDATA cpuNullServices =
{
  0,                                                                      // Revision
  (PF_CPU_DISABLE_PSTATE) CommonReturnAgesaSuccess,                       // DisablePstate
  (PF_CPU_TRANSITION_PSTATE) CommonReturnAgesaSuccess,                    // TransitionPstate
  (PF_CPU_GET_IDD_MAX) CommonReturnFalse,                                 // GetProcIddMax
  (PF_CPU_GET_TSC_RATE) CommonReturnAgesaSuccess,                         // GetTscRate
  (PF_CPU_GET_NB_FREQ) CommonReturnAgesaSuccess,                          // GetCurrentNbFrequency
  (PF_CPU_GET_MIN_MAX_NB_FREQ) CommonReturnAgesaSuccess,                  // GetMinMaxNbFrequency
  (PF_CPU_GET_NB_PSTATE_INFO) CommonReturnFalse,                          // GetNbPstateInfo
  (PF_CPU_IS_NBCOF_INIT_NEEDED) CommonReturnAgesaSuccess,                 // IsNbCofInitNeeded
  (PF_CPU_GET_NB_IDD_MAX) CommonReturnFalse,                              // GetNbIddMax
  (PF_CPU_AP_INITIAL_LAUNCH) CommonReturnFalse,                           // LaunchApCore
  (PF_CPU_NUMBER_OF_PHYSICAL_CORES) CommonReturnZero8,                    // GetNumberOfPhysicalCores
  (PF_CPU_GET_AP_CORE_NUMBER) CommonReturnZero32,                         // GetApCoreNumber
  (PF_CORE_ID_POSITION_IN_INITIAL_APIC_ID) CommonReturnAgesaSuccess,      // CoreIdPositionInInitialApicId
  (PF_CPU_SET_WARM_RESET_FLAG) CommonReturnAgesaSuccess,                  // SetWarmResetFlag
  (PF_CPU_GET_WARM_RESET_FLAG) CommonReturnAgesaSuccess,                  // GetWarmResetFlag
  GetEmptyArray,                                                          // GetBrandString1
  GetEmptyArray,                                                          // GetBrandString2
  GetEmptyArray,                                                          // GetMicroCodePatchesStruct
  GetEmptyArray,                                                          // GetMicrocodeEquivalenceTable
  GetEmptyArray,                                                          // GetCacheInfo
  GetEmptyArray,                                                          // GetSysPmTableStruct
  GetEmptyArray,                                                          // GetWheaInitData
  (PF_CPU_GET_PLATFORM_TYPE_SPECIFIC_INFO) CommonReturnAgesaSuccess,      // GetPlatformTypeSpecificInfo
  (PF_IS_NB_PSTATE_ENABLED) CommonReturnFalse,                            // IsNbPstateEnabled
  {
    (PF_DO_TABLE_ENTRY) CommonAssert,                                     // DoTableEntry[MsrRegister]
    (PF_DO_TABLE_ENTRY) CommonAssert,                                     // DoTableEntry[PciRegister]
    (PF_DO_TABLE_ENTRY) CommonAssert,                                     // DoTableEntry[FamSpecificWorkaround]
    (PF_DO_TABLE_ENTRY) CommonAssert,                                     // DoTableEntry[ProfileFixup]
    (PF_DO_TABLE_ENTRY) CommonAssert,                                     // DoTableEntry[CoreCountsPciRegister]
    (PF_DO_TABLE_ENTRY) CommonAssert,                                     // DoTableEntry[ProcCountsPciRegister]
    (PF_DO_TABLE_ENTRY) CommonAssert,                                     // DoTableEntry[CompUnitCountsPciRegister]
    (PF_DO_TABLE_ENTRY) CommonAssert,                                     // DoTableEntry[CompUnitCountsMsr]
  },
  NULL,                                                                   // RegisterTableListAfterApLaunch
  NULL,                                                                   // RegisterTableListAfterApLaunchSecureS3
  NULL,                                                                   // RegisterTableListBeforeApLaunch
  NULL,                                                                   // RegisterTableListBeforeApLaunchSecureS3
  NULL,                                                                   // ComputeUnitMap
  InitCacheDisabled,                                                      // InitCacheDisabled
  (PF_GET_EARLY_INIT_TABLE) CommonVoid,                                   // GetEarlyInitBeforeApLaunchOnCoreTable
  (PF_GET_EARLY_INIT_TABLE) CommonVoid,                                   // GetEarlyInitAfterApLaunchOnCoreTable
  (PF_IS_PSP_PRESENT) CommonReturnFalse,                                  // IsPspDevicePresent
  (PF_IS_SECURE_S3) CommonReturnFalse,                                    // IsSecureS3
  FALSE                                                                   // PatchLoaderIsSharedByCU
};

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
STATIC
GetCpuServices (
  IN       CPU_FAMILY_SUPPORT_TABLE *FamilyTable,
  IN       UINT64            *MatchData,
     OUT   CONST VOID        **CpuServices,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
extern CPU_FAMILY_SUPPORT_TABLE CpuSupportedFamiliesTable;
extern CPU_FAMILY_ID_XLAT_TABLE CpuSupportedFamilyIdTable;

/*---------------------------------------------------------------------------------------*/
/**
 *
 *  Returns the logical ID of the desired processor. This will be obtained by
 *  reading the CPUID and converting it into a "logical ID" which is not package
 *  dependent.
 *
 *  @param[in]      Socket             Socket
 *  @param[out]     LogicalId          The Processor's Logical ID
 *  @param[in]      StdHeader          Handle of Header for calling lib functions and services.
 *
 */
VOID
GetLogicalIdOfSocket (
  IN       UINT32 Socket,
     OUT   CPU_LOGICAL_ID *LogicalId,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32      RawCpuid;
  PCI_ADDR    PciAddress;
  AGESA_STATUS AssumedSuccess;

  RawCpuid = 0;

  if (GetPciAddress (StdHeader, (UINT8)Socket, 0, &PciAddress, &AssumedSuccess)) {
    PciAddress.Address.Function = FUNC_3;
    PciAddress.Address.Register = CPUID_FMR;
    LibAmdPciRead (AccessWidth32, PciAddress, &RawCpuid, StdHeader);
    GetLogicalIdFromCpuid (RawCpuid, LogicalId, StdHeader);
  } else {
    LogicalId->Family = 0;
    LogicalId->Revision = 0;
    // Logical ID was not found.
    IDS_ERROR_TRAP;
  }
}


/*---------------------------------------------------------------------------------------*/
/**
 *
 *  Returns the logical ID of the executing core. This will be obtained by reading
 *  the CPUID and converting it into a "logical ID" which is not package dependent.
 *
 *  @param[out]     LogicalId          The Processor's Logical ID
 *  @param[in]      StdHeader          Handle of Header for calling lib functions and services.
 *
 */
VOID
GetLogicalIdOfCurrentCore (
     OUT   CPU_LOGICAL_ID *LogicalId,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  CPUID_DATA  CpuidDataStruct;

  LibAmdCpuidRead (AMD_CPUID_APICID_LPC_BID, &CpuidDataStruct, StdHeader);
  GetLogicalIdFromCpuid (CpuidDataStruct.EAX_Reg, LogicalId, StdHeader);
}


/*---------------------------------------------------------------------------------------*/
/**
 *
 *  Returns the logical ID of a processor with the given CPUID value. This
 *  will be obtained by converting it into a "logical ID" which is not package
 *  dependent.
 *
 *  @param[in]      RawCpuid           The unprocessed CPUID value to be translated
 *  @param[out]     LogicalId          The Processor's Logical ID
 *  @param[in]      StdHeader          Handle of Header for calling lib functions and services
 *
 */
VOID
GetLogicalIdFromCpuid (
  IN       UINT32 RawCpuid,
     OUT   CPU_LOGICAL_ID *LogicalId,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8       i;
  UINT8       k;
  UINT8       NumberOfFamiliesSupported;
  UINT8       NumberOfLogicalSubFamilies;
  UINT8       LogicalIdEntries;
  UINT32      j;
  UINT32      RawFamily;
  UINT32      CpuModelAndExtendedModel;
  UINT64      LogicalFamily;
  UINT64      HighestSupportedLogicalId;
  BOOLEAN     IdNotFound;
  BOOLEAN     FamilyNotFound;
  CONST PF_CPU_GET_SUBFAMILY_ID_ARRAY *SubFamilyIdPtr;
  CPU_LOGICAL_ID_XLAT *CpuLogicalIdAndRevPtr;
  CONST CPU_LOGICAL_ID_FAMILY_XLAT *ImageSupportedId;

  IdNotFound = TRUE;
  FamilyNotFound = TRUE;
  CpuLogicalIdAndRevPtr = NULL;
  ImageSupportedId = CpuSupportedFamilyIdTable.FamilyIdTable;
  NumberOfFamiliesSupported = CpuSupportedFamilyIdTable.Elements;

  RawFamily = RawCpuid & RAW_TO_XLAT_MASK;
  RawCpuid &= RAW_TO_LOGICAL_MASK;
  CpuModelAndExtendedModel = (UINT16) ((RawCpuid >> 8) | RawCpuid);

  LogicalId->Family = 0;
  LogicalId->Revision = 0;
  HighestSupportedLogicalId = 0;

  for (i = 0; i < NumberOfFamiliesSupported && FamilyNotFound; i++) {
    if (ImageSupportedId[i].Family == RawFamily) {
      FamilyNotFound = FALSE;
      LogicalId->Family = ImageSupportedId[i].UnknownRevision.Family;
      LogicalId->Revision = ImageSupportedId[i].UnknownRevision.Revision;

      NumberOfLogicalSubFamilies = ImageSupportedId[i].Elements;
      SubFamilyIdPtr = ImageSupportedId[i].SubFamilyIdTable;
      for (j = 0; j < NumberOfLogicalSubFamilies && IdNotFound; j++) {
        SubFamilyIdPtr[j] (&CpuLogicalIdAndRevPtr, &LogicalIdEntries, &LogicalFamily, &HighestSupportedLogicalId, StdHeader);
        ASSERT (CpuLogicalIdAndRevPtr != NULL);
        for (k = 0; k < LogicalIdEntries; k++) {
          if (CpuLogicalIdAndRevPtr[k].RawId == CpuModelAndExtendedModel) {
            IdNotFound = FALSE;
            LogicalId->Family = LogicalFamily;
            LogicalId->Revision = CpuLogicalIdAndRevPtr[k].LogicalId;
            break;
          }
        }
      }
      if (LogicalId->Revision == ImageSupportedId[i].UnknownRevision.Revision) {
        LogicalId->Revision = HighestSupportedLogicalId;
      }
    }
  }
}


/*---------------------------------------------------------------------------------------*/
/**
 *
 *  Retrieves a pointer to the desired processor's family specific services structure.
 *
 *  @param[in]      Socket             The Processor in this Socket.
 *  @param[out]     FunctionTable      The Processor's Family Specific services.
 *  @param[in]      StdHeader          Handle of Header for calling lib functions and services.
 *
 */
VOID
GetCpuServicesOfSocket (
  IN       UINT32 Socket,
     OUT   CONST CPU_SPECIFIC_SERVICES **FunctionTable,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  GetFeatureServicesOfSocket (&CpuSupportedFamiliesTable,
                              Socket,
                              (CONST VOID **) FunctionTable,
                              StdHeader);
  if (*FunctionTable == NULL) {
    *FunctionTable = &cpuNullServices;
  }
}


/*---------------------------------------------------------------------------------------*/
/**
 *
 *  Retrieves a pointer to the desired processor's family specific services structure.
 *
 *  @param[in]      FamilyTable        The table to search in.
 *  @param[in]      Socket             The Processor in this Socket.
 *  @param[out]     CpuServices        The Processor's Family Specific services.
 *  @param[in]      StdHeader          Handle of Header for calling lib functions and services.
 *
 */
VOID
GetFeatureServicesOfSocket (
  IN       CPU_FAMILY_SUPPORT_TABLE *FamilyTable,
  IN       UINT32            Socket,
     OUT   CONST VOID        **CpuServices,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  CPU_LOGICAL_ID CpuFamilyRevision;

  GetLogicalIdOfSocket (Socket, &CpuFamilyRevision, StdHeader);
  GetFeatureServicesFromLogicalId (FamilyTable, &CpuFamilyRevision, CpuServices, StdHeader);
}


/*---------------------------------------------------------------------------------------*/
/**
 *
 *  Retrieves a pointer to the executing core's family specific services structure.
 *
 *  @param[out]     FunctionTable      The Processor's Family Specific services.
 *  @param[in]      StdHeader          Handle of Header for calling lib functions and services.
 *
 */
VOID
GetCpuServicesOfCurrentCore (
     OUT   CONST CPU_SPECIFIC_SERVICES **FunctionTable,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  GetFeatureServicesOfCurrentCore (&CpuSupportedFamiliesTable,
                                   (CONST VOID **) FunctionTable,
                                   StdHeader);
  if (*FunctionTable == NULL) {
    *FunctionTable = &cpuNullServices;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *
 *  Retrieves a pointer to the family specific services structure for a processor
 *  with the given logical ID.
 *
 *  @param[in]      FamilyTable        The table to search in.
 *  @param[out]     CpuServices        The Processor's Family Specific services.
 *  @param[in]      StdHeader          Handle of Header for calling lib functions and services.
 *
 */
VOID
GetFeatureServicesOfCurrentCore (
  IN       CPU_FAMILY_SUPPORT_TABLE *FamilyTable,
     OUT   CONST VOID        **CpuServices,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  CPU_LOGICAL_ID CpuFamilyRevision;

  GetLogicalIdOfCurrentCore (&CpuFamilyRevision, StdHeader);
  GetFeatureServicesFromLogicalId (FamilyTable, &CpuFamilyRevision, CpuServices, StdHeader);
}


/*---------------------------------------------------------------------------------------*/
/**
 *
 *  Retrieves a pointer to the family specific services structure for a processor
 *  with the given logical ID.
 *
 *  @param[in]      LogicalId          The Processor's logical ID.
 *  @param[out]     FunctionTable      The Processor's Family Specific services.
 *  @param[in]      StdHeader          Handle of Header for calling lib functions and services.
 *
 */
VOID
GetCpuServicesFromLogicalId (
  IN       CPU_LOGICAL_ID *LogicalId,
     OUT   CONST CPU_SPECIFIC_SERVICES **FunctionTable,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  GetFeatureServicesFromLogicalId (&CpuSupportedFamiliesTable,
                                   LogicalId,
                                   (CONST VOID **) FunctionTable,
                                   StdHeader);
  if (*FunctionTable == NULL) {
    *FunctionTable = &cpuNullServices;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *
 *  Retrieves a pointer to the family specific services structure for a processor
 *  with the given logical ID.
 *
 *  @param[in]      FamilyTable        The table to search in.
 *  @param[in]      LogicalId          The Processor's logical ID.
 *  @param[out]     CpuServices        The Processor's Family Specific services.
 *  @param[in]      StdHeader          Handle of Header for calling lib functions and services.
 *
 */
VOID
GetFeatureServicesFromLogicalId (
  IN       CPU_FAMILY_SUPPORT_TABLE *FamilyTable,
  IN       CPU_LOGICAL_ID    *LogicalId,
     OUT   CONST VOID        **CpuServices,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  GetCpuServices (FamilyTable, &LogicalId->Family, CpuServices, StdHeader);
}


/*---------------------------------------------------------------------------------------*/
/**
 *
 *  Finds a family match in the given table, and returns the pointer to the
 *  appropriate table.  If no match is found in the table, NULL will be returned.
 *
 *  @param[in]      FamilyTable        The table to search in.
 *  @param[in]      MatchData          Family data that must match.
 *  @param[out]     CpuServices        The Processor's Family Specific services.
 *  @param[in]      StdHeader          Handle of Header for calling lib functions and services.
 *
 */
VOID
STATIC
GetCpuServices (
  IN       CPU_FAMILY_SUPPORT_TABLE *FamilyTable,
  IN       UINT64            *MatchData,
     OUT   CONST VOID        **CpuServices,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  BOOLEAN IsFamily;
  UINT8   i;
  UINT8   NumberOfFamiliesSupported;
  CONST CPU_SPECIFIC_SERVICES_XLAT *ImageSupportedFamiliesPtr;

  ImageSupportedFamiliesPtr = FamilyTable->FamilyTable;
  NumberOfFamiliesSupported = FamilyTable->Elements;
  IsFamily = FALSE;
  for (i = 0; i < NumberOfFamiliesSupported; i++) {
    if ((ImageSupportedFamiliesPtr[i].Family & *MatchData) != 0) {
      IsFamily = TRUE;
      break;
    }
  }
  if (IsFamily) {
    *CpuServices = ImageSupportedFamiliesPtr[i].TablePtr;
  } else {
    *CpuServices = NULL;
  }
}


/*---------------------------------------------------------------------------------------*/
/**
 *  Used to stub out various family specific tables of information.
 *
 *  @param[in]      FamilySpecificServices  The current Family Specific Services.
 *  @param[in]      Empty                   NULL, to indicate no data.
 *  @param[out]     NumberOfElements        Zero, to indicate no data.
 *  @param[in]      StdHeader               Handle of Header for calling lib functions and services.
 *
 */
VOID
GetEmptyArray (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
     OUT   CONST VOID **Empty,
     OUT   UINT8 *NumberOfElements,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  *NumberOfElements = 0;
  *Empty = NULL;
}
