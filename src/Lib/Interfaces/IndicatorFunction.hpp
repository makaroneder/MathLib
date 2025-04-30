#ifndef MathLib_Interfaces_IndicatorFunction_H
#define MathLib_Interfaces_IndicatorFunction_H
#include "../Typedefs.hpp"

namespace MathLib {
    template <typename T>
    struct IndicatorFunction : Function<bool, T> {
        IndicatorFunction(const Container<T>& container) : container(container) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual bool Invoke(const void*, T x) const override {
            StartAndReturnFromBenchmark(container.Contains(x));
        }
    
        private:
        const Container<T>& container;
    };
}

#endif