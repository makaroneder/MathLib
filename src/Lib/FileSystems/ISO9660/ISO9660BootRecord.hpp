#ifndef FileSystems_ISO9660_BootRecord_H
#define FileSystems_ISO9660_BootRecord_H
#include "ISO9660VolumeDescriptor.hpp"

struct ISO9660BootRecord : ISO9660VolumeDescriptor {
    char systemIdentifier[32];
    char bootIdentifier[32];
    uint32_t elToritoBootCatalog;
    uint8_t reserved[1973];
} __attribute__((packed));

#endif