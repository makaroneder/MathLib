#ifndef MathLib_Interval_H
#define MathLib_Interval_H
#include "Typedefs.hpp"
#include "MinMax.hpp"

namespace MathLib {
    template <typename T>
    struct Interval : Allocatable {
        constexpr Interval(const T& min_ = 0, const T& max_ = 0) : min(min_), max(max_) {
            EmptyBenchmark
        }
        [[nodiscard]] constexpr T GetMin(void) const {
            StartBenchmark
            ReturnFromBenchmark(min);
        }
        [[nodiscard]] constexpr T GetMax(void) const {
            StartBenchmark
            ReturnFromBenchmark(max);
        }
        [[nodiscard]] constexpr T GetSize(void) const {
            StartBenchmark
            ReturnFromBenchmark(max - min);
        }
        [[nodiscard]] constexpr bool Contains(const T& x) const {
            StartBenchmark
            ReturnFromBenchmark(IsBetween(x, min, max));
        }
        [[nodiscard]] T constexpr Clamp(const T& x) const {
            StartBenchmark
            ReturnFromBenchmark(MathLib::Clamp(x, min, max));
        }

        private:
        T min;
        T max;
    };
}

#endif