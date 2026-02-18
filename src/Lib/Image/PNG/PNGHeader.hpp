#ifndef MathLib_Image_PNG_Header_H
#define MathLib_Image_PNG_Header_H

namespace MathLib {
    struct PNGHeader {
        static constexpr const char* expectedSignature = "\x89PNG\r\n\x1a\n";
        char signature[8];

        PNGHeader(void);
        [[nodiscard]] bool IsValid(void) const;
    } __attribute__((packed));
}

#endif