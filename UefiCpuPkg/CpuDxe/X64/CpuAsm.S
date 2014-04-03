#      TITLE   CpuAsm.S: 

#------------------------------------------------------------------------------
#*
#*   Copyright (c) 2008 - 2013, Intel Corporation. All rights reserved.<BR>
#*   This program and the accompanying materials
#*   are licensed and made available under the terms and conditions of the BSD License
#*   which accompanies this distribution.  The full text of the license may be found at
#*   http://opensource.org/licenses/bsd-license.php
#*
#*   THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#*   WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#*
#*    CpuAsm.S
#*
#*   Abstract:
#*
#------------------------------------------------------------------------------


#text  SEGMENT


#------------------------------------------------------------------------------
# VOID
# SetCodeSelector (
#   UINT16 Selector
#   );
#------------------------------------------------------------------------------
ASM_GLOBAL ASM_PFX(SetCodeSelector)
ASM_PFX(SetCodeSelector):
    # save rsp because iret will align it to a 16 byte boundary
    mov %rsp, %rdx

    # use iret to jump to a 64-bit offset in a new code segment
    # iret will pop cs:rip, flags, then ss:rsp
    mov     %ss, %ax    # need to push ss..
    push    %rax        # push ss instuction not valid in x64 mode, so use ax
    push    %rsp
    pushfq
    push    %rcx        # cx is code segment selector from caller
    leaq    L_setCodeSelectorLongJump(%rip), %rax 
    push    %rax

    # the iret will continue at next instruction, with the new cs value loaded
    iretq

L_setCodeSelectorLongJump:
    # restore rsp, it might not have been 16-byte aligned on entry
    mov     %rdx, %rsp

    ret

#------------------------------------------------------------------------------
# VOID
# SetDataSelectors (
#   UINT16 Selector
#   );
#------------------------------------------------------------------------------
ASM_GLOBAL ASM_PFX(SetDataSelectors)
ASM_PFX(SetDataSelectors):
    movw    %cx, %ss
    movw    %cx, %ds
    movw    %cx, %es
    movw    %cx, %fs
    movw    %cx, %gs
    ret

#text  ENDS

#END


