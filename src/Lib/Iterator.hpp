#ifndef MathLib_Iterator_H
#define MathLib_Iterator_H
#include "Allocator/Allocatable.hpp"
#include "Benchmark.hpp"

namespace MathLib {
    template <typename T>
    struct Iterator : Allocatable {
        Iterator(T* data) : data(data) {
            EmptyBenchmark
        }
        [[nodiscard]] T& operator*(void) const {
            StartAndReturnFromBenchmark(*data);
        }
        [[nodiscard]] T* operator->(void) {
            StartAndReturnFromBenchmark(data);
        }
        Iterator<T>& operator++(void) {
            StartBenchmark
            data++;
            ReturnFromBenchmark(*this);
        }
        Iterator<T> operator++(int) {
            StartBenchmark
            Iterator<T> tmp = *this;
            data++;
            ReturnFromBenchmark(tmp);
        }
        [[nodiscard]] bool operator==(const Iterator<T>& other) const {
            StartAndReturnFromBenchmark(data == other.data);
        }
        [[nodiscard]] bool operator!=(const Iterator<T>& other) const {
            StartAndReturnFromBenchmark(!(*this == other));
        }

        private:
        T* data;
    };
}

#endif