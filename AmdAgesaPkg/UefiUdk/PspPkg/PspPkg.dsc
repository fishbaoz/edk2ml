#;*****************************************************************************
#;
#; Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
#;
#; AMD is granting you permission to use this software (the Materials)
#; pursuant to the terms and conditions of your Software License Agreement
#; with AMD.  This header does *NOT* give you permission to use the Materials
#; or any rights under AMD's intellectual property.  Your use of any portion
#; of these Materials shall constitute your acceptance of those terms and
#; conditions.  If you do not agree to the terms and conditions of the Software
#; License Agreement, please do not use any portion of these Materials.
#;
#; CONFIDENTIALITY:  The Materials and all other information, identified as
#; confidential and provided to you by AMD shall be kept confidential in
#; accordance with the terms and conditions of the Software License Agreement.
#;
#; LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
#; PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
#; WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
#; MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
#; OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
#; IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
#; (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
#; INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
#; GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
#; RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
#; THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
#; EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
#; THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
#;
#; AMD does not assume any responsibility for any errors which may appear in
#; the Materials or any other related information provided to you by AMD, or
#; result from use of the Materials or any related information.
#;
#; You agree that you will not reverse engineer or decompile the Materials.
#;
#; NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
#; further information, software, technical information, know-how, or show-how
#; available to you.  Additionally, AMD retains the right to modify the
#; Materials at any time, without notice, and is not obligated to provide such
#; modified Materials to you.
#;
#; U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
#; "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
#; subject to the restrictions as set forth in FAR 52.227-14 and
#; DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
#; Government constitutes acknowledgement of AMD's proprietary rights in them.
#;
#; EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
#; direct product thereof will be exported directly or indirectly, into any
#; country prohibited by the United States Export Administration Act and the
#; regulations thereunder, without the required authorization from the U.S.
#; government nor will be used for any purpose prohibited by the same.
#;*****************************************************************************

[Defines] 
  PLATFORM_NAME = PspPkg
  PLATFORM_GUID = a7e607df-a337-443f-be20-7af8fd49a60a
  PLATFORM_VERSION = 0.1 
  DSC_SPECIFICATION = 0x00010005 
  OUTPUT_DIRECTORY = Build/PspPkg 
  SUPPORTED_ARCHITECTURES = IA32|X64
  BUILD_TARGETS = DEBUG|RELEASE 
  SKUID_IDENTIFIER = DEFAULT

[LibraryClasses]
  BaseLib                               |MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib                         |MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  DebugLib				|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  UefiDriverEntryPoint                  |MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiBootServicesTableLib              |MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  MemoryAllocationLib                   |MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  PcdLib                                |MdePkg/Library/DxePcdLib/DxePcdLib.inf
  # AGESA PSP Lib
  PspBaseLib                            |AmdAgesaPkg/UefiUdk/PspPkg/Library/PspBaseLib/PspBaseLib.inf
  PspfTpmLib                            |AmdAgesaPkg/UefiUdk/PspPkg/Library/PspfTpmLib/PspfTpmLib.inf
  PspMboxLib                            |AmdAgesaPkg/UefiUdk/PspPkg/Library/PspMboxLib/PspMboxLib.inf
  #Dummy Lib only for reference
  PspStorgeLib                          |AmdAgesaPkg/UefiUdk/PspPkg/Addendum/Library/PspStorgeLib/PspStorgeLib.inf

[LibraryClasses.common.PEIM]
  PeimEntryPoint                        |MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  MemoryAllocationLib                   |MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  PcdLib                                |MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  PeiServicesLib                        |MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  PeiServicesTablePointerLib            |MdePkg/Library/PeiServicesTablePointerLib/PeiServicesTablePointerLib.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  SmmServicesTableLib                   |MdePkg/Library/SmmServicesTableLib/SmmServicesTableLib.inf
  MemoryAllocationLib                   |MdePkg/Library/SmmMemoryAllocationLib/SmmMemoryAllocationLib.inf

##PCDs sections are not specified.
##All PCDs value are from their Default value in DEC.
##[PcdsFeatureFlag]
##[PcdsFixedAtBuild]


[Components.IA32]
  ##
  # AGESA PSP Drivers
  ##
  AmdAgesaPkg/UefiUdk/PspPkg/Drivers/iTpmPei/iTpmPei.inf
  AmdAgesaPkg/UefiUdk/PspPkg/Drivers/PspPei/PspPei.inf

[Components.X64]
  ##
  # AGESA PSP Drivers
  ##
  AmdAgesaPkg/UefiUdk/PspPkg/Drivers/iTpmDxe/iTpmDxe.inf
  AmdAgesaPkg/UefiUdk/PspPkg/Drivers/PspDxe/PspDxe.inf
  AmdAgesaPkg/UefiUdk/PspPkg/Drivers/PspSmm/PspSmm.inf
  AmdAgesaPkg/UefiUdk/PspPkg/Drivers/PspP2Cmbox/PspP2Cmbox.inf
  #Dummy Driver only for reference
  AmdAgesaPkg/UefiUdk/PspPkg/Addendum/Drivers/PspPlatformDriver/PspPlatform.inf 