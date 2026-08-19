#ifndef MathLib_Geometry_Model_QuakeMDL_Frame_H
#define MathLib_Geometry_Model_QuakeMDL_Frame_H
#include "QuakeMDLVertex.hpp"

namespace MathLib {
    struct QuakeMDLFrame {
        QuakeMDLVertex minBoundingBox;
        QuakeMDLVertex maxBoundingBox;
        char name[16];
        QuakeMDLVertex vertices[];
    } __attribute__((packed));
}

#endif