/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP fTPM ACPI Object code (asl)
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
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

DefinitionBlock (
  "Tpm.aml",   // Output Filename
  "SSDT",      // Signature
  0x01,        // DSDT Compliance Revision
  "AMD",       // OEMID
  "TPMACPI",   // TABLE ID
  0x1000       // OEM Revision
  )
{
  //The TPM 2.0 Device Object MUST be located under the system bus at “root\_SB”.
  Scope (\_SB)
  {
    //Debug purpose
    OperationRegion (DBG1, SystemIO, 0x80, 0x2)
    Field (DBG1, WordAcc, NoLock, Preserve)
    {
        P80H, 16
    }
    Device (TPM2)
    {
      //The actual plug and play hardware identifier (e.g. _HID) for the TPM 2.0
      // device object MUST be MSFT0101 or the device MUST have a compatible ID of 
      //MSFT0101 and the _CID could be vendor specific.
      Name (_HID, "MSFT0101")
      Name (_CID, "MSFT0101")
      Name (_STR, Unicode ("TPM 2.0 Device"))

      Method (_STA, 0)
      {
        Return (0x0F)
      }

      Name (_CRS, ResourceTemplate () {
        //TPM2.0 Command Buffer allocate by BIOS, should be updated during POST
        //Memory32Fixed (ReadWrite, 0xBBBBBBBB, 0x100000)
        Memory32Fixed (ReadWrite, 0xBBBBBBBB, 0x100000)
        //TPM2.0 Response Buffer allocate by BIOS, should be updated during POST
        //Memory32Fixed (ReadWrite, 0xCCCCCCCC, 0x100000)
        Memory32Fixed (ReadWrite, 0xCCCCCCCC, 0x100000)
      })

      //TPM2 ACPI Start Method
      Method (TPMS, 3, Serialized, 0, {BuffObj, PkgObj, IntObj, StrObj}, {UnknownObj, UnknownObj, UnknownObj})
      {
        //
        // Function 0: Return supported functions, based on revision
        //
        If (LEqual (Arg1, 0)) {
          Store (0xA2, P80H)
          Return (Buffer () {0x03})
        }
        //
        // Function 1: TPM Start
        //
        If (LEqual (Arg1, 1)) {
          //Do Nothing currently
          Store (0xA3, P80H)
          Return (0)
        }
        //No support Function ID
        Return (1)
      }
      //
      // _DSM – Device Specific Method
      //
      // Arg0:  UUID    Unique function identifier
      // Arg1:   Integer   Revision Level
      // Arg2:   Integer   Function Index (0 = Return Supported Functions)
      // Arg3:   Package   Parameters
      Method (_DSM, 4, Serialized, 0, {IntObj,BuffObj},{BuffObj, IntObj, IntObj, PkgObj})
      {
        Store (0xA0, P80H)
        Sleep (2000)
        // Optional ACPI Start Method
        If (LEqual (Arg0, ToUUID("6bbf6cab-5463-4714-b7cd-f0203c0368d4"))) {
          Store (0xA1, P80H)
          Sleep (2000)
          Return (TPMS (Arg1, Arg2, Arg3))
        }
        //Platform specific implementation >>

        //clear the MOR bit, refer to "TCG Platform Reset Attack Mitigation Specification " for details
        // If (LEqual (Arg0, ToUUID("376054ED-CC13-4675-901C-4756D7F2D45D"))) {
        // }

        //Physical Presence Interface, refer to "TCG Physical Presence Interface Specification" for details
        // If (LEqual (Arg0, ToUUID("376054ED-CC13-4675-901C-4756D7F2D45D"))) {
        // }

        //Default return
        return (Buffer () {0})
      }
    }
  }
}
