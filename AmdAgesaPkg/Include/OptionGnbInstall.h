/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of build option: GNB
 *
 * Contains AMD AGESA install macros and test conditions. Output is the
 * defaults tables reflecting the User's build options selection.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Options
 * @e \$Revision: 284859 $   @e \$Date: 2014-02-14 15:47:07 -0600 (Fri, 14 Feb 2014) $
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
 *
 ***************************************************************************/

#ifndef _OPTION_GNB_INSTALL_H_
#define _OPTION_GNB_INSTALL_H_

#include "S3SaveState.h"
/*  This option is designed to be included into the platform solution install
 *  file. The platform solution install file will define the options status.
 *  Check to validate the definition
 */

//---------------------------------------------------------------------------------------------------
// Family installation
//---------------------------------------------------------------------------------------------------

#define GNB_TYPE_ML   FALSE

#if (OPTION_FAMILY16H_ML == TRUE)
  #undef  GNB_TYPE_ML
  #define GNB_TYPE_ML  TRUE
#endif

#if (GNB_TYPE_ML == TRUE)
//---------------------------------------------------------------------------------------------------
// Service installation
//---------------------------------------------------------------------------------------------------

  #include "Gnb.h"
  #include "GnbPcie.h"
  #include "GnbGfx.h"

  #define SERVICES_POINTER  NULL
  #if (GNB_TYPE_ML == TRUE)
    #include "GnbInitMLInstall.h"
  #endif
  GNB_SERVICE   *ServiceTable = SERVICES_POINTER;

//---------------------------------------------------------------------------------------------------
// Common BUILD options
// These Common Build options apply to all families
//---------------------------------------------------------------------------------------------------

  #ifndef CFG_IGFX_AS_PCIE_EP
    #define CFG_IGFX_AS_PCIE_EP                           TRUE
  #endif

  #ifndef CFG_GNB_PCIE_LINK_RECEIVER_DETECTION_POOLING
    #define CFG_GNB_PCIE_LINK_RECEIVER_DETECTION_POOLING  (60 * 1000)
  #endif

  #ifndef CFG_GNB_PCIE_LINK_L0_POOLING
    #define CFG_GNB_PCIE_LINK_L0_POOLING                  (60 * 1000)
  #endif

  #ifndef CFG_GNB_PCIE_LINK_GPIO_RESET_ASSERT_TIME
    #define CFG_GNB_PCIE_LINK_GPIO_RESET_ASSERT_TIME      (2 * 1000)
  #endif

  #ifndef CFG_GNB_PCIE_LINK_RESET_TO_TRAINING_TIME
    #define CFG_GNB_PCIE_LINK_RESET_TO_TRAINING_TIME      (2 * 1000)
  #endif

  #ifdef BLDCFG_PCIE_TRAINING_ALGORITHM
    #define CFG_GNB_PCIE_TRAINING_ALGORITHM               BLDCFG_PCIE_TRAINING_ALGORITHM
  #else
    #define CFG_GNB_PCIE_TRAINING_ALGORITHM               PcieTrainingStandard
  #endif

  #ifndef CFG_GNB_PCIE_POWERGATING_FLAGS
    #define CFG_GNB_PCIE_POWERGATING_FLAGS                0
  #endif

  #ifndef CFG_PCIE_ASPM_BLACK_LIST_ENABLE
    #define CFG_PCIE_ASPM_BLACK_LIST_ENABLE               TRUE
  #endif

  #ifndef CFG_MAX_PAYLOAD_ENABLE
    #define CFG_MAX_PAYLOAD_ENABLE                        TRUE
  #endif

  #ifndef CFG_GNB_IVRS_RELATIVE_ADDR_NAMES_SUPPORT
    #define CFG_GNB_IVRS_RELATIVE_ADDR_NAMES_SUPPORT      FALSE
  #endif

  #ifndef CFG_GNB_LOAD_REAL_FUSE
    #define  CFG_GNB_LOAD_REAL_FUSE                       TRUE
  #endif

  GNB_BUILD_OPTIONS ROMDATA GnbBuildOptions = {
    CFG_IGFX_AS_PCIE_EP,
    CFG_GNB_PCIE_LINK_RECEIVER_DETECTION_POOLING,
    CFG_GNB_PCIE_LINK_L0_POOLING,
    CFG_GNB_PCIE_LINK_GPIO_RESET_ASSERT_TIME,
    CFG_GNB_PCIE_LINK_RESET_TO_TRAINING_TIME,
    CFG_GNB_PCIE_TRAINING_ALGORITHM,
    CFG_GNB_PCIE_POWERGATING_FLAGS,
    CFG_PCIE_ASPM_BLACK_LIST_ENABLE,
    CFG_MAX_PAYLOAD_ENABLE,
    CFG_GNB_IVRS_RELATIVE_ADDR_NAMES_SUPPORT,
    CFG_ACPI_SET_OEM_ID,
    CFG_ACPI_SET_OEM_TABLE_ID,
    CFG_GNB_LOAD_REAL_FUSE
  };

//---------------------------------------------------------------------------------------------------
// Family-specific common BUILD options
// The build options are used in all families, but may be configured differently for different families
// CFG_XXXX will override default value if CFG_XXXX is defined
// CFG_XXXX_FM will override CFG_XXXX if CFG_XXXX_FM is defined (Where _FM is family, _TN, _KB, ...
// If neither CFG_XXXX_FM or CFG_XXXX is defined, then the family specific default will be defined
// within the family-specific definition areas below
//---------------------------------------------------------------------------------------------------

  #ifdef CFG_SCS_SUPPORT
    #ifndef CFG_SCS_SUPPORT_ML
      #define CFG_SCS_SUPPORT_ML                            CFG_SCS_SUPPORT
    #endif
  #endif

  #ifdef CFG_UMA_STEERING
    #ifndef CFG_UMA_STEERING_ML
      #define CFG_UMA_STEERING_ML                           CFG_UMA_STEERING
    #endif
  #endif

  #ifdef CFG_GMC_POWER_GATING
    #ifndef CFG_GMC_POWER_GATING_ML
      #define CFG_GMC_POWER_GATING_ML                       CFG_GMC_POWER_GATING
    #endif
  #endif

  #ifdef CFG_GMC_CLOCK_GATING
    #ifndef CFG_GMC_CLOCK_GATING_ML
      #define CFG_GMC_CLOCK_GATING_ML                      CFG_GMC_CLOCK_GATING
    #endif
  #endif

  #ifdef CFG_ORB_DYN_WAKE_ENABLE
    #ifndef CFG_ORB_DYN_WAKE_ENABLE_ML
      #define CFG_ORB_DYN_WAKE_ENABLE_ML                   CFG_ORB_DYN_WAKE_ENABLE
    #endif
  #endif

  #ifdef CFG_ORB_CLOCK_GATING_ENABLE
    #ifndef CFG_ORB_CLOCK_GATING_ENABLE_ML
      #define CFG_ORB_CLOCK_GATING_ENABLE_ML               CFG_ORB_CLOCK_GATING_ENABLE
    #endif
  #endif

  #ifdef CFG_IOMMU_L1_CLOCK_GATING_ENABLE
    #ifndef CFG_IOMMU_L1_CLOCK_GATING_ENABLE_ML
      #define CFG_IOMMU_L1_CLOCK_GATING_ENABLE_ML          CFG_IOMMU_L1_CLOCK_GATING_ENABLE
    #endif
  #endif

  #ifdef CFG_IOMMU_L2_CLOCK_GATING_ENABLE
    #ifndef CFG_IOMMU_L2_CLOCK_GATING_ENABLE_ML
      #define CFG_IOMMU_L2_CLOCK_GATING_ENABLE_ML          CFG_IOMMU_L2_CLOCK_GATING_ENABLE
    #endif
  #endif

  #ifdef CFG_LCLK_DEEP_SLEEP_EN
    #ifndef CFG_LCLK_DEEP_SLEEP_EN_ML
      #define CFG_LCLK_DEEP_SLEEP_EN_ML                    CFG_LCLK_DEEP_SLEEP_EN
    #endif
  #endif

  #ifdef CFG_LCLK_DPM_EN
    #ifndef CFG_LCLK_DPM_EN_ML
      #define CFG_LCLK_DPM_EN_ML                           CFG_LCLK_DPM_EN
    #endif
  #endif

  #ifdef CFG_IOC_LCLK_CLOCK_GATING_ENABLE
    #ifndef CFG_IOC_LCLK_CLOCK_GATING_ENABLE_ML
      #define CFG_IOC_LCLK_CLOCK_GATING_ENABLE_ML          CFG_IOC_LCLK_CLOCK_GATING_ENABLE
    #endif
  #endif

  #ifdef CFG_GNB_BAPM_SUPPORT
    #ifndef CFG_GNB_BAPM_SUPPORT_ML
      #define CFG_GNB_BAPM_SUPPORT_ML                      CFG_GNB_BAPM_SUPPORT
    #endif
  #endif

//---------------------------------------------------------------------------------------------------
// ML-specific common BUILD options
//---------------------------------------------------------------------------------------------------
  #if (GNB_TYPE_ML == TRUE)

    // Common configuration defaults for ML
    #ifndef CFG_SCS_SUPPORT_ML
      #define CFG_SCS_SUPPORT_ML                        TRUE
    #endif

    #ifndef CFG_UMA_STEERING_ML
      #define CFG_UMA_STEERING_ML                       DefaultGarlic
    #endif

    #ifndef CFG_GMC_POWER_GATING_ML
      #define CFG_GMC_POWER_GATING_ML                   GmcPowerGatingWithStutter
    #endif

    #ifndef CFG_GMC_CLOCK_GATING_ML
      #define CFG_GMC_CLOCK_GATING_ML                   TRUE
    #endif

    #ifndef CFG_ORB_DYN_WAKE_ENABLE_ML
      #define CFG_ORB_DYN_WAKE_ENABLE_ML                TRUE
    #endif

    #ifndef CFG_ORB_CLOCK_GATING_ENABLE_ML
      #define CFG_ORB_CLOCK_GATING_ENABLE_ML            TRUE
    #endif

    #ifndef CFG_IOMMU_L1_CLOCK_GATING_ENABLE_ML
      #define CFG_IOMMU_L1_CLOCK_GATING_ENABLE_ML       TRUE
    #endif

    #ifndef CFG_IOMMU_L2_CLOCK_GATING_ENABLE_ML
      #define CFG_IOMMU_L2_CLOCK_GATING_ENABLE_ML       TRUE
    #endif

    #ifndef CFG_LCLK_DEEP_SLEEP_EN_ML
      #define CFG_LCLK_DEEP_SLEEP_EN_ML                 TRUE
    #endif

    #ifndef CFG_LCLK_DPM_EN_ML
      #define CFG_LCLK_DPM_EN_ML                        TRUE
    #endif

    #ifndef CFG_IOC_LCLK_CLOCK_GATING_ENABLE_ML
      #define CFG_IOC_LCLK_CLOCK_GATING_ENABLE_ML       TRUE
    #endif

    #ifndef CFG_GNB_BAPM_SUPPORT_ML
      #define CFG_GNB_BAPM_SUPPORT_ML                   TRUE
    #endif

    #ifndef CFG_DCTDP_ENABLE_ML
      #define CFG_DCTDP_ENABLE_ML                   TRUE
    #endif

    // ML-only configuration defaults
    #ifndef CFG_GNB_LHTC_SUPPORT_ML
      #define CFG_GNB_LHTC_SUPPORT_ML                   TRUE
    #endif

    #ifndef CFG_SVI_REVISION_ML
        #define CFG_SVI_REVISION_ML                     2
    #endif

    #ifndef CFG_SAMU_PATCH_ENABLED_ML
      #define CFG_SAMU_PATCH_ENABLED_ML                 FALSE
    #endif

    #ifndef CFG_GNB_TDC_SUPPORT_ML
      #define CFG_GNB_TDC_SUPPORT_ML                    TRUE
    #endif

    #ifndef CFG_NATIVE_GEN1_PLL_ENABLE_ML
      #define CFG_NATIVE_GEN1_PLL_ENABLE_ML             TRUE
    #endif

    #ifndef CFG_PCIE_PHY_ISOLATION_SUPPORT_ML
      #define CFG_PCIE_PHY_ISOLATION_SUPPORT_ML         TRUE
    #endif

    #ifndef CFG_LINK_BW_NOTIFICATION_EN_ML
      #define CFG_LINK_BW_NOTIFICATION_EN_ML            TRUE
    #endif

    #ifndef CFG_BATTERY_BOOST_EN_ML
      #define CFG_BATTERY_BOOST_EN_ML                   TRUE
    #endif

    #ifndef CFG_SPG_CLOCK_GATING_ML
      #define CFG_SPG_CLOCK_GATING_ML                   TRUE
    #endif

    #ifndef CFG_PSP_DPM_ENABLE_ML
      #define CFG_PSP_DPM_ENABLE_ML                     TRUE
    #endif

    #ifndef CFG_SMU_SERVICE_BIT_MAP_ML
      #define CFG_SMU_SERVICE_BIT_MAP_ML                0xFFFFFFFFul
    #endif

    #ifndef CFG_GNB_NUM_DISPLAY_STREAM_PIPES
      #define  CFG_GNB_NUM_DISPLAY_STREAM_PIPES         2
    #endif

    GNB_BUILD_OPTIONS_ML ROMDATA GnbBuildOptionsML = {
      {                                                 // GNB_BUILD_OPTIONS_COMMON
        CFG_SCS_SUPPORT_ML,
        CFG_UMA_STEERING_ML,
        CFG_GMC_POWER_GATING_ML,
        CFG_GMC_CLOCK_GATING_ML,
        CFG_ORB_DYN_WAKE_ENABLE_ML,
        CFG_ORB_CLOCK_GATING_ENABLE_ML,
        CFG_IOMMU_L1_CLOCK_GATING_ENABLE_ML,
        CFG_IOMMU_L2_CLOCK_GATING_ENABLE_ML,
        FALSE, //CFG_LCLK_DEEP_SLEEP_EN_ML,
        FALSE, //CFG_LCLK_DPM_EN_ML,
        CFG_IOC_LCLK_CLOCK_GATING_ENABLE_ML,
        FALSE, //CFG_GNB_BAPM_SUPPORT_ML,
        CFG_DCTDP_ENABLE_ML,
        CFG_GNB_NUM_DISPLAY_STREAM_PIPES
      },
      CFG_GNB_LHTC_SUPPORT_ML,
      CFG_SVI_REVISION_ML,
      CFG_SAMU_PATCH_ENABLED_ML,
      CFG_GNB_TDC_SUPPORT_ML,
      CFG_NATIVE_GEN1_PLL_ENABLE_ML,
      CFG_PCIE_PHY_ISOLATION_SUPPORT_ML,
      CFG_LINK_BW_NOTIFICATION_EN_ML,
      CFG_BATTERY_BOOST_EN_ML,
      CFG_SPG_CLOCK_GATING_ML,
      CFG_PSP_DPM_ENABLE_ML,
      CFG_SMU_SERVICE_BIT_MAP_ML,
      TRUE,      //Always use SMU services unless recovery mode is detected
    };
  #endif

  //---------------------------------------------------------------------------------------------------
  // Module entries
  //---------------------------------------------------------------------------------------------------

  #if (AGESA_ENTRY_INIT_EARLY == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_NB_EARLY_INIT
      #define OPTION_NB_EARLY_INIT TRUE
    #endif
    #if (OPTION_NB_EARLY_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  GnbEarlyInterfaceML;
      #define OPTION_GNBEARLYINTERFACEML_ENTRY            {AMD_FAMILY_ML, GnbEarlyInterfaceML, TpGnbEarlyInterface},
    #else
      #define OPTION_GNBEARLYINTERFACEML_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_CONFIG_MAP
      #define OPTION_PCIE_CONFIG_MAP TRUE
    #endif
    #if (OPTION_PCIE_CONFIG_MAP == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                 PcieConfigurationMap;
      #define OPTION_PCIECONFIGURATIONMAP_ENTRY          {AMD_FAMILY_ML, PcieConfigurationMap, TpGnbPcieConfigurationMap},
    #else
      #define OPTION_PCIECONFIGURATIONMAP_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_EARLY_INIT
      #define OPTION_PCIE_EARLY_INIT TRUE
    #endif
    #if (OPTION_PCIE_EARLY_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  PcieEarlyInterfaceML;
      #define OPTION_PCIEEARLYINTERFACEML_ENTRY           {AMD_FAMILY_ML, PcieEarlyInterfaceML, TpGnbPcieEarlyInterface},
    #else
      #define OPTION_PCIEEARLYINTERFACEML_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    OPTION_GNB_CONFIGURATION  GnbEarlyFeatureTable[] = {
      OPTION_GNBEARLYINTERFACEML_ENTRY
      OPTION_PCIECONFIGURATIONMAP_ENTRY
      OPTION_PCIEEARLYINTERFACEML_ENTRY
      {0, NULL, EndGnbTestPoints}
    };

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_CONFIG_INIT
      #define OPTION_PCIE_CONFIG_INIT TRUE
    #endif
    #if (OPTION_PCIE_CONFIG_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  PcieConfigurationInit;
      #define OPTION_PCIECONFIGURATIONINIT_ENTRY          {AMD_FAMILY_ML, PcieConfigurationInit, TpGnbEarlierPcieConfigurationInit},
    #else
      #define OPTION_PCIECONFIGURATIONINIT_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_NB_EARLIER_INIT
      #define OPTION_NB_EARLIER_INIT TRUE
    #endif
    #if (OPTION_NB_EARLIER_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  GnbEarlierInterfaceML;
      #define OPTION_GNBEARLIERINTERFACEML_ENTRY          {AMD_FAMILY_ML, GnbEarlierInterfaceML, TpGnbEarlierInterface},
    #else
      #define OPTION_GNBEARLIERINTERFACEML_ENTRY
    #endif

    #if (OPTION_NB_EARLIER_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  GnbScsInterfaceML;
      #define OPTION_GNBSCSINTERFACEML_ENTRY          {AMD_FAMILY_ML, GnbScsInterfaceML, TpGnbEarlierInterface},
    #else
      #define OPTION_GNBSCSINTERFACEML_ENTRY
    #endif


    OPTION_GNB_CONFIGURATION  GnbEarlierFeatureTable[] = {
      OPTION_PCIECONFIGURATIONINIT_ENTRY
      OPTION_GNBEARLIERINTERFACEML_ENTRY
      OPTION_GNBSCSINTERFACEML_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if (AGESA_ENTRY_INIT_POST == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_CONFIG_POST_INIT
      #define OPTION_GFX_CONFIG_POST_INIT TRUE
    #endif
    #if (OPTION_GFX_CONFIG_POST_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  GfxConfigPostInterface;
      #define OPTION_GFXCONFIGPOSTINTERFACE_ENTRY         {AMD_FAMILY_ML, GfxConfigPostInterface, TpGnbGfxConfigPostInterface},
    #else
      #define OPTION_GFXCONFIGPOSTINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_POST_INIT
      #define OPTION_GFX_POST_INIT TRUE
    #endif
    #if (OPTION_GFX_POST_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  GfxPostInterfaceML;
      #define OPTION_GFXPOSTINTERFACEML_ENTRY             {AMD_FAMILY_ML, GfxPostInterfaceML, TpGnbGfxPostInterface},
    #else
      #define OPTION_GFXPOSTINTERFACEML_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_NB_POST_INIT
      #define OPTION_NB_POST_INIT TRUE
    #endif
    #if (OPTION_NB_POST_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  GnbPostInterfaceML;
      #define OPTION_GNBPOSTINTERFACEML_ENTRY             {AMD_FAMILY_ML, GnbPostInterfaceML, TpGnbPostInterface},
    #else
      #define OPTION_GNBPOSTINTERFACEML_ENTRY
    #endif

    //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_POST_EARLY_INIT
      #define OPTION_PCIE_POST_EARLY_INIT TRUE
    #endif
    #if (OPTION_PCIE_POST_EARLY_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  PciePostEarlyInterfaceML;
      #define OPTION_PCIEPOSTEARLYINTERFACEML_ENTRY       {AMD_FAMILY_ML, PciePostEarlyInterfaceML, TpGnbPciePostEarlyInterface},
    #else
      #define OPTION_PCIEPOSTEARLYINTERFACEML_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_POST_INIT
      #define OPTION_PCIE_POST_INIT TRUE
    #endif
    #if (OPTION_PCIE_POST_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  PciePostInterfaceML;
      #define OPTION_PCIEPOSTINTERFACEML_ENTRY            {AMD_FAMILY_ML, PciePostInterfaceML, TpGnbPciePostInterface},
    #else
      #define OPTION_PCIEPOSTINTERFACEML_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    OPTION_GNB_CONFIGURATION  GnbPostFeatureTable[] = {
      OPTION_PCIEPOSTEARLYINTERFACEML_ENTRY
      OPTION_GFXCONFIGPOSTINTERFACE_ENTRY
      OPTION_GFXPOSTINTERFACEML_ENTRY
      {0, NULL, EndGnbTestPoints}
    };

    OPTION_GNB_CONFIGURATION  GnbPostAfterDramFeatureTable[] = {
      OPTION_GNBPOSTINTERFACEML_ENTRY
      OPTION_PCIEPOSTINTERFACEML_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if (AGESA_ENTRY_INIT_ENV == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_NB_ENV_INIT
      #define OPTION_NB_ENV_INIT TRUE
    #endif
    #if (OPTION_NB_ENV_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  GnbEnvInterfaceML;
      #define OPTION_GNBENVINTERFACEML_ENTRY              {AMD_FAMILY_ML, GnbEnvInterfaceML, TpGnbEnvInterface},
    #else
      #define OPTION_GNBENVINTERFACEML_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_CONFIG_ENV_INIT
      #define OPTION_GFX_CONFIG_ENV_INIT TRUE
    #endif
    #if (OPTION_GFX_CONFIG_ENV_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                   GfxConfigEnvInterface;
      #define OPTION_GFXCONFIGENVINTERFACE_ENTRY          {AMD_FAMILY_ML, GfxConfigEnvInterface, TpGnbGfxConfigEnvInterface},
    #else
      #define  OPTION_GFXCONFIGENVINTERFACE_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_ENV_INIT
      #define OPTION_GFX_ENV_INIT TRUE
    #endif
    #if (OPTION_GFX_ENV_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  GfxEnvInterfaceML;
      #define OPTION_GFXENVINTERFACEML_ENTRY              {AMD_FAMILY_ML, GfxEnvInterfaceML, TpGnbGfxEnvInterface},
    #else
      #define OPTION_GFXENVINTERFACEML_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_ENV_INIT
      #define OPTION_PCIE_ENV_INIT TRUE
    #endif
    #if (OPTION_PCIE_ENV_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  PcieEnvInterfaceML;
      #define OPTION_PCIEENVINTERFACEML_ENTRY             {AMD_FAMILY_ML, PcieEnvInterfaceML, TpGnbPcieEnvInterface},
    #else
      #define OPTION_PCIEENVINTERFACEML_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------

    OPTION_GNB_CONFIGURATION  GnbEnvFeatureTable[] = {
      OPTION_GNBENVINTERFACEML_ENTRY
      OPTION_PCIEENVINTERFACEML_ENTRY
      OPTION_GFXCONFIGENVINTERFACE_ENTRY
      OPTION_GFXENVINTERFACEML_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if (AGESA_ENTRY_INIT_MID == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_MID_INIT
      #define OPTION_GFX_MID_INIT TRUE
    #endif
    #if (OPTION_GFX_MID_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  GfxMidInterfaceML;
      #define OPTION_GFXMIDINTERFACEML_ENTRY              {AMD_FAMILY_ML, GfxMidInterfaceML, TpGnbGfxMidInterface},
    #else
      #define OPTION_GFXMIDINTERFACEML_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_INTEGRATED_TABLE_INIT
      #define OPTION_GFX_INTEGRATED_TABLE_INIT TRUE
    #endif
    #if (OPTION_GFX_INTEGRATED_TABLE_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  GfxIntInfoTableInterfaceML;
      #define OPTION_GFXINTINFOTABLEINTERFACEML_ENTRY     {AMD_FAMILY_ML, GfxIntInfoTableInterfaceML},
    #else
      #define OPTION_GFXINTINFOTABLEINTERFACEML_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIe_MID_INIT
      #define OPTION_PCIe_MID_INIT TRUE
    #endif
    #if (OPTION_PCIe_MID_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  PcieMidInterfaceML;
      #define OPTION_PCIEMIDINTERFACEML_ENTRY             {AMD_FAMILY_ML, PcieMidInterfaceML, TpPcieMidInterface},
    #else
      #define OPTION_PCIEMIDINTERFACEML_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_NB_MID_INIT
      #define OPTION_NB_MID_INIT TRUE
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_CONFIG_POST_INIT
      #define OPTION_GFX_CONFIG_POST_INIT TRUE
    #endif
    #if (OPTION_GFX_CONFIG_POST_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  GfxConfigMidInterface;
      #define OPTION_GFXCONFIGMIDINTERFACE_ENTRY         {AMD_FAMILY_ML, GfxConfigMidInterface, TpGnbGfxConfigMidInterface},
    #else
      #define OPTION_GFXCONFIGMIDINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #if (OPTION_NB_MID_INIT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  GnbMidInterfaceML;
      #define OPTION_GNBMIDINTERFACEML_ENTRY              {AMD_FAMILY_ML, GnbMidInterfaceML, TpGnbMidInterface},
    #else
      #define OPTION_GNBMIDINTERFACEML_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_MAXPAYLOAD_INTERFACE
      #define OPTION_PCIE_MAXPAYLOAD_INTERFACE TRUE
    #endif
    #if (OPTION_PCIE_MAXPAYLOAD_INTERFACE == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  PcieMaxPayloadInterface;
      #define OPTION_PCIEMAXPAYLOADINTERFACE_ENTRY        {AMD_FAMILY_ML, PcieMaxPayloadInterface, TpGnbPcieMaxPayloadInterface},
    #else
      #define OPTION_PCIEMAXPAYLOADINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_CLK_PM_INTERFACE
      #define OPTION_PCIE_CLK_PM_INTERFACE TRUE
    #endif
    #if ((OPTION_PCIE_CLK_PM_INTERFACE == TRUE) && (GNB_TYPE_ML == TRUE))
      OPTION_GNB_FEATURE                                  PcieClkPmInterface;
      #define OPTION_PCIECLKPMINTERFACE_ENTRY             {AMD_FAMILY_ML, PcieClkPmInterface, TpGnbPcieClkPmInterface},
    #else
      #define OPTION_PCIECLKPMINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_ASPM_INTERFACE
      #define OPTION_PCIE_ASPM_INTERFACE TRUE
    #endif
    #if ((OPTION_PCIE_ASPM_INTERFACE == TRUE) && (GNB_TYPE_ML == TRUE))
      OPTION_GNB_FEATURE                                  PcieAspmInterface;
      #define OPTION_PCIEASPMINTERFACE_ENTRY              {AMD_FAMILY_ML, PcieAspmInterface, TpGnbPcieAspmInterface},
    #else
      #define OPTION_PCIEASPMINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GNB_IOAPIC_INTERFACE
      #define OPTION_GNB_IOAPIC_INTERFACE TRUE
    #endif
    #if (OPTION_GNB_IOAPIC_INTERFACE == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  GnbNbIoapicInterface;
      #define OPTION_GNBNBIOAPICINTERFACE_ENTRY          {AMD_FAMILY_ML, GnbNbIoapicInterface, TpGnbNbIoapicInterface},
    #else
      #define OPTION_GNBNBIOAPICINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_PROGRAM_COMMON_CLOCK_INTERFACE
      #define OPTION_PCIE_PROGRAM_COMMON_CLOCK_INTERFACE TRUE
    #endif
    #if (OPTION_PCIE_PROGRAM_COMMON_CLOCK_INTERFACE == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  PcieCommClkCfgInterface;
      #define OPTION_PCIECOMMCLKCFGINTERFACE_ENTRY              {AMD_FAMILY_ML, PcieCommClkCfgInterface, TpGnbPcieClkPmInterface},
    #else
      #define OPTION_PCIECOMMCLKCFGINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    OPTION_GNB_CONFIGURATION  GnbMidFeatureTable[] = {
      OPTION_GFXCONFIGMIDINTERFACE_ENTRY
      OPTION_GFXMIDINTERFACEML_ENTRY
      OPTION_GFXINTINFOTABLEINTERFACEML_ENTRY
      OPTION_PCIEMIDINTERFACEML_ENTRY
      OPTION_GNBMIDINTERFACEML_ENTRY
      OPTION_PCIEMAXPAYLOADINTERFACE_ENTRY
      OPTION_PCIECLKPMINTERFACE_ENTRY
      OPTION_PCIEASPMINTERFACE_ENTRY
      OPTION_GNBNBIOAPICINTERFACE_ENTRY
      OPTION_PCIECOMMCLKCFGINTERFACE_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if (AGESA_ENTRY_INIT_LATE == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_ALIB
      #define OPTION_ALIB FALSE
    #endif
    #if ((OPTION_ALIB == TRUE) && (GNB_TYPE_ML == TRUE))
      #define ALIB_CALL_TABLE

      F_ALIB_UPDATE* AlibDispatchTable [] = {
        ALIB_CALL_TABLE
        NULL
      };

      #if (GNB_TYPE_ML == TRUE)
        OPTION_GNB_FEATURE_V2     PcieAlibUpdateVoltageData;
        #define OPTION_ALIB_PCIEUPDATEVOLTAGEDATA     {AMD_FAMILY_ML, PcieAlibUpdateVoltageData, TpGnbAlibDispatchFeature},
        OPTION_GNB_FEATURE_V2     PcieAlibUpdatePcieData;
        #define OPTION_ALIB_PCIEUPDATEPCIEDATA     {AMD_FAMILY_ML, PcieAlibUpdatePcieData, TpGnbAlibDispatchFeature},
      #else
        #define OPTION_ALIB_PCIEUPDATEVOLTAGEDATA
        #define OPTION_ALIB_PCIEUPDATEPCIEDATA
      #endif

      OPTION_GNB_CONFIGURATION_V2 AlibDispatchTableV2 [] = {
        OPTION_ALIB_PCIEUPDATEVOLTAGEDATA
        OPTION_ALIB_PCIEUPDATEPCIEDATA
        {0, NULL, EndGnbTestPoints}
      };

      #if ((GNB_TYPE_ML == TRUE))
        OPTION_GNB_FEATURE                                  PcieAlibV2Feature;
        #define OPTION_PCIEALIBV2FEATURE_ENTRY              {AMD_FAMILY_ML, PcieAlibV2Feature, TpGnbPcieAlibFeature},
      #endif
    #else
      F_ALIB_GET  *AlibGetBaseTable = NULL;
      F_ALIB_UPDATE* AlibDispatchTable [] = {
        NULL
      };
      OPTION_GNB_CONFIGURATION_V2 AlibDispatchTableV2 [] = {
        {0, NULL, EndGnbTestPoints}
      };
      #define OPTION_PCIEALIBFEATURE_ENTRY
      #define OPTION_PCIEALIBV2FEATURE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_IOMMU_ACPI_IVRS
      #if (CFG_IOMMU_SUPPORT == TRUE)
        #define OPTION_IOMMU_ACPI_IVRS TRUE
      #else
        #define OPTION_IOMMU_ACPI_IVRS FALSE
      #endif
    #endif
    #if (OPTION_IOMMU_ACPI_IVRS == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  GnbIommuIvrsTable;
      #define OPTIONIOMMUACPIIVRSLATE_ENTRY               {AMD_FAMILY_ML, GnbIommuIvrsTable},
    #else
      #define OPTIONIOMMUACPIIVRSLATE_ENTRY
    #endif
    #if (CFG_IOMMU_SUPPORT == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  GnbIommuScratchMemoryRangeInterfaceML;
      #define OPTIONIOMMUSCRATCHMEMORYLATEML_ENTRY          {AMD_FAMILY_ML, GnbIommuScratchMemoryRangeInterfaceML, TpGnbIommuIvrsTable},
    #else
      #define OPTIONIOMMUSCRATCHMEMORYLATEML_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    OPTION_GNB_CONFIGURATION  GnbLateFeatureTable[] = {
      #if (GNB_TYPE_ML == TRUE)
        OPTION_PCIEALIBV2FEATURE_ENTRY
      #endif
      OPTIONIOMMUSCRATCHMEMORYLATEML_ENTRY
      OPTIONIOMMUACPIIVRSLATE_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if (AGESA_ENTRY_INIT_S3SAVE == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_INIT_SVIEW
      #define OPTION_GFX_INIT_SVIEW TRUE
    #endif
    #if (OPTION_GFX_INIT_SVIEW == TRUE) && (GNB_TYPE_ML == TRUE)
      OPTION_GNB_FEATURE                                  GfxInitSview;
      #define OPTION_GFXINITSVIEW_ENTRY                   {AMD_FAMILY_ML, GfxInitSview},
    #else
      #define OPTION_GFXINITSVIEW_ENTRY
    #endif

    OPTION_GNB_CONFIGURATION  GnbS3SaveFeatureTable[] = {
      OPTION_GFXINITSVIEW_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if  (GNB_TYPE_ML == TRUE)
    #define GNBS3RESTOREV4
    #define GNBS3RESTOREV7
    #if (GNB_TYPE_ML == TRUE)
      S3_DISPATCH_FUNCTION  GnbSmuServiceRequestV7S3Script;
      #undef  GNBS3RESTOREV7
      #define GNBS3RESTOREV7 {GnbSmuServiceRequestV7S3Script_ID, GnbSmuServiceRequestV7S3Script},
    #endif
    S3_DISPATCH_FUNCTION  GnbLibStallS3Script;
    #define PCIELATERESTOREML
    #if (GNB_TYPE_ML == TRUE)
      S3_DISPATCH_FUNCTION  PcieLateRestoreInitMLS3Script;
      #undef  PCIELATERESTOREML
      #define PCIELATERESTOREML {PcieLateRestoreMLS3Script_ID, PcieLateRestoreInitMLS3Script},
    #endif
    #define GNB_S3_DISPATCH_FUNCTION_TABLE \
    GNBS3RESTOREV4      \
    GNBS3RESTOREV7      \
    PCIELATERESTOREML \
    {GnbLibStallS3Script_ID, GnbLibStallS3Script},
  #endif

#endif
#endif  // _OPTION_GNB_INSTALL_H_
