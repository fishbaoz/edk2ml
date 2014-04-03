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
#include "Tiano.h"
#include "PspDirectory.h"
#include "PspBaseLib.h"


/**
 * Get attribute of PSP NVRAM region
 *
 * @param[out] Attributes   Point to Attributes
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspGetVolumeAttributes  (
  OUT   EFI_FVB_ATTRIBUTES                          * Attributes
  )
{
  //To be filled, Remove this comment when codes have been filled
  //NOTE: This Routine will run under SMM mode.
  //Please make sure all required service is available
  return EFI_SUCCESS;
}

/**
 * Set attribute of PSP NVRAM region
 *
 * @param[out] Attributes   Point to Attributes
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspSetVolumeAttributes  (
  OUT   EFI_FVB_ATTRIBUTES                          * Attributes
  )
{
  //To be filled, Remove this comment when codes have been filled
  //NOTE: This Routine will run under SMM mode.
  //Please make sure all required service is available
  return EFI_SUCCESS;
}

/**
 * Get physical address of PSP NVRAM region
 *
 * @param[out]  Address     Point to Physical Address
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspGetPhysicalAddress (
  OUT   EFI_PHYSICAL_ADDRESS                        * Address
  )
{
  //To be filled, Remove this comment when codes have been filled
  //NOTE: This Routine will run under SMM mode.
  //Please make sure all required service is available
  return EFI_SUCCESS;
}


/**
 * Retrieve Specific LBA info: block size and number of block
 *
 * @param[in]       Lba             Start LBA of PSP NVRAM region
 * @param[in]       BlockSize       Size of each block
 * @param[in, out]  NumberOfBlocks  Number of block
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspGetBlockSize (
  IN       EFI_LBA                                      Lba,
     OUT   UINTN                                       *BlockSize,
     OUT   UINTN                                       *NumberOfBlocks
  )
{
  //To be filled, Remove this comment when codes have been filled
  //NOTE: This Routine will run under SMM mode.
  //Please make sure all required service is available
  return EFI_SUCCESS;
}

/**
 * Read PSP NVRAM region according to input
 *
 * @param[in]       Lba          Start LBA of PSP NVRAM region
 * @param[in]       Offset       Offset relative to LBA Start
 * @param[in, out]  NumBytes     Number in Byte
 * @param[in]       Buffer       Buffer contain the written data (Allocated by caller)
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspRead (
  IN       EFI_LBA                                      Lba,
  IN       UINTN                                        Offset,
  IN OUT   UINTN                                        *NumBytes,
     OUT   UINT8                                        *Buffer
  )
{
  //To be filled, Remove this comment when codes have been filled
  //NOTE: This Routine will run under SMM mode.
  //Please make sure all required service is available
  return EFI_SUCCESS;
}


/**
 * Write PSP NVRAM region according to input
 *
 * @param[in]       Lba          Start LBA of PSP NVRAM region
 * @param[in]       Offset       Offset relative to LBA Start
 * @param[in, out]  NumBytes     Number in Byte
 * @param[in]       Buffer       Buffer contain the written data (Allocated by caller)
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspWrite (
  IN       EFI_LBA                                      Lba,
  IN       UINTN                                        Offset,
  IN OUT   UINTN                                        *NumBytes,
  IN       UINT8                                        *Buffer
  )
{
  //To be filled, Remove this comment when codes have been filled
  //NOTE: This Routine will run under SMM mode.
  //Please make sure all required service is available
  return EFI_SUCCESS;
}

/**
 * Erase PSP NVRAM region according to input
 *
 *
 * @param[in]  Lba          Start LBA of PSP NVRAM region
 * @param[in]  NumOfBlocks  Number of Blocks
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspEraseBlocks (
  IN      EFI_LBA Lba,
  IN      UINTN   NumOfBlocks,
  IN      ...
  )
{
  //To be filled, Remove this comment when codes have been filled
  //NOTE: This Routine will run under SMM mode.
  //Please make sure all required service is available
  return EFI_SUCCESS;
}

/**
 * Initial Psp Storage required information
 *
 *
 * @param[in]  SystemTable Point to EFI system table structure
 *
 * @retval EFI_SUCCESS    Initial success
 * @retval Others         Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspStorageInit (
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  //To be filled, Remove this comment when codes have been filled
  //NOTE: This Routine will run under SMM mode.
  //Please make sure all required service is available
  //In this routine, you can add:
  //  1) Global variable initialization
  //  2) Locate Codes which available at SMM mode
  //  3) Locate PSP NVRAM region (PSPEntryInfo in PspBaseLib.h)
  return EFI_SUCCESS;
}

