#ifndef MathLib_Image_TGA_Header_H
#define MathLib_Image_TGA_Header_H
#include <stdint.h>

namespace MathLib {
    struct TGAHeader {
        uint8_t idLength;
        uint8_t colorMapType;
        uint8_t imageType;
        uint16_t colorMapFirstEntryIndex;
        uint16_t colorMapLength;
        uint8_t colorMapEntrySize;
        uint16_t xOrigin;
        uint16_t yOrigin;
        uint16_t width;
        uint16_t height;
        uint8_t pixelDepth;
        uint8_t imageDescriptor;
    } __attribute__((packed));
}

#endif