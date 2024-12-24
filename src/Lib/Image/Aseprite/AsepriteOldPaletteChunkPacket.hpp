#ifndef MathLib_Image_Aseprite_OldPaletteChunkPacket_H
#define MathLib_Image_Aseprite_OldPaletteChunkPacket_H
#include "AsepriteOldPaletteChunkPacketColor.hpp"

namespace MathLib {
    struct AsepriteOldPaletteChunkPacket {
        uint8_t entriesToSkip;
        uint8_t colorCount;
        AsepriteOldPaletteChunkPacketColor colors[];

        uint16_t GetColorCount(void) const;
    } __attribute__((packed));
}

#endif