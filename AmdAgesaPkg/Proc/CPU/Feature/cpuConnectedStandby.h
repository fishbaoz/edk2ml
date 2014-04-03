/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD AGESA CPU Connected Standby Functions declarations.
 *
 * Contains code that declares the AGESA CPU Connected Standby related APIs
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

#ifndef _CPU_CONNECTED_STANDBY_H_
#define _CPU_CONNECTED_STANDBY_H_

/*----------------------------------------------------------------------------------------
 *          M I X E D   (Definitions And Macros / Typedefs, Structures, Enums)
 *----------------------------------------------------------------------------------------
 */
//  Forward declaration needed for multi-structure mutual references
AGESA_FORWARD_DECLARATION (CONNECTED_STANDBY_FAMILY_SERVICES);

/*----------------------------------------------------------------------------------------
 *                 D E F I N I T I O N S     A N D     M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define BSU8(u8)      ((u8)  & 0xFF)
#define BSU16(u16)    ((u16) & 0xFF), (((u16) >> 8) & 0xFF)
#define BSU32(u32)    ((u32) & 0xFF), (((u32) >> 8) & 0xFF), (((u32) >> 16) & 0xFF), (((u32) >> 24) & 0xFF)
#define BSU64(u64)    ((u64) & 0xFF), (((u64) >> 8) & 0xFF), (((u64) >> 16) & 0xFF), (((u64) >> 24) & 0xFF), (((u64) >> 32) & 0xFF), (((u64) >> 40) & 0xFF), (((u64) >> 48) & 0xFF), (((u64) >> 56) & 0xFF)

/*----------------------------------------------------------------------------------------
 *                    T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to check if Connected Standby is supported.
 *
 * @param[in]    ConnectedStandbyServices  Connected Standby services.
 * @param[in]    PlatformConfig     Contains the runtime modifiable feature input data.
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @retval       TRUE               Connected Standby is supported.
 * @retval       FALSE              Connected Standby is not supported.
 *
 */
typedef BOOLEAN F_CONNECTED_STANDBY_IS_SUPPORTED (
  IN       CONNECTED_STANDBY_FAMILY_SERVICES *ConnectedStandbyServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

/// Reference to a Method.
typedef F_CONNECTED_STANDBY_IS_SUPPORTED *PF_CONNECTED_STANDBY_IS_SUPPORTED;

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to enable Connected Standby.
 *
 * @param[in]    ConnectedStandbyServices  Connected Standby services.
 * @param[in]    PlatformConfig     Contains the runtime modifiable feature input data.
 * @param[in]    EntryPoint         Timepoint designator.
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Family specific error value.
 *
 */
typedef AGESA_STATUS F_CONNECTED_STANDBY_INIT (
  IN       CONNECTED_STANDBY_FAMILY_SERVICES *ConnectedStandbyServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       UINT64                 EntryPoint,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

/// Reference to a Method.
typedef F_CONNECTED_STANDBY_INIT *PF_CONNECTED_STANDBY_INIT;

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to build up the Connected Standby restoration table.
 *
 * @param[in]    ConnectedStandbyServices  Connected Standby services.
 * @param[in]    RestorationTable          Complete family specific restoration table
 * @param[in]    EntryPoint                Size of family specific restoration table
 * @param[in]    StdHeader                 Config Handle for library, services.
 *
 * @return       Family specific error value.
 *
 */
typedef AGESA_STATUS F_PREPARE_RESTORATION_BUFFER (
  IN       CONNECTED_STANDBY_FAMILY_SERVICES *ConnectedStandbyServices,
     OUT   VOID                              **RestorationTable,
     OUT   UINT32                            *TableSize,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

/// Reference to a Method.
typedef F_PREPARE_RESTORATION_BUFFER *PF_PREPARE_RESTORATION_BUFFER;

/**
 * Provide the interface to the Connected Standby Family Specific Services.
 *
 * Use the methods or data in this struct to adapt the feature code to a specific cpu family or model (or stepping!).
 * Each supported Family must provide an implementation for all methods in this interface, even if the
 * implementation is a CommonReturn().
 */
struct _CONNECTED_STANDBY_FAMILY_SERVICES {
  UINT16          Revision;                                             ///< Interface version
  // Public Methods.
  PF_CONNECTED_STANDBY_IS_SUPPORTED IsConnectedStandbySupported;        ///< Method: Family specific call to check if Connected Standby is supported.
  PF_CONNECTED_STANDBY_INIT         EnableConnectedStandby;             ///< Method: Family specific call to enable Connected Standby.
  PF_PREPARE_RESTORATION_BUFFER     PrepareRestorationBuffer;           ///< Method: Family specific call to prepare the replay table.
};


/*----------------------------------------------------------------------------------------
 *                          F U N C T I O N S     P R O T O T Y P E
 *----------------------------------------------------------------------------------------
 */

#endif  // _CPU_CONNECTED_STANDBY_H_
