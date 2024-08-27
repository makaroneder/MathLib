#include "PCIHeader.hpp"

bool PCIHeader::IsValid(void) const {
    return deviceID && deviceID != UINT16_MAX;
}