#ifndef NeuralNetwork_H
#define NeuralNetwork_H
#include "NeuralNetworkNode.hpp"
#include "Point.hpp"
#include <FunctionT.hpp>

template <typename T>
struct NeuralNetwork : NeuralNetworkNode<T> {
    NeuralNetwork(NeuralNetworkNode<T>* element) : element(element) {
        if (!element) MathLib::Panic("Null network node provided");
    }
    virtual ~NeuralNetwork(void) override {
        delete element;
    }
    virtual NeuralNetworkNode<T>* Copy(void) const override {
        return new NeuralNetwork(element->Copy());
    }
    virtual size_t GetInputWidth(void) const override {
        return element->GetInputWidth();
    }
    virtual size_t GetInputHeight(void) const override {
        return element->GetInputHeight();
    }
    virtual size_t GetOutputWidth(void) const override {
        return element->GetOutputWidth();
    }
    virtual size_t GetOutputHeight(void) const override {
        return element->GetOutputHeight();
    }
    virtual MathLib::Matrix<DualNumber<T>> Evaluate(const MathLib::Matrix<DualNumber<T>>& input) const override {
        return element->Evaluate(input);
    }
    virtual void Train(const MathLib::Function<DualNumber<T>>& func) override {
        element->Train(func);
    }
    void TrainMeanSquaredError(const MathLib::Sequence<Point<MathLib::Matrix<DualNumber<T>>>>& data, const T& rate) {
        const auto& func = MathLib::MakeFunctionT<DualNumber<T>>([this, &data, rate](void) -> DualNumber<T> {
            const size_t size = data.GetSize();
            DualNumber<T> sum;
            for (size_t i = 0; i < size; i++) {
                const Point<MathLib::Matrix<DualNumber<T>>> point = data.AtUnsafe(i);
                sum += (point.y - Evaluate(point.x)).GetLengthSquared();
            }
            return sum * rate / size;
        });
        element->Train(func);
    }

    private:
    NeuralNetworkNode<T>* element;
};

#endif