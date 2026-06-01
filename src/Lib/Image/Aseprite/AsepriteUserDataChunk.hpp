#ifndef MathLib_Image_Aseprite_UserDataChunk_H
#define MathLib_Image_Aseprite_UserDataChunk_H
#include "AsepriteChunkHeader.hpp"

namespace MathLib {
    struct AsepriteUserDataChunk : AsepriteChunkHeader {
        enum class Flag : uint8_t {
            HasText = 0,
            HasColor,
            HasProperties,
        };
        uint32_t flags;
    } __attribute__((packed));
}

#endif