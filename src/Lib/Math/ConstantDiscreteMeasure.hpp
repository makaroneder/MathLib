#ifndef ConstantDiscreteMeasure_H
#define ConstantDiscreteMeasure_H
#include "DiscreteMeasure.hpp"

namespace MathLib {
    template <typename T>
    struct ConstantDiscreteMeasure : DiscreteMeasure<T> {
        ConstantDiscreteMeasure(const MathLib::Set<T>& events, const T& measure) : DiscreteMeasure<T>(events), measure(measure) {
            EmptyBenchmark
        }
        virtual T MassFunction(const T&) const override {
            StartAndReturnFromBenchmark(measure);
        }

        private:
        T measure;
    };
}

#endif