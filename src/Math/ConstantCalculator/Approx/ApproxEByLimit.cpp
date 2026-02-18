#include "ApproxEByLimit.hpp"

RationalNumber ApproxEByLimit(NaturalNumber n) {
    return RationalNumber(n + NaturalNumber::FromT<uint8_t>(1), n) ^ n;
}