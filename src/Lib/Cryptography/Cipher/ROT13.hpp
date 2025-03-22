#ifndef MathLib_Cryptography_Cipher_ROT13_H
#define MathLib_Cryptography_Cipher_ROT13_H
#include "CaesarCipher.hpp"
#include "SpecialCaseCipher.hpp"

namespace MathLib {
    struct ROT13 : SpecialCaseCipher<CaesarCipher> {
        [[nodiscard]] virtual Expected<Array<uint64_t>> GetKey(const Sequence<uint64_t>& baseKey) const override;
    };
}

#endif