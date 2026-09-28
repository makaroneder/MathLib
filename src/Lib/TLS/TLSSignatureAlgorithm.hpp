#ifndef TLS_SignatureAlgorithm_H
#define TLS_SignatureAlgorithm_H
#include <stdint.h>

namespace MathLib {
    enum class TLSSignatureAlgorithm : uint8_t {
        Anonymous = 0x00,
        RSA,
        DSA,
        ECDSA,
    };
}

#endif