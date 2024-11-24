#include <MathLib.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::MathArch emulator = MathLib::MathArch(MathLib::Array<uint8_t>(64));
        size_t i = 0;
        i += emulator.EncodeConstInstruction(i, MathLib::MathArchOpcode::SetSPToConst, emulator.GetSize());
        i += emulator.EncodeConstInstruction(i, MathLib::MathArchOpcode::PushConst, 0xabcd);
        i += emulator.EncodeOneByteInstruction(i, MathLib::MathArchOpcode::PopToA);
        i += emulator.EncodeConstInstruction(i, MathLib::MathArchOpcode::SetPCToConst, UINT64_MAX);
        if (!emulator.Run()) MathLib::Panic("Failed to update emulator");
        std::cout << "State: " << emulator << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}