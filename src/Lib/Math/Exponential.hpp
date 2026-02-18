#ifndef MathLib_Math_Exponential_H
#define MathLib_Math_Exponential_H
#include "../Expected.hpp"

#define CreateExponential(self, req, identity)                      \
[[nodiscard]] MathLib::Expected<self> UnsignedPow(size_t n) const { \
    if (!(req)) return MathLib::Expected<self>();                   \
    if (!n) return identity;                                        \
    self ret = *this;                                               \
    for (size_t i = 1; i < n; i++) ret *= *this;                    \
    return MathLib::Expected<self>(ret);                            \
}

#endif