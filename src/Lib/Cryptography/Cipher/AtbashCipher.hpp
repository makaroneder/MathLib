#ifndef MathLib_Cryptography_Cipher_AtbashCipher_H
#define MathLib_Cryptography_Cipher_AtbashCipher_H
#include "AffineCipher.hpp"
#include "SpecialCaseCipher.hpp"

namespace MathLib {
    struct AtbashCipher : SpecialCaseCipher<AffineCipher> {
        [[nodiscard]] virtual Expected<Array<uint64_t>> GetKey(const Sequence<uint64_t>& baseKey) const override;
    };
}

#endif