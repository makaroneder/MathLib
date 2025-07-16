#include "AsepriteImageCelChunk.hpp"
#include "../../Benchmark.hpp"

namespace MathLib {
    AsepriteImageCelChunk::AsepriteImageCelChunk(uint16_t width, uint16_t height, bool raw) : AsepriteCelChunk(2 * sizeof(uint16_t) + width * height * sizeof(uint32_t), raw ? AsepriteCelChunk::Type::Raw : AsepriteCelChunk::Type::CompressedImage), width(width), height(height) {
        EmptyBenchmark
    }
}
