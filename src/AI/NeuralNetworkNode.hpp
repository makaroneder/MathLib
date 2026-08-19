#ifndef NeuralNetworkNode_H
#define NeuralNetworkNode_H
#include "DualNumber.hpp"
#include <Math/Matrix.hpp>

template <typename T>
struct NeuralNetworkNode {
    virtual ~NeuralNetworkNode(void) {}
    virtual NeuralNetworkNode<T>* Copy(void) const = 0;
    virtual size_t GetInputWidth(void) const = 0;
    virtual size_t GetInputHeight(void) const = 0;
    virtual size_t GetOutputWidth(void) const = 0;
    virtual size_t GetOutputHeight(void) const = 0;
    virtual MathLib::Matrix<DualNumber<T>> Evaluate(const MathLib::Matrix<DualNumber<T>>& input) const = 0;
    virtual void Train(const MathLib::Function<DualNumber<T>>& func) = 0;
};

#endif