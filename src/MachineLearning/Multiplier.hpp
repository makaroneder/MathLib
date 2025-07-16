#ifndef Multiplier_H
#define Multiplier_H
#include "NeuralNetworkState.hpp"
#include <Interfaces/VariadicSequence.hpp>

/// @brief Creates default state for this neural network
/// @tparam T Type of number
/// @return State
[[nodiscard]] NeuralNetworkState<MathLib::num_t> GetDefaultState(void) {
    constexpr size_t inputSize = 4;
    const MathLib::num_t multiplier = MathLib::RandomNumber<MathLib::num_t>(-50, 50);
    MathLib::matrix_t ret = MathLib::matrix_t(1, inputSize);
    for (size_t i = 0; i < ret.GetHeight(); i++) ret.At(0, i) = i;
    return NeuralNetworkState<MathLib::num_t>(1e-1, ret, ret * multiplier, MathLib::NeuralNetwork<MathLib::num_t>(MathLib::NeuralNetwork<MathLib::num_t>::ActivationFunction::None, MathLib::VariadicSequence<size_t, 1, 1>()), MathLib::MakeArray<MathLib::num_t>(multiplier));
}
[[nodiscard]] MathLib::String StateToString(NeuralNetworkState<MathLib::num_t>& state) {
    MathLib::String ret;
    constexpr size_t inputSizeMultiplier = 200;
    for (size_t x = 0; x < state.trainingDataInput.GetHeight() * inputSizeMultiplier; x++) {
        state.neuralNetwork.GetInput().At(0, 0) = x;
        if (!state.Forward()) return "";
        const MathLib::num_t expected = x * state.data.At(0);
        const MathLib::num_t output = state.neuralNetwork.GetOutput().At(0, 0);
        ret += MathLib::ToString(x) + " * " + MathLib::ToString(state.data.At(0)) + " => {\n";
        ret += "\tOutput: "_M + MathLib::ToString(output) + '\n';
        ret += "\tExpected output: "_M + MathLib::ToString(expected) + '\n';
        ret += "\tError: "_M + MathLib::ToString(MathLib::RelativeError<MathLib::num_t>(output, expected)) + "\n}\n";
    }
    return ret;
}

#endif