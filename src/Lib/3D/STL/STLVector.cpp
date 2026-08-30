#include "STLVector.hpp"

namespace MathLib {
    STLVector::STLVector(void) : x(0), y(0), z(0) {}
    STLVector::STLVector(float x, float y, float z) : x(x), y(y), z(z) {}
    STLVector::STLVector(const Vector3<float>& vector) : x(vector.x), y(vector.y), z(vector.z) {}
    Vector3<float> STLVector::ToVector3(void) const {
        return Vector3<float>(x, y, z);
    }
}