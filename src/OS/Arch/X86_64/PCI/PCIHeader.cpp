#ifdef __x86_64__
#include "PCIHeader.hpp"

bool PCIHeader::IsValid(void) const {
    return deviceID && deviceID != UINT16_MAX;
}

#endif