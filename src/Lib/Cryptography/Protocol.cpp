#include "Protocol.hpp"
#include "../Host.hpp"

String Protocol::Encrypt(String str, String key) const {
    String ret = "";
    for (size_t i = 0; i < str.GetSize(); i++) {
        if (!IsAlphaDigit(str.At(i))) ret += str.At(i);
        else ret += EncryptChar(str.At(i), key);
    }
    return ret;
}
String Protocol::Decrypt(String str, String key) const {
    String ret = "";
    for (size_t i = 0; i < str.GetSize(); i++) {
        if (!IsAlphaDigit(str.At(i))) ret += str.At(i);
        else ret += DecryptChar(str.At(i), key);
    }
    return ret;
}
void Protocol::GetBounds(char chr, char& start, char& end) const {
    const uint8_t state = IsDigit(chr) ? 0 : (IsUpper(chr) ? 1 : 2);
    switch (state) {
        case 0: {
            start = '0';
            end = '9';
            break;
        }
        case 1: {
            start = 'A';
            end = 'Z';
            break;
        }
        case 2: {
            start = 'a';
            end = 'z';
            break;
        }
    }
}