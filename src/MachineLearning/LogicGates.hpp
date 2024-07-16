#ifndef LogicGates_H
#define LogicGates_H
#include "State.hpp"

/// @brief Creates default state for this neural network
/// @tparam T Type of number
/// @return State
template <typename T>
NeuralNetworkState<T> GetDefaultState(void) {
    return NeuralNetworkState<T>(1e-1, Matrix<T>(2, 4, std::vector<T> {
        0, 0,
        0, 1,
        1, 0,
        1, 1,
    }), Matrix<T>(1, 4, std::vector<T> {
        0,
        1,
        1,
        0,
    }), NeuralNetwork<T>(NeuralNetwork<T>::ActivationFunction::Tanh, std::vector<size_t> { 2, 2, 1, }));
}
template <typename T>
String StateToString(NeuralNetworkState<T>& state) {
    String ret;
    for (size_t y = 0; y < 2; y++) {
        for (size_t x = 0; x < 2; x++) {
            state.neuralNetwork.GetInput().At(0, 0) = y;
            state.neuralNetwork.GetInput().At(1, 0) = x;
            state.Forward();
            ret += String("[") + ToString(y) + ", " + ToString(x) + "] => " + ToString(state.neuralNetwork.GetOutput().At(0, 0)) + '\n';
        }
    }
    return ret;
}

#endif