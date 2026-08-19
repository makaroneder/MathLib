#ifndef MathLib_Geometry_Model_QuakeMDL_Vertex_H
#define MathLib_Geometry_Model_QuakeMDL_Vertex_H
#include <stdint.h>

namespace MathLib {
    struct QuakeMDLVertex {
        uint8_t x;
        uint8_t y;
        uint8_t z;
        uint8_t normalIndex;
    } __attribute__((packed));
}

#endif