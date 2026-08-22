#ifndef MathLib_3D_QuakeMDL_Vertex_H
#define MathLib_3D_QuakeMDL_Vertex_H
#include "../../Math/Vector3.hpp"
#include "QuakeMDLVector.hpp"
#include <stdint.h>

namespace MathLib {
    struct QuakeMDLVertex {
        uint8_t data[3];
        uint8_t normalIndex;

        [[nodiscard]] Vector3<float> ToVector3(const QuakeMDLVector& scale, const QuakeMDLVector& translate) const;
        [[nodiscard]] Vector3<float> GetNormal(void) const;
    } __attribute__((packed));
}

#endif