#ifndef MathLib_Geometry_Model_QuakeMDL_SkinType_H
#define MathLib_Geometry_Model_QuakeMDL_SkinType_H
#include <stdint.h>

namespace MathLib {
    enum class QuakeMDLSkinType : uint32_t {
        Single = 0,
        Group,
    };
}

#endif