/* $NoKeywords:$ */
/**
 * @file
 *
 * NB services
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
*****************************************************************************
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

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  "AGESA.h"
#include  "Ids.h"
#include  "amdlib.h"
#include  "Gnb.h"
#include  "GnbPcieConfig.h"
#include  "GnbCommonLib.h"
#include  "OptionGnb.h"
#include  "GnbSmuInitLibV7.h"
#include  "heapManager.h"
#include  "GnbFamServices.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBSCSLIBV1_GNBSCSLIBV1_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#ifndef TYPE_D0F0xBC
  #define TYPE_D0F0xBC 0x4
#endif

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

AGESA_STATUS
GnbSmuLoadScsDataV7 (
  IN       GNB_HANDLE           *GnbHandle,
  IN       UINT8                *ScsDataPtr,
  IN       AMD_CONFIG_PARAMS    *StdHeader
  )
{
  DEV_OBJECT                DevObject;
  AGESA_STATUS              Status;
  GNB_BUILD_OPTIONS_COMMON  *GnbCommonOptions;

  IDS_PERF_TIMESTAMP (TP_BEGINGNBLOADSCSDATA, StdHeader);

  Status = AGESA_SUCCESS;
  GnbCommonOptions = (GNB_BUILD_OPTIONS_COMMON*) GnbFmGnbBuildOptions (StdHeader);
  if (GnbCommonOptions->CfgScsSupport == TRUE) {
    IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadScsDataV7 Enter\n");
    //Check input parameters
    ASSERT ((ScsDataPtr != NULL) && (StdHeader != NULL));
    if ((ScsDataPtr == NULL) || (StdHeader == NULL)) {
      return AGESA_ERROR;
    }

    //Verify the SCS block signature
    ASSERT (*(UINT32 *)ScsDataPtr == SMU_SCS_SIGNATURE);
    if (*(UINT32 *)ScsDataPtr != SMU_SCS_SIGNATURE) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Verify SCS Binary fail\n", ScsDataPtr);
      return AGESA_ERROR;
    }

    //Load SCS block
    IDS_HDT_CONSOLE (GNB_TRACE, "Load SCS @%08x\n", ScsDataPtr);
    DevObject.DevPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
    DevObject.GnbHandle = GnbHandle;
    DevObject.StdHeader = StdHeader;
    GnbSmuServiceRequestV7 (&DevObject, SMU_SERVICE_INDEX_SCS, (UINT32) (UINTN) ScsDataPtr, 0);

    //Get SCS result and save to Heap
    IDS_HDT_CONSOLE (GNB_TRACE, "Get SCS Result\n", ScsDataPtr);
    Status = GnbSmuGetScsResultV7 (GnbHandle, StdHeader);

    IDS_HDT_CONSOLE (GNB_TRACE, "Get SCS Result %s\n", (Status == AGESA_SUCCESS) ? "Success" : "Fail");
  }

  IDS_PERF_TIMESTAMP (TP_ENDGNBLOADSCSDATA, StdHeader);
  return Status;
}

AGESA_STATUS
GnbSmuGetScsResultV7 (
  IN       GNB_HANDLE           *GnbHandle,
  IN       AMD_CONFIG_PARAMS    *StdHeader
  )
{
  UINT32                      i;
  UINT32                      ScsResultAddr;
  UINT32                      *ScsResultBuf;
  UINT32                      NumApmWeights;
  AGESA_STATUS                Status;
  DEV_OBJECT                  DevObject;
  GNB_REGISTER_SERVICE        *GnbRegisterAccessProtocol;
  ALLOCATE_HEAP_PARAMS        AllocateHeapParams;

  Status = AGESA_SUCCESS;
  DevObject.GnbHandle = GnbHandle;
  DevObject.StdHeader = StdHeader;
  DevObject.DevPciAddress = GnbGetHostPciAddress (GnbHandle);

  Status = GnbLibLocateService (GnbRegisterAccessService, GnbGetSocketId (GnbHandle), &GnbRegisterAccessProtocol, StdHeader);
  ASSERT (Status == AGESA_SUCCESS);
  if (Status != AGESA_SUCCESS) {
    return Status;
  }

  //Get the command result (A pointer to SMURAM)
  ScsResultAddr = 0;
  GnbUraGet (&DevObject, TRxSmuIntArgument, &ScsResultAddr);
  IDS_HDT_CONSOLE (GNB_TRACE, "SMURAMAddr %08x ", ScsResultAddr);
  ASSERT (ScsResultAddr != 0);


  Status = GnbRegisterAccessProtocol->Read (GnbHandle, TYPE_D0F0xBC, ScsResultAddr + offsetof (SMU_RAM_CPU_INFO, CountApmWeights), &NumApmWeights, 0, StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "NumApmWeights %d\n", NumApmWeights);
  ASSERT (Status == AGESA_SUCCESS);
  if (Status != AGESA_SUCCESS) {
    return Status;
  }

  //Allocate heap for store the result
  AllocateHeapParams.BufferHandle = AMD_SCS_SMU_RAM_INFO;
  AllocateHeapParams.Persist = HEAP_LOCAL_CACHE;
  AllocateHeapParams.BufferPtr = NULL;
  AllocateHeapParams.RequestedBufferSize = sizeof (SMU_RAM_CPU_INFO) + SIZE_OF_APMWEIGHTS * NumApmWeights;
  Status = HeapAllocateBuffer (&AllocateHeapParams, StdHeader);
  ASSERT (Status == AGESA_SUCCESS);
  if (Status != AGESA_SUCCESS) {
    return Status;
  }

  ScsResultBuf = (UINT32 *)AllocateHeapParams.BufferPtr;
  //Get the result point by RxSmuIntArgument from SMURAM
  for (i = 0; i < (sizeof (SMU_RAM_CPU_INFO) / sizeof (UINT32) + NumApmWeights); i++) {
    Status = GnbRegisterAccessProtocol->Read (GnbHandle, TYPE_D0F0xBC, ScsResultAddr, &ScsResultBuf[i], 0, StdHeader);
    ASSERT (Status == AGESA_SUCCESS);
    if (Status != AGESA_SUCCESS) {
      break;
    }
    ScsResultAddr += sizeof (UINT32);
  }

  if (Status != AGESA_SUCCESS) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Fail on get SCS Result @%08x\n", ScsResultAddr);
    HeapDeallocateBuffer (AMD_SCS_SMU_RAM_INFO, StdHeader);
  } else {
    IDS_HDT_CONSOLE (GNB_TRACE, "SCS Result:\n");
    GnbLibDebugDumpBuffer (ScsResultBuf, sizeof (SMU_RAM_CPU_INFO) + SIZE_OF_APMWEIGHTS * NumApmWeights, 3, 8);
  }

  return AGESA_SUCCESS;
}