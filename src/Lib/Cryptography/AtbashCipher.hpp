#ifndef Cryptography_AtbashCipher_H
#define Cryptography_AtbashCipher_H
#include "AffineCipher.hpp"
#include "SpecialCaseCipher.hpp"

struct AtbashCipher : SpecialCaseCipher<AffineCipher> {
    virtual Expected<String> GetKey(const String& baseKey) const override;
};

#endif