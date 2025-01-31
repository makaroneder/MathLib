#include "FATBootSector.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    bool FATBootSector::IsValid(void) const {
        StartBenchmark
        ReturnFromBenchmark(signature == expectedSignature);
    }
    uint32_t FATBootSector::GetSectorCount(void) const {
        StartBenchmark
        ReturnFromBenchmark(sectorCount16 ? sectorCount16 : sectorCount32);
    }
}