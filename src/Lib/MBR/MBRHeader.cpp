#include "MBRHeader.hpp"
#include "../Memory.hpp"

namespace MathLib {
    MBRHeader::MBRHeader(void) : uniqueDiskID(0xbeefed00), attributes(Attributes::Normal), signature(expectedSignature) {
        for (uint8_t i = 0; i < 4; i++) entries[i] = MBRPartitionEntry();
    }
    bool MBRHeader::IsValid(void) const {
        bool active = false;
        for (uint8_t i = 0; i < 4; i++) {
            if (!entries[i].IsValid()) return false;
            else if (entries[i].type == MBRPartitionEntry::Type::Active) {
                if (active) return false;
                active = true;
            }
        }
        return (attributes == Attributes::Normal || attributes == Attributes::ReadOnly) && signature == expectedSignature;
    }
}