#ifndef Adder_H
#define Adder_H
#include "State.hpp"

/// @brief Creates default state for this neural network
/// @tparam T Type of number
/// @return State
template <typename T>
NeuralNetworkState<T> GetDefaultState(void) {
    const size_t bits = 3;
    const size_t n = (1 << bits);
    const size_t rows = Pow(n, 2);
    NeuralNetworkState<T> state = NeuralNetworkState<T>(1, MathLib::Matrix<T>(bits * 2, rows), MathLib::Matrix<T>(bits, rows), MathLib::NeuralNetwork<T>(MathLib::NeuralNetwork<T>::ActivationFunction::Sigmoid, std::vector<size_t> { bits * 2, bits * 4, bits, }));
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
template <typename T>
MathLib::String StateToString(NeuralNetworkState<T>& state) {
    const size_t bits = state.trainingDataInput.GetWidth() / 2;
    const size_t n = 1 << bits;
    MathLib::String ret;
    for (size_t y = 0; y < n; y++) {
        for (size_t x = 0; x < n; x++) {
            for (size_t i = 0; i < bits; i++) {
                state.neuralNetwork.GetInput().At(i, 0) = (y >> i) & 1;
                state.neuralNetwork.GetInput().At(i + bits, 0) = (x >> i) & 1;
            }
            state.Forward();
            size_t z = 0;
            for (size_t i = 0; i < state.trainingDataOutput.GetWidth(); i++) z |= (state.neuralNetwork.GetOutput().At(i, 0) > 0.5) << i;
            ret += MathLib::String("[") + MathLib::ToString(y) + ", " + MathLib::ToString(x) + "] => " + MathLib::ToString(z) + '\n';
        }
    }
    return ret;
}

#endif