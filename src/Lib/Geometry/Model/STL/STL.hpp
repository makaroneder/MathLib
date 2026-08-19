#ifndef MathLib_Geometry_Model_STL_H
#define MathLib_Geometry_Model_STL_H
#include "STLHeader.hpp"
#include "../Model3D.hpp"
#include "STLTriangle.hpp"

namespace MathLib {
    template <typename T>
    struct STL : Model3D<T>, Saveable {
        [[nodiscard]] virtual bool Save(Writable& file) const override {
            if (!file.Write<STLHeader>(STLHeader(this->triangles.GetSize()))) return false;
            for (const Triangle<T>& triangle : this->triangles)
                if (!file.Write<STLTriangle>(STLTriangle::FromTriangle<T>(triangle))) return false;
            return true;
        }
        [[nodiscard]] virtual bool Load(Readable& file) override {
            STLHeader header;
            if (!file.Read<STLHeader>(header)) return false;
            this->triangles = Array<Triangle<T>>(header.triangleCount);
            for (Triangle<T>& triangle : this->triangles) {
                STLTriangle stlTriangle;
                if (!file.Read<STLTriangle>(stlTriangle)) return false;
                for (uint8_t i = 0; i < SizeOfArray(triangle.vertices); i++)
                    triangle.vertices[i] = Vector3<T>(stlTriangle.vertices[i].x, stlTriangle.vertices[i].y, stlTriangle.vertices[i].z);
            }
            return true;
        }
    };
}

#endif