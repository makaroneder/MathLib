#ifndef MathLib_Cryptography_Compressor_DeflateTree_H
#define MathLib_Cryptography_Compressor_DeflateTree_H
#include <stdint.h>

namespace MathLib {
    struct DeflateTree {
        uint16_t lengthTable[16];
        uint16_t translationTable[288];
    };
}

#endif