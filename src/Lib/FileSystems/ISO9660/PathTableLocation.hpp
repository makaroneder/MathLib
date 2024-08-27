#ifndef PathTableLocation_H
#define PathTableLocation_H
#include <stdint.h>

struct PathTableLocation {
    uint32_t location;
    uint32_t optionalLocation;
} __attribute__((packed));

#endif