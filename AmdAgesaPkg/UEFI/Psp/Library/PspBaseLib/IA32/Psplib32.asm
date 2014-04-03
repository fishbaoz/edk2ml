;/**
; * @file
; *
; * PSP library 32bit
; *
; * Contains AMD PSP Library
; *
; * @xrefitem bom "File Content Label" "Release Content"
; * @e project:      AGESA
; * @e sub-project:  PSP
; * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
; */
;*****************************************************************************
;
; Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
;
; AMD is granting you permission to use this software (the Materials)
; pursuant to the terms and conditions of your Software License Agreement
; with AMD.  This header does *NOT* give you permission to use the Materials
; or any rights under AMD's intellectual property.  Your use of any portion
; of these Materials shall constitute your acceptance of those terms and
; conditions.  If you do not agree to the terms and conditions of the Software
; License Agreement, please do not use any portion of these Materials.
;
; CONFIDENTIALITY:  The Materials and all other information, identified as
; confidential and provided to you by AMD shall be kept confidential in
; accordance with the terms and conditions of the Software License Agreement.
;
; LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
; PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
; WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
; MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
; OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
; IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
; (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
; INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
; GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
; RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
; THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
; EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
; THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
;
; AMD does not assume any responsibility for any errors which may appear in
; the Materials or any other related information provided to you by AMD, or
; result from use of the Materials or any related information.
;
; You agree that you will not reverse engineer or decompile the Materials.
;
; NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
; further information, software, technical information, know-how, or show-how
; available to you.  Additionally, AMD retains the right to modify the
; Materials at any time, without notice, and is not obligated to provide such
; modified Materials to you.
;
; U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
; "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
; subject to the restrictions as set forth in FAR 52.227-14 and
; DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
; Government constitutes acknowledgement of AMD's proprietary rights in them.
;
; EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
; direct product thereof will be exported directly or indirectly, into any
; country prohibited by the United States Export Administration Act and the
; regulations thereunder, without the required authorization from the U.S.
; government nor will be used for any purpose prohibited by the same.
;*****************************************************************************

.586p
.xmm
.model  flat
ASSUME FS:NOTHING
.code

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Write IO byte
; *
; *  @param[in]   Address IO port address
; *  @param[in]   Data    IO port Value
; */

public  PspLibWriteIo8
PspLibWriteIo8        PROC NEAR C USES DX AX Address:WORD, Data:Byte
        mov     dx, Address
        mov     al, Data
        out     dx, al
        ret
PspLibWriteIo8        ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Write IO word
; *
; *  @param[in]   Address IO port address
; *  @param[in]   Data    IO port Value
; */
public PspLibWriteIo16
PspLibWriteIo16       PROC NEAR C USES DX AX Address:WORD, Data:WORD
        mov     dx, Address
        mov     ax, Data
        out     dx, ax
        ret
PspLibWriteIo16       ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Write IO dword
; *
; *  @param[in]   Address IO port address
; *  @param[in]   Data    IO port Value
; */

public PspLibWriteIo32
PspLibWriteIo32       PROC NEAR C USES DX EAX Address:WORD, Data:DWORD
        mov     dx, Address
        mov     eax, Data
        out     dx, eax
        ret
PspLibWriteIo32       ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read IO byte
; *
; *  @param[in] - IO port address
; *  @retval      IO port Value
; */
public PspLibReadIo8
PspLibReadIo8 PROC NEAR C USES DX Address:WORD
        mov     dx, Address
        in      al, dx
        ret
PspLibReadIo8 ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read IO word
; *
; *  @param[in]   Address IO port address
; *  @retval      IO port Value
; */
public PspLibReadIo16
PspLibReadIo16        PROC NEAR C USES DX Address:WORD
        mov     dx, Address
        in      ax, dx
        ret
PspLibReadIo16        ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read IO dword
; *
; *  @param[in]   Address  IO port address
; *  @retval      IO port Value
; */
public PspLibReadIo32
PspLibReadIo32        PROC NEAR C USES DX Address:WORD
        mov     dx, Address
        in      eax, dx
        ret
PspLibReadIo32        ENDP


;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read MSR
; *
; *  @param[in]  Address  MSR Address
; *  @param[in]  Data     Pointer to data
; *  @param[in]  ConfigPtr (Optional)
; */
public PspLibMsrRead
PspLibMsrRead  PROC NEAR C USES ECX ESI EDX Address:DWORD, Value:PTR, ConfigPtr:PTR
        mov     esi, ConfigPtr        ;Dummy read to avoid compilation warning
        mov     ecx, Address
        rdmsr
        mov     esi, Value
        mov     [esi],   eax
        mov     [esi+4], edx
        ret
PspLibMsrRead  ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Write MSR
; *
; *  @param[in]  Address    MSR Address
; *  @param[in]  Data       Pointer to data
; *  @param[in]  ConfigPtr  (Optional)
; */
public PspLibMsrWrite
PspLibMsrWrite                PROC NEAR C USES ECX ESI EDX Address:DWORD, Data:PTR, ConfigPtr:PTR
        mov     esi, ConfigPtr         ;Dummy read to avoid compilation warning
        mov     ecx, Address
        mov     esi, Data
        mov     eax, [esi]
        mov     edx, [esi+4]
        wrmsr
        ret
PspLibMsrWrite                ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read TSC
; *
; *
; *
; */

public PspLibReadTSC
PspLibReadTSC  PROC    NEAR C
        rdtsc
        ret
PspLibReadTSC  ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read various CPU registers
; *
; * @param[in]  Reg     Register ID (0/4 - CR0/CR4, 10h/11h/12h/13h/17h - DR0/DR1/DR2/DR3/DR7)
; * @param[in]  Value   Value to write
; */

PspLibReadCpuReg PROC NEAR C Reg:BYTE, Value:NEAR PTR DWORD
    pushad
    push ds

    .if(Reg == 00h)
        mov eax, cr0
    .elseif(Reg == 04h)
        mov eax, cr4
    .elseif(Reg == 10h)
        mov eax, dr0
    .elseif(Reg == 11h)
        mov eax, dr1
    .elseif(Reg == 12h)
        mov eax, dr2
    .elseif(Reg == 13h)
        mov eax, dr3
    .elseif(Reg == 17h)
        mov eax, dr7
    .else
        xor eax,eax
    .endif

    mov edi, Value
    mov [edi], eax

    pop ds
    popad
    ret
PspLibReadCpuReg ENDP



;/*---------------------------------------------------------------------------------------*/
;/**
; *  Write various CPU registers
; *
; * @param[in]  Reg     Register ID (0/4 - CR0/CR4, 10h/11h/12h/13h/17h - DR0/DR1/DR2/DR3/DR7)
; * @param[in]  Value   Value to write
; */

PspLibWriteCpuReg PROC NEAR C Reg:BYTE, Value:DWORD
    mov eax, Value

    .if(Reg == 00h)
        mov cr0, eax
    .elseif(Reg == 4)
        mov cr4, eax
    .elseif(Reg == 10h)
        mov dr0, eax
    .elseif(Reg == 11h)
        mov dr1, eax
    .elseif(Reg == 12h)
        mov dr2, eax
    .elseif(Reg == 13h)
        mov dr3, eax
    .elseif(Reg == 17h)
        mov dr7, eax
    .endif
    ret
PspLibWriteCpuReg ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Enter debugger on SimNow
; *
; *
; *
; */
PUBLIC PspLibSimNowEnterDebugger
PspLibSimNowEnterDebugger PROC NEAR C
    pushad
    mov     eax, 0BACCD00Bh         ; Backdoor in SimNow
    mov     ebx, 2                  ; Select breakpoint feature
    cpuid
    popad
    ret
PspLibSimNowEnterDebugger ENDP

END
