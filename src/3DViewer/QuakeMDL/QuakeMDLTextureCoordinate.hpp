#ifndef QuakeMDLTextureCoordinate_H
#define QuakeMDLTextureCoordinate_H
#include <stdint.h>

struct QuakeMDLTextureCoordinate {
    uint32_t isOnBoundary;
    uint32_t s;
    uint32_t t;
} __attribute__((packed));

#endif