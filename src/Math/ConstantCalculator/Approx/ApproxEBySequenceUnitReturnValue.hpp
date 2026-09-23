#ifndef ApproxEBySequenceUnitReturnValue_H
#define ApproxEBySequenceUnitReturnValue_H
#include <BigInt/RationalNumber.hpp>

struct ApproxEBySequenceUnitReturnValue {
    MathLib::RationalNumber ret;
    MathLib::NaturalNumber factorial;

    ApproxEBySequenceUnitReturnValue(void);
    ApproxEBySequenceUnitReturnValue(const MathLib::RationalNumber& ret, const MathLib::NaturalNumber& factorial);
};

#endif