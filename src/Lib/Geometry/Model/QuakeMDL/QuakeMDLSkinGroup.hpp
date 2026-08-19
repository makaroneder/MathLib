#ifndef MathLib_Geometry_Model_QuakeMDL_SkinGroup_H
#define MathLib_Geometry_Model_QuakeMDL_SkinGroup_H
#include <stdint.h>

namespace MathLib {
    struct QuakeMDLSkinGroup {
        uint32_t pics;
        float time[];
    } __attribute__((packed));
}

#endif