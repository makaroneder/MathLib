#ifndef MathLib_Interfaces_FunctionComposition_H
#define MathLib_Interfaces_FunctionComposition_H
#include "Function.hpp"

namespace MathLib {
    template <typename Ret, typename Arg1, typename Arg2>
    struct FunctionComposition : Function<Ret, Arg2> {
        FunctionComposition(const Function<Ret, Arg1>& func1, const Function<Arg1, Arg2>& func2) : func1(func1), func2(func2) {}
        [[nodiscard]] virtual Ret Invoke(const void*, Arg2 arg) const override {
            return func1(func2(arg));
        }

        private:
        const Function<Ret, Arg1>& func1;
        const Function<Arg1, Arg2>& func2;
    };
}

#endif