#ifndef MathLib_Image_Aseprite_SliceChunk_H
#define MathLib_Image_Aseprite_SliceChunk_H
#include "AsepriteChunkHeader.hpp"
#include "AsepriteString.hpp"

namespace MathLib {
    struct AsepriteSliceChunk : AsepriteChunkHeader {
        uint32_t sliceKeyCount;
        bool is9PatchSlice : 1;
        bool hasPivotInformation : 1;
        uint64_t reserved : 62;
        AsepriteString name;
    } __attribute__((packed));
}

#endif