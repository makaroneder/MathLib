#include "AsepriteOldPaletteChunkPacket.hpp"

namespace MathLib {
    uint16_t AsepriteOldPaletteChunkPacket::GetColorCount(void) const {
        return colorCount ? colorCount : 256;
    }
}