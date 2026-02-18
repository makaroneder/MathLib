#ifndef MathLib_Math_UniformDiscreteMeasure_H
#define MathLib_Math_UniformDiscreteMeasure_H
#include "DiscreteMeasure.hpp"

namespace MathLib {
    template <typename T>
    struct UniformDiscreteMeasure : DiscreteMeasure<T> {
        UniformDiscreteMeasure(const Set<T>& events) : DiscreteMeasure<T>(events) {}
        virtual T MassFunction(const T&) const override {
            return 1.0 / this->GetEventCount();
        }
    };
}

#endif