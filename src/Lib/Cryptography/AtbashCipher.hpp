#ifndef Cryptography_AtbashCipher_H
#define Cryptography_AtbashCipher_H
#include "AffineCipher.hpp"

struct AtbashCipher : Protocol {
    virtual char EncryptChar(char chr, std::string key) const override;
    virtual char DecryptChar(char chr, std::string key) const override;

    private:
    AffineCipher base;
};

#endif