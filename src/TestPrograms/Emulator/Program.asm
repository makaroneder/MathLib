bits 16
BufferSize equ 3
; Tests mov8, mov16, inc16, dec16, jmp8, jnz8, lods8, stos8, movs8

mov cx, BufferSize
mov di, input
mov al, 0xa
loop1:
    stosb
    inc ax
    dec cx
    jnz loop1
mov cx, BufferSize
mov si, input
mov di, output
loop2:
    movsb
    dec cx
    jnz loop2
mov si, output + BufferSize - 1
lodsb

jmp end
input: times BufferSize db 0
output: times BufferSize db 0
end: