#ifndef ConstantDiscreteMeasure_H
#define ConstantDiscreteMeasure_H
#include "DiscreteMeasure.hpp"

namespace MathLib {
    template <typename T>
    struct ConstantDiscreteMeasure : DiscreteMeasure<T> {
        ConstantDiscreteMeasure(const Set<T>& events, const T& measure) : DiscreteMeasure<T>(events), measure(measure) {}
        virtual T MassFunction(const T&) const override {
            return measure;
        }

        private:
        T measure;
    };
}

#endif