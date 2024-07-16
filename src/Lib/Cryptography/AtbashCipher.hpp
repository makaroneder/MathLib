#ifndef Cryptography_AtbashCipher_H
#define Cryptography_AtbashCipher_H
#include "AffineCipher.hpp"

struct AtbashCipher : Cipher {
    virtual char EncryptChar(const char& chr, const String& key) const override;
    virtual char DecryptChar(const char& chr, const String& key) const override;

    private:
    AffineCipher base;
};

#endif