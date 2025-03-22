#ifndef Channel_H
#define Channel_H
#include <Cryptography/Cipher.hpp>

struct Channel {
    template <typename T>
    MathLib::Array<T> Send(const MathLib::Sequence<T>& message, const MathLib::Cipher& cipher, const MathLib::Sequence<uint64_t>& key) const {
        return cipher.DecryptT<T>(Convert(cipher.EncryptT<T>(message, key)), key);
    }
    MathLib::String SendString(const MathLib::Sequence<char>& message, const MathLib::Cipher& cipher, const MathLib::Sequence<uint64_t>& key) const {
        return MathLib::CollectionToString(Send<char>(message, cipher, key));
    }
    virtual MathLib::Array<uint8_t> Convert(const MathLib::Sequence<uint8_t>& message) const = 0;
};

#endif