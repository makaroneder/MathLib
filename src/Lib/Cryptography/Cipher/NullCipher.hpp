#ifndef MathLib_Cryptography_Cipher_NullCipher_H
#define MathLib_Cryptography_Cipher_NullCipher_H
#include "../Cipher.hpp"

namespace MathLib {
    struct NullCipher : Cipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const override;
    };
}

#endif