#ifndef TGAFooter_H
#define TGAFooter_H
#include <stdint.h>

struct TGAFooter {
    static constexpr const char* expectedSignature = "TRUEVISION-XFILE.";
    uint32_t extensionOffset;
    uint32_t developerOffset;
    char signature[18];
} __attribute__((packed));

#endif