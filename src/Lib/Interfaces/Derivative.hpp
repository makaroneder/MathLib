#ifndef MathLib_Interfaces_Derivative_H
#define MathLib_Interfaces_Derivative_H
#include "Interfaces/Function.hpp"

namespace MathLib {
    template <typename Ret, typename Arg>
    Ret GetDerivative(const Function<Ret, Arg>& func, const Arg& x, const Arg& eps) {
        return (func(x + eps) - func(x)) / eps;
    }
    template <typename Ret, typename Arg>
    struct Derivative : Function<Ret, Arg> {
        Derivative(const Function<Ret, Arg>& func, const Arg& eps) : func(func), eps(eps) {}
        [[nodiscard]] virtual Ret Invoke(Arg x) const override {
            return GetDerivative<Ret, Arg>(func, x, eps);
        }

        private:
        const Function<Ret, Arg>& func;
        Arg eps;
    };
}

#endif