#ifndef MathLib_Cryptography_SpecialCaseCipher_H
#define MathLib_Cryptography_SpecialCaseCipher_H
#include "Cipher.hpp"
#include "../Expected.hpp"

namespace MathLib {
    template <typename T>
    struct SpecialCaseCipher : Cipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Collection<uint8_t>& data, const String& key) const override {
            StartBenchmark
            const Expected<String> k = GetKey(key);
            ReturnFromBenchmark(k.HasValue() ? base.Encrypt(data, k.Get()) : Array<uint8_t>());
        }
        [[nodiscard]] virtual Array<uint8_t> Decrypt(const Collection<uint8_t>& data, const String& key) const override {
            StartBenchmark
            const Expected<String> k = GetKey(key);
            ReturnFromBenchmark(k.HasValue() ? base.Decrypt(data, k.Get()) : Array<uint8_t>());
        }
        [[nodiscard]] virtual Expected<String> GetKey(const String& baseKey) const = 0;

        private:
        T base;
    };
}

#endif