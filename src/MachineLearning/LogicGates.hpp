#ifndef LogicGates_H
#define LogicGates_H
#include "State.hpp"

/// @brief Creates default state for this neural network
/// @tparam T Type of number
/// @return State
template <typename T>
State<T> GetDefaultState(void) {
    return State<T>(1e-1, Matrix<T>(2, 4, std::vector<T> {
        0, 0,
        0, 1,
        1, 0,
        1, 1,
    }), Matrix<T>(1, 4, std::vector<T> {
        0,
        1,
        1,
        0,
    }), NeuralNetwork<T>(std::vector<size_t> { 2, 2, 1, }));
}

#endif