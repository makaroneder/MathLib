#ifndef Cryptography_AffineCipher_H
#define Cryptography_AffineCipher_H
#include "Protocol.hpp"

struct AffineCipher : Protocol {
    virtual char EncryptChar(char chr, std::string key) const override;
    virtual char DecryptChar(char chr, std::string key) const override;

    private:
    bool DecodeKey(std::string key, ssize_t& a, ssize_t& b) const;
};

#endif