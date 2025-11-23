#ifndef QuakeMDLFrame_H
#define QuakeMDLFrame_H
#include "QuakeMDLVertex.hpp"

struct QuakeMDLFrame {
    QuakeMDLVertex minBoundingBox;
    QuakeMDLVertex maxBoundingBox;
    char name[16];
    QuakeMDLVertex vertices[];
} __attribute__((packed));

#endif