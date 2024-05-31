#ifndef Cryptography_CaesarCipher_H
#define Cryptography_CaesarCipher_H
#include "AffineCipher.hpp"

struct CaesarCipher : Protocol {
    virtual char EncryptChar(char chr, String key) const override;
    virtual char DecryptChar(char chr, String key) const override;

    private:
    AffineCipher base;
};

#endif