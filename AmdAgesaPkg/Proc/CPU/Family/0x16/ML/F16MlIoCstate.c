/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_16 Mullins IO C-state feature support functions.
 *
 * Provides the functions necessary to initialize the IO C-state feature.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x16/ML
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
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

#include "AGESA.h"
#include "amdlib.h"
#include "Ids.h"
#include "cpuFeatures.h"
#include "cpuIoCstate.h"
#include "cpuF16PowerMgmt.h"
#include "F16MlPowerMgmt.h"
#include "cpuLateInit.h"
#include "cpuRegisters.h"
#include "cpuServices.h"
#include "cpuApicUtilities.h"
#include "cpuFamilyTranslation.h"
#include "CommonReturns.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)
#define FILECODE PROC_CPU_FAMILY_0X16_ML_F16MLIOCSTATE_FILECODE

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
STATIC
F16MlInitializeIoCstateOnCore (
  IN       VOID *CstateBaseMsr,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

BOOLEAN
F16MlIsCsdObjGenerated (
  IN       IO_CSTATE_FAMILY_SERVICES *IoCstateServices,
  IN       AMD_CONFIG_PARAMS         *StdHeader
  );

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern CPU_FAMILY_SUPPORT_TABLE            IoCstateFamilyServiceTable;

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 *  Enable IO Cstate on a family 16h Mullins CPU.
 *  Implement BIOS Requirements for Initialization of C-states
 *
 * @param[in]    IoCstateServices   Pointer to this CPU's IO Cstate family services.
 * @param[in]    EntryPoint         Timepoint designator.
 * @param[in]    PlatformConfig     Contains the runtime modifiable feature input data.
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       AGESA_SUCCESS      Always succeeds.
 *
 */
AGESA_STATUS
STATIC
F16MlInitializeIoCstate (
  IN       IO_CSTATE_FAMILY_SERVICES *IoCstateServices,
  IN       UINT64                    EntryPoint,
  IN       PLATFORM_CONFIGURATION    *PlatformConfig,
  IN       AMD_CONFIG_PARAMS         *StdHeader
  )
{
  UINT64                              LocalMsrRegister;
  AP_TASK                             TaskPtr;
  PCI_ADDR                            PciAddress;
  CSTATE_POLICY_CTRL1_REGISTER        CstatePolicyCtrl1;

  if ((EntryPoint & CPU_FEAT_AFTER_PM_INIT) != 0) {
    // Initialize F4x128
    // bit[1]   CoreCstatePolicy = 0
    // bit[4:2] HaltCstateIndex = 0
    PciAddress.AddressValue = CSTATE_POLICY_CTRL1_PCI_ADDR;
    LibAmdPciRead (AccessWidth32, PciAddress, &CstatePolicyCtrl1, StdHeader);
    CstatePolicyCtrl1.HaltCstateIndex = 0;
    LibAmdPciWrite (AccessWidth32, PciAddress, &CstatePolicyCtrl1, StdHeader);

    // Initialize MSRC001_0073[CstateAddr] on each core to a region of
    // the IO address map with 8 consecutive available addresses.
    LocalMsrRegister = 0;

    IDS_HDT_CONSOLE (CPU_TRACE, "    Init IO C-state Base at 0x%x\n", PlatformConfig->CStateIoBaseAddress);
    ((CSTATE_ADDRESS_MSR *) &LocalMsrRegister)->CstateAddr = PlatformConfig->CStateIoBaseAddress;

    TaskPtr.FuncAddress.PfApTaskI = F16MlInitializeIoCstateOnCore;
    TaskPtr.DataTransfer.DataSizeInDwords = 2;
    TaskPtr.DataTransfer.DataPtr = &LocalMsrRegister;
    TaskPtr.DataTransfer.DataTransferFlags = 0;
    TaskPtr.ExeFlags = WAIT_FOR_CORE;
    ApUtilRunCodeOnAllLocalCoresAtEarly (&TaskPtr, StdHeader, NULL);
  }
  return AGESA_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Enable CState on a family 16h Mullins core.
 *
 * @param[in]    CstateBaseMsr      MSR value to write to C001_0073 as determined by core 0.
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 */
VOID
STATIC
F16MlInitializeIoCstateOnCore (
  IN       VOID *CstateBaseMsr,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  // Initialize MSRC001_0073[CstateAddr] on each core
  LibAmdMsrWrite (MSR_CSTATE_ADDRESS, (UINT64 *) CstateBaseMsr, StdHeader);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Returns the size of CST object
 *
 * @param[in]    IoCstateServices   IO Cstate services.
 * @param[in]    PlatformConfig     Contains the runtime modifiable feature input data
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @retval       CstObjSize         Size of CST Object
 *
 */
UINT32
STATIC
F16MlGetAcpiCstObj (
  IN       IO_CSTATE_FAMILY_SERVICES *IoCstateServices,
  IN       PLATFORM_CONFIGURATION    *PlatformConfig,
  IN       AMD_CONFIG_PARAMS         *StdHeader
  )
{
  BOOLEAN                   GenerateCsdObj;
  UINT32                    CStateAcpiObjSize;
  IO_CSTATE_FAMILY_SERVICES *FamilyServices;
  ACPI_CST_GET_INPUT CstGetInput;

  CstGetInput.IoCstateServices = IoCstateServices;
  CstGetInput.PlatformConfig = PlatformConfig;
  CstGetInput.CStateAcpiObjSizePtr = &CStateAcpiObjSize;

  IDS_SKIP_HOOK (IDS_CST_SIZE, &CstGetInput, StdHeader) {
    CStateAcpiObjSize = CST_HEADER_SIZE + CST_BODY_SIZE;

    // If CSD Object is generated, add the size of CSD Object to the total size of
    // CState ACPI Object size
    GetFeatureServicesOfCurrentCore (&IoCstateFamilyServiceTable, (CONST VOID **)&FamilyServices, StdHeader);
    ASSERT (FamilyServices != NULL);
    GenerateCsdObj = FamilyServices->IsCsdObjGenerated (FamilyServices, StdHeader);

    if (GenerateCsdObj) {
      CStateAcpiObjSize += CSD_HEADER_SIZE + CSD_BODY_SIZE;
    }
  }
  return CStateAcpiObjSize;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Routine to generate the C-State ACPI objects
 *
 * @param[in]      IoCstateServices       IO Cstate services.
 * @param[in]      LocalApicId            Local Apic Id for each core.
 * @param[in, out] **PstateAcpiBufferPtr  Pointer to the Acpi Buffer Pointer.
 * @param[in]      StdHeader              Config Handle for library, services.
 *
 */
VOID
STATIC
F16MlCreateAcpiCstObj (
  IN       IO_CSTATE_FAMILY_SERVICES *IoCstateServices,
  IN       UINT8                     LocalApicId,
  IN OUT   VOID                      **PstateAcpiBufferPtr,
  IN       AMD_CONFIG_PARAMS         *StdHeader
  )
{
  UINT64                MsrData;
  BOOLEAN               GenerateCsdObj;
  CST_HEADER_STRUCT     *CstHeaderPtr;
  CST_BODY_STRUCT       *CstBodyPtr;
  CSD_HEADER_STRUCT     *CsdHeaderPtr;
  CSD_BODY_STRUCT       *CsdBodyPtr;
  IO_CSTATE_FAMILY_SERVICES    *FamilyServices;
  ACPI_CST_CREATE_INPUT  CstInput;

  CstInput.IoCstateServices = IoCstateServices;
  CstInput.LocalApicId = LocalApicId;
  CstInput.PstateAcpiBufferPtr = PstateAcpiBufferPtr;

  IDS_SKIP_HOOK (IDS_CST_CREATE, &CstInput, StdHeader) {
    // Read from MSR C0010073 to obtain CstateAddr
    LibAmdMsrRead (MSR_CSTATE_ADDRESS, &MsrData, StdHeader);

    // Typecast the pointer
    CstHeaderPtr = (CST_HEADER_STRUCT *) *PstateAcpiBufferPtr;

    // Set CST Header
    CstHeaderPtr->NameOpcode  = NAME_OPCODE;
    CstHeaderPtr->CstName_a__ = CST_NAME__;
    CstHeaderPtr->CstName_a_C = CST_NAME_C;
    CstHeaderPtr->CstName_a_S = CST_NAME_S;
    CstHeaderPtr->CstName_a_T = CST_NAME_T;

    // Typecast the pointer
    CstHeaderPtr++;
    CstBodyPtr = (CST_BODY_STRUCT *) CstHeaderPtr;

    // Set CST Body
    CstBodyPtr->PkgOpcode      = PACKAGE_OPCODE;
    CstBodyPtr->PkgLength      = CST_LENGTH;
    CstBodyPtr->PkgElements    = CST_NUM_OF_ELEMENTS;
    CstBodyPtr->BytePrefix     = BYTE_PREFIX_OPCODE;
    CstBodyPtr->Count          = CST_COUNT;
    CstBodyPtr->PkgOpcode2     = PACKAGE_OPCODE;
    CstBodyPtr->PkgLength2     = CST_PKG_LENGTH;
    CstBodyPtr->PkgElements2   = CST_PKG_ELEMENTS;
    CstBodyPtr->BufferOpcode   = BUFFER_OPCODE;
    CstBodyPtr->BufferLength   = CST_SUBPKG_LENGTH;
    CstBodyPtr->BufferElements = CST_SUBPKG_ELEMENTS;
    CstBodyPtr->BufferOpcode2  = BUFFER_OPCODE;
    CstBodyPtr->GdrOpcode      = GENERIC_REG_DESCRIPTION;
    CstBodyPtr->GdrLength      = CST_GDR_LENGTH;
    CstBodyPtr->AddrSpaceId    = GDR_ASI_SYSTEM_IO;
    CstBodyPtr->RegBitWidth    = 0x08;
    CstBodyPtr->RegBitOffset   = 0x00;
    CstBodyPtr->AddressSize    = GDR_ASZ_BYTE_ACCESS;
    CstBodyPtr->RegisterAddr   = ((CSTATE_ADDRESS_MSR *) &MsrData)->CstateAddr + 1;
    CstBodyPtr->EndTag         = 0x0079;
    CstBodyPtr->BytePrefix2    = BYTE_PREFIX_OPCODE;
    CstBodyPtr->Type           = CST_C2_TYPE;
    CstBodyPtr->WordPrefix     = WORD_PREFIX_OPCODE;
    CstBodyPtr->Latency        = 400;
    CstBodyPtr->DWordPrefix    = DWORD_PREFIX_OPCODE;
    CstBodyPtr->Power          = 0;

    CstBodyPtr++;
    //Update the pointer
    *PstateAcpiBufferPtr = CstBodyPtr;


  // Check whether CSD object should be generated
    GetFeatureServicesOfCurrentCore (&IoCstateFamilyServiceTable, (CONST VOID **)&FamilyServices, StdHeader);
    ASSERT (FamilyServices != NULL);
    GenerateCsdObj = FamilyServices->IsCsdObjGenerated (FamilyServices, StdHeader);

    if (GenerateCsdObj) {
      CsdHeaderPtr = (CSD_HEADER_STRUCT *) *PstateAcpiBufferPtr;

      // Set CSD Header
      CsdHeaderPtr->NameOpcode  = NAME_OPCODE;
      CsdHeaderPtr->CsdName_a__ = CST_NAME__;
      CsdHeaderPtr->CsdName_a_C = CST_NAME_C;
      CsdHeaderPtr->CsdName_a_S = CST_NAME_S;
      CsdHeaderPtr->CsdName_a_D = CSD_NAME_D;

      CsdHeaderPtr++;
      CsdBodyPtr = (CSD_BODY_STRUCT *) CsdHeaderPtr;

      // Set CSD Body
      CsdBodyPtr->PkgOpcode         = PACKAGE_OPCODE;
      CsdBodyPtr->PkgLength         = CSD_BODY_SIZE - 1;
      CsdBodyPtr->PkgElements       = 1;
      CsdBodyPtr->PkgOpcode2        = PACKAGE_OPCODE;
      CsdBodyPtr->PkgLength2        = CSD_BODY_SIZE - 4; // CSD_BODY_SIZE - Package() - Package Opcode
      CsdBodyPtr->PkgElements2      = 6;
      CsdBodyPtr->BytePrefix        = BYTE_PREFIX_OPCODE;
      CsdBodyPtr->NumEntries        = 6;
      CsdBodyPtr->BytePrefix2       = BYTE_PREFIX_OPCODE;
      CsdBodyPtr->Revision          = 0;
      CsdBodyPtr->DWordPrefix       = DWORD_PREFIX_OPCODE;
      CsdBodyPtr->Domain            = (LocalApicId & 0xFE) >> 1;
      CsdBodyPtr->DWordPrefix2      = DWORD_PREFIX_OPCODE;
      CsdBodyPtr->CoordType         = CSD_COORD_TYPE_HW_ALL;
      CsdBodyPtr->DWordPrefix3      = DWORD_PREFIX_OPCODE;
      CsdBodyPtr->NumProcessors     = 0x2;
      CsdBodyPtr->DWordPrefix4      = DWORD_PREFIX_OPCODE;
      CsdBodyPtr->Index             = 0x0;

      CsdBodyPtr++;

      // Update the pointer
      *PstateAcpiBufferPtr = CsdBodyPtr;
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Routine to check whether CSD object should be created.
 *
 * @param[in]      IoCstateServices      IO Cstate services.
 * @param[in]      StdHeader             Config Handle for library, services.
 *
 * @retval         TRUE                  CSD Object should be created.
 * @retval         FALSE                 CSD Object should not be created.
 *
 */
BOOLEAN
F16MlIsCsdObjGenerated (
  IN       IO_CSTATE_FAMILY_SERVICES *IoCstateServices,
  IN       AMD_CONFIG_PARAMS         *StdHeader
  )
{
  // CSD Object should only be created when there are two cores per compute unit
  if (GetComputeUnitMapping (StdHeader) == EvenCoresMapping) {
    return TRUE;
  }
  return FALSE;
}

CONST IO_CSTATE_FAMILY_SERVICES ROMDATA F16MlIoCstateSupport =
{
  0,
  (PF_IO_CSTATE_IS_SUPPORTED) CommonReturnTrue,
  F16MlInitializeIoCstate,
  F16MlGetAcpiCstObj,
  F16MlCreateAcpiCstObj,
  F16MlIsCsdObjGenerated
};
