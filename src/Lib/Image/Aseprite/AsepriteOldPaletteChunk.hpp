#ifndef MathLib_Image_Aseprite_OldPaletteChunk_H
#define MathLib_Image_Aseprite_OldPaletteChunk_H
#include "AsepriteOldPaletteChunkPacket.hpp"
#include "AsepriteChunkHeader.hpp"

namespace MathLib {
    struct AsepriteOldPaletteChunk : AsepriteChunkHeader {
        uint16_t packetCount;
        AsepriteOldPaletteChunkPacket packets[];
    } __attribute__((packed));
}

#endif