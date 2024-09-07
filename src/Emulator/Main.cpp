#include <Libc/HostFunction.hpp>
#include <MathLib.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        #ifdef Debug
        const Test test = TestSelf();
        const size_t tests = test.GetRecordCount();
        const size_t passed = test.GetPassed();
        std::cout << test << passed << "/" << tests << " tests passed" << std::endl;
        if (passed != tests) Panic("Some tests failed");
        #endif
        if (argc < 2) Panic(String("Usage: ") + argv[0] + " <input file>");
        HostFileSystem fs;
        File file = fs.Open(argv[1], OpenMode::Read);
        const size_t size = file.GetSize();
        uint8_t memory[size];
        if (!file.ReadBuffer(memory, size)) Panic("Failed to read input file");
        X86 x86 = X86(Array<uint8_t>(memory, size), X86State(0, 0));
        if (!x86.Run()) Panic("Failed to update emulator");
        std::cout << "State: " << x86 << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}