#ifndef MathLib_Cryptography_Signature_DSA_H
#define MathLib_Cryptography_Signature_DSA_H
#include "../Signature.hpp"

namespace MathLib {
    struct DSA : Signature {
        [[nodiscard]] virtual Array<Array<uint8_t>> CreateSignature(const Sequence<uint8_t>& hash, const CipherKey& key) const override;
        [[nodiscard]] virtual bool VerifySignature(const Sequence<Array<uint8_t>>& signature, const Sequence<uint8_t>& hash, const CipherKey& key) const override;
    };
}

#endif