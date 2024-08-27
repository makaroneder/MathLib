#ifndef BootRecord_H
#define BootRecord_H
#include "VolumeDescriptor.hpp"

struct BootRecord : VolumeDescriptor {
    char systemIdentifier[32];
    char bootIdentifier[32];
    uint32_t elToritoBootCatalog;
    uint8_t reserved[1973];
} __attribute__((packed));

#endif