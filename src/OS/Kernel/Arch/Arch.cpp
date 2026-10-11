#include "Arch.hpp"
#include <Math/Constants.hpp>
#include <Host.hpp>

MathLib::num_t GenericRealInversedTan(MathLib::num_t x) {
    if (MathLib::Abs(x) > 1) {
        if (x > 0) return MathLib::pi / 2 - GenericRealInversedTan(1 / x);
        else return -MathLib::pi / 2 - GenericRealInversedTan(1 / x);
    }
    else {
        MathLib::num_t result = 0;
        MathLib::num_t term = x;
        const MathLib::num_t tmp = x * x;
        size_t n = 1;
        while (MathLib::Abs(term) > MathLib::eps) {
            result += term;
            term *= -tmp * (2 * n - 1) / (2 * n + 1);
            n++;
        }
        return result;
    }
}
MathLib::num_t GenericInversedTan2(MathLib::num_t y, MathLib::num_t x) {
    if (x > 0) return GenericRealInversedTan(y / x);
    else if (x < 0) {
        if (y >= 0) return GenericRealInversedTan(y / x) + MathLib::pi;
        else return GenericRealInversedTan(y / x) - MathLib::pi;
    }
    else if (y > 0) return MathLib::pi / 2;
    else if (y < 0) return -MathLib::pi / 2;
    else return MathLib::nan;
}
MathLib::num_t GenericExp(MathLib::num_t x) {
    MathLib::num_t sum = 1;
    MathLib::num_t term = 1;
    size_t n = 1;
    while (MathLib::Abs(term) > MathLib::eps) {
        term *= x / n++;
        sum += term;
    }
    return sum;
}