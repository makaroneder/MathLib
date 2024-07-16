#ifndef Cryptography_CaesarCipher_H
#define Cryptography_CaesarCipher_H
#include "AffineCipher.hpp"

struct CaesarCipher : Cipher {
    virtual char EncryptChar(const char& chr, const String& key) const override;
    virtual char DecryptChar(const char& chr, const String& key) const override;

    private:
    AffineCipher base;
};

#endif