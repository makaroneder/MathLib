#ifndef MathLib_Interfaces_FunctionArgumentsSubstitution_H
#define MathLib_Interfaces_FunctionArgumentsSubstitution_H
#include "Sequence/SequenceSubstitution.hpp"

namespace MathLib {
    template <typename Ret, typename Arg>
    struct FunctionArgumentsSubstitution : Function<Ret, Arg> {
        FunctionArgumentsSubstitution(const Function<Ret, const Sequence<Arg>&>& func, const Sequence<Arg>& args, size_t index) : func(func), args(args), index(index) {}
        [[nodiscard]] virtual Ret Invoke(const Arg arg) const override {
            return func(SequenceSubstitution<Arg>(args, arg, index));
        }

        private:
        const Function<Ret, const Sequence<Arg>&>& func;
        const Sequence<Arg>& args;
        size_t index;
    };
}

#endif