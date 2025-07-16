bits 16
section .entry

extern start
extern size

; Input
;   dl = disk drive
global Entry
Entry:
    ; Set stack
    mov sp, 0x7c00
    mov bp, sp
    ; Read whole bootloader
    xor ax, ax
    mov es, ax
    mov bx, start
    mov cl, 2
    xor dh, dh
    mov ax, size
    call ReadDisk
    jc .error
    ; Disable NMI
    in al, 0x70
    or al, 1 << 7
    out 0x70, al
    in al, 0x71
    ; Enable A20
    in al, 0x92
    or al, 1 << 1
    out 0x92, al

    ; TODO: Load GDT
    ; TODO: Enter protected mode

    ; Jump out of boot sector
    jmp Entry2
    .error: jmp $
; Input
;   al = sector count
;   es:bx = buffer
;   cl = sector
;   ch = track
;   dl = disk drive
;   dh = disk side
; Output
;   ax = ???
; Success
;   Carry flag clear
; Error
;   Carry flag set
ReadDiskSectors:
    mov ah, 0x02
    int 0x13
    ret
; Input
;   ax = size
;   es:bx = buffer
;   cl = sector
;   dl = disk drive
;   dh = disk side
; Output
;   ax = ???
;   ch = 0
; Success
;   Carry flag clear
; Error
;   Carry flag set
ReadDisk:
    xor ch, ch
    .loop:
        inc ch
        cmp ax, 512
        jle .done
        sub ax, 512
        jmp .loop
    .done:
        mov al, ch
        xor ch, ch
        jmp ReadDiskSectors

section .text

Entry2:
    mov ax, 0xb800
    mov es, ax
    mov byte [es:0], 'a'
    mov byte [es:1], 0x7
    jmp $