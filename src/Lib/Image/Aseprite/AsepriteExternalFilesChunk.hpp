#ifndef MathLib_Image_Aseprite_ExternalFilesChunk_H
#define MathLib_Image_Aseprite_ExternalFilesChunk_H
#include "AsepriteChunkHeader.hpp"

namespace MathLib {
    struct AsepriteExternalFilesChunk : AsepriteChunkHeader {
        uint32_t entryCount;
        uint64_t reserved;
    } __attribute__((packed));
}

#endif