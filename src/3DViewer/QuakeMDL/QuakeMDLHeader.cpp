#include "QuakeMDLHeader.hpp"

bool QuakeMDLHeader::IsValid(void) const {
    return version == expectedVersion && *(const uint32_t*)signature == *(const uint32_t*)expectedSignature;
}