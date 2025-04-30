#ifndef MathLib_Cryptography_Cipher_AffineCipher_H
#define MathLib_Cryptography_Cipher_AffineCipher_H
#include "../Cipher.hpp"

namespace MathLib {
    struct AffineCipher : Cipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const override;
    };
}

#endif