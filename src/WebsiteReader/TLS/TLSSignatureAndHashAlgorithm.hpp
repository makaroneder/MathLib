#ifndef TLSSignatureAndHashAlgorithm_H
#define TLSSignatureAndHashAlgorithm_H
#include "TLSSignatureAlgorithm.hpp"
#include "TLSHashAlgorithm.hpp"

struct TLSSignatureAndHashAlgorithm {
    TLSHashAlgorithm hash;
    TLSSignatureAlgorithm signature;

    TLSSignatureAndHashAlgorithm(void);
    TLSSignatureAndHashAlgorithm(TLSHashAlgorithm hash, TLSSignatureAlgorithm signature);
} __attribute__((packed));

#endif