#ifndef MathLib_Cryptography_Cipher_IdentityCipher_H
#define MathLib_Cryptography_Cipher_IdentityCipher_H
#include "InvolutionCipher.hpp"

namespace MathLib {
    struct IdentityCipher : InvolutionCipher {
        [[nodiscard]] virtual Array<uint8_t> Convert(const Sequence<uint8_t>& data, const CipherKey& key, size_t limit) const override;
    };
}

#endif