#ifndef ApproxEBySequenceUnitReturnValue_H
#define ApproxEBySequenceUnitReturnValue_H
#include "../RationalNumber.hpp"

struct ApproxEBySequenceUnitReturnValue {
    RationalNumber ret;
    NaturalNumber factorial;

    ApproxEBySequenceUnitReturnValue(void);
    ApproxEBySequenceUnitReturnValue(const RationalNumber& ret, const NaturalNumber& factorial);
};

#endif