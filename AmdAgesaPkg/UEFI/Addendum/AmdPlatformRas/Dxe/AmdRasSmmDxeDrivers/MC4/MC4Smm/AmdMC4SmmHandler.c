/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD MC4 SMM Handler
 *
 * Provide routines to support DRAM SMM error handling.
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
 * Description: AmdMC4SmmHandler.c - Provide functions to execute DRAM error
 * SMM handling.
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
#include "Tiano.h"
#include "BitDefine.h"
#include "AmdMC4SmmHandler.h"
#include "EfiRuntimeLib.h"        // Read & Write Memory Funcs
#include "CpuFuncs.h"             // Read & Write MSR Funcs
#include "AmdApeiHest.h"
#include "AmdApei.h"
#include "MemSmmLib.h"
#include "AmdRasIntDxeDriver.h"
#include "DimmMapping.h"
#include EFI_PROTOCOL_CONSUMER (SmmCpuState)
#include EFI_PROTOCOL_CONSUMER (SmmBase)
#include "AGESA.h"                // Make sure this is listed after EDK but before AGESA includes due to Pragma Pack (1) feature not closing


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
extern  UINT32                            mCfgMmioBase;    // Extended Config Space Base address
extern  UINT8                             mNumNodes;       //
extern  UINT8                             mCoresOnNode;    //
extern  EFI_SMM_CPU_SAVE_STATE_PROTOCOL   *mCpuSave;       // extern-ed in from AmdMC4Init.c file
extern  EFI_SMM_SYSTEM_TABLE              *mSmst;          // SMM System Table Poiner
extern  CORR_MEM_ERR_STS_BLK              *mCrErrorBlk;
extern  UNCORR_MEM_ERR_STS_BLK            *mUncrErrorBlk;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
MC4ErrorDecode (
  IN UINT8  NodeNum
  );

VOID
GenerateSci ( VOID );

EFI_STATUS
ResetMC4SmmOnCore ( VOID );


/**
 *---------------------------------------------------------------------------------------
 *
 *  MC4SmmHandler
 *
 *  Description:
 *     This is the Northbridge (MC4) Error SMM handler.
 *
 *  Parameters:
 *    @param[in]      SmmImageHandle
 *    @param[in,out]  OPTIONAL     *CommunicationBuffer
 *    @param[in,out]  OPTIONAL     *SourceSize
 *
 *    @retval        EFI_HANDLER_SUCCESS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS MC4SmmHandler (
  IN       EFI_HANDLE   SmmImageHandle,
  IN OUT   VOID     *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN    *SourceSize OPTIONAL
  )
{
  EFI_SMM_CPU_SAVE_STATE         *SmmCpuSaveLocalState;
  UINT64  CurrAddr;
  UINT32  Mc4Ctl;
  UINT32  Mc4StsHi;
  UINT8   NodeNum;
  UINT8   CpuNum;
  UINT8   *SmmBaseAddr;
  UINTN   ApNum;
  BOOLEAN ErrorPresent;

  // Initialize local variables
  CurrAddr = 0;
  Mc4Ctl = 0;
  Mc4StsHi = 0;
  NodeNum = 0;
  CpuNum = 0;
  SmmBaseAddr = NULL;
  ApNum = 0;
  ErrorPresent = FALSE;

  SmmCpuSaveLocalState = (EFI_SMM_CPU_SAVE_STATE *) (mSmst->CpuSaveState);

  // Loop through all CPU cores
  for (CpuNum = 0; CpuNum < mSmst->NumberOfCpus; CpuNum++) {
    SmmBaseAddr = (UINT8 *) ((UINTN) SmmCpuSaveLocalState->SMBASE);

    // Check for a valid SMM source
    if (!((*(UINT32*) (SmmBaseAddr + 0xfec4)) & (BIT25 | BIT20 | BIT19 | BIT18))) {
      SmmCpuSaveLocalState++;
      continue;
    }

    // Determine if we have ECC error enable bits set
    CurrAddr = mCfgMmioBase | ((0x18 + NodeNum) << 15) | (3 << 12) | 0x40; //F3:40 MC4_NB_CTL
    Mc4Ctl = RasSmmReadMem32 (CurrAddr); // Read PCI config space
    //  MCA NB CTL  (MSR000_0410) 0x40003  = Bit18 - Memory parity Enabled, Bit1 - UECCEn, Bit0-CECCEn &&
    //  StsHi 0x80000000 = Bit31 (MSR0000_0411 [63:32]) - Valid Error Bit Set?
    // Determine if we have an ECC error
    CurrAddr = mCfgMmioBase | ((0x18 + NodeNum) << 15) | (3 << 12) | 0x4C; //F3:48 MC4_STATUS HI32 MSR0000_0411 [63:32]
    Mc4StsHi = RasSmmReadMem32 (CurrAddr);  // Read PCI config space

    // Check if necessary enable bits [0x04003are set that we have a valid error before proceding
    if ((Mc4Ctl & ((1 << 18) | (1 << 1) | (1 << 0))) && (Mc4StsHi & (1 << 31))) {
      // Check Error Source Bits, BIT45[13] - Uncorrectable DRAM ECC
      //                          BIT46[14] - Correctable DRAM ECC
      //                          BIT58[26] - Valid Error Address
      if ((Mc4StsHi & (BIT14 | BIT13)) && (Mc4StsHi & BIT26)) {
        // We have a valid DRAM Correctable or Uncorrectable ECC Error that is
        // associated with the respective Node, so go log it to WHEA HEST.
        MC4ErrorDecode (NodeNum);
        ErrorPresent = TRUE;   // Flag to indicate an error is present
      } // End-- if (Mc4StsHi & (BIT14|BIT13&BIT26)
    }

#if CLEAR_MC4_ERROR == 0  /// Add preprocessor define if you do not want to clear MC4 valid bit
    // Clear MCA Error valid bit if needed
    Mc4StsHi &= ~BIT31;   // If Error-valid Bit31 set, then clear it?
    RasSmmWriteMem32 (CurrAddr, Mc4StsHi);
#endif

    // Update the Node Number via Integer Division
    if ((CpuNum + 1) % mCoresOnNode == 0) {
      NodeNum++;      // Increment Node number
    }
  } // End-- for (CpuNum < mSmst->NumberOfCpus)

  // Note: This code is OPTIONAL.  Most customers will not reset error handling support once the
  // first error has been logged; however, the hardware will continue to correct errors.
  if (ErrorPresent) {
    // Done handling the error so go restore error enable and reset
    // NB threshold counter on the BSP.
    ResetMC4SmmOnCore ();
    // Now go Startup remaining Node Core 0s and restore error enable and reset NB threshold counter
    for (ApNum = (UINT8) mCoresOnNode; ApNum < (mCoresOnNode * mNumNodes); ApNum += (UINT8) mCoresOnNode) {
      mSmst->SmmStartupThisAp ((EFI_AP_PROCEDURE) ResetMC4SmmOnCore, ApNum, NULL);
    }
  }

  return EFI_HANDLER_SUCCESS;
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  MC4ErrorDecode
 *
 *  Description:
 *     This function sources Northbridge errors.
 *
 *  Parameters:
 *    @param[in]      NodeNum   respective bit set for Node with error
 *
 *    @retval         VOID
 *
 *---------------------------------------------------------------------------------------
 **/
VOID MC4ErrorDecode (
  IN       UINT8  NodeNum
  )
{
  EFI_STATUS  Status;
  UINT64      CurrAddr;
  UINT32      NbStsLo;
  UINT32      NbStsHi;
  UINT32      NbAddrLo;
  UINT32      NbAddrHi;
  UINT8       ExtndErr;
  AMD_IDENTIFY_DIMM AmdLocateDimm;

  // Initialize local variables
  EfiZeroMem (&AmdLocateDimm, sizeof (AMD_IDENTIFY_DIMM));

  // We have a node with an error so now check the extended error code in Bits[20:16] to
  // see if this is an ECC error.
  CurrAddr = mCfgMmioBase | ((0x18 + NodeNum) << 15) | (3 << 12) | 0x48; //F3:48 MC4_StsLo
  NbStsLo = RasSmmReadMem32 (CurrAddr);
  // Mask off extended error portion and put in Bits[4:0]
  ExtndErr = (UINT8) (NbStsLo >> 16) & 0x1F;

  // Check Extended Error Bits - Is this an ECC error or DRAM Parity Error?
  if ((ExtndErr & 0x08) || (ExtndErr & 0x0D)) {
    // Get the Error Address High and Low values and combine them to form a complete error address
    // Read Northbridge Error Address Low
    CurrAddr = mCfgMmioBase | ((0x18 + NodeNum) << 15) | (3 << 12) | 0x50; //F3:50 MC4_AddrLo   Bits[31:0]
    NbAddrLo = RasSmmReadMem32 (CurrAddr);

    // Read Northbridge Error Address High
    CurrAddr = mCfgMmioBase | ((0x18 + NodeNum) << 15) | (3 << 12) | 0x54; //F3:54 MC4_AddrHi  Bits [48:32] - Bits [63:48] = Reserved
    NbAddrHi = (RasSmmReadMem32 (CurrAddr)) & 0xFFFF;
    AmdLocateDimm.MemoryAddress = (UINT64) NbAddrHi << 32 | NbAddrLo;

    // Translate DIMM Address to Bank, Row, Column, Chip Select
    Status = TranslateSysAddrToCS (&AmdLocateDimm);

    if (mCrErrorBlk && mUncrErrorBlk) {
      // Get Status High data
      CurrAddr = mCfgMmioBase | ((0x18 + NodeNum) << 15) | (3 << 12) | 0x4C; //F3:4C MC4_StsHi
      NbStsHi = RasSmmReadMem32 (CurrAddr);

      // Is this a Correctable ECC error?
      if (NbStsHi & BIT14) {
        // Error is correctable - Update the respective HEST entries
        mCrErrorBlk->PlatformMemErrSection.ErrStatus = ((1 << 18) | (4 << 8));   // Error Detected on Data Transaction | Internal DRAM Error
        mCrErrorBlk->PlatformMemErrSection.PhyAddr = AmdLocateDimm.MemoryAddress;
        mCrErrorBlk->PlatformMemErrSection.Node = (UINT16) AmdLocateDimm.SocketId;
        mCrErrorBlk->PlatformMemErrSection.Bank = (UINT16) AmdLocateDimm.Bank;
        mCrErrorBlk->PlatformMemErrSection.Row = (UINT16) AmdLocateDimm.Row;
        mCrErrorBlk->PlatformMemErrSection.Column = AmdLocateDimm.Column;
        mCrErrorBlk->PlatformMemErrSection.Module = (UINT16) DimmNum[AmdLocateDimm.SocketId][AmdLocateDimm.MemChannelId][AmdLocateDimm.DimmId];
        mCrErrorBlk->PlatformMemErrSection.Device = (UINT16) AmdLocateDimm.ChipSelect;
        mCrErrorBlk->CorrectGenDataEntry.ErrorSeverity = ERROR_SEVERITY_CORRECTED;  // 0x02
        mCrErrorBlk->CorrectGenErrStsBlk.ErrSeverity = ERROR_SEVERITY_CORRECTED;  // 0x02
        mCrErrorBlk->CorrectGenErrStsBlk.BlockStatus = (1 << 4) | (ERROR_STS_BLK_CORR_VALID << 0); // 0x12 Error Data Entry count = 1, Corrected Error = 2

        if (Status == EFI_NOT_FOUND) {
          EfiAsciiStrCpy (mCrErrorBlk->CorrectGenDataEntry.FRU_Text, "DIMM# Not Sourced");  // Error Fru Text String
        } else {
          EfiAsciiStrCpy (mCrErrorBlk->CorrectGenDataEntry.FRU_Text, "DIMM# Sourced"); // New Fru Text String
        }
      } else {
        // Must be Uncorrectable ECC
        mUncrErrorBlk->UnCorrectGenDataEntry.ErrorSeverity = ERROR_SEVERITY_FATAL;  // 0x01
        mUncrErrorBlk->UnCorrectGenErrStsBlk.ErrSeverity = ERROR_SEVERITY_FATAL;  // 0x01
        mUncrErrorBlk->UnCorrectGenErrStsBlk.BlockStatus = (1 << 4) | (ERROR_STS_BLK_UNCORR_VALID << 0); // 0x11 Error Data Entry count = 1, Uncorrected Error = 1
      } // End-- if (nNbStsHi & BIT14)
      // GenerateSci();   /// [Optional] - Tell OS to search HEST Generic Error Notification Sources for updates via SCI vs. polling
    } // End-- if (mCrErrorBlk && mUncrErrorBlk))
  } // End-- if (extended error check)
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  ResetMC4SmmOnCore
 *
 *  Description:
 *     This function executed on the BSP and all APs to reset MC4 values.
 *
 *  Parameters:
 *    @retval         EFI_SUCCESS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
ResetMC4SmmOnCore (
  VOID )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT64   MsrVal;

  // Read MSR0000_017B
  MsrVal = EfiReadMsr (0x17B);
  EfiWriteMsr (0x17b, MsrVal | 0x10); //Enable MC4 - NB

  // Enable MCA Status for Writing
  MsrVal = EfiReadMsr (AMD_MSR_HWCR);
  MsrVal = (MsrVal | ((UINT64) 1 << (UINT8) 18)); // MCAStsWrEn
  EfiWriteMsr (AMD_MSR_HWCR, MsrVal);

  MsrVal = EfiReadMsr (MSR_MC4_MISC0);

  MsrVal &= 0xfff8f000ffffffffull;
  // Enable threshold counter, Set interrupt type to SMI and threshold counter to 1
  //MsrVal |= 0x000c0ffe00000000;
  MsrVal = (MsrVal | ((UINT64) 0xFFE << (UINT8) 32)); // Set Threshold limit (Limit will be reached after 1 error occurs)
  MsrVal = (MsrVal | ((UINT64) 2 << (UINT8) 49));     // Set interrupt type to SMI
  MsrVal = (MsrVal | ((UINT64) 1 << (UINT8) 51));     // Enable threshold counter
  MsrVal = (MsrVal | ((UINT64) 1 << (UINT8) 61));     // Set Locked bit
  EfiWriteMsr (MSR_MC4_MISC0, MsrVal);

  // Disable MCA Status for Writing
  MsrVal = EfiReadMsr (AMD_MSR_HWCR);
  MsrVal = (MsrVal &~ ((UINT64) 1 << (UINT8) 18)); // MCAStsWrEn
  EfiWriteMsr (AMD_MSR_HWCR, MsrVal);

  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  GenerateSci
 *
 *  Description:
 *     Generate System Control Interrupt (SCI).
 *
 *  Parameters:
 *    @retval         VOID
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
GenerateSci (
  VOID )
{
  UINT8 Data;
  UINT64 Address;

  Address = mCfgMmioBase | (0x14 << 15) | (0 << 12) | 0x7E; //Bus 0, Dev 14, F0:7E SMBus
  Data = RasSmmReadMem8 (Address);  // Read SMBUS offset 0x7E
  Data &= ~BIT1;                    // Clear Bit 1
  RasSmmWriteMem8 (Address, Data);  // Write SMBUS offset 0x7E

  RasSmmWriteIo8 (0xED, 1);        // Delay
  RasSmmWriteIo8 (0xED, 3);        // Delay
  RasSmmWriteIo8 (0xED, 5);        // Delay
  RasSmmWriteIo8 (0xED, 2);        // Delay
  RasSmmWriteIo8 (0xED, 4);        // Delay

  // Long enough, now reset the SCI bit
  Data = RasSmmReadMem8 (Address);    // Read SMBUS offset 0x7E
  Data |= BIT1;                       // Set Bit 1
  RasSmmWriteMem8 (Address, Data);    // Write SMBUS offset 0x7E
}

