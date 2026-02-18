#include "ELFHeader.hpp"

namespace MathLib {
    bool ELFHeader::IsValid(void) const {
        return *(const uint32_t*)signature == *(const uint32_t*)expectedSignature && version == expectedVersion && bitMode != BitMode::Invalid && endianness != Endianness::Invalid;
    }
    bool ELFHeader::operator==(const ELFHeader& other) const {
        return *(const uint32_t*)signature == *(const uint32_t*)other.signature && bitMode == other.bitMode && endianness == other.endianness && headerVersion == other.headerVersion && abi == other.abi && abiVersion == other.abiVersion && reserved == other.reserved && type == other.type && arch == other.arch && version == other.version && entry == other.entry && programHeader == other.programHeader && sectionHeader == other.sectionHeader && flags == other.flags && headerSize == other.headerSize && programHeaderEntrySize == other.programHeaderEntrySize && programHeaderEntries == other.programHeaderEntries && sectionHeaderEntrySize == other.sectionHeaderEntrySize && sectionHeaderEntries == other.sectionHeaderEntries && stringTable == other.stringTable;
    }
    bool ELFHeader::operator!=(const ELFHeader& other) const {
        return !(*this == other);
    }
}