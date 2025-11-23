#ifndef QuakeMDLFrameGroup_H
#define QuakeMDLFrameGroup_H
#include "QuakeMDLFrame.hpp"

struct QuakeMDLFrameGroup {
    uint32_t frames;
    QuakeMDLVertex min;
    QuakeMDLVertex max;
    float time[];
} __attribute__((packed));

#endif