#ifndef MathLib_Cryptography_Cipher_CaesarCipher_H
#define MathLib_Cryptography_Cipher_CaesarCipher_H
#include "AffineCipher.hpp"
#include "SpecialCaseCipher.hpp"

namespace MathLib {
    struct CaesarCipher : SpecialCaseCipher<AffineCipher> {
        [[nodiscard]] virtual Expected<CipherKey> GetKey(const CipherKey& baseKey) const override;
    };
}

#endif