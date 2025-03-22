#ifndef MathLib_Cryptography_Cipher_H
#define MathLib_Cryptography_Cipher_H
#include "OneWayCipher.hpp"

namespace MathLib {
    struct Cipher : OneWayCipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const override;
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const = 0;
        [[nodiscard]] String EncryptString(const Sequence<char>& str, const Sequence<uint64_t>& key, bool encrypt) const;
        template <typename T>
        Array<T> TestEncryption(const Sequence<T>& data, const Sequence<uint64_t>& key) const {
            return DecryptT<T>(EncryptT<T>(data, key), key);
        }
        template <typename T>
        [[nodiscard]] Array<T> DecryptT(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
            StartBenchmark
            const Array<uint8_t> ret = Encrypt(data, key, false);
            const size_t size = ret.GetSize();
            ReturnFromBenchmark(size % sizeof(T) ? Array<T>() : Array<T>((const T*)ret.GetValue(), size / sizeof(T)));
        }
    };
}

#endif