#include "STLTriangle.hpp"

namespace MathLib {
    STLTriangle::STLTriangle(void) : normal(), attributeByteCount(0) {}
    STLTriangle::STLTriangle(const STLVector& a, const STLVector& b, const STLVector& c) : normal(), vertices { a, b, c, }, attributeByteCount(0) {
        normal = ToTriangle().GetNormal();
    }
    STLTriangle::STLTriangle(const Vector3<float>& a, const Vector3<float>& b, const Vector3<float>& c) : STLTriangle(STLVector(a), STLVector(b), STLVector(c)) {}
    Triangle<float> STLTriangle::ToTriangle(void) const {
        return Triangle<float>(vertices[0].ToVector3(), vertices[1].ToVector3(), vertices[2].ToVector3());
    }
}