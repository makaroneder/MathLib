#ifndef ApproxEBySequence_H
#define ApproxEBySequence_H
#include "ApproxEBySequenceUnitReturnValue.hpp"

void ApproxEBySequenceUnit(MathLib::NaturalNumber start, MathLib::NaturalNumber end, ApproxEBySequenceUnitReturnValue* ret);
void ApproxEBySequenceMultiUnit(MathLib::NaturalNumber start, MathLib::NaturalNumber end, ApproxEBySequenceUnitReturnValue* ret);
MathLib::RationalNumber ApproxEBySequence(MathLib::NaturalNumber limit);

#endif