#ifndef MathLib_3D_QuakeMDL_Vertex_H
#define MathLib_3D_QuakeMDL_Vertex_H
#include <stdint.h>

namespace MathLib {
    struct QuakeMDLVertex {
        uint8_t data[3];
        uint8_t normalIndex;
    } __attribute__((packed));
    extern float quakeMDLNormalVectors[];
}

#endif