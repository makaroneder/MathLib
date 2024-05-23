#include "Protocol.hpp"
#include "../Host.hpp"

std::string Protocol::Encrypt(std::string str, std::string key) const {
    std::string ret = "";
    for (const char& chr : str) {
        if (!IsAlphaDigit(chr)) ret += chr;
        else ret += EncryptChar(chr, key);
    }
    return ret;
}
std::string Protocol::Decrypt(std::string str, std::string key) const {
    std::string ret = "";
    for (const char& chr : str) {
        if (!IsAlphaDigit(chr)) ret += chr;
        else ret += DecryptChar(chr, key);
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