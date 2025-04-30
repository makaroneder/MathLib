#ifndef MathLib_SingleTypePair_H
#define MathLib_SingleTypePair_H
#include "Pair.hpp"

namespace MathLib {
    template <typename T>
    struct SingleTypePair : Pair<T, T>, NonLinearCollection<T> {
        using Pair<T, T>::operator==;
        using Pair<T, T>::operator!=;
        SingleTypePair(void) {
            EmptyBenchmark
        }
        SingleTypePair(const T& first, const T& second) : Pair<T, T>(first, second) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(2);
        }
        [[nodiscard]] virtual T At(size_t index) const override {
            StartBenchmark
            if (index >= 2) Panic("Index out of bounds");
            ReturnFromBenchmark(index ? this->second : this->first);
        }
        [[nodiscard]] virtual T& At(size_t index) override {
            StartBenchmark
            if (index >= 2) Panic("Index out of bounds");
            ReturnFromBenchmark(index ? this->second : this->first);
        }
        [[nodiscard]] virtual bool Add(const T&) override {
            StartAndReturnFromBenchmark(false);
        }
        [[nodiscard]] virtual bool Reset(void) override {
            StartAndReturnFromBenchmark(false);
        }
    };
}

#endif