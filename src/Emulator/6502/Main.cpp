#include <Emulator/MOS6502/MOS6502.hpp>
#include <Emulator/MOS6502/MOS6502Opcode.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::MOS6502 mos6502 = MathLib::MOS6502(MathLib::MakeArray<uint8_t>(
            (uint8_t)MathLib::MOS6502Opcode::OrAZeroPage, 0x00
        ));
        if (!mos6502.Run()) MathLib::Panic("Failed to update emulator");
        std::cout << "State: " << mos6502 << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}