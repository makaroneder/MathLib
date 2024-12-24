#ifndef MathLib_Interfaces_Collection_H
#define MathLib_Interfaces_Collection_H
#include "../Iterator.hpp"

namespace MathLib {
    template <typename T>
    struct Collection : Iteratable<T> {
        virtual T At(size_t index) const = 0;
        virtual T& At(size_t index) = 0;
        virtual bool Add(const T& val) = 0;
        virtual size_t GetSize(void) const = 0;
        virtual const T* GetValue(void) const = 0;
        bool IsEmpty(void) const {
            return !GetSize();
        }
        virtual Iterator<const T> begin(void) const override {
            return Iterator<const T>(GetValue());
        }
        virtual Iterator<const T> end(void) const override {
            return Iterator<const T>(GetValue() + GetSize());
        }
        virtual Iterator<T> begin(void) override {
            return Iterator<T>((T*)GetValue());
        }
        virtual Iterator<T> end(void) override {
            return Iterator<T>((T*)GetValue() + GetSize());
        }
        bool Contains(const T& value) const {
            for (size_t i = 0; i < GetSize(); i++)
                if (At(i) == value) return true;
            return false;
        }
        bool Contains(const Collection<T>& other) const {
            return Find(other, 0) != SIZE_MAX;
        }
        size_t Find(const Collection<T>& other, size_t start = 0) const {
            if (GetSize() < other.GetSize()) return SIZE_MAX;
            for (size_t i = start; i < GetSize() - other.GetSize(); i++) {
                bool found = true;
                for (size_t j = 0; j < other.GetSize() && found; j++)
                    if (At(i + j) != other.At(j)) found = false;
                if (found) return i;
            }
            return SIZE_MAX;
        }
        T operator[](size_t index) const {
            return At(index);
        }
        T& operator[](size_t index) {
            return At(index);
        }
        bool operator==(const Collection<T>& other) const {
            if (GetSize() != other.GetSize()) return false;
            for (size_t i = 0; i < GetSize(); i++)
                if (At(i) != other.At(i)) return false;
            return true;
        }
        bool operator!=(const Collection<T>& other) const {
            return !(*this == other);
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