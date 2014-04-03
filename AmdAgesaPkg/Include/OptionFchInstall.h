/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of AGESA FCH support
 *
 * This file generates the defaults tables for AGESA FCH.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Core
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 */
/*********************************************************************************
;
; Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
;
; AMD is granting you permission to use this software (the Materials)
; pursuant to the terms and conditions of your Software License Agreement
; with AMD.  This header does *NOT* give you permission to use the Materials
; or any rights under AMD's intellectual property.  Your use of any portion
; of these Materials shall constitute your acceptance of those terms and
; conditions.  If you do not agree to the terms and conditions of the Software
; License Agreement, please do not use any portion of these Materials.
;
; CONFIDENTIALITY:  The Materials and all other information, identified as
; confidential and provided to you by AMD shall be kept confidential in
; accordance with the terms and conditions of the Software License Agreement.
;
; LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
; PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
; WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
; MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
; OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
; IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
; (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
; INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
; GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
; RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
; THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
; EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
; THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
;
; AMD does not assume any responsibility for any errors which may appear in
; the Materials or any other related information provided to you by AMD, or
; result from use of the Materials or any related information.
;
; You agree that you will not reverse engineer or decompile the Materials.
;
; NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
; further information, software, technical information, know-how, or show-how
; available to you.  Additionally, AMD retains the right to modify the
; Materials at any time, without notice, and is not obligated to provide such
; modified Materials to you.
;
; U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
; "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
; subject to the restrictions as set forth in FAR 52.227-14 and
; DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
; Government constitutes acknowledgement of AMD's proprietary rights in them.
;
; EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
; direct product thereof will be exported directly or indirectly, into any
; country prohibited by the United States Export Administration Act and the
; regulations thereunder, without the required authorization from the U.S.
; government nor will be used for any purpose prohibited by the same.
;*********************************************************************************/

#ifndef _OPTION_FCH_INSTALL_H_
#define _OPTION_FCH_INSTALL_H_

#include "AmdFch.h"

#ifndef  FCH_SUPPORT
  #define  FCH_SUPPORT   FALSE
#endif


/* ACPI block register offset definitions */
#define PM1_STATUS_OFFSET              0x00
#define PM1_ENABLE_OFFSET              0x02
#define PM_CONTROL_OFFSET              0x04
#define PM_TIMER_OFFSET                0x08
#define CPU_CONTROL_OFFSET             0x10
#define EVENT_STATUS_OFFSET            0x20
#define EVENT_ENABLE_OFFSET            0x24


#if  FCH_SUPPORT == TRUE
  /*
   * FCH subfunctions
   */
  #ifdef AGESA_ENTRY_INIT_RESET
    #if AGESA_ENTRY_INIT_RESET == TRUE
      extern FCH_TASK_ENTRY    FchInitResetHwAcpiP;
      extern FCH_TASK_ENTRY    FchInitResetHwAcpi;
      extern FCH_TASK_ENTRY    FchInitResetAb;
      extern FCH_TASK_ENTRY    FchInitResetSpi;
      extern FCH_TASK_ENTRY    FchInitResetGec;
      extern FCH_TASK_ENTRY    FchInitResetSata;
      extern FCH_TASK_ENTRY    FchInitResetLpc;
      extern FCH_TASK_ENTRY    FchInitResetPcib;
      extern FCH_TASK_ENTRY    FchInitResetPcie;
      extern FCH_TASK_ENTRY    FchInitResetGpp;
      extern FCH_TASK_ENTRY    FchInitAllinoneGpp;
      extern FCH_TASK_ENTRY    FchInitResetUsb;
      extern FCH_TASK_ENTRY    FchInitResetEhci;
      extern FCH_TASK_ENTRY    FchInitResetOhci;
      extern FCH_TASK_ENTRY    FchInitResetXhci;
      extern FCH_TASK_ENTRY    FchInitResetImc;
    #endif
  #endif

  #ifdef AGESA_ENTRY_INIT_ENV
    #if AGESA_ENTRY_INIT_ENV == TRUE
      extern FCH_TASK_ENTRY    FchInitEnvUsbXhci;
      extern FCH_TASK_ENTRY    FchInitEnvUsbOhci;
      extern FCH_TASK_ENTRY    FchInitEnvUsbEhci;
      extern FCH_TASK_ENTRY    FchInitEnvUsb;
      extern FCH_TASK_ENTRY    FchInitEnvAb;
      extern FCH_TASK_ENTRY    FchInitEnvGpp;
      extern FCH_TASK_ENTRY    FchInitEnvGppPhaseII;
      extern FCH_TASK_ENTRY    FchInitEnvPcie;
      extern FCH_TASK_ENTRY    FchInitEnvPcib;
      extern FCH_TASK_ENTRY    FchInitEnvHwAcpiP;
      extern FCH_TASK_ENTRY    FchInitEnvHwAcpi;
      extern FCH_TASK_ENTRY    FchInitEnvAbSpecial;
      extern FCH_TASK_ENTRY    FchInitEnvSpi;
      extern FCH_TASK_ENTRY    FchInitEnvGec;
      extern FCH_TASK_ENTRY    FchInitEnvSata;
      extern FCH_TASK_ENTRY    FchInitEnvIde;
      extern FCH_TASK_ENTRY    FchInitEnvSd;
      extern FCH_TASK_ENTRY    FchInitEnvIr;
      extern FCH_TASK_ENTRY    FchInitEnvAzalia;
      extern FCH_TASK_ENTRY    FchInitEnvHwm;
      extern FCH_TASK_ENTRY    FchInitEnvImc;
    #endif
  #endif

  #ifdef AGESA_ENTRY_INIT_MID
    #if AGESA_ENTRY_INIT_MID == TRUE
      extern FCH_TASK_ENTRY    FchInitMidHwm;
      extern FCH_TASK_ENTRY    FchInitMidAzalia;
      extern FCH_TASK_ENTRY    FchInitMidGec;
      extern FCH_TASK_ENTRY    FchInitMidSata;
      extern FCH_TASK_ENTRY    FchInitMidIde;
      extern FCH_TASK_ENTRY    FchInitMidAb;
      extern FCH_TASK_ENTRY    FchInitMidUsb;
      extern FCH_TASK_ENTRY    FchInitMidUsbEhci;
      extern FCH_TASK_ENTRY    FchInitMidUsbOhci;
      extern FCH_TASK_ENTRY    FchInitMidUsbXhci;
      extern FCH_TASK_ENTRY    FchInitMidImc;
    #endif
  #endif

  #ifdef AGESA_ENTRY_INIT_LATE
    #if AGESA_ENTRY_INIT_LATE == TRUE
      extern FCH_TASK_ENTRY    FchInitLateHwAcpi;
      extern FCH_TASK_ENTRY    FchInitLateSpi;
      extern FCH_TASK_ENTRY    FchInitLateGec;
      extern FCH_TASK_ENTRY    FchInitLateSata;
      extern FCH_TASK_ENTRY    FchInitLateIde;
      extern FCH_TASK_ENTRY    FchInitLatePcib;
      extern FCH_TASK_ENTRY    FchInitLateAb;
      extern FCH_TASK_ENTRY    FchInitLatePcie;
      extern FCH_TASK_ENTRY    FchInitLateGpp;
      extern FCH_TASK_ENTRY    FchInitLateUsb;
      extern FCH_TASK_ENTRY    FchInitLateUsbEhci;
      extern FCH_TASK_ENTRY    FchInitLateUsbOhci;
      extern FCH_TASK_ENTRY    FchInitLateUsbXhci;
      extern FCH_TASK_ENTRY    FchInitLateImc;
      extern FCH_TASK_ENTRY    FchInitLateAzalia;
      extern FCH_TASK_ENTRY    FchInitLateHwm;
    #endif
  #endif

  extern FCH_TASK_ENTRY    FchTaskDummy;
  extern FCH_TASK_ENTRY    FchGppHotplugSmiCallback;
  /* FCH Interface entries */
  extern  FCH_INIT     CommonFchInitStub;

  /* FCH Interface entries */
  #ifdef AGESA_ENTRY_INIT_RESET
    #if AGESA_ENTRY_INIT_RESET == TRUE
      extern  FCH_INIT      FchInitReset;
      extern  FCH_INIT      FchResetConstructor;

      #define FP_FCH_INIT_RESET              &FchInitReset
      #define FP_FCH_INIT_RESET_CONSTRUCT    &FchResetConstructor
    #else
      #define FP_FCH_INIT_RESET              &CommonFchInitStub
      #define FP_FCH_INIT_RESET_CONSTRUCT    &CommonFchInitStub
    #endif
  #endif

  #ifdef AGESA_ENTRY_INIT_ENV
    #if AGESA_ENTRY_INIT_ENV == TRUE
      extern  FCH_INIT      FchInitEnv;
      extern  FCH_INIT      FchEnvConstructor;

      #define FP_FCH_INIT_ENV                &FchInitEnv
      #define FP_FCH_INIT_ENV_CONSTRUCT      &FchEnvConstructor
    #else
      #define FP_FCH_INIT_ENV                &CommonFchInitStub
      #define FP_FCH_INIT_ENV_CONSTRUCT      &CommonFchInitStub
    #endif
  #endif

  #ifdef AGESA_ENTRY_INIT_MID
    #if AGESA_ENTRY_INIT_MID == TRUE
      extern  FCH_INIT      FchInitMid;
      extern  FCH_INIT      FchMidConstructor;

      #define FP_FCH_INIT_MID                &FchInitMid
      #define FP_FCH_INIT_MID_CONSTRUCT      &FchMidConstructor
    #else
      #define FP_FCH_INIT_MID                &CommonFchInitStub
      #define FP_FCH_INIT_MID_CONSTRUCT      &CommonFchInitStub
    #endif
  #endif

  #ifdef AGESA_ENTRY_INIT_LATE
    #if AGESA_ENTRY_INIT_LATE == TRUE
      extern  FCH_INIT      FchInitLate;
      extern  FCH_INIT      FchLateConstructor;

      #define FP_FCH_INIT_LATE               &FchInitLate
      #define FP_FCH_INIT_LATE_CONSTRUCT     &FchLateConstructor
    #else
      #define FP_FCH_INIT_LATE               &CommonFchInitStub
      #define FP_FCH_INIT_LATE_CONSTRUCT     &CommonFchInitStub
    #endif
  #endif

  /* FCH subcomponent build options */
  #undef  FCH_NO_HWACPI_SUPPORT
  #undef  FCH_NO_AB_SUPPORT
  #undef  FCH_NO_SPI_SUPPORT
  #undef  FCH_NO_GEC_SUPPORT
  #undef  FCH_NO_SATA_SUPPORT
  #undef  FCH_NO_IDE_SUPPORT
  #undef  FCH_NO_LPC_SUPPORT
  #undef  FCH_NO_PCIB_SUPPORT
  #undef  FCH_NO_PCIE_SUPPORT
  #undef  FCH_NO_GPP_SUPPORT
  #undef  FCH_NO_USB_SUPPORT
  #undef  FCH_NO_EHCI_SUPPORT
  #undef  FCH_NO_OHCI_SUPPORT
  #undef  FCH_NO_XHCI_SUPPORT
  #undef  FCH_NO_IMC_SUPPORT
  #undef  FCH_NO_SD_SUPPORT
  #undef  FCH_NO_IR_SUPPORT
  #undef  FCH_NO_AZALIA_SUPPORT
  #undef  FCH_NO_HWM_SUPPORT

  #define FCH_NO_GEC_SUPPORT        TRUE
  #define DFLT_RTC_WORKAROUND       FALSE
  #define DFLT_USB_PORT_WORKAROUND  FALSE
  // Following are determined by silicon characteristics
  #if (OPTION_FAMILY14H_FCH == TRUE)
    //#define FCH_NO_GEC_SUPPORT        TRUE
  #elif (OPTION_FAMILY15H_KV == TRUE)
    #define FCH_NO_GEC_SUPPORT        TRUE
  #elif (OPTION_FAMILY15H_CZ == TRUE)
    #define FCH_NO_GEC_SUPPORT        TRUE
    #define FCH_NO_OHCI_SUPPORT       TRUE
    #define FCH_NO_GPP_SUPPORT        TRUE
    #define FCH_NO_PCIB_SUPPORT       TRUE
    #define FCH_NO_PCIE_SUPPORT       TRUE
    #define FCH_NO_IDE_SUPPORT        TRUE
    #define FCH_NO_IR_SUPPORT         TRUE
  #elif (OPTION_FAMILY15H_TN == TRUE)
    //#define FCH_NO_GEC_SUPPORT        TRUE
  #elif (OPTION_FAMILY14H_ON == TRUE)
    #define FCH_NO_XHCI_SUPPORT        TRUE
  #elif (OPTION_FAMILY16H_MODEL_0x == TRUE)
    #define FCH_NO_GPP_SUPPORT        TRUE
    #define FCH_NO_PCIB_SUPPORT       TRUE
    #define FCH_NO_PCIE_SUPPORT       TRUE
    #undef DFLT_RTC_WORKAROUND
    #define DFLT_RTC_WORKAROUND       TRUE
    #undef DFLT_USB_PORT_WORKAROUND
    #define DFLT_USB_PORT_WORKAROUND  TRUE
  #elif (OPTION_FAMILY16H_MODEL_3x == TRUE)
    #define FCH_NO_GPP_SUPPORT        TRUE
    #define FCH_NO_PCIB_SUPPORT       TRUE
    #define FCH_NO_PCIE_SUPPORT       TRUE
    #define FCH_NO_IDE_SUPPORT        TRUE
//    #define FCH_NO_IR_SUPPORT         TRUE
    #define FCH_NO_OHCI_SUPPORT       TRUE
    #undef  FP_FCH_INIT_LATE
    #define FP_FCH_INIT_LATE          &CommonFchInitStub
  #else
    #error FCH_SUPPORT: No chip type selected.
  #endif

  #if (INSTALL_FCH_ALL_IN_ONE_GPP_SUPPORT == TRUE)
    #define FCH_NO_GPP_SUPPORT        TRUE
  #endif

  //
  // Installable blocks depending on build switches
  //
  #ifndef FCH_NO_HWACPI_SUPPORT
    #define BLOCK_HWACPI_SIZE           sizeof (FCH_ACPI)
    #define InstallFchInitResetHwAcpiP  &FchInitResetHwAcpiP
    #define InstallFchInitResetHwAcpi   &FchInitResetHwAcpi
    #define InstallFchInitEnvHwAcpiP    &FchInitEnvHwAcpiP
    #define InstallFchInitEnvHwAcpi     &FchInitEnvHwAcpi
    #define InstallFchInitMidHwAcpi     &FchTaskDummy
    #define InstallFchInitLateHwAcpi    &FchInitLateHwAcpi
  #else
    #define BLOCK_HWACPI_SIZE           0
    #define InstallFchInitResetHwAcpiP  &FchTaskDummy
    #define InstallFchInitResetHwAcpi   &FchTaskDummy
    #define InstallFchInitEnvHwAcpi     &FchTaskDummy
    #define InstallFchInitMidHwAcpi     &FchTaskDummy
    #define InstallFchInitLateHwAcpi    &FchTaskDummy
  #endif

  #ifndef FCH_NO_AB_SUPPORT
    #define BLOCK_AB_SIZE               sizeof (FCH_AB)
    #define InstallFchInitResetAb       &FchInitResetAb
    #define InstallFchInitEnvAb         &FchInitEnvAb
    #define InstallFchInitEnvAbS        &FchInitEnvAbSpecial
    #define InstallFchInitMidAb         &FchInitMidAb
    #define InstallFchInitLateAb        &FchInitLateAb
  #else
    #define BLOCK_AB_SIZE               0
    #define InstallFchInitResetAb       &FchTaskDummy
    #define InstallFchInitEnvAb         &FchTaskDummy
    #define InstallFchInitEnvAbS        &FchTaskDummy
    #define InstallFchInitMidAb         &FchTaskDummy
    #define InstallFchInitLateAb        &FchTaskDummy
  #endif

  #ifndef FCH_NO_SPI_SUPPORT
    #define BLOCK_SPI_SIZE              sizeof (FCH_SPI)
    #define InstallFchInitResetSpi      &FchInitResetSpi
    #define InstallFchInitEnvSpi        &FchInitEnvSpi
    #define InstallFchInitMidSpi        &FchTaskDummy
    #define InstallFchInitLateSpi       &FchInitLateSpi
  #else
    #define BLOCK_SPI_SIZE              0
    #define InstallFchInitResetSpi      &FchTaskDummy
    #define InstallFchInitEnvSpi        &FchTaskDummy
    #define InstallFchInitMidSpi        &FchTaskDummy
    #define InstallFchInitLateSpi       &FchTaskDummy
  #endif

  #ifndef FCH_NO_GEC_SUPPORT
    #define BLOCK_GEC_SIZE              sizeof (FCH_GEC)
    #define InstallFchInitResetGec      &FchInitResetGec
    #define InstallFchInitEnvGec        &FchInitEnvGec
    #define InstallFchInitMidGec        &FchInitMidGec
    #define InstallFchInitLateGec       &FchInitLateGec
  #else
    #define BLOCK_GEC_SIZE              0
    #define InstallFchInitResetGec      &FchTaskDummy
    #define InstallFchInitEnvGec        &FchTaskDummy
    #define InstallFchInitMidGec        &FchTaskDummy
    #define InstallFchInitLateGec       &FchTaskDummy
  #endif

  #ifndef FCH_NO_SATA_SUPPORT
    #define BLOCK_SATA_SIZE             sizeof (FCH_SATA)
    #define InstallFchInitResetSata     &FchInitResetSata
    #define InstallFchInitEnvSata       &FchInitEnvSata
    #define InstallFchInitMidSata       &FchInitMidSata
    #define InstallFchInitLateSata      &FchInitLateSata
  #else
    #define BLOCK_SATA_SIZE             0
    #define InstallFchInitResetSata     &FchTaskDummy
    #define InstallFchInitEnvSata       &FchTaskDummy
    #define InstallFchInitMidSata       &FchTaskDummy
    #define InstallFchInitLateSata      &FchTaskDummy
  #endif

  #ifndef FCH_NO_IDE_SUPPORT
    #define BLOCK_IDE_SIZE              sizeof (FCH_IDE)
    #define InstallFchInitResetIde      &FchTaskDummy
    #define InstallFchInitEnvIde        &FchInitEnvIde
    #define InstallFchInitMidIde        &FchInitMidIde
    #define InstallFchInitLateIde       &FchInitLateIde
  #else
    #define BLOCK_IDE_SIZE              0
    #define InstallFchInitResetIde      &FchTaskDummy
    #define InstallFchInitEnvIde        &FchTaskDummy
    #define InstallFchInitMidIde        &FchTaskDummy
    #define InstallFchInitLateIde       &FchTaskDummy
  #endif

  #ifndef FCH_NO_LPC_SUPPORT
    #define BLOCK_LPC_SIZE              sizeof (FCH_LPC)
    #define InstallFchInitResetLpc      &FchInitResetLpc
    #define InstallFchInitEnvLpc        &FchTaskDummy
    #define InstallFchInitMidLpc        &FchTaskDummy
    #define InstallFchInitLateLpc       &FchTaskDummy
  #else
    #define BLOCK_LPC_SIZE              0
    #define InstallFchInitResetLpc      &FchTaskDummy
    #define InstallFchInitEnvLpc        &FchTaskDummy
    #define InstallFchInitMidLpc        &FchTaskDummy
    #define InstallFchInitLateLpc       &FchTaskDummy
  #endif

  #ifndef FCH_NO_PCIB_SUPPORT
    #define BLOCK_PCIB_SIZE             sizeof (FCH_PCIB)
    #define InstallFchInitResetPcib     &FchInitResetPcib
    #define InstallFchInitEnvPcib       &FchInitEnvPcib
    #define InstallFchInitMidPcib       &FchTaskDummy
    #define InstallFchInitLatePcib      &FchInitLatePcib
  #else
    #define BLOCK_PCIB_SIZE             0
    #define InstallFchInitResetPcib     &FchTaskDummy
    #define InstallFchInitEnvPcib       &FchTaskDummy
    #define InstallFchInitMidPcib       &FchTaskDummy
    #define InstallFchInitLatePcib      &FchTaskDummy
  #endif

  #ifndef FCH_NO_PCIE_SUPPORT
    #define InstallFchInitResetPcie     &FchInitResetPcie
    #define InstallFchInitEnvPcie       &FchInitEnvPcie
    #define InstallFchInitMidPcie       &FchTaskDummy
    #define InstallFchInitLatePcie      &FchInitLatePcie
  #else
    #define InstallFchInitResetPcie     &FchTaskDummy
    #define InstallFchInitEnvPcie       &FchTaskDummy
    #define InstallFchInitMidPcie       &FchTaskDummy
    #define InstallFchInitLatePcie      &FchTaskDummy
  #endif

  #ifndef FCH_NO_GPP_SUPPORT
    #define BLOCK_GPP_SIZE              sizeof (FCH_GPP)
    #define InstallFchInitResetGpp      &FchInitResetGpp
    #define InstallFchInitAllinoneGPP   &FchTaskDummy
    #define InstallFchInitEnvGpp        &FchInitEnvGpp
    #define InstallFchInitEnvGppPhaseII &FchInitEnvGppPhaseII
    #define InstallFchInitMidGpp        &FchTaskDummy
    #define InstallFchInitLateGpp       &FchInitLateGpp
    #define InstallHpSmiCallback        &FchGppHotplugSmiCallback
  #else
    #define BLOCK_GPP_SIZE              0
    #define InstallFchInitResetGpp      &FchTaskDummy
    #if (INSTALL_FCH_ALL_IN_ONE_GPP_SUPPORT == TRUE)
      #define InstallFchInitAllinoneGPP &FchInitAllinoneGpp
    #else
      #define InstallFchInitAllinoneGPP &FchTaskDummy
    #endif
    #define InstallFchInitEnvGpp        &FchTaskDummy
    #define InstallFchInitEnvGppPhaseII &FchTaskDummy
    #define InstallFchInitMidGpp        &FchTaskDummy
    #define InstallFchInitLateGpp       &FchTaskDummy
    #if (INSTALL_FCH_ALL_IN_ONE_GPP_SUPPORT == TRUE)
      #define InstallHpSmiCallback      &FchGppHotplugSmiCallback
    #else
      #define InstallHpSmiCallback      &FchTaskDummy
    #endif
  #endif

  #ifndef FCH_NO_USB_SUPPORT
    #define BLOCK_USB_SIZE              sizeof (FCH_USB)
    #define InstallFchInitResetUsb      &FchInitResetUsb
    #define InstallFchInitEnvUsb        &FchInitEnvUsb
    #define InstallFchInitMidUsb        &FchInitMidUsb
    #define InstallFchInitLateUsb       &FchInitLateUsb
  #else
    #define BLOCK_USB_SIZE              0
    #define InstallFchInitResetUsb      &FchTaskDummy
    #define InstallFchInitEnvUsb        &FchTaskDummy
    #define InstallFchInitMidUsb        &FchTaskDummy
    #define InstallFchInitLateUsb       &FchTaskDummy
  #endif

  #ifndef FCH_NO_EHCI_SUPPORT
    #define InstallFchInitResetUsbEhci  &FchInitResetEhci
    #define InstallFchInitEnvUsbEhci    &FchInitEnvUsbEhci
    #define InstallFchInitMidUsbEhci    &FchInitMidUsbEhci
    #define InstallFchInitLateUsbEhci   &FchInitLateUsbEhci
  #else
    #define InstallFchInitResetUsbEhci  &FchTaskDummy
    #define InstallFchInitEnvUsbEhci    &FchTaskDummy
    #define InstallFchInitMidUsbEhci    &FchTaskDummy
    #define InstallFchInitLateUsbEhci   &FchTaskDummy
  #endif

  #ifndef FCH_NO_OHCI_SUPPORT
    #define InstallFchInitResetUsbOhci  &FchInitResetOhci
    #define InstallFchInitEnvUsbOhci    &FchInitEnvUsbOhci
    #define InstallFchInitMidUsbOhci    &FchInitMidUsbOhci
    #define InstallFchInitLateUsbOhci   &FchInitLateUsbOhci
  #else
    #define InstallFchInitResetUsbOhci  &FchTaskDummy
    #define InstallFchInitEnvUsbOhci    &FchTaskDummy
    #define InstallFchInitMidUsbOhci    &FchTaskDummy
    #define InstallFchInitLateUsbOhci   &FchTaskDummy
  #endif

  #ifndef FCH_NO_XHCI_SUPPORT
    #define InstallFchInitResetUsbXhci  &FchInitResetXhci
    #define InstallFchInitEnvUsbXhci    &FchInitEnvUsbXhci
    #define InstallFchInitMidUsbXhci    &FchInitMidUsbXhci
    #define InstallFchInitLateUsbXhci   &FchInitLateUsbXhci
  #else
    #define InstallFchInitResetUsbXhci  &FchTaskDummy
    #define InstallFchInitEnvUsbXhci    &FchTaskDummy
    #define InstallFchInitMidUsbXhci    &FchTaskDummy
    #define InstallFchInitLateUsbXhci   &FchTaskDummy
  #endif

  #ifndef FCH_NO_IMC_SUPPORT
    #define BLOCK_IMC_SIZE              sizeof (FCH_IMC)
    #define InstallFchInitResetImc      &FchInitResetImc
    #define InstallFchInitEnvImc        &FchInitEnvImc
    #define InstallFchInitMidImc        &FchInitMidImc
    #define InstallFchInitLateImc       &FchInitLateImc
  #else
    #define BLOCK_IMC_SIZE              0
    #define InstallFchInitResetImc      &FchTaskDummy
    #define InstallFchInitEnvImc        &FchTaskDummy
    #define InstallFchInitMidImc        &FchTaskDummy
    #define InstallFchInitLateImc       &FchTaskDummy
  #endif


  #ifndef FCH_NO_SD_SUPPORT
    #define BLOCK_SD_SIZE               sizeof (FCH_SD)
    #define InstallFchInitResetSd       &FchTaskDummy
    #define InstallFchInitEnvSd         &FchInitEnvSd
    #define InstallFchInitMidSd         &FchTaskDummy
    #define InstallFchInitLateSd        &FchTaskDummy
  #else
    #define BLOCK_SD_SIZE               0
    #define InstallFchInitResetSd       &FchTaskDummy
    #define InstallFchInitEnvSd         &FchTaskDummy
    #define InstallFchInitMidSd         &FchTaskDummy
    #define InstallFchInitLateSd        &FchTaskDummy
  #endif

  #ifndef FCH_NO_IR_SUPPORT
    #define BLOCK_IR_SIZE               sizeof (FCH_IR)
    #define InstallFchInitResetIr       &FchTaskDummy
    #define InstallFchInitEnvIr         &FchInitEnvIr
    #define InstallFchInitMidIr         &FchTaskDummy
    #define InstallFchInitLateIr        &FchTaskDummy
  #else
    #define BLOCK_IR_SIZE               0
    #define InstallFchInitResetIr       &FchTaskDummy
    #define InstallFchInitEnvIr         &FchTaskDummy
    #define InstallFchInitMidIr         &FchTaskDummy
    #define InstallFchInitLateIr        &FchTaskDummy
  #endif

  #ifndef FCH_NO_AZALIA_SUPPORT
    #define BLOCK_AZALIA_SIZE           sizeof (FCH_AZALIA)
    #define InstallFchInitResetAzalia   &FchTaskDummy
    #define InstallFchInitEnvAzalia     &FchInitEnvAzalia
    #define InstallFchInitMidAzalia     &FchInitMidAzalia
    #define InstallFchInitLateAzalia    &FchInitLateAzalia
  #else
    #define BLOCK_AZALIA_SIZE           0
    #define InstallFchInitResetAzalia   &FchTaskDummy
    #define InstallFchInitEnvAzalia     &FchTaskDummy
    #define InstallFchInitMidAzalia     &FchTaskDummy
    #define InstallFchInitLateAzalia    &FchTaskDummy
  #endif

  #ifndef FCH_NO_HWM_SUPPORT
    #define BLOCK_HWM_SIZE              sizeof (FCH_HWM)
    #define InstallFchInitResetHwm      &FchTaskDummy
    #define InstallFchInitEnvHwm        &FchTaskDummy
    #define InstallFchInitMidHwm        &FchTaskDummy
    #define InstallFchInitLateHwm       &FchInitLateHwm
  #else
    #define InstallFchInitResetHwm      &FchTaskDummy
    #define InstallFchInitEnvHwm        &FchTaskDummy
    #define InstallFchInitMidHwm        &FchTaskDummy
    #define InstallFchInitLateHwm       &FchTaskDummy
  #endif


  #define BLOCK_SMBUS_SIZE      sizeof (FCH_SMBUS)
  #define BLOCK_HPET_SIZE       sizeof (FCH_HPET)
  #define BLOCK_GCPU_SIZE       sizeof (FCH_GCPU)
  #define BLOCK_SDB_SIZE        sizeof (FCH_SERIALDB)
  #define BLOCK_MISC_SIZE       sizeof (FCH_MISC)


  // Optionally declare OEM hooks after each phase
  #ifndef FCH_INIT_RESET_HOOK
    #define InstallFchInitResetHook     FchTaskDummy
  #else
    #define InstallFchInitResetHook     OemFchInitResetHook
  #endif


  //
  // Define FCH build time options and configurations
  //
  #ifdef BLDCFG_SMBUS0_BASE_ADDRESS
    #define CFG_SMBUS0_BASE_ADDRESS           BLDCFG_SMBUS0_BASE_ADDRESS
  #else
    #define CFG_SMBUS0_BASE_ADDRESS           DFLT_SMBUS0_BASE_ADDRESS
  #endif

  #ifdef BLDCFG_SMBUS1_BASE_ADDRESS
    #define CFG_SMBUS1_BASE_ADDRESS           BLDCFG_SMBUS1_BASE_ADDRESS
  #else
    #define CFG_SMBUS1_BASE_ADDRESS           DFLT_SMBUS1_BASE_ADDRESS
  #endif

  #ifdef BLDCFG_SIO_PME_BASE_ADDRESS
    #define CFG_SIO_PME_BASE_ADDRESS          BLDCFG_SIO_PME_BASE_ADDRESS
  #else
    #define CFG_SIO_PME_BASE_ADDRESS          DFLT_SIO_PME_BASE_ADDRESS
  #endif

  #ifdef BLDCFG_ACPI_PM1_EVT_BLOCK_ADDRESS
    #define CFG_ACPI_PM1_EVT_BLOCK_ADDRESS    BLDCFG_ACPI_PM1_EVT_BLOCK_ADDRESS
  #else
    #define CFG_ACPI_PM1_EVT_BLOCK_ADDRESS    DFLT_ACPI_PM1_EVT_BLOCK_ADDRESS
  #endif
  #ifdef BLDCFG_ACPI_PM1_CNT_BLOCK_ADDRESS
    #define CFG_ACPI_PM1_CNT_BLOCK_ADDRESS    BLDCFG_ACPI_PM1_CNT_BLOCK_ADDRESS
  #else
    #define CFG_ACPI_PM1_CNT_BLOCK_ADDRESS    DFLT_ACPI_PM1_CNT_BLOCK_ADDRESS
  #endif
  #ifdef BLDCFG_ACPI_PM_TMR_BLOCK_ADDRESS
    #define CFG_ACPI_PM_TMR_BLOCK_ADDRESS     BLDCFG_ACPI_PM_TMR_BLOCK_ADDRESS
  #else
    #define CFG_ACPI_PM_TMR_BLOCK_ADDRESS     DFLT_ACPI_PM_TMR_BLOCK_ADDRESS
  #endif
  #ifdef BLDCFG_ACPI_CPU_CNT_BLOCK_ADDRESS
    #define CFG_ACPI_CPU_CNT_BLOCK_ADDRESS    BLDCFG_ACPI_CPU_CNT_BLOCK_ADDRESS
  #else
    #define CFG_ACPI_CPU_CNT_BLOCK_ADDRESS    DFLT_ACPI_CPU_CNT_BLOCK_ADDRESS
  #endif
  #ifdef BLDCFG_ACPI_GPE0_BLOCK_ADDRESS
    #define CFG_ACPI_GPE0_BLOCK_ADDRESS       BLDCFG_ACPI_GPE0_BLOCK_ADDRESS
  #else
    #define CFG_ACPI_GPE0_BLOCK_ADDRESS       DFLT_ACPI_GPE0_BLOCK_ADDRESS
  #endif


  #ifdef BLDCFG_WATCHDOG_TIMER_BASE
    #define CFG_WATCHDOG_TIMER_BASE           BLDCFG_WATCHDOG_TIMER_BASE
  #else
    #define CFG_WATCHDOG_TIMER_BASE           DFLT_WATCHDOG_TIMER_BASE_ADDRESS
  #endif

  #ifdef BLDCFG_ACPI_PMA_BLK_ADDRESS
    #define CFG_ACPI_PMA_CNTBLK_ADDRESS       BLDCFG_ACPI_PMA_BLK_ADDRESS
  #else
    #define CFG_ACPI_PMA_CNTBLK_ADDRESS       DFLT_ACPI_PMA_CNT_BLK_ADDRESS
  #endif

  #ifdef BLDCFG_SMI_CMD_PORT_ADDRESS
    #define CFG_SMI_CMD_PORT_ADDRESS          BLDCFG_SMI_CMD_PORT_ADDRESS
  #else
    #define CFG_SMI_CMD_PORT_ADDRESS          DFLT_SMI_CMD_PORT
  #endif

  #ifdef BLDCFG_ROM_BASE_ADDRESS
    #define CFG_SPI_ROM_BASE_ADDRESS          BLDCFG_ROM_BASE_ADDRESS
  #else
    #define CFG_SPI_ROM_BASE_ADDRESS          DFLT_SPI_BASE_ADDRESS
  #endif

  #ifdef BLDCFG_GEC_SHADOW_ROM_BASE
    #define CFG_GEC_SHADOW_ROM_BASE           BLDCFG_GEC_SHADOW_ROM_BASE
  #else
    #define CFG_GEC_SHADOW_ROM_BASE           DFLT_GEC_BASE_ADDRESS
  #endif

  #ifdef BLDCFG_HPET_BASE_ADDRESS
    #define CFG_HPET_BASE_ADDRESS           BLDCFG_HPET_BASE_ADDRESS
  #else
    #define CFG_HPET_BASE_ADDRESS           DFLT_HPET_BASE_ADDRESS
  #endif

  #ifdef BLDCFG_AZALIA_SSID
    #define CFG_AZALIA_SSID                 BLDCFG_AZALIA_SSID
  #else
    #define CFG_AZALIA_SSID                 DFLT_AZALIA_SSID
  #endif

  #ifdef BLDCFG_SMBUS_SSID
    #define CFG_SMBUS_SSID                  BLDCFG_SMBUS_SSID
  #else
    #define CFG_SMBUS_SSID                  DFLT_SMBUS_SSID
  #endif

  #ifdef BLDCFG_IDE_SSID
    #define CFG_IDE_SSID                    BLDCFG_IDE_SSID
  #else
    #define CFG_IDE_SSID                    DFLT_IDE_SSID
  #endif

  #ifdef BLDCFG_SATA_AHCI_SSID
    #define CFG_SATA_AHCI_SSID              BLDCFG_SATA_AHCI_SSID
  #else
    #define CFG_SATA_AHCI_SSID              DFLT_SATA_AHCI_SSID
  #endif

  #ifdef BLDCFG_SATA_IDE_SSID
    #define CFG_SATA_IDE_SSID               BLDCFG_SATA_IDE_SSID
  #else
    #define CFG_SATA_IDE_SSID               DFLT_SATA_IDE_SSID
  #endif

  #ifdef BLDCFG_SATA_RAID5_SSID
    #define CFG_SATA_RAID5_SSID             BLDCFG_SATA_RAID5_SSID
  #else
    #define CFG_SATA_RAID5_SSID             DFLT_SATA_RAID5_SSID
  #endif

  #ifdef BLDCFG_SATA_RAID_SSID
    #define CFG_SATA_RAID_SSID              BLDCFG_SATA_RAID_SSID
  #else
    #define CFG_SATA_RAID_SSID              DFLT_SATA_RAID_SSID
  #endif

  #ifdef BLDCFG_EHCI_SSID
    #define CFG_EHCI_SSID                   BLDCFG_EHCI_SSID
  #else
    #define CFG_EHCI_SSID                   DFLT_EHCI_SSID
  #endif

  #ifdef BLDCFG_OHCI_SSID
    #define CFG_OHCI_SSID                   BLDCFG_OHCI_SSID
  #else
    #define CFG_OHCI_SSID                   DFLT_OHCI_SSID
  #endif

  #ifdef BLDCFG_LPC_SSID
    #define CFG_LPC_SSID                    BLDCFG_LPC_SSID
  #else
    #define CFG_LPC_SSID                    DFLT_LPC_SSID
  #endif

  #ifdef BLDCFG_SD_SSID
    #define CFG_SD_SSID                     BLDCFG_SD_SSID
  #else
    #define CFG_SD_SSID                     DFLT_SD_SSID
  #endif

  #ifdef BLDCFG_XHCI_SSID
    #define CFG_XHCI_SSID                   BLDCFG_XHCI_SSID
  #else
    #define CFG_XHCI_SSID                   DFLT_XHCI_SSID
  #endif

  #ifdef BLDCFG_FCH_PORT80_BEHIND_PCIB
    #define CFG_FCH_PORT80_BEHIND_PCIB      BLDCFG_FCH_PORT80_BEHIND_PCIB
  #else
    #define CFG_FCH_PORT80_BEHIND_PCIB      DFLT_FCH_PORT80_BEHIND_PCIB
  #endif

  #ifdef BLDCFG_FCH_ENABLE_ACPI_SLEEP_TRAP
    #define CFG_FCH_ENABLE_ACPI_SLEEP_TRAP  BLDCFG_FCH_ENABLE_ACPI_SLEEP_TRAP
  #else
    #define CFG_FCH_ENABLE_ACPI_SLEEP_TRAP  DFLT_FCH_ENABLE_ACPI_SLEEP_TRAP
  #endif

  #ifdef BLDCFG_FCH_GPP_LINK_CONFIG
    #define CFG_FCH_GPP_LINK_CONFIG         BLDCFG_FCH_GPP_LINK_CONFIG
  #else
    #define CFG_FCH_GPP_LINK_CONFIG         DFLT_FCH_GPP_LINK_CONFIG
  #endif

  #ifdef BLDCFG_FCH_GPP_PORT0_PRESENT
    #define CFG_FCH_GPP_PORT0_PRESENT       BLDCFG_FCH_GPP_PORT0_PRESENT
  #else
    #define CFG_FCH_GPP_PORT0_PRESENT       DFLT_FCH_GPP_PORT0_PRESENT
  #endif

  #ifdef BLDCFG_FCH_GPP_PORT1_PRESENT
    #define CFG_FCH_GPP_PORT1_PRESENT       BLDCFG_FCH_GPP_PORT1_PRESENT
  #else
    #define CFG_FCH_GPP_PORT1_PRESENT       DFLT_FCH_GPP_PORT1_PRESENT
  #endif

  #ifdef BLDCFG_FCH_GPP_PORT2_PRESENT
    #define CFG_FCH_GPP_PORT2_PRESENT       BLDCFG_FCH_GPP_PORT2_PRESENT
  #else
    #define CFG_FCH_GPP_PORT2_PRESENT       DFLT_FCH_GPP_PORT2_PRESENT
  #endif

  #ifdef BLDCFG_FCH_GPP_PORT3_PRESENT
    #define CFG_FCH_GPP_PORT3_PRESENT       BLDCFG_FCH_GPP_PORT3_PRESENT
  #else
    #define CFG_FCH_GPP_PORT3_PRESENT       DFLT_FCH_GPP_PORT3_PRESENT
  #endif

  #ifdef BLDCFG_FCH_GPP_PORT0_HOTPLUG
    #define CFG_FCH_GPP_PORT0_HOTPLUG       BLDCFG_FCH_GPP_PORT0_HOTPLUG
  #else
    #define CFG_FCH_GPP_PORT0_HOTPLUG       DFLT_FCH_GPP_PORT0_HOTPLUG
  #endif

  #ifdef BLDCFG_FCH_GPP_PORT1_HOTPLUG
    #define CFG_FCH_GPP_PORT1_HOTPLUG       BLDCFG_FCH_GPP_PORT1_HOTPLUG
  #else
    #define CFG_FCH_GPP_PORT1_HOTPLUG       DFLT_FCH_GPP_PORT1_HOTPLUG
  #endif

  #ifdef BLDCFG_FCH_GPP_PORT2_HOTPLUG
    #define CFG_FCH_GPP_PORT2_HOTPLUG       BLDCFG_FCH_GPP_PORT2_HOTPLUG
  #else
    #define CFG_FCH_GPP_PORT2_HOTPLUG       DFLT_FCH_GPP_PORT2_HOTPLUG
  #endif

  #ifdef BLDCFG_FCH_GPP_PORT3_HOTPLUG
    #define CFG_FCH_GPP_PORT3_HOTPLUG       BLDCFG_FCH_GPP_PORT3_HOTPLUG
  #else
    #define CFG_FCH_GPP_PORT3_HOTPLUG       DFLT_FCH_GPP_PORT3_HOTPLUG
  #endif

  #ifdef BLDCFG_FCH_ESATA_PORT_BITMAP
    #define CFG_FCH_ESATA_PORT_BITMAP       BLDCFG_FCH_ESATA_PORT_BITMAP
  #else
    #define CFG_FCH_ESATA_PORT_BITMAP       0
  #endif

  #ifdef BLDCFG_FCH_IR_PIN_CONTROL
    #define CFG_FCH_IR_PIN_CONTROL          BLDCFG_FCH_IR_PIN_CONTROL
  #else
    #define CFG_FCH_IR_PIN_CONTROL          (BIT5 | BIT1 | BIT0)
  #endif

  #ifdef BLDCFG_FCH_SD_CLOCK_CONTROL
    #define CFG_FCH_SD_CLOCK_CONTROL        BLDCFG_FCH_SD_CLOCK_CONTROL
  #else
    #define CFG_FCH_SD_CLOCK_CONTROL        Sd50MhzTraceCableLengthWithinSixInches
  #endif

  #ifdef BLDCFG_FCH_SCI_MAP_LIST
    #define CFG_FCH_SCI_MAP_LIST            BLDCFG_FCH_SCI_MAP_LIST
  #else
    #define CFG_FCH_SCI_MAP_LIST            NULL
  #endif

  #ifdef BLDCFG_FCH_SATA_PHY_LIST
    #define CFG_FCH_SATA_PHY_LIST           BLDCFG_FCH_SATA_PHY_LIST
  #else
    #define CFG_FCH_SATA_PHY_LIST           NULL
  #endif

  #ifdef BLDCFG_FCH_GPIO_CONTROL_LIST
    #define CFG_FCH_GPIO_CONTROL_LIST       BLDCFG_FCH_GPIO_CONTROL_LIST
  #else
    #define CFG_FCH_GPIO_CONTROL_LIST       NULL
  #endif


  #ifdef AGESA_ENTRY_INIT_RESET
    #if AGESA_ENTRY_INIT_RESET == TRUE
      //
      // Define task list for InitReset phase
      //
      FCH_TASK_ENTRY ROMDATA  *FchInitResetTaskTable[] = {
        InstallFchInitResetHwAcpiP,
        InstallFchInitResetAb,
        InstallFchInitResetSpi,
        InstallFchInitResetGec,
        InstallFchInitResetHwAcpi,
        InstallFchInitResetSata,
        InstallFchInitResetLpc,
        InstallFchInitResetPcib,
        InstallFchInitResetPcie,
        InstallFchInitResetGpp,
        InstallFchInitAllinoneGPP,
        InstallFchInitResetUsb,
        InstallFchInitResetUsbEhci,
        InstallFchInitResetUsbOhci,
        InstallFchInitResetUsbXhci,
        InstallFchInitResetImc,
        NULL
      };
    #endif
  #endif

  #ifdef AGESA_ENTRY_INIT_ENV
    #if AGESA_ENTRY_INIT_ENV == TRUE
      //
      // Define task list for InitEnv phase
      //
      FCH_TASK_ENTRY ROMDATA  *FchInitEnvTaskTable[] = {
        InstallFchInitEnvHwAcpiP,
        InstallFchInitEnvPcib,
        InstallFchInitEnvPcie,
        InstallFchInitEnvGpp,
        InstallFchInitEnvIr,
        InstallFchInitEnvHwAcpi,
        InstallFchInitEnvSpi,
        InstallFchInitEnvSd,
        InstallFchInitEnvImc,
        InstallFchInitEnvUsb,
        InstallFchInitEnvUsbEhci,
        InstallFchInitEnvUsbOhci,
        InstallFchInitEnvUsbXhci,
        InstallFchInitEnvSata,
        InstallFchInitEnvIde,
        InstallFchInitEnvGec,
        InstallFchInitEnvAzalia,
        InstallFchInitEnvAb,
        InstallFchInitEnvHwm,
        InstallFchInitEnvGppPhaseII,
        InstallFchInitEnvAbS,
        NULL
      };
    #endif
  #endif


  #ifdef AGESA_ENTRY_INIT_MID
    #if AGESA_ENTRY_INIT_MID == TRUE
      //
      // Define task list for InitMid phase
      //
      FCH_TASK_ENTRY ROMDATA  *FchInitMidTaskTable[] = {
        InstallFchInitMidImc,
        InstallFchInitMidUsb,
        InstallFchInitMidUsbEhci,
        InstallFchInitMidUsbOhci,
        InstallFchInitMidUsbXhci,
        InstallFchInitMidSata,
        InstallFchInitMidIde,
        InstallFchInitMidGec,
        InstallFchInitMidAzalia,
        InstallFchInitMidHwm,
        NULL
      };
    #endif
  #endif

  #ifdef AGESA_ENTRY_INIT_LATE
    #if AGESA_ENTRY_INIT_LATE == TRUE
      //
      // Define task list for InitLate phase
      //
      FCH_TASK_ENTRY ROMDATA  *FchInitLateTaskTable[] = {
        InstallFchInitLatePcie,
        InstallFchInitLatePcib,
        InstallFchInitLateSpi,
        InstallFchInitLateUsb,
        InstallFchInitLateUsbEhci,
        InstallFchInitLateUsbOhci,
        InstallFchInitLateUsbXhci,
        InstallFchInitLateSata,
        InstallFchInitLateIde,
        InstallFchInitLateGec,
        InstallFchInitLateAzalia,
        InstallFchInitLateImc,
        InstallFchInitLateHwm,
        InstallFchInitLateGpp,
        InstallFchInitLateHwAcpi,
        NULL
      };
    #endif
  #endif


  #ifdef AGESA_ENTRY_INIT_ENV
    #if AGESA_ENTRY_INIT_ENV == TRUE
      //
      // Define task list for S3 resume before PCI phase
      //
      FCH_TASK_ENTRY ROMDATA  *FchInitS3EarlyTaskTable[] = {
        InstallFchInitEnvPcie,
        InstallFchInitEnvPcib,
        InstallFchInitEnvGpp,
        InstallFchInitEnvIr,
        InstallFchInitEnvHwAcpi,
        InstallFchInitEnvSpi,
        InstallFchInitEnvSd,
        InstallFchInitEnvUsb,
        InstallFchInitEnvUsbXhci,
        InstallFchInitEnvSata,
        InstallFchInitEnvIde,
        InstallFchInitEnvGec,
        InstallFchInitEnvAzalia,
        InstallFchInitEnvAb,
        InstallFchInitEnvGppPhaseII,
        InstallFchInitEnvAbS,
        NULL
      };
    #endif
  #endif

  #ifdef AGESA_ENTRY_INIT_LATE
    #if AGESA_ENTRY_INIT_LATE == TRUE
      //
      // Define task list for S3 resume after PCI phase
      //
      FCH_TASK_ENTRY ROMDATA  *FchInitS3LateTaskTable[] = {
        InstallFchInitLatePcie,
        InstallFchInitLatePcib,
        InstallFchInitLateSpi,
        InstallFchInitMidUsbEhci,
        InstallFchInitLateUsb,
        InstallFchInitLateUsbEhci,
        InstallFchInitLateUsbOhci,
        InstallFchInitLateUsbXhci,
        InstallFchInitMidSata,
        InstallFchInitMidIde,
        InstallFchInitMidGec,
        InstallFchInitMidAzalia,
        InstallFchInitLateSata,
        InstallFchInitLateIde,
        InstallFchInitLateHwAcpi,
        InstallFchInitLateGpp,
        InstallFchInitEnvHwm,
        InstallFchInitLateGpp,
        InstallFchInitLateHwm,
        NULL
      };
    #endif
  #endif
  FCH_TASK_ENTRY   *FchGppHotplugSmiCallbackPtr = InstallHpSmiCallback;


#else   // FCH_SUPPORT == FALSE
  /* FCH Interface entries */
  extern  FCH_INIT     CommonFchInitStub;

  #define FP_FCH_INIT_RESET              &CommonFchInitStub
  #define FP_FCH_INIT_RESET_CONSTRUCT    &CommonFchInitStub
  #define FP_FCH_INIT_ENV                &CommonFchInitStub
  #define FP_FCH_INIT_ENV_CONSTRUCT      &CommonFchInitStub
  #define FP_FCH_INIT_MID                &CommonFchInitStub
  #define FP_FCH_INIT_MID_CONSTRUCT      &CommonFchInitStub
  #define FP_FCH_INIT_LATE               &CommonFchInitStub
  #define FP_FCH_INIT_LATE_CONSTRUCT     &CommonFchInitStub

  #define CFG_SMBUS0_BASE_ADDRESS            0
  #define CFG_SMBUS1_BASE_ADDRESS            0
  #define CFG_SIO_PME_BASE_ADDRESS           0
  #define CFG_ACPI_PM1_EVT_BLOCK_ADDRESS     0
  #define CFG_ACPI_PM1_CNT_BLOCK_ADDRESS     0
  #define CFG_ACPI_PM_TMR_BLOCK_ADDRESS      0
  #define CFG_ACPI_CPU_CNT_BLOCK_ADDRESS     0
  #define CFG_ACPI_GPE0_BLOCK_ADDRESS        0
  #define CFG_SPI_ROM_BASE_ADDRESS           0
  #define CFG_WATCHDOG_TIMER_BASE            0
  #define CFG_HPET_BASE_ADDRESS              0
  #define CFG_SMI_CMD_PORT_ADDRESS           0
  #define CFG_ACPI_PMA_CNTBLK_ADDRESS        0
  #define CFG_GEC_SHADOW_ROM_BASE            0
  #define CFG_AZALIA_SSID                    0
  #define CFG_SMBUS_SSID                     0
  #define CFG_IDE_SSID                       0
  #define CFG_SATA_AHCI_SSID                 0
  #define CFG_SATA_IDE_SSID                  0
  #define CFG_SATA_RAID5_SSID                0
  #define CFG_SATA_RAID_SSID                 0
  #define CFG_EHCI_SSID                      0
  #define CFG_OHCI_SSID                      0
  #define CFG_LPC_SSID                       0
  #define CFG_SD_SSID                        0
  #define CFG_XHCI_SSID                      0
  #define CFG_FCH_PORT80_BEHIND_PCIB         0
  #define CFG_FCH_ENABLE_ACPI_SLEEP_TRAP     0
  #define CFG_FCH_GPP_LINK_CONFIG            0
  #define CFG_FCH_GPP_PORT0_PRESENT          0
  #define CFG_FCH_GPP_PORT1_PRESENT          0
  #define CFG_FCH_GPP_PORT2_PRESENT          0
  #define CFG_FCH_GPP_PORT3_PRESENT          0
  #define CFG_FCH_GPP_PORT0_HOTPLUG          0
  #define CFG_FCH_GPP_PORT1_HOTPLUG          0
  #define CFG_FCH_GPP_PORT2_HOTPLUG          0
  #define CFG_FCH_GPP_PORT3_HOTPLUG          0

  #define CFG_FCH_ESATA_PORT_BITMAP          0
  #define CFG_FCH_IR_PIN_CONTROL             0
  #define CFG_FCH_SD_CLOCK_CONTROL           0
  #define CFG_FCH_SCI_MAP_LIST               0
  #define CFG_FCH_SATA_PHY_LIST              0
  #define CFG_FCH_GPIO_CONTROL_LIST          0

#endif

#ifdef BLDOPT_RTC_WORKAROUND
  #undef  CFG_FCH_RTC_WORKAROUND
  #define CFG_FCH_RTC_WORKAROUND      BLDOPT_RTC_WORKAROUND
#else
  #undef  CFG_FCH_RTC_WORKAROUND
  #define CFG_FCH_RTC_WORKAROUND      DFLT_RTC_WORKAROUND
#endif

#ifdef BLDOPT_USB_PORT_WORKAROUND
  #undef  CFG_FCH_USB_PORT_DIS_WORKAROUND
  #define CFG_FCH_USB_PORT_DIS_WORKAROUND  BLDOPT_USB_PORT_WORKAROUND
#else
  #undef  CFG_FCH_USB_PORT_DIS_WORKAROUND
  #define CFG_FCH_USB_PORT_DIS_WORKAROUND  DFLT_USB_PORT_WORKAROUND
#endif

CONST BLDOPT_FCH_FUNCTION ROMDATA BldoptFchFunction = {
  FP_FCH_INIT_RESET,
  FP_FCH_INIT_RESET_CONSTRUCT,
  FP_FCH_INIT_ENV,
  FP_FCH_INIT_ENV_CONSTRUCT,
  FP_FCH_INIT_MID,
  FP_FCH_INIT_MID_CONSTRUCT,
  FP_FCH_INIT_LATE,
  FP_FCH_INIT_LATE_CONSTRUCT,
};

#endif  // _OPTION_FCH_INSTALL_H_
