#ifndef FileSystems_ISO9660_PathTableLocation_H
#define FileSystems_ISO9660_PathTableLocation_H
#include <stdint.h>

struct ISO9660PathTableLocation {
    uint32_t location;
    uint32_t optionalLocation;
} __attribute__((packed));

#endif