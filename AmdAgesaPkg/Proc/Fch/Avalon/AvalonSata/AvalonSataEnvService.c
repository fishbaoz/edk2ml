/* $NoKeywords:$ */
/**
 * @file
 *
 * SATA Controller family specific service procedure
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
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
*/


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  "FchPlatform.h"
#include  "Filecode.h"
#define FILECODE PROC_FCH_AVALON_AVALONSATA_AVALONSATAENVSERVICE_FILECODE


SATA_PHY_SETTING SataPhyTable[] =
{
  0x0030, 0x0040F407,
  0x0120, 0x00403204,
  0x0110, 0x00403103,

  0x0031, 0x0040F407,
  0x0121, 0x00403204,
  0x0111, 0x00403103,

};

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
//
// Local Routine
//
VOID FchSataCombineControlDataByte (IN UINT8  *ControlReg);
VOID FchSataCombineControlDataWord (IN UINT16  *ControlReg);


/**
 * FchInitEnvProgramSataPciRegs - Sata Pci Configuration Space
 * register setting
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvProgramSataPciRegs (
  IN  VOID     *FchDataPtr
  )
{
  UINT8                  *PortRegByte;
  UINT16                 *PortRegWord;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  //
  //Caculate SataPortReg for SATA_ESP_PORT
  //
  PortRegByte = &(LocalCfgPtr->Sata.SataEspPort.SataPortReg);
  FchSataCombineControlDataByte (PortRegByte);
  PortRegByte = &(LocalCfgPtr->Sata.SataPortPower.SataPortReg);
  FchSataCombineControlDataByte (PortRegByte);
  PortRegWord = &(LocalCfgPtr->Sata.SataPortMd.SataPortMode);
  FchSataCombineControlDataWord (PortRegWord);
  PortRegByte = &(LocalCfgPtr->Sata.SataHotRemovalEnhPort.SataPortReg);
  FchSataCombineControlDataByte (PortRegByte);

  //
  // Set Sata PCI Configuration Space Write enable
  //
  SataEnableWriteAccess (StdHeader);

  // *
  // Enables the SATA watchdog timer register prior to the SATA BIOS post
  //
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG44), AccessWidth8, 0xff, BIT0, StdHeader);

  // *
  // SATA PCI Watchdog timer setting
  //  Set timer out to 0x20 to fix IDE to SATA Bridge dropping drive issue.
  //
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG44 + 2), AccessWidth8, 0, 0x20, StdHeader);

  //
  // BIT4: Enable fast boot (SpeedupXPBoot)
  //
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40), AccessWidth8, 0xef, 0, StdHeader);

  //
  // HBA Initialization setting
  //
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG48 + 3), AccessWidth8, 0xff, BIT7, StdHeader);

  //
  // Unused SATA Ports Disabled
  //
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40 + 2), AccessWidth8, 0, LocalCfgPtr->Sata.SataPortPower.SataPortReg, StdHeader);

  //
  // Disable Prefetch In Ahci Mode
  //
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40 + 1), AccessWidth8, 0xDF, BIT5, StdHeader);

  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG84), AccessWidth32, (UINT32) (~ (0x01 << 31)), (UINT32) (0x00 << 31), StdHeader);
}

/**
 * FchSataCombineControlDataByte - Combine port control options
 * to one control byte.
 *
 *
 * @param[in] *ControlReg - Data pointer for control byte.
 *
 */
VOID
FchSataCombineControlDataByte (
  IN  UINT8    *ControlReg
  )
{
  UINT8   Index;
  UINT8   PortControl;

  *ControlReg = 0;
  for ( Index = 0; Index < 8; Index++ ) {
    PortControl = *( ControlReg + 1 + Index );
    *ControlReg |= PortControl << Index;
  }
}
/**
 * FchSataCombineControlDataWord - Combine port control options
 * to one control Word.
 *
 *
 * @param[in] *ControlReg - Data pointer for control byte.
 *
 */
VOID
FchSataCombineControlDataWord (
  IN  UINT16    *ControlReg
  )
{
  UINT8   Index;
  UINT8   PortControl;

  *ControlReg = 0;
  for ( Index = 0; Index < 8; Index++ ) {
    PortControl = *( (UINT8 *)ControlReg + 2 + Index );
    *ControlReg |= PortControl << (Index * 2);
  }
}

/**
 * FchProgramSataPhy - Program Sata PHY registers
 *
 * @param[in]   StdHeader
 *
 */
VOID
FchProgramSataPhy (
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  SATA_PHY_SETTING         *PhyTablePtr;
  UINT16                   Index;
  UINT8                    eFuseSquelchValue;
  UINT32                   SquelchValue[2];
  UINT8                    PortNum;

  PhyTablePtr = &SataPhyTable[0];

  for (Index = 0; Index < (sizeof (SataPhyTable) / sizeof (SATA_PHY_SETTING)); Index++) {
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG80, AccessWidth16, 0xFC00, PhyTablePtr->PhyCoreControlWord, StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG98, AccessWidth32, 0x00, PhyTablePtr->PhyFineTuneDword, StdHeader);
    ++PhyTablePtr;
  }
  SquelchValue[0] = (0x07 << 9);
  SquelchValue[1] = (0x07 << 9);
  eFuseSquelchValue = 0;
  //
  // SATA RX register settings need to be updated to pass Gen3 RX JTOL
  //
  for (PortNum = 0; PortNum < 2; PortNum ++) {
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG80, AccessWidth16, 0x00, ( 0x30 + PortNum), StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG9C, AccessWidth32, (UINT32) (~(0xF << 24)), (UINT32) (0xF << 24), StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG9C, AccessWidth32, (UINT32) (~(0x3 << 22)), (UINT32) (0x2 << 22), StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG9C, AccessWidth32, (UINT32) (~(0x1F << 17)), 0, StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG9C, AccessWidth32, (UINT32) (~(0x1 << 16)), (UINT32) (0x1 << 16), StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG9C, AccessWidth32, (UINT32) (~(0x1 << 12)), 0, StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG9C, AccessWidth32, (UINT32) (~(0xF << 5)), (UINT32) (0x4 << 5), StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG9C, AccessWidth32, (UINT32) (~(0x1F)), (UINT32) ( 0x6), StdHeader);
  }
  RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG94, AccessWidth32, (UINT32) (~(0x1F << 2)), (UINT32) ( 0x1F << 2 ), StdHeader);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGC8, AccessWidth8, 0xDF, 0x20);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD8, AccessWidth8, 0, 1);
  ReadMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD8 + 1, AccessWidth8, &eFuseSquelchValue);
  if ( (eFuseSquelchValue & BIT4) == BIT4) {
    SquelchValue[0] = ( UINT32 ) ( eFuseSquelchValue & 0xE0);
    SquelchValue[0] = (SquelchValue[0] << 4);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD8, AccessWidth8, 0, 2);
    ReadMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD8 + 1, AccessWidth8, &eFuseSquelchValue);
    SquelchValue[1] = ( UINT32 ) ( eFuseSquelchValue & 0x07);
    SquelchValue[1] = (SquelchValue[1] << 9);
  } else {
    SquelchValue[0] = (0x05 << 9);
    SquelchValue[1] = (0x05 << 9);
  }
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD8, AccessWidth8, 0, 0);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGC8, AccessWidth8, 0xDF, 0);
  for (PortNum = 0; PortNum < 2; PortNum ++) {
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG80, AccessWidth16, 0x00, ( 0x30 + PortNum), StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG9C, AccessWidth32, (UINT32) (~(0x7 << 9)), SquelchValue[PortNum], StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG9C, AccessWidth32, (UINT32) (~(0x7 << 13)), (UINT32) (0x0 << 13), StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG9C, AccessWidth32, (UINT32) (~(0x1f << 17)), (UINT32) (0x7 << 17), StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG80, AccessWidth16, 0x00, ( 0x20 + PortNum), StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG94, AccessWidth32, (UINT32) (~(0x1F << 2)), (UINT32) ( 0x04 << 2 ), StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG9C, AccessWidth32, (UINT32) (~(0x7 << 9)), SquelchValue[PortNum], StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG9C, AccessWidth32, (UINT32) (~(0x1f << 17)), (UINT32) (0x7 << 17), StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG80, AccessWidth16, 0x00, ( 0x10 + PortNum), StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG94, AccessWidth32, (UINT32) (~(0x1F << 2)), (UINT32) ( 0x03 << 2 ), StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG9C, AccessWidth32, (UINT32) (~(0x7 << 9)), SquelchValue[PortNum], StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG9C, AccessWidth32, (UINT32) (~(0x1f << 17)), (UINT32) (0x7 << 17), StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG80, AccessWidth16, 0x00, 0x010, StdHeader);
  }
}


/**
 * FchInitEnvSataRaidProgram - Configuration SATA Raid
 * controller (No longer exist)
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvSataRaidProgram (
  IN  VOID     *FchDataPtr
  )
{
}
