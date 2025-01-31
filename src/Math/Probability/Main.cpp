#include "FairProbabilitySpace.hpp"
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        const MathLib::Set<uint8_t> heads = MathLib::Set<uint8_t>(MathLib::MakeArrayFromSingle<uint8_t>(0));
        const MathLib::Set<uint8_t> tails = MathLib::Set<uint8_t>(MathLib::MakeArrayFromSingle<uint8_t>(1));
        FairProbabilitySpace<uint8_t, MathLib::num_t> space = FairProbabilitySpace<uint8_t, MathLib::num_t>((heads + tails).Get("Failed to combine sets"));
        std::cout << "P(null) = " << space.GetProbability(MathLib::Set<uint8_t>()) << std::endl;
        std::cout << "P(heads) = " << space.GetProbability(heads) << std::endl;
        std::cout << "P(tails) = " << space.GetProbability(tails) << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}