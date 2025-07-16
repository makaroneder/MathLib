#include <Cryptography/Code/RepetitionCode.hpp>
#include <Cryptography/Cipher/NestedCipher.hpp>
#include <Cryptography/Channel/FlipChannel.hpp>
#include <Cryptography/Channel/Channel.hpp>
#include <Math/UniformDiscreteMeasure.hpp>
#include <Interfaces/VariadicSequence.hpp>
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
        const MathLib::Channel channel = MathLib::Channel(
            new MathLib::FlipChannel<MathLib::num_t, MathLib::UniformDiscreteMeasure<MathLib::num_t>>(),
            new MathLib::NestedCipherData(new MathLib::NestedCipher(MathLib::MakeArray<MathLib::NestedCipherData*>(
                new MathLib::NestedCipherData(new MathLib::RepetitionCode(), 1),
                new MathLib::NestedCipherData(new MathLib::ROT13(), 0)
            )), 1)
        );
        const MathLib::String message = "Hello";
        const MathLib::String tmp = channel.SendString(message, MathLib::VariadicSequence<uint64_t, 3>());
        std::cout << "Sent: " << message << std::endl;
        std::cout << "Recieved: " << tmp << std::endl;
        const size_t diff = message.GetSize() - tmp.GetSize();
        std::cout << "Lost data: " << diff << std::endl;
        if (!diff) {
            std::cout << "Hamming distance: " << message.GetHammingDistance(tmp) << std::endl;
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}