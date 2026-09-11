section .text

global PerformRawSystemCall
PerformRawSystemCall:
    push rbx
    mov rax, rdi
    mov rbx, rsi
    int 0x80
    pop rbx
    ret