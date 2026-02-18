#include "FATBootSector.hpp"

namespace MathLib {
    bool FATBootSector::IsValid(void) const {
        return signature == expectedSignature;
    }
    uint32_t FATBootSector::GetSectorCount(void) const {
        return sectorCount16 ? sectorCount16 : sectorCount32;
    }
}