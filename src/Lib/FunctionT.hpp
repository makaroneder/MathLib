#ifndef MathLib_FunctionT_H
#define MathLib_FunctionT_H
#include "Interfaces/Function.hpp"

namespace MathLib {
    template <typename T, typename Ret, typename... Args>
    struct FunctionT : Function<Ret, Args...> {
        FunctionT(void) {}
        FunctionT(const T& function) : function(function) {}
        [[nodiscard]] T GetFunction(void) const {
            return function;
        }
        [[nodiscard]] virtual Ret Invoke(Args... args) const override {
            return function(args...);
        }

        private:
        T function;
    };
    template <typename Ret, typename... Args, typename T>
    FunctionT<T, Ret, Args...> MakeFunctionT(T function) {
        return FunctionT<T, Ret, Args...>(function);
    }
    template <typename Ret, typename... Args, typename T>
    FunctionT<T, Ret, Args...>* AllocFunctionT(T function) {
        return new FunctionT<T, Ret, Args...>(function);
    }
}

#endif