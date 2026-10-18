#ifndef MathLib_Cryptography_Signature_H
#define MathLib_Cryptography_Signature_H
#include "OneWayCipher.hpp"

namespace MathLib {
    struct Signature : Allocatable {
        [[nodiscard]] virtual Array<Array<uint8_t>> CreateSignature(const Sequence<uint8_t>& hash, const CipherKey& key) const = 0;
        [[nodiscard]] virtual bool VerifySignature(const Sequence<Array<uint8_t>>& signature, const Sequence<uint8_t>& hash, const CipherKey& key) const = 0;
    };
}

#endif