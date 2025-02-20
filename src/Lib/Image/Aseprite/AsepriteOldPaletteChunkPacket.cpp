#include "AsepriteOldPaletteChunkPacket.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    uint16_t AsepriteOldPaletteChunkPacket::GetColorCount(void) const {
        StartAndReturnFromBenchmark(colorCount ? colorCount : 256);
    }
}