#ifndef MathLib_Image_TGA_ExtensionArea_H
#define MathLib_Image_TGA_ExtensionArea_H
#include <stdint.h>

namespace MathLib {
    struct TGAExtensionArea {
        uint16_t extensionSize;
        char authorName[41];
        char authorComments[324];
        uint16_t timestamp[6];
        char jobName[41];
        uint16_t jobTime[3];
        char softwareId[41];
        uint16_t softwareVersion;
        uint32_t keyColor;
        uint16_t pixelAspectRatio[2];
        uint16_t gammaValue[2];
        uint32_t colorCorrectionOffset;
        uint32_t postageStampOffset;
        uint32_t scanLineOffset;
        uint8_t attributesType;
    } __attribute__((packed));
}

#endif