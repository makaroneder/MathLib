#ifndef MathLib_Interfaces_IndicatorFunction_H
#define MathLib_Interfaces_IndicatorFunction_H
#include "Function.hpp"
#include "Container.hpp"

namespace MathLib {
    template <typename T>
    struct IndicatorFunction : Function<bool, T> {
        IndicatorFunction(const Container<T>& container) : container(container) {}
        [[nodiscard]] virtual bool Invoke(T x) const override {
            return container.Contains(x);
        }
    
        private:
        const Container<T>& container;
    };
}

#endif