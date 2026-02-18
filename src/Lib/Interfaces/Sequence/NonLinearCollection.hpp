#ifndef MathLib_Interfaces_Sequence_NonLinearCollection_H
#define MathLib_Interfaces_Sequence_NonLinearCollection_H
#include "WritableSequence.hpp"

namespace MathLib {
    template <typename T>
    struct NonLinearCollection : WritableSequence<T> {
        using Sequence<T>::At;
        using Sequence<T>::AtUnsafe;
        using Sequence<T>::operator[];
        [[nodiscard]] virtual T& AtUnsafe(size_t index) = 0;
        [[nodiscard]] virtual bool SetUnsafe(size_t index, const T& val) override {
            AtUnsafe(index) = val;
            return true;
        }
        [[nodiscard]] T& At(size_t index) {
            if (index < this->GetSize()) return AtUnsafe(index);
            Panic("Index out of bounds");
        }
        void Foreach(const Function<void, T&>& function) {
            const size_t size = this->GetSize();
            for (size_t i = 0; i < size; i++) function(AtUnsafe(i));
        }
        [[nodiscard]] T& operator[](size_t index) {
            return At(index);
        }
    };
}

#endif