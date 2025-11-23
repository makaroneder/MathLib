#ifndef MathLib_FileSystem_ISO9660_PrimaryVolumeDescriptorDate_H
#define MathLib_FileSystem_ISO9660_PrimaryVolumeDescriptorDate_H
#include <stdint.h>

namespace MathLib {
    struct ISO9660PrimaryVolumeDescriptorDate {
        char year[4];
        char month[2];
        char day[2];
        char hour[2];
        char minute[2];
        char second[2];
        char hundredthsOfSecond[2];
        uint8_t timeZoneOffset;

        ISO9660PrimaryVolumeDescriptorDate(void);
        [[nodiscard]] bool IsValid(void) const;
    } __attribute__((packed));
}

#endif