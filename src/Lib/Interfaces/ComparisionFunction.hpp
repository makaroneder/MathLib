#ifndef MathLib_Interfaces_ComparisionFunction_H
#define MathLib_Interfaces_ComparisionFunction_H
#include "../Typedefs.hpp"

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
        ComparisionFunction(ComparisionFunctionType type) : type(type) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual bool Invoke(const void*, T x, T y) const override {
            StartBenchmark
            switch (type) {
                case ComparisionFunctionType::Equal: ReturnFromBenchmark(x == y);
                case ComparisionFunctionType::NotEqual: ReturnFromBenchmark(x != y);
                case ComparisionFunctionType::LessThan: ReturnFromBenchmark(x < y);
                case ComparisionFunctionType::LessThanEqual: ReturnFromBenchmark(x <= y);
                case ComparisionFunctionType::GreaterThan: ReturnFromBenchmark(x > y);
                case ComparisionFunctionType::GreaterThanEqual: ReturnFromBenchmark(x >= y);
                default: ReturnFromBenchmark(false);
            }
        }
    
        private:
        ComparisionFunctionType type;
    };
}

#endif