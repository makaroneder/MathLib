#ifndef TLS_HashAlgorithm_H
#define TLS_HashAlgorithm_H
#include <stdint.h>

namespace MathLib {
    enum class TLSHashAlgorithm : uint8_t {
        None = 0x00,
        MD5,
        SHA1,
        SHA224,
        SHA256,
        SHA384,
        SHA512,
    };
}

#endif