#ifndef MathLib_Cryptography_Cipher_H
#define MathLib_Cryptography_Cipher_H
#include "../Interval.hpp"

namespace MathLib {
    struct Cipher : Allocatable {
        Cipher(void);
        Cipher(const String& letters, const String& upperLetters, const String& digits);
        virtual Array<uint8_t> Encrypt(const Array<uint8_t>& data, const String& key) const = 0;
        virtual Array<uint8_t> Decrypt(const Array<uint8_t>& data, const String& key) const = 0;
        String EncryptString(const String& str, const String& key) const;
        String DecryptString(const String& str, const String& key) const;

        private:
        String GetString(const String& str, const String& key, bool encrypt) const;
        uint8_t GetIndex(char chr) const;

        String letters;
        String upperLetters;
        String digits;
    };
}

#endif