      TITLE   CpuAsm.asm: 
;------------------------------------------------------------------------------
;*
;*   Copyright (c) 2008 - 2013, Intel Corporation. All rights reserved.<BR>
;*   This program and the accompanying materials                          
;*   are licensed and made available under the terms and conditions of the BSD License         
;*   which accompanies this distribution.  The full text of the license may be found at        
;*   http://opensource.org/licenses/bsd-license.php                                            
;*                                                                                             
;*   THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
;*   WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             
;*   
;*    CpuAsm.asm
;*  
;*   Abstract:
;*
;------------------------------------------------------------------------------

    .code

;------------------------------------------------------------------------------
; VOID
; SetCodeSelector (
;   UINT16 Selector
;   );
;------------------------------------------------------------------------------
SetCodeSelector PROC PUBLIC

    ; save rsp because iret will align it to a 16 byte boundary
    mov rdx, rsp

    ; use iret to jump to a 64-bit offset in a new code segment
    ; iret will pop cs:rip, flags, then ss:rsp
    mov     ax, ss      ; need to push ss..
    push    rax         ; push ss instuction not valid in x64 mode, so use ax
    push    rsp
    pushfq
    push    rcx         ; cx is code segment selector from caller
    mov     rax, offset setCodeSelectorLongJump
    push    rax

    ; the iret will continue at next instruction, with the new cs value loaded
    iretq

setCodeSelectorLongJump:
    ; restore rsp, it might not have been 16-byte aligned on entry
    mov     rsp, rdx

    ret
SetCodeSelector ENDP

;------------------------------------------------------------------------------
; VOID
; SetDataSelectors (
;   UINT16 Selector
;   );
;------------------------------------------------------------------------------
SetDataSelectors PROC PUBLIC
    mov     ss, cx
    mov     ds, cx
    mov     es, cx
    mov     fs, cx
    mov     gs, cx
    ret
SetDataSelectors ENDP

END

