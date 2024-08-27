#ifndef PrimaryVolumeDescriptor_H
#define PrimaryVolumeDescriptor_H
#include "PrimaryVolumeDescriptorDate.hpp"
#include "PathTableLocation.hpp"
#include "VolumeDescriptor.hpp"
#include "DirectoryEntry.hpp"

struct PrimaryVolumeDescriptor : VolumeDescriptor {
    uint8_t reserved1;
    char systemIdentifier[32];
    char volumeIdentifier[32];
    uint64_t reserved2;
    LittleAndBigEndian<uint32_t> sectors;
    uint64_t reserved3[4];
    LittleAndBigEndian<uint16_t> sizeOfSet;
    LittleAndBigEndian<uint16_t> volumeNumber;
    LittleAndBigEndian<uint16_t> bytesPerSector;
    LittleAndBigEndian<uint32_t> bytesPerPathTable;
    LittleAndBigEndian<PathTableLocation> pathTable;
    DirectoryEntry root;
    char volumeSetIdentifier[128];
    char publisherIdentifier[128];
    char dataPreparerIdentifier[128];
    char applicationIdentifier[128];
    char copyrightPath[37];
    char abstractPath[37];
    char bibliographicPath[37];
    PrimaryVolumeDescriptorDate creationDate;
    PrimaryVolumeDescriptorDate modificationDate;
    PrimaryVolumeDescriptorDate expirationDate;
    PrimaryVolumeDescriptorDate effectiveDate;
    uint8_t fileStructureVersion;
    uint8_t reserved4;
    uint8_t osSpecific[512];
    uint8_t reserved5[653];

    bool IsValid(void) const;
} __attribute__((packed));

#endif