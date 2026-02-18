#ifndef MathLib_Cryptography_CompressorLZMA2ChunkHeader_H
#define MathLib_Cryptography_CompressorLZMA2ChunkHeader_H
#include <stdint.h>

namespace MathLib {
    struct LZMA2ChunkHeader {
        enum class ResetType : uint8_t {
            None = 0b00,
            State,
            StateAndProperties,
            StatePropertiesAndDictionary,
        };
        uint8_t uncompressedSizeFrom16To20Minus1 : 5;
        uint8_t reset : 2;
        bool reserved : 1;
    } __attribute__((packed));
}

#endif