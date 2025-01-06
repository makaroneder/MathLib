bits 32
section .multibootHeader
extern end
extern start
extern bssStart

%define TextMode 1

Multiboot1Magic equ 0x1badb002
Multiboot1AlignModules equ 0
Multiboot1MemoryInfo equ 1
Multiboot1VideoInfo equ 2
Multiboot1AoutKludge equ 16
Multiboot1Flags equ 1 << Multiboot1VideoInfo | 1 << Multiboot1MemoryInfo | 1 << Multiboot1AlignModules
Multiboot2Magic equ 0xe85250d6
Multiboot2Length equ Multiboot2Header.end - Multiboot2Header
P2TableCount equ 512

align 4
Multiboot1Header:
    .magic: dd Multiboot1Magic
    .flags: dd Multiboot1Flags
    .checksum: dd -(Multiboot1Magic + Multiboot1Flags)
    .headerAddress: dd Multiboot1Header
    .loadAddress: dd start
    .loadEndAddress: dd bssStart
    .bssEndAddress: dd end
    .entryAddress: dd Entry
    .framebufferMode: dd TextMode
    .framebufferWidth: dd 0
    .framebufferHeight: dd 0
    .framebufferDepth: dd 32 * !TextMode

align 8
Multiboot2Header:
    .magic: dd Multiboot2Magic
    .arch: dd 0
    .length: dd Multiboot2Length
    .checksum: dd -(Multiboot2Magic + Multiboot2Length)
    align 8
        .entryTagType: dw 3
        .entryTagFlags: dw 0
        .entryTagSize: dd 12
        .entryTagAddress: dd Entry
    align 8
        .flagsTagType: dw 4
        .flagsTagFlags: dw 0
        .flagsTagSize: dd 12
        .flagsTagConsoleFlags: dd 1 << 1
    %if !TextMode
    align 8
        .framebufferTagType: dw 5
        .framebufferTagFlags: dw 0
        .framebufferTagSize: dd 20
        .framebufferTagWidth: dd 0
        .framebufferTagHeight: dd 0
        .framebufferTagDepth: dd 32
    %endif
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
    .bottom: resb 2 * 4096
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

section .text

; eax = 0x2badb002 | 0x36d76289
; ebx -> MB1Info | MB2Info
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