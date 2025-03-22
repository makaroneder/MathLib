#include "ConstantStrategy.hpp"
#include "Game.hpp"
#include <iostream>

template <typename T>
Tensor<T> PrisonersDilemmaPayoffMatrix(const T& cooperationPayoff, const T& punishmentPayoff, const T& temptationPayoff, const T& suckersPayoff) {
    Tensor<T> ret = Tensor<T>(MathLib::MakeArray<size_t>(2, 2, 2));
    ret.At(MathLib::MakeArray<size_t>(0, 0, 0)) = ret.At(MathLib::MakeArray<size_t>(0, 0, 1)) = cooperationPayoff;
    ret.At(MathLib::MakeArray<size_t>(1, 0, 0)) = ret.At(MathLib::MakeArray<size_t>(0, 1, 1)) = temptationPayoff;
    ret.At(MathLib::MakeArray<size_t>(1, 0, 1)) = ret.At(MathLib::MakeArray<size_t>(0, 1, 0)) = suckersPayoff;
    ret.At(MathLib::MakeArray<size_t>(1, 1, 0)) = ret.At(MathLib::MakeArray<size_t>(1, 1, 1)) = punishmentPayoff;
    return ret;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        Game<ssize_t> game = Game<ssize_t>(MathLib::MakeArray<Player<ssize_t>*>(
            new Player<ssize_t>(nullptr, 0),
            new Player<ssize_t>(new ConstantStrategy(1), 0)
        ), PrisonersDilemmaPayoffMatrix<ssize_t>(-1, -2, 0, -3));
        while (true) {
            std::cout << "Game: " << game << std::endl;
            std::cout << "> ";
            std::string tmp;
            std::cin >> tmp;
            if (tmp == "exit") break;
            game.Update(MathLib::StringToNumber(MathLib::String(tmp)));
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}