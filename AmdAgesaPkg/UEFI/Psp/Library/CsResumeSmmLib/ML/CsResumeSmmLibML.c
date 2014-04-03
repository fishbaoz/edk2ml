/* $NoKeywords:$ */
/**
 * @file
 *
 * CS Resume SMM Library for family 0x16 ML CPU
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   CS Resume SMM Driver
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
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
 ******************************************************************************
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "CsResumeSmmLibML.h"
#include "CpuFuncs.h"
#include "Filecode.h"
//#include "GnbPcie.h"
#include "GnbRegistersML.h"

#define FILECODE  UEFI_PSP_LIBRARY_CSRESUMESMMLIB_ML_CSRESUMESMMLIBML_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define  PMIO_INDEX                    (0xCD6)
#define  PMIO_DATA                     (0xCD7)
#define    ISA_CONTROL_ADDRESS         (0x04)
#define      MMIO_EN_BIT               (0x02)

#define  FCH_MMIO_BASE                 (0xFED80000ull)
#define    FCH_PMIO_MMIO_OFFSET        (0x300)
#define    FCH_PMIO2_MMIO_OFFSET       (0x400)

#define  FCH_AB_BAR_OFFSET             (0xE0)
#define  FCH_AB_IO_DATA_OFFSET         (0x4)
#define  FCH_ABCFG_SELECT              (0xC0000000ul)

#define  PCI_MCFG_BASE_MSR             (0xC0010058ul)
#define    PCI_MCFG_MASK               (0x000000FFFFF00000ull)

#define  C6_BASE_MSR                   (0xC0010073ul)
#define    C6_BASE_MASK                (0xFFFF)
#define    C6_BASE_SHIFT               (0)
#define    C6_PORT_OFFSET              (1)

#define  GNB_ROOT_PORT_PCI_DEVICE      (0x2)
#define  GNB_ROOT_PORT_PCI_OFFSET      (0xE0)

#define  GNB_MISC_PCI_FUNCTION         (0x0)
#define  GNB_MISC_INDEX_PCI_OFFSET     (0x60)
#define  GNB_MISC_DATA_PCI_OFFSET      (0x64)

#define  GNB_ORB_PCI_FUNCTION          (0x0)
#define  GNB_ORB_INDEX_PCI_OFFSET      (0x94)
#define  GNB_ORB_DATA_PCI_OFFSET       (0x98)

#define  GNB_SMU_PCI_FUNCTION          (0x0)
#define  GNB_SMU_INDEX_PCI_OFFSET      (0xB8)
#define  GNB_SMU_DATA_PCI_OFFSET       (0xBC)

#define  GNB_DEV_PCI_FUNCTION          (0x0)
#define  GNB_DEV_INDEX_PCI_OFFSET      (0xC8)
#define  GNB_DEV_DATA_PCI_OFFSET       (0xCC)

#define  GNB_GBIF_PCI_FUNCTION         (0x0)
#define  GNB_GBIF_INDEX_PCI_OFFSET     (0xD0)
#define  GNB_GBIF_DATA_PCI_OFFSET      (0xD4)

#define  GNB_LINK_PCI_FUNCTION         (0x0)
#define  GNB_LINK_INDEX_PCI_OFFSET     (0xE0)
#define  GNB_LINK_DATA_PCI_OFFSET      (0xE4)

#define  GNB_IOAPIC_PCI_FUNCTION       (0x0)
#define  GNB_IOAPIC_INDEX_PCI_OFFSET   (0xF8)
#define  GNB_IOAPIC_DATA_PCI_OFFSET    (0xFC)

#define  GNB_IOMMU_L2_PCI_FUNCTION     (0x2)
#define  GNB_IOMMU_L2_INDEX_PCI_OFFSET (0xF0)
#define  GNB_IOMMU_L2_DATA_PCI_OFFSET  (0xF4)
#define    GNB_IOMMU_L2_WRITE_EN       (0x100)

#define  GNB_IOMMU_L1_PCI_FUNCTION     (0x2)
#define  GNB_IOMMU_L1_INDEX_PCI_OFFSET (0xF8)
#define  GNB_IOMMU_L1_DATA_PCI_OFFSET  (0xFC)
#define    GNB_IOMMU_L1_WRITE_EN       (0x80000000ul)

#define  GNB_GMM_BAR_PCI_DEVICE        (0x1)
#define  GNB_GMM_BAR_PCI_FUNCTION      (0x0)
#define  GNB_GMM_BAR_PCI_OFFSET        (0x24)
#define    GNB_GMM_BAR_MASK            (0xFFFF0000ull)

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/// Specific PCI bits restoration entry
typedef struct {
  BOOLEAN       PortPresent;          ///< Specifies whether port was present at end of POST
  UINT8         TrainingState;        ///< Current state of training sequence
  UINT8         Function;             ///< Function number of this port
} CS_PCIE_TRAINING_DATA;

/// PCIe Link Training State
typedef enum {
  LinkStateResetAssert,                                 ///< Assert port GPIO reset
  LinkStateResetDuration,                               ///< Timeout for reset duration
  LinkStateResetExit,                                   ///< Deassert port GPIO reset
  LinkTrainingResetTimeout,                             ///< Port GPIO reset timeout
  LinkStateReleaseTraining,                             ///< Release link training
  LinkStateDetectPresence,                              ///< Detect device presence
  LinkStateDetecting,                                   ///< Detect link training.
  LinkStateBrokenLane,                                  ///< Check and handle broken lane
  LinkStateGen2Fail,                                    ///< Check and handle device that fail training if GEN2 capability advertised
  LinkStateL0,                                          ///< Device trained to L0
  LinkStateVcoNegotiation,                              ///< Check VCO negotiation complete
  LinkStateRetrain,                                     ///< Force retrain link.
  LinkStateTrainingFail,                                ///< Link training fail
  LinkStateTrainingSuccess,                             ///< Link training success
  LinkStateGfxWorkaround,                               ///< GFX workaround
  LinkStateCompliance,                                  ///< Link in compliance mode
  LinkStateDeviceNotPresent,                            ///< Link is not connected
  LinkStateTrainingCompleted                            ///< Link training completed
} PCIE_LINK_TRAINING_STATE;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


UINT64    mPciMcfgBase = 0;
UINT8     *mGnbMiscIndex = NULL;
UINT32    *mGnbMiscData = NULL;
UINT8     *mGnbOrbIndex = NULL;
UINT32    *mGnbOrbData = NULL;
UINT32    *mGnbSmuIndex = NULL;
UINT32    *mGnbSmuData = NULL;
UINT32    *mGnbDevIndex = NULL;
UINT32    *mGnbDevData = NULL;
UINT32    *mGnbGbifIndex = NULL;
UINT32    *mGnbGbifData = NULL;
UINT32    *mGnbLinkIndex = NULL;
UINT32    *mGnbLinkData = NULL;
UINT8     *mGnbIoapicIndex = NULL;
UINT32    *mGnbIoapicData = NULL;
UINT16    *mGnbIommuL2Index = NULL;
UINT32    *mGnbIommuL2Data = NULL;
UINT32    *mGnbIommuL1Index = NULL;
UINT32    *mGnbIommuL1Data = NULL;
UINT64    mGnbGmmBase = 0;
UINT16    mFchAbIoAddress = 0xFFFF;


CS_RESTORATION_ENTRY_HEADER*
F16MlRestorePciEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_PCI    *PciEntry;
  UINT64    PciAddr;
  UINT32    *Register;

  PciEntry = (CS_PCI *) CurrentEntry;
  PciAddr = mPciMcfgBase + PciEntry->Address;
  Register = (UINT32 *) PciAddr;
  *Register = PciEntry->Value;
  PciEntry++;
  return &PciEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestorePciBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_PCI_BITS  *PciBitsEntry;
  UINT64    PciAddr;
  UINT32    *Register;
  UINT32    Scratch;

  PciBitsEntry = (CS_PCI_BITS *) CurrentEntry;
  PciAddr = mPciMcfgBase + PciBitsEntry->Address;
  Register = (UINT32 *) PciAddr;
  Scratch = (*Register & PciBitsEntry->Mask) | PciBitsEntry->Value;
  *Register = Scratch;
  PciBitsEntry++;
  return &PciBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreMmioEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_MMIO  *MmioEntry;
  UINT32   *Register;

  MmioEntry = (CS_MMIO *) CurrentEntry;
  Register = (UINT32 *) MmioEntry->Address;
  *Register = MmioEntry->Value;
  MmioEntry++;
  return &MmioEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreMmioBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_MMIO_BITS  *MmioBitsEntry;
  UINT32   *Register;
  UINT32   Scratch;

  MmioBitsEntry = (CS_MMIO_BITS *) CurrentEntry;
  Register = (UINT32 *) MmioBitsEntry->Address;
  Scratch = (*Register & MmioBitsEntry->Mask) | MmioBitsEntry->Value;
  *Register = Scratch;
  MmioBitsEntry++;
  return &MmioBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbMiscIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_MISC  *GnbMiscEntry;

  GnbMiscEntry = (CS_GNB_MISC *) CurrentEntry;
  *mGnbMiscIndex = GnbMiscEntry->Address;
  *mGnbMiscData = GnbMiscEntry->Value;
  GnbMiscEntry++;
  return &GnbMiscEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbMiscIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_MISC_BITS  *GnbMiscBitsEntry;

  GnbMiscBitsEntry = (CS_GNB_MISC_BITS *) CurrentEntry;
  *mGnbMiscIndex = GnbMiscBitsEntry->Address;
  *mGnbMiscData = ((*mGnbMiscData & GnbMiscBitsEntry->Mask) | GnbMiscBitsEntry->Value);
  GnbMiscBitsEntry++;
  return &GnbMiscBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbOrbCfgEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_ORB_CFG  *GnbOrbCfgEntry;

  GnbOrbCfgEntry = (CS_GNB_ORB_CFG *) CurrentEntry;
  *mGnbOrbIndex = GnbOrbCfgEntry->Address;
  *mGnbOrbData = GnbOrbCfgEntry->Value;
  GnbOrbCfgEntry++;
  return &GnbOrbCfgEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbOrbCfgBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_ORB_CFG_BITS  *GnbOrbCfgBitsEntry;

  GnbOrbCfgBitsEntry = (CS_GNB_ORB_CFG_BITS *) CurrentEntry;
  *mGnbOrbIndex = GnbOrbCfgBitsEntry->Address;
  *mGnbOrbData = (*mGnbOrbData & GnbOrbCfgBitsEntry->Mask) | GnbOrbCfgBitsEntry->Value;
  GnbOrbCfgBitsEntry++;
  return &GnbOrbCfgBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbSmuIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_SMU  *GnbSmuEntry;

  GnbSmuEntry = (CS_GNB_SMU *) CurrentEntry;
  *mGnbSmuIndex = GnbSmuEntry->Address;
  *mGnbSmuData = GnbSmuEntry->Value;
  GnbSmuEntry++;
  return &GnbSmuEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbSmuIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_SMU_BITS  *GnbSmuBitsEntry;

  GnbSmuBitsEntry = (CS_GNB_SMU_BITS *) CurrentEntry;
  *mGnbSmuIndex = GnbSmuBitsEntry->Address;
  *mGnbSmuData = (*mGnbSmuData & GnbSmuBitsEntry->Mask) | GnbSmuBitsEntry->Value;
  GnbSmuBitsEntry++;
  return &GnbSmuBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbDevIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_DEV  *GnbDevEntry;

  GnbDevEntry = (CS_GNB_DEV *) CurrentEntry;
  *mGnbDevIndex = (UINT32) (((UINT32) GnbDevEntry->Device << 16) | (UINT32) GnbDevEntry->Address);
  *mGnbDevData = GnbDevEntry->Value;
  GnbDevEntry++;
  return &GnbDevEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbDevIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_DEV_BITS  *GnbDevBitsEntry;

  GnbDevBitsEntry = (CS_GNB_DEV_BITS *) CurrentEntry;
  *mGnbDevIndex = (UINT32) (((UINT32) GnbDevBitsEntry->Device << 16) | (UINT32) GnbDevBitsEntry->Address);
  *mGnbDevData = ((*mGnbDevData & GnbDevBitsEntry->Mask) | GnbDevBitsEntry->Value);
  GnbDevBitsEntry++;
  return &GnbDevBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbGbifIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_GBIF  *GnbGbifEntry;

  GnbGbifEntry = (CS_GNB_GBIF *) CurrentEntry;
  *mGnbGbifIndex = GnbGbifEntry->Address;
  *mGnbGbifData = GnbGbifEntry->Value;
  GnbGbifEntry++;
  return &GnbGbifEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbGbifIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_GBIF_BITS  *GnbGbifBitsEntry;

  GnbGbifBitsEntry = (CS_GNB_GBIF_BITS *) CurrentEntry;
  *mGnbGbifIndex = GnbGbifBitsEntry->Address;
  *mGnbGbifData = (*mGnbGbifData & GnbGbifBitsEntry->Mask) | GnbGbifBitsEntry->Value;
  GnbGbifBitsEntry++;
  return &GnbGbifBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbLinkIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_LINK  *GnbLinkEntry;

  GnbLinkEntry = (CS_GNB_LINK *) CurrentEntry;
  *mGnbLinkIndex = (UINT32) ((((UINT32) GnbLinkEntry->Block << 24) | (UINT32) GnbLinkEntry->Frame << 16) | (UINT32) GnbLinkEntry->Address);
  *mGnbLinkData = GnbLinkEntry->Value;
  GnbLinkEntry++;
  return &GnbLinkEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbLinkIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_LINK_BITS  *GnbLinkBitsEntry;

  GnbLinkBitsEntry = (CS_GNB_LINK_BITS *) CurrentEntry;
  *mGnbLinkIndex = (UINT32) ((((UINT32) GnbLinkBitsEntry->Block << 24) | (UINT32) GnbLinkBitsEntry->Frame << 16) | (UINT32) GnbLinkBitsEntry->Address);
  *mGnbLinkData = ((*mGnbLinkData & GnbLinkBitsEntry->Mask) | GnbLinkBitsEntry->Value);
  GnbLinkBitsEntry++;
  return &GnbLinkBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbIoapicIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_IOAPIC  *GnbIoapicEntry;

  GnbIoapicEntry = (CS_GNB_IOAPIC *) CurrentEntry;
  *mGnbIoapicIndex = GnbIoapicEntry->Address;
  *mGnbIoapicData = GnbIoapicEntry->Value;
  GnbIoapicEntry++;
  return &GnbIoapicEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbIoapicIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_IOAPIC_BITS  *GnbIoapicBitsEntry;

  GnbIoapicBitsEntry = (CS_GNB_IOAPIC_BITS *) CurrentEntry;
  *mGnbIoapicIndex = GnbIoapicBitsEntry->Address;
  *mGnbIoapicData = (*mGnbIoapicData & GnbIoapicBitsEntry->Mask) | GnbIoapicBitsEntry->Value;
  GnbIoapicBitsEntry++;
  return &GnbIoapicBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreIommuL2IndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_IOMMU_L2  *GnbIommuL2Entry;

  GnbIommuL2Entry = (CS_GNB_IOMMU_L2 *) CurrentEntry;
  *mGnbIommuL2Index = ((UINT16) GnbIommuL2Entry->Address) | GNB_IOMMU_L2_WRITE_EN;
  *mGnbIommuL2Data = GnbIommuL2Entry->Value;
  GnbIommuL2Entry++;
  return &GnbIommuL2Entry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreIommuL2IndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_IOMMU_L2_BITS  *GnbIommuL2BitsEntry;

  GnbIommuL2BitsEntry = (CS_GNB_IOMMU_L2_BITS *) CurrentEntry;
  *mGnbIommuL2Index = ((UINT16) GnbIommuL2BitsEntry->Address) | GNB_IOMMU_L2_WRITE_EN;
  *mGnbIommuL2Data = (*mGnbIommuL2Data & GnbIommuL2BitsEntry->Mask) | GnbIommuL2BitsEntry->Value;
  GnbIommuL2BitsEntry++;
  return &GnbIommuL2BitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreIommuL1IndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_IOMMU_L1  *GnbIommuL1Entry;

  GnbIommuL1Entry = (CS_GNB_IOMMU_L1 *) CurrentEntry;
  *mGnbIommuL1Index = ((((UINT32) GnbIommuL1Entry->Select << 16) | (UINT32) GnbIommuL1Entry->Address) | GNB_IOMMU_L1_WRITE_EN);
  *mGnbIommuL1Data = GnbIommuL1Entry->Value;
  GnbIommuL1Entry++;
  return &GnbIommuL1Entry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreIommuL1IndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_IOMMU_L1_BITS  *GnbIommuL1BitsEntry;

  GnbIommuL1BitsEntry = (CS_GNB_IOMMU_L1_BITS *) CurrentEntry;
  *mGnbIommuL1Index = ((((UINT32) GnbIommuL1BitsEntry->Select << 16) | (UINT32) GnbIommuL1BitsEntry->Address) | GNB_IOMMU_L1_WRITE_EN);
  *mGnbIommuL1Data = (*mGnbIommuL1Data & GnbIommuL1BitsEntry->Mask) | GnbIommuL1BitsEntry->Value;
  GnbIommuL1BitsEntry++;
  return &GnbIommuL1BitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbRootPortIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64            PciAddr;
  UINT32            *GnbRootPortAddr;
  CS_GNB_ROOT_PORT  *GnbRootPortEntry;

  GnbRootPortEntry = (CS_GNB_ROOT_PORT *) CurrentEntry;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, GNB_ROOT_PORT_PCI_DEVICE, 0, GNB_ROOT_PORT_PCI_OFFSET);
  PciAddr += ((UINT64) GnbRootPortEntry->Function << 12);
  GnbRootPortAddr = (UINT32 *) PciAddr;
  *GnbRootPortAddr++ = (UINT32) GnbRootPortEntry->Address;
  *GnbRootPortAddr = GnbRootPortEntry->Value;
  GnbRootPortEntry++;
  return &GnbRootPortEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbRootPortIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64                 PciAddr;
  UINT32                 *GnbRootPortAddr;
  CS_GNB_ROOT_PORT_BITS  *GnbRootPortBitsEntry;

  GnbRootPortBitsEntry = (CS_GNB_ROOT_PORT_BITS *) CurrentEntry;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, GNB_ROOT_PORT_PCI_DEVICE, 0, GNB_ROOT_PORT_PCI_OFFSET);
  PciAddr += ((UINT64) GnbRootPortBitsEntry->Function << 12);
  GnbRootPortAddr = (UINT32 *) PciAddr;
  *GnbRootPortAddr++ = (UINT32) GnbRootPortBitsEntry->Address;
  *GnbRootPortAddr = (*GnbRootPortAddr & GnbRootPortBitsEntry->Mask) | GnbRootPortBitsEntry->Value;
  GnbRootPortBitsEntry++;
  return &GnbRootPortBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbGmmEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32      *GmmRegister;
  UINT64      MmioAddr;
  CS_GNB_GMM  *GnbGmmEntry;

  GnbGmmEntry = (CS_GNB_GMM *) CurrentEntry;
  MmioAddr = mGnbGmmBase + GnbGmmEntry->Address;
  GmmRegister = (UINT32 *) MmioAddr;
  *GmmRegister = GnbGmmEntry->Value;
  GnbGmmEntry++;
  return &GnbGmmEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGnbGmmBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32           *GmmRegister;
  UINT64           MmioAddr;
  CS_GNB_GMM_BITS  *GnbGmmBitsEntry;

  GnbGmmBitsEntry = (CS_GNB_GMM_BITS *) CurrentEntry;
  MmioAddr = mGnbGmmBase + GnbGmmBitsEntry->Address;
  GmmRegister = (UINT32 *) MmioAddr;
  *GmmRegister = (*GmmRegister & GnbGmmBitsEntry->Mask) | GnbGmmBitsEntry->Value;
  GnbGmmBitsEntry++;
  return &GnbGmmBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreFchAbEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_FCH_AB  *FchAbEntry;

  FchAbEntry = (CS_FCH_AB *) CurrentEntry;
  CsResumeSmmLibWriteIo32 (mFchAbIoAddress, ((UINT32) FchAbEntry->Address | FCH_ABCFG_SELECT));
  CsResumeSmmLibWriteIo32 (mFchAbIoAddress + FCH_AB_IO_DATA_OFFSET, FchAbEntry->Value);
  FchAbEntry++;
  return &FchAbEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreFchAbBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32          CurrentValue;
  CS_FCH_AB_BITS  *FchAbBitsEntry;

  FchAbBitsEntry = (CS_FCH_AB_BITS *) CurrentEntry;
  CsResumeSmmLibWriteIo32 (mFchAbIoAddress, ((UINT32) FchAbBitsEntry->Address | FCH_ABCFG_SELECT));
  CurrentValue = CsResumeSmmLibReadIo32 (mFchAbIoAddress + FCH_AB_IO_DATA_OFFSET);
  CurrentValue = (CurrentValue & FchAbBitsEntry->Mask) | FchAbBitsEntry->Value;
  CsResumeSmmLibWriteIo32 (mFchAbIoAddress + FCH_AB_IO_DATA_OFFSET, CurrentValue);
  FchAbBitsEntry++;
  return &FchAbBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreFchPmioDwordEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64             MmioAddr;
  UINT32             *PmioAddr;
  CS_FCH_PMIO_DWORD  *FchPmioDwordEntry;

  FchPmioDwordEntry = (CS_FCH_PMIO_DWORD *) CurrentEntry;
  MmioAddr = FCH_MMIO_BASE + FCH_PMIO_MMIO_OFFSET + FchPmioDwordEntry->Address;
  PmioAddr = (UINT32 *) MmioAddr;
  *PmioAddr = FchPmioDwordEntry->Value;
  FchPmioDwordEntry++;
  return &FchPmioDwordEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreFchPmioDwordBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64             MmioAddr;
  UINT32             *PmioAddr;
  CS_FCH_PMIO_DWORD_BITS  *FchPmioDwordBitsEntry;

  FchPmioDwordBitsEntry = (CS_FCH_PMIO_DWORD_BITS *) CurrentEntry;
  MmioAddr = FCH_MMIO_BASE + FCH_PMIO_MMIO_OFFSET + FchPmioDwordBitsEntry->Address;
  PmioAddr = (UINT32 *) MmioAddr;
  *PmioAddr = (*PmioAddr & FchPmioDwordBitsEntry->Mask) | FchPmioDwordBitsEntry->Value;
  FchPmioDwordBitsEntry++;
  return &FchPmioDwordBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreFchPmioWordEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64            MmioAddr;
  UINT16            *PmioAddr;
  CS_FCH_PMIO_WORD  *FchPmioWordEntry;

  FchPmioWordEntry = (CS_FCH_PMIO_WORD *) CurrentEntry;
  MmioAddr = FCH_MMIO_BASE + FCH_PMIO_MMIO_OFFSET + FchPmioWordEntry->Address;
  PmioAddr = (UINT16 *) MmioAddr;
  *PmioAddr = FchPmioWordEntry->Value;
  FchPmioWordEntry++;
  return &FchPmioWordEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreFchPmioWordBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64                 MmioAddr;
  UINT16                 *PmioAddr;
  CS_FCH_PMIO_WORD_BITS  *FchPmioWordBitsEntry;

  FchPmioWordBitsEntry = (CS_FCH_PMIO_WORD_BITS *) CurrentEntry;
  MmioAddr = FCH_MMIO_BASE + FCH_PMIO_MMIO_OFFSET + FchPmioWordBitsEntry->Address;
  PmioAddr = (UINT16 *) MmioAddr;
  *PmioAddr = (*PmioAddr & FchPmioWordBitsEntry->Mask) | FchPmioWordBitsEntry->Value;
  FchPmioWordBitsEntry++;
  return &FchPmioWordBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreFchPmioByteEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64            MmioAddr;
  UINT8             *PmioAddr;
  CS_FCH_PMIO_BYTE  *FchPmioByteEntry;

  FchPmioByteEntry = (CS_FCH_PMIO_BYTE *) CurrentEntry;
  MmioAddr = FCH_MMIO_BASE + FCH_PMIO_MMIO_OFFSET + FchPmioByteEntry->Address;
  PmioAddr = (UINT8 *) MmioAddr;
  *PmioAddr = FchPmioByteEntry->Value;
  FchPmioByteEntry++;
  return &FchPmioByteEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreFchPmioByteBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64                 MmioAddr;
  UINT8                  *PmioAddr;
  CS_FCH_PMIO_BYTE_BITS  *FchPmioByteBitsEntry;

  FchPmioByteBitsEntry = (CS_FCH_PMIO_BYTE_BITS *) CurrentEntry;
  MmioAddr = FCH_MMIO_BASE + FCH_PMIO_MMIO_OFFSET + FchPmioByteBitsEntry->Address;
  PmioAddr = (UINT8 *) MmioAddr;
  *PmioAddr = (*PmioAddr & FchPmioByteBitsEntry->Mask) | FchPmioByteBitsEntry->Value;
  FchPmioByteBitsEntry++;
  return &FchPmioByteBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreFchPmio2Entry  (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64        MmioAddr;
  UINT8         *Pmio2Addr;
  CS_FCH_PMIO2  *FchPmio2Entry;

  FchPmio2Entry = (CS_FCH_PMIO2 *) CurrentEntry;
  MmioAddr = FCH_MMIO_BASE + FCH_PMIO2_MMIO_OFFSET + FchPmio2Entry->Address;
  Pmio2Addr = (UINT8 *) MmioAddr;
  *Pmio2Addr = FchPmio2Entry->Value;
  FchPmio2Entry++;
  return &FchPmio2Entry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreFchPmio2BitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64             MmioAddr;
  UINT8              *Pmio2Addr;
  CS_FCH_PMIO2_BITS  *FchPmio2BitsEntry;

  FchPmio2BitsEntry = (CS_FCH_PMIO2_BITS *) CurrentEntry;
  MmioAddr = FCH_MMIO_BASE + FCH_PMIO2_MMIO_OFFSET + FchPmio2BitsEntry->Address;
  Pmio2Addr = (UINT8 *) MmioAddr;
  *Pmio2Addr = (*Pmio2Addr & FchPmio2BitsEntry->Mask) | FchPmio2BitsEntry->Value;
  FchPmio2BitsEntry++;
  return &FchPmio2BitsEntry->Header;
}

VOID
F16MlDispatchEnableFchAbEntries (
  IN       UINT32  Context
  )
{
  UINT16    *FchAbRegBar;
  UINT64    MmioAddr;

  MmioAddr = FCH_MMIO_BASE + FCH_PMIO_MMIO_OFFSET + FCH_AB_BAR_OFFSET;
  FchAbRegBar = (UINT16 *) MmioAddr;
  mFchAbIoAddress = *FchAbRegBar;
}

VOID
F16MlDispatchEnableGnbGmmEntries (
  IN       UINT32  Context
  )
{
  UINT32    *GnbGmmBar;
  UINT64    PciAddr;

  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, GNB_GMM_BAR_PCI_DEVICE, GNB_GMM_BAR_PCI_FUNCTION, GNB_GMM_BAR_PCI_OFFSET);
  GnbGmmBar = (UINT32 *) PciAddr;
  mGnbGmmBase = (UINT64) (*GnbGmmBar) & GNB_GMM_BAR_MASK;
}

VOID
F16MlDispatchWaitForReconfig (
  IN       UINT32  Context
  )
{
  UINT32    GnbLinkIndexValue;
  UINT32    GnbLinkDataValue;
  UINT64    PciAddr;

  do {
    PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_INDEX_PCI_OFFSET);
    GnbLinkIndexValue = 0x01308062;
    *((UINT32 *) PciAddr) = GnbLinkIndexValue;
    PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_DATA_PCI_OFFSET);
    GnbLinkDataValue = *((UINT32 *) PciAddr);
  } while ((GnbLinkDataValue & 0x0001) == 0x1);
}

VOID
F16MlDispatchPcieTraining (
  IN       UINT32  Context
  )
{
  UINT64                  PciIndexAddr;
  UINT64                  PciDataAddr;
  UINT8                   PortId;
  UINT8                   AllPortsDone;
  UINT8                   Index;
  UINT8                   TrainingHistory[24];
  CS_PCIE_TRAINING_DATA   TrainingData[5];
  UINT32                  GnbDataValue;
  UINT32                  InitialTimerValue;
  UINT32                  CurrentTimerValue;
  UINT32                  TimeStampDelta;
  UINT32                  *pTrainingHistory;

// 1) On entry, training is released... could be an issue with reset release?
  //a) Identify which ports are being trained - Need BOOLEAN for training enabled
  //aa) Need state machine value for
  for (PortId = 0; PortId < 5; PortId++) {
    PciIndexAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_INDEX_PCI_OFFSET);
    PciDataAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_DATA_PCI_OFFSET);
    *((UINT32 *) PciIndexAddr) = 0x01300800 + (0x100 * PortId);
    GnbDataValue = *((UINT32 *) PciDataAddr);
    if ((GnbDataValue & D0F0xE4_WRAP_0800_HoldTraining_MASK) == 0) {
      TrainingData[PortId].TrainingState = LinkStateDeviceNotPresent;
      TrainingData[PortId].PortPresent = FALSE;
    } else {
      TrainingData[PortId].TrainingState = LinkStateDetectPresence;
      TrainingData[PortId].PortPresent = TRUE;
      PciIndexAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_MISC_INDEX_PCI_OFFSET);
      PciDataAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_MISC_DATA_PCI_OFFSET);
      *((UINT32 *) PciIndexAddr) = 0x30 + PortId;
      GnbDataValue = *((UINT32 *) PciDataAddr);
      TrainingData[PortId].Function = (UINT8) (PciDataAddr & 0x7);
    }
  }
  //b) Is timer running???
  //c) Is reset released???
  //d) Set initial timer value

  PciIndexAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_INDEX_PCI_OFFSET);
  PciDataAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_DATA_PCI_OFFSET);
  *((UINT32 *) PciIndexAddr) = 0x013080F0;
  InitialTimerValue = *((UINT32 *) PciDataAddr);
  do {
    AllPortsDone = 0;
    *((UINT32 *) PciIndexAddr) = 0x013080F0;
    CurrentTimerValue = *((UINT32 *) PciDataAddr);
    TimeStampDelta = CurrentTimerValue - InitialTimerValue;
    if (TrainingData[PortId].PortPresent == TRUE) {
      // Read LC State History for training state determination
      PciIndexAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, GNB_ROOT_PORT_PCI_DEVICE, TrainingData[PortId].Function, GNB_ROOT_PORT_PCI_OFFSET);
      PciDataAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, GNB_ROOT_PORT_PCI_DEVICE, TrainingData[PortId].Function, GNB_ROOT_PORT_PCI_OFFSET + 4);
      pTrainingHistory = (UINT32 *)&TrainingHistory[0];
      for (Index = 0; Index < 6; Index++) {
        *((UINT32 *) PciIndexAddr) = 0xA5 + Index;
        *pTrainingHistory = *((UINT32 *) PciDataAddr);
        pTrainingHistory++;
      }
    }
    for (PortId = 0; PortId < 5; PortId++) {
      switch (TrainingData[PortId].TrainingState) {
      case LinkStateDetectPresence:
        if (TrainingHistory[0] > 4) {
          TrainingData[PortId].TrainingState = LinkStateDetecting;
          }
        break;
      case LinkStateDetecting:
        if (TrainingHistory[0] > 0xF) {
          TrainingData[PortId].TrainingState = LinkStateTrainingSuccess;
          AllPortsDone++;
        } else {
          // Check for training failure - this should be a critical failure during CS Resume
          for (Index = 0; Index < 16; Index++) {
            if (TrainingHistory[Index] == 0x2A) {
              if ((TrainingHistory[Index + 1] == 0x6) || (TrainingHistory[Index + 1] == 0x9) || (TrainingHistory[Index + 1] == 0xB)) {
                TrainingData[PortId].TrainingState = LinkStateDeviceNotPresent;
                AllPortsDone++;
              }
            }
          }
        }
        break;
      case LinkStateTrainingSuccess:
      case LinkStateDeviceNotPresent:
      case LinkStateTrainingCompleted:
        AllPortsDone++;
        break;
      }
    }
  } while ((AllPortsDone < 5) && (TimeStampDelta < 60000));
}

#define X(GenericFunction) F16MlDispatch##GenericFunction,
CONST PF_GENERIC_DISPATCH_FUNCTION_RESTORE ROMDATA F16MlCsDispatchFunctions [] =
{
  LIST_OF_DISPATCHABLE_FUNCTIONS
};
#undef X

CS_RESTORATION_ENTRY_HEADER*
F16MlRestoreGenericDispatchEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GENERIC_DISPATCH  *GenericDispatchEntry;

  GenericDispatchEntry = (CS_GENERIC_DISPATCH *) CurrentEntry;
  F16MlCsDispatchFunctions[GenericDispatchEntry->FunctionIndex] (GenericDispatchEntry->Context);
  GenericDispatchEntry++;
  return &GenericDispatchEntry->Header;
}


#define X(CsType) F16MlRestore##CsType##Entry,
CONST PF_RESTORE_FROM_TABLE_ENTRY ROMDATA F16MlCsRestorationFunctions [] =
{
  LIST_OF_REGISTER_SPACES
};
#undef X

VOID
ProcessSocReplayBuffer (
  IN       VOID *CsRestoreBuffer
  )
{
  UINT8   PMx04;
  UINT64  PciAddr;
  CS_RESTORATION_ENTRY_HEADER  *CurrentEntry;

  // Initialize global data for faster processing
  mPciMcfgBase = (EfiReadMsr (PCI_MCFG_BASE_MSR)) & PCI_MCFG_MASK;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_MISC_PCI_FUNCTION, GNB_MISC_INDEX_PCI_OFFSET);
  mGnbMiscIndex = (UINT8 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_MISC_PCI_FUNCTION, GNB_MISC_DATA_PCI_OFFSET);
  mGnbMiscData = (UINT32 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_ORB_PCI_FUNCTION, GNB_ORB_INDEX_PCI_OFFSET);
  mGnbOrbIndex = (UINT8 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_ORB_PCI_FUNCTION, GNB_ORB_DATA_PCI_OFFSET);
  mGnbOrbData = (UINT32 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_SMU_PCI_FUNCTION, GNB_SMU_INDEX_PCI_OFFSET);
  mGnbSmuIndex = (UINT32 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_SMU_PCI_FUNCTION, GNB_SMU_DATA_PCI_OFFSET);
  mGnbSmuData = (UINT32 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_DEV_PCI_FUNCTION, GNB_DEV_INDEX_PCI_OFFSET);
  mGnbDevIndex = (UINT32 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_DEV_PCI_FUNCTION, GNB_DEV_DATA_PCI_OFFSET);
  mGnbDevData = (UINT32 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_GBIF_PCI_FUNCTION, GNB_GBIF_INDEX_PCI_OFFSET);
  mGnbGbifIndex = (UINT32 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_GBIF_PCI_FUNCTION, GNB_GBIF_DATA_PCI_OFFSET);
  mGnbGbifData = (UINT32 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_INDEX_PCI_OFFSET);
  mGnbLinkIndex = (UINT32 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_DATA_PCI_OFFSET);
  mGnbLinkData = (UINT32 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_IOAPIC_PCI_FUNCTION, GNB_IOAPIC_INDEX_PCI_OFFSET);
  mGnbIoapicIndex = (UINT8 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_IOAPIC_PCI_FUNCTION, GNB_IOAPIC_DATA_PCI_OFFSET);
  mGnbIoapicData = (UINT32 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_IOMMU_L2_PCI_FUNCTION, GNB_IOMMU_L2_INDEX_PCI_OFFSET);
  mGnbIommuL2Index = (UINT16 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_IOMMU_L2_PCI_FUNCTION, GNB_IOMMU_L2_DATA_PCI_OFFSET);
  mGnbIommuL2Data = (UINT32 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_IOMMU_L1_PCI_FUNCTION, GNB_IOMMU_L1_INDEX_PCI_OFFSET);
  mGnbIommuL1Index = (UINT32 *) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_IOMMU_L1_PCI_FUNCTION, GNB_IOMMU_L1_DATA_PCI_OFFSET);
  mGnbIommuL1Data = (UINT32 *) PciAddr;

  // Enable FCH MMIO range for faster processing
  CsResumeSmmLibWriteIo8 (PMIO_INDEX, ISA_CONTROL_ADDRESS);
  PMx04 = CsResumeSmmLibReadIo8 (PMIO_DATA);
  PMx04 |= MMIO_EN_BIT;
  CsResumeSmmLibWriteIo8 (PMIO_DATA, PMx04);

  for (CurrentEntry = (CS_RESTORATION_ENTRY_HEADER *) CsRestoreBuffer; CurrentEntry->Type != CsEndOfTable; CurrentEntry = F16MlCsRestorationFunctions [CurrentEntry->Type] (CurrentEntry)) {
  }
}

VOID
PutExecutingSocCoreIntoCC6State (
  )
{
  UINT64  C6BaseMsr;
  UINT16  C6Port;
  UINT8   DummyRead;

  C6BaseMsr = EfiReadMsr (C6_BASE_MSR);
  C6Port = (UINT16) ((C6BaseMsr & C6_BASE_MASK) >> C6_BASE_SHIFT);
  for (;;) {
    DummyRead = CsResumeSmmLibReadIo8 (C6Port + C6_PORT_OFFSET);
  }
}
