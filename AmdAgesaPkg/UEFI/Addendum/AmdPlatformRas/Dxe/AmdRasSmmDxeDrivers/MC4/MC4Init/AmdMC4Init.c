/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD MC4 Initialization for RAS
 *
 * AGESA RAS Library to perform RAS initialization functionality
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI/Addendum/AmdPlatformRas/RAS
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 * ****************************************************************************
 * AMD Generic Encapsulated Software Architecture
 *
 * Description: AmdMC4Init.c - AGESA RAS Library to perform RAS initialization
 * functionality.
 *
 *****************************************************************************
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
#include "AmdMC4Init.h"
#include "CpuFuncs.h"
#include "EfiRuntimeLib.h"
#include "AmdApeiHest.h"

#include EFI_PROTOCOL_CONSUMER (SmmCpuState)
#include EFI_PROTOCOL_CONSUMER (SmmControl)
#include EFI_PROTOCOL_CONSUMER (PciRootBridgeIo)
#include EFI_PROTOCOL_CONSUMER (MpService)
#include EFI_PROTOCOL_CONSUMER (SmmBase)
#include EFI_PROTOCOL_CONSUMER (AmdRasApeiProtocol)
#include EFI_PROTOCOL_CONSUMER (AcpiSupport)

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
EFI_GUID    gEfiMpServicesProtocolGuid = EFI_MP_SERVICES_PROTOCOL_GUID;
EFI_GUID    gEfiPciRootBridgeIoProtocolGuid = EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_GUID;
EFI_GUID    gAmdRasApeiProtocolGuid = AMD_RAS_APEI_PROTOCOL_GUID;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
STATIC EFI_SMM_BASE_PROTOCOL      *mSmmBase;
EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *mPciRootBridgeIo;
UINT32                            mCfgMmioBase;
UINT8                             mNumNodes;
UINT8                             mCoresOnNode;
STATIC  EFI_SMM_CONTROL_PROTOCOL  *mSmmRtControl = NULL;
EFI_SMM_SYSTEM_TABLE              *mSmst = NULL;
EFI_SMM_CPU_SAVE_STATE_PROTOCOL   *mCpuSave;
EFI_HANDLE                        mImageHandle;
AMD_RAS_APEI_PROTOCOL             *mAmdApeiProtocol;
extern EFI_BOOT_SERVICES          *gBS;
CORR_MEM_ERR_STS_BLK              *mCrErrorBlk;
UNCORR_MEM_ERR_STS_BLK            *mUncrErrorBlk;


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
ConfigureTLVTEntry (
  IN       UINT8 Cpu,
  IN       VOID *Context
  );

VOID
ConfigureSmiTrigIoCylReg (
  VOID );

/**
 *---------------------------------------------------------------------------------------
 *
 *  UpdateGlobalVars
 *
 *  Description:
 *    This function loads global variables with CPU Save State data before launching other cores.
 *
 *  Parameters:
 *    @param[in]      VOID
 *
 *    @retval         EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
UpdateGlobalVars (
  IN       VOID
  )
{
  EFI_STATUS Status;
  UINT64 MsrVal;
  UINT64 MmioAddr;
  UINT32 MmioVal;
  UINTN Index;
  UINTN Handle;
  EFI_ACPI_DESCRIPTION_HEADER *Table;
  EFI_ACPI_TABLE_VERSION  Version;
  EFI_ACPI_SUPPORT_PROTOCOL *AcpiSupport;
  APEI_HEST_ACPI_TABLE *ApeiHestTable;

  Index = 0;
  MsrVal = EfiReadMsr (AMD_MSR_MMIO_CFG_BASE);
  mCfgMmioBase = (UINT32) MsrVal & 0xfff00000ul;

  Status  = gBS->LocateProtocol (
                                 &gEfiSmmBaseProtocolGuid,
                                 NULL,
                                 &mSmmBase
                                 );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  mSmmBase->GetSmstLocation (mSmmBase, &mSmst);

  // mCpuSave is acquired during Boot Services but is used during runtime in the SMM handler via extern
  Status  = gBS->LocateProtocol (
                                 &gEfiSmmCpuSaveStateProtocolGuid,
                                 NULL,
                                 &mCpuSave
                                 );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = gBS->LocateProtocol (
                                &gEfiAcpiSupportGuid,   // IN EFI_GUID Published unique identifier of requested protocol GUID to locate
                                NULL,                   // IN VOID* Published unique identifier of requested protocol GUID
                                (VOID **) &AcpiSupport  // OUT VOID** Returns address of pointer for ACPI protocol interface
                                );
  if (EFI_ERROR (Status)) {
    return Status;
  }


  do {
    Table  = NULL;
    Status = AcpiSupport->GetAcpiTable (
                                        AcpiSupport,
                                        Index,
                                        &Table,
                                        &Version,
                                        &Handle
                                        );
    if (EFI_ERROR (Status)) {
      break;
    }

    //
    // Check Signature and update HEST table
    //
    if (Table->Signature == 0x54534548ul) {
      ApeiHestTable = (APEI_HEST_ACPI_TABLE*) Table;
      mCrErrorBlk = (CORR_MEM_ERR_STS_BLK*) ApeiHestTable->ErrorSourceStruct.CorrGenErrSource.RegisterRegion.Address;
      mUncrErrorBlk = (UNCORR_MEM_ERR_STS_BLK*) ApeiHestTable->ErrorSourceStruct.UncorrGenErrSource.RegisterRegion.Address;
      break;
    }
    Index++;
  } while (TRUE);

  // Extended Config Space (Bus 0, D18,Func 0, Offset 0x60) [0x?00C0060]
  MmioAddr = mCfgMmioBase | ((0x18) << 15) | (0 << 12) | 0x60; //F0:60
  // Prepare to run task on AP to enable MC4

  // Read Extended Config Space [0x?00C0060]
  MmioVal = RasSmmReadMem32 (MmioAddr);

  // Get number of nodes in the system from Bits[6:4]
  mNumNodes = (UINT8) ((MmioVal >> 4) & 0x7) + 1;
  // Get the number of cores enabled on the *PLATFORM* from Bits[20:16]
  mCoresOnNode = (UINT8) (MmioVal >> 16) & 0x1F;

  // Extended Config Space (Bus 0, D18,Func 0, Offset 0x160) [0x?00C0160]
  MmioAddr |= 0x100;  //F0:160
  // Read Extended Config Space [0x?00C0160]
  MmioVal = RasSmmReadMem32 (MmioAddr);

  // Get Node count on CPU socket
  MmioVal = (MmioVal >> (16 - 5)) & 0xe0;
  //
  mCoresOnNode = (mCoresOnNode | (UINT8) MmioVal) + 1;
  mCoresOnNode = mCoresOnNode / mNumNodes;

  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  ConfigNbSmiProcessing
 *
 *  Description:
 *    Configure Northbridge SMM processing for each processor cores.
 *
 *  Parameters:
 *    @param[in]      VOID
 *
 *    @retval         VOID
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
ConfigNbSmiProcessing (
  IN       VOID
  )
{
  EFI_MP_SERVICES_PROTOCOL *MpServices;
  EFI_STATUS Status = EFI_SUCCESS;

  //Get MP Services Protocol
  Status = gBS->LocateProtocol (
                                &gEfiMpServicesProtocolGuid,
                                NULL,
                                &MpServices
                                );
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  // Locate the PCI Root Bridge IO Protocol
  Status = gBS->LocateProtocol (
                                &gEfiPciRootBridgeIoProtocolGuid,
                                NULL,
                                &mPciRootBridgeIo
                                );
  ASSERT_EFI_ERROR (Status);

  // Configure SMI Trigger IO cycle Register(MSRC001_0056) to enable the
  // local-event-triggered SMI message to generate software SMI (by issuing IO Cycle
  // to south bridge)(**Assuming CPU IoTrap is Not enabled)
  ConfigureSmiTrigIoCylReg();       // Execute on BSP first


  Status = MpServices->StartupAllAPs (
                                      MpServices,                                   // EFI_MP_SERVICES_PROTOCOL *this
                                      (EFI_AP_PROCEDURE) ConfigureSmiTrigIoCylReg,  // EFI_AP_PROCEDURE
                                      FALSE,                                        // BOOLEAN SingleThreaded? FALSE=execute all functions at the same time on all CPUs
                                      NULL,                                         // EFI_EVENT WaitEvent OPTIONAL
                                      0,                                            // UINTN Timeout (Unsupported)
                                      NULL,                                         // VOID *ProcArguments OPTIONAL
                                      NULL                                          // Failed CPUList OPTIONAL (unsupported)
                                      );

  if (Status != EFI_SUCCESS) {
    return Status;
  }

  return Status;
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  ConfigureTLVTEntry
 *
 *  Description:
 *    Configure LVT Thermal Sensor for Local Vector Table Entry(APIC330) to allow delivering
 *    of SMI type message.  Note: This code to be run on each processor core.
 *
 *  Parameters:
 *    @param[in]      Cpu
 *    @param[in]      *Context
 *
 *---------------------------------------------------------------------------------------
 **/
VOID ConfigureTLVTEntry (
  IN       UINT8 Cpu,
  IN       VOID *Context
  )
{
  UINT64 Address64;
  UINT32 Value32;

  Value32 = 0x200;       //Set Bits[10:8]=0x02 for SMI Type

  Address64 = APIC_BASE + LOCAL_APIC_TLVT;  //Local Apic Register 330
  mPciRootBridgeIo->Mem.Write (mPciRootBridgeIo, EfiPciWidthUint32, Address64, 1, &Value32);
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  ConfigureSmiTrigIoCylReg
 *
 *  Description:
 *    Configure SMI Trigger IO cycle Register(MSRC001_0056) to enable the
 *    local-event-triggered SMI message to generate software SMI (by issuing
 *    IO Cycle to south bridge)(**Assuming CPU IoTrap is Not enabled)
 *
 *  Parameters:
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
ConfigureSmiTrigIoCylReg (
   VOID
   )
{
  UINT64 MsrValue64;

  MsrValue64 = EfiReadMsr (AMD_MSR_SMI_TRIGGER_IO);

  // Has 16-bit [IoPortAddress] been programmed yet?
  if (!(MsrValue64 & 0xfffful)) {
    MsrValue64 = 0x02B300B0ul;          // Send AMD_DRAM_ECC_THRESHOLD_EXCEEDED(0xB3) to SW_SMI_COMMAND_PORT (0xB0)
    EfiWriteMsr (AMD_MSR_SMI_TRIGGER_IO, MsrValue64);
  } else {
    MsrValue64 = EfiReadMsr (AMD_MSR_SMI_TRIGGER_IO);  // Read requested MSR C001_0056 value
    MsrValue64 &= 0xFF00FFFFul;         // Mask off data byte
    MsrValue64 |= (DRAM_ECC_SWSMI_IO << 16);   // OR in DRAM_ECC_SWSMI_IO write data this value is available
    EfiWriteMsr (AMD_MSR_SMI_TRIGGER_IO, MsrValue64);
  }
}