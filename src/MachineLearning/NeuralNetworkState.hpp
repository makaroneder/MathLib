#ifndef NeuralNetworkState_H
#define NeuralNetworkState_H
#include <NeuralNetwork.hpp>

template <typename T>
struct NeuralNetworkState : MathLib::Saveable {
    T rate;
    MathLib::Matrix<T> trainingDataInput;
    MathLib::Matrix<T> trainingDataOutput;
    MathLib::NeuralNetwork<T> neuralNetwork;
    MathLib::Array<T> data;

    constexpr NeuralNetworkState(void) {}
    constexpr NeuralNetworkState(const T& rate, const MathLib::Matrix<T>& trainingDataInput, const MathLib::Matrix<T>& trainingDataOutput, const MathLib::NeuralNetwork<T>& neuralNetwork, const MathLib::Sequence<T>& data = MathLib::Array<T>()) : rate(rate), trainingDataInput(trainingDataInput), trainingDataOutput(trainingDataOutput), neuralNetwork(neuralNetwork), data(MathLib::CollectionToArray<T>(data)) {}
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
    [[nodiscard]] virtual bool Save(MathLib::Writable& file) const override {
        if (!file.Write<T>(rate) || !trainingDataInput.Save(file) || !trainingDataOutput.Save(file) || !neuralNetwork.Save(file) || !file.Write<size_t>(data.GetSize())) return false;
        for (const T& x : data)
            if (!file.Write<T>(x)) return false;
        return true;
    }
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override {
        size_t size = 0;
        if (!file.Read<T>(rate) || !trainingDataInput.Load(file) || !trainingDataOutput.Load(file) || !neuralNetwork.Load(file) || !file.Read<size_t>(size)) return false;
        data = MathLib::Array<T>(size);
        for (T& x : data)
            if (!file.Read<T>(x)) return false;
        return true;
    }
};

#endif