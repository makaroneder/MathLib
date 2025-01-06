#ifndef MathLib_Interfaces_Function_H
#define MathLib_Interfaces_Function_H
#include "Typedefs.hpp"

namespace MathLib {
    template <typename Ret, typename... Args>
    struct Function : Allocatable {
        Function(void) {
            EmptyBenchmark
        }
        Function(void* data) : data(data) {
            EmptyBenchmark
        }
        [[nodiscard]] const void* GetData(void) const {
            StartBenchmark
            ReturnFromBenchmark(data);
        }
        [[nodiscard]] Ret operator()(Args... args) const {
            StartBenchmark
            ReturnFromBenchmark(Invoke(data, args...));
        }
        [[nodiscard]] virtual Ret Invoke(const void* data, Args... args) const = 0;

        private:
        void* data;
    };
}

#endif