#ifndef NeuralNetworkLayer_H
#define NeuralNetworkLayer_H
#include "NeuralNetworkNode.hpp"

enum class NeuralNetworkLayerMerge : uint8_t {
    SideBySide,
    Stacked,
};
template <typename T>
struct NeuralNetworkLayer : NeuralNetworkNode<T> {
    NeuralNetworkLayer(NeuralNetworkLayerMerge merge, NeuralNetworkNode<T>* a, NeuralNetworkNode<T>* b) : a(a), b(b), merge(merge) {
        if (!(a && b)) MathLib::Panic("Null network node provided");
        if (a->GetInputWidth() != b->GetInputWidth() || a->GetInputHeight() != b->GetInputHeight()) MathLib::Panic("Invalid network nodes input size");
        switch (merge) {
            case NeuralNetworkLayerMerge::SideBySide: {
                if (a->GetOutputHeight() != b->GetOutputHeight()) MathLib::Panic("Invalid network nodes input size");
                break;
            }
            case NeuralNetworkLayerMerge::Stacked: {
                if (a->GetOutputWidth() != b->GetOutputWidth()) MathLib::Panic("Invalid network nodes input size");
                break;
            }
            default: MathLib::Panic("Invalid merge type");
        }
    }
    virtual ~NeuralNetworkLayer(void) override {
        delete a;
        delete b;
    }
    virtual NeuralNetworkNode<T>* Copy(void) const override {
        return new NeuralNetworkLayer(merge, a->Copy(), b->Copy());
    }
    virtual size_t GetInputWidth(void) const override {
        return a->GetInputWidth();
    }
    virtual size_t GetInputHeight(void) const override {
        return a->GetInputHeight();
    }
    virtual size_t GetOutputWidth(void) const override {
        switch (merge) {
            case NeuralNetworkLayerMerge::SideBySide: return a->GetOutputWidth() + b->GetOutputWidth();
            case NeuralNetworkLayerMerge::Stacked: return a->GetOutputWidth();
            default: return 0;
        }
    }
    virtual size_t GetOutputHeight(void) const override {
        switch (merge) {
            case NeuralNetworkLayerMerge::SideBySide: return a->GetOutputHeight();
            case NeuralNetworkLayerMerge::Stacked: return a->GetOutputHeight() + b->GetOutputHeight();
            default: return 0;
        }
    }
    virtual MathLib::Matrix<DualNumber<T>> Evaluate(const MathLib::Matrix<DualNumber<T>>& input) const override {
        const MathLib::Matrix<DualNumber<T>> matA = a->Evaluate(input);
        const MathLib::Matrix<DualNumber<T>> matB = b->Evaluate(input);
        const size_t widthA = matA.GetWidth();
        const size_t heightA = matA.GetHeight();
        const size_t widthB = matB.GetWidth();
        const size_t heightB = matB.GetHeight();
        switch (merge) {
            case NeuralNetworkLayerMerge::SideBySide: {
                MathLib::Matrix<DualNumber<T>> ret = MathLib::Matrix<DualNumber<T>>(widthA + widthB, heightA);
                for (size_t y = 0; y < heightA; y++) {
                    for (size_t x = 0; x < widthA; x++) ret.AtUnsafe(x, y) = matA.AtUnsafe(x, y);
                    for (size_t x = 0; x < widthB; x++) ret.AtUnsafe(x + widthA, y) = matB.AtUnsafe(x, y);
                }
                return ret;
            }
            case NeuralNetworkLayerMerge::Stacked: {
                MathLib::Matrix<DualNumber<T>> ret = MathLib::Matrix<DualNumber<T>>(widthA, heightA + heightB);
                for (size_t y = 0; y < heightA; y++)
                    for (size_t x = 0; x < widthA; x++) ret.AtUnsafe(x, y) = matA.AtUnsafe(x, y);
                for (size_t y = 0; y < heightB; y++)
                    for (size_t x = 0; x < widthA; x++) ret.AtUnsafe(x, y + heightA) = matB.AtUnsafe(x, y);
                return ret;
            }
            default: return MathLib::Matrix<DualNumber<T>>();
        }
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
    NeuralNetworkLayerMerge merge;
};

#endif