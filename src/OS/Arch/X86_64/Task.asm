bits 64
section .text

; rdi -> prev
; rsi -> new
global ArchSwitchTasks
ArchSwitchTasks:
    pop qword [rdi + 0 * 8]
    mov [rdi + 1 * 8], rsp
    mov rsp, [rsi + 1 * 8]
    jmp [rsi + 0 * 8]