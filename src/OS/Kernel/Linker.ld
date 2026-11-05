ENTRY(Entry)
SECTIONS {
    . = 2M;
    kernelStart = .;
    .multibootHeader ALIGN(4K) : {
        KEEP(*(.multibootHeader))
    }
    .text ALIGN(4K) : {
        textStart = .;
        KEEP(*(.text))
        KEEP(*(.ctors))
        KEEP(*(.init))
        KEEP(*(.fini))
        *(.text*)
    }
    .data ALIGN(4K) : {
        dataStart = .;
        KEEP(*(.data))
        *(.data*)
    }
    .rodata ALIGN(4K) : {
        rodataStart = .;
        KEEP(*(.rodata))
        *(.rodata*)
    }
    .bss ALIGN(4K) : {
        bssStart = .;
        KEEP(*(COMMON))
        KEEP(*(.bss))
        *(.bss*)
    }
    kernelEnd = .;
}