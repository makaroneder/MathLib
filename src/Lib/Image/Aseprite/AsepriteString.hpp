#ifndef MathLib_Image_Aseprite_String_H
#define MathLib_Image_Aseprite_String_H
#include "AsepriteChunkHeader.hpp"

namespace MathLib {
    struct AsepriteString {
        uint16_t size;
        char data[];
    } __attribute__((packed));
}

#endif