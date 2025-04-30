#ifndef MathLib_Interfaces_Sequence_H
#define MathLib_Interfaces_Sequence_H
#include "../MinMax.hpp"
#include "Container.hpp"
#include "Function.hpp"
#include <stdint.h>

namespace MathLib {
    template <typename T>
    struct Sequence : Container<T>, Function<T, size_t> {
        [[nodiscard]] virtual size_t GetSize(void) const = 0;
        [[nodiscard]] virtual T At(size_t index) const = 0;
        [[nodiscard]] bool IsEmpty(void) const {
            return !GetSize();
        }
        [[nodiscard]] virtual T Invoke(const void*, size_t index) const override {
            return At(index);
        }
        [[nodiscard]] T operator[](size_t index) const {
            return At(index);
        }
        [[nodiscard]] bool IsSorted(const Function<bool, T, T>& compare) const {
            const size_t size = GetSize();
            for (size_t i = 1; i < size; i++)
                if (compare(At(i - 1), At(i))) return false;
            return true;
        }
        [[nodiscard]] bool HasDuplicate(void) const {
            const size_t size = GetSize();
            for (size_t i = 1; i < size; i++)
                if (Find(At(i - 1), i) != SIZE_MAX) return true;
            return false;
        }
        [[nodiscard]] bool Contains(const T& value, size_t start) const {
            const size_t size = GetSize();
            for (size_t i = start; i < size; i++)
                if (At(i) == value) return true;
            return false;
        }
        [[nodiscard]] virtual bool Contains(const T& value) const override {
            return Contains(value, 0);
        }
        [[nodiscard]] bool Contains(const Sequence<T>& other, size_t start = 0) const {
            return Find(other, start) != SIZE_MAX;
        }
        [[nodiscard]] bool StartsWith(const T& value) const {
            return !IsEmpty() && At(0) == value;
        }
        [[nodiscard]] bool StartsWith(const Sequence<T>& other) const {
            const size_t size = other.GetSize();
            if (GetSize() < size) return false;
            for (size_t i = 0; i < size; i++)
                if (At(i) != other.At(i)) return false;
            return true;
        }
        [[nodiscard]] size_t Find(const T& value, size_t start = 0) const {
            const size_t size = GetSize();
            for (size_t i = start; i < size; i++)
                if (At(i) == value) return i;
            return SIZE_MAX;
        }
        [[nodiscard]] size_t Find(const Sequence<T>& other, size_t start = 0) const {
            const size_t size1 = GetSize();
            const size_t size2 = other.GetSize();
            if (size1 < size2) return SIZE_MAX;
            for (size_t i = start; i < size1 - size2; i++) {
                bool found = true;
                for (size_t j = 0; j < size2 && found; j++)
                    if (At(i + j) != other.At(j)) found = false;
                if (found) return i;
            }
            return SIZE_MAX;
        }
        [[nodiscard]] size_t Find(const Function<bool, T>& function, size_t start = 0) const {
            const size_t size = GetSize();
            for (size_t i = start; i < size; i++)
                if (function(At(i))) return i;
            return SIZE_MAX;
        }
        [[nodiscard]] size_t GetCountOf(const T& x) const {
            const size_t size = GetSize();
            size_t ret = 0;
            for (size_t i = 0; i < size; i++) ret += At(i) == x;
            return ret;
        }
        [[nodiscard]] size_t GetHammingDistance(const Sequence<T>& other) const {
            const size_t size = GetSize();
            if (size != other.GetSize()) return SIZE_MAX;
            size_t ret = 0;
            for (size_t i = 0; i < size; i++) ret += At(i) != other.At(i);
            return ret;
        }
        [[nodiscard]] T Sum(const T& t0) const {
            T ret = t0;
            const size_t size = GetSize();
            for (size_t i = 0; i < size; i++) ret += At(i);
            return ret;
        }
        [[nodiscard]] T Product(const T& t0) const {
            T ret = t0;
            const size_t size = GetSize();
            for (size_t i = 0; i < size; i++) ret *= At(i);
            return ret;
        }
        void Foreach(const Function<void, T>& function) const {
            const size_t size = GetSize();
            for (size_t i = 0; i < size; i++) function(At(i));
        }
        template <typename Ret>
        [[nodiscard]] Ret Foreach(const Function<Ret, Ret, T>& function, const Ret& startRet) const {
            Ret ret = startRet;
            const size_t size = GetSize();
            for (size_t i = 0; i < size; i++) ret = function(ret, At(i));
            return ret;
        }
        [[nodiscard]] bool operator==(const Sequence<T>& other) const {
            const size_t size = GetSize();
            if (size != other.GetSize()) return false;
            for (size_t i = 0; i < size; i++)
                if (At(i) != other.At(i)) return false;
            return true;
        }
        [[nodiscard]] bool operator!=(const Sequence<T>& other) const {
            return !(*this == other);
        }
        [[nodiscard]] bool operator<(const Sequence<T>& other) const {
            size_t size = Min<size_t>(GetSize(), other.GetSize());
            for (size_t i = 0; i < size; i++)
                if (At(i) != other.At(i)) return At(i) < other.At(i);
            return GetSize() < other.GetSize();
        }
    };
}

#endif