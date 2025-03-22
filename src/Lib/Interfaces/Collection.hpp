#ifndef MathLib_Interfaces_Collection_H
#define MathLib_Interfaces_Collection_H
#include "Sequence.hpp"
#include "../Iteratable.hpp"
#include "../FunctionT.hpp"
#include "../MinMax.hpp"
#include "../Swap.hpp"

namespace MathLib {
    template <typename T>
    struct Collection : Sequence<T>, Iteratable<T> {
        using Sequence<T>::At;
        using Sequence<T>::operator[];
        [[nodiscard]] virtual T& At(size_t index) = 0;
        [[nodiscard]] virtual bool Add(const T& val) = 0;
        [[nodiscard]] virtual const T* GetValue(void) const = 0;
        [[nodiscard]] virtual Iterator<const T> begin(void) const override {
            return Iterator<const T>(GetValue());
        }
        [[nodiscard]] virtual Iterator<const T> end(void) const override {
            return Iterator<const T>(GetValue() + this->GetSize());
        }
        [[nodiscard]] virtual Iterator<T> begin(void) override {
            return Iterator<T>((T*)GetValue());
        }
        [[nodiscard]] virtual Iterator<T> end(void) override {
            return Iterator<T>((T*)GetValue() + this->GetSize());
        }
        [[nodiscard]] bool Reverse(size_t start, size_t end) {
            if (end > this->GetSize() || !end) return false;
            while (start < end) Swap<T>(At(start++), At(--end));
            return true;
        }
        [[nodiscard]] T& operator[](size_t index) {
            return At(index);
        }
        Collection<T>& operator+=(const T& val) {
            Add(val);
            return *this;
        }
        Collection<T>& operator+=(const Sequence<T>& other) {
            other.Foreach(MakeFunctionT<void, T>(nullptr, [this] (const void*, T x) -> void {
                Add(x);
            }));
            return *this;
        }
    };
}

#endif