section .text

global PerformSystemCall
PerformSystemCall:
    push rbx
    mov rax, rdi
    mov rbx, rsi
    int 0x80
    pop rbx
    ret