#ifndef FileSystems_ISO9660_PrimaryVolumeDescriptorDate_H
#define FileSystems_ISO9660_PrimaryVolumeDescriptorDate_H
#include <stdint.h>

struct ISO9660PrimaryVolumeDescriptorDate {
    char year[4];
    char month[2];
    char day[2];
    char hour[2];
    char minute[2];
    char second[2];
    char hundredthsOfSecond[2];
    uint8_t timeZoneOffset;

    bool IsValid(void) const;
} __attribute__((packed));

#endif