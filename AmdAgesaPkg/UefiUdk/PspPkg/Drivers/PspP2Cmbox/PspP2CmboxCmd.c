
/* $NoKeywords:$ */
/**
 * @file
 *
 * PspP2Cmbox Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD PSP UEFI Driver
 * @e sub-project   PspP2Cmbox Driver
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
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
 ******************************************************************************
 */
#include "PspP2Cmbox.h"


EFI_STATUS
EFIAPI
P2CmboxSpiGetAttrib (
  IN OUT   SPI_ATTRIB_REQ *Req
  )
{
  EFI_STATUS Status;

  Status = PspGetVolumeAttributes ( (EFI_FVB_ATTRIBUTES_2 *) &(Req->Attribute));

  PSP_DEBUG ("Psp.SpiGetAttrib:0x%x\n", Req->Attribute);
  return (Status);
}

EFI_STATUS
EFIAPI
P2CmboxSpiSetAttrib (
  IN OUT   SPI_ATTRIB_REQ  *Req
  )
{
  EFI_STATUS Status;

  Status = PspSetVolumeAttributes ( (EFI_FVB_ATTRIBUTES_2 *) &(Req->Attribute));

  PSP_DEBUG ("Psp.SpiSetAttrib:0x%x\n", Req->Attribute);
  return (Status);
}

EFI_STATUS
EFIAPI
P2CmboxSpiGetBlockSize (
  IN OUT   SPI_INFO_REQ *Req
  )
{
  EFI_STATUS Status;
  Status = PspGetBlockSize (Req->Lba, &Req->BlockSize, &Req->NumberOfBlocks);

  PSP_DEBUG ("Psp.SpiGetBlockSize Lba:0x%x BlockSize:0x%x NumberOfBlocks:0x%x\n",
         Req->Lba,
         Req->BlockSize,
         Req->NumberOfBlocks);
  return (Status);
}

EFI_STATUS
EFIAPI
P2CmboxSpiReadFv (
  IN OUT   SPI_RW_REQ *Req
  )
{
  EFI_STATUS Status;

  Status = PspRead ( Req->Lba, Req->Offset, &Req->NumByte, &Req->Buffer[0]);

  PSP_DEBUG ("Psp.SpiReadFv Lba:0x%x Offset:0x%x NumByte:0x%x Buffer:0x%x\n",
          Req->Lba,
          Req->Offset,
          Req->NumByte,
          &Req->Buffer);
  return (Status);
}

EFI_STATUS
EFIAPI
P2CmboxSpiWriteFv (
  IN OUT   SPI_RW_REQ  *Req
  )
{
  EFI_STATUS Status;

  Status = PspWrite ( Req->Lba, Req->Offset, &Req->NumByte, &Req->Buffer[0]);

  PSP_DEBUG ("Psp.SpiWriteFv Lba:0x%x Offset:0x%x NumByte:0x%x Buffer:0x%x\n",
          Req->Lba,
          Req->Offset,
          Req->NumByte,
          &Req->Buffer);

  return (Status);

}

EFI_STATUS
EFIAPI
P2CmboxSpiEraseFv (
  IN OUT   SPI_ERASE_REQ *Req
  )
{
  EFI_STATUS Status;

  Status = PspEraseBlocks ( Req->Lba, Req->NumberOfBlocks, EFI_LBA_LIST_TERMINATOR);

  PSP_DEBUG ("Psp.SpiEraseFv Lba:0x%x NumberOfBlocks:0x%x\n",
          Req->Lba,
          Req->NumberOfBlocks);
  return (Status);
}

EFI_STATUS
EFIAPI
ProcessPspCommand (
  IN UINT64                 Cmd,
  IN MBOX_DEFAULT_BUFFER   *MboxBuffer
  )
{

  EFI_STATUS Status = EFI_NOT_FOUND;

  switch (Cmd) {

  case  MboxPspCmdSpiGetAttrib:
    Status = P2CmboxSpiGetAttrib  (&(((MBOX_SPI_ATTRIB_BUFFER *)MboxBuffer)->Req) );
    break;

  case MboxPspCmdSpiSetAttrib:
    Status = P2CmboxSpiSetAttrib (&(((MBOX_SPI_ATTRIB_BUFFER *)MboxBuffer)->Req));
    break;

  case MboxPspCmdSpiGetBlockSize :
    Status = P2CmboxSpiGetBlockSize ( &(((MBOX_SPI_INFO_BUFFER *)MboxBuffer)->Req) );
    break;

  case MboxPspCmdSpiReadFV:
    Status = P2CmboxSpiReadFv (&(((MBOX_SPI_RW_BUFFER *)MboxBuffer)->Req));
    break;

  case MboxPspCmdSpiWriteFV:
    Status = P2CmboxSpiWriteFv (&(((MBOX_SPI_RW_BUFFER *)MboxBuffer)->Req) );
    break;

  case MboxPspCmdSpiEraseFV:
    Status = P2CmboxSpiEraseFv (&(((MBOX_SPI_ERASE_BUFFER *)MboxBuffer)->Req));
    break;

  default:
    PSP_DEBUG ("Unsupported P2C Command 0x%x\n", Cmd);
    break;
  }

  MboxBuffer->Header.Status  = (UINT32)Status;

  return (Status);
}

