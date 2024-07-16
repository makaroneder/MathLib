#ifndef State_H
#define State_H
#include <NeuralNetwork.hpp>

/// @brief Contains every information needed by neural network
/// @tparam T Type of number
template <typename T>
struct NeuralNetworkState : Saveable {
    /// @brief Rate by which the neural network is learning
    T rate;
    /// @brief Input data for learning
    Matrix<T> trainingDataInput;
    /// @brief Output data for learning
    Matrix<T> trainingDataOutput;
    /// @brief Neural network
    NeuralNetwork<T> neuralNetwork;

    /// @brief Creates empty state
    constexpr NeuralNetworkState(void) {}
    /// @brief Creates a new state
    /// @param r Rate by which the neural network is learning
    /// @param in Input data for learning
    /// @param out Output data for learning
    /// @param nn Neural network
    constexpr NeuralNetworkState(const T& r, const Matrix<T>& in, const Matrix<T>& out, const NeuralNetwork<T>& nn) : rate(r), trainingDataInput(in), trainingDataOutput(out), neuralNetwork(nn) {}
    constexpr void Forward(void) {
        neuralNetwork.Forward();
    }
    constexpr T Cost(void) {
        return neuralNetwork.Cost(trainingDataInput, trainingDataOutput);
    }
    constexpr NeuralNetwork<T> FiniteDiff(const T& eps_ = eps) {
        return neuralNetwork.FiniteDiff(trainingDataInput, trainingDataOutput, eps_);
    }
    constexpr NeuralNetwork<T> Backprop(void) {
        return neuralNetwork.Backprop(trainingDataInput, trainingDataOutput);
    }
    constexpr void Learn(const NeuralNetwork<T>& diff) {
        neuralNetwork.Learn(diff, rate);
    }
    virtual bool Save(ByteDevice& file) const override {
        return file.Write<T>(rate) && trainingDataInput.Save(file) && trainingDataOutput.Save(file) && neuralNetwork.Save(file);
    }
    virtual bool Load(ByteDevice& file) override {
        return file.Read<T>(rate) && trainingDataInput.Load(file) && trainingDataOutput.Load(file) && neuralNetwork.Load(file);
    }
};

#endif