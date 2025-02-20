#include "DNA.hpp"
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        const DNA dna1 = DNA(MathLib::MakeArray<NucleicAcid>(
            adenine,
            adenine,
            thymine,
            cytosine,
            cytosine,
            guanine,
            cytosine,
            thymine,
            adenine,
            guanine
        ));
        const DNA dna2 = DNA(MathLib::MakeArray<NucleicAcid>(
            adenine,
            adenine,
            adenine,
            cytosine,
            cytosine,
            cytosine,
            thymine,
            thymine,
            adenine,
            guanine
        ));
        std::cout << dna1 << std::endl;
        std::cout << dna2 << std::endl;
        std::cout << dna1.GetDifference<MathLib::num_t>(dna2) << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}