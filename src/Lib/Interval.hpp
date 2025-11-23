#ifndef MathLib_Interval_H
#define MathLib_Interval_H
#include "Interfaces/Printable.hpp"
#include "MinMax.hpp"

namespace MathLib {
    template <typename T>
    struct Interval : Container<T>, Printable {
        constexpr Interval(const T& min_ = 0, const T& max_ = 0) : min(min_), max(max_) {}
        [[nodiscard]] constexpr bool IsEmpty(void) const {
            return FloatsEqual<T>(GetSize(), 0);
        }
        [[nodiscard]] constexpr T GetMin(void) const {
            return min;
        }
        [[nodiscard]] constexpr T GetMax(void) const {
            return max;
        }
        [[nodiscard]] constexpr T GetSize(void) const {
            return max - min;
        }
        [[nodiscard]] virtual bool Contains(const T& value) const override {
            return IsBetween(value, min, max);
        }
        [[nodiscard]] T constexpr Clamp(const T& value) const {
            return MathLib::Clamp(value, min, max);
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
        [[nodiscard]] T Random(void) const {
            return RandomNumber<T>(min, max);
        }
        [[nodiscard]] bool operator==(const Interval<T>& other) const {
            return FloatsEqual<T>(min, other.min) && FloatsEqual<T>(max, other.max);
        }
        [[nodiscard]] bool operator!=(const Interval<T>& other) const {
            return !(*this == other);
        }
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override {
            return CollectionToString(padding) + '[' + Formatter<T>::ToString(min) + ", " + Formatter<T>::ToString(max) + ']';
        }

        T min;
        T max;
    };
}

#endif