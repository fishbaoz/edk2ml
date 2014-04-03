/* $NoKeywords: $ */
  /**
 * @file
 *
 * AMD RAS Reset PEIM
 *
 * Provide support to source errors present after a reset event.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI/Addendum/AmdPlatformRas/RAS
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
 /*
 ****************************************************************************
 * AMD Generic Encapsulated Software Architecture
 *
 * Description: AmdRasResetPeim.c - Contains support for sourcing errors
 *                              preset following a sync-flood reset event.
 *
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
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "Tiano.h"
#include "Pei.h"
#include "PeiLib.h"
#include "CpuFuncs.h"
#include "AmdRasResetPeim.h"

#include EFI_GUID_DEFINITION (AmdRasInfoHob)

EFI_GUID gAmdRasInfoHobGuid = AMD_RAS_INFO_HOB_GUID;

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                     L O C A L   D A T A
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_PEIM_ENTRY_POINT (AmdRasResetPeimEntryPoint);

/**
 *---------------------------------------------------------------------------------------
 *
 *  AmdRasResetPeimEntryPoint
 *
 *  Description:
 *     Entry point for the reset manager
 *     The reset manager currently supports up to 5 callbacks functions
 *
 *  Parameters:
 *    @param[in] *FfsHeader
 *    @param[in] **PeiServices
 *
 *    @retval    EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
AmdRasResetPeimEntryPoint (
  IN EFI_FFS_FILE_HEADER  *FfsHeader,
  IN EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS              Status;
  UINT8                   RasInfoHobBuffer[MAX_SIZEOF_AMD_RAS_INFO_HOB_BUFFER];
  AMD_RAS_INFO_HOB        *RasInfoHob;
  AMD_CPU_RAS_SOCKET      *RasCpuSocket;
  AMD_MCA_STAT_DESCRIPTOR *McaStatDesc;
  UINTN                   SizeOfRasInfoHob;
  UINTN                   CurrNode;
  UINTN                   CurrCore;
  UINT32                  CurrAddr;
  UINT32                  McCtl;
  UINT32                  StsLo;
  UINT32                  StsHi;
  UINT32                  AddrLo;
  UINT32                  AddrHi;
  UINT64                  MsrVal;
  UINT32                  MmioVal;
  UINT32                  CfgMmioBase;
  UINT8                   NumNodes;
  UINT8                   CoresOnNode;
  UINT8                   RasHobCreated;

  Status = EFI_SUCCESS;
  RasInfoHob = NULL;
  RasCpuSocket = NULL;
  McaStatDesc = NULL;
  SizeOfRasInfoHob = MAX_SIZEOF_AMD_RAS_INFO_HOB_BUFFER;
  CurrCore = 0;
  RasHobCreated = FALSE;

  RasInfoHob = (AMD_RAS_INFO_HOB *) &RasInfoHobBuffer[0];                // pRasInfoHob points to first index of the RasInfoHobBuffer
  RasCpuSocket = (AMD_CPU_RAS_SOCKET *) &RasInfoHob->RasCpuSocket;      // pRasCpuSocket points to first index of the RasCpuSocket array
  McaStatDesc = (AMD_MCA_STAT_DESCRIPTOR *) &RasCpuSocket->McaStatDesc; // pMcaStatDesc points to first index of the McaStatDesc array
  RasInfoHob->Version = AMD_RAS_INFO_HOB_VERISION;
  RasInfoHob->ErrorPresent = FALSE;                                      // Initialize ErrorPresent parameter to FALSE

  MsrVal = EfiReadMsr (AMD_MSR_MMIO_CFG_BASE);
  CfgMmioBase = (UINT32)MsrVal & 0xfff00000;

  // Extended Config Space (Bus 0, D18,Func 0, Offset 0x60) [0x?00C0060]
  CurrAddr = CfgMmioBase | ((0x18) << 15) | (0 << 12) | 0x60; //F0:60

  // Read Extended Config Space [0x?00C0060]
  MmioVal = *(UINT32*) CurrAddr;

  // Get number of nodes in the system from Bits[6:4]
  NumNodes = (UINT8) ((MmioVal >> 4) & 0x7) + 1;

  // Get the number of cores enabled on the entire *PLATFORM* from Bits[20:16] = CoreCnt[4:0]
  CoresOnNode = (UINT8) (MmioVal >> 16) & 0x1F;

  // Extended Config Space (Bus 0, D18,Func 0, Offset 0x60) [0x?00C00160]
  CurrAddr = CfgMmioBase | ((0x18) << 15) | (0 << 12) | 0x160; //F0:160

  // Read Extended Config Space [0x?00C0060]
  MmioVal = *(UINT32*) CurrAddr;

  // Get the total number of cores enabled on the entire *PLATFORM* from Bits[18:16] = CoreCnt[7:5]
  CoresOnNode |= (MmioVal >> (16 - 4)) & 0xE0;

  CoresOnNode = CoresOnNode / NumNodes;

  // Loop through all Nodes in the system to determine exact location of MCA errors
  for (CurrNode = 0; CurrNode < NumNodes; CurrNode++) {
    CurrAddr = CfgMmioBase | ((0x18 + CurrNode) << 15) | (3 << 12) | 0x40; //F3:40 MC4_NB_CTL
    McCtl = *(UINT32*) CurrAddr;
    CurrAddr = CfgMmioBase | ((0x18 + CurrNode) << 15) | (3 << 12) | 0x48; //F3:48 MC4_STATUS Low32 MSR0000_0411 [31:0]
    StsLo = *(UINT32*) CurrAddr;
    CurrAddr = CfgMmioBase | ((0x18 + CurrNode) << 15) | (3 << 12) | 0x4c; //F3:4c MC4_STATUS Hi32 MSR0000_0411 [63:32]
    StsHi = *(UINT32*) CurrAddr;

    // StsHi 0x80000000 = Bit63 (MSR0000_0411 [63:32]) - Valid Error Bit Set?
    if (StsHi & (1 << 31)) {
      // StsHi 0x2000 = Bit45 (MSR0000_0411 [63:32]) - UCECC - Uncorrectable ECC DRAM error Bit Set?
      if (StsHi & (1 << 13)) {
        // Get Uncorrectable Error Address Information
        CurrAddr = CfgMmioBase | ((0x18 + CurrNode) << 15) | (3 << 12) | 0x50; //F3:50 MC4_ADDRESS Lo32 MSR0000_0412 [31:1] Bit [0] = Reserved
        AddrLo = *(UINT32*) CurrAddr;

        //F3:54 MC4_ADDRESS Hi32 MSR0000_0412 [47:32] Note: Bits [63:48 are reserved)
        CurrAddr = CfgMmioBase | ((0x18 + CurrNode) << 15) | (3 << 12) | 0x54;
        // Mask off Bits [63:48] since they are reserved
        AddrHi = (*(UINT32*) CurrAddr) & 0xFFFF;

        McaStatDesc->MC_Control = McCtl;
        McaStatDesc->MC_StatusHi = StsHi;
        McaStatDesc->MC_StatusLow = StsLo;
        McaStatDesc->MC_AddressLo = AddrLo;
        McaStatDesc->MC_AddressHi = AddrHi;
        McaStatDesc->NodeNum = (UINT8) CurrNode;
        McaStatDesc->CoreNum = (UINT8) CurrCore;
        RasInfoHob->ErrorPresent = TRUE;
        RasInfoHob->ErrorCode = UNCORR_MC4_MEM;
        RasInfoHob->ClearError = FALSE;

        if (RasInfoHob->ClearError) {
          // Clear 32-Bit StHi and StLo MC4 Registers but retain reserved bits
          CurrAddr = CfgMmioBase | ((0x18 + CurrNode) << 15) | (3 << 12) | 0x48; //F3:48 MC4_STATUS Low32 MSR0000_0411 [31:0]
          *(UINT32*) CurrAddr &= 0x00800000;
          CurrAddr = CfgMmioBase | ((0x18 + CurrNode) << 15) | (3 << 12) | 0x4c; //F3:4c MC4_STATUS Hi32 MSR0000_0411 [63:32]
          *(UINT32*) CurrAddr &= 0x00E00000;
        } // End-- (pRasInfoHob->ClearError)

        Status = PeiBuildHobGuidData (PeiServices, &gAmdRasInfoHobGuid, &RasInfoHobBuffer, SizeOfRasInfoHob);
        RasHobCreated = TRUE;
      } // End-- if UCECC bit set
    } // End-- if Valid bit set
  } // End for(CurrNode)

  if (!RasHobCreated) {
    Status = PeiBuildHobGuidData (PeiServices, &gAmdRasInfoHobGuid, &RasInfoHobBuffer, SizeOfRasInfoHob);
  }

  return (Status);
}
