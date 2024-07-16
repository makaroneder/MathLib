#ifndef Cryptography_AffineCipher_H
#define Cryptography_AffineCipher_H
#include "Cipher.hpp"

struct AffineCipher : Cipher {
    virtual char EncryptChar(const char& chr, const String& key) const override;
    virtual char DecryptChar(const char& chr, const String& key) const override;

    private:
    bool DecodeKey(const String& key, ssize_t& a, ssize_t& b) const;
};

#endif