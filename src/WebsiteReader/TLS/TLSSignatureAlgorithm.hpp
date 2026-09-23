#ifndef TLSSignatureAlgorithm_H
#define TLSSignatureAlgorithm_H
#include <stdint.h>

enum class TLSSignatureAlgorithm : uint8_t {
    Anonymous = 0x00,
    RSA,
    DSA,
    ECDSA,
};

#endif