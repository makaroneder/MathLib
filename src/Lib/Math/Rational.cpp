#include "Rational.hpp"

namespace MathLib {
    Rational::Rational(bool sign, size_t a, size_t b) : sign(sign), a(a), b(b) {
        EmptyBenchmark
    }
    Rational::Rational(ssize_t a, size_t b) : sign(a >= 0), a(sign ? a : -a), b(b) {
        EmptyBenchmark
    }
    num_t Rational::ToFloat(void) const {
        StartBenchmark
        const num_t ret = a / b;
        ReturnFromBenchmark(sign ? ret : -ret);
    }
    Rational Rational::Simplify(void) const {
        // TODO:
        StartBenchmark
        ReturnFromBenchmark(*this);
    }
    Rational Rational::operator*(const Rational& other) const {
        StartBenchmark
        Rational ret = *this;
        ret.sign = sign == other.sign;
        ret.a *= other.a;
        ret.b *= other.b;
        ReturnFromBenchmark(ret);
    }
    Rational Rational::operator/(const Rational& other) const {
        StartBenchmark
        Rational ret = *this;
        ret.sign = sign == other.sign;
        ret.a *= other.b;
        ret.b *= other.a;
        ReturnFromBenchmark(ret);
    }
}