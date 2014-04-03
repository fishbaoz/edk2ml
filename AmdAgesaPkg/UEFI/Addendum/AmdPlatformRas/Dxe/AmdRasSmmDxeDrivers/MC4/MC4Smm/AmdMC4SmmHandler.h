/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD MC4 SMM Handler for RAS
 *
 * Provide functions to execute DRAM error SMM handling
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI/Addendum/AmdPlatformRas/ADDENDUM/AMD_UEFI_RAS
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ****************************************************************************
 * AMD Generic Encapsulated Software Architecture
 *
 * Description: AmdMC4SmmHandler.h - Provide functions to execute DRAM error
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

#ifndef   _AMD_MC4_SMM_HANDLER_H_
#define   _AMD_MC4_SMM_HANDLER_H_

#ifdef __cplusplus
  extern "C" {
  #endif

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
  #include "AmdMC4Init.h"
  #include "Acpi.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define BUS_0 0
#define FUNCTION_1 1
#define FUNCTION_2 2
#define FUNCTION_3 3
#define FUNCTION_4 4
#define DEVICE_24 24

#define NUM_CHANNELS_PER_NODE  2
#define NUM_CS_PER_CHANNEL  8
#define MAX_BANK_ADDR  0x7
#define MAX_ROW_ADDR  0x3FFFFul
#define MAX_COL_ADDR  0x3FFF


/*----------------------------------------------------------------------------------------
 *                              D A T A   S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
  VOID
  EnableNorthBridgeErrLog ( VOID );

  UINT8
  RasSmmReadMem8 (
    IN UINT64 Address
    );

  VOID
  RasSmmWriteMem8 (
    IN UINT64 Address,
    IN UINT8 Data
    );

  UINT32
  RasSmmReadMem32 (
    IN UINT64 Address
    );

  VOID
  RasSmmWriteMem32 (
    IN UINT64 Address,
    IN UINT32 Data
    );

  VOID
  RasSmmWriteIo8 (
    IN UINT64 Address,
    IN UINT8 Data
    );

  UINT8
  RasSmmReadIo8 (
    IN UINT64 Address
    );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
  EFI_STATUS
  MC4SmmHandler (
    IN EFI_HANDLE   SmmImageHandle,
    IN OUT VOID     *CommunicationBuffer OPTIONAL,
    IN OUT UINTN    *SourceSize OPTIONAL
    );

/****** DO NOT WRITE BELOW THIS LINE *******/
  #ifdef __cplusplus
  }
#endif
#endif // _AMD_MC4_SMM_HANDLER_H_
