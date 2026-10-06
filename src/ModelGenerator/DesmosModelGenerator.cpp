#include "DesmosModelGenerator.hpp"
#include <iostream>

size_t DesmosModelGenerator::Vertex(const MathLib::Vector4& vertex) {
    std::cout << "v_" << firstVertex << " = (" << vertex.data[0] << ", " << vertex.data[1] << ", " << vertex.data[2] << ')' << std::endl;
    return firstVertex++;
}
void DesmosModelGenerator::Face(size_t a, size_t b, size_t c) {
    std::cout << "triangle(v_" << a << ", v_" << b << ", v_" << c << ')' << std::endl;
}