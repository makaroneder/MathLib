#ifndef MathLib_Image_Aseprite_Tag_H
#define MathLib_Image_Aseprite_Tag_H
#include "AsepriteString.hpp"

namespace MathLib {
    struct AsepriteTag {
        enum class Direction : uint8_t {
            Forward = 0,
            Reverse,
            PingPong,
            ReversePingPong,
        };
        uint16_t startFrame;
        uint16_t endFrame;
        Direction direction;
        uint16_t repeat;
        uint16_t reserved1[3];
        uint8_t rgb[3];
        uint8_t reserved2;
        AsepriteString name;
    } __attribute__((packed));
}

#endif