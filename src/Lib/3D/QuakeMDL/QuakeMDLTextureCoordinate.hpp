#ifndef MathLib_3D_QuakeMDL_TextureCoordinate_H
#define MathLib_3D_QuakeMDL_TextureCoordinate_H
#include <stdint.h>

namespace MathLib {
    struct QuakeMDLTextureCoordinate {
        uint32_t isOnBoundary;
        uint32_t s;
        uint32_t t;
    } __attribute__((packed));
}

#endif