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
            StartBenchmark
            if (index >= this->GetSize()) ReturnFromBenchmark(false);
            At(index) = val;
            ReturnFromBenchmark(true);
        }
        void Foreach(const Function<void, T&>& function) {
            StartBenchmark
            const size_t size = this->GetSize();
            for (size_t i = 0; i < size; i++) function(At(i));
            EndBenchmark
        }
        [[nodiscard]] T& operator[](size_t index) {
            StartAndReturnFromBenchmark(At(index));
        }
    };
}

#endif