#ifndef MathLib_CPU_Ln_H
#define MathLib_CPU_Ln_H
#include "../Typedefs.hpp"

namespace MathLib {
    using LnFunction = num_t (*)(num_t);
    extern LnFunction lnFunction;
    [[nodiscard]] num_t DefaultLn(num_t x);
    [[nodiscard]] num_t GenericLn(num_t x);
}

#endif