#ifndef NearestCentroidClassifier_H
#define NearestCentroidClassifier_H
#include "SupervisedClassifier.hpp"
#include "NearestCentroidClassifierLabel.hpp"
#include <Interfaces/Sequence/TransformSequence.hpp>
#include <FunctionT.hpp>

template <typename T>
struct NearestCentroidClassifier : SupervisedClassifier<T> {
    NearestCentroidClassifier(void) : labels() {}
    NearestCentroidClassifier(const MathLib::Array<NearestCentroidClassifierLabel<T>>& labels) : labels(labels) {}
    virtual size_t Classify(const MathLib::Sequence<T>& data) override {
        return MathLib::TransformSequence<T, NearestCentroidClassifierLabel<T>>(labels,
            MathLib::MakeFunctionT<T, NearestCentroidClassifierLabel<T>>([&data](NearestCentroidClassifierLabel<T> label) -> T {
                return label.GetDistanceSquared(data);
            })
        ).GetIndexOfMin();
    }
    virtual bool Train(const MathLib::Sequence<T>& data, size_t label) override {
        const size_t size = labels.GetSize();
        if (label < size) return labels.AtUnsafe(label).Add(data);
        for (size_t i = size; i < label; i++)
            if (!labels.Add(NearestCentroidClassifierLabel<T>())) return false;
        return labels.Add(NearestCentroidClassifierLabel<T>(data));
    }

    private:
    MathLib::Array<NearestCentroidClassifierLabel<T>> labels;
};

#endif