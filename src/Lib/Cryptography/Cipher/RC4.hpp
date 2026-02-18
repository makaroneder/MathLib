#ifndef MathLib_Cryptography_Cipher_RC4_H
#define MathLib_Cryptography_Cipher_RC4_H
#include "InvolutionCipher.hpp"

namespace MathLib {
    struct RC4 : InvolutionCipher {
        [[nodiscard]] static CipherKey GenerateKey(const CipherKey& key);
        [[nodiscard]] virtual Array<uint8_t> Convert(const Sequence<uint8_t>& data, const CipherKey& key, size_t limit) const override;
    };
}

#endif