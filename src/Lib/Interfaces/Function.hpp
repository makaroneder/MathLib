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
        const void* GetData(void) const {
            StartBenchmark
            ReturnFromBenchmark(data);
        }
        Ret operator()(Args... args) const {
            StartBenchmark
            ReturnFromBenchmark(Invoke(data, args...));
        }
        virtual Ret Invoke(const void* data, Args... args) const = 0;

        private:
        void* data;
    };
}

#endif