#ifndef MathLib_Executable_H
#define MathLib_Executable_H
#include "ExecutableData.hpp"

namespace MathLib {
    struct Executable {
        [[nodiscard]] virtual ExecutableData GetCode(void) const = 0;
    };
}

#endif