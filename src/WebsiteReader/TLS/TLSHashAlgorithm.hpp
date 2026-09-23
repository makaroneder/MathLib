#ifndef TLSHashAlgorithm_H
#define TLSHashAlgorithm_H
#include <stdint.h>

enum class TLSHashAlgorithm : uint8_t {
    None = 0x00,
    MD5,
    SHA1,
    SHA224,
    SHA256,
    SHA384,
    SHA512,
};

#endif