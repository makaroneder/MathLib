#ifndef MathLib_Interfaces_Sequence_H
#define MathLib_Interfaces_Sequence_H
#include "Function.hpp"
#include "Container.hpp"
#include "../MinMax.hpp"
#include <stdint.h>

namespace MathLib {
    template <typename T>
    struct Sequence : Container<T>, Function<T, size_t> {
        [[nodiscard]] virtual size_t GetSize(void) const = 0;
        [[nodiscard]] virtual T At(size_t index) const = 0;
        [[nodiscard]] bool IsEmpty(void) const {
            StartAndReturnFromBenchmark(!GetSize());
        }
        [[nodiscard]] virtual T Invoke(const void*, size_t index) const override {
            StartAndReturnFromBenchmark(At(index));
        }
        [[nodiscard]] T operator[](size_t index) const {
            StartAndReturnFromBenchmark(At(index));
        }
        [[nodiscard]] T Fold(T initialValue, const Function<T, T, T>& func, bool left) const {
            StartBenchmark
            const size_t size = GetSize();
            T ret = initialValue;
            if (left) for (size_t i = 0; i < size; i++) ret = func(ret, At(i));
            else for (size_t i = size; i; i--) ret = func(At(i - 1), ret);
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] bool IsSorted(const Function<bool, T, T>& compare) const {
            StartBenchmark
            const size_t size = GetSize();
            for (size_t i = 1; i < size; i++)
                if (compare(At(i - 1), At(i))) ReturnFromBenchmark(false);
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] bool HasDuplicate(void) const {
            StartBenchmark
            const size_t size = GetSize();
            for (size_t i = 1; i < size; i++)
                if (Find(At(i - 1), i) != SIZE_MAX) ReturnFromBenchmark(true);
            ReturnFromBenchmark(false);
        }
        [[nodiscard]] bool Contains(const T& value, size_t start) const {
            StartBenchmark
            const size_t size = GetSize();
            for (size_t i = start; i < size; i++)
                if (At(i) == value) ReturnFromBenchmark(true);
            ReturnFromBenchmark(false);
        }
        [[nodiscard]] virtual bool Contains(const T& value) const override {
            StartAndReturnFromBenchmark(Contains(value, 0));
        }
        [[nodiscard]] bool Contains(const Sequence<T>& other, size_t start = 0) const {
            StartAndReturnFromBenchmark(Find(other, start) != SIZE_MAX);
        }
        [[nodiscard]] bool StartsWith(const T& value) const {
            StartAndReturnFromBenchmark(!IsEmpty() && At(0) == value);
        }
        [[nodiscard]] bool StartsWith(const Sequence<T>& other) const {
            StartBenchmark
            const size_t size = other.GetSize();
            if (GetSize() < size) ReturnFromBenchmark(false);
            for (size_t i = 0; i < size; i++)
                if (At(i) != other.At(i)) ReturnFromBenchmark(false);
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] size_t Find(const T& value, size_t start = 0) const {
            StartBenchmark
            const size_t size = GetSize();
            for (size_t i = start; i < size; i++)
                if (At(i) == value) ReturnFromBenchmark(i);
            ReturnFromBenchmark(SIZE_MAX);
        }
        [[nodiscard]] size_t Find(const Sequence<T>& other, size_t start = 0) const {
            StartBenchmark
            const size_t size1 = GetSize();
            const size_t size2 = other.GetSize();
            if (size1 < size2) ReturnFromBenchmark(SIZE_MAX);
            if (!start && size1 == size2) ReturnFromBenchmark(*this == other ? 0 : SIZE_MAX);
            for (size_t i = start; i < size1 - size2; i++) {
                bool found = true;
                for (size_t j = 0; j < size2 && found; j++)
                    if (At(i + j) != other.At(j)) found = false;
                if (found) ReturnFromBenchmark(i);
            }
            ReturnFromBenchmark(SIZE_MAX);
        }
        [[nodiscard]] size_t Find(const Function<bool, T>& function, size_t start = 0) const {
            StartBenchmark
            const size_t size = GetSize();
            for (size_t i = start; i < size; i++)
                if (function(At(i))) ReturnFromBenchmark(i);
            ReturnFromBenchmark(SIZE_MAX);
        }
        [[nodiscard]] size_t GetCountOf(const T& x) const {
            StartBenchmark
            const size_t size = GetSize();
            size_t ret = 0;
            for (size_t i = 0; i < size; i++) ret += At(i) == x;
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] size_t GetHammingDistance(const Sequence<T>& other) const {
            StartBenchmark
            const size_t size = GetSize();
            if (size != other.GetSize()) ReturnFromBenchmark(SIZE_MAX);
            size_t ret = 0;
            for (size_t i = 0; i < size; i++) ret += At(i) != other.At(i);
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] T Sum(const T& t0) const {
            StartBenchmark
            T ret = t0;
            const size_t size = GetSize();
            for (size_t i = 0; i < size; i++) ret += At(i);
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] T Product(const T& t0) const {
            StartBenchmark
            T ret = t0;
            const size_t size = GetSize();
            for (size_t i = 0; i < size; i++) ret *= At(i);
            ReturnFromBenchmark(ret);
        }
        void Foreach(const Function<void, T>& function) const {
            StartBenchmark
            const size_t size = GetSize();
            for (size_t i = 0; i < size; i++) function(At(i));
            EndBenchmark
        }
        template <typename Ret>
        [[nodiscard]] Ret Foreach(const Function<Ret, Ret, T>& function, const Ret& startRet) const {
            StartBenchmark
            Ret ret = startRet;
            const size_t size = GetSize();
            for (size_t i = 0; i < size; i++) ret = function(ret, At(i));
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] bool operator==(const Sequence<T>& other) const {
            StartBenchmark
            const size_t size = GetSize();
            if (size != other.GetSize()) ReturnFromBenchmark(false);
            for (size_t i = 0; i < size; i++)
                if (At(i) != other.At(i)) ReturnFromBenchmark(false);
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] bool operator!=(const Sequence<T>& other) const {
            StartAndReturnFromBenchmark(!(*this == other));
        }
        [[nodiscard]] bool operator<(const Sequence<T>& other) const {
            StartBenchmark
            size_t size = Min<size_t>(GetSize(), other.GetSize());
            for (size_t i = 0; i < size; i++)
                if (At(i) != other.At(i)) ReturnFromBenchmark(At(i) < other.At(i));
            ReturnFromBenchmark(GetSize() < other.GetSize());
        }
    };
}

#endif