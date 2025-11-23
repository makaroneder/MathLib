#ifndef MathLib_FileSystem_ISO9660_DirectoryEntry_H
#define MathLib_FileSystem_ISO9660_DirectoryEntry_H
#include "ISO9660LittleAndBigEndian.hpp"
#include "ISO9660DirectoryEntryDate.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    struct ISO9660DirectoryEntry {
        uint8_t length;
        uint8_t extendedAttributeRecordLength;
        ISO9660LittleAndBigEndian<uint32_t> extent;
        ISO9660LittleAndBigEndian<uint32_t> bytesPerExtent;
        ISO9660DirectoryEntryDate date;
        bool hidden : 1;
        bool directory : 1;
        bool associatedFile : 1;
        bool formatInformationInExtendedAttributeRecord : 1;
        bool permissionsInExtendedAttributeRecord : 1;
        uint8_t reserved : 2;
        bool notFinalDirectory : 1;
        uint8_t interleaveUnitSize;
        uint8_t interleaveGapSize;
        ISO9660LittleAndBigEndian<uint16_t> volumeNumber;
        uint8_t nameLength;
        char name[1];

        ISO9660DirectoryEntry(void);
        [[nodiscard]] bool IsValid(void) const;
        [[nodiscard]] String GetName(void) const;
        [[nodiscard]] bool operator==(const ISO9660DirectoryEntry& other) const;
        [[nodiscard]] bool operator!=(const ISO9660DirectoryEntry& other) const;
    } __attribute__((packed));
}

#endif