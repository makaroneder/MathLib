#ifndef MathLib_Image_TGA_Footer_H
#define MathLib_Image_TGA_Footer_H
#include <stdint.h>

namespace MathLib {
    struct TGAFooter {
        static constexpr const char* expectedSignature = "TRUEVISION-XFILE.";
        uint32_t extensionOffset;
        uint32_t developerOffset;
        char signature[18];
    } __attribute__((packed));
}

#endif