/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP Mailbox related functions
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 285254 $   @e \$Date: 2014-02-18 23:04:21 -0600 (Tue, 18 Feb 2014) $
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
#include "Uefi.h"
#include <Library/DebugLib.h>
#include <Library/PspMboxLib.h>
#include <Library/PspBaseLib.h>
#include "PspDebug.h"


/**
  Return the PspMbox MMIO location


  @retval EFI_STATUS  0: Success, NonZero Error

**/
EFI_STATUS
GetPspMboxLocation (
  IN OUT   PSP_MBOX **PspMbox
  )
{
  EFI_PHYSICAL_ADDRESS PspMmio;

  if (GetPspBar1Addr (&PspMmio)) {
    return (EFI_UNSUPPORTED);
  }

  *PspMbox = (PSP_MBOX *)( (UINTN)PspMmio + PSP_MAILBOX_BASE);   // PSPMbox base is at offset CP2MSG_28 ie. offset 28*4 = 0x70

  return (EFI_SUCCESS);
}


/**
  BIOS sends command to PSP

  @param[in]  MboxBuffer        PSP mbox buffer
  @param[in]  Cmd               Psp Mailbox Command

  @retval BOOLEAN                0: Success, 1 Error

**/
/// @todo Change function to some status type and appropriate status
/// Also add code in caller to handle this change
BOOLEAN
SendPspCommand (
  IN       VOID       *MboxBuffer,
  IN       MBOX_COMMAND       Cmd
  )
{
  UINT32     Command;
  BOOLEAN    Status;
  PSP_MBOX   *PspMbox;

  Status = TRUE;

  PSP_DEBUG ("Psp.SendC2PCMD [0x%x]\n", Cmd);

  // Get PspMbox location. fail if not found
  if (EFI_ERROR (GetPspMboxLocation (&PspMbox))) {
    PSP_DEBUG ("GetPspMboxLocation Error\n");
    ASSERT (FALSE); // Assertion in the debug build
    return (FALSE);
  }

  // The Command register may be disable by PSP driver when entering D3.
  // Save Command register
  Command = PspLibPciReadPspConfig (PSP_PCIE_CMD_REG);
   //Enable BusMaster & MemAccess
  PspLibPciWritePspConfig (PSP_PCIE_CMD_REG, Command | 0x6);

  if (PspMbox->MboxSts.Halt) {
    PSP_DEBUG ("MboxSts Halt\n");
    ASSERT (FALSE); // Assertion in the debug build
    //Restore Command register
    PspLibPciWritePspConfig (PSP_PCIE_CMD_REG, Command);
    return (FALSE);
  }

  if (PspMbox->MboxSts.Recovery) {
    PSP_DEBUG ("Recovery Flag detected, ignore the command\n");
    //Restore Command register
    PspLibPciWritePspConfig (PSP_PCIE_CMD_REG, Command);
    return (FALSE);
  }

  // Wait till mailbox is initialized or done processing command
  /// Wait for PSP to be ready. @todo add timeout
  while ( (!PspMbox->MboxSts.MboxInitialized) ||  (PspMbox->MboxCmd)) {
    ;
  }

  // Now send the command
  PspMbox->Buffer = (MBOX_BUFFER *)MboxBuffer;
  PspMbox->MboxCmd = Cmd;


  /// Wait for PSP to be done or reflect error @todo add timeout
  while (PspMbox->MboxCmd) {
    ;
  }

  // error vs. terminated
  if (PspMbox->MboxSts.Error || PspMbox->MboxSts.Terminated) {
    PSP_DEBUG ("MboxSts Error\n");
    Status = FALSE;
  }
  //Restore Command register
  PspLibPciWritePspConfig (PSP_PCIE_CMD_REG, Command);
  return Status;

}


/**
  BIOS sends the DRAM ready message to PSP.

  @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
PspMboxBiosCmdDramInfo (
  )
{
  UNALIGNED_MBOX_BUFFER   UBuff;    // Unaligned buffer
  MBOX_DEFAULT_BUFFER     *Buffer;  // Pointer to aligned buffer in stack

  Buffer = BALIGN32 (&UBuff);
  PSP_DEBUG ("Psp.C2PMbox.DramInfo\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_DEFAULT_BUFFER);

  SendPspCommand (Buffer, MboxBiosCmdDramInfo);
  PSP_DEBUG ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return (Buffer->Header.Status);
}

/**
 *
 *
 * Bios send this command to inform PSP to save the data needed to restore SRAM during resume from S3
 *
 * @param[in]  S3RestoreBufferBase           PSP reserve memory base near TOM
 * @param[in]  S3RestoreBufferSize           Size of PSP memory
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxBiosCmdS3Info (
  IN UINT64 S3RestoreBufferBase,
  IN UINT64 S3RestoreBufferSize
   )
{
  UNALIGNED_MBOX_BUFFER   UBuff;    // Unaligned buffer
  MBOX_S3DATA_BUFFER      *Buffer;

  Buffer = BALIGN32 (&UBuff);
  PSP_DEBUG ("Psp.C2PMbox.S3Info\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_S3DATA_BUFFER);

  Buffer->Req.S3RestoreBufferBase  = S3RestoreBufferBase;
  Buffer->Req.S3RestoreBufferSize  = S3RestoreBufferSize;

  SendPspCommand (Buffer, MboxBiosS3DataInfo);
  PSP_DEBUG ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return (Buffer->Header.Status);

}

/**
  BIOS sends the SMM information to PSP. SMM information is used by PSP to pass data back to BIOS

  @param[in]  SMMBase             Location of SMM base (TSEG)

  @param[in]  SMMLength           Length/Size of SMM space (TSEG)

  @param[in]  PSPSmmDataRegion    Location of reserved PSPSmmDataRegion within the SMM space.
                                  PSP will use this region to communicate with BIOS (i.e pass
                                  data/command at this region for BIOS SMM to process)

  @param[in]  PspSmmDataLength    Length of reserve PSPSmmDataRegion in SMM space for PSP communication

  @param[in]  SmmTrigInfo  IO port address to generate SoftSmi

  @retval EFI_STATUS              0: Success, NonZero Error

**/
EFI_STATUS
PspMboxBiosCmdSmmInfo (
  IN UINT64            SMMBase,
  IN UINT64            SMMLength,
  IN UINT64            PSPSmmDataRegion,
  IN UINT64            PspSmmDataLength,
  IN SMM_TRIGGER_INFO  *SmmTrigInfo
  )
{
  MBOX_SMM_BUFFER    *Buffer;
  UNALIGNED_MBOX_BUFFER   UBuff;    // Unaligned buffer

  Buffer = BALIGN32 (&UBuff);
  PSP_DEBUG ("Psp.C2PMbox.SmmInfo\n");

  Buffer->Header.TotalSize             = sizeof (MBOX_SMM_BUFFER);

  Buffer->Req.SMMBase                  = SMMBase;
  Buffer->Req.SMMLength                = SMMLength;
  Buffer->Req.PSPSmmDataRegion         = PSPSmmDataRegion;
  Buffer->Req.PspSmmDataLength         = PspSmmDataLength;

  Buffer->Req.SmmTrigInfo.AddressType  = SmmTrigInfo->AddressType;
  Buffer->Req.SmmTrigInfo.Address      = SmmTrigInfo->Address;
  Buffer->Req.SmmTrigInfo.ValueWidth   = SmmTrigInfo->ValueWidth;
  Buffer->Req.SmmTrigInfo.ValueAndMask = SmmTrigInfo->ValueAndMask;
  Buffer->Req.SmmTrigInfo.ValueOrMask  = SmmTrigInfo->ValueOrMask;

  SendPspCommand (Buffer, MboxBiosCmdSmmInfo);
  PSP_DEBUG ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return (Buffer->Header.Status);

}


/**
  BIOS sends the Sx information to PSP. This mailbox command is send just prior of entering Sx state

  @param[in]  SleepType           SleepType the system is transition to.
                                    3 : S3, 4:S4, 5:S5

  @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
PspMboxBiosCmdSxInfo (
  IN UINT8 SleepType
  )
{
  MBOX_SX_BUFFER    *Buffer;
  UNALIGNED_MBOX_BUFFER   UBuff;    // Unaligned buffer

  Buffer = BALIGN32 (&UBuff);

  PSP_DEBUG ("Psp.C2PMbox.SxInfo\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_SX_BUFFER);

  Buffer->Req.SleepType        = SleepType;

  SendPspCommand (Buffer, MboxBiosCmdSxInfo);

  PSP_DEBUG ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return (Buffer->Header.Status);

}

/**
  BIOS sends the Resume information to PSP. PSP will use this information to measure the resume code.
  During resume path PSP will use this information before handing off to BIOS to ensure the resume
  code is not tampered

  @param[in]  S3ResumeAddress       Location of BIOS reset code that will first fetch on resume

  @param[in]  S3ResumeCodeSize      Size of resume code that PSP need to measure to ensure authenticity

  @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
PspMboxBiosCmdResumeInfo (
  IN UINT64 S3ResumeAddress,
  IN UINT64 S3ResumeCodeSize
  )
{
  MBOX_RSM_BUFFER    *Buffer;
  UNALIGNED_MBOX_BUFFER   UBuff;    // Unaligned buffer

  Buffer = BALIGN32 (&UBuff);
  PSP_DEBUG ("Psp.C2PMbox.ResumeInfo\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_RSM_BUFFER);

  Buffer->Req.ResumeVecorAddress = S3ResumeAddress;
  Buffer->Req.ResumeVecorLength  = S3ResumeCodeSize;

  SendPspCommand (Buffer, MboxBiosCmdRsmInfo);
  PSP_DEBUG ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return (Buffer->Header.Status);
}


/**
  BIOS sends this command to inform PSP that BIOS is handing off to OS/OROM. Any mailbox command
  after this message need to come from SMM space (the SMM info is provided earlier via mailbox)
  i.e. PSP should check the parameter address to ensure it falls in SMM region

  @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
PspMboxBiosCmdExitBootServices ()
{
  MBOX_DEFAULT_BUFFER     *Buffer;
  UNALIGNED_MBOX_BUFFER   UBuff;    // Unaligned buffer

  Buffer = BALIGN32 (&UBuff);
  PSP_DEBUG ("Psp.C2PMbox.ExitBootServices\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_DEFAULT_BUFFER);

  SendPspCommand (Buffer, MboxBiosCmdBootDone);
  PSP_DEBUG ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return (Buffer->Header.Status);
}

/**
  BIOS requests the capabilities from the PSP

  @param[in]  Capabilities      PSP Writes capabilities into this field when it returns.

  @retval EFI_STATUS                0: Success, NonZero Error

**/
EFI_STATUS
PspMboxBiosQueryCaps (
  IN OUT   UINT32 *Capabilities
  )
{
  MBOX_CAPS_BUFFER   *Buffer;
  UNALIGNED_MBOX_BUFFER   UBuff;    // Unaligned buffer

  Buffer = BALIGN32 (&UBuff);
  PSP_DEBUG ("Psp.PspMboxBiosQueryCaps\n");

  if (Capabilities == NULL) {
    return (EFI_NOT_FOUND);
  }

  Buffer->Header.TotalSize      = sizeof (MBOX_CAPS_BUFFER);

  Buffer->Req.Capabilities      = 0; // Result of command will be placed here

  SendPspCommand (Buffer, MboxBiosCmdPspQuery);
  *Capabilities = Buffer->Req.Capabilities;
  PSP_DEBUG ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return (Buffer->Header.Status);
}


/**
  BIOS sends this dummy command to inform PSP that its done processing the SMM request
  The purpose of this command is to generate interrupt to PSP

  @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
PspMboxBiosCmdNop ()
{
  MBOX_DEFAULT_BUFFER     *Buffer;
  UNALIGNED_MBOX_BUFFER   UBuff;    // Unaligned buffer

  Buffer = BALIGN32 (&UBuff);
  PSP_DEBUG ("Psp.C2PMbox.Nop\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_DEFAULT_BUFFER);

  SendPspCommand (Buffer, MboxBiosCmdNop);
  PSP_DEBUG ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return (Buffer->Header.Status);

}
/**
  BIOS sends the clear S3 Status message to PSP.

  @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
PspMboxBiosClearS3Status (
  )
{
  MBOX_DEFAULT_BUFFER     *Buffer;
  UNALIGNED_MBOX_BUFFER   UBuff;    // Unaligned buffer

  Buffer = BALIGN32 (&UBuff);
  PSP_DEBUG ("Psp.C2PMbox.ClearS3Sts\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_DEFAULT_BUFFER);

  SendPspCommand (Buffer, MboxBiosCmdClearS3Sts);
  PSP_DEBUG ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return (Buffer->Header.Status);
}

/**
  Check if iTpm is ready

  @retval EFI_STATUS  0: Success, NonZero Error

**/
EFI_STATUS
CheckITPMSupported ()
{
  UINT32 Capabilities;

  if (PspMboxBiosQueryCaps (&Capabilities)) {
    return (EFI_UNSUPPORTED);
  }

  if (Capabilities & PSP_CAP_TPM) {
    return (EFI_SUCCESS);
  }

  return (EFI_UNSUPPORTED);
}
