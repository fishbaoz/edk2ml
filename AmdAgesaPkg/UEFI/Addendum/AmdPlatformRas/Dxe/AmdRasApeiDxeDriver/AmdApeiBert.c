/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD APEI Bert for RAS
 *
 * DXE Driver to perform APEI (WHEA) BERT - Boot Error Record Table tasks.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI/Addendum/AmdPlatformRas/Addendum/AmdPlatformRas
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ****************************************************************************
 * AMD Generic Encapsulated Software Architecture
 *
 * Description: AmdApeiBert.c - DXE Driver to perform APEI (WHEA) BERT - Boot
 *                              Error Record Table tasks.
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
  #include "AmdApeiBert.h"
  #include "EfiDriverLib.h"
  #include "AmdApei.h"


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
extern APEI_DRIVER_PRIVATE_DATA *mApeiPrivData;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */



/*----------------------------------------------------------------------------------------
 *                          T A B L E    D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */
APEI_BERT_ACPI_TABLE mBertAcpiTable = {
  //EFI_ACPI_DESCRIPTION_HEADER
  { BERT_SIG, sizeof (APEI_BERT_ACPI_TABLE), 1, 0, "AMD", AMD_BERT, 0, 0, 0},
  { //  APEI_BERT_CONTEXT
    sizeof (APEI_BOOT_ERROR_REGION_TABLE),  // UINT32 nBootErrRegLen - Size of Boot Error Region Table
    0                                       // UNIT64 BootErrRegPtr - Address of Boot Error Region Table Address
  }
};


APEI_BOOT_ERROR_REGION_TABLE mApeiBootErrRegTbl = {
  0x00000000ul,                               // UINT32 nBlockStatus - WHEA Platform Design Guide Table 3-2 Block Status
  sizeof (APEI_BOOT_ERROR_REGION_TABLE),      // UINT32 nRawDataOffet - WHEA Platform Design Guide Table 3-2 Raw Data Offset
  0,                                          // UINT32 nRawDataLength - WHEA Platform Design Guide Table 3-2 Raw Data Offset
  0,                                          // UINT32 nGenericDataLen - WHEA Platform Design Guide Table 3-2 Raw Data Offset
  0x03                                        // UINT32 ErrorSeverity - WHEA Platform Design Guide Table 3-2 Raw Data Offset (Initialize Severity to [None] = 0x03)
};


/**
 *---------------------------------------------------------------------------------------
 *
 *  ApeiBertInit
 *
 *  Description:
 *    Allocate ACPI NVS memory for Boot error region table and provides the pointer to BERT
 *    ACPI table interface.  Copy BERT ACPI table to Boot Services memory space. Updates the ACPI
 *    table address in APEI private data structure which is shared by a protocol interface.
 *
 *  Parameters:
 *    @retval     EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
ApeiBertInit ( VOID )
{
  EFI_STATUS            Status;
  UINT8                 *BootErrRgnTbl;
  APEI_BERT_ACPI_TABLE  *ApeiBertAcpiTable;

  //
  // Reserve EFI ACPI NVS Memory for Boot Error Region Table
  //
  Status = gBS->AllocatePool (EfiACPIMemoryNVS, sizeof (APEI_BOOT_ERROR_REGION_TABLE), &BootErrRgnTbl);
  // Copy Boot Error Region Table into EFI ACPI NVS Memory
  gBS->CopyMem (BootErrRgnTbl, &mApeiBootErrRegTbl, sizeof (APEI_BOOT_ERROR_REGION_TABLE));
  // Assign  Boot Error Region pointer
  mBertAcpiTable.BertContext.BootErrRegPtr = (UINT64) ((UINTN) BootErrRgnTbl);

  //
  // Reserve EFI Boot Services Memory for APEI BERT Table
  //
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (APEI_BERT_ACPI_TABLE), &ApeiBertAcpiTable);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Copy APEI BERT table to EFI ACPI NVS Memory
  gBS->CopyMem (ApeiBertAcpiTable, &mBertAcpiTable, sizeof (APEI_BERT_ACPI_TABLE));

  // Set global Pointer to BERT table
  mApeiPrivData->ApeiBertTbl = ApeiBertAcpiTable;

  return EFI_SUCCESS;
}

