ENTRY(Entry)
SECTIONS {
    . = 2M;
    kernelStart = .;
    .multibootHeader ALIGN(4K) : {
        *(.multibootHeader)
    }
    .text ALIGN(4K) : {
        textStart = .;
        *(.text)
    }
    .data ALIGN(4K) : {
        dataStart = .;
        *(.data)
    }
    .rodata ALIGN(4K) : {
        rodataStart = .;
        *(.rodata)
    }
    .bss ALIGN(4K) : {
        bssStart = .;
        *(COMMON)
        *(.bss)
    }
    kernelEnd = .;
}