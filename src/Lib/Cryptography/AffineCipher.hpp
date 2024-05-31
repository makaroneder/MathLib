#ifndef Cryptography_AffineCipher_H
#define Cryptography_AffineCipher_H
#include "Protocol.hpp"

struct AffineCipher : Protocol {
    virtual char EncryptChar(char chr, String key) const override;
    virtual char DecryptChar(char chr, String key) const override;

    private:
    bool DecodeKey(String key, ssize_t& a, ssize_t& b) const;
};

#endif