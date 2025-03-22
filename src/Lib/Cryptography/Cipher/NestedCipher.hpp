#ifndef MathLib_Cryptography_Cipher_NestedCipher_H
#define MathLib_Cryptography_Cipher_NestedCipher_H
#include "NestedCipherData.hpp"

namespace MathLib {
    struct NestedCipher : Cipher {
        NestedCipher(const Sequence<NestedCipherData*>& ciphers);
        virtual ~NestedCipher(void) override;
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const override;

        private:
        Array<NestedCipherData*> ciphers;
    };
}

#endif