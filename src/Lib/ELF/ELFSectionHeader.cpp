#include "ELFSectionHeader.hpp"

namespace MathLib {
    bool ELFSectionHeader::operator==(const ELFSectionHeader& other) const {
        return name == other.name && type == other.type && writable == other.writable && inMemory == other.inMemory && executable == other.executable && reserved1 == other.reserved1 && mergable == other.mergable && containsStrings == other.containsStrings && hasSHTIndex == other.hasSHTIndex && preserveLinkOrder == other.preserveLinkOrder && osSpecificHandling == other.osSpecificHandling && isInGroup == other.isInGroup && hasThreadLocalData == other.hasThreadLocalData && compressed == other.compressed && reserved2 == other.reserved2 && retain == other.retain && reserved3 == other.reserved3 && specialOrdering == other.specialOrdering && exclude == other.exclude && reserved4 == other.reserved4 && address == other.address && offset == other.offset && size == other.size && link == other.link && info == other.info && alignment == other.alignment && entrySize == other.entrySize;
    }
    bool ELFSectionHeader::operator!=(const ELFSectionHeader& other) const {
        return !(*this == other);
    }
}