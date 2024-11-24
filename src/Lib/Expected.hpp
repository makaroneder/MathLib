#ifndef Expected_H
#define Expected_H
#include "Host.hpp"

namespace MathLib {
    template <typename T>
    struct Expected : Allocatable {
        Expected(void) : value(T()), error(true) {}
        Expected(const T& value) : value(value), error(false) {}
        T Get(const String& panicStr = "Expected value doesn't exist") const {
            if (error) Panic(panicStr);
            else return value;
        }
        bool HasValue(void) const {
            return !error;
        }

        private:
        T value;
        bool error;
    };
}

#endif