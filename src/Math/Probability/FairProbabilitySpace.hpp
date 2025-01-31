#ifndef FairProbabilitySpace_H
#define FairProbabilitySpace_H
#include "DiscreteProbabilitySpace.hpp"

template <typename T, typename Number>
struct FairProbabilitySpace : DiscreteProbabilitySpace<T, Number> {
    FairProbabilitySpace(const MathLib::Set<T>& events) : DiscreteProbabilitySpace<T, Number>(events) {}
    virtual Number ProbabilityMassFunction(const T&) const override {
        return 1.0 / this->GetEventCount();
    }
};

#endif