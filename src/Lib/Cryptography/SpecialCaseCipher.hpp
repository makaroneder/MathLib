#ifndef Cryptography_SpecialCaseCipher_H
#define Cryptography_SpecialCaseCipher_H
#include "Cipher.hpp"
#include "../Expected.hpp"

template <typename T>
struct SpecialCaseCipher : Cipher {
    virtual Array<uint8_t> Encrypt(const Array<uint8_t>& data, const String& key) const override {
        const Expected<String> k = GetKey(key);
        return k.HasValue() ? base.Encrypt(data, k.Get()) : Array<uint8_t>();
    }
    virtual Array<uint8_t> Decrypt(const Array<uint8_t>& data, const String& key) const override {
        const Expected<String> k = GetKey(key);
        return k.HasValue() ? base.Decrypt(data, k.Get()) : Array<uint8_t>();
    }
    virtual Expected<String> GetKey(const String& baseKey) const = 0;

    private:
    T base;
};

#endif