#ifndef MathLib_Image_TGA_Header_H
#define MathLib_Image_TGA_Header_H
#include <stdint.h>

namespace MathLib {
    struct TGAHeader {
        enum class ImageType : uint8_t {
            None = 0,
            UncompressedColorMapped,
            UncompressedTrueColor,
            UncompressedGrayscale,
            RunLengthEncodedColorMapped,
            RunLengthEncodedTrueColor,
            RunLengthEncodedGrayscale,
        };
        uint8_t idLength;
        uint8_t colorMapType;
        ImageType imageType;
        uint16_t colorMapFirstEntryIndex;
        uint16_t colorMapLength;
        uint8_t colorMapEntrySize;
        uint16_t xOrigin;
        uint16_t yOrigin;
        uint16_t width;
        uint16_t height;
        uint8_t pixelDepth;
        uint8_t alphaDepth : 4;
        bool rightLeft : 1;
        bool topBottom : 1;
        uint8_t reserved : 2;

        TGAHeader(void);
        TGAHeader(uint8_t idLength, ImageType imageType, uint16_t width, uint16_t height, uint8_t pixelDepth);
    } __attribute__((packed));
}

#endif