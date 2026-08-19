#ifndef MathLib_Geometry_Model_QuakeMDL_Triangle_H
#define MathLib_Geometry_Model_QuakeMDL_Triangle_H
#include <stdint.h>

namespace MathLib {
    struct QuakeMDLTriangle {
        uint32_t isFrontFace;
        uint32_t vertices[3];
    } __attribute__((packed));
}

#endif