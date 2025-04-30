#ifndef MathLib_Cryptography_Cipher_IteratedCipher_H
#define MathLib_Cryptography_Cipher_IteratedCipher_H
#include "../Cipher.hpp"

namespace MathLib {
    struct IteratedCipher : Cipher {
        IteratedCipher(Cipher& cipher);
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const override;

        private:
        Cipher& cipher;
    };
}

#endif