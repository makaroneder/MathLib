#ifndef MathLib_Cryptography_OneWayCipher_H
#define MathLib_Cryptography_OneWayCipher_H
#include "../Typedefs.hpp"

namespace MathLib {
    struct OneWayCipher : Allocatable {
        static Array<uint64_t> StringToKey(const Sequence<char>& key);
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const = 0;
        template <typename T>
        [[nodiscard]] Array<uint8_t> EncryptT(const Sequence<T>& data, const Sequence<uint64_t>& key) const {
            const size_t size = data.GetSize();
            T tmp[size];
            for (size_t i = 0; i < size; i++) tmp[i] = data.At(i);
            return Encrypt(Array<uint8_t>((const uint8_t*)tmp, size * sizeof(T)), key);
        }
        template <typename T>
        [[nodiscard]] bool IsEncrypted(const Sequence<T>& data, const Sequence<uint8_t>& encrypted, const Sequence<uint64_t>& key) const {
            return EncryptT<T>(data, key) == encrypted;
        }
    };
}

#endif