#include <Libc/HostFunction.hpp>
#include <Emulator/X86.hpp>
#include <MathLib.hpp>
#include <Host.hpp>
#include <iostream>

// TODO: imul idiv sbb in out movsd lodsb stosb rep lgdt int

/// @brief Entry point for this program
/// @return Status
int main(int, char**) {
    try {
        const uint8_t bios[] = {
            0xe9, 0xfd, 0x7b,   // jmp 0x7c00
        };
        const uint8_t program[] = {
            0xbc, 0x00, 0x7c,   // mov sp, 0x7c00
            0xb8, 0x02, 0x00,   // mov ax, 0x0002
            0xbb, 0x02, 0x00,   // mov bx, 0x0002
            0x39, 0xd8,         // cmp ax, bx
        };
        Array<uint8_t> memory = Array<uint8_t>(0x7c00 + SizeOfArray(program));
        for (size_t i = 0; i < SizeOfArray(bios); i++) memory.At(i) = bios[i];
        for (size_t i = 0; i < SizeOfArray(program); i++) memory.At(i + 0x7c00) = program[i];
        X86 x86 = X86(memory);
        while (x86.GetIP().value < x86.GetSize()) if (!x86.Step()) Panic("Failed to update emulator");
        std::cout << x86 << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}