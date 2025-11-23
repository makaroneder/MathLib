#include "XorSwap.hpp"

namespace MathLib {
    void XorSwap(uint64_t& x, uint64_t& y) {
        x ^= y;
        y ^= x;
        x ^= y;
    }
}