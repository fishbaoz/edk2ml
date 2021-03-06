/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU POST API, and related functions.
 *
 * Contains code that initialized the CPU after memory init.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ****************************************************************************
 * AMD Generic Encapsulated Software Architecture
 *
 * Description: cpuPostInit.c - Cpu POST Initialization Functions.
 *
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
#include "Options.h"
#include "cpuRegisters.h"
#include "cpuApicUtilities.h"
#include "heapManager.h"
#include "cpuServices.h"
#include "cpuFeatures.h"
#include "GeneralServices.h"
#include "cpuPostInit.h"
#include "cpuPstateTables.h"
#include "cpuFamilyTranslation.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)
#define FILECODE PROC_CPU_CPUPOSTINIT_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
STATIC
SyncVariableMTRR (
  IN       AMD_CONFIG_PARAMS  *StdHeader
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
extern BUILD_OPT_CFG UserOptions;
extern CPU_FAMILY_SUPPORT_TABLE            PstateFamilyServiceTable;

extern
VOID
ExecuteWbinvdInstruction (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

AGESA_STATUS
PstateCreateHeapInfo (
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

/*---------------------------------------------------------------------------------------*/
/**
 * Performs CPU related initialization at the POST entry point
 *
 * This function performs a large list of initialization items.  These items
 * include:
 *
 *    -1      AP MTRR sync
 *    -2      P-state data gather
 *    -3      P-state leveling
 *    -4      AP cache breakdown & release
 *
 * @param[in]  StdHeader         Config handle for library and services
 * @param[in]  PlatformConfig    Config handle for platform specific information
 *
 * @retval     AGESA_SUCCESS
 *
 */
AGESA_STATUS
AmdCpuPost (
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN       PLATFORM_CONFIGURATION *PlatformConfig
  )
{
  AGESA_STATUS AgesaStatus;
  AGESA_STATUS CalledStatus;

  AgesaStatus = AGESA_SUCCESS;
  //
  // Sync variable MTRR
  //
  AGESA_TESTPOINT (TpProcCpuApMtrrSync, StdHeader);
  SyncVariableMTRR (StdHeader);

  AGESA_TESTPOINT (TpProcCpuPostFeatureInit, StdHeader);
  IDS_HDT_CONSOLE (CPU_TRACE, "  Dispatch CPU features after AP MTRR sync\n");
  CalledStatus = DispatchCpuFeatures (CPU_FEAT_AFTER_POST_MTRR_SYNC, PlatformConfig, StdHeader);
  if (CalledStatus > AgesaStatus) {
    AgesaStatus = CalledStatus;
  }
  //
  // P-state Gathered and set heap info
  //
  IDS_HDT_CONSOLE (CPU_TRACE, "  Create P-state info in the heap\n");
  PstateCreateHeapInfo (PlatformConfig, StdHeader);

  // Dispatch CPU features before relinquishing control of APs
  AGESA_TESTPOINT (TpProcCpuBeforeRelinquishAPsFeatureInit, StdHeader);
  IDS_HDT_CONSOLE (CPU_TRACE, "  Dispatch CPU features before Relinquishing control of APs\n");
  CalledStatus = DispatchCpuFeatures (CPU_FEAT_BEFORE_RELINQUISH_AP, PlatformConfig, StdHeader);
  if (CalledStatus > AgesaStatus) {
    AgesaStatus = CalledStatus;
  }

  // Relinquish control of all APs to IBV.
  IDS_HDT_CONSOLE (CPU_TRACE, "  Relinquish control of APs\n");
  RelinquishControlOfAllAPs (StdHeader);

  return (AgesaStatus);
}

/*---------------------------------------------------------------------------------------
 *                           L O C A L    F U N C T I O N S
 *---------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 * Determines the address in system DRAM that should be used for p-state data
 * gather and leveling.
 *
 * @param[out] Ptr               Address to utilize
 * @param[in]  StdHeader         Config handle for library and services
 *
 * @retval     AGESA_SUCCESS
 *
 */
AGESA_STATUS
GetPstateGatherDataAddressAtPost (
     OUT   UINT64            **Ptr,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT64 AddressValue;

  AddressValue = P_STATE_DATA_GATHER_TEMP_ADDR;

  *Ptr = (UINT64 *)(AddressValue);

  return AGESA_SUCCESS;
}


/*---------------------------------------------------------------------------------------*/
/**
 * AP task to sync memory subsystem MSRs with the BSC
 *
 * This function processes a list of MSRs and the BSC's current values for those
 * MSRs.  This will allow the APs to see system RAM.
 *
 * @param[in]  MtrrTable         Memory related MSR table
 * @param[in]  StdHeader         Config handle for library and services
 *
 */
VOID
SyncAllApMtrrToBsc (
  IN       VOID *MtrrTable,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8   i;

  for (i = 0; ((BSC_AP_MSR_SYNC *) MtrrTable)[i].RegisterAddress != 0; i++) {
    LibAmdMsrWrite (((BSC_AP_MSR_SYNC *) MtrrTable)[i].RegisterAddress,
                    &((BSC_AP_MSR_SYNC *) MtrrTable)[i].RegisterValue,
                    StdHeader);
  }
}


/*---------------------------------------------------------------------------------------*/
/**
 * Creates p-state information on the heap
 *
 * This function gathers p-state information from all processors in the system,
 * determines a level set of p-states, and places that information into the
 * heap.  This heap data will be used by GenerateSsdt to generate the
 * final _PSS and XPSS objects.
 *
 * @param[in]  PlatformConfig    Pointer to runtime configuration options
 * @param[in]  StdHeader         Config handle for library and services
 *
 * @retval     AGESA_SUCCESS     No error
 * @retval     AGESA_ERROR       CPU_ERROR_PSTATE_HEAP_NOT_AVAILABLE
 */
AGESA_STATUS
PstateCreateHeapInfo (
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  AGESA_STATUS            AgesaStatus;
  S_CPU_AMD_PSTATE        *PStateBufferPtr;
  ALLOCATE_HEAP_PARAMS    AllocHeapParams;
  UINT8                   *PStateBufferPtrInHeap;

  ASSERT (IsBsp (StdHeader, &AgesaStatus));

  //
  //Get proper address for gather data pool address
  //Zero P-state gather data pool
  //
  GetPstateGatherDataAddressAtPost ((UINT64 **)&PStateBufferPtr, StdHeader);
  LibAmdMemFill (PStateBufferPtr, 0, sizeof (S_CPU_AMD_PSTATE), StdHeader);

  //
  //Get all the CPUs P-States and fill the PStateBufferPtr for each core
  //
  AgesaStatus = PStateGatherData (PlatformConfig, PStateBufferPtr, StdHeader);
  if (AgesaStatus != AGESA_SUCCESS) {
    return AgesaStatus;
  }

  //
  //Do Pstate Leveling for each core if needed.
  //
  AgesaStatus = PStateLeveling (PStateBufferPtr, StdHeader);

  //
  //Create Heap and store p-state data for ACPI table in CpuLate
  //
  AllocHeapParams.RequestedBufferSize = PStateBufferPtr->SizeOfBytes;
  AllocHeapParams.BufferHandle = AMD_PSTATE_DATA_BUFFER_HANDLE;
  AllocHeapParams.Persist = HEAP_SYSTEM_MEM;
  AgesaStatus = HeapAllocateBuffer (&AllocHeapParams, StdHeader);
  if (AgesaStatus == AGESA_SUCCESS) {
    //
    // Zero Buffer
    //
    PStateBufferPtrInHeap = (UINT8 *) AllocHeapParams.BufferPtr;
    LibAmdMemFill (PStateBufferPtrInHeap, 0, PStateBufferPtr->SizeOfBytes, StdHeader);
    LibAmdMemCopy  (PStateBufferPtrInHeap, PStateBufferPtr, PStateBufferPtr->SizeOfBytes, StdHeader);

  } else {
    PutEventLog (AGESA_ERROR,
                   CPU_ERROR_PSTATE_HEAP_NOT_AVAILABLE,
                 0, 0, 0, 0, StdHeader);
  }

  return AgesaStatus;
}

VOID
SyncApMsrsToBsc (
  IN OUT   BSC_AP_MSR_SYNC    *ApMsrSync,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  )
{
  AP_TASK                 TaskPtr;
  UINT16                  i;
  UINT32                  BscSocket;
  UINT32                  Ignored;
  UINT32                  BscCoreNum;
  UINT32                  Core;
  UINT32                  Socket;
  UINT32                  NumberOfSockets;
  UINT32                  NumberOfCores;
  AGESA_STATUS            IgnoredSts;

  ASSERT (IsBsp (StdHeader, &IgnoredSts));

  IdentifyCore (StdHeader, &BscSocket, &Ignored, &BscCoreNum, &IgnoredSts);
  NumberOfSockets = GetPlatformNumberOfSockets ();

  //
  //Sync all MTRR settings with BSP
  //
  for (i = 0; ApMsrSync[i].RegisterAddress != 0; i++) {
    LibAmdMsrRead (ApMsrSync[i].RegisterAddress, &ApMsrSync[i].RegisterValue, StdHeader);
  }

  TaskPtr.FuncAddress.PfApTaskI = SyncAllApMtrrToBsc;
  TaskPtr.DataTransfer.DataSizeInDwords = (UINT16) ((((sizeof (BSC_AP_MSR_SYNC)) * i) + 4) >> 2);
  TaskPtr.ExeFlags = WAIT_FOR_CORE;
  TaskPtr.DataTransfer.DataPtr = ApMsrSync;
  TaskPtr.DataTransfer.DataTransferFlags = 0;

  for (Socket = 0; Socket < NumberOfSockets; Socket++) {
    if (GetActiveCoresInGivenSocket (Socket, &NumberOfCores, StdHeader)) {
      for (Core = 0; Core < NumberOfCores; Core++) {
        if ((Socket != BscSocket) || (Core != BscCoreNum)) {
          ApUtilRunCodeOnSocketCore ((UINT8) Socket, (UINT8) Core, &TaskPtr, StdHeader);
        }
      }
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * SyncVariableMTRR
 *
 * Sync variable MTRR
 *
 * @param[in]  StdHeader         Config handle for library and services
 *
 */
VOID
STATIC
SyncVariableMTRR (
  IN       AMD_CONFIG_PARAMS  *StdHeader
  )
{
  BSC_AP_MSR_SYNC ApMsrSync[20];

  ApMsrSync[0].RegisterAddress = SYS_CFG;
  ApMsrSync[1].RegisterAddress = TOP_MEM;
  ApMsrSync[2].RegisterAddress = TOP_MEM2;
  ApMsrSync[3].RegisterAddress = 0x200;
  ApMsrSync[4].RegisterAddress = 0x201;
  ApMsrSync[5].RegisterAddress = 0x202;
  ApMsrSync[6].RegisterAddress = 0x203;
  ApMsrSync[7].RegisterAddress = 0x204;
  ApMsrSync[8].RegisterAddress = 0x205;
  ApMsrSync[9].RegisterAddress = 0x206;
  ApMsrSync[10].RegisterAddress = 0x207;
  ApMsrSync[11].RegisterAddress = 0x208;
  ApMsrSync[12].RegisterAddress = 0x209;
  ApMsrSync[13].RegisterAddress = 0x20A;
  ApMsrSync[14].RegisterAddress = 0x20B;
  ApMsrSync[15].RegisterAddress = 0xC0010016;
  ApMsrSync[16].RegisterAddress = 0xC0010017;
  ApMsrSync[17].RegisterAddress = 0xC0010018;
  ApMsrSync[18].RegisterAddress = 0xC0010019;
  ApMsrSync[19].RegisterAddress = 0;
  SyncApMsrsToBsc (ApMsrSync, StdHeader);
}

/*---------------------------------------------------------------------------------------*/
/**
 * The function suppose to do any thing need to be done at the end of AmdInitPost.
 *
 * @param[in]  StdHeader         Config handle for library and services
 *
 * @retval     AGESA_SUCCESS
 *
 */
AGESA_STATUS
FinalizeAtPost (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  //
  // Execute wbinvd to ensure heap data in cache write back to memory.
  //
  ExecuteWbinvdInstruction (StdHeader);

  return AGESA_SUCCESS;
}
