#include "Arch.hpp"
#include <Math/Constants.hpp>
#include <Host.hpp>

MathLib::num_t GenericSqrt(MathLib::num_t x) {
    if (x < 0) return MathLib::nan;
    if (!x) return 0;
    MathLib::num_t guess = x;
    MathLib::num_t prev;
    do {
        prev = guess;
        guess = (guess + x / guess) / 2;
    } while (MathLib::Abs(guess - prev) > MathLib::eps);
    return guess;
}
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
MathLib::num_t GenericLn(MathLib::num_t x) {
    if (x <= 0) return MathLib::nan;
    MathLib::num_t logx = 0;
    while (x >= 2) {
        x /= 2;
        logx += MathLib::ln2;
    }
    MathLib::num_t result = (x - 1) / (x + 1);
    MathLib::num_t numerator = result;
    const MathLib::num_t tmp = result * result;
    size_t i = 1;
    while (MathLib::Abs(numerator) > MathLib::eps) {
        numerator *= tmp;
        i += 2;
        result += numerator / i;
    }
    return logx + result * 2;
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