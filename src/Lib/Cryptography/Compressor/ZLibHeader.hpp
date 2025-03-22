#ifndef MathLib_Cryptography_Compressor_ZLibHeader_H
#define MathLib_Cryptography_Compressor_ZLibHeader_H
#include <stdint.h>

namespace MathLib {
    struct ZLibHeader {
        enum class CompressionMethod : uint8_t {
            Deflate = 8,
        };
        enum class DeflateLevel : uint8_t {
            Fastest = 0,
            Fast,
            Default,
            Slowest,
        };
        uint8_t compressionMethod : 4;
        uint8_t compressionInfo : 4;
        uint8_t checkBits : 5;
        bool dictionary : 1;
        uint8_t level : 2;

        ZLibHeader(void);
    } __attribute__((packed));
}

#endif