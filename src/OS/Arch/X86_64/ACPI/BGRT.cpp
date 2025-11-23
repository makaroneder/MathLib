#ifdef __x86_64__
#include "BGRT.hpp"

bool BGRT::IsValid(void) const {
    return MathLib::ACPITable::IsValid() && version == expectedVersion && type == Type::Bitmap;
}

#endif