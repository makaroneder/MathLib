#include "TLSSignatureAndHashAlgorithm.hpp"

TLSSignatureAndHashAlgorithm::TLSSignatureAndHashAlgorithm(void) : hash(TLSHashAlgorithm::None), signature(TLSSignatureAlgorithm::Anonymous) {}
TLSSignatureAndHashAlgorithm::TLSSignatureAndHashAlgorithm(TLSHashAlgorithm hash, TLSSignatureAlgorithm signature) : hash(hash), signature(signature) {}