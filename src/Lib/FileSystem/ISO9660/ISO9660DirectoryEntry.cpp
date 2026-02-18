#include "ISO9660DirectoryEntry.hpp"

namespace MathLib {
    ISO9660DirectoryEntry::ISO9660DirectoryEntry(void) : length(0), extendedAttributeRecordLength(0), extent(), bytesPerExtent(), date(), hidden(false), directory(false), associatedFile(false), formatInformationInExtendedAttributeRecord(false), permissionsInExtendedAttributeRecord(false), reserved(0), notFinalDirectory(false), interleaveUnitSize(0), interleaveGapSize(0), volumeNumber(), nameLength(0), name() {}
    bool ISO9660DirectoryEntry::IsValid(void) const {
        return length && date.IsValid();
    }
    String ISO9660DirectoryEntry::GetName(void) const {
        String ret;
        for (uint8_t i = 0; i < nameLength && name[i] != ';'; i++) ret += name[i];
        return ret;
    }
    bool ISO9660DirectoryEntry::operator==(const ISO9660DirectoryEntry& other) const {
        return GetName() == other.GetName();
    }
    bool ISO9660DirectoryEntry::operator!=(const ISO9660DirectoryEntry& other) const {
        return !(*this == other);
    }
}