#ifndef MathLib_Interfaces_Gradient_H
#define MathLib_Interfaces_Gradient_H
#include "FunctionArgumentsSubstitution.hpp"
#include "../Typedefs.hpp"
#include "Derivative.hpp"

namespace MathLib {
    template <typename Ret, typename Arg>
    struct Gradient : Function<Array<Ret>, const Sequence<Arg>&> {
        Gradient(const Function<Ret, const Sequence<Arg>&>& func, const Arg& eps) : func(func), eps(eps) {}
        [[nodiscard]] virtual Array<Ret> Invoke(const Sequence<Arg>& args) const override {
            const size_t size = args.GetSize();
            Array<Ret> ret = size;
            for (size_t i = 0; i < size; i++) ret.AtUnsafe(i) = GetDerivative<Ret, Arg>(FunctionArgumentsSubstitution<Ret, Arg>(func, args, i), args.AtUnsafe(i), eps);
            return ret;
        }

        private:
        const Function<Ret, const Sequence<Arg>&>& func;
        Arg eps;
    };
    template <typename T>
    Array<T> GradientDescent(const Function<T, const Sequence<T>&>& func, const T& eps, const Sequence<T>& x, const Sequence<T>& rate) {
        const size_t rateSize = rate.GetSize();
        const Gradient<T, T> gradient = Gradient<T, T>(func, eps);
        Array<T> ret = CollectionToArray<T>(x);
        const size_t size = ret.GetSize();
        for (size_t i = 0; i < rateSize; i++) {
            const Array<T> grad = gradient(ret);
            for (size_t j = 0; j < size; j++) ret.AtUnsafe(j) -= grad.At(j) * rate.AtUnsafe(i);
        }
        return ret;
    }
}

#endif