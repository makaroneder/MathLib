#ifndef MathLib_Image_Aseprite_TagsChunk_H
#define MathLib_Image_Aseprite_TagsChunk_H
#include "AsepriteChunkHeader.hpp"

namespace MathLib {
    struct AsepriteTagsChunk : AsepriteChunkHeader {
        uint16_t tagCount;
        uint64_t reserved;
    } __attribute__((packed));
}

#endif