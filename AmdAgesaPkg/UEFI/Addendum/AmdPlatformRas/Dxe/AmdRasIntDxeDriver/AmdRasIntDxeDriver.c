/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD RAS Interface for DXE Driver
 *
 * RAS Interface DXE Driver to perform RAS configuration tasks
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
 * Description: AmdRasIntDxeDriver.c - RAS Interface DXE Driver to perform
 *                                     RAS configuration tasks.
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
#include "AmdRasIntDxeDriver.h"
#include "CpuFuncs.h"
#include "EfiDriverLib.h"
#include EFI_PROTOCOL_DEFINITION (AmdRasInterfaceProtocol)
#include EFI_PROTOCOL_DEFINITION (MpService)
#include EFI_PROTOCOL_DEFINITION (CpuIo)

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
EFI_GUID  gEfiMpServiceProtocolGuid       = EFI_MP_SERVICES_PROTOCOL_GUID;
EFI_GUID  gAmdRasDxeInterfaceProtocolGuid = AMD_RAS_DXE_INTERFACE_PROTOCOL_GUID;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
RAS_NB_CORR_MEM_CONFIG    *mRasMemCorrErrConfig;
RAS_NB_UNCORR_MEM_CONFIG  *mRasMemUnCorrErrConfig;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
GetErrorConfigAll (
  IN       AMD_RAS_DXE_INTERFACE_PROTOCOL   *This,
  IN OUT   VOID                             *ConfigBuffer,
  IN OUT   UINTN                            SourceSize
  );

EFI_STATUS
EFIAPI
GetErrorConfigDevice (
  IN       AMD_RAS_DXE_INTERFACE_PROTOCOL  *This,
  IN       UINTN                           DeviceID,
  IN OUT   VOID                            *ConfigBuffer
  );

EFI_STATUS
EFIAPI
SetErrorConfigAll (
  IN       AMD_RAS_DXE_INTERFACE_PROTOCOL  *This,
  IN OUT   VOID                            *ConfigBuffer,
  IN       UINTN                           SourceSize
  );

EFI_STATUS
EFIAPI
SetErrorConfigDevice (
  IN       AMD_RAS_DXE_INTERFACE_PROTOCOL  *This,
  IN       UINTN                           DeviceID,
  IN OUT   VOID                            *ConfigBuffer
  );

VOID
EFIAPI
AmdRasReady2Boot (
  IN      EFI_EVENT  Event,
  IN      VOID       *Context
 );


EFI_STATUS
InitializeMemSmiLogic ( VOID );

EFI_STATUS
InitializeMemApicLogic ( VOID );

EFI_STATUS
InitNBMemSmiOnAP ( VOID );

EFI_STATUS
InitializeMemUnCorrLogic ( VOID );

EFI_STATUS
InitNBMemUnCorrOnAP ( VOID );


UINT32
ReadExtPci32 (
  IN       UINT8 Bus,
  IN       UINT8 Device,
  IN       UINT8 Func,
  IN       UINT16 Offset
  );

VOID
WriteExtPci32 (
  IN       UINT8 Bus,
  IN       UINT8 Device,
  IN       UINT8 Func,
  IN       UINT16 Offset,
  IN       UINT32 Value
  );


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

EFI_DRIVER_ENTRY_POINT (AmdRasIntDxeDriverEntryPoint);


/**
 *---------------------------------------------------------------------------------------
 *
 *  EfiInitializeRasIntLib
 *
 *  Description:
 *     Initialize global variables for RAS Interface Driver.
 *
 *  Parameters:
 *    @param[in]     ImageHandle
 *    @param[in]     *SystemTable
 *
 *    @retval         EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
EfiInitializeRasIntLib (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  gST = SystemTable;

  ASSERT (gST != NULL);

  gBS = gST->BootServices;
  gRT = gST->RuntimeServices;

  ASSERT (gBS != NULL);
  ASSERT (gRT != NULL);

  return EFI_SUCCESS;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  AgesaDxeDriverEntryPoint
 *
 *  Description:
 *     Entry point for the RAS Interface DXE driver.
 *
 *  Parameters:
 *    @param[in]     ImageHandle
 *    @param[in]     *SystemTable
 *
 *    @retval         EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/

EFI_STATUS
EFIAPI
AmdRasIntDxeDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                      Status = EFI_SUCCESS;
  EFI_EVENT                       ReadyToBootEvent;
  EFI_HANDLE                      Handle = NULL;
  AMD_RAS_DXE_INTERFACE_PROTOCOL  *RasInterfaceProtocol;

  //
  // Initialize Global Variable
  //
  EfiInitializeRasIntLib (ImageHandle, SystemTable);

  Status = gBS->AllocatePool (
                              EfiBootServicesData,
                              sizeof (AMD_RAS_DXE_INTERFACE_PROTOCOL),
                              &RasInterfaceProtocol
                              );

  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return EFI_OUT_OF_RESOURCES;  // Error detected while trying to locate SMM Base Protocol
  }

  EfiCommonLibZeroMem (RasInterfaceProtocol, sizeof (AMD_RAS_DXE_INTERFACE_PROTOCOL));

  // Initialize function pointers to protocol interfaces
  RasInterfaceProtocol->AmdSetErrorConfigAll = SetErrorConfigAll;
  RasInterfaceProtocol->AmdSetErrorConfigDevice = SetErrorConfigDevice;
  RasInterfaceProtocol->AmdGetErrorConfigAll = GetErrorConfigAll;
  RasInterfaceProtocol->AmdGetErrorConfigDevice = GetErrorConfigDevice;

  Status = gBS->InstallProtocolInterface (
                                          &Handle,                          // IN OUT EFI_HANDLE
                                          &gAmdRasDxeInterfaceProtocolGuid, // IN EFI_GUID
                                          EFI_NATIVE_INTERFACE,             // IN EFI_INITERFACE_TYPE
                                          RasInterfaceProtocol              // IN VOID* Interface
                                          );

  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return EFI_PROTOCOL_ERROR;   // Error detected while trying to locate SMM Base Protocol
  }

  //
  // Setup callback for AmdRasReady2Boot.
  //
  Status = gBS->CreateEventEx (
                               EFI_EVENT_NOTIFY_SIGNAL,
                               EFI_TPL_NOTIFY,
                               AmdRasReady2Boot,
                               NULL,
                               &gEfiEventReadyToBootGuid,
                               &ReadyToBootEvent
                               );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return EFI_OUT_OF_RESOURCES; // Error detected while trying to locate SMM Base Protocol
  }

  return EFI_SUCCESS;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  SetErrorConfigAll
 *
 *  Description:
 *     Set the error configuration for all RAS devices at one time.
 *
 *  Parameters:
 *    @param[in]         *This
 *    @param[in, out]    *ConfigBuffer
 *    @param[in]         SourceSize
 *
 *    @retval            EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
EFIAPI
SetErrorConfigAll (
  IN       AMD_RAS_DXE_INTERFACE_PROTOCOL  *This,
  IN OUT   VOID                            *ConfigBuffer,
  IN       UINTN                           SourceSize
  )
{
  EFI_STATUS           Status = EFI_SUCCESS;
  AMD_ERROR_CONFIG_ALL *RasSetErrConfigAll = NULL;

  RasSetErrConfigAll = (AMD_ERROR_CONFIG_ALL*) ConfigBuffer;

  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  SetErrorConfigDevice
 *
 *  Description:
 *     Set the error configuration for a single RAS device.
 *
 *  Parameters:
 *    @param[in]        *This
 *    @param[in]        DeviceID
 *    @param[in, out]   *ConfigBuffer
 *
 *    @retval           EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
EFIAPI
SetErrorConfigDevice (
  IN       AMD_RAS_DXE_INTERFACE_PROTOCOL  *This,
  IN       UINTN                           DeviceID,
  IN OUT   VOID                            *ConfigBuffer
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  // Check to make sure it is non-NULL
  if (ConfigBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Select RAS device to be configured
  switch (DeviceID) {
  case CPU_CORE:
    break;
  case NB_CORR_MEM:
    mRasMemCorrErrConfig = (RAS_NB_CORR_MEM_CONFIG*) ConfigBuffer;

    // Make sure user wants this RAS feature enabled
    if (mRasMemCorrErrConfig->DevCommon.Enable) {
      switch (mRasMemCorrErrConfig->DevCommon.ErrorMonitorType) {
      case SMI:
        Status = InitializeMemSmiLogic ();
        break;
      case APIC:
        Status = InitializeMemApicLogic ();
        break;
      default:
        break;
      }
    }
    break;
  case NB_UNCORR_MEM:
    mRasMemUnCorrErrConfig = (RAS_NB_UNCORR_MEM_CONFIG*) ConfigBuffer;

    // Make sure user wants this RAS feature enabled
    if (mRasMemUnCorrErrConfig->DevCommon.Enable) {
      switch (mRasMemUnCorrErrConfig->DevCommon.ErrorMonitorType) {
      case SYNC_FLOOD:
        Status = InitializeMemUnCorrLogic ();
        break;
      case NMI:
        Status = EFI_UNSUPPORTED;
        break;
      default:
        Status = EFI_UNSUPPORTED;
        break;
      }
    }
    break;
  case NB_CORR_L3_CACHE:
    Status = EFI_UNSUPPORTED;
    break;
  default:
    Status = EFI_UNSUPPORTED;
  } // End-- switch (DeviceID)

  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  GetErrorConfigAll
 *
 *  Description:
 *     Retrieve all the RAS configurations in a single buffer provided by the caller.
 *
 *  Parameters:
 *    @param[in]        *This
 *    @param[in, out]   *ConfigBuffer
 *    @param[in, out]   SourceSize
 *
 *    @retval           EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
EFIAPI
GetErrorConfigAll (
  IN       AMD_RAS_DXE_INTERFACE_PROTOCOL  *This,
  IN OUT   VOID                            *ConfigBuffer,
  IN OUT   UINTN                           SourceSize
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  if (ConfigBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  GetErrorConfigDevice
 *
 *  Description:
 *     Retrieve a RAS configurations device in a buffer provided by the caller.
 *
 *  Parameters:
 *    @param[in]        *This
 *    @param[in]        DeviceID
 *    @param[in, out]   *ConfigBuffer
 *
 *    @retval           EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
EFIAPI
GetErrorConfigDevice (
  IN       AMD_RAS_DXE_INTERFACE_PROTOCOL  *This,
  IN       UINTN                           DeviceID,
  IN OUT   VOID                            *ConfigBuffer
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;

  if (ConfigBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return Status;
}
/**
 *---------------------------------------------------------------------------------------
 *
 *  InitializeMemSmiLogic
 *
 *  Description:
 *     Initialize Runtime SMI Memory Logic
 *
 *  Parameters:
 *    @retval               EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
InitializeMemSmiLogic (
  VOID
  )
{
  EFI_MP_SERVICES_PROTOCOL *MpServices;
  EFI_STATUS                Status;
  UINT64                    MsrValue;
  UINT32                    MmioVal;
  UINT32                    TempVal;
  UINT32                    CfgMmioBase;
  UINT8                     CurrNode;
  UINT8                     NumNodes;
  UINT8                     CoresOnNode;
  UINTN                     CoreNum;

  MpServices = NULL;
  Status = EFI_SUCCESS;
  MsrValue = 0;
  MmioVal = 0;
  TempVal = 0;
  CfgMmioBase = 0;
  CurrNode = 0;
  NumNodes = 0;
  CoresOnNode = 0;
  CoreNum = 0;

  MsrValue = EfiReadMsr (AMD_MSR_MMIO_CFG_BASE);
  CfgMmioBase = (UINT32) MsrValue & 0xfff00000;

  // Read MSR
  MsrValue = EfiReadMsr (AMD_MSR_HWCR);
  // Set Bit[18] in Hardware Cntrl Register to enable MCA Misc Writes
  MsrValue = (MsrValue | ((UINT64) 1 << (UINT8) 18)); // MCAStsWrEn
  // Write MSR
  EfiWriteMsr (AMD_MSR_HWCR, MsrValue);

  // Config Interrupt reporting type and threshold count
  MsrValue = EfiReadMsr (MSR_MC4_MISC0);
  MsrValue &= 0xfff0f000ffffffff;

  // Enable threshold counter, Set interrupt type to SMI and threshold counter to 1
  //Example MsrVal |= 0x000c0ffe00000000; when thresholding limit set 0xFFE.
  MsrValue = (MsrValue | ((UINT64) mRasMemCorrErrConfig->ThreshLimit << (UINT8) 32));       // Set Threshold limit
  MsrValue = (MsrValue | ((UINT64) mRasMemCorrErrConfig->DevCommon.ErrorMonitorType << (UINT8) 49));  // Set interrupt type to SMI
  MsrValue = (MsrValue | ((UINT64) mRasMemCorrErrConfig->EnThreshCounter << (UINT8) 51));   // Enable threshold counter
  MsrValue = (MsrValue | ((UINT64) mRasMemCorrErrConfig->SetLockedBit << (UINT8) 61));      // Set Locked bit

  EfiWriteMsr (MSR_MC4_MISC0, MsrValue);

  // Now disable writes to Hardware Control Register
  MsrValue = EfiReadMsr (AMD_MSR_HWCR);
  MsrValue = (MsrValue &~((UINT64) 1 << (UINT8) 18)); // Clear MCAStsWrEn
  EfiWriteMsr (AMD_MSR_HWCR, MsrValue);

  // Extended Config Space (Bus 0, D18,Func 0, Offset 0x60)
  MmioVal = ReadExtPci32 (0, 0x18, 0x00, 0x60);
  TempVal = MmioVal;  // Save off data in temp var
  // Get the zero-based number of nodes in the system from Bits[6:4] and make it 1-based
  NumNodes = (UINT8) ((MmioVal >> 4) & 0x7) + 1;
  // Get the number of cores enabled on the *PLATFORM* from Bits[20:16]
  MmioVal = TempVal;  // Restore data from temp var
  CoresOnNode = (UINT8) (MmioVal >> 16) & 0x1F;

  // Extended Config Space (Bus 0, D18,Func 0, Offset 0x160) [0x?00C0160]
  MmioVal = ReadExtPci32 (0, 0x18, 0x00, 0x160);

  // Get the zero-based CPU socket count on the platform
  MmioVal = (MmioVal >> (16 - 5)) & 0xe0;
  // Calculate number of enabled cores on the platform and then make it 1-based
  CoresOnNode =  (CoresOnNode | (UINT8) MmioVal) + 1;
  CoresOnNode = CoresOnNode / NumNodes;

  // Enable MC4 on BSP
  MsrValue = EfiReadMsr (0x17b);
  EfiWriteMsr (0x17b, MsrValue | 0x10); //enable MC4 - NB

  //Get MP Services Protocol
  Status = gBS->LocateProtocol (
                                &gEfiMpServiceProtocolGuid, // IN EFI_GUID Published unique identifier of requested protocol GUID to locate
                                NULL,                       // IN VOID* Published unique identifier of requested protocol GUID
                                (VOID*)&MpServices          // OUT VOID** Returns address of pointer for protocol interface
                                );
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  for (CoreNum = CoresOnNode; CoreNum < (CoresOnNode * NumNodes); CoreNum += CoresOnNode) {
    // Launch each Core 0 on a given Node and run InitNBMemSmiOnAP routine on that core.
    Status = MpServices->StartupThisAP (
                                        MpServices,                         // IN EFI_MP_SERVICES_PROTOCOL* This
                                        (EFI_AP_PROCEDURE)InitNBMemSmiOnAP, // IN EFI_AP_PROCEDURE Procedure to be called once proc is launched
                                        CoreNum,                            // IN UINTN ProcessorNumber to be launched
                                        NULL,                               // IN EFI_EVENT WaitEvent OPTIONAL: If = NULL, then run in blocking mode - BSP waits for AP to finish
                                        0,                                  // IN UINTN TimeoutInMicroseconds
                                        NULL                                // IN VOID* ProcedureArgument OPTIONAL
                                        //NULL                              // OUT BOOLEAN* Finished OPTIONAL
                                        );
    if (Status != EFI_SUCCESS) {
      return Status;
    }
  }

  // Enable Parity, UECC, & ECC on each Node (*not Core*) in the system via alias MMIO registers
  for (CurrNode = 0; CurrNode < NumNodes; CurrNode++) {
    // Read MMIO Extended Config Space
    MmioVal = ReadExtPci32 (0, (0x18 + CurrNode), 0x03, 0x40);
    //Mask off applicable bit(s)
    MmioVal &= ~(1 << 18);
    MmioVal &= ~(1 << 0);

    // Set bits to desired value
    MmioVal |= (UINT32) mRasMemCorrErrConfig->EnMemParity << (UINT8) 18; // Enable Memory Parity Bit[18]
    MmioVal |= (UINT32) mRasMemCorrErrConfig->EnCorrECC << (UINT8) 0;    // Enable CECCEn Bit[0]
/// FIX (handle in uncorrectable mem config) MmioVal = (MmioVal | ((UINT64)1 << (UINT8)1)); // Enable UECCEn Bit[1]
    // Write MMIO Extended Config Space
    WriteExtPci32 (0, (0x18 + CurrNode), 0x03, 0x40, MmioVal);

  } // End-- for(CurrNode < NumNodes)

  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  InitializeMemSmiLogic
 *
 *  Description:
 *     Initialize Runtime SMI Memory Logic
 *
 *  Parameters:
 *    @retval               EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
InitializeMemApicLogic (
  VOID
  )
{
  EFI_MP_SERVICES_PROTOCOL *MpServices = NULL;
  EFI_STATUS               Status;
  UINT64                   MsrValue;
  UINT32                   MmioVal;
  UINT32                   TempVal;
  UINT32                   CfgMmioBase;
  UINT8                    CurrNode;
  UINT8                    NumNodes;
  UINT8                    CoresOnNode;
  UINTN                    CoreNum;

  MpServices = NULL;
  Status      = EFI_SUCCESS;
  MsrValue    = 0;
  MmioVal     = 0;
  TempVal     = 0;
  CfgMmioBase = 0;  // Extended Config Space Base address
  CurrNode    = 0;
  NumNodes    = 0;
  CoresOnNode = 0;
  CoreNum     = 0;

  MsrValue = EfiReadMsr (AMD_MSR_MMIO_CFG_BASE);
  CfgMmioBase = (UINT32) MsrValue & 0xfff00000;

  // Read MSR
  MsrValue = EfiReadMsr (AMD_MSR_HWCR);
  // Set Bit[18] in Hardware Cntrl Register to enable MCA Misc Writes
  MsrValue = (MsrValue | ((UINT64)1 << (UINT8)18)); // MCAStsWrEn
  // Write MSR
  EfiWriteMsr (AMD_MSR_HWCR, MsrValue);

  // Config Interrupt reporting type and threshold count
  MsrValue = EfiReadMsr (MSR_MC4_MISC0);
  MsrValue &= 0xfff0f000ffffffff;

  // Enable threshold counter, Set interrupt type to SMI and threshold counter to 1
  //Example MsrVal |= 0x000c0ffe00000000; when thresholding limit set 0xFFE.
  MsrValue = (MsrValue | ((UINT64) mRasMemCorrErrConfig->ThreshLimit << (UINT8) 32));                 // Set Threshold limit
  MsrValue = (MsrValue | ((UINT64) mRasMemCorrErrConfig->DevCommon.ErrorMonitorType << (UINT8) 49));  // Set interrupt type to APIC
  MsrValue = (MsrValue | ((UINT64) mRasMemCorrErrConfig->EnThreshCounter << (UINT8) 51));             // Enable threshold counter
  MsrValue = (MsrValue | ((UINT64) mRasMemCorrErrConfig->SetLockedBit << (UINT8) 61));                // Set Locked bit

  EfiWriteMsr (MSR_MC4_MISC0, MsrValue);

  // Now disable writes to Hardware Control Register
  MsrValue = EfiReadMsr (AMD_MSR_HWCR);
  MsrValue = (MsrValue &~((UINT64)1 << (UINT8) 18)); // Clear MCAStsWrEn
  EfiWriteMsr (AMD_MSR_HWCR, MsrValue);

  // Extended Config Space (Bus 0, D18,Func 0, Offset 0x60)
  MmioVal = ReadExtPci32 (0, 0x18, 0x00, 0x60);
  TempVal = MmioVal;  // Save off data in temp var
  // Get the zero-based number of nodes in the system from Bits[6:4] and make it 1-based
  NumNodes = (UINT8) ((MmioVal >> 4) & 0x7) + 1;
  // Get the number of cores enabled on the *PLATFORM* from Bits[20:16]
  MmioVal = TempVal;  // Restore data from temp var
  CoresOnNode = (UINT8) (MmioVal >> 16) & 0x1F;

  // Extended Config Space (Bus 0, D18,Func 0, Offset 0x160) [0x?00C0160]
  MmioVal = ReadExtPci32 (0, 0x18, 0x00, 0x160);

  // Get the zero-based CPU socket count on the platform
  MmioVal = (MmioVal >> (16 - 5)) & 0xe0;
  // Calculate number of enabled cores on the platform and then make it 1-based
  CoresOnNode =  (CoresOnNode | (UINT8) MmioVal) + 1;
  CoresOnNode = CoresOnNode / NumNodes;

  // Enable MC4 on BSP
  MsrValue = EfiReadMsr (0x17b);
  EfiWriteMsr (0x17b, MsrValue | 0x10); //enable MC4 - NB

  //Get MP Services Protocol
  Status = gBS->LocateProtocol (
                                &gEfiMpServiceProtocolGuid, // IN EFI_GUID Published unique identifier of requested protocol GUID to locate
                                NULL,                       // IN VOID* Published unique identifier of requested protocol GUID
                                (VOID*)&MpServices          // OUT VOID** Returns address of pointer for protocol interface
                                );
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  for (CoreNum = CoresOnNode; CoreNum < (CoresOnNode * NumNodes); CoreNum += CoresOnNode) {
    // Launch each Core 0 on a given Node and run InitNBMemSmiOnAP routine on that core.
    Status = MpServices->StartupThisAP (
                                        MpServices,                         // IN EFI_MP_SERVICES_PROTOCOL* This
                                        (EFI_AP_PROCEDURE)InitNBMemSmiOnAP, // IN EFI_AP_PROCEDURE Procedure to be called once proc is launched
                                        CoreNum,                            // IN UINTN ProcessorNumber to be launched
                                        NULL,                               // IN EFI_EVENT WaitEvent OPTIONAL: If = NULL, then run in blocking mode - BSP waits for AP to finish
                                        0,                                  // IN UINTN TimeoutInMicroseconds
                                        NULL                                // IN VOID* ProcedureArgument OPTIONAL
                                        //NULL                              // OUT BOOLEAN* Finished OPTIONAL
                                        );
    if (Status != EFI_SUCCESS) {
      return Status;
    }
  }

  // Enable Parity, UECC, & ECC on each Node (*not Core*) in the system via alias MMIO registers
  for (CurrNode = 0; CurrNode < NumNodes; CurrNode++) {
    // Read MMIO Extended Config Space
    MmioVal = ReadExtPci32 (0, (0x18 + CurrNode), 0x03, 0x40);
    //Mask off applicable bit(s)
    MmioVal &= ~(1 << 18);
    MmioVal &= ~(1 << 0);

    // Set bits to desired value
    MmioVal |= (UINT32) mRasMemCorrErrConfig->EnMemParity << (UINT8) 18; // Enable Memory Parity Bit[18]
    MmioVal |= (UINT32) mRasMemCorrErrConfig->EnCorrECC << (UINT8) 0;    // Enable CECCEn Bit[0]
    // Write MMIO Extended Config Space
    WriteExtPci32 (0, (0x18 + CurrNode), 0x03, 0x40, MmioVal);
  } // End-- for (CurrNode < NumNodes)

  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  ReadExtPci32
 *
 *  Description:
 *     Read 32-bit PCI Extended Config Space
 *
 *  Parameters:
 *    @param[in]        Bus
 *    @param[in]        Device
 *    @param[in]        Func
 *    @param[in]        Offset
 *
 *    @retval           UINT32
 *
 *---------------------------------------------------------------------------------------
 **/
UINT32
ReadExtPci32 (
  IN UINT8 Bus,
  IN UINT8 Device,
  IN UINT8 Func,
  IN UINT16 Offset
  )
{
  UINT32 CfgMmioBase;
  UINT64 MsrValue;
  UINT64 MmioAddr;

  CfgMmioBase = 0;
  MsrValue    = 0;
  MmioAddr    = 0;

  MsrValue = EfiReadMsr (AMD_MSR_MMIO_CFG_BASE);
  CfgMmioBase = (UINT32) MsrValue & 0xfff00000;

  // Read PCI extended config space via MMIO
  MmioAddr = CfgMmioBase | (Bus << 20) | (Device << 15) | (Func << 12) | Offset;

  return *(UINT32*) MmioAddr;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  ReadExtPci32
 *
 *  Description:
 *     Write 32-bit PCI Extended Config Space
 *
 *  Parameters:
 *    @param[in]        Bus
 *    @param[in]        Device
 *    @param[in]        Func
 *    @param[in]        Offset
 *    @param[in]        Value
 *
 *    @retval           VOID
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
WriteExtPci32 (
  IN UINT8 Bus,
  IN UINT8 Device,
  IN UINT8 Func,
  IN UINT16 Offset,
  IN UINT32 Value
  )
{
  UINT32 CfgMmioBase;
  UINT64 MsrValue;
  UINT64 MmioAddr;

  CfgMmioBase = 0;
  MsrValue = 0;
  MmioAddr = 0;

  MsrValue = EfiReadMsr (AMD_MSR_MMIO_CFG_BASE);
  CfgMmioBase = (UINT32) MsrValue & 0xfff00000;

  MmioAddr = CfgMmioBase | (Bus << 20) | (Device << 15) | (Func << 12) | Offset;

  // Write out PCI extended config space via MMIO
  *(UINT32*) MmioAddr = Value;

  return;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  InitNBMemSmiOnAP
 *
 *  Description:
 *     Initialize Northbridge SMI configuration on each additional AP that is started
 *
 *  Parameters:
 *    @retval           EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
InitNBMemSmiOnAP (
  VOID
  )
{
  EFI_STATUS   Status = EFI_SUCCESS;
  UINT64       MsrValue;

  // Read MSR0000_017B
  MsrValue = EfiReadMsr (0x17B);
  EfiWriteMsr (0x17b, MsrValue | 0x10); //Enable MC4 - NB

  // Enable MCA Status for Writing
  MsrValue = EfiReadMsr (AMD_MSR_HWCR);
  MsrValue = (MsrValue | ((UINT64) 1 << (UINT8) 18)); // MCAStsWrEn
  EfiWriteMsr (AMD_MSR_HWCR, MsrValue);

  MsrValue = EfiReadMsr (MSR_MC4_MISC0);

  // Mask off MSR data to be configured
  MsrValue &= 0xfff8f000ffffffff;
  // Enable threshold counter, Set interrupt type to SMI and threshold counter to 1
  //MsrVal |= 0x000c0ffe00000000;
  MsrValue |= ((UINT64) mRasMemCorrErrConfig->ThreshLimit << (UINT8) 32);                 // Set Threshold limit
  MsrValue |= ((UINT64) mRasMemCorrErrConfig->DevCommon.ErrorMonitorType << (UINT8) 49);  // Set interrupt type to SMI
  MsrValue |= ((UINT64) mRasMemCorrErrConfig->EnThreshCounter << (UINT8) 51);             // Enable threshold counter
  MsrValue |= ((UINT64) mRasMemCorrErrConfig->SetLockedBit << (UINT8) 61);                // Set Locked bit

  EfiWriteMsr (MSR_MC4_MISC0, MsrValue);

  // Disable MCA Status for Writing
  MsrValue = EfiReadMsr (AMD_MSR_HWCR);
  MsrValue = (MsrValue &~((UINT64) 1 << (UINT8) 18)); // MCAStsWrEn
  EfiWriteMsr (AMD_MSR_HWCR, MsrValue);

  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  InitializeMemUnCorrLogic
 *
 *  Description:
 *     Initialize Uncorrectable Memory Logic
 *
 *  Parameters:
 *    @retval               EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
InitializeMemUnCorrLogic (
  VOID
  )
{
  EFI_STATUS                 Status;
  UINT64                     MsrValue;
  UINT32                     MmioVal;
  UINT32                     TempVal;
  UINT32                     CfgMmioBase;
  UINT8                      CurrNode;
  UINT8                      NumNodes;
  UINT8                      CoresOnNode;

  Status = EFI_SUCCESS;
  MsrValue = 0;
  MmioVal = 0;
  TempVal = 0;
  CfgMmioBase = 0;
  CurrNode = 0;
  NumNodes = 0;
  CoresOnNode = 0;

  MsrValue = EfiReadMsr (AMD_MSR_MMIO_CFG_BASE);
  CfgMmioBase = (UINT32) MsrValue & 0xfff00000;

  // Extended Config Space (Bus 0, D18,Func 0, Offset 0x60)
  MmioVal = ReadExtPci32 (0, 0x18, 0x00, 0x60);
  TempVal = MmioVal;  // Save off data in temp var
  // Get the zero-based number of nodes in the system from Bits[6:4] and make it 1-based
  NumNodes = (UINT8) ((MmioVal >> 4) & 0x7) + 1;
  // Get the number of cores enabled on the *PLATFORM* from Bits[20:16]
  MmioVal = TempVal;  // Restore data from temp var
  CoresOnNode = (UINT8) (MmioVal >> 16) & 0x1F;

  // Extended Config Space (Bus 0, D18,Func 0, Offset 0x160) [0x?00C0160]
  MmioVal = ReadExtPci32 (0, 0x18, 0x00, 0x160);

  // Get the zero-based CPU socket count on the platform
  MmioVal = (MmioVal >> (16 - 5)) & 0xe0;
  // Calculate number of enabled cores on the platform and then make it 1-based
  CoresOnNode = (CoresOnNode | (UINT8) MmioVal) + 1;
  CoresOnNode = CoresOnNode / NumNodes;

  // Enable Parity, UECC, & ECC on each Node (*not Core*) in the system via alias MMIO registers
  for (CurrNode = 0; CurrNode < NumNodes; CurrNode++) {
    // Extended Config Space (Bus 0, D18,Func 0, Offset 0x60)
    MmioVal = ReadExtPci32 (0, (0x18 + CurrNode), 0x03, 0x40); //F3:40 MC4_CTL

    //Mask off applicable bit(s)
    MmioVal &= ~(1 << 18);

    // MC4_CTL |=  0x40003
    MmioVal |= (UINT32)mRasMemUnCorrErrConfig->EnMemParity << (UINT8) 18;   // Enable Memory Parity Bit[18]
//    MmioVal |= ((UINT32)mRasMemUnCorrErrConfig->EnCorrECC << (UINT8) 0)); // Enable CECCEn Bit[0]
///FIX (handle in uncorrectable mem config) MmioVal = (MmioVal | ((UINT64)1 << (UINT8)1)); // Enable UECCEn Bit[1]

    // Write MMIO Extended Config Space
    WriteExtPci32 (0, (0x18 + CurrNode), 0x03, 0x40, MmioVal);

    // UNCORRECTABLE ERROR SYNC-FLOOD CONFIGURATION
    // Extended Config Space (Bus 0, (D18 + CurrNode), Func 3, Offset 0x44)
    MmioVal = ReadExtPci32 (0, (0x18 + CurrNode), 0x03, 0x44); //F3:44 MCA_NB_CFG

    if (mRasMemUnCorrErrConfig->DevCommon.Enable) {
      //F3:44 MCA NB CFG
      MmioVal |= ((UINT64) 1 << (UINT8) 30); // Enable Sync-Flood On DRAM Parity Error Bit[30]
      MmioVal |= ((UINT64) 1 << (UINT8) 21); // Enable Sync-Flood On Any Error Bit[21]
      MmioVal |= ((UINT64) 1 << (UINT8) 2);  // Enable Sync-Flood On Uncorrectable ECC Bit[2]
    } else {
      //F3:44 MCA NB CFG &= ~0x200004; // Bit21 - SyncOnAnyErrEn, Bit2 - SyncOnUcEccEn
      MmioVal &= ~((UINT64) 1 << (UINT8) 30); // Disable Sync-Flood On DRAM Parity Error Bit[30]
      MmioVal &= ~((UINT64) 1 << (UINT8) 21); // Disable Sync-Flood On Any Error Bit[21]
      MmioVal &= ~((UINT64) 1 << (UINT8) 2);  // Disable Sync-Flood On Uncorrectable ECC Bit[2]
    }

    // Write MMIO Extended Config Space
    WriteExtPci32 (0, (0x18 + CurrNode), 0x03, 0x44, MmioVal);
  }

  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  InitNBMemUnCorrOnAP
 *
 *  Description:
 *     Initialize Northbridge Uncorrectable Error Configuration on each additional AP that is started
 *
 *  Parameters:
 *    @retval           EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
InitNBMemUnCorrOnAP (
  VOID
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT32      CurrNode;
  UINT64      MmioAddr;
  UINT32      MmioVal;
  UINT32      CfgMmioBase;

  Status      = EFI_SUCCESS;
  CurrNode    = 0;
  MmioAddr    = 0;
  MmioVal     = 0;
  CfgMmioBase = 0;

  // UNCORRECTABLE ERROR SYNC-FLOOD CONFIGURATION
  // MMIObase = Bus 0, Device 18,Func 3, Offset 0x44 [0x?00C3044]
  MmioAddr = CfgMmioBase | ((0x18 + CurrNode) << 15) | (3 << 12) | 0x44; //F3:44 MCA NB CFG

  // MCA NB CFG &= ~0x200004; // Bit21 - SyncOnAnyErrEn, Bit2 - SyncOnUcEccEn
  // Write Extended Config Space [0x?00C3044]
  *(UINT64*) MmioAddr = MmioVal;

  // Read Extended Config Space [0x?00C3044]
  MmioVal = *(UINT32*) MmioAddr;
  MmioVal = (MmioVal | ((UINT64) 1 << (UINT8) 21)); // Enable Sync-Flood On Any Error Bit[21]
  MmioVal = (MmioVal | ((UINT64) 1 << (UINT8) 2));  // Enable Sync-Flood On Uncorrectable ECC Bit[2]

  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  AmdRasReady2Boot
 *
 *  Description:
 *  Installs ???. This function gets called
 *  each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled
 *
 *
 *  Parameters:
 *    @param[in]            Event
 *    @param[in]            *Context
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
EFIAPI
AmdRasReady2Boot (
  IN EFI_EVENT      Event,
  IN VOID           *Context
  )
{

  //
  // Update Ready To Boot event config data here.
  //

  return;
}
