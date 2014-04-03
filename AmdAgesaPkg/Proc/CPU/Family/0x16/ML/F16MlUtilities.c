/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_16 models 30h - 3Fh Mullins specific utility functions.
 *
 * Provides numerous utility functions specific to family 16h Mullins.
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
#include "cpuFamilyTranslation.h"
#include "cpuF16PowerMgmt.h"
#include "F16MlPowerMgmt.h"
#include "cpuEarlyInit.h"
#include "GeneralServices.h"
#include "F16MlUtilities.h"
#include "heapManager.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)
#define FILECODE PROC_CPU_FAMILY_0X16_ML_F16MLUTILITIES_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
STATIC
F16MlNbPstateDisCore (
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
STATIC
F16MlGetNbFreqNumeratorInMHz (
  IN       UINT32                NbFid,
     OUT   UINT32                *FreqNumeratorInMHz,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

VOID
STATIC
F16MlGetNbFreqDivisor (
  IN       UINT32                NbDid,
     OUT   UINT32                *FreqDivisor,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

AGESA_STATUS
STATIC
F16MlCalculateNbFrequencyInMHz (
  IN       UINT32                NbFid,
  IN       UINT32                NbDid,
     OUT   UINT32                *FrequencyInMHz,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
STATIC
F16MlCovertVidInuV (
  IN       UINT32                Vid,
     OUT   UINT32                *VoltageInuV,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

VOID
STATIC
F16MlCmnGetIddDivisor (
  IN       UINT32                IddDiv,
     OUT   UINT32                *IddDivisor,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

VOID
STATIC
F16MlCmnCalculateCurrentInmA (
  IN       UINT32                IddValue,
  IN       UINT32                IddDiv,
     OUT   UINT32                *CurrentInmA,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

BOOLEAN
F16MlSetDownCoreRegister (
  IN       CPU_CORE_LEVELING_FAMILY_SERVICES *FamilySpecificServices,
  IN       UINT32 *Socket,
  IN       UINT32 *Module,
  IN       UINT32 *LeveledCores,
  IN       CORE_LEVELING_TYPE CoreLevelMode,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 *  Get CPU pstate current.
 *
 *  @CpuServiceMethod{::F_CPU_GET_IDD_MAX}.
 *
 *    This function returns the ProcIddMax.
 *
 *  @param[in]     FamilySpecificServices    The current Family Specific Services.
 *  @param[in]     Pstate                    The P-state to check.
 *  @param[out]    ProcIddMax                P-state current in mA.
 *  @param[in]     StdHeader                 Handle of Header for calling lib functions and services.
 *
 *  @retval        TRUE                      P-state is enabled
 *  @retval        FALSE                     P-state is disabled
 */
BOOLEAN
F16MlGetProcIddMax (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       UINT8                  Pstate,
     OUT   UINT32                 *ProcIddMax,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32       MsrAddress;
  PSTATE_MSR   PstateMsr;
  BOOLEAN      IsPstateEnabled;
  PCI_ADDR     PciAddress;
  NB_CAPS_2_REGISTER  NbCap2;
  UINT32       ProcIddMaxPerCore;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F16MlGetProcIddMax - P%d\n", Pstate);

  IsPstateEnabled = FALSE;

  MsrAddress = (UINT32) (Pstate + PS_REG_BASE);
  ASSERT (MsrAddress <= PS_MAX_REG);

  LibAmdMsrRead (MsrAddress, (UINT64 *) &PstateMsr, StdHeader);
  F16MlCmnCalculateCurrentInmA ((UINT32) PstateMsr.IddValue, (UINT32) PstateMsr.IddDiv, &ProcIddMaxPerCore, StdHeader);
  PciAddress.AddressValue = NB_CAPS_REG2_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &NbCap2, StdHeader);
  *ProcIddMax = (UINT32) ProcIddMaxPerCore * (NbCap2.CmpCap + 1);
  IDS_HDT_CONSOLE (CPU_TRACE, "    Pstate %d ProcIddMax %d CmpCap %d\n", Pstate, *ProcIddMax, NbCap2.CmpCap);

  if (PstateMsr.PsEnable == 1) {
    IsPstateEnabled = TRUE;
  }
  return IsPstateEnabled;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set down core register on Mullins
 *
 * This function set F3x190 Downcore Control Register[5:0]
 *
 * @param[in]   FamilySpecificServices   The current Family Specific Services.
 * @param[in]   Socket                   Socket ID.
 * @param[in]   Module                   Module ID in socket.
 * @param[in]   LeveledCores             Number of core.
 * @param[in]   CoreLevelMode            Core level mode.
 * @param[in]   StdHeader                Header for library and services.
 *
 * @retval      TRUE                     Down Core register is updated.
 * @retval      FALSE                    Down Core register is not updated.
 */
BOOLEAN
F16MlSetDownCoreRegister (
  IN       CPU_CORE_LEVELING_FAMILY_SERVICES *FamilySpecificServices,
  IN       UINT32 *Socket,
  IN       UINT32 *Module,
  IN       UINT32 *LeveledCores,
  IN       CORE_LEVELING_TYPE CoreLevelMode,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8     NumOfComputeUnit;
  UINT8     CoresPerComputeUnit;
  UINT32    LocalPciRegister;
  UINT32    CoreDisableBits;
  UINT32    TempVar32_a;
  PCI_ADDR  PciAddress;
  BOOLEAN   IsUpdated;
  AGESA_STATUS AgesaStatus;

  IsUpdated = FALSE;
  CoreDisableBits = 0;
  TempVar32_a = 1;
  CoresPerComputeUnit = 1;

  switch (CoreLevelMode) {
  // There's no 'break' except 'case CORE_LEVEL_COMPUTE_UNIT'.
  // It's for generating CoreDisableBits and CoresPerComputeUnit
  case CORE_LEVEL_COMPUTE_UNIT_THREE:
    TempVar32_a = TempVar32_a << 1;
    CoresPerComputeUnit++;
  case CORE_LEVEL_COMPUTE_UNIT_TWO:
    TempVar32_a = TempVar32_a << 1;
    CoresPerComputeUnit++;
  case CORE_LEVEL_COMPUTE_UNIT:
    TempVar32_a = (TempVar32_a << 1) - 1;
    TempVar32_a = FOUR_CORE_COMPUTE_UNIT_BITMAP & (~TempVar32_a);
    NumOfComputeUnit = (UINT8) ((*LeveledCores) / CoresPerComputeUnit);
    for (CoreDisableBits = 0; NumOfComputeUnit > 0; NumOfComputeUnit--) {
      CoreDisableBits <<= FOUR_CORE_COMPUTE_UNIT_BITWIDTH;
      CoreDisableBits |= TempVar32_a;
    }
    break;
  default:
    TempVar32_a = *LeveledCores;
    if (TempVar32_a == 1) {
      CoreDisableBits = DOWNCORE_MASK_SINGLE;
    } else {
      CoreDisableBits = ALL_CORES_DISABLE_BITMAP;
      TempVar32_a = ((1 << TempVar32_a) - 1);
      CoreDisableBits &= ~TempVar32_a;
    }
  }

  if (CoreDisableBits != 0) {
    if (GetPciAddress (StdHeader, (UINT8) *Socket, (UINT8) *Module, &PciAddress, &AgesaStatus)) {
      PciAddress.Address.Function = FUNC_5;
      PciAddress.Address.Register = NORTH_BRIDGE_CAPABILITIES_2_REG;
      LibAmdPciRead (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader);
      LocalPciRegister = (LocalPciRegister & 0xFF) + 1;
      LocalPciRegister = (1 << LocalPciRegister) - 1;
      CoreDisableBits &= LocalPciRegister;

      PciAddress.Address.Function = FUNC_3;
      PciAddress.Address.Register = DOWNCORE_CTRL;
      LibAmdPciRead (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader);
      if ((LocalPciRegister | CoreDisableBits) != LocalPciRegister) {
        LocalPciRegister |= CoreDisableBits;
        LibAmdPciWrite (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader);
        IsUpdated = TRUE;
      }
    }
  }

  return IsUpdated;
}


CONST CPU_CORE_LEVELING_FAMILY_SERVICES ROMDATA F16MlCoreLeveling =
{
  0,
  F16MlSetDownCoreRegister
};


/*---------------------------------------------------------------------------------------*/
/**
 *  Determines the NB clock on the desired node.
 *
 *  @CpuServiceMethod{::F_CPU_GET_NB_FREQ}.
 *
 *  @param[in]   FamilySpecificServices   The current Family Specific Services.
 *  @param[out]  FrequencyInMHz           Northbridge clock frequency in MHz.
 *  @param[in]   StdHeader                Header for library and services.
 *
 *  @return      AGESA_SUCCESS            FrequencyInMHz is valid.
 */
AGESA_STATUS
F16MlGetCurrentNbFrequency (
  IN       CPU_SPECIFIC_SERVICES *FamilySpecificServices,
     OUT   UINT32 *FrequencyInMHz,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  NB_PSTATE_STS_REGISTER NbPstateStsReg;
  PCI_ADDR     PciAddress;
  AGESA_STATUS ReturnCode;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F16MlGetCurrentNbFrequency\n");

  PciAddress.AddressValue = NB_PSTATE_STATUS_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &NbPstateStsReg, StdHeader);
  ReturnCode = F16MlCalculateNbFrequencyInMHz (
                 NbPstateStsReg.CurNbFid,
                 NbPstateStsReg.CurNbDid,
                 FrequencyInMHz,
                 StdHeader
                 );
  return ReturnCode;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Returns the node's minimum and maximum northbridge frequency.
 *
 *  @CpuServiceMethod{::F_CPU_GET_MIN_MAX_NB_FREQ}.
 *
 *  @param[in]     FamilySpecificServices  The current Family Specific Services.
 *  @param[in]     PlatformConfig          Platform profile/build option config structure.
 *  @param[in]     PciAddress              The segment, bus, and device numbers of the CPU in question.
 *  @param[out]    MinFreqInMHz            The node's minimum northbridge frequency.
 *  @param[out]    MaxFreqInMHz            The node's maximum northbridge frequency.
 *  @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 *
 *  @retval        AGESA_SUCCESS           Northbridge frequency is valid
 */
AGESA_STATUS
F16MlGetMinMaxNbFrequency (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       PCI_ADDR               *PciAddress,
     OUT   UINT32                 *MinFreqInMHz,
     OUT   UINT32                 *MaxFreqInMHz,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  INT8         NbPsMaxVal;
  UINT8        i;
  UINT32       LocalPciRegister;
  AGESA_STATUS AgesaStatus;

  AgesaStatus = AGESA_ERROR;

  // Obtain the max NB frequency on the node
  PciAddress->Address.Function = FUNC_5;
  PciAddress->Address.Register = NB_PSTATE_CTRL;
  LibAmdPciRead (AccessWidth32, *PciAddress, &LocalPciRegister, StdHeader);
  NbPsMaxVal = (INT8) ((NB_PSTATE_CTRL_REGISTER *) &LocalPciRegister)->NbPstateMaxVal;

  // Starting from NB Pmax
  for (i = 0; i <= NbPsMaxVal; i++) {
    PciAddress->Address.Function = FUNC_5;
    PciAddress->Address.Register = (NB_PSTATE_0 + (4 * i));
    LibAmdPciRead (AccessWidth32, *PciAddress, &LocalPciRegister, StdHeader);

    // Ensure that the NB Pstate is enabled
    if (((NB_PSTATE_REGISTER *) &LocalPciRegister)->NbPstateEn == 1) {
      AgesaStatus = F16MlCalculateNbFrequencyInMHz (((NB_PSTATE_REGISTER *) &LocalPciRegister)->NbFid,
                                                    ((NB_PSTATE_REGISTER *) &LocalPciRegister)->NbDid,
                                                    MaxFreqInMHz,
                                                    StdHeader);
      break;
    }
  }
  // If all of NbPstates are disabled, get MaxFreqInMHz from CurNbPstate
  if (i > NbPsMaxVal) {
    PciAddress->Address.Register = NB_PSTATE_STATUS;
    LibAmdPciRead (AccessWidth32, *PciAddress, &LocalPciRegister, StdHeader);
    F16MlCalculateNbFrequencyInMHz (((NB_PSTATE_STS_REGISTER *) &LocalPciRegister)->CurNbFid,
                                      ((NB_PSTATE_STS_REGISTER *) &LocalPciRegister)->CurNbDid,
                                      MaxFreqInMHz,
                                      StdHeader);
    // No one NbPstate is enabled, so set Min = Max
    *MinFreqInMHz = *MaxFreqInMHz;
    ASSERT (FALSE);
  } else {
    // If platform configuration disable NB P-states, return the NB P0 frequency
    // as both the min and max frequency on the node.
    if (PlatformConfig->PlatformProfile.PlatformPowerPolicy == Performance) {
      *MinFreqInMHz = *MaxFreqInMHz;
    } else {
      PciAddress->Address.Function = FUNC_5;
      PciAddress->Address.Register = NB_PSTATE_CTRL;
      LibAmdPciRead (AccessWidth32, *PciAddress, &LocalPciRegister, StdHeader);
      NbPsMaxVal = (INT8) ((NB_PSTATE_CTRL_REGISTER *) &LocalPciRegister)->NbPstateMaxVal;

      // Obtain the min NB frequency on the node, starting from NB Pmin
      for (NbPsMaxVal; NbPsMaxVal >= 0; NbPsMaxVal--) {
        PciAddress->Address.Function = FUNC_5;
        PciAddress->Address.Register = (NB_PSTATE_0 + (4 * NbPsMaxVal));
        LibAmdPciRead (AccessWidth32, *PciAddress, &LocalPciRegister, StdHeader);

        // Ensure that the NB Pstate is enabled
        if (((NB_PSTATE_REGISTER *) &LocalPciRegister)->NbPstateEn == 1) {
          AgesaStatus = F16MlCalculateNbFrequencyInMHz (((NB_PSTATE_REGISTER *) &LocalPciRegister)->NbFid,
                                                        ((NB_PSTATE_REGISTER *) &LocalPciRegister)->NbDid,
                                                        MinFreqInMHz,
                                                        StdHeader);
          break;
        }
      }
    }
  }
  IDS_OPTION_HOOK (IDS_NBPS_MIN_FREQ, MinFreqInMHz, StdHeader);

  return AgesaStatus;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Determines the NB clock on the desired node.
 *
 *  @CpuServiceMethod{::F_CPU_GET_NB_PSTATE_INFO}.
 *
 *  @param[in]     FamilySpecificServices  The current Family Specific Services.
 *  @param[in]     PlatformConfig          Platform profile/build option config structure.
 *  @param[in]     PciAddress              The segment, bus, and device numbers of the CPU in question.
 *  @param[in]     NbPstate                The NB P-state number to check.
 *  @param[out]    FreqNumeratorInMHz      The desired node's frequency numerator in megahertz.
 *  @param[out]    FreqDivisor             The desired node's frequency divisor.
 *  @param[out]    VoltageInuV             The desired node's voltage in microvolts.
 *  @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 *
 *  @retval        TRUE                    NbPstate is valid
 *  @retval        FALSE                   NbPstate is disabled or invalid
 */
BOOLEAN
F16MlGetNbPstateInfo (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       PCI_ADDR               *PciAddress,
  IN       UINT32                 NbPstate,
     OUT   UINT32                 *FreqNumeratorInMHz,
     OUT   UINT32                 *FreqDivisor,
     OUT   UINT32                 *VoltageInuV,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32   NbVid;
  BOOLEAN  PstateIsValid;
  NB_PSTATE_CTRL_REGISTER   NbPstateCtrlReg;
  NB_PSTATE_REGISTER        NbPstateReg;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F16MlGetNbPstateInfo - NB P%d\n", NbPstate);

  ASSERT ((PciAddress->Address.Segment == 0) && (PciAddress->Address.Bus == 0) && (PciAddress->Address.Device == 0x18));

  PstateIsValid = FALSE;

  // If NB P1, P2, or P3 is requested, make sure that NB Pstate is enabled
  if ((NbPstate == 0) || (FamilySpecificServices->IsNbPstateEnabled (FamilySpecificServices, PlatformConfig, StdHeader))) {
    PciAddress->AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
    LibAmdPciRead (AccessWidth32, *PciAddress, &NbPstateCtrlReg, StdHeader);

    ASSERT ((NbPstate < NM_NB_PS_REG) && (NbPstateCtrlReg.NbPstateMaxVal < NM_NB_PS_REG));
    if (NbPstate <= NbPstateCtrlReg.NbPstateMaxVal) {
      PciAddress->Address.Register = (NB_PSTATE_0 + (sizeof (NB_PSTATE_REGISTER) * NbPstate));
      LibAmdPciRead (AccessWidth32, *PciAddress, &NbPstateReg, StdHeader);
      IDS_HDT_CONSOLE (CPU_TRACE, "    En:%d Fid:%x Did:%x Vid:%x\n", NbPstateReg.NbPstateEn, NbPstateReg.NbFid, NbPstateReg.NbDid, GetF16MlNbVid (&NbPstateReg));

      // Check if at least NB P0 is enabled.
      ASSERT ((NbPstate == 0) ? (NbPstateReg.NbPstateEn == 1) : TRUE);
      // Ensure that requested NbPstate is enabled
      if (NbPstateReg.NbPstateEn == 1) {
        // Check for P-state Bandwidth Requirements on
        // "All NB P-states must be defined such that D18F5x1[6C:60][NbFid] <= 2Eh"
        ASSERT (NbPstateReg.NbFid <= 0x2E);
        F16MlGetNbFreqNumeratorInMHz (NbPstateReg.NbFid, FreqNumeratorInMHz, StdHeader);
        F16MlGetNbFreqDivisor (NbPstateReg.NbDid, FreqDivisor, StdHeader);
        // Check for P-state Bandwidth Requirements on
        // "400MHz <= NBCOF <= 1600MHz"
        ASSERT ((*FreqNumeratorInMHz / *FreqDivisor) >= 400);
        ASSERT ((*FreqNumeratorInMHz / *FreqDivisor) <= 1600);

        NbVid = GetF16MlNbVid (&NbPstateReg);
        F16MlCovertVidInuV  (NbVid, VoltageInuV, StdHeader);
        PstateIsValid = TRUE;
        IDS_HDT_CONSOLE (CPU_TRACE, "    NB Pstate %d is Valid. NbVid=%d VoltageInuV=%d\n", NbPstate, NbVid, *VoltageInuV);
      }
    }
  }
  return PstateIsValid;
}


/*---------------------------------------------------------------------------------------*/
/**
 *  Get NB pstate current.
 *
 *  @CpuServiceMethod{::F_CPU_GET_NB_IDD_MAX}.
 *
 *    This function returns the NbIddMax.
 *
 *  @param[in]     FamilySpecificServices    The current Family Specific Services.
 *  @param[in]     NbPstate                  The NB P-state to check.
 *  @param[out]    NbIddMax                  NB P-state current in mA.
 *  @param[in]     StdHeader                 Handle of Header for calling lib functions and services.
 *
 *  @retval        TRUE                      NB P-state is enabled, and NbIddMax is valid.
 *  @retval        FALSE                     NB P-state is disabled
 */
BOOLEAN
F16MlGetNbIddMax (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       UINT8                  NbPstate,
     OUT   UINT32                 *NbIddMax,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  BOOLEAN      IsNbPsEnabled;
  PCI_ADDR     PciAddress;
  NB_PSTATE_CTRL_REGISTER   NbPstateCtrlReg;
  NB_PSTATE_REGISTER        NbPstateReg;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F16MlGetNbIddMax - NB P%d\n", NbPstate);

  IsNbPsEnabled = FALSE;

  PciAddress.AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &NbPstateCtrlReg, StdHeader);

  ASSERT (NbPstate < NM_NB_PS_REG);
  if (NbPstate <= NbPstateCtrlReg.NbPstateMaxVal) {
    PciAddress.Address.Register = (NB_PSTATE_0 + (sizeof (NB_PSTATE_REGISTER) * NbPstate));
    LibAmdPciRead (AccessWidth32, PciAddress, &NbPstateReg, StdHeader);

    // Ensure that requested NbPstate is enabled
    if (NbPstateReg.NbPstateEn == 1) {
      F16MlCmnCalculateCurrentInmA (NbPstateReg.NbIddValue, NbPstateReg.NbIddDiv, NbIddMax, StdHeader);
      IsNbPsEnabled = TRUE;
      IDS_HDT_CONSOLE (CPU_TRACE, "  NB Pstate %d is Valid. NbIddMax %d\n", NbPstate, *NbIddMax);
    }
  }
  return IsNbPsEnabled;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get the number of physical cores of current processor.
 *
 * @CpuServiceMethod{::F_CPU_NUMBER_OF_PHYSICAL_CORES}.
 *
 * @param[in]      FamilySpecificServices         The current Family Specific Services.
 * @param[in]      StdHeader                      Handle of Header for calling lib functions and services.
 *
 * @return         The number of physical cores.
 */
UINT8
F16MlGetNumberOfPhysicalCores (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  CPUID_DATA    CpuId;

  //
  //CPUID.80000008h.ECX.NC + 1, 000b = 1, 001b = 2, etc.
  //
  LibAmdCpuidRead (CPUID_LONG_MODE_ADDR, &CpuId, StdHeader);
  return ((UINT8) ((CpuId.ECX_Reg & 0xff) + 1));
}


/*---------------------------------------------------------------------------------------*/
/**
 *  Get this AP's system core number from hardware.
 *
 *  @CpuServiceMethod{::F_CPU_GET_AP_CORE_NUMBER}.
 *
 *  Returns the system core number from the scratch MSR, where
 *  it was saved at heap initialization.
 *
 *  @param[in]     FamilySpecificServices  The current Family Specific Services.
 *  @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 *
 *  @return        The AP's unique core number
 */
UINT32
F16MlGetApCoreNumber (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  CPUID_DATA Cpuid;

  LibAmdCpuidRead (0x1, &Cpuid, StdHeader);
  return ((Cpuid.EBX_Reg >> 24) & 0xFF);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Is the Northbridge PState feature enabled?
 *
 * @CpuServiceMethod{::F_IS_NB_PSTATE_ENABLED}.
 *
 * @param[in]      FamilySpecificServices         The current Family Specific Services.
 * @param[in]      PlatformConfig                 Platform profile/build option config structure.
 * @param[in]      StdHeader                      Handle of Header for calling lib functions and services.
 *
 * @retval         TRUE                           The NB PState feature is enabled.
 * @retval         FALSE                          The NB PState feature is not enabled.
 */
BOOLEAN
F16MlIsNbPstateEnabled (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  PCI_ADDR PciAddress;
  BOOLEAN  NbPstatesSupported;
  BOOLEAN  SkipHwCfg;
  NB_PSTATE_STS_REGISTER   NbPstateSts;
  NB_PSTATE_CTRL_REGISTER  NbPstateCtrl;


  PciAddress.AddressValue = NB_PSTATE_STATUS_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, (VOID *) &NbPstateSts, StdHeader);
  if (NbPstateSts.NbPstateDis == 1) {
    return FALSE;
  }

  SkipHwCfg = FALSE;
  IDS_OPTION_HOOK (IDS_NBPSDIS_OVERRIDE, &SkipHwCfg, StdHeader);

  // NbPstates is supported by default
  NbPstatesSupported = TRUE;

  // Get user options
  if (!PlatformConfig->PlatformProfile.NbPstatesSupported) {
    NbPstatesSupported = FALSE;
  }

  PciAddress.AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, (VOID *) &NbPstateCtrl, StdHeader);
  if (((NbPstateCtrl.NbPstateMaxVal != 0) || SkipHwCfg) && (NbPstatesSupported)) {
    return TRUE;
  }
  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Disable NB P-state.
 *   - clear F5x1[6C:64]
 *   - clear F5x170[NbPstateMaxVal]
 *   - set   F5x170[SwNbPstateLoDis]
 *   - clear MSRC001_00[6B:64][NbPstate]
 *
 * @param[in]     FamilySpecificServices  The current Family Specific Services
 * @param[in]     CpuEarlyParamsPtr       Service Parameters
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 */
VOID
F16MlNbPstateDis (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       AMD_CPU_EARLY_PARAMS   *CpuEarlyParamsPtr,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32       i;
  UINT32       PciData;
  UINT32       NbPsCtrl;
  UINT32       NbPsCtrlOrg;
  BOOLEAN      SkipNbPsLoPart;
  PCI_ADDR     PciAddress;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F16MlNbPstateDis\n");

  // D18F5x170[30, NbPstateFidVidSbcEn] = 1
  PciAddress.AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);
  ((NB_PSTATE_CTRL_REGISTER *) &NbPsCtrl)->NbPstateFidVidSbcEn = 1;
  LibAmdPciWrite (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);

  // Check whether NB P-state is disabled
  if (!FamilySpecificServices->IsNbPstateEnabled (FamilySpecificServices, &CpuEarlyParamsPtr->PlatformConfig, StdHeader)) {

    IDS_HDT_CONSOLE (CPU_TRACE, "  NB Pstates disabled\n");

    PciAddress.AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
    LibAmdPciRead (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);
    NbPsCtrlOrg = NbPsCtrl;

    // If CurNbPstate is not NB P0, get the Pstate pointed to by CurNbPstate and copy it's value to NB P0 to P3 and clear NbPstateHi
    PciAddress.Address.Register = NB_PSTATE_STATUS;
    LibAmdPciRead (AccessWidth32, PciAddress, &PciData, StdHeader);

    SkipNbPsLoPart = FALSE;
    if ((((NB_PSTATE_STS_REGISTER *) &PciData)->StartupNbPstate != ((NB_PSTATE_CTRL_REGISTER *) &NbPsCtrl)->NbPstateHi) &&
        (((NB_PSTATE_STS_REGISTER *) &PciData)->StartupNbPstate != ((NB_PSTATE_CTRL_REGISTER *) &NbPsCtrl)->NbPstateLo)) {
      SkipNbPsLoPart = TRUE;
    }

    if (((NB_PSTATE_STS_REGISTER *) &PciData)->CurNbPstate != 0) {
      PciAddress.Address.Register = NB_PSTATE_0 + (((NB_PSTATE_STS_REGISTER *) &PciData)->CurNbPstate * 4);
      LibAmdPciRead (AccessWidth32, PciAddress, &PciData, StdHeader);

      for (i = 1; i < NM_NB_PS_REG; i++) {
        PciAddress.Address.Register = NB_PSTATE_0 + (i * 4);
        LibAmdPciWrite (AccessWidth32, PciAddress, &PciData, StdHeader);
      }

      if (!SkipNbPsLoPart) {
        // Program D18F5x170 to transition the NB P-state:
        // 1) NbPstateLo = NbPstateMaxVal.
        // 2) SwNbPstateLoDis = NbPstateDisOnP0 = NbPstateThreshold = 0.

        PciAddress.Address.Register = NB_PSTATE_CTRL;
        ((NB_PSTATE_CTRL_REGISTER *) &NbPsCtrl)->NbPstateLo = ((NB_PSTATE_CTRL_REGISTER *) &NbPsCtrl)->NbPstateMaxVal;
        LibAmdPciWrite (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);
        ((NB_PSTATE_CTRL_REGISTER *) &NbPsCtrl)->SwNbPstateLoDis = 0;
        ((NB_PSTATE_CTRL_REGISTER *) &NbPsCtrl)->NbPstateDisOnP0 = 0;
        ((NB_PSTATE_CTRL_REGISTER *) &NbPsCtrl)->NbPstateThreshold = 0;
        LibAmdPciWrite (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);

        // Wait for D18F5x174[CurNbPstate] to equal NbPstateLo.
        PciAddress.Address.Register = NB_PSTATE_STATUS;
        do {
          LibAmdPciRead (AccessWidth32, PciAddress, &PciData, StdHeader);
        } while (((NB_PSTATE_STS_REGISTER *) &PciData)->CurNbPstate != ((NB_PSTATE_CTRL_REGISTER *) &NbPsCtrl)->NbPstateLo);
      }
    }

    // Program D18F5x170 to force the NB P-state:
    // 1) NbPstateHi = target NB P-state.
    // 2) SwNbPstateLoDis = 1
    // And clear F5x170[NbPstateMaxVal]
    PciAddress.Address.Register = NB_PSTATE_CTRL;
    ((NB_PSTATE_CTRL_REGISTER *) &NbPsCtrl)->NbPstateHi = 0;
    ((NB_PSTATE_CTRL_REGISTER *) &NbPsCtrl)->NbPstateMaxVal = 0;
    LibAmdPciWrite (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);
    ((NB_PSTATE_CTRL_REGISTER *) &NbPsCtrl)->SwNbPstateLoDis = 1;
    LibAmdPciWrite (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);

    // Wait for D18F5x174[CurNbPstate] to equal the target NB P-state.
    PciAddress.Address.Register = NB_PSTATE_STATUS;
    do {
      LibAmdPciRead (AccessWidth32, PciAddress, &PciData, StdHeader);
    } while (((NB_PSTATE_STS_REGISTER *) &PciData)->CurNbPstate != 0);

    // Clear F5x1[6C:64]
    PciData = 0x00000000;
    for (i = 1; i < NM_NB_PS_REG; i++) {
      PciAddress.Address.Register = NB_PSTATE_0 + (i * 4);
      LibAmdPciWrite (AccessWidth32, PciAddress, &PciData, StdHeader);

    }

    // Clear MSRC001_00[6B:64][NbPstate] on cores
    F16MlNbPstateDisCore (StdHeader);

    // BIOS performs the following to release the NB P-state force:
    // 1. Restore the initial D18F5x170[SwNbPstateLoDis, NbPstateDisOnP0, NbPstateLo] values.
    // 2. Restore the initial D18F5x170[NbPstateThreshold, NbPstateHi] values.
    PciAddress.Address.Register = NB_PSTATE_CTRL;
    LibAmdPciRead (AccessWidth32, PciAddress, &PciData, StdHeader);
    ((NB_PSTATE_CTRL_REGISTER *) &PciData)->NbPstateLo = 0;
    LibAmdPciWrite (AccessWidth32, PciAddress, &PciData, StdHeader);
    ((NB_PSTATE_CTRL_REGISTER *) &PciData)->SwNbPstateLoDis = ((NB_PSTATE_CTRL_REGISTER *) &NbPsCtrlOrg)->SwNbPstateLoDis;
    ((NB_PSTATE_CTRL_REGISTER *) &PciData)->NbPstateDisOnP0 = ((NB_PSTATE_CTRL_REGISTER *) &NbPsCtrlOrg)->NbPstateDisOnP0;
    LibAmdPciWrite (AccessWidth32, PciAddress, &PciData, StdHeader);

    ((NB_PSTATE_CTRL_REGISTER *) &PciData)->NbPstateHi = 0;
    ((NB_PSTATE_CTRL_REGISTER *) &PciData)->NbPstateMaxVal = 0;
    LibAmdPciWrite (AccessWidth32, PciAddress, &PciData, StdHeader);
    ((NB_PSTATE_CTRL_REGISTER *) &PciData)->NbPstateThreshold = ((NB_PSTATE_CTRL_REGISTER *) &NbPsCtrlOrg)->NbPstateThreshold;
    LibAmdPciWrite (AccessWidth32, PciAddress, &PciData, StdHeader);
  }
}


/*---------------------------------------------------------------------------------------*/
/**
 * Disable NB P-state on core.
 *   - clear MSRC001_00[6B:64][NbPstate].
 *
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 */
VOID
STATIC
F16MlNbPstateDisCore (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32 i;
  UINT64 MsrData;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F16MlNbPstateDisCore\n");

  // Only one core per compute unit needs to clear NbPstate in P-state MSRs
  if (IsCoreComputeUnitPrimary (FirstCoreIsComputeUnitPrimary, StdHeader)) {
    for (i = MSR_PSTATE_0; i <= MSR_PSTATE_7; i++) {
      LibAmdMsrRead (i, &MsrData, StdHeader);
      ((PSTATE_MSR *) &MsrData)->NbPstate = 0;
      LibAmdMsrWrite (i, &MsrData, StdHeader);
    }
  }
}


/*---------------------------------------------------------------------------------------*/
/**
 * Get NB Frequency Numerator in MHz
 *
 * @param[in]     NbFid                   NB Frequency ID to convert
 * @param[out]    FreqNumeratorInMHz      The desire NB FID's frequency numerator in megahertz.
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 */
VOID
STATIC
F16MlGetNbFreqNumeratorInMHz (
  IN       UINT32                NbFid,
     OUT   UINT32                *FreqNumeratorInMHz,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  IDS_HDT_CONSOLE (CPU_TRACE, "  F16MlGetNbFreqNumeratorInMHz - NbFid=%d\n", NbFid);
  *FreqNumeratorInMHz = (NbFid + 4) * 100;
  IDS_HDT_CONSOLE (CPU_TRACE, "    FreqNumeratorInMHz=%d\n", *FreqNumeratorInMHz);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Get NB Frequency Divisor
 *
 * @param[in]     NbDid                   NB Divisor ID to convert.
 * @param[out]    FreqDivisor             The desire NB DID's frequency divisor.
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 */
VOID
STATIC
F16MlGetNbFreqDivisor (
  IN       UINT32                NbDid,
     OUT   UINT32                *FreqDivisor,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  IDS_HDT_CONSOLE (CPU_TRACE, "  F16MlGetNbFreqDivisor - NbDid=%d\n", NbDid);
  *FreqDivisor = (1 << NbDid);
  IDS_HDT_CONSOLE (CPU_TRACE, "    FreqDivisor=%d\n", *FreqDivisor);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Calculate NB Frequency in MHz
 *
 * @param[in]     NbFid                   NB Frequency ID to convert
 * @param[in]     NbDid                   NB Divisor ID to convert.
 * @param[out]    FrequencyInMHz          The Northbridge clock frequency in megahertz.
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 *
 * @return        AGESA_SUCCESS           FrequencyInMHz is valid.
 */
AGESA_STATUS
STATIC
F16MlCalculateNbFrequencyInMHz (
  IN       UINT32                NbFid,
  IN       UINT32                NbDid,
     OUT   UINT32                *FrequencyInMHz,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32        FreqNumeratorInMHz;
  UINT32        FreqDivisor;
  AGESA_STATUS  ReturnStatus;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F16MlCalculateNbFrequencyInMHz - NbFid=%x, NbDid=%x\n", NbFid, NbDid);

  ReturnStatus = AGESA_SUCCESS;
  F16MlGetNbFreqNumeratorInMHz (NbFid, &FreqNumeratorInMHz, StdHeader);
  F16MlGetNbFreqDivisor (NbDid, &FreqDivisor, StdHeader);
  *FrequencyInMHz = FreqNumeratorInMHz / FreqDivisor;
  IDS_HDT_CONSOLE (CPU_TRACE, "    FrequencyInMHz=%d\n", *FrequencyInMHz);

  return ReturnStatus;
}


/*---------------------------------------------------------------------------------------*/
/**
 * Convert VID to microvolts(uV)
 *
 * @param[in]     Vid                     The voltage ID of SVI2 encoding to be converted.
 * @param[out]    VoltageInuV             The voltage in microvolts.
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 */
VOID
STATIC
F16MlCovertVidInuV (
  IN       UINT32                Vid,
     OUT   UINT32                *VoltageInuV,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  IDS_HDT_CONSOLE (CPU_TRACE, "  F16MlCovertVidInuV\n");
  // Maximum 1.55V, 6.25mV per stpe
  *VoltageInuV = ConvertVidInuV(Vid);
  IDS_HDT_CONSOLE (CPU_TRACE, "    Vid=%x, VoltageInuV=%d\n", Vid, *VoltageInuV);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Get Core/NB Idd Divisor
 *
 * @param[in]     IddDiv                  Core/NB current divisor to convert.
 * @param[out]    IddDivisor              The desire Core/NB current divisor.
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 *
 */
VOID
STATIC
F16MlCmnGetIddDivisor (
  IN       UINT32                IddDiv,
     OUT   UINT32                *IddDivisor,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  IDS_HDT_CONSOLE (CPU_TRACE, "  F16MlCmnGetIddDivisor - IddDiv=%d\n", IddDiv);

  switch (IddDiv) {
  case 0:
    *IddDivisor = 1000;
    break;
  case 1:
    *IddDivisor = 100;
    break;
  case 2:
    *IddDivisor = 10;
    break;
  default:  // IddDiv = 3 is reserved. Use 10
    *IddDivisor = 10;
    ASSERT (FALSE);
    break;
  }
  IDS_HDT_CONSOLE (CPU_TRACE, "    IddDivisor=%d\n", *IddDivisor);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Calculate Core/NB current in mA
 *
 * @param[in]     IddValue                Core/NB current value.
 * @param[in]     IddDiv                  Core/NB current divisor.
 * @param[out]    CurrentInmA             The Core/NB current in milliampere.
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 *
 */
VOID
STATIC
F16MlCmnCalculateCurrentInmA (
  IN       UINT32                IddValue,
  IN       UINT32                IddDiv,
     OUT   UINT32                *CurrentInmA,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  UINT32        IddDivisor;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F16MlCmnCalculateCurrentInmA - IddValue=%x, IddDiv=%x\n", IddValue, IddDiv);

  F16MlCmnGetIddDivisor (IddDiv, &IddDivisor, StdHeader);
  *CurrentInmA = IddValue * IddDivisor;

  IDS_HDT_CONSOLE (CPU_TRACE, "    CurrentInmA=%d\n", *CurrentInmA);
}

/*---------------------------------------------------------------------------------------*/
/**
 *
 * Is PSP device present in currect processor
 *
 * @param[in]    StdHeader        Header for library and services
 *
 * @retval       TRUE             PSP device presents
 * @retval       FALSE            PSP device doesn't present
 *
 */
BOOLEAN
F16MlIsPspDevicePresent (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32   PciData;
  PCI_ADDR PciAddress;

  PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, FUNC_0, 0xB8);
  PciData = 0xC00C002C;
  LibAmdPciWrite (AccessWidth32, PciAddress, (VOID *)&PciData, StdHeader);
  PciAddress.Address.Register = 0xBC;
  LibAmdPciRead (AccessWidth32, PciAddress, (VOID *)&PciData, StdHeader);

  PciData &= ( SMU_CC_PSP_FUSES_SECURE +
               SMU_CC_PSP_FUSES_FRA_ENABLE +
               SMU_CC_PSP_FUSES_PROTO +
               SMU_CC_PSP_FUSES_PCPU_DIS);

  if ((PciData == PSP_SECURE_PART) || (PciData == PSP_PROTO_PART)) {
    return TRUE;
  }
  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 *
 * Is it SecureS3
 *
 * @param[in]    StdHeader        Header for library and services
 *
 * @retval       TRUE             It's SecureS3
 * @retval       FALSE            It's NOT SecureS3
 *
 */
BOOLEAN
F16MlIsSecureS3 (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32       SystemCoreNumber;
  UINT64       BaseAddress;
  CPU_SPECIFIC_SERVICES  *IgnoreService;

  // The best way to check IsSecureS3 is locating a PPI which is published by PSP driver
  // But APs don't have PEI service, and can't locate PPI
  // During SecureS3 resume path, PSP driver will relocate heap (including BSP and APs)
  // Below is an alternate solution: checking BSP's heap address
  BaseAddress = HeapGetBaseAddress (StdHeader);
  IgnoreService = NULL;
  SystemCoreNumber = F16MlGetApCoreNumber (IgnoreService, StdHeader);
  BaseAddress = BaseAddress - (SystemCoreNumber * AMD_HEAP_SIZE_PER_CORE);
  if ((BaseAddress != AMD_HEAP_START_ADDRESS) && ((StdHeader->HeapStatus == HEAP_DO_NOT_EXIST_YET) || (StdHeader->HeapStatus == HEAP_LOCAL_CACHE))) {
    return TRUE;
  }

  return FALSE;
}

