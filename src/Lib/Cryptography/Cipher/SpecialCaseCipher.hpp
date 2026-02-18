#ifndef MathLib_Cryptography_Cipher_SpecialCaseCipher_H
#define MathLib_Cryptography_Cipher_SpecialCaseCipher_H
#include "../Cipher.hpp"
#include "../../Expected.hpp"

namespace MathLib {
    template <typename T>
    struct SpecialCaseCipher : Cipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
            const Expected<CipherKey> k = GetKey(key);
            return k.HasValue() ? base.Encrypt(data, k.Get()) : Array<uint8_t>();
        }
        [[nodiscard]] virtual Array<uint8_t> DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const override {
            const Expected<CipherKey> k = GetKey(key);
            return k.HasValue() ? base.DecryptPartial(data, k.Get(), range) : Array<uint8_t>();
        }
        [[nodiscard]] virtual Expected<CipherKey> GetKey(const CipherKey& baseKey) const = 0;

        private:
        T base;
    };
}

#endif