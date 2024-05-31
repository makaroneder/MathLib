#ifndef Cryptography_AtbashCipher_H
#define Cryptography_AtbashCipher_H
#include "AffineCipher.hpp"

struct AtbashCipher : Protocol {
    virtual char EncryptChar(char chr, String key) const override;
    virtual char DecryptChar(char chr, String key) const override;

    private:
    AffineCipher base;
};

#endif