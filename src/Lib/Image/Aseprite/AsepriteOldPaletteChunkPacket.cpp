#include "AsepriteOldPaletteChunkPacket.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    uint16_t AsepriteOldPaletteChunkPacket::GetColorCount(void) const {
        StartBenchmark
        ReturnFromBenchmark(colorCount ? colorCount : 256);
    }
}