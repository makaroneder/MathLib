#ifndef MathLib_Image_Aseprite_PivotSliceKey_H
#define MathLib_Image_Aseprite_PivotSliceKey_H
#include <stdint.h>

namespace MathLib {
    struct AsepritePivotSliceKey {
        int32_t x;
        int32_t y;
    } __attribute__((packed));
}

#endif