#ifndef Cryptography_Protocol_H
#define Cryptography_Protocol_H
#include "../Typedefs.hpp"

struct Protocol {
    virtual char EncryptChar(char chr, String key) const = 0;
    virtual char DecryptChar(char chr, String key) const = 0;
    String Encrypt(String str, String key) const;
    String Decrypt(String str, String key) const;

    protected:
    void GetBounds(char chr, char& start, char& end) const;
};

#endif