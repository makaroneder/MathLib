#ifndef NeuralNetworkNeuron_H
#define NeuralNetworkNeuron_H
#include "NeuralNetworkNode.hpp"

template <typename T>
MathLib::Matrix<DualNumber<T>> GetDerivative(MathLib::Matrix<DualNumber<T>>& matrix, const MathLib::Function<DualNumber<T>>& func) {
    const size_t width = matrix.GetWidth();
    const size_t height = matrix.GetHeight();
    MathLib::Matrix<DualNumber<T>> delta = MathLib::Matrix<DualNumber<T>>(width, height);
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            matrix.AtUnsafe(x, y).eps = 1;
            delta.AtUnsafe(x, y) = func().eps;
            matrix.AtUnsafe(x, y).eps = 0;
        }
    }
    return delta;
}
template <typename T>
struct NeuralNetworkNeuron : NeuralNetworkNode<T> {
    NeuralNetworkNeuron(size_t inputWidth, size_t inputHeight, size_t outputWidth, const MathLib::Function<MathLib::Matrix<DualNumber<T>>, MathLib::Matrix<DualNumber<T>>>& activate) : weight(MathLib::Matrix<DualNumber<T>>(outputWidth, inputWidth)), bias(MathLib::Matrix<DualNumber<T>>(outputWidth, inputHeight)), activate(activate) {
        for (DualNumber<T>& x : weight) x = MathLib::RandomFloat();
        for (DualNumber<T>& x : bias) x = MathLib::RandomFloat();
    }
    NeuralNetworkNeuron(const MathLib::Matrix<DualNumber<T>>& weight, const MathLib::Matrix<DualNumber<T>>& bias, const MathLib::Function<MathLib::Matrix<DualNumber<T>>, MathLib::Matrix<DualNumber<T>>>& activate) : weight(weight), bias(bias), activate(activate) {}
    virtual NeuralNetworkNode<T>* Copy(void) const override {
        return new NeuralNetworkNeuron(weight, bias, activate);
    }
    virtual size_t GetInputWidth(void) const override {
        return weight.GetHeight();
    }
    virtual size_t GetInputHeight(void) const override {
        return bias.GetHeight();
    }
    virtual size_t GetOutputWidth(void) const override {
        return weight.GetWidth();
    }
    virtual size_t GetOutputHeight(void) const override {
        return bias.GetHeight();
    }
    virtual MathLib::Matrix<DualNumber<T>> Evaluate(const MathLib::Matrix<DualNumber<T>>& input) const override {
        const MathLib::Expected<MathLib::Matrix<DualNumber<T>>> tmp = input * weight;
        return tmp.HasValue() ? activate(tmp.Get() + bias) : MathLib::Matrix<DualNumber<T>>();
    }
    virtual void Train(const MathLib::Function<DualNumber<T>>& func) override {
        const MathLib::Matrix<DualNumber<T>> deltaWeight = GetDerivative<T>(weight, func);
        const MathLib::Matrix<DualNumber<T>> deltaBias = GetDerivative<T>(bias, func);
        weight -= deltaWeight;
        bias -= deltaBias;
    }

    private:
    MathLib::Matrix<DualNumber<T>> weight;
    MathLib::Matrix<DualNumber<T>> bias;
    const MathLib::Function<MathLib::Matrix<DualNumber<T>>, MathLib::Matrix<DualNumber<T>>>& activate;
};

#endif