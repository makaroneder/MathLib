#ifndef STLTriangle_H
#define STLTriangle_H
#include "STLVector.hpp"
#include "../Triangle.hpp"

struct STLTriangle {
    STLVector normal;
    STLVector vertices[3];
    uint16_t attributeByteCount;

    STLTriangle(void);
    template <typename T>
    static STLTriangle FromTriangle(const Triangle<T>& triangle) {
        STLTriangle ret;
        ret.normal = STLVector::FromVector3<T>(triangle.GetNormal());
        for (uint8_t i = 0; i < SizeOfArray(vertices); i++) {
            ret.vertices[i].x = triangle.vertices[i].x;
            ret.vertices[i].y = triangle.vertices[i].y;
            ret.vertices[i].z = triangle.vertices[i].z;
        }
        return ret;
    }
} __attribute__((packed));

#endif