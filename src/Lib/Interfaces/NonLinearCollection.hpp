#ifndef MathLib_Interfaces_NonLinearCollection_H
#define MathLib_Interfaces_NonLinearCollection_H
#include "WritableSequence.hpp"

namespace MathLib {
    template <typename T>
    struct NonLinearCollection : WritableSequence<T> {
        using Sequence<T>::At;
        using Sequence<T>::operator[];
        [[nodiscard]] virtual T& At(size_t index) = 0;
        [[nodiscard]] virtual bool Set(size_t index, const T& val) override {
            if (index >= this->GetSize()) return false;
            At(index) = val;
            return true;
        }
        void Foreach(const Function<void, T&>& function) {
            const size_t size = this->GetSize();
            for (size_t i = 0; i < size; i++) function(At(i));
        }
        [[nodiscard]] T& operator[](size_t index) {
            return At(index);
        }
    };
}

#endif