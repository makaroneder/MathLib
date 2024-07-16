#include "Cipher.hpp"
#include "../Host.hpp"

String Cipher::Encrypt(const String& str, const String& key) const {
    String ret = "";
    for (const char& chr : str) {
        if (!IsAlphaDigit(chr)) ret += chr;
        else ret += EncryptChar(chr, key);
    }
    return ret;
}
String Cipher::Decrypt(const String& str, const String& key) const {
    String ret = "";
    for (const char& chr : str) {
        if (!IsAlphaDigit(chr)) ret += chr;
        else ret += DecryptChar(chr, key);
    }
    return ret;
}
Interval<char> Cipher::GetBounds(const char& chr) const {
    const uint8_t state = IsDigit(chr) ? 0 : (IsUpper(chr) ? 1 : 2);
    switch (state) {
        case 0: return Interval<char>('0', '9');
        case 1: return Interval<char>('A', 'Z');
        case 2: return Interval<char>('a', 'z');
        default: return Interval<char>();
    }
}