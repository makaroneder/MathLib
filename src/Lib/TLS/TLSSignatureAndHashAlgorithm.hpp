#ifndef TLS_SignatureAndHashAlgorithm_H
#define TLS_SignatureAndHashAlgorithm_H
#include "TLSSignatureAlgorithm.hpp"
#include "TLSHashAlgorithm.hpp"

namespace MathLib {
    struct TLSSignatureAndHashAlgorithm {
        TLSHashAlgorithm hash;
        TLSSignatureAlgorithm signature;

        TLSSignatureAndHashAlgorithm(void);
        TLSSignatureAndHashAlgorithm(TLSHashAlgorithm hash, TLSSignatureAlgorithm signature);
    } __attribute__((packed));
}

#endif