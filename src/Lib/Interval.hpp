#ifndef MathLib_Interval_H
#define MathLib_Interval_H
#include "Typedefs.hpp"

namespace MathLib {
    template <typename T>
    struct Interval : Allocatable {
        Interval(const T& min_ = 0, const T& max_ = 0) : min(min_), max(max_) {
            EmptyBenchmark
        }
        T GetMin(void) const {
            StartBenchmark
            ReturnFromBenchmark(min);
        }
        T GetMax(void) const {
            StartBenchmark
            ReturnFromBenchmark(max);
        }
        T GetSize(void) const {
            StartBenchmark
            ReturnFromBenchmark(max - min);
        }
        bool Contains(const T& x) const {
            StartBenchmark
            ReturnFromBenchmark(IsBetween(x, min, max));
        }
        T Clamp(const T& x) const {
            StartBenchmark
            if (x < min) ReturnFromBenchmark(min)
            else if (x > max) ReturnFromBenchmark(max)
            else ReturnFromBenchmark(x)
        }

        private:
        T min;
        T max;
    };
}

#endif