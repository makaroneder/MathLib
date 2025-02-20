#include "UniformDiscreteMeasure.hpp"
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        const MathLib::Set<MathLib::num_t> heads = MathLib::Set<MathLib::num_t>(MathLib::MakeArray<MathLib::num_t>(0));
        const MathLib::Set<MathLib::num_t> tails = MathLib::Set<MathLib::num_t>(MathLib::MakeArray<MathLib::num_t>(1));
        const UniformDiscreteMeasure<MathLib::num_t, MathLib::num_t> space = UniformDiscreteMeasure<MathLib::num_t, MathLib::num_t>((heads + tails).Get("Failed to combine sets"));
        std::cout << "P(null) = " << space.GetMeasure(MathLib::Set<MathLib::num_t>()) << std::endl;
        std::cout << "P(heads) = " << space.GetMeasure(heads) << std::endl;
        std::cout << "P(tails) = " << space.GetMeasure(tails) << std::endl;
        std::cout << "E(X) = " << space.GetExpectedValue() << std::endl;
        std::cout << "Var(X) = " << space.GetVariance() << std::endl;
        std::cout << "H(X) = " << space.GetTotalEntropy(2) << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}