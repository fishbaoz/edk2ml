;------------------------------------------------------------------------------
;*
;*   Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
;*   This program and the accompanying materials                          
;*   are licensed and made available under the terms and conditions of the BSD License         
;*   which accompanies this distribution.  The full text of the license may be found at        
;*   http://opensource.org/licenses/bsd-license.php                                            
;*                                                                                             
;*   THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
;*   WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             
;*   
;*    efi64.asm
;*  
;*   Abstract:
;*
;------------------------------------------------------------------------------
MKF_DEBUGGER_SUPPORT = 1

JmpCommonIdtEntry  macro
    ; jmp     commonIdtEntry - this must be hand coded to keep the assembler from
    ;                          using a 8 bit reletive jump when the entries are
    ;                          within 255 bytes of the common entry.  This must
    ;                          be done to maintain the consistency of the size
    ;                          of entry points...
    db      0e9h                        ; jmp 16 bit relative 
    dd      commonIdtEntry - $ - 4      ;  offset to jump to
endm    

;------------------------------------------------------------------------------
; Now in 32-bit legacy mode [if launched as coreboot payload]
;     or 64-bit long mode   [if launched from disk boot]
; Write the following 32-bit code so that it is also valid
; if executed in x64 mode (avoid INC, etc).
;------------------------------------------------------------------------------

        .586p
        .model  flat        
_TEXT32 SEGMENT USE32

        org EFI64CODESTART
text32start:
        
DEFAULT_HANDLER_SIZE EQU INT1 - INT0

        
Start:
    ; make final stack aligned
    mov     esp,X64_STACK_TOP - 8

    ; enable OSFXSR and OSXMMEXCPT
    mov     eax, cr4
    bts     eax, 9
    bts     eax, 10
    mov     cr4, eax

    ; switch to long mode if currently in 32-bit legacy mode
    mov  ecx, 0c0000080h ; EFER MSR number
    rdmsr
    bt   eax, 8
    jc   inLongMode

;----------------------------------------------------------------------------
; called as a coreboot payload, coreboot unique code goes here
;----------------------------------------------------------------------------
    ; the upper 64KB of DOS low memory will be used
    ; for tmp page tables and real mode code
    mov     edi, X64_PAGE_TABLE_BASE

    ; build identity mapped page table needed for 64-bit mode
    call    identityMapLongMode

    ; setup 16-bit real mode vectors required by BiosVideoThunkDxe
    call    setupRmVectors
    call    setupDebug

    call    readEip
    lea     ebx, [eax + (dataStart - $)]  ; ebx is dataStart

    ; load new GDT
    lgdt    fword ptr [ebx + (gdtr - dataStart)]

    ; load new IDT
    lidt    fword ptr [ebx + (idtr - dataStart)]

    ; enable PAE/PSE
    mov     eax, cr4
    bts     eax, 5
    mov     cr4, eax

    mov  eax, X64_PAGE_TABLE_BASE
    mov  cr3, eax

    ; LM enable
    mov     ecx, 0c0000080h
    rdmsr
    or      eax, 100h
    wrmsr

    ; enable paging
    mov     eax, cr0
    bts     eax, 31
    mov     cr0, eax

    lea     eax, [ebx + (inLongMode - dataStart)]
    push    8
    push    eax
    retf

;---------------------------------------------
;use64
;---------------------------------------------

inLongMode:
;----------------------------temp-------------------------------
;    mov     edi, 86000h
;    call    setupDebug
;----------------------------temp-------------------------------

    ; Populate IDT with meaningful offsets for exception handlers...
    mov     eax, offset Idtr
    sidt    fword ptr [eax]             ; get fword address of IDT

    mov     eax, offset Halt
    mov     ebx, eax                    ; use bx to copy 15..0 to descriptors
    shr     eax, 16                     ; use ax to copy 31..16 to descriptors 
                                        ; 63..32 of descriptors is 0
    mov     ecx, 78h                    ; 78h IDT entries to initialize with unique entry points (exceptions)
    mov     esi, [offset Idtr + 2]
    mov     edi, [esi]

@@:                                             ; loop through all IDT entries exception handlers and initialize to default handler
    mov     word ptr [edi], bx                  ; write bits 15..0 of offset
    mov     word ptr [edi+2], 38h               ; SYS_CODE64_SEL from GDT
    mov     word ptr [edi+4], 0e00h OR 8000h    ; type = 386 interrupt gate, present
    mov     word ptr [edi+6], ax                ; write bits 31..16 of offset
    mov     dword ptr [edi+8], 0                ; write bits 63..32 of offset
    add     edi, 16                             ; move up to next descriptor
    add     bx, DEFAULT_HANDLER_SIZE            ; move to next entry point
    loop    @b                                  ; loop back through again until all descriptors are initialized
    
    mov     esi,EFI64CODESTART+EFI64CODESIZE
    mov     eax,[esi+014h]              ; eax = [22014]
    add     esi,eax                     ; esi = 22000 + [22014] = Base of EFILDR.C
    mov     ebp,[esi+03ch]              ; ebp = [22000 + [22014] + 3c] = NT Image Header for EFILDR.C
    add     ebp,esi
    mov     edi,[ebp+030h]              ; edi = [[22000 + [22014] + 3c] + 2c] = ImageBase (63..32 is zero, ignore)
    mov     eax,[ebp+028h]              ; eax = [[22000 + [22014] + 3c] + 24] = EntryPoint
    add     eax,edi                     ; eax = ImageBase + EntryPoint
    push    eax                         ; save entry address

    mov     bx,word ptr[ebp+6]          ; bx = Number of sections
    xor     eax,eax
    mov     ax,word ptr[ebp+014h]       ; ax = Optional Header Size
    add     ebp,eax
    add     ebp,018h                    ; ebp = Start of 1st Section

SectionLoop:
    push    esi                         ; Save Base of EFILDR.C
    push    edi                         ; Save ImageBase
    add     esi,[ebp+014h]              ; esi = Base of EFILDR.C + PointerToRawData
    add     edi,[ebp+00ch]              ; edi = ImageBase + VirtualAddress
    mov     ecx,[ebp+010h]              ; ecs = SizeOfRawData

    cld
    shr     ecx,2
    rep     movsd

    pop     edi                         ; Restore ImageBase
    pop     esi                         ; Restore Base of EFILDR.C

    add     bp,028h                     ; ebp = ebp + 028h = Pointer to next section record
    sub     ebx,1
    cmp     bx,0
    jne     SectionLoop

    mov     edx, offset Idtr
    movzx   eax, word ptr [edx]          ; size of IDT (-1)
    add     eax,1                        ; size of IDT
    add     eax, dword ptr [edx + 2]     ; add to base of IDT to get location of memory map...
    mov     ecx, [eax]                   ; read start of E820 buffer
    cmp     ecx, 0FFFFFFFFh              ; 0FFFFFFFFh: use coreboot table in place of E820 buffer
    mov     ecx, corebootMemMap          ; pass mem map derived from coreboot table
    je      coreboot                     ; jump if running as coreboot payload
    mov     ecx, eax                     ; put argument to RCX
    sub     ecx, IMAGE_BUFFER_HIGH       ; fix up memory map address (it is below 1MB)
    add     ecx, IMAGE_BUFFER_LOW 
coreboot:
    ret                                  ; jump to entry address pushed by code above

;    db      "**** DEFAULT IDT ENTRY ***",0
    align 02h
Halt:
INT0:
    push    0h      ; push error code place holder on the stack
    push    0h
    JmpCommonIdtEntry
;    db      0e9h                        ; jmp 16 bit reletive 
;    dd      commonIdtEntry - $ - 4      ;  offset to jump to
    
INT1:
    push    0h      ; push error code place holder on the stack
    push    1h
    JmpCommonIdtEntry
    
INT2:
    push    0h      ; push error code place holder on the stack
    push    2h
    JmpCommonIdtEntry
    
INT3:
    push    0h      ; push error code place holder on the stack
    push    3h
    JmpCommonIdtEntry
    
INT4:
    push    0h      ; push error code place holder on the stack
    push    4h
    JmpCommonIdtEntry
    
INT5:
    push    0h      ; push error code place holder on the stack
    push    5h
    JmpCommonIdtEntry
    
INT6:
    push    0h      ; push error code place holder on the stack
    push    6h
    JmpCommonIdtEntry
    
INT7:
    push    0h      ; push error code place holder on the stack
    push    7h
    JmpCommonIdtEntry
    
INT8:
;   Double fault causes an error code to be pushed so no phony push necessary
    nop
    nop
    push    8h
    JmpCommonIdtEntry
    
INT9:
    push    0h      ; push error code place holder on the stack
    push    9h
    JmpCommonIdtEntry
    
INT10:
;   Invalid TSS causes an error code to be pushed so no phony push necessary
    nop
    nop
    push    10
    JmpCommonIdtEntry
    
INT11:
;   Segment Not Present causes an error code to be pushed so no phony push necessary
    nop
    nop
    push    11
    JmpCommonIdtEntry
    
INT12:
;   Stack fault causes an error code to be pushed so no phony push necessary
    nop
    nop
    push    12
    JmpCommonIdtEntry
    
INT13:
;   GP fault causes an error code to be pushed so no phony push necessary
    nop
    nop
    push    13
    JmpCommonIdtEntry
    
INT14:
;   Page fault causes an error code to be pushed so no phony push necessary
    nop
    nop
    push    14
    JmpCommonIdtEntry
    
INT15:
    push    0h      ; push error code place holder on the stack
    push    15
    JmpCommonIdtEntry
    
INT16:
    push    0h      ; push error code place holder on the stack
    push    16
    JmpCommonIdtEntry
    
INT17:
;   Alignment check causes an error code to be pushed so no phony push necessary
    nop
    nop
    push    17
    JmpCommonIdtEntry
    
INT18:
    push    0h      ; push error code place holder on the stack
    push    18
    JmpCommonIdtEntry
    
INT19:
    push    0h      ; push error code place holder on the stack
    push    19
    JmpCommonIdtEntry

INTUnknown:
REPEAT  (78h - 20)
    push    0h      ; push error code place holder on the stack
;    push    xxh     ; push vector number
    db      06ah
    db      ( $ - INTUnknown - 3 ) / 9 + 20 ; vector number
    JmpCommonIdtEntry
ENDM

commonIdtEntry:
    push    eax
    push    ecx
    push    edx
    push    ebx
    push    esp
    push    ebp
    push    esi
    push    edi
    db 41h
    db 50h
;    push    r8
    db 41h
    db 51h
;    push    r9
    db 41h
    db 52h
;    push    r10
    db 41h
    db 53h
;    push    r11
    db 41h
    db 54h
;    push    r12
    db 41h
    db 55h
;    push    r13
    db 41h
    db 56h
;    push    r14
    db 41h
    db 57h
;    push    r15
    db 48h
    mov     ebp, esp
;    mov     rbp, rsp

;;
;;  At this point the stack looks like this:
;;
;;      Calling SS
;;      Calling RSP
;;      rflags
;;      Calling CS
;;      Calling RIP
;;      Error code or 0
;;      Int num or 0ffh for unknown int num
;;      rax
;;      rcx
;;      rdx
;;      rbx
;;      rsp
;;      rbp
;;      rsi
;;      rdi
;;      r8
;;      r9
;;      r10
;;      r11
;;      r12
;;      r13
;;      r14
;;      r15 <------- RSP, RBP
;;      

    call    ClearScreen
    mov     esi, offset String1
    call    PrintString
    db 48h
    mov     eax, [ebp + 16*8]     ;; move Int number into RAX 
    db 48h
    cmp     eax, 18
    ja      PrintDefaultString
PrintExceptionString:
    shl     eax, 3              ;; multiply by 8 to get offset from StringTable to actual string address
    add     eax, offset StringTable
    mov     esi, [eax]
    jmp     PrintTheString
PrintDefaultString:
    mov     esi, offset IntUnknownString
    ; patch Int number
    mov     edx, eax
    call    A2C
    mov     [esi + 1], al
    mov     eax, edx
    shr     eax, 4
    call    A2C
    mov     [esi], al
PrintTheString:        
    call    PrintString
    mov     esi, offset String2
    call    PrintString
    db 48h
    mov     eax, [ebp+19*8]    ; CS
    call    PrintQword
    mov     al, ':'
    mov     byte ptr [edi], al
    add     edi, 2
    db 48h
    mov     eax, [ebp+18*8]    ; RIP
    call    PrintQword
    mov     esi, offset String3
    call    PrintString
    
    mov     edi, 0b8140h
    
    mov     esi, offset StringRax     ; rax
    call    PrintString
    db 48h
    mov     eax, [ebp+15*8]
    call    PrintQword
   
    mov     esi, offset StringRcx     ; rcx
    call    PrintString
    db 48h
    mov     eax, [ebp+14*8]
    call    PrintQword
    
    mov     esi, offset StringRdx     ; rdx
    call    PrintString
    db 48h
    mov     eax, [ebp+13*8]
    call    PrintQword
    
    mov     edi, 0b81e0h
    
    mov     esi, offset StringRbx     ; rbx
    call    PrintString
    db 48h
    mov     eax, [ebp+12*8]
    call    PrintQword
     
    mov     esi, offset StringRsp     ; rsp
    call    PrintString
    db 48h
    mov     eax, [ebp+21*8]
    call    PrintQword
    
    mov     esi, offset StringRbp     ; rbp
    call    PrintString
    db 48h
    mov     eax, [ebp+10*8]
    call    PrintQword
    
    mov     edi, 0b8280h
     
    mov     esi, offset StringRsi     ; rsi
    call    PrintString
    db 48h
    mov     eax, [ebp+9*8]
    call    PrintQword
    
    mov     esi, offset StringRdi     ; rdi
    call    PrintString
    db 48h
    mov     eax, [ebp+8*8]
    call    PrintQword
    
    mov     esi, offset StringEcode   ; error code
    call    PrintString
    db 48h
    mov     eax, [ebp+17*8]
    call    PrintQword
    
    mov     edi, 0b8320h
 
    mov     esi, offset StringR8      ; r8
    call    PrintString
    db 48h
    mov     eax, [ebp+7*8]
    call    PrintQword

    mov     esi, offset StringR9      ; r9
    call    PrintString
    db 48h
    mov     eax, [ebp+6*8]
    call    PrintQword

    mov     esi, offset StringR10     ; r10
    call    PrintString
    db 48h
    mov     eax, [ebp+5*8]
    call    PrintQword

    mov     edi, 0b83c0h

    mov     esi, offset StringR11     ; r11
    call    PrintString
    db 48h
    mov     eax, [ebp+4*8]
    call    PrintQword

    mov     esi, offset StringR12     ; r12
    call    PrintString
    db 48h
    mov     eax, [ebp+3*8]
    call    PrintQword

    mov     esi, offset StringR13     ; r13
    call    PrintString
    db 48h
    mov     eax, [ebp+2*8]
    call    PrintQword
 
    mov     edi, 0b8460h

    mov     esi, offset StringR14     ; r14
    call    PrintString
    db 48h
    mov     eax, [ebp+1*8]
    call    PrintQword
 
    mov     esi, offset StringR15     ; r15
    call    PrintString
    db 48h
    mov     eax, [ebp+0*8]
    call    PrintQword

    mov     esi, offset StringSs      ; ss
    call    PrintString
    db 48h
    mov     eax, [ebp+22*8]
    call    PrintQword
  
    mov     edi, 0b8500h

    mov     esi, offset StringRflags  ; rflags
    call    PrintString
    db 48h
    mov     eax, [ebp+20*8]
    call    PrintQword
    
    mov     edi, 0b8640h

    mov     esi, ebp
    add     esi, 23*8
    mov     ecx, 4

    
OuterLoop:
    push    ecx
    mov     ecx, 4
    db 48h
    mov     edx, edi

InnerLoop:
    db 48h
    mov     eax, [esi]
    call    PrintQword
    add     esi, 8
    mov     al, ' '
    mov     [edi], al
    add     edi, 2
    loop    InnerLoop

    pop     ecx
    add     edx, 0a0h
    mov     edi, edx
    loop    OuterLoop


    mov     edi, 0b8960h

    db 48h
    mov     eax, [ebp+18*8]  ; RIP
    sub     eax, 8 * 8
    db 48h
    mov     esi, eax        ; esi = rip - 8 QWORD linear (total 16 QWORD)

    mov     ecx, 4
    
OuterLoop1:
    push    ecx
    mov     ecx, 4
    mov     edx, edi

InnerLoop1:
    db 48h
    mov     eax, [esi]
    call    PrintQword
    add     esi, 8
    mov     al, ' '
    mov     [edi], al
    add     edi, 2
    loop    InnerLoop1

    pop     ecx
    add     edx, 0a0h
    mov     edi, edx
    loop    OuterLoop1



    ;wbinvd
@@:    
    jmp     @b

;
; return
;
    mov     esp, ebp
;    mov     rsp, rbp
    db 41h
    db 5fh
;    pop    r15
    db 41h
    db 5eh
;    pop    r14
    db 41h
    db 5dh
;    pop    r13
    db 41h
    db 5ch
;    pop    r12
    db 41h
    db 5bh
;    pop    r11
    db 41h
    db 5ah
;    pop    r10
    db 41h
    db 59h
;    pop    r9
    db 41h
    db 58h
;    pop    r8
    pop    edi
    pop    esi
    pop    ebp
    pop    eax ; esp
    pop    ebx
    pop    edx
    pop    ecx
    pop    eax
 
    db 48h
    db 83h
    db 0c4h
    db 10h   
;    add    esp, 16 ; error code and INT number

    db 48h
    db 0cfh
;    iretq

PrintString:
    push    eax
@@:
    mov     al, byte ptr [esi]
    cmp     al, 0
    je      @f
    mov     byte ptr [edi], al
    db 0ffh
    db 0c6h
;    inc     esi
    add     edi, 2
    jmp     @b
@@:
    pop     eax
    ret
        
;; RAX contains qword to print
;; RDI contains memory location (screen location) to print it to
PrintQword:
    push    ecx
    push    ebx
    push    eax
    
    db 48h
    db 0c7h
    db 0c1h
    dd 16
;    mov     rcx, 16
looptop:
    db 48h
    rol     eax, 4
    mov     bl, al
    and     bl, 0fh
    add     bl, '0'
    cmp     bl, '9'
    jle     @f
    add     bl, 7
@@:
    mov     byte ptr [edi], bl
    add     edi, 2
    loop    looptop
    ;wbinvd
    
    pop     eax
    pop     ebx
    pop     ecx
    ret

ClearScreen:
    push    eax
    push    ecx
    
    mov     al, ' '
    mov     ah, 0ch
    mov     edi, 0b8000h
    mov     ecx, 80 * 24
@@:
    mov     word ptr [edi], ax
    add     edi, 2
    loop    @b
    mov     edi, 0b8000h
    
    pop     ecx
    pop     eax

    ret                
        
A2C:
    and     al, 0fh
    add     al, '0'
    cmp     al, '9'
    jle     @f
    add     al, 7
@@:
    ret
        
String1           db  "*** INT ",0

Int0String        db  "00h Divide by 0 -",0
Int1String        db  "01h Debug exception -",0
Int2String        db  "02h NMI -",0
Int3String        db  "03h Breakpoint -",0
Int4String        db  "04h Overflow -",0
Int5String        db  "05h Bound -",0
Int6String        db  "06h Invalid opcode -",0
Int7String        db  "07h Device not available -",0
Int8String        db  "08h Double fault -",0
Int9String        db  "09h Coprocessor seg overrun (reserved) -",0
Int10String       db  "0Ah Invalid TSS -",0
Int11String       db  "0Bh Segment not present -",0
Int12String       db  "0Ch Stack fault -",0
Int13String       db  "0Dh General protection fault -",0
Int14String       db  "0Eh Page fault -",0
Int15String       db  "0Fh (Intel reserved) -",0
Int16String       db  "10h Floating point error -",0
Int17String       db  "11h Alignment check -",0
Int18String       db  "12h Machine check -",0
Int19String       db  "13h SIMD Floating-Point Exception -",0
IntUnknownString  db  "??h Unknown interrupt -",0

StringTable       dq  offset Int0String, offset Int1String, offset Int2String, offset Int3String, 
                      offset Int4String, offset Int5String, offset Int6String, offset Int7String,
                      offset Int8String, offset Int9String, offset Int10String, offset Int11String,
                      offset Int12String, offset Int13String, offset Int14String, offset Int15String,
                      offset Int16String, offset Int17String, offset Int18String, offset Int19String

String2           db  " HALT!! *** (",0
String3           db  ")",0
StringRax         db  "RAX=",0
StringRcx         db  " RCX=",0
StringRdx         db  " RDX=",0
StringRbx         db  "RBX=",0
StringRsp         db  " RSP=",0
StringRbp         db  " RBP=",0
StringRsi         db  "RSI=",0
StringRdi         db  " RDI=",0
StringEcode       db  " ECODE=",0
StringR8          db  "R8 =",0
StringR9          db  " R9 =",0
StringR10         db  " R10=",0
StringR11         db  "R11=",0
StringR12         db  " R12=",0
StringR13         db  " R13=",0
StringR14         db  "R14=",0
StringR15         db  " R15=",0
StringSs          db  " SS =",0
StringRflags      db  "RFLAGS=",0

;----------------------------------------------------------------------------
readEip proc near
   mov      eax, [esp]
   ret
readEip endp
;----------------------------------------------------------------------------
; identityMapLongMode - build identity mapped page tables for use in long mode
;
; input  : edi - starting address for page tables
; output : edi - next available address
;
mappingSize equ 0FFFFFFFFh

count4      equ ((mappingSize - 1) SHR 39) + 1
count3      equ ((mappingSize - 1) SHR 30) + 1
count2      equ ((mappingSize - 1) SHR 21) + 1

pageMapLevel4           STRUC
lowPart  dd ?
highPart dd ?
pageMapLevel4           ENDS

pageDirectoryPointer    STRUC
lowPart  dd ?
highPart dd ?
pageDirectoryPointer    ENDS

pageDirectory           STRUC
lowPart  dd ?
highPart dd ?
pageDirectory           ENDS

tableStruct             STRUC
    align               4096
    map                 pageMapLevel4           count4 dup (<>)
    align               4096
    pointer             pageDirectoryPointer    count3 dup (<>)
    align               4096
    directory           pageDirectory           count2 dup (<>)
tableStruct             ENDS

identityMapLongMode proc near private
local index:dword
        mov     ecx, sizeof tableStruct
        push    edi
        xor     al, al  
        rep     stosb                                   ; clear the page table memory
        pop     edi

        mov     ebx, edi                                ; ebx = table base physical address
        mov     index, 0
        .while (index < count4)
            lea   eax, [ebx].tableStruct.pointer
            shr   eax, 12
            add   eax, index
            shl   eax, 12
            or    al, 3                                 ; present, writable
            or    (tableStruct ptr [edi]).map.lowPart, eax
            inc   index
            add   edi, sizeof pageMapLevel4
        .endw

        mov     index, 0
        mov     edi, ebx
        .while (index < count3)
            lea   eax, [ebx].tableStruct.directory
            shr   eax, 12
            add   eax, index
            shl   eax, 12
            or    al, 3                                 ; present, writable
            or    (tableStruct ptr [edi]).pointer.lowPart, eax
            inc   index
            add   edi, sizeof pageDirectoryPointer
        .endw

        mov     index, 0
        mov     edi, ebx
        .while (index < count2)
            mov   eax, index
            shl   eax, 21
            or    al, 83h                               ; present, writable, big
            or    (tableStruct ptr [edi]).directory.lowPart, eax
            inc   index
            add   edi, sizeof pageDirectory
        .endw
        lea     edi, [ebx + size tableStruct]           ; return next free address
        ret
identityMapLongMode endp

;----------------------------------------------------------------------------
; setupRmVectors - Replace coreboot real mode vectors with debug hooks
;                  to make it apparent when one executes.
;
; input  : edi - starting address below 1MB for real mode ISR code
; output : edi - next available address

setupRmVectors proc near private
    xor    ecx, ecx                     ; INT number: start with zero
vecloop:
    lea    ebx, [ecx * 4]               ; vector address from INT number
    mov    eax, edi                     ; physical address of ISR code
    shr    eax, 16                      ; segment value
    shl    eax, 12
    mov    si, [ebx + 2]                ; check existing vector
    cmp    si, 0                        ; replace only empty vectors
    jne    skipit
    mov    [ebx + 2], ax
    mov    [ebx + 0], di

    ; INT8: 8254 periodic interrupt
    mov    esi, text32start + text32size + (PicHandlerStart - text16start)
    mov    ebx, PicHandlerEnd - PicHandlerStart
    cmp    cl, 8+0                      ; master PIC INT 0, 8254 periodic
    je     installHandler

    ; INT1 and INT7 (PS/2 KB and spurious): ignore
    mov    esi, text32start + text32size + (nopHandlerStart - text16start)
    mov    ebx, nopHandlerEnd - nopHandlerStart
    cmp    cl, 8+1                      ; master PIC INT 1, PS/2 keyboard
    je     installHandler
    cmp    cl, 8+7                      ; master PIC INT 7, spurious
    je     installHandler
   
    ; use debug handler for all others
    mov    esi, text32start + text32size + (debugHandlerStart - text16start)
    mov    ebx, debugHandlerEnd - debugHandlerStart
    mov    [esi + (intno-debugHandlerStart)], cl ; patch in interrupt number

    ; now copy the ISR code to address below 1MB
installHandler:
    push   ecx
    mov    ecx, ebx
    rep    movsb
    pop    ecx

skipit:
    add    ecx, 1
    cmp    ecx, 256
    jne    vecloop

    ; DuetPkg/BiosVideoThunkDxe will call INT 15h if it finds a vector.
    ; Clear the vector to tell BiosVideoThunkDxe that INT 15h is not
    ; available on this system.
    mov    esi, 15h * 4
    mov    dword ptr [esi], 0
    ret
setupRmVectors endp

;-----------------------------------------------------------------------------

installVector32f proc
        mov     eax, ebx                     ; physical address of ISR code
        shr     ebx, 16                      ; segment value
        shl     ebx, 12
        mov     [esi + 2], bx
        mov     [esi + 0], ax
        ret
installVector32f endp

installVector32 macro id
        mov     esi, id * 4
        lea     ebx, [ebp + handler&id - debuggerStart]
        call    installVector32f
        endm

;----------------------------------------------------------------------------
; setupDebug - Replace some real mode vectors with debug hooks
;
; input  : edi - starting address below 1MB for real mode ISR code
; output : edi - next available address

setupDebug proc public
        IF MKF_DEBUGGER_SUPPORT
        push    ebp
        push    esi
        push    ecx
        push    ebx
        push    eax

        ; save address where debugger is copied to
        mov     ebp, edi

        ; copy real mode code to below 1MB
        mov     ecx, debuggerEnd - debuggerStart
        mov     esi, text32start + text32size + (debuggerStart - text16start)
        rep     movsb
        
        installVector32 0
        installVector32 1
        installVector32 2
        installVector32 3
        installVector32 4
        installVector32 5
        installVector32 6
        
        mov     eax, cr4
        bts     eax, 3     ; enable debug extensions
        mov     cr4, eax
        pop     eax
        pop     ebx
        pop     ecx
        pop     esi
        pop     ebp
        ENDIF
        ret
setupDebug endp

;----------------------------------------------------------------------------

align 8
dataStart:

gdt64:
    dd    0
    dd    0
gdt64_code:
    dw    0
    dw    0
    db    0
    db    98h
    db    20h
    db    0
gdt64_data:
    dw    0
    dw    0
    db    0
    db    90h
    db    0
    db    0
gdt64end:

align 16
gdtr:
    dw    gdt64end - gdt64 - 1; length
    dq    gdt64

align 16
idtr:
    dw    idt64end - idt64 - 1; length
    dq    idt64

align 16
idt64:
    dq    78h dup (0,0) 
idt64end:

biosMemoryMapEntry STRUC
baseAddress     dq ?
byteCount       dq ?
memType         dd ?
biosMemoryMapEntry ENDS

align 16
e820map:
    dd    0FFFFFFFFh ; SeaBIOS not present: E820 data not available

align 16
corebootMemMap:
    dd  corebootMemMapEnd-corebootMemMapStart     ; maximum size
corebootMemMapStart:
    biosMemoryMapEntry 50 dup (<0, 0, 0FFFFFFFFh>)
corebootMemMapEnd:

;----------------------------------------------------------------------------
align 8
text32end:
_TEXT32 ends
_TEXT16 SEGMENT dword USE16
text16start:

debugHandlerStart:
        db     0b0h         ; "mov al, hh" opcode byte 1 of 2
intno   db     0            ; "mov al, hh" opcode byte 1 of 2: interrupt number
        mov    dx, 3F8h     ; com1 base address
        out    dx, al
        jmp    $            ; if hang here, above reg AL has INT number 
        iret
debugHandlerEnd:

;----------------------------------------------------------------------------

nopHandlerStart:
    push   ax
    mov    al, 20h          ; send end of interrupt to 8259
    out    20h, al
    pop    ax
    iret
nopHandlerEnd:
;----------------------------------------------------------------------------

timerIntCount dd 0
PicHandlerStart:
    push   ax
    push   ds
    xor    ax, ax
    mov    ds, ax
    add    cs:timerIntCount, 59659   ; convert 100 Hz UEFI to 18.2 Hz DOS
    cmp    cs:timerIntCount, 327675
    jb     noDosUpdate
    inc    dword ptr ds:[46Ch]
    sub    cs:timerIntCount, 327675
noDosUpdate:
    mov    al, 20h          ; send end of interrupt to 8259
    out    20h, al
    pop    ds
    pop    ax
    iret
PicHandlerEnd:
;=============================================================================
IF MKF_DEBUGGER_SUPPORT
debuggerStart:
;----------------------------------------------------------------------------
;
; change these as needed for debugging
;

port                    equ     3F8h
baud                    equ     115200

;-----------------------------------------------------------------------------
; a message from the Windows system always starts with 01, and is followed by a command code and optional data
; a response from the target system starts with 81. The second byte is the command code copied from the request.
; On exception, the target system sends a message starting with 82
;-----------------------------------------------------------------------------

   _RD_LOCALDEBUG          = 01h
   _RD_READ_CSEIP          = 02h
   _RD_READ_FLAT_EIP       = 03h
   _RD_READ_FLAT_ESP       = 04h
   _RD_RUNTO_ADDRESS       = 05h
   _RD_RUN                 = 06h
   _RD_READ_MEMORY_BYTE    = 07h
   _RD_READ_MEMORY_WORD    = 08h
   _RD_READ_MEMORY_DWORD   = 09h
   _RD_READ_MEMORY_QWORD   = 0Ah
   _RD_WRITE_MEMORY_BYTE   = 0Bh
   _RD_WRITE_MEMORY_WORD   = 0Ch
   _RD_WRITE_MEMORY_DWORD  = 0Dh
   _RD_WRITE_MEMORY_QWORD  = 0Eh
   _RD_BACKUP_EIP          = 0Fh
   _RD_READ_EXEBASE        = 10h
   _RD_TRACE               = 11h
   _RD_READ_EXCEPTION      = 12h
   _RD_READ_MAIN_REGS      = 13h
   _RD_WRITE_MAIN_REGS     = 14h
   _RD_READ_MISC_REGS      = 15h
   _RD_READ_MMX_REGS       = 16h
   _RD_READ_XMM_REGS       = 17h
   _RD_READ_CPUID          = 18h
   _RD_EXIT_TO_DOS         = 19h
   _RD_READ_MSR            = 1Ah
   _RD_READ_IO_BYTE        = 1Bh
   _RD_READ_IO_WORD        = 1Ch
   _RD_READ_IO_DWORD       = 1Dh
   _RD_WRITE_IO_BYTE       = 1Eh
   _RD_WRITE_IO_WORD       = 1Fh
   _RD_WRITE_IO_DWORD      = 20h
   _RD_READ_PCI_CONFIG     = 21h
   _RD_WRITE_PCI_CONFIG    = 22h
   _RD_PCI_BUS_CHECK       = 23h
   _RD_READ_DEBUG_REGS     = 24h
   _RD_WRITE_DEBUG_REGS    = 25h
   _RD_TEST_WRITABLE       = 26h
   _RD_READ_CMOS           = 27h
   _RD_WRITE_CMOS          = 28h
   _RD_MEM_SEARCH          = 29h
   _RD_MEM_FILL            = 2Ah
   _RD_MEM_SUM             = 2Bh

;-----------------------------------------------------------------------------
; these registers are saved and restored by the debugger

cpuregs struct
xnumber word    0       ; the order of these must match the C code
gsreg   word    0
fsreg   word    0
ssreg   word    0
esreg   word    0
dsreg   word    0
ebpreg  dword   0
espreg  dword   0
edireg  dword   0
esireg  dword   0
edxreg  dword   0
ecxreg  dword   0
ebxreg  dword   0
eaxreg  dword   0

ipreg   word    0       ; do not re-order these 3, the cpu pushes them this way
csreg   word    0
flags   word    0
cpuregs ends

;-----------------------------------------------------------------------------
; layout used when reading additional processor registers
miscregs struct
cr0reg  dword   0
cr2reg  dword   0
cr3reg  dword   0
cr4reg  dword   0
cr8reg  dword   0
ldtr    dword   0
tr      dword   0
idtinfo byte    12 dup (0)
gdtinfo byte    12 dup (0)
ldtinfo byte    12 dup (0)
miscregs ends

;-----------------------------------------------------------------------------
; layout used when reading debug registers
debugregs struct
dr0reg  dword   0
dr1reg  dword   0
dr2reg  dword   0
dr3reg  dword   0
dr6reg  dword   0
dr7reg  dword   0
debugregs ends

;-----------------------------------------------------------------------------
; layout used when reading mmx/xmm processor registers
mmxregs struct
fxsave1 byte 512 dup (0)
mmxregs ends

;-----------------------------------------------------------------------------
; txbyte - send byte in al out the serial port
;          no registers are modified

txbyte  proc near private
        push    ax
        push    dx
        mov     dx, port
        out     dx, al
        add     dx, 5
txbyte1:
        in      al, dx
        and     al, 20h
        je      txbyte1
        pop     dx
        pop     ax
        ret
txbyte endp

;-----------------------------------------------------------------------------
; txword - send byte in ax out the serial port
;          no registers are modified

txword proc near private
        push    ax
        call    txbyte
        mov     al, ah
        call    txbyte
        pop     ax
        ret
txword endp

;-----------------------------------------------------------------------------
; txdword - send dword in eax out the serial port
;           no registers are modified

txdword proc near private
        call    txword
        ror     eax, 16
        call    txword
        ror     eax, 16
        ret
txdword endp

;-----------------------------------------------------------------------------
; rxbyte - read com port data into al
;          no other registers are modified
;
rxbyte proc near private
        push    dx
        mov     dx, port+5
rxbyte1:
        in      al, dx
        and     al, 1
        je      rxbyte1
        mov     dx, port
        in      al, dx
        pop     dx
        ret
rxbyte endp

;-----------------------------------------------------------------------------
; rxword - read com port data into ax
;          no other registers are modified
;
rxword  proc    near    private
        call    rxbyte
        xchg    al, ah
        call    rxbyte
        xchg    al, ah
        ret
rxword  endp

;-----------------------------------------------------------------------------
; rxdword - read com port data into eax
;           no other registers are modified
;
rxdword proc    near    private
        call    rxword
        ror     eax, 16
        call    rxword
        ror     eax, 16
        ret
rxdword endp

;-----------------------------------------------------------------------------
; txbuffer - send cx bytes of data at es:di to com port
;            no registers are modified

txbuffer proc    near    private
        push    ax
        push    cx
txbuffer1:
        sub     cx, 1
        jc      txbufferReturn
        mov     al, es:[di]
        inc     di
        call    txbyte
        jmp     txbuffer1
txbufferReturn:
        pop     cx
        pop     ax
        ret
txbuffer endp

;-----------------------------------------------------------------------------
; rxbuffer - receive cx bytes of data from com port and put in buffer at es:di
;            no registers are modified

rxbuffer proc near private
        push    ax
        push    cx
rxbuffer1:
        sub     cx, 1
        jc      rxbufferReturn
        call    rxbyte
        mov     es:[di], al
        inc     di
        jmp     rxbuffer1
rxbufferReturn:
        pop     cx
        pop     ax
        ret
rxbuffer endp

;-----------------------------------------------------------------------------
; remoteDebugReply - reply to command from Windows app
;                    bl    command code to echo
;                    cx    buffer size
;                    es:di buffer address
;                    no registers are modified

remoteDebugReply proc near private
        push    ax
        mov     al, 81h         ; start of response byte
        call    txbyte          ; send it
        mov     al, bl          ; command code to echo
        call    txbyte          ; echo command code
        call    txbuffer        ; send the additional data
        pop     ax
        ret
remoteDebugReply endp

;-----------------------------------------------------------------------------
; remoteDebugReportEvent - transmit a message on entry to exception handler
;                          ebp points to registers
;                          no registers are modified

remoteDebugReportEvent proc near private
        push    eax
        push    ebx
        mov     al, 82h                         ; flag start of remote message
        call    txbyte
        mov     ax, [ebp].cpuregs.xnumber
        call    txbyte                          ; send exception number
        mov     ax, 0FFh
        call    txbyte                          ; no error code in real mode
        mov     ax, [ebp].cpuregs.ipreg
        call    txword                          ; send ip
        movzx   eax, [ebp].cpuregs.csreg
        call    txword                          ; send cs
        movzx   ebx, [ebp].cpuregs.ipreg
        shl     eax, 4
        add     eax, ebx
        call    txdword
        movzx   eax, [ebp].cpuregs.csreg
        shl     eax, 4
        call    txdword
        pop     ebx
        pop     eax
        ret
remoteDebugReportEvent endp

;-----------------------------------------------------------------------------
; structures to access temporary gdt on stack

DESCRIPTOR      struct
lo      dword   0
hi      dword   0
DESCRIPTOR      ends

FWORDMEM        struct
limit    word   0
physical dword  0
FWORDMEM        ends

gdtmem  struct
desc0   DESCRIPTOR <>
desc1   DESCRIPTOR <>
address FWORDMEM   <>
gdtmem  ends

gdtlimit        equ     offset gdtmem.address - offset gdtmem.desc0 - 1
;-----------------------------------------------------------------------------
;
; bigReal - set ds=0, es=0 with big real mode enabled
;           no other registers are modified

bigReal proc near private
        push eax
        push ebx
        push ecx

        cld                         ; set rep to go in normal (forward) direction
        cli                         ; Interrupts clear throughout

        mov     al, 2               ; turn on fast a20 so odd MBs address OK
        out     92h, al
        ;
        ; build a GDT on the stack
        ;
        mov     ecx, esp
        sub     sp, size gdtmem + 63    ; align
        and     sp, (1000h-64)
        mov     [esp].gdtmem.desc0.lo, 0
        mov     [esp].gdtmem.desc0.hi, 0
        mov     [esp].gdtmem.desc1.lo, 0000FFFFh
        mov     [esp].gdtmem.desc1.hi, 00CF9200h

        xor     ebx, ebx
        mov     bx, ss
        mov     ds, bx
        shl     ebx, 4
        add     ebx, esp                                ; ebx is physical address of gdt on stack
        mov     [esp].gdtmem.address.physical,ebx       ; fill in offset part of fword
        mov     [esp].gdtmem.address.limit,gdtlimit     ; fill in limit part of fword

        ; Load the GDT descriptor
        lgdt    [esp].gdtmem.address

        ;
        ; Switch to protected mode.
        ;
        mov ebx,CR0     ; Set protected mode
        inc bx
        mov CR0,ebx

        jmp short to_pm_flush ; this is needed for older processors like the 80386
        to_pm_flush:
        ;
        ; Load es & ds with big segs
        ;
        mov ax, offset gdtmem.desc1 - offset gdtmem.desc0 ; 8, the first usable selector
        mov ds, ax
        mov es, ax
        ;
        ; Switch back to real mode
        ;
        dec bx
        mov CR0,ebx

        ; now put zero in the segment registers so that they will be valid if saved and restored
        xor  ax, ax
        mov  ds, ax
        mov  es, ax

        mov  esp, ecx
        pop  ecx
        pop  ebx
        pop  eax
        cld
        ret
bigReal endp

;-----------------------------------------------------------------------------
; checkBus - returns carry if any pci config space is present for the given bus
;            this operation is done here because it is slow to do over the serial link
;            no registers are modified

checkBus proc near private
        push    eax
        push    ebx
        push    ecx
        push    edx
        push    esi

        mov     dx, 0CF8h

        ; build initial CF8 value
        movzx   eax, al         ; eax is bus number
        or      ax, 8000h
        shl     eax, 16
        mov     esi, eax        ; esi is initial CF8 value
        xor     ebx, ebx        ; ebx is function number
        xor     ecx, ecx        ; ecx is device number
checkNextDevice:
        mov     eax, esi        ; eax is initial CF8 value
        or      eax, ecx        ; combine device number
        or      eax, ebx        ; combine function number
        out     dx, eax
        add     dx, 4
        in      eax, dx
        sub     dx, 4
        inc     eax             ; no config FFFFFFFF decrements to zero
        jnz     busIsPresent

        add     ebx, 1 shl 8    ; next function number
        cmp     ebx, 8 shl 8    ; 0-7 complete?
        jne     checkNextDevice
        xor     ebx, ebx        ; reset function number
        add     ecx, 1 shl 11   ; next device number
        cmp     ecx, 32 shl 11  ; 0-31 complete?     
        jne     checkNextDevice ; continue, or fall through to no bus found return
noBusPresent:
        clc
        jmp     checkBusReturn
busIsPresent:
        stc
checkBusReturn:
        pop     esi
        pop     edx
        pop     ecx
        pop     ebx
        pop     eax
        ret
checkBus endp

;-----------------------------------------------------------------------------
; readCmos - read al from cmos offset ah (00-FF)
;            no other registers are modified

readCmos proc near private
        push    bx
        mov     bx, ax        ; save ah
        btr     ax, 15        ; see if offset is in high bank
        xchg    al, ah        ; al is index into bank
        jc      highBank
        out     70h, al
        xchg    al, ah
        in      al, 71h
        jmp     readCmosDone
highBank:
        out     72h, al
        xchg    al, ah
        in      al, 73h
readCmosDone:
        mov     ah, bh        ; restore ah
        pop     bx
        ret
readCmos endp
              
;-----------------------------------------------------------------------------
; writeCmos - write al to cmos offset ah (00-FF)
;             no registers are modified

writeCmos proc near private
        push    ax
        btr     ax, 15
        xchg    al, ah        
        jc      highBank
        out     70h, al
        xchg    al, ah
        out     71h, al
        jmp     writeCmosDone
highBank:
        out     72h, al
        xchg    al, ah
        out     73h, al
writeCmosDone:
        pop     ax
        ret
writeCmos endp
              
;-----------------------------------------------------------------------------
; remoteDebug - exception handler interface to remote debugger
;               ebp points to main registers
;               all other registers are modified
;-----------------------------------------------------------------------------

remoteDebug proc near private

remoteDebugLoop:
        mov     ax, ss
        mov     es, ax                          ; es is stack segment, where registers are kept

        call    rxbyte                          ; sync up to the (possible) start of message
        cmp     al, 1
        jne     remoteDebugLoop

        call    rxbyte                          ; al is command code
        mov     bl, al                          ; bl is command code
        xor     cx, cx                          ; default amount of additional response data - zero bytes
        ;---------------------------------------
        cmp     bl, _RD_RUN
        jne     @f
        call    remoteDebugReply
        jmp     remoteDebugReturn
@@:
        ;---------------------------------------
        cmp     bl, _RD_TRACE
        jne     @f
        call    remoteDebugReply
        or      [ebp].cpuregs.flags, 1 shl 8
        jmp     remoteDebugReturn
@@:
        ;---------------------------------------
        cmp     bl, _RD_READ_CSEIP
        jne     @f
        mov     cx, 4                           ; size cpuregs.ipreg + size cpuregs.csreg
        lea     di, [ebp].cpuregs.ipreg
        call    remoteDebugReply
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_EXIT_TO_DOS
        jne     @f
        call    remoteDebugReply
        xor     al, al
        out     92h, al
        mov     ax, 4C00h
        int     21h
@@:
        ;---------------------------------------
        cmp     bl, _RD_WRITE_MAIN_REGS
        jne     @f
        mov     di, bp                          ; address of registers
        mov     cx, sizeof cpuregs              ; size of registers
        call    rxbuffer                        ; accept new register data
        xor     cx, cx
        call    remoteDebugReply
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_READ_MAIN_REGS
        jne     @f
        mov     di, bp
        mov     cx, sizeof cpuregs
        call    remoteDebugReply
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_READ_MISC_REGS
        jne     @f
        sub     sp, size miscregs               ; allocate space for misc regs
        mov     eax, cr0
        mov     [esp].miscregs.cr0reg, eax
        mov     eax, cr2
        mov     [esp].miscregs.cr2reg, eax
        mov     eax, cr3
        mov     [esp].miscregs.cr3reg, eax
        mov     eax, cr4
        mov     [esp].miscregs.cr4reg, eax
        mov     di, sp
        mov     cx, sizeof miscregs
        call    remoteDebugReply
        add     sp, size miscregs               ; de-allocate space for misc regs
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_READ_DEBUG_REGS
        jne     @f
        sub     sp, size debugregs               ; allocate space for misc regs
        mov     eax, dr0
        mov     [esp].debugregs.dr0reg, eax
        mov     eax, dr1
        mov     [esp].debugregs.dr1reg, eax
        mov     eax, dr2
        mov     [esp].debugregs.dr2reg, eax
        mov     eax, dr3
        mov     [esp].debugregs.dr3reg, eax
        mov     eax, dr6
        mov     [esp].debugregs.dr6reg, eax
        mov     eax, dr7
        mov     [esp].debugregs.dr7reg, eax
        mov     di, sp
        mov     cx, sizeof debugregs
        call    remoteDebugReply
        add     sp, size debugregs              ; de-allocate space for misc regs
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_WRITE_DEBUG_REGS
        jne     @f
        sub     sp, size debugregs              ; allocate space for debug regs
        mov     di, sp
        mov     cx, sizeof debugregs            ; size of registers
        call    rxbuffer                        ; accept new register data
        mov     eax, [esp].debugregs.dr0reg
        mov     dr0, eax
        mov     eax, [esp].debugregs.dr1reg
        mov     dr1, eax
        mov     eax, [esp].debugregs.dr2reg
        mov     dr2, eax
        mov     eax, [esp].debugregs.dr3reg
        mov     dr3, eax
        mov     eax, [esp].debugregs.dr6reg
        mov     dr6, eax
        mov     eax, [esp].debugregs.dr7reg
        mov     dr7, eax
        add     sp, size debugregs              ; de-allocate space for debug regs
        xor     cx, cx
        call    remoteDebugReply
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_READ_MMX_REGS
        jne     @f
        sub     sp, size mmxregs                ; allocate space for mmx regs
        mov     di, sp
        mov     cx, sizeof mmxregs
        call    remoteDebugReply
        add     sp, size mmxregs                ; de-allocate space for mmx regs
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_READ_FLAT_EIP           ; return cs * 16 + ip
        jne     @f
        movzx   eax, [ebp].cpuregs.csreg
        movzx   ebx, [ebp].cpuregs.ipreg
        shl     eax, 4
        add     eax, ebx
        push    eax
        mov     cx, 4
        mov     di, sp
        call    remoteDebugReply
        add     sp, 4                
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_READ_FLAT_ESP           ; return ss * 16 + sp
        jne     @f
        movzx   eax, [ebp].cpuregs.ssreg
        mov     ecx, [ebp].cpuregs.espreg
        shl     eax, 4
        add     eax, ecx
        push    eax
        mov     cx, 4
        mov     di, sp
        call    remoteDebugReply
        add     sp, 4                
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_RUNTO_ADDRESS
        jne     @f
        call    rxdword
        mov     dr0, eax
        mov     eax, dr7
        and     eax, not 0F0000h        ; set breakpoint zero type to execution breakpoint
        or      eax, 303h               ; enable breakpoint zero
        mov     dr7, eax
        call    remoteDebugReply
        jmp     remoteDebugReturn
@@:
        ;---------------------------------------
        cmp     bl, _RD_BACKUP_EIP
        jne     @f
        dec     [ebp].cpuregs.ipreg
        call    remoteDebugReply
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_READ_MEMORY_BYTE
        je      readMemory
        cmp     bl, _RD_READ_MEMORY_WORD
        je      readMemory
        cmp     bl, _RD_READ_MEMORY_DWORD
        je      readMemory
        cmp     bl, _RD_READ_MEMORY_QWORD
        je      readMemory
        jmp     @f
readMemory:
        call    rxdword
        mov     esi, eax                ; eax, esi is addess
        call    rxdword
        call    remoteDebugReply        ; initial part of response
        mov     ecx, eax                ; ecx is byte count, esi is address
        call    bigReal                 ; put ds, es in big real mode
        cmp     bl, _RD_READ_MEMORY_BYTE
        je      readNextByte
        cmp     bl, _RD_READ_MEMORY_WORD
        je      readNextWord
        jmp     readNextDword
readNextByte:
        mov     al, [esi]
        call    txbyte
        inc     esi
        dec     ecx
        ja      readNextByte
        jmp     remoteDebugLoop
readNextWord:
        mov     ax, [esi]
        call    txword
        add     esi, 2
        sub     ecx, 2
        ja      readNextWord
        jmp     remoteDebugLoop
readNextDword:
        mov     eax, [esi]
        call    txdword
        add     esi, 4
        sub     ecx, 4
        ja      readNextDword
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_WRITE_MEMORY_BYTE
        je      writeMemory
        cmp     bl, _RD_WRITE_MEMORY_WORD
        je      writeMemory
        cmp     bl, _RD_WRITE_MEMORY_DWORD
        je      writeMemory
        cmp     bl, _RD_WRITE_MEMORY_QWORD
        je      writeMemory
        jmp     @f
writeMemory:
        mov     bl, al                  ; save command code
        call    rxdword
        mov     esi, eax                ; eax, esi is addess
        call    rxdword
        push    eax                     ; save byte count
        call    remoteDebugReply        ; initial part of response
        pop     ecx                     ; ecx is byte count, esi is address
        call    bigReal                 ; put ds, es in big real mode
        cmp     bl, _RD_WRITE_MEMORY_BYTE
        je      writeNextByte
        cmp     bl, _RD_WRITE_MEMORY_WORD
        je      writeNextWord
        jmp     writeNextDword
writeNextByte:
        call    rxbyte
        mov     [esi], al
        inc     esi
        dec     ecx
        ja      writeNextByte
        jmp     remoteDebugLoop
writeNextWord:
        call    rxword
        mov     [esi], ax
        add     esi, 2
        sub     ecx, 2
        ja      writeNextWord
        jmp     remoteDebugLoop
writeNextDword:
        call    rxdword
        mov     [esi], eax
        add     esi, 4
        sub     ecx, 4
        ja      writeNextDword
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_READ_EXCEPTION
        jne     @f
        call    remoteDebugReply
        call    remoteDebugReportEvent
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_READ_IO_BYTE
        jne     @f
        call    rxword                  ; receive I/O address
        mov     dx, ax                  ; dx = I/O address
        in      al, dx
        call    remoteDebugReply
        call    txbyte                  ; data portion of response
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_READ_IO_WORD
        jne     @f
        call    rxword                  ; receive I/O address
        mov     dx, ax                  ; dx = I/O address
        in      ax, dx
        call    remoteDebugReply
        call    txword                  ; data portion of response
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_READ_IO_DWORD
        jne     @f
        call    rxword                  ; receive I/O address
        mov     dx, ax                  ; dx = I/O address
        in      eax, dx
        call    remoteDebugReply
        call    txdword                 ; data portion of response
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_WRITE_IO_BYTE
        jne     @f
        call    rxword                  ; receive I/O address
        mov     dx, ax                  ; dx = I/O address
        call    rxbyte                  ; receive I/O data
        out     dx, al
        call    remoteDebugReply
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_WRITE_IO_WORD
        jne     @f
        call    rxword                  ; receive I/O address
        mov     dx, ax                  ; dx = I/O address
        call    rxword                  ; receive I/O data
        out     dx, ax
        call    remoteDebugReply
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_WRITE_IO_DWORD
        jne     @f
        call    rxword                  ; receive I/O address
        mov     dx, ax                  ; dx = I/O address
        call    rxdword                 ; receive I/O data
        out     dx, eax
        call    remoteDebugReply
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_READ_PCI_CONFIG
        jne     @f
        call    rxdword                 ; receive I/O address data
        call    remoteDebugReply
        mov     dx, 0CF8h
        out     dx, eax
        add     dx, 4
        in      eax, dx
        call    txdword                 ; return data
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_PCI_BUS_CHECK
        jne     @f
        call    rxbyte                  ; receive bus number in al
        call    remoteDebugReply        ; initial part of reply
        call    checkBus                ; returns carry status if bus is present
        mov     al, 0
        adc     al, 0                   ; make al non-zero if bus is present
        call    txbyte                  ; return data: al is bus present status
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_WRITE_PCI_CONFIG
        jne     @f
        push    bx                      ; save command code to echo
        call    rxdword                 ; receive CF8 address data
        mov     ecx, eax
        call    rxdword                 ; receive and-mask for clearing write data
        mov     ebx, eax
        call    rxdword                 ; receive CF8 new data to write
        mov     esi, eax
        mov     dx, 0CF8h               ; write CF8 address data
        mov     eax, ecx
        out     dx, eax
        add     dx, 4
        in      eax, dx                 ; read current data
        and     eax, ebx                ; clear data to write
        or      eax, esi
        out     dx, eax                 ; write the new data
        pop     bx                      ; restore command code to echo
        xor     cx, cx                  ; zero additional response bytes
        call    remoteDebugReply        ; initial part of reply
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_TEST_WRITABLE
        jne     @f
        call    rxdword                 ; receive flat address
        call    bigReal                 ; put ds, es in big real mode
        mov     dl, [eax]               ; read current data
        mov     dh, dl
        not     dh                      ; create new data for test write
        mov     [eax], dh               ; write, then read back
        mov     dh, [eax]
        mov     [eax], dl               ; write original data back
        xor     dl, dh                  ; difference
        mov     al, dl
        call    remoteDebugReply
        call    txbyte                  ; data portion of response
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_READ_CMOS
        jne     @f
        call    rxbyte                  ; receive cmos starting offset
        mov     ah, al                  ; ah is starting offset
        call    rxbyte                  ; al is ending offset
        call    remoteDebugReply        ; initial part of reply, cx=0 bytes of data
        mov     cl, al                  ; cl is ending offset        
cmosloop1:
        call    readCmos                ; read cmos location ah into al
        call    txbyte                  ; send it out
        cmp     ah, cl                  ; last byte?
        je      remoteDebugLoop
        inc     ah                      ; move offset to next location
        jmp     cmosloop1
@@:
        ;---------------------------------------
        cmp     bl, _RD_WRITE_CMOS
        jne     @f
        call    rxbyte                  ; receive cmos starting offset
        mov     ah, al                  ; ah is starting offset
        call    rxbyte                  ; al is ending offset
        call    remoteDebugReply        ; initial part of reply, cx=0 bytes of data
        mov     cl, al                  ; cl is ending offset        
cmosloop2:
        call    rxbyte                  ; get next byte from serial port
        call    writeCmos               ; write al to cmos offset ah
        cmp     ah, cl                  ; last byte?
        je      remoteDebugLoop
        inc     ah                      ; move offset to next location
        jmp     cmosloop2
@@:
        ;---------------------------------------
        cmp     bl, _RD_MEM_SEARCH
        jne     @f
        mov     dl, bl                  ; save command code to echo
        call    rxdword                 ; receive start address
        mov     esi, eax                ; esi is start address
        call    rxdword                 ; receive last offset to search
        mov     edi, eax                ; edi is last offset to search
        call    rxbyte                  ; receive pattern byte count
        movzx   ecx, al                 ; ecx is number of pattern bytes to receive
        call    rxbyte                  ; receive match type
        mov     dh, al                  ; dh is match type
        sub     sp, cx                  ; allocate pattern buffer on stack
        sub     sp, cx                  ; increase allocation from bytes to words
        xor     ebx, ebx                ; initialize index into pattern buffer
rxPatternLoop:
        call    rxbyte                  ; receive next pattern byte
        cbw                             ; expand bytes to words to avoid false match
        mov     [esp+ebx*2], ax         ; save pattern data in stack buffer
        inc     ebx
        cmp     ebx, ecx
        jne     rxPatternLoop
        mov     ax, cx                  ; ax is number of pattern bytes
        xor     cx, cx                  ; amount of extra data for remoteDebugReply
        mov     bl, dl                  ; restore command code to echo
        call    remoteDebugReply        ; initial part of reply, cx=0 bytes of data
        mov     cx, ax                  ; cx is number of pattern bytes

; esi is start address
; edi is last address to search
; dh is match type
; ecx is number of pattern bytes

        call    bigReal
        or      dh, dh                  ; see if we are looking for match or no match
        jne     searchLoop2
        
searchLoop1:
        call    samePattern
        jne     matchFound
        inc     esi                     ; move to next position in buffer
        cmp     esi, edi                ; see if all of the buffer has been searched
        jbe     searchLoop1
        jmp     patternNotFound

searchLoop2:
        call    samePattern
        je      matchFound
        inc     esi                     ; move to next position in buffer
        cmp     esi, edi                ; see if all of the buffer has been searched
        jbe     searchLoop2
        jmp     patternNotFound

patternNotFound:        
        mov     esi, 0FFFFFFFFh         ; return code for pattern not found

matchFound:
        lea     esp, [esp+ecx*2]        ; de-allocate pattern buffer from stack
        mov     eax, esi                ; address of match
        call    txdword                 ; return address of match, or FFFFFFFF for no match
        jmp     remoteDebugLoop
@@:
        ;---------------------------------------
        cmp     bl, _RD_MEM_FILL
        jne     @f
        mov     dl, bl                  ; save command code to echo
        call    rxdword                 ; receive start address
        mov     esi, eax                ; esi is start address
        call    rxdword                 ; receive last offset to fill
        mov     edi, eax                ; edi is last offset to fill
        call    rxbyte                  ; receive pattern byte count
        movzx   ecx, al                 ; ecx is number of pattern bytes to receive
        sub     sp, cx                  ; allocate pattern buffer on stack
        xor     ebx, ebx                ; initialize index into pattern buffer
rxPatternLoop2:
        call    rxbyte                  ; receive next pattern byte
        mov     [esp+ebx], al           ; save pattern data in stack buffer
        inc     ebx
        cmp     ebx, ecx
        jne     rxPatternLoop2
        mov     ax, cx                  ; save number of pattern bytes in ax
        xor     cx, cx                  ; amount of extra data for remoteDebugReply
        mov     bl, dl                  ; restore command code to echo
        call    remoteDebugReply        ; initial part of reply, cx=0 bytes of data
        mov     cx, ax                  ; restore number of pattern bytes

; esi is start address
; edi is last address to fill
; ecx is number of pattern bytes

        call    bigReal
fillLoop2:
        xor     bx, bx                  ; reset pattern index
fillLoop1:
        mov     al, [esp+ebx]           ; get next pattern byte
        mov     [esi+ebx], al           ; write it to memory
        inc     bx                      ; offset of next pattern byte
        cmp     bx, cx                  ; all pattern bytes written
        jne     fillLoop1               ; no - complete pattern write
        add     esi, ecx                ; advance destination pointer
        cmp     esi, edi                ; end of buffer reached?
        jbe     fillLoop2               ; no - write another pattern
        add     sp, cx                  ; de-allocate pattern buffer on stack
        xor     al, al                  ; return a byte to signal completion
        call    txbyte
        jmp     remoteDebugLoop
         
@@:
        ;---------------------------------------
        jmp     remoteDebugLoop
remoteDebugReturn:
        ret
remoteDebug endp

;-----------------------------------------------------------------------------
; samePattern - see if memory matches pattern
;               esp+2 points to pattern
;               esi is memory address
;               ecx is number of pattern bytes
;               return equal if pattern matches memory
;               modified: al, ebx

samePattern proc near private
        xor     ebx, ebx                ; ebx is pattern index
searchLoop1:
        mov     al, [esp+ebx*2+2]       ; al is next pattern byte
        cmp     al, [esi+ebx]           ; compare to memory
        jne     noMatch
        inc     bx
        cmp     bx, cx                  ; all pattern bytes matched?
        jne     searchLoop1             ; no - keep checking
noMatch:
        ret 
samePattern endp

;-----------------------------------------------------------------------------

handler macro id
handler&id:
        sub     sp, offset cpuregs.ipreg        ; allocate space for registers not yet on stack
        mov     [esp].cpuregs.xnumber, id
        jmp     exception
        endm

;-----------------------------------------------------------------------------

handler 0   ; divide error
handler 1   ; debug exception
handler 2   ; NMI
handler 3   ; breakpoint exception
handler 4   ; overflow exception
handler 5   ; bound range exceeded
handler 6   ; invalid opcode exception
handler 7   ; device not available

; exceptions 8 and above conflict with slave PIC interrupts, so enable with caution
;handler 8   ; double fault
;handler 13  ; GP fault


;-----------------------------------------------------------------------------
; 16-bit real mode exception common code

exception       proc    private
        ; save the general purpose registers on the stack
        mov     [esp].cpuregs.eaxreg, eax
        mov     [esp].cpuregs.ebxreg, ebx
        mov     [esp].cpuregs.ecxreg, ecx
        mov     [esp].cpuregs.edxreg, edx
        mov     [esp].cpuregs.esireg, esi
        mov     [esp].cpuregs.edireg, edi
        mov     [esp].cpuregs.ebpreg, ebp
        mov     [esp].cpuregs.dsreg, ds
        mov     [esp].cpuregs.esreg, es
        mov     [esp].cpuregs.ssreg, ss
        mov     [esp].cpuregs.fsreg, fs
        mov     [esp].cpuregs.gsreg, gs
        mov     eax, esp
        add     ax, size cpuregs                ; sp before exception        
        mov     [esp].cpuregs.espreg, eax
        mov     ebp, esp                        ; ebp points to saved registers

        ; disable hardware breakpoints while the debugger is running
        mov     eax, dr7
        and     eax, not 3FFh
        mov     dr7, eax
        
        ; clear single step for next time
        and     [ebp].cpuregs.flags, not (1 shl 8)

        call    remoteDebugReportEvent
        call    remoteDebug

        ; clear the cause of the debug exception for next time
        xor     eax, eax
        mov     dr6, eax

        ; restore the general purpose registers from the stack
        mov     eax, [esp].cpuregs.eaxreg
        mov     ebx, [esp].cpuregs.ebxreg
        mov     ecx, [esp].cpuregs.ecxreg
        mov     edx, [esp].cpuregs.edxreg
        mov     esi, [esp].cpuregs.esireg
        mov     edi, [esp].cpuregs.edireg
        mov     ebp, [esp].cpuregs.ebpreg
        mov     ds, [esp].cpuregs.dsreg
        mov     es, [esp].cpuregs.esreg
        mov     fs, [esp].cpuregs.fsreg
        mov     gs, [esp].cpuregs.gsreg
        add     sp, offset cpuregs.ipreg        ; de-allocate space for registers added to stack
        iret                                    ; return to the application

exception       endp
;----------------------------------------------------------------------------
ENDIF ;MKF_DEBUGGER_SUPPORT
debuggerEnd:
;=============================================================================
align 4
text16end:
;----------------------------------------------------------------------------

text32size = text32end - text32start
text16size = text16end - text16start
padcount = (EFI64CODESIZE - 2) - text32size - text16size

    db      padcount dup (0)
    dw      0aa55h
_TEXT16 ends
    end
