#ifndef MathLib_Image_Aseprite_ExtraCelChunk_H
#define MathLib_Image_Aseprite_ExtraCelChunk_H
#include "AsepriteChunkHeader.hpp"

namespace MathLib {
    struct AsepriteExtraCelChunk : AsepriteChunkHeader {
        bool preciseBounds : 1;
        uint32_t reserved1 : 31;
        uint32_t x;
        uint32_t y;
        uint32_t width;
        uint32_t height;
        uint64_t reserved2[2];
    } __attribute__((packed));
}

#endif