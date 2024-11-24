#ifndef Cryptography_ROT13_H
#define Cryptography_ROT13_H
#include "CaesarCipher.hpp"
#include "SpecialCaseCipher.hpp"

namespace MathLib {
    struct ROT13 : SpecialCaseCipher<CaesarCipher> {
        virtual Expected<String> GetKey(const String& baseKey) const override;
    };
}

#endif