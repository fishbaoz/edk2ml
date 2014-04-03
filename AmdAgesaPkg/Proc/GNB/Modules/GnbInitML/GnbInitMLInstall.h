/* $NoKeywords:$ */
/**
 * @file
 *
 * ML service installation file
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
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
*
*/

#ifndef _GNB_INIT_ML_INSTALL_H_
#define _GNB_INIT_ML_INSTALL_H_

//-----------------------------------------------------------------------
// Specify definition used by module services
//-----------------------------------------------------------------------

#include  "GnbPcie.h"
#include  "GnbUraServices.h"
#include  "GnbPcieFamServices.h"
#include  "GnbFamServices.h"
#include  "GnbGfxFamServices.h"

//-----------------------------------------------------------------------
// Export services
//-----------------------------------------------------------------------

#if  (AGESA_ENTRY_INIT_EARLY == TRUE)

  extern F_PCIEFMGETSBCONFIGINFO                  PcieGetSbConfigInfoML;
  extern F_PCIEFMGETCOMPLEXDATALENGTH             PcieGetComplexDataLengthML;
  extern F_PCIEFMBUILDCOMPLEXCONFIGURATION        PcieBuildComplexConfigurationML;
  extern F_PCIEFMCONFIGUREENGINESLANEALLOCATION   PcieConfigureEnginesLaneAllocationML;
  extern F_PCIEFMCHECKPORTPCIDEVICEMAPPING        PcieCheckPortPciDeviceMappingML;
  extern F_PCIEFMMAPPORTPCIADDRESS                PcieMapPortPciAddressML;
  extern F_PCIEFMCHECKPORTPCIELANECANBEMUXED      PcieCheckPortPcieLaneCanBeMuxedML;


  PCIe_FAM_CONFIG_SERVICES GnbPcieConfigProtocolML = {
    PcieGetComplexDataLengthML,
    PcieBuildComplexConfigurationML,
    PcieConfigureEnginesLaneAllocationML,
    PcieCheckPortPciDeviceMappingML,
    PcieMapPortPciAddressML,
    PcieCheckPortPcieLaneCanBeMuxedML,
    PcieGetSbConfigInfoML
  };

  GNB_SERVICE GnbPcieConfigServicesML = {
    GnbPcieFamConfigService,
    AMD_FAMILY_ML,
    &GnbPcieConfigProtocolML,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbPcieConfigServicesML
#endif

#if  (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_MID == TRUE)
  extern F_PCIEFMGETCORECONFIGURATIONVALUE        PcieGetCoreConfigurationValueML;
  extern F_PCIEFMGETLINKSPEEDCAP                  PcieGetLinkSpeedCapML;
  extern F_PCIEFMGETNATIVEPHYLANEBITMAP           PcieGetNativePhyLaneBitmapML;
  extern F_PCIEFMSETLINKSPEEDCAP                  PcieSetLinkSpeedCapV4;

  PCIe_FAM_INIT_SERVICES GnbPcieInitProtocolML = {
    PcieGetCoreConfigurationValueML,
    PcieGetLinkSpeedCapML,
    PcieGetNativePhyLaneBitmapML,
    PcieSetLinkSpeedCapV4
  };

  GNB_SERVICE GnbPcieInitServicesML = {
    GnbPcieFamInitService,
    AMD_FAMILY_ML,
    &GnbPcieInitProtocolML,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbPcieInitServicesML
#endif

#if  (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_MID == TRUE)
  #if IDSOPT_IDS_ENABLED == TRUE
    #if IDSOPT_TRACING_ENABLED == TRUE
      extern F_PCIEFMDEBUGGETHOSTREGADDRESSSPACESTRING  PcieDebugGetHostRegAddressSpaceStringML;
      extern F_PCIEFMDEBUGGETWRAPPERNAMESTRING          PcieDebugGetWrapperNameStringML;
      extern F_PCIEFMDEBUGGETCORECONFIGURATIONSTRING    PcieDebugGetCoreConfigurationStringML;

      PCIe_FAM_DEBUG_SERVICES GnbPcieDebugProtocolML = {
        PcieDebugGetHostRegAddressSpaceStringML,
        PcieDebugGetWrapperNameStringML,
        PcieDebugGetCoreConfigurationStringML
      };

      GNB_SERVICE GnbPcieDebugServicesML = {
        GnbPcieFamDebugService,
        AMD_FAMILY_ML,
        &GnbPcieDebugProtocolML,
        SERVICES_POINTER
      };
      #undef  SERVICES_POINTER
      #define SERVICES_POINTER  &GnbPcieDebugServicesML
    #endif
  #endif
#endif

#if  (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_MID == TRUE) || (AGESA_ENTRY_INIT_LATE == TRUE)
  extern F_GNB_REGISTER_ACCESS  GnbRegisterReadML;
  extern F_GNB_REGISTER_ACCESS  GnbRegisterWriteML;

  GNB_REGISTER_SERVICE GnbRegisterAccessProtocolML = {
    GnbRegisterReadML,
    GnbRegisterWriteML
  };

  GNB_SERVICE GnbRegisterAccessServicesML = {
    GnbRegisterAccessService,
    AMD_FAMILY_ML,
    &GnbRegisterAccessProtocolML,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbRegisterAccessServicesML

  extern F_GNBFMCREATEIVRSENTRY    GnbCreateIvrsEntryML;
  extern F_GNBFMCHECKIOMMUPRESENT  GnbCheckIommuPresentML;

  GNB_FAM_IOMMU_SERVICES GnbIommuConfigProtocolML = {
    GnbCheckIommuPresentML,
    GnbCreateIvrsEntryML
  };

  GNB_SERVICE GnbIommuConfigServicesML = {
    GnbIommuService,
    AMD_FAMILY_ML,
    &GnbIommuConfigProtocolML,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbIommuConfigServicesML

  extern F_GNBURASERVICELOCATEREGTBL GnbUraLocateRegTblML;
  extern F_GNBURASERVICEGET GnbUraGetML;
  extern F_GNBURASERVICESET GnbUraSetML;
  extern F_GNBURASERVICESTREAMSET GnbUraStreamSetML;

  GNB_URA_SERVICE GnbUraProtocolML = {
    GnbUraLocateRegTblML,
    GnbUraGetML,
    GnbUraSetML,
    GnbUraStreamSetML
  };

  GNB_SERVICE GnbUraServicesML = {
    GnbUraService,
    AMD_FAMILY_ML,
    &GnbUraProtocolML,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbUraServicesML

  extern F_GFXMAPENGINETODISPLAYPATH GfxMapEngineToDisplayPathML;
  extern F_GFXDISABLECONTROLLER      GfxDisableControllerML;
  extern F_GFXCALCULATECLOCK         GfxCalculateClockML;
  extern F_GFXISVBIOSPOSTED          GfxIsVbiosPostedML;

  GFX_FAM_SERVICES GfxFamilyServiceProtocolML = {
    GfxMapEngineToDisplayPathML,
    GfxDisableControllerML,
    GfxCalculateClockML,
    GfxIsVbiosPostedML
  };

  GNB_SERVICE GfxFamilyServicesML = {
    GfxFamService,
    AMD_FAMILY_ML,
    &GfxFamilyServiceProtocolML,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GfxFamilyServicesML


  extern F_PCIE_MAXPAYLOAD_SETTING  PcieMaxPayloadML;

  PCIE_MAXPAYLOAD_SERVICE PcieMaxPayloadProtocolML = {
    PcieMaxPayloadML
  };

  GNB_SERVICE PcieMaxPayloadServicesML = {
    GnbPcieMaxPayloadService,
    AMD_FAMILY_ML,
    &PcieMaxPayloadProtocolML,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &PcieMaxPayloadServicesML

  extern F_GNBTIMESTAMP GnbTimeStampML;

  GNB_FAM_TS_SERVICES GnbFamTsProtocolML = {
    GnbTimeStampML,
  };

  GNB_SERVICE GnbFamTsServicesML = {
    GnbFamTsService,
    AMD_FAMILY_ML,
    &GnbFamTsProtocolML,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbFamTsServicesML

  extern F_GNBFMALIBGETBASETABLE  PcieAlibGetBaseTableML;

  GNB_FAM_ALIB_SERVICES AlibProtocolML = {
    PcieAlibGetBaseTableML
  };

  GNB_SERVICE AlibServicesML = {
    GnbFamAlibService,
    AMD_FAMILY_ML,
    &AlibProtocolML,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &AlibServicesML

  extern GNB_BUILD_OPTIONS_ML  GnbBuildOptionsML;

  GNB_FAM_BUILDOPTIONS_POINTERS GnbOptionsProtocolML = {
    &GnbBuildOptionsML
  };

  GNB_SERVICE GnbOptionsServicesML = {
    GnbFamBuildOptionPointer,
    AMD_FAMILY_ML,
    &GnbOptionsProtocolML,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbOptionsServicesML

#endif
#endif  // _GNB_INIT_ML_INSTALL_H_
