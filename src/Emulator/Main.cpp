#include <Emulator/X86.hpp>
#include <Host.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @return Status
int main(void) {
    try {
        X86 x86 = X86(std::vector<uint8_t> {
            0xb8, 0x00, 0x00,
            0xb9, 0x00, 0x01,
            0xba, 0x00, 0x02,
            0xbb, 0x00, 0x03,
            0xbc, 0x00, 0x04,
            0xbd, 0x00, 0x05,
            0xbe, 0x00, 0x06,
            0xbf, 0x00, 0x07,
        });
        while (x86.Step()) {}
        std::cout << x86 << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}