#ifndef Cryptography_ROT13_H
#define Cryptography_ROT13_H
#include "CaesarCipher.hpp"

struct ROT13 : Protocol {
    virtual char EncryptChar(char chr, String key) const override;
    virtual char DecryptChar(char chr, String key) const override;

    private:
    CaesarCipher base;
};

#endif