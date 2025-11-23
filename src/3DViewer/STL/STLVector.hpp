#ifndef STLVector_H
#define STLVector_H
#include <Math/Vector3.hpp>

struct STLVector {
    float x;
    float y;
    float z;

    STLVector(void);
    STLVector(float x, float y, float z);
    template <typename T>
    static STLVector FromVector3(const MathLib::Vector3<T>& vector) {
        return STLVector(
            vector.x,
            vector.y,
            vector.z
        );
    }
} __attribute__((packed));

#endif