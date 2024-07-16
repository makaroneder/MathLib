#ifndef Cryptography_Cipher_H
#define Cryptography_Cipher_H
#include "../Interval.hpp"

struct Cipher : Allocatable {
    virtual char EncryptChar(const char& chr, const String& key) const = 0;
    virtual char DecryptChar(const char& chr, const String& key) const = 0;
    String Encrypt(const String& str, const String& key) const;
    String Decrypt(const String& str, const String& key) const;

    protected:
    Interval<char> GetBounds(const char& chr) const;
};

#endif