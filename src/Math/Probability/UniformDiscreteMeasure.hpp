#ifndef UniformDiscreteMeasure_H
#define UniformDiscreteMeasure_H
#include "DiscreteMeasure.hpp"

template <typename T, typename Number>
struct UniformDiscreteMeasure : DiscreteMeasure<T, Number> {
    UniformDiscreteMeasure(const MathLib::Set<T>& events) : DiscreteMeasure<T, Number>(events) {}
    virtual Number MassFunction(const T&) const override {
        return 1.0 / this->GetEventCount();
    }
};

#endif