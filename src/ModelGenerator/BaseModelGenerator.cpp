#include "BaseModelGenerator.hpp"

MathLib::Vector4 BaseModelGenerator::GetX(void) const {
    return MathLib::Vector4(1, 0, 0, 0);
}
MathLib::Vector4 BaseModelGenerator::GetY(void) const {
    return MathLib::Vector4(0, 1, 0, 0);
}
MathLib::Vector4 BaseModelGenerator::GetZ(void) const {
    return MathLib::Vector4(0, 0, 1, 0);
}
MathLib::Vector4 BaseModelGenerator::GetW(void) const {
    return MathLib::Vector4(0, 0, 0, 1);
}