#ifndef MathLib_Interfaces_FunctionComposition_H
#define MathLib_Interfaces_FunctionComposition_H
#include "Function.hpp"

namespace MathLib {
    template <typename Ret, typename Arg1, typename... Args2>
    struct FunctionComposition : Function<Ret, Args2...> {
        FunctionComposition(const Function<Ret, Arg1>& func1, const Function<Arg1, Args2...>& func2) : func1(func1), func2(func2) {}
        [[nodiscard]] virtual Ret Invoke(Args2... args) const override {
            return func1(func2(args...));
        }

        private:
        const Function<Ret, Arg1>& func1;
        const Function<Arg1, Args2...>& func2;
    };
}

#endif