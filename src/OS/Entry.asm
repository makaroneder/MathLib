bits 32
section .multibootHeader

Multiboot2Magic equ 0xe85250d6
Multiboot2Length equ Multiboot2Header.end - Multiboot2Header
P2TableCount equ 512

align 8
Multiboot2Header:
    .magic: dd Multiboot2Magic
    .arch: dd 0
    .length: dd Multiboot2Length
    .checksum: dd -(Multiboot2Magic + Multiboot2Length)
    align 8
        .entryTagType: dw 3
        .entryTagFlags: dw 3
        .entryTagSize: dd 12
        .entryAddr: dd Entry
    align 8
        .framebufferTagType: dw 5
        .framebufferTagFlags: dw 0
        .framebufferTagSize: dd 20
        .framebufferWidth: dd 0
        .framebufferHeight: dd 0
        .framebufferDepth: dd 32
    align 8
        .moduleAlignmentTagType: dw 6
        .moduleAlignmentTagFlags: dw 0
        .moduleAlignmentTagSize: dd 8
    align 8
        .lastTagType: dw 0
        .lastTagFlags: dw 0
        .lastTagSize: dd 8
    .end:

section .bss
PreserveBSSStart:
align 4096
P4Table: resb 4096
P3Table: resb 4096
P2Tables: resb 4096 * P2TableCount
align 16
Stack:
    .bottom: resb 4096
    .top:
PreserveBSSEnd:

section .rodata
GDT:
    .nullLimit: dw 0x0000
    .nullBase1: dw 0x0000
    .nullBase2: db 0x00
    .nullAccess: db 0x00
    .nullGranularity: db 0x00
    .nullBase3: db 0x00
    .codeLimit: dw 0xffff
    .codeBase1: dw 0x0000
    .codeBase2: db 0x00
    .codeAccess: db 0x9a
    .codeGranularity: db 0xa0
    .codeBase3: db 0x00
    .dataLimit: dw 0xffff
    .dataBase1: dw 0x0000
    .dataBase2: db 0x00
    .dataAccess: db 0x92
    .dataGranularity: db 0xc0
    .dataBase3: db 0x00
    .pointer:
        dw $ - GDT - 1
        dq GDT

global isrFunctionTable
isrFunctionTable:
    %assign i 0
        %rep 256
            dq ISR%+i
        %assign i i + 1
    %endrep

section .text

; eax = 0x36d76289
; ebx -> MB2Info
global Entry
Entry:
    cli
    mov esp, Stack.top
    mov edi, eax
    mov esi, ebx
    ; Check if CPUID is supported
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    cmp eax, ecx
    je .error
    ; Check if long mode is supported
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb .error
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz .error
    ; Enable paging
    mov eax, P3Table
    or eax, 0b11
    mov [P4Table], eax
    %assign i 0
        %rep P2TableCount
            mov eax, P2Tables + 4096 * i
            or eax, 0b11
            mov [P3Table + 8 * i], eax
        %assign i i + 1
    %endrep
    xor ecx, ecx
    .mapP2:
        mov eax, ecx
        shl eax, 21
        or eax, 0b10000011
        %assign i 0
            %rep P2TableCount
                mov [P2Tables + 4096 * i + ecx * 8], eax
                add eax, 0x200000 * 512
            %assign i i + 1
        %endrep
        inc ecx
        cmp ecx, 512
        jne .mapP2
    mov eax, P4Table
    mov cr3, eax
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax
    ; Enable long mode
    mov ecx, 0xc0000080
    rdmsr
    or eax, 1 << 8
    wrmsr
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax
    lgdt [GDT.pointer]
    jmp 0x08:Entry64
    .error:
        mov al, 'E'
        out 0xe9, al
        mov al, 'r'
        out 0xe9, al
        out 0xe9, al
        mov al, 'o'
        out 0xe9, al
        mov al, 'r'
        out 0xe9, al
        jmp $

bits 64
extern InterruptHandler
extern bssStart
extern _init
extern Main
extern end

Entry64:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    xor rbp, rbp
    push rdi
    push rsi
    mov rdi, bssStart
    mov rcx, PreserveBSSStart
    sub rcx, rdi
    xor al, al
    cld
    rep stosb
    mov rdi, PreserveBSSEnd
    mov rcx, end
    sub rcx, rdi
    xor al, al
    cld
    rep stosb
    call _init
    pop rsi
    pop rdi
    jmp Main

%macro PushAll 0
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rbp
    push rsp
    push rdi
    push rsi
    push rdx
    push rcx
    push rbx
    push rax
%endmacro
%macro PopAll 0
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop rsp
    pop rbp
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
%endmacro

%macro ISR 1
ISR%1:
    PushAll
    mov rdi, %1
    mov rsi, rsp
    xor rdx, rdx
    call InterruptHandler
    PopAll
    iretq
%endmacro
%macro ISRWithErrorCode 1
ISR%1:
    pop qword [.errorCode]
    PushAll
    mov rdi, %1
    mov rsi, rsp
    mov rdx, [.errorCode]
    call InterruptHandler
    PopAll
    iretq
    .errorCode: dq 0
%endmacro
%assign i 0
    %rep 8
        ISR i
    %assign i i + 1
%endrep
%assign i 10
    %rep 5
        ISR i
    %assign i i + 1
%endrep
%assign i 15
    %rep 2
        ISR i
    %assign i i + 1
%endrep
%assign i 18
    %rep 3
        ISR i
    %assign i i + 1
%endrep
%assign i 22
    %rep 7
        ISR i
    %assign i i + 1
%endrep
%assign i 29
    %rep 2
        ISR i
    %assign i i + 1
%endrep
%assign i 31
    %rep 256 - 31
        ISR i
    %assign i i + 1
%endrep

ISR 9
ISRWithErrorCode 8
ISRWithErrorCode 17
ISRWithErrorCode 21

section .data
global constructorsCalled
constructorsCalled: db 0