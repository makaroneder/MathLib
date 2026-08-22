#include "Vertex.hpp"

namespace MathLib {
    Vertex::Vertex(void) : position(), normal(), texturePosition() {}
    Vertex::Vertex(const Vector3<float>& position, const Vector3<float>& normal, const Vector3<float>& texturePosition) : position(position), normal(normal), texturePosition(texturePosition) {}
    bool Vertex::Equals(const Vertex& other) const {
        return position == other.position && normal == other.normal && texturePosition == other.texturePosition;
    }
}