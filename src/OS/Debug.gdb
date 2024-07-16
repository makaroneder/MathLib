set disassembly-flavor intel
symbol-file bin/Kernel.elf
break *Main
target remote | qemu-system-x86_64 -cdrom bin/OS.img -S -gdb stdio