#include <Libc/HostFileSystem.hpp>
#include <Emulator/X86/X86.hpp>
#include <ExternArray.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <input file>");
        MathLib::HostFileSystem fs;
        MathLib::File file = fs.Open(MathLib::String(argv[1]), MathLib::OpenMode::Read);
        const size_t size = file.GetSize();
        uint8_t memory[size];
        if (!file.ReadBuffer(memory, size)) MathLib::Panic("Failed to read input file");
        MathLib::X86 x86 = MathLib::X86(MathLib::ExternArray<uint8_t>(memory, size));
        if (!x86.Run()) MathLib::Panic("Failed to update emulator");
        std::cout << "State: " << x86 << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}