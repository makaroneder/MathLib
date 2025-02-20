#ifndef MathLib_Interfaces_Function_H
#define MathLib_Interfaces_Function_H
#include "Typedefs.hpp"

namespace MathLib {
    template <typename Ret, typename... Args>
    struct Function : Allocatable {
        void* data;

        Function(void) {
            EmptyBenchmark
        }
        Function(void* data) : data(data) {
            EmptyBenchmark
        }
        [[nodiscard]] Ret operator()(Args... args) const {
            StartAndReturnFromBenchmark(Invoke(data, args...));
        }
        [[nodiscard]] virtual Ret Invoke(const void* data, Args... args) const = 0;
    };
}

#endif