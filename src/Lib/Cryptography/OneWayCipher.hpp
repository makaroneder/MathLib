#ifndef MathLib_Cryptography_OneWayCipher_H
#define MathLib_Cryptography_OneWayCipher_H
#include "CipherKey.hpp"
#include "../ExternArray.hpp"

namespace MathLib {
    struct OneWayCipher : Allocatable {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const = 0;
        template <typename T>
        [[nodiscard]] Array<uint8_t> EncryptT(const Sequence<T>& data, const CipherKey& key) const {
            const size_t size = data.GetSize();
            T tmp[size];
            for (size_t i = 0; i < size; i++) tmp[i] = data.AtUnsafe(i);
            const ExternArray<uint8_t> seq = ExternArray<uint8_t>((uint8_t*)tmp, size * sizeof(T));
            return Encrypt(seq, key);
        }
        template <typename T>
        [[nodiscard]] bool IsEncrypted(const Sequence<T>& data, const Sequence<uint8_t>& encrypted, const CipherKey& key) const {
            return EncryptT<T>(data, key) == encrypted;
        }
    };
}

#endif