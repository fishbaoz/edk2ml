/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD SMM Callout for RAS
 *
 * AGESA DXE RAS SMM Callout Functions
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
 * Description: AmdRasSmmCallout.c - AGESA DXE RAS SMM Callout Functions.
 *                                   NOTE: This file contains protocol accesses
 *                                   that may not be BIOS vendor-specific.
 *                                   Therefore, BIOS vendor-specific protocol
 *                                   substitution may be required to successfully
 *                                   compile this source file.
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
#include "AmdRasSmmCallout.h"
#include "RAS.h"              // Setup Options
#include "SetupConfig.h"      // BIOS Setup Variable structure - BIOS Vendor Specific
#include EFI_PROTOCOL_CONSUMER (AmdRasInterfaceProtocol)
#include EFI_PROTOCOL_DEFINITION (EfiSetupUtility)  // BIOS Setup Protocol - BIOS Vendor Specific


/*----------------------------------------------------------------------------------------
 *                      E X T E R N A L   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */
extern EFI_BOOT_SERVICES  *gBS;
extern UINT32             mCfgMmioBase;

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
EFI_GUID gRASSETUPGuid = AMD_RAS_SETUP_GUID;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */



/*----------------------------------------------------------------------------------------
 *                   D R I V E R  L I B  G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
Ecc_Callout_Config (
  IN SMM_SETUP_CONFIG*  SmmSetupConfig
  );

VOID
ThreshCounter_Callout_Config (
  IN SMM_SETUP_CONFIG*  SmmSetupConfig
  );

VOID
RAS_Handle_Type_Callout_Config (
  IN SMM_SETUP_CONFIG*  SmmSetupConfig
  );

/**
 *---------------------------------------------------------------------------------------
 *
 *  AmdRasSmmCallout
 *
 *  Description:
 *     AMD RAS SMM Callout Function.
 *
 *  Parameters:
 *    @param[in]     CalloutID
 *    @param[in]     *ConfigBuffer
 *
 *    @retval         AGESA_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
AmdRasSmmCallout (
  IN       CALLOUT_ID CalloutID,
  IN OUT   VOID*      ConfigBuffer
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  SMM_SETUP_CONFIG* SmmSetupConfig = NULL;

  switch (CalloutID)
  {
  case ECC_HNDLR_CALLOUT_ID:
    SmmSetupConfig = (SMM_SETUP_CONFIG*) ConfigBuffer;
    Ecc_Callout_Config (SmmSetupConfig);
    break;
  case FF_CALLOUT_ID:
    break;
  case ECC_THRESH_COUNTER_CALLOUT_ID:
    SmmSetupConfig = (SMM_SETUP_CONFIG*) ConfigBuffer;
    ThreshCounter_Callout_Config (SmmSetupConfig);
    break;
  case RAS_HANDLE_CALLOUT_ID:
    SmmSetupConfig = (SMM_SETUP_CONFIG*) ConfigBuffer;
    RAS_Handle_Type_Callout_Config (SmmSetupConfig);
    break;
  default:
    EFI_UNSUPPORTED;
  }
  return Status;
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  Ecc_Callout_Config
 *
 *  Description:
 *     ECC Callout Config Function.
 *
 *  Parameters:
 *    @param[in]     SmmSetupConfig
 *
 *    @retval         VOID
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
Ecc_Callout_Config (
  IN SMM_SETUP_CONFIG*  SmmSetupConfig
  )
{
  RAS_SYSTEM_CONFIGURATION RasSysCfgValue;  // RAS Setup Configuration Struct
  UINT64  Address64;
  UINT32  Value32;

  // Determine if we have ECC error enable bits set
  Address64 = mCfgMmioBase | ((0x18 << 15) | (3 << 12) | 0x44); //D18F3x44 MC4_NB_CFG
  Value32 = RasSmmReadMem32 (Address64);
  if (Value32 & (1 << 22)) {
    RasSysCfgValue.Mca_Ecc_Enable = RAS_ECC_FW_FIRST;
  } else {
    RasSysCfgValue.Mca_Ecc_Enable = RAS_ECC_DISABLED; //Load default value = ECC Disabled
  }

  SmmSetupConfig->EccHandlerEnable = (RasSysCfgValue.Mca_Ecc_Enable > 0) ? TRUE : FALSE;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  ThreshCounter_Callout_Config
 *
 *  Description:
 *     Threshold Counter Callout Configuration.
 *
 *  Parameters:
 *    @param[in]     SmmSetupConfig
 *
 *    @retval        VOID
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
ThreshCounter_Callout_Config (
  IN SMM_SETUP_CONFIG*  SmmSetupConfig
  )
{
  EFI_STATUS                      Status;
  EFI_SETUP_UTILITY_PROTOCOL      *SetupUtility;   // BIOS Setup Protocol - BIOS Vendor Specific
  SYSTEM_CONFIGURATION            *SetupVariable;  // BIOS Setup Variable structure - BIOS Vendor Specific

  // Get Setup "DRAM Error Threshold" setting and override it to gRasNbCorrMemConfig.ThreshLimit
  Status = gBS->LocateProtocol (
                                &gEfiSetupUtilityProtocolGuid,
                                NULL,
                                &SetupUtility
                                );

  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;    // Error detected while trying to locate SMM Base Protocol
  }

  SetupVariable = (SYSTEM_CONFIGURATION *) (SetupUtility->SetupNvData);

  // Convert "DRAM Error Threshold" to one's complement and limit to 12 bits
  SmmSetupConfig->DramErrorThreshold = (UINT16) (~SetupVariable->DramErrorThreshold) & 0xfff;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  RAS_Handle_Type_Callout_Config
 *
 *  Description:
 *     Handle Type Callout Configuration.
 *
 *  Parameters:
 *    @param[in]     SmmSetupConfig
 *
 *    @retval        VOID
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
RAS_Handle_Type_Callout_Config (
  IN SMM_SETUP_CONFIG*  SmmSetupConfig
  )
{
  EFI_STATUS                      Status;
  EFI_SETUP_UTILITY_PROTOCOL      *SetupUtility;   // BIOS Setup Protocol - BIOS Vendor Specific
  SYSTEM_CONFIGURATION            *SetupVariable;  // BIOS Setup Variable structure - BIOS Vendor Specific

  // Get Setup "DRAM Error Threshold" setting and override it to gRasNbCorrMemConfig.ThreshLimit
  Status = gBS->LocateProtocol (
                                &gEfiSetupUtilityProtocolGuid,
                                NULL,
                                &SetupUtility
                                );

  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;    // Error detected while trying to locate SMM Base Protocol
  }

  SetupVariable = (SYSTEM_CONFIGURATION *) (SetupUtility->SetupNvData);

  // Get Setup "RAS Handle Type" and override it to gRasNbCorrMemConfig.DevCommon.ErrorMonitorType
  if (SetupVariable->RasHandleType == 1) {
    SmmSetupConfig->RasHandleType = APIC;
  }
}
