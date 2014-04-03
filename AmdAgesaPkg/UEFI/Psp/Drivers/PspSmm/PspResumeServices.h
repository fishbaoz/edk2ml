/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP related functions in SMM
 *
 * Contains PSP SMM Resume functions prototype
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
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
 *
 ***************************************************************************/

#ifndef _PSP_SMM_RESUME_SERVIES_H_
#define _PSP_SMM_RESUME_SERVIES_H_

#include "Tiano.h"
#include "EfiDebug.h"
#include "EfiCombinationLib.h"
#include "EfiSmmDriverLib.h"
#include "CpuIa32.h"
#include "LinkedList.h"
#include "PspMbox.h"
#include "PspResumeServiceProtocol.h"

#include EFI_PROTOCOL_DEPENDENCY (SmmBase)

/// Define the structure for Smmsave override during resume @todo move into seperate OEM header
typedef struct {
  UINT16   CodeSegOverrideValue;  ///< CodeSegOverrideValue
  UINT16   CodeSegAttrib;         ///< CodeSegAttrib
  UINT16   DateSegOverrideValue;  ///< DateSegOverrideValue
  UINT16   DataSegAttrib;         ///< DataSegAttrib
  UINT64   RipBspOverrideValue;   ///< RipBspOverrideValue
  UINT64   RipApOverrideValue;    ///< RipApOverrideValue
  UINT64   RaxOverrideValue;      ///< RaxOverrideValue
  UINT64   RdxOverrideValue;      ///< RdxOverrideValue
  UINT64   StackBase;             ///< RspOverrideValue
  UINT64   BspStackSize;          ///< RspOverrideValue
  UINT64   ApStackSize;           ///< RspOverrideValue
  UINT64   GdtOverrideValue;      ///< GdtOverrideValue
  UINT64   GdtOverrideLimit;      ///< GdtOverrideLimit
  UINT64   Cr0;                   ///< Cr0
  UINT64   Cr3;                   ///< Cr3
  UINT64   Cr4;                   ///< Cr4
} REGISTER_OVERRIDE;


EFI_STATUS
EFIAPI
InstallPspResumeCallbackProtocol ();

EFI_STATUS
EFIAPI
PspRegister (
  IN       PSP_RESUME_SERVICE_PROTOCOL        *This,
  IN       PSP_RESUME_CALLBACK                CallBackFunction,
  IN OUT   VOID                               *Context,
  IN       UINTN                              CallbackPriority,
  OUT      EFI_HANDLE                         *DispatchHandle
);


EFI_STATUS
EFIAPI
PspUnregister (
  IN       PSP_RESUME_SERVICE_PROTOCOL        *This,
  IN       EFI_HANDLE                         DispatchHandle
  );

EFI_STATUS
EFIAPI
PspResumeServiceCallBack (
  IN       UINT8 ResumeType
   );

VOID S3ExitToBios ();

VOID LockedIncrement (
  VOLATILE VOID *Address
  );

VOID LockedDecrement (
  VOLATILE VOID *Address
  );

VOID UpdateS3SaveAreaMap ();

/// Define layout of SMM save area
typedef struct {
  UINT16 ES_Selector;                ///< 0xFE00, sizeof (UINT16)
  UINT16 ES_Attributes;              ///< 0xFE02, sizeof (UINT16)
  UINT32 ES_Limit;                   ///< 0xFE04, sizeof (UINT32)
  UINT64 ES_Base;                    ///< 0xFE08, sizeof (UINT64)
  UINT16 CS_Selector;                ///< 0xFE10, sizeof (UINT16)
  UINT16 CS_Attributes;              ///< 0xFE12, sizeof (UINT16)
  UINT32 CS_Limit;                   ///< 0xFE14, sizeof (UINT32)
  UINT64 CS_Base;                    ///< 0xFE18, sizeof (UINT64)
  UINT16 SS_Selector;                ///< 0xFE20, sizeof (UINT16)
  UINT16 SS_Attributes;              ///< 0xFE22, sizeof (UINT16)
  UINT32 SS_Limit;                   ///< 0xFE24, sizeof (UINT32)
  UINT64 SS_Base;                    ///< 0xFE28, sizeof (UINT64)
  UINT16 DS_Selector;                ///< 0xFE30, sizeof (UINT16)
  UINT16 DS_Attributes;              ///< 0xFE32, sizeof (UINT16)
  UINT32 DS_Limit;                   ///< 0xFE34, sizeof (UINT32)
  UINT64 DS_Base;                    ///< 0xFE38, sizeof (UINT64)
  UINT16 FS_Selector;                ///< 0xFE40, sizeof (UINT16)
  UINT16 FS_Attributes;              ///< 0xFE42, sizeof (UINT16)
  UINT32 FS_Limit;                   ///< 0xFE44, sizeof (UINT32)
  UINT64 FS_Base;                    ///< 0xFE48, sizeof (UINT64)
  UINT16 GS_Selector;                ///< 0xFE50, sizeof (UINT16)
  UINT16 GS_Attributes;              ///< 0xFE52, sizeof (UINT16)
  UINT32 GS_Limit;                   ///< 0xFE54, sizeof (UINT32)
  UINT64 GS_Base;                    ///< 0xFE58, sizeof (UINT64)
  UINT8  Rsvd_FE60_63[4];            ///< 0xFE60, sizeof (UINT32)
  UINT16 GDTR_Limit;                 ///< 0xFE64, sizeof (UINT16)
  UINT8  Rsvd_FE66_FE67[2];          ///< 0xFE66, sizeof (UINT16)
  UINT64 GDTR_Base;                  ///< 0xFE68, sizeof (UINT64)
  UINT16 LDTR_Selector;              ///< 0xFE70, sizeof (UINT16)
  UINT16 LDTR_Attributes;            ///< 0xFE72, sizeof (UINT16)
  UINT32 LDTR_Limit;                 ///< 0xFE74, sizeof (UINT32)
  UINT64 LDTR_Base;                  ///< 0xFE78, sizeof (UINT64)
  UINT8  Rsvd_FE80_FE83[4];          ///< 0xFE80, sizeof (UINT32)
  UINT16 IDTR_Limit;                 ///< 0xFE84, sizeof (UINT16)
  UINT8  Rsvd_FE86_FE87[2];          ///< 0xFE86, sizeof (UINT16)
  UINT64 IDTR_Base;                  ///< 0xFE88, sizeof (UINT64)
  UINT16 TR_Selector;                ///< 0xFE90, sizeof (UINT16)
  UINT16 TR_Attributes;              ///< 0xFE92, sizeof (UINT16)
  UINT32 TR_Limit;                   ///< 0xFE94, sizeof (UINT32)
  UINT64 TR_Base;                    ///< 0xFE98, sizeof (UINT64

  UINT64 IO_RIP;                     ///< 0xFEA0, sizeof (UINT64)
  UINT64 IO_RCX;                     ///< 0xFEA8, sizeof (UINT64)
  UINT64 IO_RSI;                     ///< 0xFEB0, sizeof (UINT64)
  UINT64 IO_RDI;                     ///< 0xFEB8, sizeof (UINT64)
  UINT32 IO_Restart_Dword;           ///< 0xFEC0, sizeof (UINT32)
  UINT8  Rsvd_FEC4_FEC7[4];          ///< 0xFEC4, sizeof (UINT32)
  UINT8 IO_Restart;                  ///< 0xFEC8, sizeof (UINT8)
  UINT8 Auto_Halt_Restart;           ///< 0xFEC9, sizeof (UINT8)
  UINT8  Rsvd_FECA_FECF[6];          ///< 0xFECA, 6 byte

  UINT64 EFER;                       ///< 0xFED0, sizeof (UINT64)
  UINT64 SVM_Guest;                  ///< 0xFED8, sizeof (UINT64)
  UINT64 SVM_Guest_VMCB_Address;     ///< 0xFEE0, sizeof (UINT64)
  UINT64 SVM_Guest_Virtual_Interrupt;///< 0xFEE8, sizeof (UINT64)
  UINT8  Rsvd_FFF0_FFFB[12];         ///< 0xFEF0, 12 byte
  UINT32 SMM_Revision_Identifier1;   ///< 0xFEFC, sizeof (UINT32)
  UINT32 SMBASE;                     ///< 0xFF00, sizeof (UINT32)
  UINT8  Rsvd_FF04_FF1F[28];         ///< 0xFF04, 28 byte
  UINT64 SVM_Guest_PAT;              ///< 0xFF20, sizeof (UINT64)
  UINT64 SVM_Host_EFER;              ///< 0xFF28, sizeof (UINT64)
  UINT64 SVM_Host_CR4;               ///< 0xFF30, sizeof (UINT64)
  UINT64 SVM_Host_CR3;               ///< 0xFF38, sizeof (UINT64)
  UINT64 SVM_Host_CR0;               ///< 0xFF40, sizeof (UINT64)

  UINT64 CR4;                        ///< 0xFF48, sizeof (UINT64)
  UINT64 CR3;                        ///< 0xFF50, sizeof (UINT64)
  UINT64 CR0;                        ///< 0xFF58, sizeof (UINT64)
  UINT64 DR7;                        ///< 0xFF60, sizeof (UINT64)
  UINT64 DR6;                        ///< 0xFF68, sizeof (UINT64)
  UINT64 RFLAGS;                     ///< 0xFF70, sizeof (UINT64)
  UINT64 RIP;                        ///< 0xFF78, sizeof (UINT64)

  UINT64 R15;                        ///< 0xFF80, sizeof (UINT64)
  UINT64 R14;                        ///< 0xFF88, sizeof (UINT64)
  UINT64 R13;                        ///< 0xFF90, sizeof (UINT64)
  UINT64 R12;                        ///< 0xFF98, sizeof (UINT64)
  UINT64 R11;                        ///< 0xFFA0, sizeof (UINT64)
  UINT64 R10;                        ///< 0xFFA8, sizeof (UINT64)
  UINT64 R9;                         ///< 0xFFB0, sizeof (UINT64)
  UINT64 R8;                         ///< 0xFFB8, sizeof (UINT64)
  UINT64 RDI;                        ///< 0xFFC0, sizeof (UINT64)
  UINT64 RSI;                        ///< 0xFFC8, sizeof (UINT64)
  UINT64 RBP;                        ///< 0xFFD0, sizeof (UINT64)
  UINT64 RSP;                        ///< 0xFFD8, sizeof (UINT64)
  UINT64 RBX;                        ///< 0xFFE0, sizeof (UINT64)
  UINT64 RDX;                        ///< 0xFFE8, sizeof (UINT64)
  UINT64 RCX;                        ///< 0xFFF0, sizeof (UINT64)
  UINT64 RAX;                        ///< 0xFFF8, sizeof (UINT64)
} SMM_SAVE_MAP;


/// Define structure for Smm callback on resume from sleep
typedef struct {
  EFI_LIST_ENTRY            ListEntry;          ///< Linked list
  PSP_RESUME_CALLBACK       CallBackFunction;   ///< Callback fn.
  UINTN                     CallbackPriority;   ///< Priority/Order of callback
  VOID                      *Context;           ///< Context
} PSP_SMM_CALLBACK_NODE;

#endif //_PSP_SMM_RESUME_SERVIES_H_
