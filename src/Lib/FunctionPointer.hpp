#ifndef MathLib_FunctionPointer_H
#define MathLib_FunctionPointer_H
#include "Interfaces/Function.hpp"

namespace MathLib {
    template <typename Ret, typename... Args>
    struct FunctionPointer : Function<Ret, Args...> {
        using RawFunction = Ret (*)(Args...);
        FunctionPointer(void) {}
        FunctionPointer(const RawFunction& function) : function(function) {}
        [[nodiscard]] RawFunction GetFunction(void) const {
            return function;
        }
        [[nodiscard]] virtual Ret Invoke(Args... args) const override {
            return function(args...);
        }

        private:
        RawFunction function;
    };
}

#endif