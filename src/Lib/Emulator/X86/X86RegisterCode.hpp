#ifndef Emulator_X86_RegisterCode_H
#define Emulator_X86_RegisterCode_H
#include <stdint.h>

namespace MathLib {
    enum class X86RegisterCode : uint8_t {
        A,
        C,
        D,
        B,
        SP,
        BP,
        SI,
        DI,
    };
}

#endif