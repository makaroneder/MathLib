#ifndef Freestanding
#ifndef MathLib_Libc_HostFunction_H
#define MathLib_Libc_HostFunction_H
#include "../Interfaces/Function.hpp"
#include <functional>

namespace MathLib {
    template <typename Ret, typename... Args>
    struct HostFunction : Function<Ret, Args...> {
        using RawFunction = std::function<Ret(Args...)>;
        HostFunction(void) {}
        HostFunction(RawFunction function) : function(function) {}
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
#endif