ENTRY(Entry)
SECTIONS {
    . = 0x7c00;
    .entry : {
        *(.entry)
    }
    .signature 0x7dfe : {
        SHORT(0xaa55)
    }
    start = .;
    .text ALIGN(512) : {
        *(.text)
    }
    .data ALIGN(512) : {
        *(.data)
    }
    .rodata ALIGN(512) : {
        *(.rodata)
    }
    end = .;
    size = end - start;
    .bss ALIGN(512) : {
        *(COMMON)
        *(.bss)
    }
}