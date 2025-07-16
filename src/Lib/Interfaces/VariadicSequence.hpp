#ifndef MathLib_Interfaces_VariadicSequence_H
#define MathLib_Interfaces_VariadicSequence_H
#include "Sequence.hpp"
#include "../Benchmark.hpp"

namespace MathLib {
    template <typename T>
    [[nodiscard]] size_t GetVariadicSize(void) {
        StartAndReturnFromBenchmark(0);
    }
    template <typename T, typename... Data>
    [[nodiscard]] size_t GetVariadicSize(T, Data... args) {
        StartAndReturnFromBenchmark(GetVariadicSize<T>(args...) + 1);
    }
    template <typename T>
    [[nodiscard]] T GetVariadicElement(size_t) {
        StartAndReturnFromBenchmark(T());
    }
    template <typename T, typename... Data>
    [[nodiscard]] T GetVariadicElement(size_t index, T arg, Data... args) {
        StartAndReturnFromBenchmark(index ? GetVariadicElement<T>(index - 1, args...) : arg);
    }
    template <typename T, T... data>
    struct VariadicSequence : Sequence<T> {
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(GetVariadicSize<T>(data...));
        }
        [[nodiscard]] virtual T At(size_t index) const override {
            StartAndReturnFromBenchmark(GetVariadicElement<T>(index, data...));
        }
    };
}

#endif