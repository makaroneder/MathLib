#ifndef QuakeMDLTriangle_H
#define QuakeMDLTriangle_H
#include <stdint.h>

struct QuakeMDLTriangle {
    uint32_t isFrontFace;
    uint32_t vertices[3];
} __attribute__((packed));

#endif