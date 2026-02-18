#ifndef MathLib_Image_Aseprite_PaletteChunk_H
#define MathLib_Image_Aseprite_PaletteChunk_H
#include "AsepriteChunkHeader.hpp"

namespace MathLib {
    struct AsepritePaletteChunk : AsepriteChunkHeader {
        uint32_t entryCount;
        uint32_t first;
        uint32_t last;
        uint64_t reserved;
    } __attribute__((packed));
}

#endif