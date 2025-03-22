#ifndef MathLib_Cryptography_Cipher_NullCipher_H
#define MathLib_Cryptography_Cipher_NullCipher_H
#include "../Cipher.hpp"

namespace MathLib {
    struct NullCipher : Cipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const override;
    };
}

#endif