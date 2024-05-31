#ifndef State_H
#define State_H
#include <NeuralNetwork.hpp>

/// @brief Contains every information needed by neural network
/// @tparam T Type of number
template <typename T>
struct State : Saveable {
    /// @brief Rate by which the neural network is learning
    T rate;
    /// @brief Input data for learning
    Matrix<T> trainingDataInput;
    /// @brief Output data for learning
    Matrix<T> trainingDataOutput;
    /// @brief Neural network
    NeuralNetwork<T> neuralNetwork;

    /// @brief Creates empty state
    constexpr State(void) {}
    /// @brief Creates a new state
    /// @param r Rate by which the neural network is learning
    /// @param in Input data for learning
    /// @param out Output data for learning
    /// @param nn Neural network
    constexpr State(T r, Matrix<T> in, Matrix<T> out, NeuralNetwork<T> nn) : rate(r), trainingDataInput(in), trainingDataOutput(out), neuralNetwork(nn) {}
    constexpr void Forward(void) {
        neuralNetwork.Forward();
    }
    constexpr T Cost(void) {
        return neuralNetwork.Cost(trainingDataInput, trainingDataOutput);
    }
    constexpr NeuralNetwork<T> FiniteDiff(T eps_ = eps) {
        return neuralNetwork.FiniteDiff(trainingDataInput, trainingDataOutput, eps_);
    }
    constexpr NeuralNetwork<T> Backprop(void) {
        return neuralNetwork.Backprop(trainingDataInput, trainingDataOutput);
    }
    constexpr void Learn(NeuralNetwork<T> diff) {
        neuralNetwork.Learn(diff, rate);
    }
    virtual bool Save(FileSystem& fileSystem, size_t file) const override {
        return fileSystem.Write(file, &rate, sizeof(T)) && trainingDataInput.Save(fileSystem, file) && trainingDataOutput.Save(fileSystem, file) && neuralNetwork.Save(fileSystem, file);
    }
    virtual bool Load(FileSystem& fileSystem, size_t file) override {
        return fileSystem.Read(file, &rate, sizeof(T)) && trainingDataInput.Load(fileSystem, file) && trainingDataOutput.Load(fileSystem, file) && neuralNetwork.Load(fileSystem, file);
    }
};

#endif