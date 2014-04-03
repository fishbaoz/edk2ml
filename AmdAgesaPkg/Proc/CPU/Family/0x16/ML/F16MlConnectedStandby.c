/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_16 Connected Standby Initialization
 *
 * Enables connected standby.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/0x16/ML
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
#include "GeneralServices.h"
#include "cpuFamilyTranslation.h"
#include "cpuFeatures.h"
#include "cpuConnectedStandby.h"
#include "F16MlConnectedStandby.h"
#include "cpuApicUtilities.h"
#include "Gnb.h"
#include "GnbLib.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X16_ML_F16MLCONNECTEDSTANDBY_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define  CURRENT_VALUE       (0)
#define  NOT_APPLICABLE      (FALSE)
#define  NO_CONTEXT_REQUIRED (0)

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


CONST UINT8 ROMDATA F16MlCsRestoreTable[] = {
  // Type                        Read Value       Address                        Mask        Value           Function Index             Function Context
  MAKE_CS_FCH_PMIO_WORD         (TRUE,            0xE0,                                      CURRENT_VALUE),
  MAKE_CS_GENERIC_DISPATCH      (NOT_APPLICABLE,                                                             CsProcEnableFchAbEntries,  NO_CONTEXT_REQUIRED),
  MAKE_CS_FCH_AB_BITS           (FALSE,           0x80,                          0xFFC0FFFE, 0x00060001),
  MAKE_CS_FCH_AB                (TRUE,            0x10054,                                   CURRENT_VALUE),
  MAKE_CS_FCH_AB                (TRUE,            0x94,                                      CURRENT_VALUE),
  MAKE_CS_FCH_AB_BITS           (FALSE,           0x90,                          0xFFDFFFFF, 0x00200000),
  MAKE_CS_FCH_AB_BITS           (FALSE,           0x10090,                       0xFFFEFFFF, 0x00010000),
  MAKE_CS_FCH_AB_BITS           (FALSE,           0x58,                          0x5FFFFFFF, 0xA0000000),
  MAKE_CS_FCH_AB_BITS           (FALSE,           0x54,                          0xFE00FFEA, 0x01100014),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,0,0,0x48),     0xFFFFFF7F, CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0,0,0x4C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0,0,0x50),                 CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x00,                                      CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x0D,                                      CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x16,                                      CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x17,                                      CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x18,                                      CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x19,                                      CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x1A,                                      CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x1D,                                      CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x1F,                                      CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x22,                                      CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x23,                                      CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x30,                                      CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x31,                                      CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x32,                                      CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x33,                                      CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x34,                                      CURRENT_VALUE),
  MAKE_CS_GNB_MISC              (TRUE,            0x46,                                      CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0,0,0x7C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0,0,0x84),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0,0,0x90),                 CURRENT_VALUE),
  MAKE_CS_GNB_ORB_CFG           (TRUE,            0x06,                                      CURRENT_VALUE),
  MAKE_CS_GNB_ORB_CFG           (TRUE,            0x07,                                      CURRENT_VALUE),
  MAKE_CS_GNB_ORB_CFG           (TRUE,            0x08,                                      CURRENT_VALUE),
  MAKE_CS_GNB_ORB_CFG           (TRUE,            0x09,                                      CURRENT_VALUE),
  MAKE_CS_GNB_ORB_CFG           (TRUE,            0x0C,                                      CURRENT_VALUE),
  MAKE_CS_GNB_ORB_CFG           (TRUE,            0x1E,                                      CURRENT_VALUE),
  MAKE_CS_GNB_ORB_CFG           (TRUE,            0x26,                                      CURRENT_VALUE),
  MAKE_CS_GNB_ORB_CFG           (TRUE,            0x27,                                      CURRENT_VALUE),
  MAKE_CS_GNB_ORB_CFG           (TRUE,            0x28,                                      CURRENT_VALUE),
  MAKE_CS_GNB_ORB_CFG           (TRUE,            0x2C,                                      CURRENT_VALUE),
  MAKE_CS_GNB_ORB_CFG           (TRUE,            0x37,                                      CURRENT_VALUE),
  MAKE_CS_GNB_ORB_CFG           (TRUE,            0x3A,                                      CURRENT_VALUE),
  MAKE_CS_GNB_ORB_CFG           (TRUE,            0x3B,                                      CURRENT_VALUE),
  MAKE_CS_GNB_ORB_CFG           (TRUE,            0x49,                                      CURRENT_VALUE),
  MAKE_CS_GNB_ORB_CFG           (TRUE,            0x4A,                                      CURRENT_VALUE),
  MAKE_CS_GNB_DEV               (TRUE,            0x11, 0x01,                                CURRENT_VALUE),
  MAKE_CS_GNB_DEV               (TRUE,            0x12, 0x01,                                CURRENT_VALUE),
  MAKE_CS_GNB_DEV               (TRUE,            0x13, 0x01,                                CURRENT_VALUE),
  MAKE_CS_GNB_DEV               (TRUE,            0x14, 0x01,                                CURRENT_VALUE),
  MAKE_CS_GNB_DEV               (TRUE,            0x15, 0x01,                                CURRENT_VALUE),
  MAKE_CS_GNB_GBIF_BITS         (TRUE,            0x013014B6,                    0xFFFFDAFF, CURRENT_VALUE),
  MAKE_CS_GNB_GBIF_BITS         (TRUE,            0x013014AC,                    0xFFFEFFFF, CURRENT_VALUE),
  MAKE_CS_GNB_GBIF_BITS         (FALSE,           0x013014BE,                    0xFFEFFFFF, 0x00100000),
  MAKE_CS_GNB_GBIF_BITS         (FALSE,           0x01301486,                    0xFFFFFFFB, 0x00000004),
  MAKE_CS_GNB_GBIF_BITS         (FALSE,           0x01091518,                    0xFFFFFFFE, 0x00000001),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x10, 0x0010,            0xFFF1FF2F, CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x10, 0x0011,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x10, 0x0012,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x10, 0x0013,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x10, 0x0017,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x10, 0x0018,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x20, 0x0004,            0xFFFCFFFF, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x20, 0x4450,            0xBFFFFF00, CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x0046,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x0080,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x0800,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x0900,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x0A00,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x0B00,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x0C00,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x30, 0x0803,            0xFFFFFFDF, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x30, 0x0903,            0xFFFFFFDF, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x30, 0x0A03,            0xFFFFFFDF, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x30, 0x0B03,            0xFFFFFFDF, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x30, 0x0C03,            0xFFFFFFDF, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x30, 0x0804,            0xFFFFFFF1, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x30, 0x0904,            0xFFFFFFF1, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x30, 0x0A04,            0xFFFFFFF1, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x30, 0x0B04,            0xFFFFFFF1, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x30, 0x0C04,            0xFFFFFFF1, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x30, 0x0805,            0x00FFFFFF, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x30, 0x0905,            0x00FFFFFF, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x30, 0x0A05,            0x00FFFFFF, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x30, 0x0B05,            0x00FFFFFF, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x30, 0x0C05,            0x00FFFFFF, CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x8002,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x8011,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x8012,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x8013,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x8014,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x8015,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x8016,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x8021,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x8022,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x8023,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x8024,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x8025,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x8026,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x8027,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x8028,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x8029,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK              (TRUE,            0x01, 0x30, 0x80F1,                        CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x40, 0x0002,            0xFFFFFFFE, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x40, 0x0010,            0xFFFFE1F0, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x40, 0x0011,            0xFFFFFFF0, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x40, 0x001C,            0xFFFFF800, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x40, 0x0020,            0xFFFFFDFF, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x40, 0x0040,            0xFFFF3FFF, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x40, 0x00B0,            0xFFFFFFDB, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x40, 0x00C0,            0x8FFFFFFF, CURRENT_VALUE),
  MAKE_CS_GNB_LINK_BITS         (TRUE,            0x01, 0x40, 0x00C1,            0xFFFFFFFC, CURRENT_VALUE),
  MAKE_CS_GNB_IOAPIC            (TRUE,            0x01,                                      CURRENT_VALUE),
  MAKE_CS_GNB_IOAPIC            (TRUE,            0x02,                                      CURRENT_VALUE),
  MAKE_CS_GNB_IOAPIC            (TRUE,            0x00,                                      CURRENT_VALUE),
  MAKE_CS_GNB_IOAPIC            (TRUE,            0x0F,                                      CURRENT_VALUE),
  MAKE_CS_GNB_IOAPIC            (TRUE,            0x10,                                      CURRENT_VALUE),
  MAKE_CS_GNB_IOAPIC            (TRUE,            0x11,                                      CURRENT_VALUE),
  MAKE_CS_GNB_IOAPIC            (TRUE,            0x12,                                      CURRENT_VALUE),
  MAKE_CS_GNB_IOAPIC            (TRUE,            0x13,                                      CURRENT_VALUE),
  MAKE_CS_GNB_IOAPIC            (TRUE,            0x14,                                      CURRENT_VALUE),
  MAKE_CS_GNB_IOAPIC            (TRUE,            0x30,                                      CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,0,0x0C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,0,0x10),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,0,0x14),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,0,0x18),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,0,0x1C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,0,0x24),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,0,0x3C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,0,0x4C),                 CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,1,0,0x60),     0xFFFFF6E0, CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,0,0xA4),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,0,0xA8),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,0,0xAC),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,0,0x108),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,0,0x10C),                CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,1,0,0x04),     0xFFFFFFF8, CURRENT_VALUE),
  MAKE_CS_GENERIC_DISPATCH      (NOT_APPLICABLE,                                                             CsProcEnableGnbGmmEntries, NO_CONTEXT_REQUIRED),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,1,0x0C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,1,0x10),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,1,0x3C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,1,0x4C),                 CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,1,1,0x60),     0xFFFFF6E0, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,1,1,0xA0),     0xFF8EFFFF, CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,1,0xA4),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,1,0xA8),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,1,0xAC),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,1,0x108),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,1,1,0x10C),                CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,1,1,0x04),     0xFFFFFAB8, CURRENT_VALUE),
  MAKE_CS_GNB_GMM_BITS          (TRUE,            0x00002004,                    0xFFFF0FFF, CURRENT_VALUE),
  MAKE_CS_GNB_GMM_BITS          (TRUE,            0x00002008,                    0xFFFFF000, CURRENT_VALUE),
  MAKE_CS_GNB_GMM               (TRUE,            0x00002024,                                CURRENT_VALUE),
  MAKE_CS_GNB_GMM               (TRUE,            0x00002068,                                CURRENT_VALUE),
  MAKE_CS_GNB_GMM               (TRUE,            0x00002C04,                                CURRENT_VALUE),
  MAKE_CS_GNB_GMM_BITS          (TRUE,            0x00003508,                    0xFFFFFFFE, CURRENT_VALUE),
  MAKE_CS_GNB_GMM_BITS          (TRUE,            0x0000350C,                    0xFFFEF7FF, CURRENT_VALUE),
  MAKE_CS_GNB_GMM               (TRUE,            0x00005428,                                CURRENT_VALUE),
  MAKE_CS_GNB_GMM_BITS          (TRUE,            0x00006464,                    0xFDFFFFEE, CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,0,0x40),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x0C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x0C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x0C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x0C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x0C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x18),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x18),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x18),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x18),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x18),                 CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,1,0x1C),     0xFFFF0F0F, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,2,0x1C),     0xFFFF0F0F, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,3,0x1C),     0xFFFF0F0F, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,4,0x1C),     0xFFFF0F0F, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,5,0x1C),     0xFFFF0F0F, CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x20),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x20),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x20),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x20),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x20),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x24),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x24),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x24),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x24),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x24),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x28),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x28),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x28),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x28),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x28),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x2C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x2C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x2C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x2C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x2C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x30),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x30),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x30),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x30),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x30),                 CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,1,0x3C),     0xFFE0F800, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,2,0x3C),     0xFFE0F800, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,3,0x3C),     0xFFE0F800, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,4,0x3C),     0xFFE0F800, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,5,0x3C),     0xFFE0F800, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,1,0x60),     0xFFFF8600, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,2,0x60),     0xFFFF8600, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,3,0x60),     0xFFFF8600, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,4,0x60),     0xFFFF8600, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,5,0x60),     0xFFFF8600, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,1,0x68),     0xFFFFF03C, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,2,0x68),     0xFFFFF03C, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,3,0x68),     0xFFFFF03C, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,4,0x68),     0xFFFFF03C, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,5,0x68),     0xFFFFF03C, CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x6C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x6C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x6C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x6C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x6C),                 CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,1,0x70),     0xFFFFEFFF, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,2,0x70),     0xFFFFEFFF, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,3,0x70),     0xFFFFEFFF, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,4,0x70),     0xFFFFEFFF, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,5,0x70),     0xFFFFEFFF, CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x74),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x74),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x74),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x74),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x74),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x80),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x80),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x80),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x80),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x80),                 CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,1,0x88),     0xFFFFF040, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,2,0x88),     0xFFFFF040, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,3,0x88),     0xFFFFF040, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,4,0x88),     0xFFFFF040, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,5,0x88),     0xFFFFF040, CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0xA0),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0xA0),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0xA0),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0xA0),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0xA0),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0xA4),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0xA4),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0xA4),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0xA4),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0xA4),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0xA8),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0xA8),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0xA8),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0xA8),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0xA8),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0xAC),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0xAC),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0xAC),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0xAC),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0xAC),                 CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,1,0x04),     0xFFFFFAB8, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,2,0x04),     0xFFFFFAB8, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,3,0x04),     0xFFFFFAB8, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,4,0x04),     0xFFFFFAB8, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,2,5,0x04),     0xFFFFFAB8, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x01, 0x20,                    0xFFFF7FFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x02, 0x20,                    0xFFFF7FFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x03, 0x20,                    0xFFFF7FFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x04, 0x20,                    0xFFFF7FFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x05, 0x20,                    0xFFFF7FFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x01, 0x6A,                    0xFFFFFFFE, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x02, 0x6A,                    0xFFFFFFFE, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x03, 0x6A,                    0xFFFFFFFE, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x04, 0x6A,                    0xFFFFFFFE, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x05, 0x6A,                    0xFFFFFFFE, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x01, 0x70,                    0xFFF0FFFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x02, 0x70,                    0xFFF0FFFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x03, 0x70,                    0xFFF0FFFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x04, 0x70,                    0xFFF0FFFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x05, 0x70,                    0xFFF0FFFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x01, 0xA0,                    0xFF7F000F, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x02, 0xA0,                    0xFF7F000F, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x03, 0xA0,                    0xFF7F000F, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x04, 0xA0,                    0xFF7F000F, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x05, 0xA0,                    0xFF7F000F, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x01, 0xA1,                    0xFFFFF7FF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x02, 0xA1,                    0xFFFFF7FF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x03, 0xA1,                    0xFFFFF7FF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x04, 0xA1,                    0xFFFFF7FF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x05, 0xA1,                    0xFFFFF7FF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x01, 0xA3,                    0xFFFFFDFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x02, 0xA3,                    0xFFFFFDFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x03, 0xA3,                    0xFFFFFDFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x04, 0xA3,                    0xFFFFFDFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x05, 0xA3,                    0xFFFFFDFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x01, 0xB1,                    0xFFE7FFFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x02, 0xB1,                    0xFFE7FFFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x03, 0xB1,                    0xFFE7FFFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x04, 0xB1,                    0xFFE7FFFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x05, 0xB1,                    0xFFE7FFFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x01, 0xB5,                    0xBFFFFFF8, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x02, 0xB5,                    0xBFFFFFF8, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x03, 0xB5,                    0xBFFFFFF8, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x04, 0xB5,                    0xBFFFFFF8, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x05, 0xB5,                    0xBFFFFFF8, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x01, 0xC0,                    0xFFF85FFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x02, 0xC0,                    0xFFF85FFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x03, 0xC0,                    0xFFF85FFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x04, 0xC0,                    0xFFF85FFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT_BITS    (TRUE,            0x05, 0xC0,                    0xFFF85FFF, CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT         (TRUE,            0x01, 0xC1,                                CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT         (TRUE,            0x02, 0xC1,                                CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT         (TRUE,            0x03, 0xC1,                                CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT         (TRUE,            0x04, 0xC1,                                CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT         (TRUE,            0x05, 0xC1,                                CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT         (TRUE,            0x01, 0xD0,                                CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT         (TRUE,            0x02, 0xD0,                                CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT         (TRUE,            0x03, 0xD0,                                CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT         (TRUE,            0x04, 0xD0,                                CURRENT_VALUE),
  MAKE_CS_GNB_ROOT_PORT         (TRUE,            0x05, 0xD0,                                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x108),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x108),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x108),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x108),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x108),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x10C),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x10C),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x10C),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x10C),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x10C),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x158),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x158),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x158),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x158),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x158),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x15C),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x15C),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x15C),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x15C),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x15C),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x164),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x164),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x164),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x164),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x164),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x168),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x168),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x168),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x168),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x168),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,1,0x17C),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,2,0x17C),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,3,0x17C),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,4,0x17C),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,2,5,0x17C),                CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,8,0,0x10),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,8,0,0x14),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,8,0,0x18),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,8,0,0x1C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,8,0,0x24),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,8,0,0x3C),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,8,0,0x40),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,8,0,0x44),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,8,0,0x48),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,8,0,0x50),                 CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,8,0,0x60),                 CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,8,0,0x70),     0xFFFFF6EF, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,8,0,0x04),     0xFFFFFBBD, CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,0,0x60),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,0,0x64),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,0,0x68),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,0,0x84),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,0,0x8C),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,0,0x90),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,0,0x94),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,0,0x110),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,0,0x168),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0x84),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0x80),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0x8C),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0x88),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0x94),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0x90),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0x9C),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0x98),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xA4),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xA0),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xAC),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xA8),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xB4),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xB0),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xBC),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xB8),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0x1A4),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0x1A0),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0x1AC),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0x1A8),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0x1B4),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0x1B0),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0x1BC),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0x1B8),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xC4),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xC0),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xCC),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xC8),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xD4),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xD0),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xDC),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xD8),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xE0),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xE4),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xE8),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xEC),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,1,0xF4),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0xDC),              CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x64),  0x8000FF3E, CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x68),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x6C),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x70),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x74),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x78),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x7C),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x84),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x88),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x8C),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0xA0),              CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0xA4),              CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,0x18,3,0xA8),  0x1FFFFFFF, CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0xD4),              CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (FALSE,           MAKE_SBDFO (0,0,0x18,3,0xD8),  0xFFFFFF8F, 0x00000040),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x140),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x144),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x148),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x160),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x168),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x17C),             CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x188), 0xF7FFFDFF, CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x1A0),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x1CC),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x1E4),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,3,0x200),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,0,0x6C),              CURRENT_VALUE), //Must come after buffer counts
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,4,0x110),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,4,0x118),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,4,0x11C),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,4,0x128),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,4,0x15C),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,4,0x16C),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,4,0x210),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,4,0x250),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,0,0x1DC),             CURRENT_VALUE), //Launches APs!
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,0x18,5,0x88),  0xFEFBBFFE, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,0x18,5,0x8C),  0x7FFF7FFF, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,0x18,5,0xB8),  0xFEFBBFFE, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,0x18,5,0xBC),  0x7FFF7FFF, CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,5,0xE8),              CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,0x18,5,0x128), 0xB75F0000, CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,0x18,5,0x12C), 0xFFFFFFC0, CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,5,0x160),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,5,0x164),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,5,0x168),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,5,0x16C),             CURRENT_VALUE),
  MAKE_CS_PCI_BITS              (TRUE,            MAKE_SBDFO (0,0,0x18,5,0x178), 0xFFF0F3F3, CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,5,0x17C),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,5,0x188),             CURRENT_VALUE),
  MAKE_CS_PCI                   (TRUE,            MAKE_SBDFO (0,0,0x18,5,0x18C),             CURRENT_VALUE),
  MAKE_CS_RESTORATION_ENTRY_HEADER (CsEndOfTable, NOT_APPLICABLE)
};


UINT16  AbIoAddress = 0xFFFF;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
BOOLEAN
STATIC
F16MlIsConnectedStandbySupported (
  IN       CONNECTED_STANDBY_FAMILY_SERVICES *ConnectedStandbyServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

AGESA_STATUS
STATIC
F16MlInitializeConnectedStandby (
  IN       CONNECTED_STANDBY_FAMILY_SERVICES *ConnectedStandbyServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       UINT64                 EntryPoint,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

VOID
STATIC
F16MlSetConnectedStandbyEnable (
  IN       AMD_CONFIG_PARAMS *StdHeader
  );
/*---------------------------------------------------------------------------------------*/
/**
 * BSC entry point for checking whether or not Connected Standby is supported.
 *
 * @param[in]  ConnectedStandbyServices  The current CPU's family services.
 * @param[in]  PlatformConfig          Contains the runtime modifiable feature input data.
 * @param[in]  StdHeader               Config handle for library and services.
 *
 * @retval     TRUE                    Connected Standby is supported.
 * @retval     FALSE                   Connected Standby is not supported.
 *
 */
BOOLEAN
STATIC
F16MlIsConnectedStandbySupported (
  IN       CONNECTED_STANDBY_FAMILY_SERVICES    *ConnectedStandbyServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  if (IsPspDevicePresent (StdHeader)) {
    return TRUE;
  }
  return FALSE;
}


/*---------------------------------------------------------------------------------------*/
/**
 * BSC entry point for for enabling Connected Standby.
 *
 * Set up MSRC001_0015[CSEnable] on each core
 *
 * @param[in]  ConnectedStandbyServices  The current CPU's family services.
 * @param[in]  PlatformConfig          Contains the runtime modifiable feature input data.
 * @param[in]  EntryPoint              Current CPU feature dispatch point.
 * @param[in]  StdHeader               Config handle for library and services.
 *
 * @retval     AGESA_SUCCESS           Always succeeds.
 *
 */
AGESA_STATUS
STATIC
F16MlInitializeConnectedStandby (
  IN       CONNECTED_STANDBY_FAMILY_SERVICES *ConnectedStandbyServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       UINT64                 EntryPoint,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  AP_TASK  TaskPtr;

  if ((EntryPoint & CPU_FEAT_AFTER_PM_INIT) != 0) {
    // Initialize MSRC001_0015[CSEnable] on each core
    TaskPtr.FuncAddress.PfApTask = F16MlSetConnectedStandbyEnable;
    TaskPtr.DataTransfer.DataSizeInDwords = 0;
    TaskPtr.ExeFlags = 0;
    ApUtilRunCodeOnAllLocalCoresAtEarly (&TaskPtr, StdHeader, NULL);
  }
  return AGESA_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Enable CSEnable on a family 16h Mullins core.
 *
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 */
VOID
STATIC
F16MlSetConnectedStandbyEnable (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT64  LocalMsrRegister;

  // Initialize MSRC001_0015[CSEnable] on each core
  LibAmdMsrRead (0xC0010015, &LocalMsrRegister, StdHeader);
  LocalMsrRegister |= BIT29;
  LibAmdMsrWrite (0xC0010015, &LocalMsrRegister, StdHeader);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a PCI entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessPciEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  CS_PCI    *PciEntry;
  PCI_ADDR  PciAddr;

  PciEntry = (CS_PCI *) CurrentEntry;
  if (PciEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = PciEntry->Address;
    LibAmdPciRead (AccessWidth32, PciAddr, &PciEntry->Value, StdHeader);
  }
  PciEntry++;
  return &PciEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a PCI bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessPciBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  PCI_ADDR     PciAddr;
  CS_PCI_BITS  *PciBitsEntry;

  PciBitsEntry = (CS_PCI_BITS *) CurrentEntry;
  if (PciBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = PciBitsEntry->Address;
    LibAmdPciRead (AccessWidth32, PciAddr, &PciBitsEntry->Value, StdHeader);
    PciBitsEntry->Value &= ~PciBitsEntry->Mask;
  }
  PciBitsEntry++;
  return &PciBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a MMIO entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessMmioEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  CS_MMIO  *MmioEntry;

  MmioEntry = (CS_MMIO *) CurrentEntry;
  if (MmioEntry->Header.SaveReadValue) {
    LibAmdMemRead (AccessWidth32, MmioEntry->Address, &MmioEntry->Value, StdHeader);
  }
  MmioEntry++;
  return &MmioEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a MMIO bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessMmioBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  CS_MMIO_BITS  *MmioBitsEntry;

  MmioBitsEntry = (CS_MMIO_BITS *) CurrentEntry;
  if (MmioBitsEntry->Header.SaveReadValue) {
    LibAmdMemRead (AccessWidth32, MmioBitsEntry->Address, &MmioBitsEntry->Value, StdHeader);
    MmioBitsEntry->Value &= ~MmioBitsEntry->Mask;
  }
  MmioBitsEntry++;
  return &MmioBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB miscellaneous entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbMiscIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  PCI_ADDR     PciAddr;
  CS_GNB_MISC  *GnbMiscEntry;

  GnbMiscEntry = (CS_GNB_MISC *) CurrentEntry;
  if (GnbMiscEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0x60);
    GnbLibPciIndirectRead (PciAddr.AddressValue, GnbMiscEntry->Address, AccessWidth32, &GnbMiscEntry->Value, StdHeader);
  }
  GnbMiscEntry++;
  return &GnbMiscEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB miscellaneous bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbMiscIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  PCI_ADDR          PciAddr;
  CS_GNB_MISC_BITS  *GnbMiscBitsEntry;

  GnbMiscBitsEntry = (CS_GNB_MISC_BITS *) CurrentEntry;
  if (GnbMiscBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0x60);
    GnbLibPciIndirectRead (PciAddr.AddressValue, GnbMiscBitsEntry->Address, AccessWidth32, &GnbMiscBitsEntry->Value, StdHeader);
    GnbMiscBitsEntry->Value &= ~GnbMiscBitsEntry->Mask;
  }
  GnbMiscBitsEntry++;
  return &GnbMiscBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB ORB entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbOrbCfgEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  PCI_ADDR        PciAddr;
  CS_GNB_ORB_CFG  *GnbOrbCfgEntry;

  GnbOrbCfgEntry = (CS_GNB_ORB_CFG *) CurrentEntry;
  if (GnbOrbCfgEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0x94);
    GnbLibPciIndirectRead (PciAddr.AddressValue, (UINT32) GnbOrbCfgEntry->Address, AccessWidth32, &GnbOrbCfgEntry->Value, StdHeader);
  }
  GnbOrbCfgEntry++;
  return &GnbOrbCfgEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB ORB bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbOrbCfgBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  PCI_ADDR             PciAddr;
  CS_GNB_ORB_CFG_BITS  *GnbOrbCfgBitsEntry;

  GnbOrbCfgBitsEntry = (CS_GNB_ORB_CFG_BITS *) CurrentEntry;
  if (GnbOrbCfgBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0x94);
    GnbLibPciIndirectRead (PciAddr.AddressValue, (UINT32) GnbOrbCfgBitsEntry->Address, AccessWidth32, &GnbOrbCfgBitsEntry->Value, StdHeader);
    GnbOrbCfgBitsEntry->Value &= ~GnbOrbCfgBitsEntry->Mask;
  }
  GnbOrbCfgBitsEntry++;
  return &GnbOrbCfgBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB SMU entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbSmuIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  PCI_ADDR    PciAddr;
  CS_GNB_SMU  *GnbSmuEntry;

  GnbSmuEntry = (CS_GNB_SMU *) CurrentEntry;
  if (GnbSmuEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xB8);
    GnbLibPciIndirectRead (PciAddr.AddressValue, GnbSmuEntry->Address, AccessWidth32, &GnbSmuEntry->Value, StdHeader);
  }
  GnbSmuEntry++;
  return &GnbSmuEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB SMU bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbSmuIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  PCI_ADDR         PciAddr;
  CS_GNB_SMU_BITS  *GnbSmuBitsEntry;

  GnbSmuBitsEntry = (CS_GNB_SMU_BITS *) CurrentEntry;
  if (GnbSmuBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xB8);
    GnbLibPciIndirectRead (PciAddr.AddressValue, GnbSmuBitsEntry->Address, AccessWidth32, &GnbSmuBitsEntry->Value, StdHeader);
    GnbSmuBitsEntry->Value &= ~GnbSmuBitsEntry->Mask;
  }
  GnbSmuBitsEntry++;
  return &GnbSmuBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB device entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbDevIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT32      Index;
  PCI_ADDR    PciAddr;
  CS_GNB_DEV  *GnbDevEntry;

  GnbDevEntry = (CS_GNB_DEV *) CurrentEntry;
  if (GnbDevEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xC8);
    Index = ((UINT32) GnbDevEntry->Device << 16) | (UINT32) GnbDevEntry->Address;
    GnbLibPciIndirectRead (PciAddr.AddressValue, Index, AccessWidth32, &GnbDevEntry->Value, StdHeader);
  }
  GnbDevEntry++;
  return &GnbDevEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB device bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbDevIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT32           Index;
  PCI_ADDR         PciAddr;
  CS_GNB_DEV_BITS  *GnbDevBitsEntry;

  GnbDevBitsEntry = (CS_GNB_DEV_BITS *) CurrentEntry;
  if (GnbDevBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xC8);
    Index = ((UINT32) GnbDevBitsEntry->Device << 16) | (UINT32) GnbDevBitsEntry->Address;
    GnbLibPciIndirectRead (PciAddr.AddressValue, Index, AccessWidth32, &GnbDevBitsEntry->Value, StdHeader);
    GnbDevBitsEntry->Value &= ~GnbDevBitsEntry->Mask;
  }
  GnbDevBitsEntry++;
  return &GnbDevBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB GBIF entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbGbifIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  PCI_ADDR     PciAddr;
  CS_GNB_GBIF  *GnbGbifEntry;

  GnbGbifEntry = (CS_GNB_GBIF *) CurrentEntry;
  if (GnbGbifEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xD0);
    GnbLibPciIndirectRead (PciAddr.AddressValue, GnbGbifEntry->Address, AccessWidth32, &GnbGbifEntry->Value, StdHeader);
  }
  GnbGbifEntry++;
  return &GnbGbifEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB GBIF bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbGbifIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  PCI_ADDR          PciAddr;
  CS_GNB_GBIF_BITS  *GnbGbifBitsEntry;

  GnbGbifBitsEntry = (CS_GNB_GBIF_BITS *) CurrentEntry;
  if (GnbGbifBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xD0);
    GnbLibPciIndirectRead (PciAddr.AddressValue, GnbGbifBitsEntry->Address, AccessWidth32, &GnbGbifBitsEntry->Value, StdHeader);
    GnbGbifBitsEntry->Value &= ~GnbGbifBitsEntry->Mask;
  }
  GnbGbifBitsEntry++;
  return &GnbGbifBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB link entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbLinkIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT32       Index;
  PCI_ADDR     PciAddr;
  CS_GNB_LINK  *GnbLinkEntry;

  GnbLinkEntry = (CS_GNB_LINK *) CurrentEntry;
  if (GnbLinkEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xE0);
    Index = (((UINT32) GnbLinkEntry->Block << 24) | ((UINT32) GnbLinkEntry->Frame << 16)) | (UINT32) GnbLinkEntry->Address;
    GnbLibPciIndirectRead (PciAddr.AddressValue, Index, AccessWidth32, &GnbLinkEntry->Value, StdHeader);
  }
  GnbLinkEntry++;
  return &GnbLinkEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB link bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbLinkIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT32            Index;
  PCI_ADDR          PciAddr;
  CS_GNB_LINK_BITS  *GnbLinkBitsEntry;

  GnbLinkBitsEntry = (CS_GNB_LINK_BITS *) CurrentEntry;
  if (GnbLinkBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xE0);
    Index = (((UINT32) GnbLinkBitsEntry->Block << 24) | ((UINT32) GnbLinkBitsEntry->Frame << 16)) | (UINT32) GnbLinkBitsEntry->Address;
    GnbLibPciIndirectRead (PciAddr.AddressValue, Index, AccessWidth32, &GnbLinkBitsEntry->Value, StdHeader);
    GnbLinkBitsEntry->Value &= ~GnbLinkBitsEntry->Mask;
  }
  GnbLinkBitsEntry++;
  return &GnbLinkBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB IOAPIC entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbIoapicIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT32         RegisterValue;
  PCI_ADDR       PciAddr;
  CS_GNB_IOAPIC  *GnbIoapicEntry;

  GnbIoapicEntry = (CS_GNB_IOAPIC *) CurrentEntry;
  if (GnbIoapicEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xF8);
    RegisterValue = (UINT32) GnbIoapicEntry->Address;
    LibAmdPciWrite (AccessWidth32, PciAddr, &RegisterValue, StdHeader);
    PciAddr.AddressValue += 4;
    LibAmdPciRead (AccessWidth32, PciAddr, &GnbIoapicEntry->Value, StdHeader);
  }
  GnbIoapicEntry++;
  return &GnbIoapicEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB IOAPIC bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbIoapicIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT32              RegisterValue;
  PCI_ADDR            PciAddr;
  CS_GNB_IOAPIC_BITS  *GnbIoapicBitsEntry;

  GnbIoapicBitsEntry = (CS_GNB_IOAPIC_BITS *) CurrentEntry;
  if (GnbIoapicBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xF8);
    RegisterValue = (UINT32) GnbIoapicBitsEntry->Address;
    LibAmdPciWrite (AccessWidth32, PciAddr, &RegisterValue, StdHeader);
    PciAddr.AddressValue += 4;
    LibAmdPciRead (AccessWidth32, PciAddr, &GnbIoapicBitsEntry->Value, StdHeader);
    GnbIoapicBitsEntry->Value &= ~GnbIoapicBitsEntry->Mask;
  }
  GnbIoapicBitsEntry++;
  return &GnbIoapicBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB IOMMU L2 entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessIommuL2IndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  PCI_ADDR         PciAddr;
  CS_GNB_IOMMU_L2  *GnbIommuL2Entry;

  GnbIommuL2Entry = (CS_GNB_IOMMU_L2 *) CurrentEntry;
  if (GnbIommuL2Entry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 2, 0xF0);
    GnbLibPciIndirectRead (PciAddr.AddressValue, (UINT32) GnbIommuL2Entry->Address, AccessWidth32, &GnbIommuL2Entry->Value, StdHeader);
  }
  GnbIommuL2Entry++;
  return &GnbIommuL2Entry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB IOMMU L2 bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessIommuL2IndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  PCI_ADDR              PciAddr;
  CS_GNB_IOMMU_L2_BITS  *GnbIommuL2BitsEntry;

  GnbIommuL2BitsEntry = (CS_GNB_IOMMU_L2_BITS *) CurrentEntry;
  if (GnbIommuL2BitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 2, 0xF0);
    GnbLibPciIndirectRead (PciAddr.AddressValue, (UINT32) GnbIommuL2BitsEntry->Address, AccessWidth32, &GnbIommuL2BitsEntry->Value, StdHeader);
    GnbIommuL2BitsEntry->Value &= ~GnbIommuL2BitsEntry->Mask;
  }
  GnbIommuL2BitsEntry++;
  return &GnbIommuL2BitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB IOMMU L1 entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessIommuL1IndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT32           Index;
  PCI_ADDR         PciAddr;
  CS_GNB_IOMMU_L1  *GnbIommuL1Entry;

  GnbIommuL1Entry = (CS_GNB_IOMMU_L1 *) CurrentEntry;
  if (GnbIommuL1Entry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 2, 0xF8);
    Index = ((UINT32) GnbIommuL1Entry->Select << 16) | (UINT32) GnbIommuL1Entry->Address;
    GnbLibPciIndirectRead (PciAddr.AddressValue, Index, AccessWidth32, &GnbIommuL1Entry->Value, StdHeader);
  }
  GnbIommuL1Entry++;
  return &GnbIommuL1Entry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB IOMMU L1 bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessIommuL1IndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT32                Index;
  PCI_ADDR              PciAddr;
  CS_GNB_IOMMU_L1_BITS  *GnbIommuL1BitsEntry;

  GnbIommuL1BitsEntry = (CS_GNB_IOMMU_L1_BITS *) CurrentEntry;
  if (GnbIommuL1BitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 2, 0xF8);
    Index = ((UINT32) GnbIommuL1BitsEntry->Select << 16) | (UINT32) GnbIommuL1BitsEntry->Address;
    GnbLibPciIndirectRead (PciAddr.AddressValue, Index, AccessWidth32, &GnbIommuL1BitsEntry->Value, StdHeader);
    GnbIommuL1BitsEntry->Value &= ~GnbIommuL1BitsEntry->Mask;
  }
  GnbIommuL1BitsEntry++;
  return &GnbIommuL1BitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB root port entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbRootPortIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT32            RegisterValue;
  PCI_ADDR          PciAddr;
  CS_GNB_ROOT_PORT  *GnbRootPortEntry;

  GnbRootPortEntry = (CS_GNB_ROOT_PORT *) CurrentEntry;
  if (GnbRootPortEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 2, 0, 0xE0);
    PciAddr.Address.Function = GnbRootPortEntry->Function;
    RegisterValue = (UINT32) GnbRootPortEntry->Address;
    LibAmdPciWrite (AccessWidth32, PciAddr, &RegisterValue, StdHeader);
    PciAddr.AddressValue += 4;
    LibAmdPciRead (AccessWidth32, PciAddr, &GnbRootPortEntry->Value, StdHeader);
  }
  GnbRootPortEntry++;
  return &GnbRootPortEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB root port bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbRootPortIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT32                 RegisterValue;
  PCI_ADDR               PciAddr;
  CS_GNB_ROOT_PORT_BITS  *GnbRootPortBitsEntry;

  GnbRootPortBitsEntry = (CS_GNB_ROOT_PORT_BITS *) CurrentEntry;
  if (GnbRootPortBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 2, 0, 0xE0);
    PciAddr.Address.Function = GnbRootPortBitsEntry->Function;
    RegisterValue = (UINT32) GnbRootPortBitsEntry->Address;
    LibAmdPciWrite (AccessWidth32, PciAddr, &RegisterValue, StdHeader);
    PciAddr.AddressValue += 4;
    LibAmdPciRead (AccessWidth32, PciAddr, &GnbRootPortBitsEntry->Value, StdHeader);
    GnbRootPortBitsEntry->Value &= ~GnbRootPortBitsEntry->Mask;
  }
  GnbRootPortBitsEntry++;
  return &GnbRootPortBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB GMM entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbGmmEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT32         *GmmRegister;
  UINT32         GmmBar;
  UINT64         MmioAddr;
  PCI_ADDR       PciAddr;
  CS_GNB_GMM     *GnbGmmEntry;

  GnbGmmEntry = (CS_GNB_GMM *) CurrentEntry;
  if (GnbGmmEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 1, 0, 0x24);
    LibAmdPciRead (AccessWidth32, PciAddr, &GmmBar, StdHeader);
    MmioAddr = (UINT64) (GmmBar & 0xFFFF0000);
    MmioAddr += GnbGmmEntry->Address;
    GmmRegister = (UINT32 *) MmioAddr;
    GnbGmmEntry->Value = *GmmRegister;
  }
  GnbGmmEntry++;
  return &GnbGmmEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB GMM bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGnbGmmBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT32            *GmmRegister;
  UINT32            GmmBar;
  UINT64            MmioAddr;
  PCI_ADDR          PciAddr;
  CS_GNB_GMM_BITS   *GnbGmmBitsEntry;

  GnbGmmBitsEntry = (CS_GNB_GMM_BITS *) CurrentEntry;
  if (GnbGmmBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 1, 0, 0x24);
    LibAmdPciRead (AccessWidth32, PciAddr, &GmmBar, StdHeader);
    MmioAddr = (UINT64) (GmmBar & 0xFFFF0000);
    MmioAddr += GnbGmmBitsEntry->Address;
    GmmRegister = (UINT32 *) MmioAddr;
    GnbGmmBitsEntry->Value = *GmmRegister & ~GnbGmmBitsEntry->Mask;
  }
  GnbGmmBitsEntry++;
  return &GnbGmmBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH AB link entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessFchAbEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT32     RegisterIndex;
  UINT64     MmioAddr;
  CS_FCH_AB  *FchAbEntry;

  FchAbEntry = (CS_FCH_AB *) CurrentEntry;
  if (FchAbEntry->Header.SaveReadValue) {
    if (AbIoAddress == 0xFFFF) {
      MmioAddr = 0xFED803E0;
      LibAmdMemRead (AccessWidth16, MmioAddr, &AbIoAddress, StdHeader);
    }
    RegisterIndex = FchAbEntry->Address + 0xC0000000;
    LibAmdIoWrite (AccessWidth32, AbIoAddress, &RegisterIndex, StdHeader);
    LibAmdIoRead (AccessWidth32, AbIoAddress + 4, &FchAbEntry->Value, StdHeader);
  }
  FchAbEntry++;
  return &FchAbEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH AB link bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessFchAbBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT32          RegisterIndex;
  UINT64          MmioAddr;
  CS_FCH_AB_BITS  *FchAbBitsEntry;

  FchAbBitsEntry = (CS_FCH_AB_BITS *) CurrentEntry;
  if (FchAbBitsEntry->Header.SaveReadValue) {
    if (AbIoAddress == 0xFFFF) {
      MmioAddr = 0xFED803E0;
      LibAmdMemRead (AccessWidth16, MmioAddr, &AbIoAddress, StdHeader);
    }
    RegisterIndex = FchAbBitsEntry->Address + 0xC0000000;
    LibAmdIoWrite (AccessWidth32, AbIoAddress, &RegisterIndex, StdHeader);
    LibAmdIoRead (AccessWidth32, AbIoAddress + 4, &FchAbBitsEntry->Value, StdHeader);
    FchAbBitsEntry->Value &= ~FchAbBitsEntry->Mask;
  }
  FchAbBitsEntry++;
  return &FchAbBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH 32bit PM IO entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessFchPmioDwordEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT64             MmioAddress;
  CS_FCH_PMIO_DWORD  *FchPmioDwordEntry;

  FchPmioDwordEntry = (CS_FCH_PMIO_DWORD *) CurrentEntry;
  if (FchPmioDwordEntry->Header.SaveReadValue) {
    MmioAddress = 0xFED80300 + FchPmioDwordEntry->Address;
    LibAmdMemRead (AccessWidth32, MmioAddress, &FchPmioDwordEntry->Value, StdHeader);
  }
  FchPmioDwordEntry++;
  return &FchPmioDwordEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH 32bit PM IO bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessFchPmioDwordBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT64                  MmioAddress;
  CS_FCH_PMIO_DWORD_BITS  *FchPmioDwordBitsEntry;

  FchPmioDwordBitsEntry = (CS_FCH_PMIO_DWORD_BITS *) CurrentEntry;
  if (FchPmioDwordBitsEntry->Header.SaveReadValue) {
    MmioAddress = 0xFED80300 + FchPmioDwordBitsEntry->Address;
    LibAmdMemRead (AccessWidth32, MmioAddress, &FchPmioDwordBitsEntry->Value, StdHeader);
    FchPmioDwordBitsEntry->Value &= ~FchPmioDwordBitsEntry->Mask;
  }
  FchPmioDwordBitsEntry++;
  return &FchPmioDwordBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH 16bit PM IO entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessFchPmioWordEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT64            MmioAddress;
  CS_FCH_PMIO_WORD  *FchPmioWordEntry;

  FchPmioWordEntry = (CS_FCH_PMIO_WORD *) CurrentEntry;
  if (FchPmioWordEntry->Header.SaveReadValue) {
    MmioAddress = 0xFED80300 + FchPmioWordEntry->Address;
    LibAmdMemRead (AccessWidth16, MmioAddress, &FchPmioWordEntry->Value, StdHeader);
  }
  FchPmioWordEntry++;
  return &FchPmioWordEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH 16bit PM IO bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessFchPmioWordBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT64                 MmioAddress;
  CS_FCH_PMIO_WORD_BITS  *FchPmioWordBitsEntry;

  FchPmioWordBitsEntry = (CS_FCH_PMIO_WORD_BITS *) CurrentEntry;
  if (FchPmioWordBitsEntry->Header.SaveReadValue) {
    MmioAddress = 0xFED80300 + FchPmioWordBitsEntry->Address;
    LibAmdMemRead (AccessWidth16, MmioAddress, &FchPmioWordBitsEntry->Value, StdHeader);
    FchPmioWordBitsEntry->Value &= ~FchPmioWordBitsEntry->Mask;
  }
  FchPmioWordBitsEntry++;
  return &FchPmioWordBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH 8bit PM IO entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessFchPmioByteEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT64            MmioAddress;
  CS_FCH_PMIO_BYTE  *FchPmioByteEntry;

  FchPmioByteEntry = (CS_FCH_PMIO_BYTE *) CurrentEntry;
  if (FchPmioByteEntry->Header.SaveReadValue) {
    MmioAddress = 0xFED80300 + FchPmioByteEntry->Address;
    LibAmdMemRead (AccessWidth8, MmioAddress, &FchPmioByteEntry->Value, StdHeader);
  }
  FchPmioByteEntry++;
  return &FchPmioByteEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH 8bit PM IO bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessFchPmioByteBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT64                 MmioAddress;
  CS_FCH_PMIO_BYTE_BITS  *FchPmioByteBitsEntry;

  FchPmioByteBitsEntry = (CS_FCH_PMIO_BYTE_BITS *) CurrentEntry;
  if (FchPmioByteBitsEntry->Header.SaveReadValue) {
    MmioAddress = 0xFED80300 + FchPmioByteBitsEntry->Address;
    LibAmdMemRead (AccessWidth8, MmioAddress, &FchPmioByteBitsEntry->Value, StdHeader);
    FchPmioByteBitsEntry->Value &= ~FchPmioByteBitsEntry->Mask;
  }
  FchPmioByteBitsEntry++;
  return &FchPmioByteBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH PM2 IO entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessFchPmio2Entry  (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT64        MmioAddress;
  CS_FCH_PMIO2  *FchPmio2Entry;

  FchPmio2Entry = (CS_FCH_PMIO2 *) CurrentEntry;
  if (FchPmio2Entry->Header.SaveReadValue) {
    MmioAddress = 0xFED80400 + FchPmio2Entry->Address;
    LibAmdMemRead (AccessWidth8, MmioAddress, &FchPmio2Entry->Value, StdHeader);
  }
  FchPmio2Entry++;
  return &FchPmio2Entry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH PM2 IO bits entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessFchPmio2BitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  UINT64             MmioAddress;
  CS_FCH_PMIO2_BITS  *FchPmio2BitsEntry;

  FchPmio2BitsEntry = (CS_FCH_PMIO2_BITS *) CurrentEntry;
  if (FchPmio2BitsEntry->Header.SaveReadValue) {
    MmioAddress = 0xFED80400 + FchPmio2BitsEntry->Address;
    LibAmdMemRead (AccessWidth8, MmioAddress, &FchPmio2BitsEntry->Value, StdHeader);
    FchPmio2BitsEntry->Value &= ~FchPmio2BitsEntry->Mask;
  }
  FchPmio2BitsEntry++;
  return &FchPmio2BitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a generic function dispatch entry on a family 16h Mullins core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F16MlProcessGenericDispatchEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  CS_GENERIC_DISPATCH  *GenericDispatchEntry;

  GenericDispatchEntry = (CS_GENERIC_DISPATCH *) CurrentEntry;
  // Maybe add a call on the save side to determine if the call is needed on resume
  GenericDispatchEntry++;
  return &GenericDispatchEntry->Header;
}


// Instantiate the array of processing functions
#define X(CsType) F16MlProcess##CsType##Entry,
CONST PF_PROCESS_RESTORATION_TABLE_ENTRY ROMDATA ProcessF16MlRestorationBufferFunctions [] =
{
  LIST_OF_REGISTER_SPACES
};
#undef X

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to build up the Connected Standby restoration table.
 *
 * @param[in]    ConnectedStandbyServices  Connected Standby services.
 * @param[in]    RestorationTable          Complete family specific restoration table
 * @param[in]    TableSize                 Size of family specific restoration table
 * @param[in]    StdHeader                 Config Handle for library, services.
 *
 * @return       Family specific error value.
 *
 */
AGESA_STATUS
STATIC
F16MlPrepareRestorationBuffer (
  IN       CONNECTED_STANDBY_FAMILY_SERVICES *ConnectedStandbyServices,
     OUT   VOID                              **RestorationTable,
     OUT   UINT32                            *TableSize,
  IN       AMD_CONFIG_PARAMS                 *StdHeader
  )
{
  CS_RESTORATION_ENTRY_HEADER  *CurrentEntry;

  *TableSize = sizeof (F16MlCsRestoreTable);
  *RestorationTable = (VOID *) &F16MlCsRestoreTable;

  for (CurrentEntry = (CS_RESTORATION_ENTRY_HEADER *) *RestorationTable; CurrentEntry->Type != CsEndOfTable; CurrentEntry = ProcessF16MlRestorationBufferFunctions [CurrentEntry->Type] (CurrentEntry, StdHeader)) {
    ASSERT (CurrentEntry->Type < CsEndOfTable);
  }
  return AGESA_SUCCESS;
}


CONST CONNECTED_STANDBY_FAMILY_SERVICES ROMDATA F16MlConnectedStandbySupport =
{
  0,
  F16MlIsConnectedStandbySupported,
  F16MlInitializeConnectedStandby,
  F16MlPrepareRestorationBuffer
};
