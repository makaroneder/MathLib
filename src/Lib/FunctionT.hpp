#ifndef MathLib_FunctionT_H
#define MathLib_FunctionT_H
#include "Interfaces/Function.hpp"

namespace MathLib {
    template <typename T, typename Ret, typename... Args>
    struct FunctionT : Function<Ret, Args...> {
        FunctionT(void) {
            EmptyBenchmark
        }
        FunctionT(void* data, T function) : Function<Ret, Args...>(data), function(function) {
            EmptyBenchmark
        }
        [[nodiscard]] T GetFunction(void) const {
            StartAndReturnFromBenchmark(function);
        }
        [[nodiscard]] virtual Ret Invoke(const void* data, Args... args) const override {
            StartAndReturnFromBenchmark(function(data, args...));
        }

        private:
        T function;
    };
    template <typename Ret, typename... Args, typename T>
    FunctionT<T, Ret, Args...> MakeFunctionT(void* data, T function) {
        return FunctionT<T, Ret, Args...>(data, function);
    }
}

#endif