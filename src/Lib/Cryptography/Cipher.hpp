#ifndef MathLib_Cryptography_Cipher_H
#define MathLib_Cryptography_Cipher_H
#include "../Interval.hpp"

namespace MathLib {
    struct Cipher : Allocatable {
        Cipher(void);
        Cipher(const String& letters, const String& upperLetters, const String& digits);
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Collection<uint8_t>& data, const String& key) const = 0;
        [[nodiscard]] virtual Array<uint8_t> Decrypt(const Collection<uint8_t>& data, const String& key) const = 0;
        [[nodiscard]] String EncryptString(const String& str, const String& key) const;
        [[nodiscard]] String DecryptString(const String& str, const String& key) const;

        private:
        [[nodiscard]] String GetString(const String& str, const String& key, bool encrypt) const;
        [[nodiscard]] uint8_t GetIndex(char chr) const;

        String letters;
        String upperLetters;
        String digits;
    };
}

#endif