#include "WavefrontObjModelGenerator.hpp"
#include <FileSystem/FileSystem.hpp>

void Main(int, char**, MathLib::FileSystem&) {
    WavefrontObjModelGenerator state;
    state.PrismWithIsoscelesTriangleBase(MathLib::Vector4(0, 0, 0, 1), 1, 2, 3);
}