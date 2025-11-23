#ifndef ApproxEBySequence_H
#define ApproxEBySequence_H
#include "ApproxEBySequenceUnitReturnValue.hpp"

void ApproxEBySequenceUnit(NaturalNumber start, NaturalNumber end, ApproxEBySequenceUnitReturnValue* ret);
void ApproxEBySequenceMultiUnit(NaturalNumber start, NaturalNumber end, ApproxEBySequenceUnitReturnValue* ret);
RationalNumber ApproxEBySequence(NaturalNumber limit);

#endif