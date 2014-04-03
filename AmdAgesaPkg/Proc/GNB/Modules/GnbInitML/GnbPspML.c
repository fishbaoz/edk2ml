/* $NoKeywords:$ */
/**
 * @file
 *
 * GNB PSP access functions
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 282985 $   @e \$Date: 2014-01-20 12:02:23 -0600 (Mon, 20 Jan 2014) $
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
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  "AGESA.h"
#include  "Ids.h"
#include  "amdlib.h"
#include  "Gnb.h"
#include  "OptionGnb.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieConfig.h"
#include  "GnbTable.h"
#include  "GnbSmuInitLibV7.h"
#include  "GnbRegisterAccML.h"
#include  "GnbRegistersML.h"
#include  "GnbPspML.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITML_GNBPSPML_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------*/
/**
 * Get PSP directory base address
 *
 *
 *
 * @param[in, out]  Address  Pointer to PSP diretory base address
 * @retval     AGESA_STATUS
 */
AGESA_STATUS
GetPspDirBaseML (
  IN OUT   UINTN     *Address
  )
{
  AGESA_STATUS              Status;
  AGESA_STATUS              AgesaStatus;
  UINTN                     i;
  FIRMWARE_ENTRY_TABLE      *FirmwareTableBase;
  CONST UINTN RomSigAddrTable[] =
  {
    0xFFFA0000, //  --> 512KB base
    0xFFF20000, //  --> 1MB base
    0xFFE20000, //  --> 2MB base
    0xFFC20000, //  --> 4MB base
    0xFF820000, //  --> 8MB base
    0xFF020000  //  --> 16MB base
  };

  AgesaStatus = AGESA_SUCCESS;
  Status = AGESA_SUCCESS;
  for (i = 0; i < sizeof (RomSigAddrTable) / sizeof (UINTN); i++) {
    FirmwareTableBase  = (FIRMWARE_ENTRY_TABLE *) (UINTN) RomSigAddrTable[i];
    // Search flash for unique signature 0x55AA55AA
    if (FirmwareTableBase->Signature  == FIRMWARE_TABLE_SIGNATURE) {
      IDS_HDT_CONSOLE (GNB_TRACE, " PSP FirmwareTableBase @%08x\n", RomSigAddrTable[i]);
      *Address = (UINTN)(FirmwareTableBase->PspDirBase);
      break;
    }
  }

  if (*Address == NULL) {
    Status = AGESA_ERROR;
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
  }

  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get smu firmware
 *
 *
 * @param[in, out]  EntryAddress  Pointer to SMU firwmare
 * @param[in, out]  EntrySize  The size of SMU firwmare
 * @retval     AGESA_STATUS
 */
AGESA_STATUS
GnbGetSmuFirmwareML (
  IN OUT   UINTN        *EntryAddress,
  IN OUT   UINTN        *EntrySize
  )
{
  AGESA_STATUS      Status;
  AGESA_STATUS      AgesaStatus;
  PSP_DIRECTORY     *PspDir;
  UINTN             i;

  AgesaStatus = AGESA_SUCCESS;
  Status = AGESA_SUCCESS;
  PspDir = NULL;
  Status = GetPspDirBaseML ((UINTN *)&PspDir);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  for (i = 0; i < PspDir->Header.TotalEntries; i++) {
    if (PspDir->PspEntry[i].Type == SMU_OFFCHIP_FW) {
      *EntryAddress = (UINTN) PspDir->PspEntry[i].Location;
      *EntrySize = PspDir->PspEntry[i].Size;
      Status = AGESA_SUCCESS;
      AGESA_STATUS_UPDATE (Status, AgesaStatus);
      break;
    }
  }

  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get SCS data
 *
 *
 * @param[in, out]  EntryAddress  Pointer to SCS data
 * @param[in, out]  EntrySize  The size of SCS data
 * @retval     AGESA_STATUS
 */
AGESA_STATUS
GnbGetScsDataML (
  IN OUT   UINTN        *EntryAddress,
  IN OUT   UINTN        *EntrySize
  )
{
  AGESA_STATUS      Status;
  AGESA_STATUS      AgesaStatus;
  PSP_DIRECTORY     *PspDir;
  UINTN             i;

  AgesaStatus = AGESA_SUCCESS;
  Status = AGESA_SUCCESS;
  PspDir = NULL;
  Status = GetPspDirBaseML ((UINTN *)&PspDir);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  for (i = 0; i < PspDir->Header.TotalEntries; i++) {
    if (PspDir->PspEntry[i].Type == SCS_BIN_SIGNATURE) {
      *EntryAddress = (UINTN) PspDir->PspEntry[i].Location;
      *EntrySize = PspDir->PspEntry[i].Size;
      Status = AGESA_SUCCESS;
      AGESA_STATUS_UPDATE (Status, AgesaStatus);
      break;
    }
  }

  return  AgesaStatus;
}



/*----------------------------------------------------------------------------------------*/
/**
 * GNB PSP MBox Read
 *
 *
 * @param[in]  PspMBox    Pointer to UINT32 value from PSP Mailbox
 * @param[in]  GnbHandle  GNB_HANDLE
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */
AGESA_STATUS
GnbPspMBoxRead (
  IN OUT   UINT32                          *PspMBox,
  IN       GNB_HANDLE                      *GnbHandle,
  IN       AMD_CONFIG_PARAMS               *StdHeader
  )
{
  UINT32 RegValue;
  UINT32 PspMmioSize;
  D18F1xBC_STRUCT D18F1xBC;
  D18F1xB8_STRUCT D18F1xB8;
  D18F1xBC_STRUCT D18F1xBCSave;
  D18F1xB8_STRUCT D18F1xB8Save;
  UINT32          PspCmdSave;
  UINT32          PspBar1Save;
  UINT32          Pspx48Save;

  GnbLibPciRead (GET_PSP_PCI_ADDR (PSP_PCIE_DEVID_REG), AccessWidth32, &RegValue, StdHeader);
  if (RegValue == 0xFFFFFFFF) {
    return (AGESA_ERROR);
  }

  GnbRegisterReadML (GnbHandle, D18F1xB8_TYPE, D18F1xB8_ADDRESS, &D18F1xB8Save.Value, 0, StdHeader);
  GnbRegisterReadML (GnbHandle, D18F1xBC_TYPE, D18F1xBC_ADDRESS, &D18F1xBCSave.Value, 0, StdHeader);
  GnbLibPciRead (GET_PSP_PCI_ADDR (PSP_PCIE_CMD_REG), AccessWidth32, &PspCmdSave, StdHeader);
  GnbLibPciRead (GET_PSP_PCI_ADDR (PSP_PCIE_BAR1_REG), AccessWidth32, &PspBar1Save, StdHeader);
  GnbLibPciRead (GET_PSP_PCI_ADDR (0x48), AccessWidth32, &Pspx48Save, StdHeader);

  /// Get PSP BAR1 Size
  RegValue = 0xFFFFFFFF;
  GnbLibPciWrite (GET_PSP_PCI_ADDR (PSP_PCIE_BAR1_REG), AccessWidth32, &RegValue, StdHeader);
  GnbLibPciRead (GET_PSP_PCI_ADDR (PSP_PCIE_BAR1_REG), AccessWidth32, &PspMmioSize, StdHeader);
  PspMmioSize = ~PspMmioSize + 1;
  /// Assign BAR1 Temporary Address
  RegValue = PSP_BAR1_TMP_BASE;
  GnbLibPciWrite (GET_PSP_PCI_ADDR (PSP_PCIE_BAR1_REG), AccessWidth32, &RegValue, StdHeader);
  RegValue = 0x4;
  GnbLibPciWrite (GET_PSP_PCI_ADDR (0x48), AccessWidth32, &RegValue, StdHeader);
  RegValue = 0x6;
  GnbLibPciWrite (GET_PSP_PCI_ADDR (PSP_PCIE_CMD_REG), AccessWidth32, &RegValue, StdHeader);

  /// Enable GNB redirection to this space
  D18F1xB8.Value = 0;
  D18F1xB8.Field.MMIOBase_39_16_ = PSP_BAR1_TMP_BASE >> 16;
  D18F1xB8.Field.RE = 1;
  D18F1xB8.Field.WE = 1;

  D18F1xBC.Value = 0;
  D18F1xBC.Field.MMIOLimit_39_16_ = (PSP_BAR1_TMP_BASE + PspMmioSize -1) >> 16;

  GnbRegisterWriteML (GnbHandle, D18F1xBC_TYPE, D18F1xBC_ADDRESS, &D18F1xBC.Value, 0, StdHeader);
  GnbRegisterWriteML (GnbHandle, D18F1xB8_TYPE, D18F1xB8_ADDRESS, &D18F1xB8.Value, 0, StdHeader);

  /// Enable MsixBarEn, Bar2En, Bar1En, Bar0En
  /// Enable MsixBarEn, Bar1En, Bar3En
  RegValue = 0x34;
  GnbLibPciWrite (GET_PSP_PCI_ADDR (0x48), AccessWidth32, &RegValue, StdHeader);

  /// Read PSP Mailbox data
  GnbLibMemRead (PSP_BAR1_TMP_BASE + PSP_MAILBOX_STATUS, AccessWidth32, PspMBox, StdHeader);

  /// Restore registers
  GnbLibPciWrite (GET_PSP_PCI_ADDR (0x48), AccessWidth32, &Pspx48Save, StdHeader);
  GnbRegisterWriteML (GnbHandle, D18F1xBC_TYPE, D18F1xBC_ADDRESS, &D18F1xBCSave.Value, 0, StdHeader);
  GnbRegisterWriteML (GnbHandle, D18F1xB8_TYPE, D18F1xB8_ADDRESS, &D18F1xB8Save.Value, 0, StdHeader);
  GnbLibPciWrite (GET_PSP_PCI_ADDR (PSP_PCIE_BAR1_REG), AccessWidth32, &PspBar1Save, StdHeader);
  GnbLibPciWrite (GET_PSP_PCI_ADDR (PSP_PCIE_CMD_REG), AccessWidth32, &PspCmdSave, StdHeader);

  return (AGESA_SUCCESS);
}

