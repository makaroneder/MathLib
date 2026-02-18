#ifndef MathLib_Cryptography_Compressor_XZ_BlockFlags_H
#define MathLib_Cryptography_Compressor_XZ_BlockFlags_H
#include <stdint.h>

namespace MathLib {
    struct XZBlockFlags {
        uint8_t filters : 2;
        uint8_t reserved : 4;
        bool hasCompressedSize : 1;
        bool hasUncompressedSize : 1;
    } __attribute__((packed));
}

#endif