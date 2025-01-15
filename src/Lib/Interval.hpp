#ifndef MathLib_Interval_H
#define MathLib_Interval_H
#include "Typedefs.hpp"
#include "MinMax.hpp"

namespace MathLib {
    template <typename T>
    struct Interval : Allocatable {
        Interval(const T& min_ = 0, const T& max_ = 0) : min(min_), max(max_) {
            EmptyBenchmark
        }
        [[nodiscard]] T GetMin(void) const {
            StartBenchmark
            ReturnFromBenchmark(min);
        }
        [[nodiscard]] T GetMax(void) const {
            StartBenchmark
            ReturnFromBenchmark(max);
        }
        [[nodiscard]] T GetSize(void) const {
            StartBenchmark
            ReturnFromBenchmark(max - min);
        }
        [[nodiscard]] bool Contains(const T& x) const {
            StartBenchmark
            ReturnFromBenchmark(IsBetween(x, min, max));
        }
        [[nodiscard]] T Clamp(const T& x) const {
            StartBenchmark
            ReturnFromBenchmark(MathLib::Clamp(x, min, max));
        }

        private:
        T min;
        T max;
    };
}

#endif