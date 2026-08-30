#ifndef MathLib_3D_STL_Vector_H
#define MathLib_3D_STL_Vector_H
#include "../../Math/Vector3.hpp"

namespace MathLib {
    struct STLVector {
        float x;
        float y;
        float z;

        STLVector(void);
        STLVector(float x, float y, float z);
        STLVector(const Vector3<float>& vector);
        [[nodiscard]] Vector3<float> ToVector3(void) const;
    } __attribute__((packed));
}

#endif