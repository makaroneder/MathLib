#include "ApproxEByLimit.hpp"

MathLib::RationalNumber ApproxEByLimit(MathLib::NaturalNumber n) {
    return MathLib::RationalNumber(n + MathLib::NaturalNumber::FromT<uint8_t>(1), n) ^ n;
}