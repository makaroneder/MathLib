#ifndef Freestanding
#ifndef Libc_HostFunction_H
#define Libc_HostFunction_H
#include "../Interfaces/Function.hpp"
#include <functional>

namespace MathLib {
    template <typename Ret, typename... Args>
    struct HostFunction : Function<Ret, Args...> {
        using RawFunction = std::function<Ret(const void*, Args...)>;
        HostFunction(void) {}
        HostFunction(void* data, RawFunction function) : Function<Ret, Args...>(data), function(function) {}
        RawFunction GetFunction(void) const {
            return function;
        }
        virtual Ret Invoke(const void* data, Args... args) const override {
            return function(data, args...);
        }

        private:
        RawFunction function;
    };
}

#endif
#endif