#ifndef Cryptography_Protocol_H
#define Cryptography_Protocol_H
#include <string>

struct Protocol {
    virtual char EncryptChar(char chr, std::string key) const = 0;
    virtual char DecryptChar(char chr, std::string key) const = 0;
    std::string Encrypt(std::string str, std::string key) const;
    std::string Decrypt(std::string str, std::string key) const;

    protected:
    void GetBounds(char chr, char& start, char& end) const;
};

#endif