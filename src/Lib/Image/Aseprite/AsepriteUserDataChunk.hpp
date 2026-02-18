#ifndef MathLib_Image_Aseprite_UserDataChunk_H
#define MathLib_Image_Aseprite_UserDataChunk_H
#include "AsepriteChunkHeader.hpp"

namespace MathLib {
    struct AsepriteUserDataChunk : AsepriteChunkHeader {
        bool hasText : 1;
        bool hasColor : 1;
        bool hasProperties : 1;
        uint32_t reserved : 29;
    } __attribute__((packed));
}

#endif