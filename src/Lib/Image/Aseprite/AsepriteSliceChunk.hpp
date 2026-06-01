#ifndef MathLib_Image_Aseprite_SliceChunk_H
#define MathLib_Image_Aseprite_SliceChunk_H
#include "AsepriteChunkHeader.hpp"
#include "AsepriteString.hpp"

namespace MathLib {
    struct AsepriteSliceChunk : AsepriteChunkHeader {
        enum class Flag : uint8_t {
            Is9PatchSlice = 0,
            HasPivotInformation,
        };
        uint32_t sliceKeyCount;
        uint64_t flags;
        AsepriteString name;
    } __attribute__((packed));
}

#endif