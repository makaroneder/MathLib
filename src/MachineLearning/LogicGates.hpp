#ifndef LogicGates_H
#define LogicGates_H
#include "State.hpp"
#include <iostream>
#include <unistd.h>

/// @brief Creates default state for this neural network
/// @tparam T Type of number
/// @return State
template <typename T>
State<T> GetDefaultState(void) {
    return State<T>(1e-1, Matrix<T>(2, 4, {
        0, 0,
        0, 1,
        1, 0,
        1, 1,
    }), Matrix<T>(1, 4, {
        0,
        1,
        1,
        0,
    }), NeuralNetwork<T>({ 2, 2, 1, }));
}

#endif