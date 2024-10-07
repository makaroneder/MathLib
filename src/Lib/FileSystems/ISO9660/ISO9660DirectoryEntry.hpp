#ifndef FileSystems_ISO9660_DirectoryEntry_H
#define FileSystems_ISO9660_DirectoryEntry_H
#include "ISO9660LittleAndBigEndian.hpp"
#include "ISO9660DirectoryEntryDate.hpp"
#include "../../Typedefs.hpp"

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

    bool IsValid(void) const;
    String GetName(void) const;
} __attribute__((packed));

#endif