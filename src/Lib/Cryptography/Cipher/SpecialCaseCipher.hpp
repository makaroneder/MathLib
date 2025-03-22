#ifndef MathLib_Cryptography_Cipher_SpecialCaseCipher_H
#define MathLib_Cryptography_Cipher_SpecialCaseCipher_H
#include "../Cipher.hpp"
#include "../../Expected.hpp"

namespace MathLib {
    template <typename T>
    struct SpecialCaseCipher : Cipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const override {
            StartBenchmark
            const Expected<Array<uint64_t>> k = GetKey(key);
            ReturnFromBenchmark(k.HasValue() ? base.Encrypt(data, k.Get(), encrypt) : Array<uint8_t>());
        }
        [[nodiscard]] virtual Expected<Array<uint64_t>> GetKey(const Sequence<uint64_t>& baseKey) const = 0;

        private:
        T base;
    };
}

#endif