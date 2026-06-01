#ifndef SupervisedClassifier_H
#define SupervisedClassifier_H
#include <Interfaces/Sequence/Sequence.hpp>

template <typename T>
struct SupervisedClassifier {
    virtual size_t Classify(const MathLib::Sequence<T>& data) = 0;
    virtual bool Train(const MathLib::Sequence<T>& data, size_t label) = 0;
};

#endif