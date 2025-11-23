#ifdef __x86_64__
#include "VBE.hpp"
#include <Typedefs.hpp>

bool VBEInfo::IsValid(void) const {
    for (uint8_t i = 0; i < SizeOfArray(signature); i++)
        if (signature[i] != expectedSignature[i]) return false;
    return true;
}

#endif