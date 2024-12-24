#ifndef MathLib_Image_Aseprite_FrameHeader_H
#define MathLib_Image_Aseprite_FrameHeader_H
#include <stdint.h>

namespace MathLib {
    struct AsepriteFrameHeader {
        static constexpr uint16_t expectedSignature = 0xf1fa;
        uint32_t size;
        uint16_t signature;
        uint16_t chunks16;
        uint16_t duration;
        uint16_t reserved;
        uint32_t chunks32;
    } __attribute__((packed));
}

#endif