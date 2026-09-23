#include "ApproxEBySequenceUnitReturnValue.hpp"

ApproxEBySequenceUnitReturnValue::ApproxEBySequenceUnitReturnValue(void) : ret(), factorial(MathLib::NaturalNumber::FromT<uint8_t>(1)) {}
ApproxEBySequenceUnitReturnValue::ApproxEBySequenceUnitReturnValue(const MathLib::RationalNumber& ret, const MathLib::NaturalNumber& factorial) : ret(ret), factorial(factorial) {}