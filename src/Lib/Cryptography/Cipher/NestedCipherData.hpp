#ifndef MathLib_Cryptography_Cipher_NestedCipherData_H
#define MathLib_Cryptography_Cipher_NestedCipherData_H
#include "../Cipher.hpp"

namespace MathLib {
    struct NestedCipherData : Cipher {
        NestedCipherData(Cipher* cipher, size_t keySize);
        virtual ~NestedCipherData(void) override;
        [[nodiscard]] size_t GetKeySize(void) const;
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const override;

        private:
        Cipher* cipher;
        size_t keySize;
    };
}

#endif