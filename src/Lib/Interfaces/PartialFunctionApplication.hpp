#ifndef MathLib_Interfaces_PartialFunctionApplication_H
#define MathLib_Interfaces_PartialFunctionApplication_H
#include "Function.hpp"

namespace MathLib {
    template <typename Ret, typename T, typename... Args>
    struct PartialFunctionApplication : Function<Ret, Args...> {
        PartialFunctionApplication(const Function<Ret, T, Args...>& function, const T& value) : value(value), function(function) {}
        [[nodiscard]] virtual Ret Invoke(Args... args) const override {
            return function(value, args...);
        }

        private:
        T value;
        const Function<Ret, T, Args...>& function;
    };
}

#endif