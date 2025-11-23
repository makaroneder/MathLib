#ifndef QuakeMDLVertex_H
#define QuakeMDLVertex_H
#include <stdint.h>

struct QuakeMDLVertex {
    uint8_t x;
    uint8_t y;
    uint8_t z;
    uint8_t normalIndex;
} __attribute__((packed));

#endif