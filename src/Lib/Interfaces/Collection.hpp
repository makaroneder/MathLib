#ifndef MathLib_Interfaces_Collection_H
#define MathLib_Interfaces_Collection_H
#include "../Iteratable.hpp"
#include "../MinMax.hpp"
#include "../Swap.hpp"

namespace MathLib {
    template <typename T>
    struct Collection : Iteratable<T> {
        [[nodiscard]] virtual T At(size_t index) const = 0;
        [[nodiscard]] virtual T& At(size_t index) = 0;
        [[nodiscard]] virtual bool Add(const T& val) = 0;
        [[nodiscard]] virtual size_t GetSize(void) const = 0;
        [[nodiscard]] virtual const T* GetValue(void) const = 0;
        [[nodiscard]] bool IsEmpty(void) const {
            return !GetSize();
        }
        [[nodiscard]] virtual Iterator<const T> begin(void) const override {
            return Iterator<const T>(GetValue());
        }
        [[nodiscard]] virtual Iterator<const T> end(void) const override {
            return Iterator<const T>(GetValue() + GetSize());
        }
        [[nodiscard]] virtual Iterator<T> begin(void) override {
            return Iterator<T>((T*)GetValue());
        }
        [[nodiscard]] virtual Iterator<T> end(void) override {
            return Iterator<T>((T*)GetValue() + GetSize());
        }
        [[nodiscard]] bool Contains(const T& value) const {
            for (size_t i = 0; i < GetSize(); i++)
                if (At(i) == value) return true;
            return false;
        }
        [[nodiscard]] bool Contains(const Collection<T>& other) const {
            return Find(other, 0) != SIZE_MAX;
        }
        [[nodiscard]] size_t Find(const T& value, size_t start = 0) const {
            for (size_t i = start; i < GetSize(); i++)
                if (At(i) == value) return i;
            return SIZE_MAX;
        }
        [[nodiscard]] size_t Find(const Collection<T>& other, size_t start = 0) const {
            if (GetSize() < other.GetSize()) return SIZE_MAX;
            for (size_t i = start; i < GetSize() - other.GetSize(); i++) {
                bool found = true;
                for (size_t j = 0; j < other.GetSize() && found; j++)
                    if (At(i + j) != other.At(j)) found = false;
                if (found) return i;
            }
            return SIZE_MAX;
        }
        [[nodiscard]] bool Reverse(size_t start, size_t end) {
            if (end >= GetSize() || !end) return false;
            while (start < end) Swap<T>(At(start++), At(end--));
            return true;
        }
        [[nodiscard]] T operator[](size_t index) const {
            return At(index);
        }
        [[nodiscard]] T& operator[](size_t index) {
            return At(index);
        }
        [[nodiscard]] bool operator==(const Collection<T>& other) const {
            if (GetSize() != other.GetSize()) return false;
            for (size_t i = 0; i < GetSize(); i++)
                if (At(i) != other.At(i)) return false;
            return true;
        }
        [[nodiscard]] bool operator!=(const Collection<T>& other) const {
            return !(*this == other);
        }
        [[nodiscard]] bool operator<(const Collection<T>& other) const {
            size_t size = Min<size_t>(GetSize(), other.GetSize());
            for (size_t i = 0; i < size; i++)
                if (At(i) != other.At(i)) return At(i) < other.At(i);
            return GetSize() < other.GetSize();
        }
        Collection<T>& operator+=(const T& val) {
            Add(val);
            return *this;
        }
        Collection<T>& operator+=(const Collection<T>& other) {
            for (size_t i = 0; i < other.GetSize(); i++) Add(other.At(i));
            return *this;
        }
    };
}

#endif