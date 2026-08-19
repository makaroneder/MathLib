#ifndef NeuralNetworkConnection_H
#define NeuralNetworkConnection_H
#include "NeuralNetworkNode.hpp"

template <typename T>
struct NeuralNetworkConnection : NeuralNetworkNode<T> {
    NeuralNetworkConnection(NeuralNetworkNode<T>* a, NeuralNetworkNode<T>* b) : a(a), b(b) {
        if (!(a && b)) MathLib::Panic("Null network node provided");
        if (a->GetOutputWidth() != b->GetInputWidth() || a->GetOutputHeight() != b->GetInputHeight()) MathLib::Panic("Invalid network nodes input size");
    }
    virtual ~NeuralNetworkConnection(void) override {
        delete a;
        delete b;
    }
    virtual NeuralNetworkNode<T>* Copy(void) const override {
        return new NeuralNetworkConnection(a->Copy(), b->Copy());
    }
    virtual size_t GetInputWidth(void) const override {
        return a->GetInputWidth();
    }
    virtual size_t GetInputHeight(void) const override {
        return a->GetInputHeight();
    }
    virtual size_t GetOutputWidth(void) const override {
        return b->GetOutputWidth();
    }
    virtual size_t GetOutputHeight(void) const override {
        return b->GetOutputHeight();
    }
    virtual MathLib::Matrix<DualNumber<T>> Evaluate(const MathLib::Matrix<DualNumber<T>>& input) const override {
        return b->Evaluate(a->Evaluate(input));
    }
    virtual void Train(const MathLib::Function<DualNumber<T>>& func) override {
        NeuralNetworkNode<T>* saveA = a->Copy();
        a->Train(func);
        MathLib::Swap<NeuralNetworkNode<T>*>(a, saveA);
        b->Train(func);
        delete a;
        a = saveA;
    }

    private:
    NeuralNetworkNode<T>* a;
    NeuralNetworkNode<T>* b;
};

#endif