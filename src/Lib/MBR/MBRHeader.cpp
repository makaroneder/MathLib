#include "MBRHeader.hpp"
#include <stddef.h>

MBRHeader::MBRHeader(void) : uniqueDiskID(0xbeefed00), attributes(Attributes::Normal), signature(expectedSignature) {
    for (size_t i = 0; i < SizeOfArray(code); i++) code[i] = 0;
    for (size_t i = 0; i < 4; i++) entries[i] = MBRPartitionEntry();
}
bool MBRHeader::IsValid(void) const {
    bool active = false;
    for (size_t i = 0; i < 4; i++) {
        if (!entries[i].IsValid()) return false;
        else if (entries[i].type == MBRPartitionEntry::Type::Active) {
            if (active) return false;
            active = true;
        }
    }
    return (attributes == Attributes::Normal || attributes == Attributes::ReadOnly) && signature == expectedSignature;
}