#ifndef MathLib_Math_Exponential_H
#define MathLib_Math_Exponential_H
#include "../Expected.hpp"

#define CreateExponential(self, req, identity)                      \
[[nodiscard]] MathLib::Expected<self> UnsignedPow(size_t n) const { \
    StartBenchmark                                                  \
    if (!(req)) ReturnFromBenchmark(MathLib::Expected<self>());     \
    if (!n) ReturnFromBenchmark(identity);                          \
    self ret = *this;                                               \
    for (size_t i = 1; i < n; i++) ret *= *this;                    \
    ReturnFromBenchmark(MathLib::Expected<self>(ret));              \
}


#endif