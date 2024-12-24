#ifndef Freestanding
#ifndef MathLib_Libc_HostFunction_H
#define MathLib_Libc_HostFunction_H
#include "../Interfaces/Function.hpp"
#include <functional>

namespace MathLib {
    template <typename Ret, typename... Args>
    struct HostFunction : Function<Ret, Args...> {
        using RawFunction = std::function<Ret(const void*, Args...)>;
        HostFunction(void) {
            EmptyBenchmark
        }
        HostFunction(void* data, RawFunction function) : Function<Ret, Args...>(data), function(function) {
            EmptyBenchmark
        }
        RawFunction GetFunction(void) const {
            StartBenchmark
            ReturnFromBenchmark(function);
        }
        virtual Ret Invoke(const void* data, Args... args) const override {
            StartBenchmark
            ReturnFromBenchmark(function(data, args...));
        }

        private:
        RawFunction function;
    };
}

#endif
#endif