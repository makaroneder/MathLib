#ifndef MathLib_Interfaces_Sequence_Variadic_H
#define MathLib_Interfaces_Sequence_Variadic_H
#include "Sequence.hpp"

namespace MathLib {
    template <typename T>
    [[nodiscard]] T GetVariadicElement(size_t) {
        return T();
    }
    template <typename T, typename... Data>
    [[nodiscard]] T GetVariadicElement(size_t index, T arg, Data... args) {
        return index ? GetVariadicElement<T>(index - 1, args...) : arg;
    }
    template <typename T, T... data>
    struct VariadicSequence : Sequence<T> {
        static constexpr size_t size = sizeof...(data);
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return size;
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return GetVariadicElement<T>(index, data...);
        }
    };
}

#endif