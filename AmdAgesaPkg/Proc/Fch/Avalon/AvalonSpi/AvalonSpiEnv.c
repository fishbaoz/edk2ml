/* $NoKeywords:$ */
/**
 * @file
 *
 * Config Fch Spi (Lpc) controller
 *
 * Init Spi (Lpc) Controller features.
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
****************************************************************************
*/
#include "FchPlatform.h"
#define FILECODE PROC_FCH_AVALON_AVALONSPI_AVALONSPIENV_FILECODE
#define SPI_BASE UserOptions.FchBldCfg->CfgSpiRomBaseAddress


SPI_CONTROLLER_PROFILE SpiControllerProfile[4] = {
  {128, 100, 100, 100, 100},
  {128,  66,  66,  66,  66},
  {128,  33,  33,  33,  33},
  {128,  16,  16,  16,  16},
  };
SPI_DEVICE_PROFILE DefaultSpiDeviceTable[] = {
  //JEDEC_ID,RomSize,SecSize;MaxNormal;MaxFast;MaxDual;MaxQuad;QeReadReg;QeWriteReg;QeRegSize;QeLocation;
  {0x001524C2, 2 << 20, 4096,  33, 108, 150, 300, 0x05, 0x01, 0x1, 0x0040}, //Macronix_MX25L1635D
  {0x001525C2, 2 << 20, 4096,  33, 108, 160, 432, 0x05, 0x01, 0x1, 0x0040}, //Macronix_MX25L1635E
  {0x00165EC2, 4 << 20, 4096,  33, 104, 208, 400, 0x05, 0x01, 0x1, 0x0040}, //Macronix_MX25L3235D
//  {0x003625C2, 4 << 20, 4096,  33, 104, 168, 432, 0x05, 0x01, 0x1, 0x0040}, //Macronix_MX25U3235F
  {0x001720C2, 8 << 20, 4096,  50, 104, 140, 280, 0x05, 0x01, 0x1, 0x0040}, //Macronix_MX25L6436E
  {0x003725C2, 8 << 20, 4096,  33, 104, 168, 432, 0x05, 0x01, 0x1, 0x0040}, //Macronix_MX25U6435F

  {0x0046159D, 4 << 20, 4096,  33, 104, 208, 400, 0x05, 0x01, 0x1, 0x0040}, //PFLASH Pm25LQ032C

  {0x001540EF, 2 << 20, 4096,  33, 104, 208, 416, 0x35, 0x01, 0x2, 0x0200}, //Wnbond_W25Q16CV
  {0x001640EF, 4 << 20, 4096,  33, 104, 208, 320, 0x35, 0x01, 0x2, 0x0200}, //Wnbond_W25Q32BV
  {0x001740EF, 8 << 20, 4096, 104, 104, 208, 416, 0x35, 0x01, 0x2, 0x0200}, //Wnbond_W25Q64

  {0x004326BF, 8 << 20, 4096,  40, 104, 160, 416, 0x35, 0x01, 0x2, 0x0200}, //SST26VF064BA

  {0x001640C8, 4 << 20, 4096,  33, 100, 160, 320, 0x35, 0x01, 0x2, 0x0200}, //GigaDecice GD25Q32BSIGR

  {0x00164037, 4 << 20, 4096,  33, 100, 200, 400, 0x35, 0x01, 0x2, 0x0200}, //AMIC A25LQ32B

  {0x0016BA20, 4 << 20, 4096, 108, 108, 216, 432, 0x00, 0x00, 0x1, 0x0000}, //N25Q032
  {0x0017BA20, 8 << 20, 4096, 108, 108, 216, 432, 0x00, 0x00, 0x1, 0x0000}, //N25Q064

  {0x00000000, 4 << 20, 4096,  33,  33,  33,  33, 0x05, 0x01, 0x1, 0x0040}
};

UINT8 RomSigBuffer[0x100] = {0};

/**
 * FchWriteSpiExtReg - Write SPI Extension Register
 *
 *
 *
 * @param[in] SpiExtRegIndex - Extension Register Index.
 * @param[in] SpiExtRegData - Extension Register Data.
 *
 */
VOID
FchWriteSpiExtReg (
  IN       UINT8      SpiExtRegIndex,
  IN       UINT8      SpiExtRegData
  )
{
  ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG1E) = SpiExtRegIndex;
  ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG1F) = SpiExtRegData;
}
/**
 * FchSetSpiCounter - Set SPI RX/TX Counters
 *
 *
 *
 * @param[in] TxCounter - Transfer Counter.
 * @param[in] RxCounter - Receive Counter.
 *
 */
VOID
FchSetSpiCounter (
  IN       UINT8      TxCounter,
  IN       UINT8      RxCounter
  )
{
  FchWriteSpiExtReg (FCH_SPI_MMIO_REG1F_X05_TX_BYTE_COUNT, TxCounter);
  FchWriteSpiExtReg (FCH_SPI_MMIO_REG1F_X06_RX_BYTE_COUNT, RxCounter);
}
/**
 * FchSpiControllerNotBusy - SPI Conroller Not Busy
 *
 *
 *
 *
 */
VOID
FchSpiControllerNotBusy (
  )
{
  UINT32 SpiReg00;
  SpiReg00 = FCH_SPI_BUSY + FCH_SPI_EXEC_OPCODE;
  do {
    SpiReg00 = ACPIMMIO32 (SPI_BASE + FCH_SPI_MMIO_REG00);
  } while ((SpiReg00 & (FCH_SPI_BUSY + FCH_SPI_EXEC_OPCODE)));
}
/**
 * FchSpiExecute - SPI Execute
 *
 *
 *
 *
 */
VOID
FchSpiExecute (
  )
{
  UINT32 SpiReg00;
  SpiReg00 = FCH_SPI_BUSY + FCH_SPI_EXEC_OPCODE;
  ACPIMMIO32 (SPI_BASE + FCH_SPI_MMIO_REG00) |= FCH_SPI_EXEC_OPCODE;
  do {
    SpiReg00 = ACPIMMIO32 (SPI_BASE + FCH_SPI_MMIO_REG00);
  } while ((SpiReg00 & (FCH_SPI_BUSY + FCH_SPI_EXEC_OPCODE)));
}
/**
 * FchResetFifo - Reset SPI FIFO
 *
 *
 *
 *
 */
VOID
FchResetFifo (
  )
{
  ACPIMMIO32 (SPI_BASE + FCH_SPI_MMIO_REG00) |= BIT20;
}
/**
 * WaitForSpiDeviceWriteEnabled -
 *
 *
 *
 *
 */
BOOLEAN
WaitForSpiDeviceWriteEnabled (
  )
{
  UINT8 bStatus;
  bStatus = 0;
  do
  {
    FchSpiTransfer (
      0, //IN       UINT8    PrefixCode,
      0x05,//IN       UINT8    Opcode,
      &bStatus,//IN       OUT UINT8    *DataPtr,
      NULL,//IN       UINT8    *AddressPtr,
      0,//IN       UINT8    Length,
      FALSE,//IN       BOOLEAN  WriteFlag,
      FALSE,//IN       BOOLEAN  AddressFlag,
      TRUE,//IN       BOOLEAN  DataFlag,
      FALSE //IN       BOOLEAN  FinishedFlag
    );
  } while ((bStatus & 2) == 0);
  return TRUE;
}
/**
 * WaitForSpiDeviceComplete -
 *
 *
 *
 *
 */
BOOLEAN
WaitForSpiDeviceComplete (
  )
{
  UINT8 bStatus;
  bStatus = 1;
  do
  {
    FchSpiTransfer (
      0, //IN       UINT8    PrefixCode,
      0x05,//IN       UINT8    Opcode,
      &bStatus,//IN       OUT UINT8    *DataPtr,
      NULL,//IN       UINT8    *AddressPtr,
      0,//IN       UINT8    Length,
      FALSE,//IN       BOOLEAN  WriteFlag,
      FALSE,//IN       BOOLEAN  AddressFlag,
      TRUE,//IN       BOOLEAN  DataFlag,
      FALSE //IN       BOOLEAN  FinishedFlag
    );
  } while (bStatus & 1);
  return TRUE;
}
/**
 * FchSpiTransfer - FCH Spi Transfer
 *
 *
 *
 * @param[in] PrefixCode   - Prefix code.
 * @param[in] Opcode       - Opcode.
 * @param[in] DataPtr      - Data Pointer.
 * @param[in] AddressPtr   - Address Pointer.
 * @param[in] Length       - Read/Write Length.
 * @param[in] WriteFlag    - Write Flag.
 * @param[in] AddressFlag  - Address Flag.
 * @param[in] DataFlag     - Data Flag.
 * @param[in] FinishedFlag - Finished Flag.
 *
 */
//static
AGESA_STATUS
FchSpiTransfer (
  IN       UINT8    PrefixCode,
  IN       UINT8    Opcode,
  IN OUT   UINT8    *DataPtr,
  IN       UINT8    *AddressPtr,
  IN       UINT8    Length,
  IN       BOOLEAN  WriteFlag,
  IN       BOOLEAN  AddressFlag,
  IN       BOOLEAN  DataFlag,
  IN       BOOLEAN  FinishedFlag
  )
{
  UINTN  Addr;
  UINTN  Retry;
  UINTN  i;
  UINTN  index;
  UINT8  WriteCount;
  UINT8  ReadCount;
  //UINT8  Dummy;
  //UINT8  CurrFifoIndex;

  if (!((Opcode == 0x9f) && (!DataFlag))) {
    if (PrefixCode) {
      Retry = 0;
      do {
        ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG00 + 0) = PrefixCode;
        //ACPIMMIO8(SPI_BASE + FCH_SPI_MMIO_REG00 + 1) = 0;
        FchSetSpiCounter (0, 0);
        ACPIMMIO32 (SPI_BASE + FCH_SPI_MMIO_REG00) |= FCH_SPI_EXEC_OPCODE;
        FchSpiControllerNotBusy ();

        if (FinishedFlag) {
          if (WaitForSpiDeviceWriteEnabled ()) {
            Retry = 0;
          } else {
            Retry ++;
            if (Retry >= FCH_SPI_RETRY_TIMES) {
              return AGESA_ERROR;
            }
          }
        }
      } while (Retry);
    }
    Retry = 0;
    do {
      WriteCount = 0;
      ReadCount = 0;
      //
      // Reset Fifo Ptr
      //
      FchResetFifo ();
      //
      // Check Address Mode
      //
      index = 0;
      Addr = (UINTN) AddressPtr;
      if (AddressFlag) {
        //for (i = 16, Addr = (UINTN) AddressPtr; i >= 0; i -= 8) {
        for (i = 0; i < 3; i ++) {
          //ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG0C + 0) = (UINT8) ((Addr >> i) & 0xff);
          ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG80_FIFO + index) = (UINT8) ((Addr >> (16 - i * 8)) & 0xff);
          index ++;
        }
        WriteCount += 3;
      }
      if (DataFlag) {
        //
        // Check Read/Write Mode
        //
        if (WriteFlag) {
          WriteCount += Length + 1;
          for (i = 0; i < (UINTN) (Length + 1); i ++) {
            //ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG0C + 0) = DataPtr[i];
            ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG80_FIFO + index) = DataPtr[i];
            index ++;
          }
        } else {
          //
          // Read operation must plus extra 1 byte
          //
          ReadCount += Length + 2;
        }
      }
      ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG00 + 0) = Opcode;
      //ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG00 + 1) = (ReadCount << 4) + WriteCount;
      FchSetSpiCounter (WriteCount, ReadCount);
      ACPIMMIO32 (SPI_BASE + FCH_SPI_MMIO_REG00) |= FCH_SPI_EXEC_OPCODE;
      FchSpiControllerNotBusy ();

      if (FinishedFlag) {
        if (WaitForSpiDeviceComplete ()) {
          Retry = 0;
        } else {
          Retry ++;
          if (Retry >= FCH_SPI_RETRY_TIMES) {
            return AGESA_ERROR;
          }
        }
      }
    } while (Retry);
    if (DataFlag && ReadCount) {
      //
      // Reset Fifo Ptr
      //
      FchResetFifo ();
      //while (DataFlag && ReadCount) {
      //  CurrFifoIndex = ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG4E + 1) & 0x07;
      //  if (CurrFifoIndex != WriteCount) {
      //    Dummy = ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG0C + 0);
      //  } else break;
      //}
      for (i = 0; i < (UINTN) (Length + 1); i ++) {
        //DataPtr[i] = ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG0C + 0);
        DataPtr[i] = ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG80_FIFO + WriteCount + i);
      }
    }
  }
  return AGESA_SUCCESS;
}

/**
 * FchSetQualMode - Set SPI Qual Mode
 *
 *
 *
 * @param[in] SpiQualMode- Spi Qual Mode.
 * @param[in] StdHeader  - Standard Header.
 *
 */
VOID
FchSetQualMode (
  IN       UINT32      SpiQualMode,
  IN       AMD_CONFIG_PARAMS         *StdHeader
  )
{
  UINT32 SpiMode;
//  UINT8  Src;
//  UINT8  *SrcPoint;
//  UINTN  FlashWriteAddress;

//  SrcPoint = (UINT8*)(UINTN)0xff820040ul;
//  Src = *SrcPoint;
//  if ( Src != 0xAA ) {
//      Src = 0xAA;
//      FlashWriteAddress = 0x20040;
//      FchSpiTransfer (
//        0x06, //IN  UINT8    PrefixCode,
//        0x02,//IN  UINT8    Opcode,
//        &Src,//IN  OUT UINT8    *DataPtr,
//        (UINT8 *)FlashWriteAddress,//IN  UINT8    *AddressPtr,
//        0,//IN  UINT8    Length,
//        1,//IN  BOOLEAN  WriteFlag,
//        1,//IN  BOOLEAN  AddressFlag,
//        1,//IN  BOOLEAN  DataFlag,
//        1 //IN  BOOLEAN  FinishedFlag
//      );
//  }


  RwMem (ACPI_MMIO_BASE + GPIO_BASE + FCH_GEVENT_REG09, AccessWidth8, ~(UINT32) BIT3, BIT3);
  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGBB, AccessWidth8, ~(UINT32) BIT0, BIT0, StdHeader);
  SpiMode = ACPIMMIO32 (SPI_BASE + FCH_SPI_MMIO_REG00);
  if ((SpiMode & (BIT18 + BIT29 + BIT30)) != ((SpiQualMode & 1) << 18) + ((SpiQualMode & 6) << 28)) {
    RwMem (SPI_BASE + FCH_SPI_MMIO_REG00, AccessWidth32, ~(UINT32) ( BIT18 + BIT29 + BIT30), ((SpiQualMode & 1) << 18) + ((SpiQualMode & 6) << 28));
  }

}



UINT32
FchReadSpiId (
  IN       BOOLEAN    Flag
  )
{
  UINT32 DeviceID;
  UINT8 *DeviceIdPtr;
  DeviceID = 0;
  DeviceIdPtr = (UINT8 *) (((UINTN) (&DeviceID)));
  if (Flag) {
    FchSpiTransfer (
      0, //IN       UINT8    PrefixCode,
      0x9F,//IN       UINT8    Opcode,
      DeviceIdPtr,//IN  OUT   UINT8    *DataPtr,
      (UINT8 *) (NULL),//IN       UINT8    *AddressPtr,
      2,//IN       UINT8    Length,
      FALSE,//IN       BOOLEAN  WriteFlag,
      FALSE,//IN       BOOLEAN  AddressFlag,
      TRUE,//IN       BOOLEAN  DataFlag,
      FALSE //IN       BOOLEAN  FinishedFlag
    );
  } else {
    FchSpiTransfer (
      0, //IN       UINT8    PrefixCode,
      0x9F,//IN       UINT8    Opcode,
      DeviceIdPtr,//IN  OUT   UINT8    *DataPtr,
      (UINT8 *) (NULL),//IN       UINT8    *AddressPtr,
      2,//IN       UINT8    Length,
      FALSE,//IN       BOOLEAN  WriteFlag,
      FALSE,//IN       BOOLEAN  AddressFlag,
      FALSE,//IN       BOOLEAN  DataFlag,
      FALSE //IN       BOOLEAN  FinishedFlag
    );
  }
  return DeviceID;
}
/**
 * FchReadSpiQe - Read SPI Qual Enable
 *
 *
 *
 * @param[in] SpiDeviceProfilePtr - Spi Device Profile Pointer
 * @param[in] SpiQeRegValue   - Spi QuadEnable Register Value
 *
 */
BOOLEAN
FchReadSpiQe (
  IN OUT   SPI_DEVICE_PROFILE    *SpiDeviceProfilePtr,
  IN       UINT16     SpiQeRegValue
  )
{
  UINT16     Value16;
  SpiQeRegValue = 0;
  Value16 = 0;

  FchSpiTransfer (
    0, //IN       UINT8    PrefixCode,
    SpiDeviceProfilePtr->QeReadRegister,//IN       UINT8    Opcode,
    (UINT8 *)(&SpiQeRegValue),//IN  OUT   UINT8    *DataPtr,
    NULL,//IN       UINT8    *AddressPtr,
    0,//IN       UINT8    Length,
    FALSE,//IN       BOOLEAN  WriteFlag,
    FALSE,//IN       BOOLEAN  AddressFlag,
    TRUE,//IN       BOOLEAN  DataFlag,
    FALSE //IN       BOOLEAN  FinishedFlag
  );
  if (SpiDeviceProfilePtr->QeOperateSize == 2) {
    FchSpiTransfer (
      0, //IN       UINT8    PrefixCode,
      0x05,//IN       UINT8    Opcode,
      (UINT8 *)(&SpiQeRegValue),//IN  OUT   UINT8    *DataPtr,
      NULL,//IN       UINT8    *AddressPtr,
      0,//IN       UINT8    Length,
      FALSE,//IN       BOOLEAN  WriteFlag,
      FALSE,//IN       BOOLEAN  AddressFlag,
      TRUE,//IN       BOOLEAN  DataFlag,
      FALSE //IN       BOOLEAN  FinishedFlag
    );
    FchSpiTransfer (
      0, //IN       UINT8    PrefixCode,
      SpiDeviceProfilePtr->QeReadRegister,//IN       UINT8    Opcode,
      (UINT8 *)(&Value16),//IN  OUT   UINT8    *DataPtr,
      NULL,//IN       UINT8    *AddressPtr,
      0,//IN       UINT8    Length,
      FALSE,//IN       BOOLEAN  WriteFlag,
      FALSE,//IN       BOOLEAN  AddressFlag,
      TRUE,//IN       BOOLEAN  DataFlag,
      FALSE //IN       BOOLEAN  FinishedFlag
    );
    SpiQeRegValue |= (Value16 << 8);
  }

  if (SpiDeviceProfilePtr->QeLocation & SpiQeRegValue) {
    return TRUE;
  }
  SpiQeRegValue |= SpiDeviceProfilePtr->QeLocation;
  return FALSE;
}
/**
 * FchWriteSpiQe - Write SPI Qual Enable
 *
 *
 *
 * @param[in] SpiDeviceProfilePtr - Spi Device Profile Pointer
 * @param[in] SpiQeRegValue   - Spi QuadEnable Register Value
 *
 */
VOID
FchWriteSpiQe (
  IN OUT   SPI_DEVICE_PROFILE    *SpiDeviceProfilePtr,
  IN       UINT16     SpiQeRegValue
  )
{

  SpiQeRegValue |= SpiDeviceProfilePtr->QeLocation;
  FchSpiTransfer (
    0x06, //IN       UINT8    PrefixCode,
    SpiDeviceProfilePtr->QeWriteRegister,//IN       UINT8    Opcode,
    (UINT8 *)(&SpiQeRegValue),//IN  OUT   UINT8    *DataPtr,
    NULL,//IN       UINT8    *AddressPtr,
    SpiDeviceProfilePtr->QeOperateSize - 1,//IN       UINT8    Length,
    TRUE,//IN       BOOLEAN  WriteFlag,
    FALSE,//IN       BOOLEAN  AddressFlag,
    TRUE,//IN       BOOLEAN  DataFlag,
    TRUE //IN       BOOLEAN  FinishedFlag
  );
}

/**
 * FchFindSpiDeviceProfile - Find SPI Device Profile
 *
 *
 *
 * @param[in] DeviceID   - Device ID
 * @param[in] SpiDeviceProfilePtr - Spi Device Profile Pointer
 *
 */
BOOLEAN
FchFindSpiDeviceProfile (
  IN       UINT32     DeviceID,
  IN OUT   SPI_DEVICE_PROFILE    *SpiDeviceProfilePtr
  )
{
  SPI_DEVICE_PROFILE   *CurrentSpiDeviceProfilePtr;
  UINT16     SpiQeRegValue;
  SpiQeRegValue = 0;
  CurrentSpiDeviceProfilePtr = SpiDeviceProfilePtr;
  do {
    if (CurrentSpiDeviceProfilePtr->JEDEC_ID == DeviceID) {
      SpiDeviceProfilePtr = CurrentSpiDeviceProfilePtr;
      if (!(FchReadSpiQe (SpiDeviceProfilePtr, SpiQeRegValue))) {
        FchWriteSpiQe (SpiDeviceProfilePtr, SpiQeRegValue);
        if (!(FchReadSpiQe (SpiDeviceProfilePtr, SpiQeRegValue))) {
          return FALSE;
        }
      }
      return TRUE;
    }
    CurrentSpiDeviceProfilePtr++;
  } while (CurrentSpiDeviceProfilePtr->JEDEC_ID != NULL);
  return FALSE;
}

/**
 * FchConfigureSpiDeviceDummyCycle - Configure Spi Device Dummy
 * Cycle
 *
 *
 *
 * @param[in] DeviceID   - Device ID
 * @param[in] FchDataPtr  - FchData Pointer.
 *
 */
BOOLEAN
FchConfigureSpiDeviceDummyCycle (
  IN       UINT32     DeviceID,
  IN OUT   FCH_DATA_BLOCK    *FchDataPtr
  )
{
  UINT16     Mode16;
  UINT16     Value16;
  UINT8      Value8;
  UINT16     DummyValue16;
  UINT16     CurrentDummyValue16;
  UINT16     CurrentMode16;
  AMD_CONFIG_PARAMS         *StdHeader;

  StdHeader = FchDataPtr->StdHeader;
  Value16 = 0;
  DummyValue16 = 8;

  switch (DeviceID) {
  case 0x17BA20://N25Q064
  case 0x16BA20://N25Q032

    FchSpiTransfer (
      0, //IN       UINT8    PrefixCode,
      0xB5,//IN       UINT8    Opcode,
      (UINT8 *)(&Value16),//IN  OUT   UINT8    *DataPtr,
      NULL,//IN       UINT8    *AddressPtr,
      1,//IN       UINT8    Length,
      FALSE,//IN       BOOLEAN  WriteFlag,
      FALSE,//IN       BOOLEAN  AddressFlag,
      TRUE,//IN       BOOLEAN  DataFlag,
      FALSE //IN       BOOLEAN  FinishedFlag
    );
    CurrentDummyValue16 = Value16 >> 12;
    CurrentMode16 = (Value16 >> 9) & 7;

    FchSpiTransfer (
      0, //IN       UINT8    PrefixCode,
      0x85,//IN       UINT8    Opcode,
      (UINT8 *)(&Value8),//IN  OUT   UINT8    *DataPtr,
      NULL,//IN       UINT8    *AddressPtr,
      0,//IN       UINT8    Length,
      FALSE,//IN       BOOLEAN  WriteFlag,
      FALSE,//IN       BOOLEAN  AddressFlag,
      TRUE,//IN       BOOLEAN  DataFlag,
      FALSE //IN       BOOLEAN  FinishedFlag
    );
    CurrentDummyValue16 = Value8 >> 4;

    switch (FchDataPtr->Spi.SpiMode) {
    case FCH_SPI_MODE_QUAL_144:
      DummyValue16 = 6;
      Mode16 = FCH_SPI_DEVICE_MODE_144;
      break;
    case FCH_SPI_MODE_QUAL_114:
      DummyValue16 = 8;
      Mode16 = FCH_SPI_DEVICE_MODE_114;
      Mode16 = 7;
      break;
    case FCH_SPI_MODE_QUAL_122:
      DummyValue16 = 4;
      Mode16 = FCH_SPI_DEVICE_MODE_122;
      break;
    case FCH_SPI_MODE_QUAL_112:
      DummyValue16 = 8;
      Mode16 = FCH_SPI_DEVICE_MODE_112;
      break;
    case FCH_SPI_MODE_FAST:
      DummyValue16 = 8;
      Mode16 = FCH_SPI_DEVICE_MODE_FAST;
      break;
    default:
      DummyValue16 = 15;
      Mode16 = 7;
      break;
    }
    if ((CurrentDummyValue16 != DummyValue16) || (CurrentMode16 != Mode16)) {
      //FCH_DEADLOOP();
      Value16 &= ~ (0x7f << 9);
      Value16 |= (DummyValue16 << 12);
      Value16 |= (Mode16 << 9);
      FchSpiTransfer (
        0x06, //IN       UINT8    PrefixCode,
        0xB1,//IN       UINT8    Opcode,
        (UINT8 *)(&Value16),//IN  OUT   UINT8    *DataPtr,
        NULL,//IN       UINT8    *AddressPtr,
        1,//IN       UINT8    Length,
        TRUE,//IN       BOOLEAN  WriteFlag,
        FALSE,//IN       BOOLEAN  AddressFlag,
        TRUE,//IN       BOOLEAN  DataFlag,
        TRUE //IN       BOOLEAN  FinishedFlag
      );

      FchSpiTransfer (
        0, //IN       UINT8    PrefixCode,
        0x85,//IN       UINT8    Opcode,
        (UINT8 *)(&Value8),//IN  OUT   UINT8    *DataPtr,
        NULL,//IN       UINT8    *AddressPtr,
        0,//IN       UINT8    Length,
        FALSE,//IN       BOOLEAN  WriteFlag,
        FALSE,//IN       BOOLEAN  AddressFlag,
        TRUE,//IN       BOOLEAN  DataFlag,
        FALSE //IN       BOOLEAN  FinishedFlag
      );

      Value8 &= ~ (0xf << 4);
      Value8 |= (UINT8) (DummyValue16 << 4);
      FchSpiTransfer (
        0x06, //IN       UINT8    PrefixCode,
        0x81,//IN       UINT8    Opcode,
        (UINT8 *)(&Value8),//IN  OUT   UINT8    *DataPtr,
        NULL,//IN       UINT8    *AddressPtr,
        0,//IN       UINT8    Length,
        TRUE,//IN       BOOLEAN  WriteFlag,
        FALSE,//IN       BOOLEAN  AddressFlag,
        TRUE,//IN       BOOLEAN  DataFlag,
        TRUE //IN       BOOLEAN  FinishedFlag
      );
//      FchStall (100000, StdHeader);
//      WriteIo8 ((UINT16) (0xCF9), 0x0E);
    }
    return TRUE;
  default:
    return FALSE;
  }
}
/**
 * FchPlatformSpiQe - Platform SPI Qual Enable
 *
 *
 *
 * @param[in] FchDataPtr  - FchData Pointer.
 *
 */
BOOLEAN
FchPlatformSpiQe (
  IN       VOID     *FchDataPtr
  )
{
  UINT32 DeviceID;
  SPI_DEVICE_PROFILE        *LocalSpiDeviceProfilePtr;
  FCH_DATA_BLOCK      *LocalCfgPtr;
  AMD_CONFIG_PARAMS         *StdHeader;
  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
//  if (LocalCfgPtr->QeEnabled) {
//    return TRUE;
//  }
  StdHeader = LocalCfgPtr->StdHeader;
//  FchReadSpiId (FALSE);
  DeviceID = FchReadSpiId (TRUE);
//  if (LocalCfgPtr->OemSpiDeviceTable != NULL) {
//    LocalSpiDeviceProfilePtr = LocalCfgPtr->OemSpiDeviceTable;
//    if (FchFindSpiDeviceProfile (DeviceID, LocalSpiDeviceProfilePtr)) {
//      return TRUE;
//    }
//  }
  LocalSpiDeviceProfilePtr = (SPI_DEVICE_PROFILE *) (&DefaultSpiDeviceTable);
//  FchConfigureSpiDeviceDummyCycle (0, LocalCfgPtr); //dummy once
  if (FchConfigureSpiDeviceDummyCycle (DeviceID, LocalCfgPtr)) {
    return TRUE;
  }
  if (FchFindSpiDeviceProfile (DeviceID, LocalSpiDeviceProfilePtr)) {
    return TRUE;
  }
  return FALSE;
}

/**
 * AutoSpiModeSpeed - Auto SPI Mode Speed
 *
 *
 *
 * @param[in] FchDataPtr  - FchData Pointer.
 *
 */
BOOLEAN
AutoSpiModeSpeed (
  IN       VOID     *FchDataPtr
  )
{
  UINT32 DeviceID;
  FCH_DATA_BLOCK      *LocalCfgPtr;
  AMD_CONFIG_PARAMS         *StdHeader;
  SPI_DEVICE_PROFILE   *CurrentSpiDeviceProfilePtr;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  DeviceID = FchReadSpiId (TRUE);
  CurrentSpiDeviceProfilePtr = (SPI_DEVICE_PROFILE *) (&DefaultSpiDeviceTable);

  do {
    if (CurrentSpiDeviceProfilePtr->JEDEC_ID == DeviceID) {
      LocalCfgPtr->Spi.SpiMode = FCH_SPI_MODE_QUAL_144;
      if (CurrentSpiDeviceProfilePtr->MaxQuadSpeed >= 100 * 4) {
        LocalCfgPtr->Spi.SpiFastSpeed = FCH_SPI_SPEED_100M;
      } else if (CurrentSpiDeviceProfilePtr->MaxQuadSpeed >= 66 * 4) {
        LocalCfgPtr->Spi.SpiFastSpeed = FCH_SPI_SPEED_66M;
      } else if (CurrentSpiDeviceProfilePtr->MaxQuadSpeed >= 33 * 4) {
        LocalCfgPtr->Spi.SpiFastSpeed = FCH_SPI_SPEED_33M;
      }
      return TRUE;
    }
    CurrentSpiDeviceProfilePtr++;
  } while (CurrentSpiDeviceProfilePtr->JEDEC_ID != NULL);
  return FALSE;
}


/**
 * FchSetSpi - Config Spi controller before PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchSetSpi (
  IN  VOID     *FchDataPtr
  )
{
  UINT8  SpiMode;
  UINT8  SpiFastSpeed;
  UINT8  *SrcPoint;
  UINTN  FlashWriteAddress;
  UINTN  CopyLength;
  UINTN  RomSigStartingAddr;
  UINT16  Value16;

  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  //FCH_DEADLOOP();
  GetRomSigPtr (&RomSigStartingAddr, StdHeader);
  Value16 = *((UINT16*) (UINTN) (RomSigStartingAddr + 0x40));
  SrcPoint = (UINT8*) (UINTN) (RomSigStartingAddr + 0x40);
  SpiMode = *SrcPoint;
  SrcPoint ++;
  SpiFastSpeed = *SrcPoint;

  if (( 0 ==  LocalCfgPtr->Spi.SpiMode) && ( 0 ==  (LocalCfgPtr->Spi.SpiFastSpeed))) {
    AutoSpiModeSpeed (FchDataPtr);
  }

  if (( SpiMode !=  LocalCfgPtr->Spi.SpiMode) || ( SpiFastSpeed !=  (LocalCfgPtr->Spi.SpiFastSpeed -1))) {
    if (FchPlatformSpiQe (FchDataPtr)) {
      SpiMode =  LocalCfgPtr->Spi.SpiMode;
      SpiFastSpeed =  LocalCfgPtr->Spi.SpiFastSpeed -1;

      SrcPoint = (UINT8*) (UINTN) RomSigStartingAddr;
      CopyLength = 0x100;
      if (*(SrcPoint) != RomSigBuffer[0]) {
        LibAmdMemCopy (RomSigBuffer,
                  SrcPoint,
                  CopyLength,
                  StdHeader);
      }

      RomSigBuffer [0x40] = SpiMode;
      RomSigBuffer [0x41] = SpiFastSpeed;

      if (Value16 != 0xffff) {
        FlashWriteAddress = 0x20000;
        FchSpiTransfer (
          0x06, //IN  UINT8    PrefixCode,
          0x20,//IN  UINT8    Opcode,
          NULL,//IN  OUT UINT8    *DataPtr,
          (UINT8 *)FlashWriteAddress,//IN  UINT8    *AddressPtr,
          0,//IN  UINT8    Length,
          1,//IN  BOOLEAN  WriteFlag,
          1,//IN  BOOLEAN  AddressFlag,
          0,//IN  BOOLEAN  DataFlag,
          1 //IN  BOOLEAN  FinishedFlag
        );

        FlashWriteAddress = 0x20000;
        FchSpiTransfer (
          0x06, //IN  UINT8    PrefixCode,
          0x02,//IN  UINT8    Opcode,
          RomSigBuffer,//IN  OUT UINT8    *DataPtr,
          (UINT8 *)FlashWriteAddress,//IN  UINT8    *AddressPtr,
          31,//IN  UINT8    Length,
          1,//IN  BOOLEAN  WriteFlag,
          1,//IN  BOOLEAN  AddressFlag,
          1,//IN  BOOLEAN  DataFlag,
          1 //IN  BOOLEAN  FinishedFlag
        );
      }

      FlashWriteAddress = 0x20040;
      FchSpiTransfer (
        0x06, //IN  UINT8    PrefixCode,
        0x02,//IN  UINT8    Opcode,
        &SpiMode,//IN  OUT UINT8    *DataPtr,
        (UINT8 *)FlashWriteAddress,//IN  UINT8    *AddressPtr,
        0,//IN  UINT8    Length,
        1,//IN  BOOLEAN  WriteFlag,
        1,//IN  BOOLEAN  AddressFlag,
        1,//IN  BOOLEAN  DataFlag,
        1 //IN  BOOLEAN  FinishedFlag
      );

      FlashWriteAddress ++;
      SpiFastSpeed = LocalCfgPtr->Spi.SpiFastSpeed -1;
      FchSpiTransfer (
        0x06, //IN  UINT8    PrefixCode,
        0x02,//IN  UINT8    Opcode,
        &SpiFastSpeed,//IN  OUT UINT8    *DataPtr,
        (UINT8 *)FlashWriteAddress,//IN  UINT8    *AddressPtr,
        0,//IN  UINT8    Length,
        1,//IN  BOOLEAN  WriteFlag,
        1,//IN  BOOLEAN  AddressFlag,
        1,//IN  BOOLEAN  DataFlag,
        1 //IN  BOOLEAN  FinishedFlag
      );

      FchSetQualMode (LocalCfgPtr->Spi.SpiMode, StdHeader);
    }
  }
}

/**
 * FchInitEnvSpi - Config Spi controller before PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvSpi (
  IN  VOID     *FchDataPtr
  )
{
  FchInitEnvLpc (FchDataPtr);
  FchSetSpi (FchDataPtr);
}
