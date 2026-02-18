#ifndef MathLib_SingleTypePair_H
#define MathLib_SingleTypePair_H
#include "Interfaces/Sequence/NonLinearCollection.hpp"
#include "Pair.hpp"

namespace MathLib {
    template <typename T>
    struct SingleTypePair : Pair<T, T>, NonLinearCollection<T> {
        using Pair<T, T>::operator==;
        using Pair<T, T>::operator!=;
        SingleTypePair(void) {}
        SingleTypePair(const T& first, const T& second) : Pair<T, T>(first, second) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return 2;
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return index ? this->second : this->first;
        }
        [[nodiscard]] virtual T& AtUnsafe(size_t index) override {
            return index ? this->second : this->first;
        }
        [[nodiscard]] virtual bool Add(const T&) override {
            return false;
        }
        [[nodiscard]] virtual bool Reset(void) override {
            return false;
        }
    };
}

#endif