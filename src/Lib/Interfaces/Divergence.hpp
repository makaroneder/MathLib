#ifndef MathLib_Interfaces_Divergence_H
#define MathLib_Interfaces_Divergence_H
#include "Gradient.hpp"

namespace MathLib {
    template <typename Ret, typename Arg>
    struct Divergence : Function<Ret, const Sequence<Arg>&> {
        Divergence(const Function<Ret, const Sequence<Arg>&>& func, const Arg& eps) : gradient(func, eps) {}
        [[nodiscard]] virtual Ret Invoke(const Sequence<Arg>& args) const override {
            return gradient(args).Sum(Ret());
        }

        private:
        Gradient<Ret, Arg> gradient;
    };
}

#endif