#ifndef MathLib_Cryptography_AffineCipher_H
#define MathLib_Cryptography_AffineCipher_H
#include "Cipher.hpp"

namespace MathLib {
    struct AffineCipher : Cipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Array<uint8_t>& data, const String& key) const override;
        [[nodiscard]] virtual Array<uint8_t> Decrypt(const Array<uint8_t>& data, const String& key) const override;

        private:
        [[nodiscard]] bool DecodeKey(const String& key, ssize_t& a, ssize_t& b) const;
    };
}

#endif