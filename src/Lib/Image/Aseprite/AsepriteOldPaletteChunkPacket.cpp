#include "AsepriteOldPaletteChunkPacket.hpp"
#include "../../Benchmark.hpp"

namespace MathLib {
    uint16_t AsepriteOldPaletteChunkPacket::GetColorCount(void) const {
        StartAndReturnFromBenchmark(colorCount ? colorCount : 256);
    }
}