#ifndef Cryptography_ROT13_H
#define Cryptography_ROT13_H
#include "CaesarCipher.hpp"

struct ROT13 : Protocol {
    virtual char EncryptChar(char chr, std::string key) const override;
    virtual char DecryptChar(char chr, std::string key) const override;

    private:
    CaesarCipher base;
};

#endif