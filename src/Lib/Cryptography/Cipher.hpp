#ifndef MathLib_Cryptography_Cipher_H
#define MathLib_Cryptography_Cipher_H
#include "OneWayCipher.hpp"
#include "../Interval.hpp"

namespace MathLib {
    struct Cipher : OneWayCipher {
        [[nodiscard]] virtual Array<uint8_t> DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const = 0;
        [[nodiscard]] Array<uint8_t> Decrypt(const Sequence<uint8_t>& data, const CipherKey& key) const;
        template <typename T>
        Array<T> CheckEncryption(const Sequence<T>& data, const CipherKey& key) const {
            return DecryptT<T>(EncryptT<T>(data, key), key);
        }
        template <typename T>
        bool TestEncryption(const Sequence<T>& data, const CipherKey& key) const {
            return CheckEncryption(data, key) == data;
        }
        template <typename T>
        [[nodiscard]] Array<T> DecryptT(const Sequence<uint8_t>& data, const CipherKey& key) const {
            const Array<uint8_t> ret = Decrypt(data, key);
            const size_t size = ret.GetSize();
            return size % sizeof(T) ? Array<T>() : Array<T>((const T*)ret.GetValue(), size / sizeof(T));
        }
    };
}

#endif