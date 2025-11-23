#ifndef MathLib_Interfaces_ComparisionFunction_H
#define MathLib_Interfaces_ComparisionFunction_H
#include "Function.hpp"
#include <stdint.h>

namespace MathLib {
    enum class ComparisionFunctionType : uint8_t {
        Equal,
        NotEqual,
        LessThan,
        LessThanEqual,
        GreaterThan,
        GreaterThanEqual,
    };
    template <typename T>
    struct ComparisionFunction : Function<bool, T, T> {
        ComparisionFunction(ComparisionFunctionType type) : type(type) {}
        [[nodiscard]] virtual bool Invoke(T x, T y) const override {
            switch (type) {
                case ComparisionFunctionType::Equal: return x == y;
                case ComparisionFunctionType::NotEqual: return x != y;
                case ComparisionFunctionType::LessThan: return x < y;
                case ComparisionFunctionType::LessThanEqual: return x <= y;
                case ComparisionFunctionType::GreaterThan: return x > y;
                case ComparisionFunctionType::GreaterThanEqual: return x >= y;
                default: return false;
            }
        }
    
        private:
        ComparisionFunctionType type;
    };
}

#endif