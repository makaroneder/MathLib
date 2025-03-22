#include <Math/SystemOfLinearEquations.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        const MathLib::SingleTypePair<MathLib::num_t> data[] = {
            MathLib::SingleTypePair<MathLib::num_t>(0, 0),
            MathLib::SingleTypePair<MathLib::num_t>(1, 1),
        };
        MathLib::SystemOfLinearEquations<MathLib::num_t> equations = MathLib::SystemOfLinearEquations<MathLib::num_t>(SizeOfArray(data));
        for (size_t y = 0; y < SizeOfArray(data); y++) {
            for (size_t x = 0; x < SizeOfArray(data); x++)
                equations.EquationAt(x, y) = MathLib::Pow(data[y].first, x);
            equations.ValueAt(y) = data[y].second;
        }
        std::cout << equations.GetMatrixSolution().Get("Failed to solve").GetTranspose() << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}