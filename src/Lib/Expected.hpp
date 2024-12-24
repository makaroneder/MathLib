#ifndef MathLib_Expected_H
#define MathLib_Expected_H
#include "Host.hpp"

namespace MathLib {
    template <typename T>
    struct Expected : Allocatable {
        Expected(void) : value(T()), error(true) {
            EmptyBenchmark
        }
        Expected(const T& value) : value(value), error(false) {
            EmptyBenchmark
        }
        T Get(const String& panicStr = "Expected value doesn't exist") const {
            StartBenchmark
            if (error) Panic(panicStr);
            else ReturnFromBenchmark(value);
        }
        bool HasValue(void) const {
            StartBenchmark
            ReturnFromBenchmark(!error);
        }

        private:
        T value;
        bool error;
    };
}

#endif