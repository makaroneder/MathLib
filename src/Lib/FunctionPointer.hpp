#ifndef MathLib_FunctionPointer_H
#define MathLib_FunctionPointer_H
#include "Interfaces/Function.hpp"
#include "Typedefs.hpp"

namespace MathLib {
    template <typename Ret, typename... Args>
    struct FunctionPointer : Function<Ret, Args...> {
        using RawFunction = Ret (*)(const void*, Args...);
        FunctionPointer(void) {
            EmptyBenchmark
        }
        FunctionPointer(void* data, RawFunction function) : Function<Ret, Args...>(data), function(function) {
            EmptyBenchmark
        }
        [[nodiscard]] RawFunction GetFunction(void) const {
            StartAndReturnFromBenchmark(function);
        }
        [[nodiscard]] virtual Ret Invoke(const void* data, Args... args) const override {
            StartAndReturnFromBenchmark(function(data, args...));
        }

        private:
        RawFunction function;
    };
}

#endif