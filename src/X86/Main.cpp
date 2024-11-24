#include <MathLib.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic(MathLib::String("Usage: ") + argv[0] + " <input file>");
        MathLib::HostFileSystem fs;
        MathLib::File file = fs.Open(argv[1], MathLib::OpenMode::Read);
        const size_t size = file.GetSize();
        uint8_t memory[size];
        if (!file.ReadBuffer(memory, size)) MathLib::Panic("Failed to read input file");
        MathLib::X86 x86 = MathLib::X86(MathLib::Array<uint8_t>(memory, size), MathLib::X86State(0, 0));
        if (!x86.Run()) MathLib::Panic("Failed to update emulator");
        std::cout << "State: " << x86 << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}