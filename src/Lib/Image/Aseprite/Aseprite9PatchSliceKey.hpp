#ifndef MathLib_Image_Aseprite_9PatchSliceKey_H
#define MathLib_Image_Aseprite_9PatchSliceKey_H
#include <stdint.h>

namespace MathLib {
    struct Aseprite9PatchSliceKey {
        int32_t x;
        int32_t y;
        uint32_t width;
        uint32_t height;
    } __attribute__((packed));
}

#endif