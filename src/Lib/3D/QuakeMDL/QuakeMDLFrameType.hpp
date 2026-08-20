#ifndef MathLib_3D_QuakeMDL_FrameType_H
#define MathLib_3D_QuakeMDL_FrameType_H
#include <stdint.h>

namespace MathLib {
    enum class QuakeMDLFrameType : uint32_t {
        Single = 0,
        Group,
    };
}

#endif