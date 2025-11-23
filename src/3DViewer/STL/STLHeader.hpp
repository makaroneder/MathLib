#ifndef STLHeader_H
#define STLHeader_H
#include <stdint.h>

struct STLHeader {
    uint64_t reserved[10];
    uint32_t triangleCount;

    STLHeader(uint32_t triangleCount = 0);
} __attribute__((packed));

#endif