#ifndef MathLib_Math_Orderable_H
#define MathLib_Math_Orderable_H
#include "../Allocator/Allocatable.hpp"
#include "../Benchmark.hpp"

namespace MathLib {
    struct Orderable : Allocatable {
        [[nodiscard]] bool operator<=(const Orderable& other) const {
            StartAndReturnFromBenchmark(LessThanEqual(other));
        }
        [[nodiscard]] bool operator>=(const Orderable& other) const {
            StartAndReturnFromBenchmark(other.LessThanEqual(*this));
        }
        [[nodiscard]] bool operator==(const Orderable& other) const {
            StartAndReturnFromBenchmark(LessThanEqual(other) && other.LessThanEqual(*this));
        }
        [[nodiscard]] bool operator!=(const Orderable& other) const {
            StartAndReturnFromBenchmark(!LessThanEqual(other) || !other.LessThanEqual(*this));
        }
        [[nodiscard]] bool operator<(const Orderable& other) const {
            StartAndReturnFromBenchmark(LessThanEqual(other) && !other.LessThanEqual(*this));
        }
        [[nodiscard]] bool operator>(const Orderable& other) const {
            StartAndReturnFromBenchmark(other.LessThanEqual(*this) && !LessThanEqual(other));
        }
        [[nodiscard]] bool Uncomparable(const Orderable& other) const {
            StartAndReturnFromBenchmark(!LessThanEqual(other) && !other.LessThanEqual(*this));
        }

        protected:
        [[nodiscard]] virtual bool LessThanEqual(const Orderable& other) const = 0;
    };
}

#endif