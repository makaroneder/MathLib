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
        [[nodiscard]] T GetOr(const T& alternative) const {
            StartAndReturnFromBenchmark(error ? alternative : value);
        }
        [[nodiscard]] T Get(const Sequence<char>& panicStr) const {
            StartBenchmark
            if (error) Panic(panicStr);
            else ReturnFromBenchmark(value);
        }
        [[nodiscard]] T Get(const char* panicStr = "Expected value doesn't exist") const {
            StartBenchmark
            if (error) Panic(panicStr);
            else ReturnFromBenchmark(value);
        }
        [[nodiscard]] bool HasValue(void) const {
            StartAndReturnFromBenchmark(!error);
        }
        template <typename F>
        [[nodiscard]] Expected<F> Convert(void) const {
            StartAndReturnFromBenchmark(HasValue() ? Expected<F>((F)value) : Expected<F>());
        }

        private:
        T value;
        bool error;
    };
    template <typename T>
    [[nodiscard]] String ToString(const Expected<T>& expected) {
        StartAndReturnFromBenchmark(expected.HasValue() ? MathLib::ToString(expected.Get()) : "No value");
    }
}

#endif