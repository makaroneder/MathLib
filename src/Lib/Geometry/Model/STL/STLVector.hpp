#ifndef MathLib_Geometry_Model_STL_Vector_H
#define MathLib_Geometry_Model_STL_Vector_H
#include "../../../Math/Vector3.hpp"

namespace MathLib {
    struct STLVector {
        float x;
        float y;
        float z;

        STLVector(void);
        STLVector(float x, float y, float z);
        template <typename T>
        static STLVector FromVector3(const Vector3<T>& vector) {
            return STLVector(
                vector.x,
                vector.y,
                vector.z
            );
        }
    } __attribute__((packed));
}

#endif