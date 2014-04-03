/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD SMM CPU IO Library for RAS
 *
 * Device Path Protocol navigation Library
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
 * Description: AmdRasSmmCpuIoLib.c - Device Path Protocol navigation Library.
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
#include "AmdRasSmmCpuIoLib.h"


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
EFI_SMM_SYSTEM_TABLE            *gSmst;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */



/**
 *---------------------------------------------------------------------------------------
 *
 *  RasSmmReadMem8
 *
 *  Description:
 *     Returns data located at the specified memory address.
 *
 *  Parameters:
 *    @param[in]     Address
 *
 *    @retval        UINT8
 *
 *---------------------------------------------------------------------------------------
 **/

UINT8
RasSmmReadMem8 (
  IN UINT64 Address
  )
{
  UINT8 Buffer;

  Buffer = 0;

  gSmst->SmmIo.Mem.Read (&gSmst->SmmIo, SMM_IO_UINT8, (UINT64) Address, 1, &Buffer);
  return Buffer;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  RasSmmWriteMem8
 *
 *  Description:
 *     Writes 8-bit data at the specified memory address.
 *
 *  Parameters:
 *    @param[in]     Address
 *    @param[in]     Data
 *
 *---------------------------------------------------------------------------------------
 **/

VOID
RasSmmWriteMem8 (
  IN UINT64 Address,
  IN UINT8 Data
  )
{
  gSmst->SmmIo.Mem.Write (&gSmst->SmmIo, SMM_IO_UINT8, (UINT64) Address, 1, &Data);
  return;
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  RasSmmReadMem32
 *
 *  Description:
 *     Returns data located at the specified memory address.
 *
 *  Parameters:
 *    @param[in]     Address
 *
 *    @retval        UINT32
 *
 *---------------------------------------------------------------------------------------
 **/
UINT32
RasSmmReadMem32 (
  IN UINT64 Address
  )
{
  UINT32 Buffer;

  Buffer = 0;

  gSmst->SmmIo.Mem.Read (&gSmst->SmmIo, SMM_IO_UINT32, (UINT64) Address, 1, &Buffer);
  return Buffer;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  RasSmmReadMem64
 *
 *  Description:
 *     Returns data located at the specified memory address.
 *
 *  Parameters:
 *    @param[in]     Address
 *
 *    @retval        UINT32
 *
 *---------------------------------------------------------------------------------------
 **/
UINT64
RasSmmReadMem64 (
  IN UINT64 Address
  )
{
  UINT64 Buffer;

  Buffer = 0;

  gSmst->SmmIo.Mem.Read (&gSmst->SmmIo, SMM_IO_UINT64, (UINT64) Address, 1, &Buffer);
  return Buffer;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  RasSmmWriteMem32
 *
 *  Description:
 *     Writes 32-bit data at the specified memory address.
 *
 *  Parameters:
 *    @param[in]     Address
 *    @param[in]     Data
 *
 *---------------------------------------------------------------------------------------
 **/

VOID
RasSmmWriteMem32 (
  IN UINT64 Address,
  IN UINT32 Data
  )
{
  gSmst->SmmIo.Mem.Write (&gSmst->SmmIo, SMM_IO_UINT32, (UINT64) Address, 1, &Data);
  return;
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  RasSmmWriteMem64
 *
 *  Description:
 *     Writes 64-bit data at the specified memory address.
 *
 *  Parameters:
 *    @param[in]     Address
 *    @param[in]     Data
 *
 *    @retval        VOID
 *
 *---------------------------------------------------------------------------------------
 **/

VOID
RasSmmWriteMem64 (
  IN UINT64 Address,
  IN UINT64 Data
  )
{
  gSmst->SmmIo.Mem.Write (&gSmst->SmmIo, SMM_IO_UINT64, (UINT64) Address, 1, &Data);
  return;
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  RasSmmReadIo8
 *
 *  Description:
 *     Returns data located at the specified memory address.
 *
 *  Parameters:
 *    @param[in]     Address
 *
 *    @retval        UINT8
 *
 *---------------------------------------------------------------------------------------
 **/

UINT8
RasSmmReadIo8 (
  IN UINT64 Address
  )
{
  UINT8 Buffer;

  Buffer = 0;

  gSmst->SmmIo.Io.Read (&gSmst->SmmIo, SMM_IO_UINT8, (UINT64) Address, 1, &Buffer);
  return Buffer;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  RasSmmWriteIo8
 *
 *  Description:
 *     Writes 8-bit data at the specified I/O address.
 *
 *  Parameters:
 *    @param[in]     Address
 *    @param[in]     Data
 *
 *---------------------------------------------------------------------------------------
 **/

VOID
RasSmmWriteIo8 (
  IN UINT64 Address,
  IN UINT8 Data
  )
{
  gSmst->SmmIo.Io.Write (&gSmst->SmmIo, SMM_IO_UINT8, (UINT64) Address, 1, &Data);
  return;
}


