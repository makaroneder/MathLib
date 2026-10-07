#include "ModelGenerator.hpp"
#include <FileSystem/FileSystem.hpp>
#include <Logger.hpp>

void Main(int, char**, MathLib::FileSystem&) {
    MathLib::Array<MathLib::Vector4> vertices;
    MathLib::Array<size_t> faces;
    ModelGenerator generator = ModelGenerator(vertices, faces,
        MathLib::Vector4(1, 0, 0, 0),
        MathLib::Vector4(0, 1, 0, 0),
        MathLib::Vector4(0, 0, 1, 0),
        MathLib::Vector4(0, 0, 0, 1)
    );
    if (!generator.PrismWithIsoscelesTriangleBase(MathLib::Vector4(0, 0, 0, 1), 0.375, 0.6, 1)) MathLib::Panic("Failed to generate model");
    if (!generator.PrismWithIsoscelesTriangleBase(MathLib::Vector4(0, 0, -1, 1), 0.25, 0.3, 2)) MathLib::Panic("Failed to generate model");
    for (size_t i = 0; i < vertices.GetSize(); i++) {
        LogChar('v');
        for (uint8_t j = 0; j < 4; j++) {
            LogChar(' ');
            LogString(MathLib::ToString(vertices.AtUnsafe(i).data[j]));
        }
        LogChar('\n');
    }
    for (size_t i = 0; i < faces.GetSize(); i += 3) {
        LogChar('f');
        for (uint8_t j = 0; j < 3; j++) {
            LogChar(' ');
            LogString(MathLib::ToString(faces.AtUnsafe(i + j) + 1, 10));
        }
        LogChar('\n');
    }
}