#ifndef Freestanding
#ifndef MathLib_Libc_HostFunction_H
#define MathLib_Libc_HostFunction_H
#include "../Interfaces/Function.hpp"
#include <functional>

namespace MathLib {
    template <typename Ret, typename... Args>
    struct HostFunction : Function<Ret, Args...> {
        using RawFunction = std::function<Ret(Args...)>;
        HostFunction(void) {
            EmptyBenchmark
        }
        HostFunction(RawFunction function) : Function<Ret, Args...>(nullptr), function(function) {
            EmptyBenchmark
        }
        [[nodiscard]] RawFunction GetFunction(void) const {
            StartBenchmark
            ReturnFromBenchmark(function);
        }
        [[nodiscard]] virtual Ret Invoke(const void*, Args... args) const override {
            StartBenchmark
            ReturnFromBenchmark(function(args...));
        }

        private:
        RawFunction function;
    };
}

#endif
#endif