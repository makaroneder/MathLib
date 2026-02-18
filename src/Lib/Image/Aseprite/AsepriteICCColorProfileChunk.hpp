#ifndef MathLib_Image_Aseprite_ICCColorProfileChunk_H
#define MathLib_Image_Aseprite_ICCColorProfileChunk_H
#include "AsepriteColorProfileChunk.hpp"

namespace MathLib {
    struct AsepriteICCColorProfileChunk : AsepriteColorProfileChunk {
        // TODO: http://www.color.org/ICC1V42.pdf
        uint32_t size;
        uint8_t data[];
    } __attribute__((packed));
}

#endif