#ifndef State_H
#define State_H
#include <NeuralNetwork.hpp>

/// @brief Contains every information needed by neural network
/// @tparam T Type of number
template <typename T>
struct NeuralNetworkState : MathLib::Saveable {
    /// @brief Rate by which the neural network is learning
    T rate;
    /// @brief Input data for learning
    MathLib::Matrix<T> trainingDataInput;
    /// @brief Output data for learning
    MathLib::Matrix<T> trainingDataOutput;
    /// @brief Neural network
    MathLib::NeuralNetwork<T> neuralNetwork;

    /// @brief Creates empty state
    constexpr NeuralNetworkState(void) {}
    /// @brief Creates a new state
    /// @param r Rate by which the neural network is learning
    /// @param in Input data for learning
    /// @param out Output data for learning
    /// @param nn Neural network
    constexpr NeuralNetworkState(const T& r, const MathLib::Matrix<T>& in, const MathLib::Matrix<T>& out, const MathLib::NeuralNetwork<T>& nn) : rate(r), trainingDataInput(in), trainingDataOutput(out), neuralNetwork(nn) {}
    [[nodiscard]] constexpr bool Forward(void) {
        return neuralNetwork.Forward();
    }
    [[nodiscard]] constexpr T Cost(void) {
        return neuralNetwork.Cost(trainingDataInput, trainingDataOutput);
    }
    [[nodiscard]] constexpr MathLib::NeuralNetwork<T> FiniteDiff(const T& eps_ = MathLib::eps) {
        return neuralNetwork.FiniteDiff(trainingDataInput, trainingDataOutput, eps_);
    }
    [[nodiscard]] constexpr MathLib::Expected<MathLib::NeuralNetwork<T>> Backprop(void) {
        return neuralNetwork.Backprop(trainingDataInput, trainingDataOutput);
    }
    constexpr void Learn(const MathLib::NeuralNetwork<T>& diff) {
        neuralNetwork.Learn(diff, rate);
    }
    [[nodiscard]] virtual bool Save(MathLib::Writeable& file) const override {
        return file.Write<T>(rate) && trainingDataInput.Save(file) && trainingDataOutput.Save(file) && neuralNetwork.Save(file);
    }
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override {
        return file.Read<T>(rate) && trainingDataInput.Load(file) && trainingDataOutput.Load(file) && neuralNetwork.Load(file);
    }
};

#endif