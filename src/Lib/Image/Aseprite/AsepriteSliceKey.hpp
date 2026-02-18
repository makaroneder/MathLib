#ifndef MathLib_Image_Aseprite_SliceKey_H
#define MathLib_Image_Aseprite_SliceKey_H
#include <stdint.h>

namespace MathLib {
    struct AsepriteSliceKey {
        uint32_t frame;
        int32_t x;
        int32_t y;
        uint32_t width;
        uint32_t height;
    } __attribute__((packed));
}

#endif