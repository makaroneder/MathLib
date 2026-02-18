bits 16
BufferSize equ 3

mov cx, BufferSize
mov di, input
mov al, 0xa
loop1:
    stosb
    inc ax
    loop loop1

mov cx, BufferSize
mov si, input
mov di, output
rep movsb

mov si, output + BufferSize - 1
lodsb
lds bx, [loadInput]
les cx, [loadInput]
mov dx, 1
neg dx

jmp end
input: times BufferSize db 0
output: times BufferSize db 0
loadInput:
    dw 0x1234
    dw 0x5678
end: