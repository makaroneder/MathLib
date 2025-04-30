#ifndef MathLib_Interval_H
#define MathLib_Interval_H
#include "Interfaces/Printable.hpp"
#include "MinMax.hpp"

namespace MathLib {
    template <typename T>
    struct Interval : Container<T>, Printable {
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
        [[nodiscard]] virtual bool Contains(const T& value) const override {
            StartAndReturnFromBenchmark(IsBetween(value, min, max));
        }
        [[nodiscard]] T constexpr Clamp(const T& value) const {
            StartAndReturnFromBenchmark(MathLib::Clamp(value, min, max));
        }
        [[nodiscard]] Interval<T> Expand(const T& stepSize) const {
            StartBenchmark
            Interval<T> ret;
            while (ret.GetMin() > GetMin()) ret.min -= stepSize;
            while (ret.GetMax() < GetMax()) ret.max += stepSize;
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] Interval<T> Min(const Interval<T>& other) const {
            StartAndReturnFromBenchmark(Interval<T>(MathLib::Max<T>(min, other.min), MathLib::Min<T>(max, other.max)));
        }
        [[nodiscard]] Interval<T> Max(const Interval<T>& other) const {
            StartAndReturnFromBenchmark(Interval<T>(MathLib::Min<T>(min, other.min), MathLib::Max<T>(max, other.max)));
        }
        [[nodiscard]] T Random(void) const {
            StartAndReturnFromBenchmark(RandomNumber<T>(min, max));
        }
        [[nodiscard]] bool operator==(const Interval<T>& other) const {
            StartAndReturnFromBenchmark(FloatsEqual<T>(min, other.min) && FloatsEqual<T>(max, other.max));
        }
        [[nodiscard]] bool operator!=(const Interval<T>& other) const {
            StartAndReturnFromBenchmark(!(*this == other));
        }
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override {
            return CollectionToString(padding) + '[' + MathLib::ToString(min) + ", " + MathLib::ToString(max) + ']';
        }

        private:
        T min;
        T max;
    };
}

#endif