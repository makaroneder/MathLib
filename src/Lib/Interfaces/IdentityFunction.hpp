#ifndef MathLib_Interfaces_IdentityFunction_H
#define MathLib_Interfaces_IdentityFunction_H
#include "Function.hpp"

namespace MathLib {
    template <typename Ret, typename Arg>
    struct IdentityFunction : Function<Ret, Arg> {
        [[nodiscard]] virtual Ret Invoke(Arg arg) const override {
            return arg;
        }
    };
}

#endif