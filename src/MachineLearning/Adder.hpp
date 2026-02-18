#ifndef Adder_H
#define Adder_H
#include "NeuralNetworkState.hpp"

/// @brief Creates default state for this neural network
/// @tparam T Type of number
/// @return State
[[nodiscard]] NeuralNetworkState<MathLib::num_t> GetDefaultState(void) {
    const size_t bits = 3;
    const size_t n = (1 << bits);
    const size_t rows = n * n;
    NeuralNetworkState<MathLib::num_t> state = NeuralNetworkState<MathLib::num_t>(1, MathLib::matrix_t(bits * 2, rows), MathLib::matrix_t(bits, rows), MathLib::NeuralNetwork<MathLib::num_t>(MathLib::NeuralNetwork<MathLib::num_t>::ActivationFunction::Sigmoid, MathLib::MakeArray<size_t>(bits * 2, bits * 4, bits)));
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
[[nodiscard]] MathLib::String StateToString(NeuralNetworkState<MathLib::num_t>& state) {
    const size_t bits = state.trainingDataInput.GetWidth() / 2;
    const size_t n = 1 << bits;
    MathLib::String ret;
    for (size_t y = 0; y < n; y++) {
        for (size_t x = 0; x < n; x++) {
            for (size_t i = 0; i < bits; i++) {
                state.neuralNetwork.GetInput().At(i, 0) = (y >> i) & 1;
                state.neuralNetwork.GetInput().At(i + bits, 0) = (x >> i) & 1;
            }
            if (!state.Forward()) return "";
            size_t z = 0;
            for (size_t i = 0; i < state.trainingDataOutput.GetWidth(); i++) z |= (state.neuralNetwork.GetOutput().At(i, 0) > 0.5) << i;
            ret += '['_M + MathLib::ToString(y) + ", " + MathLib::ToString(x) + "] => " + MathLib::ToString(z) + '\n';
        }
    }
    return ret;
}

#endif