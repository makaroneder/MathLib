#ifndef MathLib_3D_QuakeMDL_FrameGroup_H
#define MathLib_3D_QuakeMDL_FrameGroup_H
#include "QuakeMDLFrame.hpp"

namespace MathLib {
    struct QuakeMDLFrameGroup {
        uint32_t frames;
        QuakeMDLVertex min;
        QuakeMDLVertex max;
        float time[];
    } __attribute__((packed));
}

#endif