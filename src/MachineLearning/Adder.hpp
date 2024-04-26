#ifndef Adder_H
#define Adder_H
#include "State.hpp"

/// @brief Creates default state for this neural network
/// @tparam T Type of number
/// @return State
template <typename T>
State<T> GetDefaultState(void) {
    const size_t bits = 3;
    const size_t n = (1 << bits);
    const size_t rows = std::pow(n, 2);
    State<T> state = State<T>(1, Matrix<T>(bits * 2, rows), Matrix<T>(bits, rows), NeuralNetwork<T>({ bits * 2, bits * 4, bits, }));
    for (size_t i = 0; i < rows; i++) {
        const size_t x = i / n;
        const size_t y = i % n;
        const size_t z = x + y;
        for (size_t j = 0; j < bits; j++) {
            state.trainingDataInput.At(j, i) = (x >> j) & 1;
            state.trainingDataInput.At(j + bits, i) = (y >> j) & 1;
            state.trainingDataOutput.At(j, i) = (z >= n) ? 0 : ((z >> j) & 1);
        }
    }
    return state;
}

#endif