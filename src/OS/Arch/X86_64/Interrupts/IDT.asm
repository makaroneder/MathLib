bits 64
section .rodata
global isrFunctionTable
isrFunctionTable:
    %assign i 0
        %rep 256
            dq ISR%+i
        %assign i i + 1
    %endrep

section .text
extern InterruptHandler
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
    push rdi
    push rsi
    push rdx
    push rcx
    push rbx
    push rax
    mov rax, ss
    push rax
    mov rax, gs
    push rax
    mov rax, fs
    push rax
    mov rax, es
    push rax
    mov rax, ds
    push rax
    push rsp
%endmacro
%macro PopAll 0
    pop rsp
    pop rax
    mov ds, rax
    pop rax
    mov es, rax
    pop rax
    mov fs, rax
    pop rax
    mov gs, rax
    pop rax
    mov ss, rax
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
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