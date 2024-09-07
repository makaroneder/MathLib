#ifndef DirectoryEntry_H
#define DirectoryEntry_H
#include "LittleAndBigEndian.hpp"
#include "DirectoryEntryDate.hpp"
#include "../../Typedefs.hpp"

struct DirectoryEntry {
    uint8_t length;
    uint8_t extendedAttributeRecordLength;
    LittleAndBigEndian<uint32_t> extent;
    LittleAndBigEndian<uint32_t> bytesPerExtent;
    DirectoryEntryDate date;
    bool hidden : 1;
    bool directory : 1;
    bool associatedFile : 1;
    bool formatInformationInExtendedAttributeRecord : 1;
    bool permissionsInExtendedAttributeRecord : 1;
    uint8_t reserved : 2;
    bool notFinalDirectory : 1;
    uint8_t interleaveUnitSize;
    uint8_t interleaveGapSize;
    LittleAndBigEndian<uint16_t> volumeNumber;
    uint8_t nameLength;
    char name[1];

    bool IsValid(void) const;
    String GetName(void) const;
} __attribute__((packed));

#endif