#ifndef MathLib_Cryptography_CaesarCipher_H
#define MathLib_Cryptography_CaesarCipher_H
#include "AffineCipher.hpp"
#include "SpecialCaseCipher.hpp"

namespace MathLib {
    struct CaesarCipher : SpecialCaseCipher<AffineCipher> {
        [[nodiscard]] virtual Expected<String> GetKey(const String& baseKey) const override;
    };
}

#endif