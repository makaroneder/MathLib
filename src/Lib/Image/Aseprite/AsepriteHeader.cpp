#include "AsepriteHeader.hpp"
#include "../../Benchmark.hpp"

namespace MathLib {
    AsepriteHeader::AsepriteHeader(void) : size(0), signature(expectedSignature), frames(0), width(0), height(0), bpp(0), hasOpacityLayer(false), reserved1(0), speed(0), reserved2(0), paletteEntry(0), colors(0), pixelWidth(0), pixelHeight(0), gridX(0), gridY(0), gridWidth(0), gridHeight(0) {
        EmptyBenchmark
    }
    bool AsepriteHeader::IsValid(void) const {
        StartAndReturnFromBenchmark(signature == expectedSignature);
    }
}