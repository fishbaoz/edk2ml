/* $NoKeywords: $ */
/**
 * @file
 *
 * AMD Device Path Library for RAS
 *
 * Device Path Protocol navigation Library
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI/Addendum/AmdPlatformRas/RAS
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ****************************************************************************
 * AMD Generic Encapsulated Software Architecture
 *
 * Description: AmdDevicePathLib.c - Device Path Protocol navigation Library.
 *
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
#include "Tiano.h"
#include "AmdDevicePathLib.h"
#include "EfiCommonLib.h"


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
EFI_BOOT_SERVICES             *gBS;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


VOID*
CopyDevicePath (
  IN       EFI_DEVICE_PATH_PROTOCOL *DevicePath
  );

UINTN
DevicePathLength (
  IN       EFI_DEVICE_PATH_PROTOCOL *DevicePath
  );

VOID*
AddDevicePathInstance (
  IN       EFI_DEVICE_PATH_PROTOCOL *DevicePathLoadedImage,
  IN       EFI_DEVICE_PATH_PROTOCOL *DevicePathToAdd
  );

/**
 *---------------------------------------------------------------------------------------
 *
 *  AddDevicePath
 *
 *  Description:
 *     Returns a pointer to an added device path protocol b.
 *
 *  Parameters:
 *    @param[in]     *DevicePathLoadedImage
 *    @param[in]     *DevicePathToAdd
 *
 *    @retval        EFI_DEVICE_PATH_PROTOCOL*
 *
 *---------------------------------------------------------------------------------------
 **/

EFI_DEVICE_PATH_PROTOCOL*
AddDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePathLoadedImage,
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePathToAdd
  )
{
  EFI_DEVICE_PATH_PROTOCOL* NewDevicePath = NULL;

  // Check for a valid Device Path Pointer for which we intend to add
  if (!DevicePathToAdd) {
    return DevicePathLoadedImage;
  }
  // Check for a valid Device Path Pointer for which we intend to add
  if (!DevicePathLoadedImage) {
    return CopyDevicePath (DevicePathToAdd);
  } else {
    NewDevicePath = AddDevicePathInstance (DevicePathLoadedImage, DevicePathToAdd);
    return NewDevicePath;
  }
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  CopyDevicePath
 *
 *  Description:
 *  Copies the user provided device path into a new buffer and returns its address.
 *
 *  Parameters:
 *    @param[in]     *DevicePath
 *
 *
 *  Uses:
 *    DPLength
 *    Malloc
 *    MemCpy
 *---------------------------------------------------------------------------------------
 **/
VOID*
CopyDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
  )
{
  UINTN Length;
  UINT8 *Buffer;

  // Check for a valid DevicePath pointer
  if (!DevicePath) {
    return NULL;
  }
  // Get length of device path
  Length = DevicePathLength (DevicePath);

  // Create a buffer for the device to be copied to
  gBS->AllocatePool (EfiBootServicesData, Length, (VOID **) &Buffer);
  // Zero out memory buffer
  EfiCommonLibZeroMem (Buffer, Length);
  // Copy device path into the buffer using BootServices routine
  gBS->CopyMem (Buffer, DevicePath, Length);

  return Buffer;  // Return pointer to the buffer containing the copied device path
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  DevicePathLength
 *
 *  Description:
 *  Returns the length of the provided device path (including the size of EndofDevicePath).
 *
 *  Parameters:
 *    @param[in]     *DevicePath
 *
 *    @retval        UINTN
 *
 *
 *  Uses:
 *  isEndNode
 *  NEXT_NODE
 *  NODE_LENGTH
 *---------------------------------------------------------------------------------------
 **/
UINTN
DevicePathLength (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
  )
{
  UINTN NodeLength;
  UINTN PathSize;

  NodeLength = 0;
  PathSize = 0;

  // Check for a valid DevicePath pointer
  if (!DevicePath) {
    return 0;  // Invalid Device Path Pointer, return 0
  }
  while (!IsDevicePathEnd (DevicePath)) {
    NodeLength = DevicePathNodeLength (DevicePath); // Device path Node Length MACRO

    //Protection from the junk data. Zero type and zero length are illegal.
    //If we encountered them, return what we have
    if (!DevicePathType (DevicePath) || !NodeLength) {
      return PathSize;
    }

    // Get pointer to next next node in the device path
    // DevicePath = NEXT_NODE (DevicePath);
    DevicePath = NextDevicePathNode (DevicePath);  // Next node in device path MACRO
    // Add the node length to the running total device path length
    PathSize += NodeLength;
  }

  return PathSize + END_DEVICE_PATH_LENGTH; // add (sizeof (EFI_DEVICE_PATH_PROTOCOL) = 4 for return
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  DevicePathAddInstance
 *
 *  Description:
 *  Makes a new device path with pDevicePath#1 as its first instance and pDevicePath#2
 *  as its second instance, then returns the result in a newly allocated buffer.
 *
 *  Notes:
 *    User is responsible for managing this piece of memory.
 *
 *  Parameters:
 *    @param[in]     *DevicePathLoadedImage
 *    @param[in]     *DevicePathToAdd
 *
 *    @retval        VOID* address of the modified device path.
 *
 *
 *  Uses:
 *  CopyDevicePath
 *  DevicePathLength
 *
 *---------------------------------------------------------------------------------------
 **/
VOID*
AddDevicePathInstance (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePathLoadedImage,
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePathToAdd
  )
{
  UINTN LengthLoadedDevicePath;
  UINTN LengthDevicePathToAdd;
  UINT8 *NewDevicePathBuffer;
  UINT8 *WorkingBuffer;

  NewDevicePathBuffer = NULL;
  WorkingBuffer = NULL;

  // Check to see if DevicePath is a valid pointer
  if (!DevicePathToAdd) {
    return DevicePathLoadedImage;  // No Valid Device Path available
  }

  // Check to see if pFilePathList is a valid pointer
  if (!DevicePathLoadedImage) {
    return CopyDevicePath (DevicePathToAdd);
  }

  // Get the length of File Path List Protocol
  LengthLoadedDevicePath = DevicePathLength (DevicePathLoadedImage);
  // Subtract End-Device-Path marker (4-bytes) from Loaded Device Path length
  LengthLoadedDevicePath -= END_DEVICE_PATH_LENGTH;

  // Get the length of new Device Path Protocol
  LengthDevicePathToAdd = DevicePathLength (DevicePathToAdd);

  // Allocate Boot Services memory for total of file path list and new Device Path Protocol buffer
  gBS->AllocatePool (EfiBootServicesData, (LengthLoadedDevicePath + LengthDevicePathToAdd), (VOID **) &NewDevicePathBuffer);
  // Zero out memory buffer
  EfiCommonLibZeroMem (NewDevicePathBuffer, (LengthLoadedDevicePath + LengthDevicePathToAdd));
  // Set local working buffer equal to new allocated device path protocol buffer
  WorkingBuffer = NewDevicePathBuffer;
  // Copy the Loaded Device Path into working buffer
  gBS->CopyMem (WorkingBuffer, DevicePathLoadedImage, LengthLoadedDevicePath);
  // Move the pointer of the working buffer to the end of File Path List
  WorkingBuffer += LengthLoadedDevicePath;
  // Now copy the File Path List Protocol into the working buffer starting right after Device Path Protocol data
  gBS->CopyMem (WorkingBuffer, DevicePathToAdd, LengthDevicePathToAdd);

  // Now just return pointer to both Device Path Protocols
  return NewDevicePathBuffer;
}
