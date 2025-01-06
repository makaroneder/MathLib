#include <MathLib.hpp>
#include <SingleTypePair.hpp>
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
        MathLib::matrix_t a = MathLib::matrix_t(SizeOfArray(data), SizeOfArray(data));
        MathLib::matrix_t b = MathLib::matrix_t(1, SizeOfArray(data));
        for (size_t y = 0; y < SizeOfArray(data); y++) {
            for (size_t x = 0; x < SizeOfArray(data); x++)
                a.At(x, y) = MathLib::Pow(data[y].first, x);
            b.At(0, y) = data[y].second;
        }
        std::cout << (a.GetInverse().Get("Failed to get inverse of matrix") * b).Get("Failed to multiply to matrices").GetTranspose() << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}