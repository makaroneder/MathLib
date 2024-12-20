#ifndef Cryptography_AtbashCipher_H
#define Cryptography_AtbashCipher_H
#include "AffineCipher.hpp"
#include "SpecialCaseCipher.hpp"

namespace MathLib {
    struct AtbashCipher : SpecialCaseCipher<AffineCipher> {
        virtual Expected<String> GetKey(const String& baseKey) const override;
    };
}

#endif