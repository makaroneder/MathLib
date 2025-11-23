#ifndef QuakeMDLSkinGroup_H
#define QuakeMDLSkinGroup_H
#include <stdint.h>

struct QuakeMDLSkinGroup {
    uint32_t pics;
    float time[];
} __attribute__((packed));

#endif