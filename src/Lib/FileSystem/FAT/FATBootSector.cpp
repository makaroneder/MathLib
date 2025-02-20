#include "FATBootSector.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    bool FATBootSector::IsValid(void) const {
        StartAndReturnFromBenchmark(signature == expectedSignature);
    }
    uint32_t FATBootSector::GetSectorCount(void) const {
        StartAndReturnFromBenchmark(sectorCount16 ? sectorCount16 : sectorCount32);
    }
}