#include "Cipher.hpp"
#include "../Host.hpp"

namespace MathLib {
    Cipher::Cipher(void) {
        for (char chr = '0'; chr <= '9'; chr++) digits += chr;
        for (char chr = 'a'; chr <= 'z'; chr++) {
            letters += chr;
            upperLetters += ToUpper(chr);
        }
    }
    Cipher::Cipher(const String& letters, const String& upperLetters, const String& digits) : letters(letters), upperLetters(upperLetters), digits(digits) {}
    uint8_t Cipher::GetIndex(char chr) const {
        if (IsDigit(chr)) {
            for (uint8_t i = 0; i < digits.GetSize(); i++)
                if (chr == digits.At(i)) return i;
            return UINT8_MAX;
        }
        else if (IsAlpha(chr)) {
            if (IsUpper(chr)) {
                for (uint8_t i = 0; i < upperLetters.GetSize(); i++)
                    if (chr == upperLetters.At(i)) return i;
                return UINT8_MAX;
            }
            else {
                for (uint8_t i = 0; i < letters.GetSize(); i++)
                    if (chr == letters.At(i)) return i;
                return UINT8_MAX;
            }
        }
        else return UINT8_MAX;
    }
    String Cipher::GetString(const String& str, const String& key, bool encrypt) const {
        Array<uint8_t> tmp = Array<uint8_t>(str.GetSize());
        for (size_t i = 0; i < tmp.GetSize(); i++) tmp.At(i) = GetIndex(str.At(i));
        tmp = encrypt ? Encrypt(tmp, key) : Decrypt(tmp, key);
        String ret;
        for (size_t i = 0; i < str.GetSize(); i++) {
            String base;
            uint8_t bounds;
            if (IsDigit(str.At(i))) {
                base = digits;
                bounds = '9' - '0' + 1;
            }
            else if (IsAlpha(str.At(i))) {
                if (IsUpper(str.At(i))) {
                    base = upperLetters;
                    bounds = 'Z' - 'A' + 1;
                }
                else {
                    base = letters;
                    bounds = 'z' - 'a' + 1;
                }
            }
            else bounds = 0;
            char chr = tmp.At(i);
            if (chr < '\0') chr += bounds;
            ret += bounds ? base.At(chr % bounds) : str.At(i);
        }
        return ret;
    }
    String Cipher::EncryptString(const String& str, const String& key) const {
        return GetString(str, key, true);
    }
    String Cipher::DecryptString(const String& str, const String& key) const {
        return GetString(str, key, false);
    }
}