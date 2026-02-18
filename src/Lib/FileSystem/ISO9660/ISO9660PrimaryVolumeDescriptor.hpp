#ifndef MathLib_FileSystem_ISO9660_PrimaryVolumeDescriptor_H
#define MathLib_FileSystem_ISO9660_PrimaryVolumeDescriptor_H
#include "ISO9660PrimaryVolumeDescriptorDate.hpp"
#include "ISO9660PathTableLocation.hpp"
#include "ISO9660VolumeDescriptor.hpp"
#include "ISO9660DirectoryEntry.hpp"

namespace MathLib {
    struct ISO9660PrimaryVolumeDescriptor : ISO9660VolumeDescriptor {
        uint8_t reserved1;
        char systemIdentifier[32];
        char volumeIdentifier[32];
        uint64_t reserved2;
        ISO9660LittleAndBigEndian<uint32_t> sectors;
        uint64_t reserved3[4];
        ISO9660LittleAndBigEndian<uint16_t> sizeOfSet;
        ISO9660LittleAndBigEndian<uint16_t> volumeNumber;
        ISO9660LittleAndBigEndian<uint16_t> bytesPerSector;
        ISO9660LittleAndBigEndian<uint32_t> bytesPerPathTable;
        ISO9660LittleAndBigEndian<ISO9660PathTableLocation> pathTable;
        ISO9660DirectoryEntry root;
        char volumeSetIdentifier[128];
        char publisherIdentifier[128];
        char dataPreparerIdentifier[128];
        char applicationIdentifier[128];
        char copyrightPath[37];
        char abstractPath[37];
        char bibliographicPath[37];
        ISO9660PrimaryVolumeDescriptorDate creationDate;
        ISO9660PrimaryVolumeDescriptorDate modificationDate;
        ISO9660PrimaryVolumeDescriptorDate expirationDate;
        ISO9660PrimaryVolumeDescriptorDate effectiveDate;
        uint8_t fileStructureVersion;
        uint8_t reserved4;
        uint8_t osSpecific[512];
        uint8_t reserved5[653];

        ISO9660PrimaryVolumeDescriptor(void);
        [[nodiscard]] bool IsValid(void) const;
    } __attribute__((packed));
}

#endif