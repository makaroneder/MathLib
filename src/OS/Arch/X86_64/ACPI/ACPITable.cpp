#ifdef __x86_64__
#include "ACPITable.hpp"

bool ACPITable::IsValid(void) const {
    uint8_t sum = 0;
    const uint8_t* self8 = (const uint8_t*)this;
    for (uint32_t i = 0; i < length; i++) sum += self8[i];
    return !sum;
}

#endif