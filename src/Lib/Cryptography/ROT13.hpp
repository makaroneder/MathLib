#ifndef Cryptography_ROT13_H
#define Cryptography_ROT13_H
#include "CaesarCipher.hpp"

struct ROT13 : Cipher {
    virtual char EncryptChar(const char& chr, const String& key) const override;
    virtual char DecryptChar(const char& chr, const String& key) const override;

    private:
    CaesarCipher base;
};

#endif