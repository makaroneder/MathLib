#ifndef MathLib_Interfaces_Sequence_H
#define MathLib_Interfaces_Sequence_H
#include "../Function.hpp"
#include "../Container.hpp"
#include "../../MinMax.hpp"
#include "../Comparable.hpp"
#include <stdint.h>

namespace MathLib {
    [[noreturn]] void Panic(const char* str);
    template <typename T>
    struct Sequence;
    template <typename T>
    struct Sequence : Container<T>, Function<T, size_t>, Comparable<Sequence<T>> {
        [[nodiscard]] virtual size_t GetSize(void) const = 0;
        [[nodiscard]] virtual T AtUnsafe(size_t index) const = 0;
        [[nodiscard]] T At(size_t index) const {
            if (index < GetSize()) return AtUnsafe(index);
            Panic("Index out of bounds");
        }
        [[nodiscard]] virtual bool Contains(const T& value) const override {
            return Contains(value, 0);
        }
        [[nodiscard]] virtual T Invoke(size_t index) const override {
            return At(index);
        }
        [[nodiscard]] bool IsEmpty(void) const {
            return !GetSize();
        }
        [[nodiscard]] T operator[](size_t index) const {
            return At(index);
        }
        template <typename F>
        [[nodiscard]] F LeftFold(F initialValue, const Function<F, F, T>& func) const {
            const size_t size = GetSize();
            F ret = initialValue;
            for (size_t i = 0; i < size; i++) ret = func(ret, AtUnsafe(i));
            return ret;
        }
        template <typename F>
        [[nodiscard]] F RightFold(F initialValue, const Function<F, T, F>& func) const {
            const size_t size = GetSize();
            F ret = initialValue;
            for (size_t i = size; i; i--) ret = func(AtUnsafe(i - 1), ret);
            return ret;
        }
        [[nodiscard]] bool IsSorted(const Function<bool, T, T>& compare) const {
            const size_t size = GetSize();
            for (size_t i = 1; i < size; i++)
                if (compare(AtUnsafe(i - 1), AtUnsafe(i))) return false;
            return true;
        }
        [[nodiscard]] bool HasDuplicate(void) const {
            const size_t size = GetSize();
            for (size_t i = 1; i < size; i++)
                if (Contains(AtUnsafe(i - 1), i)) return true;
            return false;
        }
        [[nodiscard]] bool Contains(const T& value, size_t start) const {
            const size_t size = GetSize();
            for (size_t i = start; i < size; i++)
                if (AtUnsafe(i) == value) return true;
            return false;
        }
        [[nodiscard]] bool Contains(const Sequence<T>& other, size_t start = 0) const {
            return Find(other, start) != SIZE_MAX;
        }
        [[nodiscard]] bool StartsWith(const T& value) const {
            return GetSize() && AtUnsafe(0) == value;
        }
        [[nodiscard]] bool StartsWith(const Sequence<T>& other) const {
            const size_t size = other.GetSize();
            if (GetSize() < size) return false;
            for (size_t i = 0; i < size; i++)
                if (AtUnsafe(i) != other.AtUnsafe(i)) return false;
            return true;
        }
        [[nodiscard]] size_t Find(const T& value, size_t start = 0) const {
            const size_t size = GetSize();
            for (size_t i = start; i < size; i++)
                if (AtUnsafe(i) == value) return i;
            return SIZE_MAX;
        }
        [[nodiscard]] size_t Find(const Sequence<T>& other, size_t start = 0) const {
            const size_t size1 = GetSize();
            const size_t size2 = other.GetSize();
            if (size1 < size2) return SIZE_MAX;
            if (!start && size1 == size2) return *this == other ? 0 : SIZE_MAX;
            for (size_t i = start; i < size1 - size2; i++) {
                bool found = true;
                for (size_t j = 0; j < size2; j++) {
                    if (AtUnsafe(i + j) == other.AtUnsafe(j)) continue;
                    found = false;
                    break;
                }
                if (found) return i;
            }
            return SIZE_MAX;
        }
        [[nodiscard]] size_t Find(const Function<bool, T>& function, size_t start = 0) const {
            const size_t size = GetSize();
            for (size_t i = start; i < size; i++)
                if (function(AtUnsafe(i))) return i;
            return SIZE_MAX;
        }
        [[nodiscard]] size_t GetCountOf(const T& x) const {
            const size_t size = GetSize();
            size_t ret = 0;
            for (size_t i = 0; i < size; i++) ret += AtUnsafe(i) == x;
            return ret;
        }
        [[nodiscard]] size_t GetHammingDistance(const Sequence<T>& other) const {
            const size_t size = GetSize();
            if (size != other.GetSize()) return SIZE_MAX;
            size_t ret = 0;
            for (size_t i = 0; i < size; i++) ret += AtUnsafe(i) != other.AtUnsafe(i);
            return ret;
        }
        [[nodiscard]] T GetMin(void) const {
            const size_t size = GetSize();
            if (!size) return T();
            T ret = AtUnsafe(0);
            for (size_t i = 1; i < size; i++) {
                const T tmp = AtUnsafe(i);
                if (tmp < ret) ret = tmp;
            }
            return ret;
        }
        [[nodiscard]] T GetMax(void) const {
            const size_t size = GetSize();
            if (!size) return T();
            T ret = AtUnsafe(0);
            for (size_t i = 1; i < size; i++) {
                const T tmp = AtUnsafe(i);
                if (tmp > ret) ret = tmp;
            }
            return ret;
        }
        [[nodiscard]] T HarmonicMean(void) const {
            const size_t size = GetSize();
            if (!size) return T();
            T ret = 1 / AtUnsafe(0);
            for (size_t i = 1; i < size; i++) ret += 1 / AtUnsafe(i);
            return size / ret;
        }
        [[nodiscard]] T ContraharmonicMean(void) const {
            const size_t size = GetSize();
            if (!size) return T();
            T b = AtUnsafe(0);
            T a = b * b;
            for (size_t i = 1; i < size; i++) {
                const T tmp = AtUnsafe(i);
                a += tmp * tmp;
                b += tmp;
            }
            return a / b;
        }
        [[nodiscard]] T Median(const Function<bool, T, T>& compare) const {
            if (!IsSorted(compare)) return T();
            const size_t size = GetSize();
            if (!size) return T();
            if (size == 1) return AtUnsafe(0);
            if (size % 2) return (AtUnsafe(size / 2) + At(size / 2 + 1)) / 2;
            return AtUnsafe(size / 2);
        }
        [[nodiscard]] T MidRange(void) const {
            return (GetMax() + GetMin()) / 2;
        }
        [[nodiscard]] T ArithmeticMean(void) const {
            const size_t size = GetSize();
            if (!size) return T();
            T ret = AtUnsafe(0);
            for (size_t i = 1; i < size; i++) ret += AtUnsafe(i);
            return ret / size;
        }
        [[nodiscard]] T Variance(void) const {
            const size_t size = GetSize();
            if (!size) return T();
            const T mean = ArithmeticMean();
            T ret = AtUnsafe(0) - mean;
            ret *= ret;
            for (size_t i = 1; i < size; i++) {
                const T tmp = AtUnsafe(i) - mean;
                ret += tmp * tmp;
            }
            return ret / size;
        }
        [[nodiscard]] T Sum(const T& t0) const {
            const size_t size = GetSize();
            T ret = t0;
            for (size_t i = 0; i < size; i++) ret += AtUnsafe(i);
            return ret;
        }
        [[nodiscard]] T Product(const T& t0) const {
            const size_t size = GetSize();
            T ret = t0;
            for (size_t i = 0; i < size; i++) ret *= AtUnsafe(i);
            return ret;
        }
        void Foreach(const Function<void, T>& function) const {
            const size_t size = GetSize();
            for (size_t i = 0; i < size; i++) function(AtUnsafe(i));
        }
        template <typename Ret>
        [[nodiscard]] Ret Foreach(const Function<Ret, Ret, T>& function, const Ret& startRet) const {
            Ret ret = startRet;
            const size_t size = GetSize();
            for (size_t i = 0; i < size; i++) ret = function(ret, AtUnsafe(i));
            return ret;
        }

        protected:
        [[nodiscard]] virtual bool Equals(const Sequence<T>& other) const override {
            const size_t size = GetSize();
            if (size != other.GetSize()) return false;
            for (size_t i = 0; i < size; i++)
                if (AtUnsafe(i) != other.AtUnsafe(i)) return false;
            return true;
        }
    };
}

#endif