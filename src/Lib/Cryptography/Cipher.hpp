#ifndef MathLib_Cryptography_Cipher_H
#define MathLib_Cryptography_Cipher_H
#include "OneWayCipher.hpp"
#include "../Interval.hpp"

namespace MathLib {
    struct Cipher : OneWayCipher {
        [[nodiscard]] virtual Array<uint8_t> DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const = 0;
        [[nodiscard]] Array<uint8_t> Decrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const;
        [[nodiscard]] String EncryptString(const Sequence<char>& str, const Sequence<uint64_t>& key, bool encrypt) const;
        template <typename T>
        Array<T> CheckEncryption(const Sequence<T>& data, const Sequence<uint64_t>& key) const {
            StartAndReturnFromBenchmark(DecryptT<T>(EncryptT<T>(data, key), key));
        }
        template <typename T>
        bool TestEncryption(const Sequence<T>& data, const Sequence<uint64_t>& key) const {
            StartAndReturnFromBenchmark(CheckEncryption(data, key) == data);
        }
        template <typename T>
        [[nodiscard]] Array<T> DecryptT(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
            StartBenchmark
            const Array<uint8_t> ret = Decrypt(data, key);
            const size_t size = ret.GetSize();
            ReturnFromBenchmark(size % sizeof(T) ? Array<T>() : Array<T>((const T*)ret.GetValue(), size / sizeof(T)));
        }
    };
}

#endif