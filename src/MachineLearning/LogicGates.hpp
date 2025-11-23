#ifndef LogicGates_H
#define LogicGates_H
#include "NeuralNetworkState.hpp"
#include <Interfaces/Sequence/VariadicSequence.hpp>

/// @brief Creates default state for this neural network
/// @tparam T Type of number
/// @return State
[[nodiscard]] NeuralNetworkState<MathLib::num_t> GetDefaultState(void) {
    return NeuralNetworkState<MathLib::num_t>(1e-1, MathLib::matrix_t(2, 4, MathLib::VariadicSequence<MathLib::num_t,
        0, 0,
        0, 1,
        1, 0,
        1, 1
    >()), MathLib::matrix_t(1, 4, MathLib::VariadicSequence<MathLib::num_t,
        0,
        1,
        1,
        0
    >()), MathLib::NeuralNetwork<MathLib::num_t>(MathLib::NeuralNetwork<MathLib::num_t>::ActivationFunction::Tanh, MathLib::VariadicSequence<size_t, 2, 2, 1>()));
}
[[nodiscard]] MathLib::String StateToString(NeuralNetworkState<MathLib::num_t>& state) {
    MathLib::String ret;
    for (size_t y = 0; y < 2; y++) {
        for (size_t x = 0; x < 2; x++) {
            state.neuralNetwork.GetInput().At(0, 0) = y;
            state.neuralNetwork.GetInput().At(1, 0) = x;
            if (!state.Forward()) return "";
            ret += '['_M + MathLib::ToString(y) + ", " + MathLib::ToString(x) + "] => " + MathLib::ToString(state.neuralNetwork.GetOutput().At(0, 0)) + '\n';
        }
    }
    return ret;
}

#endif