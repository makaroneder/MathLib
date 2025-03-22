#include "MBRHeader.hpp"
#include "../Memory.hpp"

namespace MathLib {
    MBRHeader::MBRHeader(void) : uniqueDiskID(0xbeefed00), attributes(Attributes::Normal), signature(expectedSignature) {
        StartBenchmark
        for (uint8_t i = 0; i < 4; i++) entries[i] = MBRPartitionEntry();
        EndBenchmark
    }
    bool MBRHeader::IsValid(void) const {
        StartBenchmark
        bool active = false;
        for (uint8_t i = 0; i < 4; i++) {
            if (!entries[i].IsValid()) ReturnFromBenchmark(false)
            else if (entries[i].type == MBRPartitionEntry::Type::Active) {
                if (active) ReturnFromBenchmark(false);
                active = true;
            }
        }
        ReturnFromBenchmark((attributes == Attributes::Normal || attributes == Attributes::ReadOnly) && signature == expectedSignature);
    }
}