#ifndef MathLib_Interval_H
#define MathLib_Interval_H
#include "Interfaces/Printable.hpp"
#include "MinMax.hpp"

namespace MathLib {
    template <typename T>
    struct Interval : Printable {
        constexpr Interval(const T& min_ = 0, const T& max_ = 0) : min(min_), max(max_) {
            EmptyBenchmark
        }
        [[nodiscard]] constexpr bool IsEmpty(void) const {
            return FloatsEqual<T>(GetSize(), 0);
        }
        [[nodiscard]] constexpr T GetMin(void) const {
            StartAndReturnFromBenchmark(min);
        }
        [[nodiscard]] constexpr T GetMax(void) const {
            StartAndReturnFromBenchmark(max);
        }
        [[nodiscard]] constexpr T GetSize(void) const {
            StartAndReturnFromBenchmark(max - min);
        }
        [[nodiscard]] constexpr bool Contains(const T& x) const {
            StartAndReturnFromBenchmark(IsBetween(x, min, max));
        }
        [[nodiscard]] T constexpr Clamp(const T& x) const {
            StartAndReturnFromBenchmark(MathLib::Clamp(x, min, max));
        }
        [[nodiscard]] Interval<T> Expand(const T& stepSize) const {
            Interval<T> ret;
            while (ret.GetMin() > GetMin()) ret.min -= stepSize;
            while (ret.GetMax() < GetMax()) ret.max += stepSize;
            return ret;
        }
        [[nodiscard]] Interval<T> Min(const Interval<T>& other) const {
            return Interval<T>(MathLib::Max<T>(min, other.min), MathLib::Min<T>(max, other.max));
        }
        [[nodiscard]] Interval<T> Max(const Interval<T>& other) const {
            return Interval<T>(MathLib::Min<T>(min, other.min), MathLib::Max<T>(max, other.max));
        }
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const String& padding = "") const override {
            return padding + '[' + MathLib::ToString(min) + ", " + MathLib::ToString(max) + ']';
        }

        private:
        T min;
        T max;
    };
}

#endif