#include "LambdaTerm.hpp"
#include <Host.hpp>
#include <iostream>

LambdaTerm ToChurchNumeral(const LambdaTerm& zero, const LambdaTerm& successor, size_t n) {
    LambdaTerm ret = zero;
    for (size_t i = 0; i < n; i++) ret = successor.Apply(ret);
    return ret;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        const LambdaTerm zero = LambdaTerm(LambdaTerm(LambdaTerm('x'_M), 'x'_M), 'f'_M);
        const LambdaTerm successor = LambdaTerm(LambdaTerm(
            LambdaTerm(
                LambdaTerm(LambdaTerm('f'_M), LambdaTerm(
                    LambdaTerm(LambdaTerm('n'_M), LambdaTerm('f'_M)), LambdaTerm('x'_M)
                )), 'x'_M
            ), 'f'_M
        ), 'n'_M);
        const LambdaTerm add = LambdaTerm(LambdaTerm(
            LambdaTerm(
                LambdaTerm(
                    LambdaTerm(
                        LambdaTerm(
                            LambdaTerm('m'_M),
                            LambdaTerm('f'_M)
                        ),
                        LambdaTerm(
                            LambdaTerm(
                                LambdaTerm('n'_M),
                                LambdaTerm('f'_M)
                            ),
                            LambdaTerm('x'_M)
                        )
                    ), 'x'_M
                ), 'f'_M
            ), 'n'_M
        ), 'm'_M);
        // TODO: FromChurchNumeral
        std::cout << "SUCC := " << successor << std::endl;
        std::cout << "ADD := " << add << std::endl;
        for (size_t i = 0; i < 10; i++)
            std::cout << i << " := " << ToChurchNumeral(zero, successor, i) << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}