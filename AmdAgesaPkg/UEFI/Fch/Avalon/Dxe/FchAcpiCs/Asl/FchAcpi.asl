/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH Asl
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH ACPI
 * @e \$Revision: 84150 $   @e \$Date: 2012-12-12 13:46:25 -0800 (Wed, 12 Dec 2012) $
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
DefinitionBlock (
	 "FchAcpi.aml",   // Output Filename
	 "SSDT",      // Signature
	  0x01,       // DSDT Compliance Revision
	 "AMDFCH",    // OEMID
	 "FCHACPI",   // TABLE ID
	 0x1000       // OEM Revision
  )
{
  Scope(\_SB)
  {
    OperationRegion (FCHD, SystemMemory, 0x5CF7BA00, 0x000000B0)
    Field (FCHD, AnyAcc, NoLock, Preserve)
    {
     Offset(0x04),
     GPOE,  1,
     SMBE,  1,
     Offset(0x6C),
     GP13,  32,  // GPIO 13
    }

    Device(GPIO) {
      Name (_HID, "GPIO0010")
      Name (_CID, "GPIO0010")
      Name(_UID, 4)

      Method (_CRS, 0x0, NotSerialized) {
        Name (RBUF, ResourceTemplate () {

          Interrupt(ResourceConsumer, Level, ActiveLow, Shared, , , ) {9}

          Memory32Fixed(ReadWrite, 0xfed81500, 0x300)
          Memory32Fixed(ReadWrite, 0xfed80200, 0x100)
        })

        Return (RBUF)
      }

      Method (_STA, 0x0, NotSerialized) {
        If (LEqual(\_SB.GPOE, 0x01)) {
          Return (0x0f)
        } Else {
          Return (0x00)
        }
      }
    }

    Device(SMB1)
    {
      Name(_HID, EISAID("SMB0002"))
      Name(_CRS, ResourceTemplate()
      {
        IO(Decode16, 0x0B20, 0x0B20, 0x20, 0x20)
        IRQ(Level, ActiveLow, Shared) {7}
      })
      Method (_STA, 0x0, NotSerialized) {
        If (LEqual(\_SB.SMBE, 0x01)) {
          Return (0x0f)
        } Else {
          Return (0x00)
        }
      }
    }
  }
}
