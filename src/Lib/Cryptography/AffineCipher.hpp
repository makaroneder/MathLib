#ifndef Cryptography_AffineCipher_H
#define Cryptography_AffineCipher_H
#include "Cipher.hpp"

namespace MathLib {
    struct AffineCipher : Cipher {
        virtual Array<uint8_t> Encrypt(const Array<uint8_t>& data, const String& key) const override;
        virtual Array<uint8_t> Decrypt(const Array<uint8_t>& data, const String& key) const override;

        private:
        bool DecodeKey(const String& key, ssize_t& a, ssize_t& b) const;
    };
}

#endif