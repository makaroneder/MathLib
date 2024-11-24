#ifndef Cryptography_CaesarCipher_H
#define Cryptography_CaesarCipher_H
#include "AffineCipher.hpp"
#include "SpecialCaseCipher.hpp"

namespace MathLib {
    struct CaesarCipher : SpecialCaseCipher<AffineCipher> {
        virtual Expected<String> GetKey(const String& baseKey) const override;
    };
}

#endif