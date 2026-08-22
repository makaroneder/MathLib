#ifndef MathLib_3D_Vertex_H
#define MathLib_3D_Vertex_H
#include "../Math/Vector3.hpp"

namespace MathLib {
    struct Vertex;
    struct Vertex : Comparable<Vertex> {
        Vector3<float> position;
        Vector3<float> normal;
        Vector3<float> texturePosition;

        Vertex(void);
        Vertex(const Vector3<float>& position, const Vector3<float>& normal, const Vector3<float>& texturePosition);
        [[nodiscard]] virtual bool Equals(const Vertex& other) const override;
    };
}

#endif