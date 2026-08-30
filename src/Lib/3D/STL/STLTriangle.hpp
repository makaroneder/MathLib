#ifndef MathLib_3D_STL_Triangle_H
#define MathLib_3D_STL_Triangle_H
#include "STLVector.hpp"
#include "../../Geometry/Triangle.hpp"

namespace MathLib {
    struct STLTriangle {
        STLVector normal;
        STLVector vertices[3];
        uint16_t attributeByteCount;

        STLTriangle(void);
        STLTriangle(const STLVector& a, const STLVector& b, const STLVector& c);
        STLTriangle(const Vector3<float>& a, const Vector3<float>& b, const Vector3<float>& c);
        [[nodiscard]] Triangle<float> ToTriangle(void) const;
    } __attribute__((packed));
}

#endif