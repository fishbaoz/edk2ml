/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD AGESA Family 16h ML Connected Standby Functions declarations.
 *
 * Contains code that declares the AGESA ML Connected Standby related APIs
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Feature
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

#ifndef _F16_ML_CONNECTED_STANDBY_H_
#define _F16_ML_CONNECTED_STANDBY_H_

/*----------------------------------------------------------------------------------------
 *          M I X E D   (Definitions And Macros / Typedefs, Structures, Enums)
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                 D E F I N I T I O N S     A N D     M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                    T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
#define LIST_OF_REGISTER_SPACES \
    X (Pci) \
    X (PciBits) \
    X (Mmio) \
    X (MmioBits) \
    X (GnbMiscIndex) \
    X (GnbMiscIndexBits) \
    X (GnbOrbCfg) \
    X (GnbOrbCfgBits) \
    X (GnbSmuIndex) \
    X (GnbSmuIndexBits) \
    X (GnbDevIndex) \
    X (GnbDevIndexBits) \
    X (GnbGbifIndex) \
    X (GnbGbifIndexBits) \
    X (GnbLinkIndex) \
    X (GnbLinkIndexBits) \
    X (GnbIoapicIndex) \
    X (GnbIoapicIndexBits) \
    X (IommuL2Index) \
    X (IommuL2IndexBits) \
    X (IommuL1Index) \
    X (IommuL1IndexBits) \
    X (GnbRootPortIndex) \
    X (GnbRootPortIndexBits) \
    X (GnbGmm) \
    X (GnbGmmBits) \
    X (FchAb) \
    X (FchAbBits) \
    X (FchPmioDword) \
    X (FchPmioDwordBits) \
    X (FchPmioWord) \
    X (FchPmioWordBits) \
    X (FchPmioByte) \
    X (FchPmioByteBits) \
    X (FchPmio2) \
    X (FchPmio2Bits) \
    X (GenericDispatch)


#define X(CsType) Restore##CsType,
#define MAKE_CS_ENTRY_TYPES(CsType) Restore##CsType
/// List of all valid table entry types
typedef enum {
  LIST_OF_REGISTER_SPACES  ///< Declare all register spaces in the proper order
  CsEndOfTable             ///< End of table marker
} CS_RESTORATION_ENTRY_TYPES;
#define MAKE_CS_RESTORATION_ENTRY_TYPES(Type) BSU32 (Type)
#undef X

#define LIST_OF_DISPATCHABLE_FUNCTIONS \
    X (EnableFchAbEntries) \
    X (EnableGnbGmmEntries)
#undef X

#define X(GenericFunction) CsProc##GenericFunction,
/// List of all valid dispatch functions
typedef enum {
  LIST_OF_DISPATCHABLE_FUNCTIONS  ///< Declare all function indices in the proper order
  MaxCsDispatchFunctions          ///< Not a valid entry, used for bounds checking
} CS_DISPATCH_FUNCTIONS;
#undef X



/// Common header for all table entries
typedef struct {
  CS_RESTORATION_ENTRY_TYPES   Type;           ///< Entry type
  BOOLEAN                      SaveReadValue;  ///< Determines whether the current value of the register should be saved or not
} CS_RESTORATION_ENTRY_HEADER;
#define MAKE_CS_RESTORATION_ENTRY_HEADER(Type, SaveReadValue) MAKE_CS_RESTORATION_ENTRY_TYPES (Type), BSU8 (SaveReadValue)

/// PCI restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT32                       Address;        ///< PCI address in MAKE_SBDFO format
  UINT32                       Value;          ///< Value to write
} CS_PCI;
#define MAKE_CS_PCI(SaveReadValue, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (Pci), SaveReadValue), BSU32 (Address), BSU32 (Value)

/// Specific PCI bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT32                       Address;        ///< PCI address in MAKE_SBDFO format
  UINT32                       Mask;           ///< Specifies the bits to isolate
  UINT32                       Value;          ///< Value to write
} CS_PCI_BITS;
#define MAKE_CS_PCI_BITS(SaveReadValue, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (PciBits), SaveReadValue), BSU32 (Address), BSU32 (Mask), BSU32 (Value)

/// Memory mapped IO restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT64                       Address;        ///< Raw MMIO address
  UINT32                       Value;          ///< Value to write
} CS_MMIO;
#define MAKE_CS_MMIO(SaveReadValue, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (Mmio), SaveReadValue), BSU64 (Address), BSU32 (Value)

/// Specific memory mapped IO bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT64                       Address;        ///< Raw MMIO address
  UINT32                       Mask;           ///< Specifies the bits to isolate
  UINT32                       Value;          ///< Value to write
} CS_MMIO_BITS;
#define MAKE_CS_MMIO_BITS(SaveReadValue, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (MmioBits), SaveReadValue), BSU64 (Address), BSU32 (Mask), BSU32 (Value)

/// GNB Miscellaneous restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Address;        ///< GNB Misc index
  UINT32                       Value;          ///< Value to write
} CS_GNB_MISC;
#define MAKE_CS_GNB_MISC(SaveReadValue, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbMiscIndex), SaveReadValue), BSU8 (Address), BSU32 (Value)

/// Specific GNB Miscellaneous bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Address;        ///< GNB Misc index
  UINT32                       Mask;           ///< Specifies the bits to isolate
  UINT32                       Value;          ///< Value to write
} CS_GNB_MISC_BITS;
#define MAKE_CS_GNB_MISC_BITS(SaveReadValue, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbMiscIndexBits), SaveReadValue), BSU8 (Address), BSU32 (Mask), BSU32 (Value)

/// GNB ORB configuration restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Address;        ///< GNB ORB index
  UINT32                       Value;          ///< Value to write
} CS_GNB_ORB_CFG;
#define MAKE_CS_GNB_ORB_CFG(SaveReadValue, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbOrbCfg), SaveReadValue), BSU8 (Address), BSU32 (Value)

/// Specific GNB ORB configuration bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Address;        ///< GNB ORB index
  UINT32                       Mask;           ///< Specifies the bits to isolate
  UINT32                       Value;          ///< Value to write
} CS_GNB_ORB_CFG_BITS;
#define MAKE_CS_GNB_ORB_CFG_BITS(SaveReadValue, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbOrbCfgBits), SaveReadValue), BSU8 (Address), BSU32 (Mask), BSU32 (Value)

/// GNB SMU restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT32                       Address;        ///< GNB SMU index
  UINT32                       Value;          ///< Value to write
} CS_GNB_SMU;
#define MAKE_CS_GNB_SMU(SaveReadValue, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbSmuIndex), SaveReadValue), BSU32 (Address), BSU32 (Value)

/// Specific GNB SMU bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT32                       Address;        ///< GNB SMU index
  UINT32                       Mask;           ///< Specifies the bits to isolate
  UINT32                       Value;          ///< Value to write
} CS_GNB_SMU_BITS;
#define MAKE_CS_GNB_SMU_BITS(SaveReadValue, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbSmuIndexBits), SaveReadValue), BSU32 (Address), BSU32 (Mask), BSU32 (Value)

/// GNB Device restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Device;         ///< Device to index
  UINT8                        Address;        ///< Offset within Device
  UINT32                       Value;          ///< Value to write
} CS_GNB_DEV;
#define MAKE_CS_GNB_DEV(SaveReadValue, Device, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbDevIndex), SaveReadValue), BSU8 (Device), BSU8 (Address), BSU32 (Value)

/// Specific GNB Device bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Device;         ///< Device to index
  UINT8                        Address;        ///< Offset within Device
  UINT32                       Mask;           ///< Specifies the bits to isolate
  UINT32                       Value;          ///< Value to write
} CS_GNB_DEV_BITS;
#define MAKE_CS_GNB_DEV_BITS(SaveReadValue, Device, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbDevIndexBits), SaveReadValue), BSU8 (Device), BSU8 (Address), BSU32 (Mask), BSU32 (Value)

/// GNB GBIF restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT32                       Address;        ///< GNB GBIF register address
  UINT32                       Value;          ///< Value to write
} CS_GNB_GBIF;
#define MAKE_CS_GNB_GBIF(SaveReadValue, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbGbifIndex), SaveReadValue), BSU32 (Address), BSU32 (Value)

/// Specific GNB GBIF bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT32                       Address;        ///< GNB GBIF register address
  UINT32                       Mask;           ///< Specifies the bits to isolate
  UINT32                       Value;          ///< Value to write
} CS_GNB_GBIF_BITS;
#define MAKE_CS_GNB_GBIF_BITS(SaveReadValue, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbGbifIndexBits), SaveReadValue), BSU32 (Address), BSU32 (Mask), BSU32 (Value)

/// GNB link restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Block;          ///< GNB Link block
  UINT8                        Frame;          ///< GNB Link frame
  UINT16                       Address;        ///< Offset within Block / Frame
  UINT32                       Value;          ///< Value to write
} CS_GNB_LINK;
#define MAKE_CS_GNB_LINK(SaveReadValue, Block, Frame, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbLinkIndex), SaveReadValue), BSU8 (Block), BSU8 (Frame), BSU16 (Address), BSU32 (Value)

/// Specific GNB link bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Block;          ///< GNB Link block
  UINT8                        Frame;          ///< GNB Link frame
  UINT16                       Address;        ///< Offset within Block / Frame
  UINT32                       Mask;           ///< Specifies the bits to isolate
  UINT32                       Value;          ///< Value to write
} CS_GNB_LINK_BITS;
#define MAKE_CS_GNB_LINK_BITS(SaveReadValue, Block, Frame, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbLinkIndexBits), SaveReadValue), BSU8 (Block), BSU8 (Frame), BSU16 (Address), BSU32 (Mask), BSU32 (Value)

/// GNB IOAPIC restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Address;        ///< GNB IOAPIC index
  UINT32                       Value;          ///< Value to write
} CS_GNB_IOAPIC;
#define MAKE_CS_GNB_IOAPIC(SaveReadValue, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbIoapicIndex), SaveReadValue), BSU8 (Address), BSU32 (Value)

/// Specific GNB IOAPIC bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Address;        ///< GNB IOAPIC index
  UINT32                       Mask;           ///< Specifies the bits to isolate
  UINT32                       Value;          ///< Value to write
} CS_GNB_IOAPIC_BITS;
#define MAKE_CS_GNB_IOAPIC_BITS(SaveReadValue, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbIoapicIndexBits), SaveReadValue), BSU8 (Address), BSU32 (Mask), BSU32 (Value)

/// GNB IOMMU L2 restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Address;        ///< GNB IOMMU L2 index
  UINT32                       Value;          ///< Value to write
} CS_GNB_IOMMU_L2;
#define MAKE_CS_GNB_IOMMU_L2(SaveReadValue, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (IommuL2Index), SaveReadValue), BSU8 (Address), BSU32 (Value)

/// Specific GNB IOMMU L2 bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Address;        ///< GNB IOMMU L2 index
  UINT32                       Mask;           ///< Specifies the bits to isolate
  UINT32                       Value;          ///< Value to write
} CS_GNB_IOMMU_L2_BITS;
#define MAKE_CS_GNB_IOMMU_L2_BITS(SaveReadValue, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (IommuL2IndexBits), SaveReadValue), BSU8 (Address), BSU32 (Mask), BSU32 (Value)

/// GNB IOMMU L1 restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Select;         ///< GNB IOMMU L1 device select
  UINT16                       Address;        ///< Offset within Select
  UINT32                       Value;          ///< Value to write
} CS_GNB_IOMMU_L1;
#define MAKE_CS_GNB_IOMMU_L1(SaveReadValue, Select, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (IommuL1Index), SaveReadValue), BSU8 (Select), BSU16 (Address), BSU32 (Value)

/// Specific GNB IOMMU L1 bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Select;         ///< GNB IOMMU L1 device select
  UINT16                       Address;        ///< Offset within Select
  UINT32                       Mask;           ///< Specifies the bits to isolate
  UINT32                       Value;          ///< Value to write
} CS_GNB_IOMMU_L1_BITS;
#define MAKE_CS_GNB_IOMMU_L1_BITS(SaveReadValue, Select, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (IommuL1IndexBits), SaveReadValue), BSU8 (Select), BSU16 (Address), BSU32 (Mask), BSU32 (Value)

/// GNB root port restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Function;       ///< PCI address Function
  UINT8                        Address;        ///< Offset within Function
  UINT32                       Value;          ///< Value to write
} CS_GNB_ROOT_PORT;
#define MAKE_CS_GNB_ROOT_PORT(SaveReadValue, Function, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbRootPortIndex), SaveReadValue), BSU8 (Function), BSU8 (Address), BSU32 (Value)

/// Specific GNB root port bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Function;       ///< PCI address Function
  UINT8                        Address;        ///< Offset within Function
  UINT32                       Mask;           ///< Specifies the bits to isolate
  UINT32                       Value;          ///< Value to write
} CS_GNB_ROOT_PORT_BITS;
#define MAKE_CS_GNB_ROOT_PORT_BITS(SaveReadValue, Function, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbRootPortIndexBits), SaveReadValue), BSU8 (Function), BSU8 (Address), BSU32 (Mask), BSU32 (Value)

/// GNB GMM restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT32                       Address;        ///< Offset in GMM space
  UINT32                       Value;          ///< Value to write
} CS_GNB_GMM;
#define MAKE_CS_GNB_GMM(SaveReadValue, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbGmm), SaveReadValue), BSU32 (Address), BSU32 (Value)

/// Specific GNB GMM bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT32                       Address;        ///< Offset in GMM space
  UINT32                       Mask;           ///< Specifies the bits to isolate
  UINT32                       Value;          ///< Value to write
} CS_GNB_GMM_BITS;
#define MAKE_CS_GNB_GMM_BITS(SaveReadValue, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GnbGmmBits), SaveReadValue), BSU32 (Address), BSU32 (Mask), BSU32 (Value)

/// FCH AB link restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT16                       Address;        ///< Offset within AB block
  UINT32                       Value;          ///< Value to write
} CS_FCH_AB;
#define MAKE_CS_FCH_AB(SaveReadValue, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (FchAb), SaveReadValue), BSU16 (Address), BSU32 (Value)

/// Specific FCH AB link bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT16                       Address;        ///< Offset within AB block
  UINT32                       Mask;           ///< Specifies the bits to isolate
  UINT32                       Value;          ///< Value to write
} CS_FCH_AB_BITS;
#define MAKE_CS_FCH_AB_BITS(SaveReadValue, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (FchAbBits), SaveReadValue), BSU16 (Address), BSU32 (Mask), BSU32 (Value)

/// FCH 32bit PM IO restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Address;        ///< Offset within PM IO space
  UINT32                       Value;          ///< Value to write
} CS_FCH_PMIO_DWORD;
#define MAKE_CS_FCH_PMIO_DWORD(SaveReadValue, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (FchPmioDword), SaveReadValue), BSU8 (Address), BSU32 (Value)

/// Specific FCH 32bit PM IO bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Address;        ///< Offset within PM IO space
  UINT32                       Mask;           ///< Specifies the bits to isolate
  UINT32                       Value;          ///< Value to write
} CS_FCH_PMIO_DWORD_BITS;
#define MAKE_CS_FCH_PMIO_DWORD_BITS(SaveReadValue, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (FchPmioDwordBits), SaveReadValue), BSU8 (Address), BSU32 (Mask), BSU32 (Value)

/// FCH 16bit PM IO restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Address;        ///< Offset within PM IO space
  UINT16                       Value;          ///< Value to write
} CS_FCH_PMIO_WORD;
#define MAKE_CS_FCH_PMIO_WORD(SaveReadValue, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (FchPmioWord), SaveReadValue), BSU8 (Address), BSU16 (Value)

/// Specific FCH 16bit PM IO bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Address;        ///< Offset within PM IO space
  UINT16                       Mask;           ///< Specifies the bits to isolate
  UINT16                       Value;          ///< Value to write
} CS_FCH_PMIO_WORD_BITS;
#define MAKE_CS_FCH_PMIO_WORD_BITS(SaveReadValue, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (FchPmioWordBits), SaveReadValue), BSU8 (Address), BSU16 (Mask), BSU16 (Value)

/// FCH 8bit PM IO restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Address;        ///< Offset within PM IO space
  UINT8                        Value;          ///< Value to write
} CS_FCH_PMIO_BYTE;
#define MAKE_CS_FCH_PMIO_BYTE(SaveReadValue, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (FchPmioByte), SaveReadValue), BSU8 (Address), BSU8 (Value)

/// Specific FCH 8bit PM IO bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Address;        ///< Offset within PM IO space
  UINT8                        Mask;           ///< Specifies the bits to isolate
  UINT8                        Value;          ///< Value to write
} CS_FCH_PMIO_BYTE_BITS;
#define MAKE_CS_FCH_PMIO_BYTE_BITS(SaveReadValue, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (FchPmioByteBits), SaveReadValue), BSU8 (Address), BSU8 (Mask), BSU8 (Value)

/// FCH PM2 IO restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Address;        ///< Offset within PM2 IO space
  UINT8                        Value;          ///< Value to write
} CS_FCH_PMIO2;
#define MAKE_CS_FCH_PMIO2(SaveReadValue, Address, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (FchPmio2), SaveReadValue), BSU8 (Address), BSU8 (Value)

/// Specific FCH PM2 IO bits restoration entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT8                        Address;        ///< Offset within PM2 IO space
  UINT8                        Mask;           ///< Specifies the bits to isolate
  UINT8                        Value;          ///< Value to write
} CS_FCH_PMIO2_BITS;
#define MAKE_CS_FCH_PMIO2_BITS(SaveReadValue, Address, Mask, Value) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (FchPmio2Bits), SaveReadValue), BSU8 (Address), BSU8 (Mask), BSU8 (Value)

/// Restoration function dispatch entry
typedef struct {
  CS_RESTORATION_ENTRY_HEADER  Header;         ///< Common header
  UINT16                       FunctionIndex;  ///< Enum value associated with an entry in LIST_OF_DISPATCHABLE_FUNCTIONS
  UINT32                       Context;        ///< Optional data to be passed to the function
} CS_GENERIC_DISPATCH;
#define MAKE_CS_GENERIC_DISPATCH(SaveReadValue, FunctionIndex, Context) MAKE_CS_RESTORATION_ENTRY_HEADER (MAKE_CS_ENTRY_TYPES (GenericDispatch), SaveReadValue), BSU16 (FunctionIndex), BSU32 (Context)


/*----------------------------------------------------------------------------------------
 *                          F U N C T I O N S     P R O T O T Y P E
 *----------------------------------------------------------------------------------------
 */

/**
 * Entry type implementer function to save a register during initial boot.
 *
 * @param[in]       CurrentEntry        The type specific entry data to be implemented.
 * @param[in]       StdHeader           Config params for library, services.
 */
typedef CS_RESTORATION_ENTRY_HEADER* F_PROCESS_RESTORATION_TABLE_ENTRY (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  );
/// Reference to a method
typedef F_PROCESS_RESTORATION_TABLE_ENTRY *PF_PROCESS_RESTORATION_TABLE_ENTRY;

/**
 * Entry type implementer function to restore a register during resume.
 *
 * @param[in]       CurrentEntry        The type specific entry data to be implemented.
 */
typedef CS_RESTORATION_ENTRY_HEADER* F_RESTORE_FROM_TABLE_ENTRY (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  );
/// Reference to a method
typedef F_RESTORE_FROM_TABLE_ENTRY *PF_RESTORE_FROM_TABLE_ENTRY;

/**
 * Generic dispatch restoration function.
 *
 * @param[in]       Context             Optional entry specific data
 */
typedef VOID F_GENERIC_DISPATCH_FUNCTION_RESTORE (
  IN       UINT32  Context
  );
/// Reference to a method
typedef F_GENERIC_DISPATCH_FUNCTION_RESTORE *PF_GENERIC_DISPATCH_FUNCTION_RESTORE;


#endif  // _F16_ML_CONNECTED_STANDBY_H_
