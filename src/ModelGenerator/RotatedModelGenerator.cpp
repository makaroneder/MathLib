#include "RotatedModelGenerator.hpp"

RotatedModelGenerator::RotatedModelGenerator(ModelGenerator& base, float x, float y, float z) : base(base), rotation(MathLib::Rotate(x, y, z)) {}
MathLib::Vector4 RotatedModelGenerator::GetX(void) const {
    return rotation * base.GetX();
}
MathLib::Vector4 RotatedModelGenerator::GetY(void) const {
    return rotation * base.GetY();
}
MathLib::Vector4 RotatedModelGenerator::GetZ(void) const {
    return rotation * base.GetZ();
}
MathLib::Vector4 RotatedModelGenerator::GetW(void) const {
    return rotation * base.GetW();
}
size_t RotatedModelGenerator::Vertex(const MathLib::Vector4& vertex) {
    return base.Vertex(vertex);
}
void RotatedModelGenerator::Face(size_t a, size_t b, size_t c) {
    base.Face(a, b, c);
}