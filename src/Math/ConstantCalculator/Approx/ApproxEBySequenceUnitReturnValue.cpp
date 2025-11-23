#include "ApproxEBySequenceUnitReturnValue.hpp"

ApproxEBySequenceUnitReturnValue::ApproxEBySequenceUnitReturnValue(void) : ret(), factorial(NaturalNumber::FromT<uint8_t>(1)) {}
ApproxEBySequenceUnitReturnValue::ApproxEBySequenceUnitReturnValue(const RationalNumber& ret, const NaturalNumber& factorial) : ret(ret), factorial(factorial) {}