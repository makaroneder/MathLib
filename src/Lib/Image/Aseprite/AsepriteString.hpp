#ifndef MathLib_Image_Aseprite_String_H
#define MathLib_Image_Aseprite_String_H
#include <stdint.h>

namespace MathLib {
    struct AsepriteString {
        uint16_t size;
        char data[];

        AsepriteString(void);
    } __attribute__((packed));
}

#endif