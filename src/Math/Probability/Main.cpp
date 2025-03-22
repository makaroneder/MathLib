#include "FlipChannel.hpp"
#include <Cryptography/Code/RepetitionCode.hpp>
#include <Cryptography/Cipher/NestedCipher.hpp>
#include <Math/UniformDiscreteMeasure.hpp>
#include <Cryptography/Cipher/ROT13.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        #ifndef Debug
        srand(time(nullptr));
        #endif
        FlipChannel<MathLib::num_t, MathLib::UniformDiscreteMeasure<MathLib::num_t>> channel;
        const MathLib::NestedCipher cipher = MathLib::NestedCipher(MathLib::MakeArray<MathLib::NestedCipherData*>(
            new MathLib::NestedCipherData(new MathLib::RepetitionCode(), 1),
            new MathLib::NestedCipherData(new MathLib::ROT13(), 0)
        ));
        const MathLib::String message = "Hello";
        const ssize_t n = 3;
        const MathLib::String tmp = channel.SendString(message, cipher, MathLib::MakeArray<uint64_t>(n));
        std::cout << "Sent: " << message << std::endl;
        std::cout << "Recieved: " << tmp << std::endl;
        std::cout << "Lost data: " << message.GetSize() - tmp.GetSize() << std::endl;
        std::cout << "Hamming distance: " << message.GetHammingDistance(tmp) << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}