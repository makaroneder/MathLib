#include "WavefrontObjModelGenerator.hpp"
#include <iostream>

size_t WavefrontObjModelGenerator::Vertex(const MathLib::Vector4& vertex) {
    std::cout << "v " << vertex.data[0] << ' ' << vertex.data[1] << ' ' << vertex.data[2] << ' ' << vertex.data[3] << std::endl;
    return firstVertex++;
}
void WavefrontObjModelGenerator::Face(size_t a, size_t b, size_t c) {
    std::cout << "f " << a + 1 << ' ' << b + 1 << ' ' << c + 1 << std::endl;
}