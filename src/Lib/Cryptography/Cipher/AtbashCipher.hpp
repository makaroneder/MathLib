#ifndef MathLib_Cryptography_Cipher_AtbashCipher_H
#define MathLib_Cryptography_Cipher_AtbashCipher_H
#include "AffineCipher.hpp"
#include "SpecialCaseCipher.hpp"

namespace MathLib {
    struct AtbashCipher : SpecialCaseCipher<AffineCipher> {
        [[nodiscard]] virtual Expected<CipherKey> GetKey(const CipherKey& baseKey) const override;
    };
}

#endif