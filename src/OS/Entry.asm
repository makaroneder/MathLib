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
GDT64:
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
        dw $ - GDT64 - 1
        dq GDT64

section .text

; eax = 0x36d76289
; ebx -> MB2Info
global Entry
Entry:
    cli
    mov esp, Stack.top
    mov edi, eax
    mov esi, ebx
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
    lgdt [GDT64.pointer]
    jmp 0x08:Entry64

bits 64
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

section .data
global constructorsCalled
constructorsCalled: db 0