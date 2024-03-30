#include "AffineCipher.hpp"

bool AffineCipher::DecodeKey(std::string key, ssize_t& a, ssize_t& b) const {
    std::string str = "";
    size_t i = 0;
    while (key[i] == '-' || isdigit(key[i])) str += key[i++];
    a = std::stoll(str);
    str = "";
    if (key[i++] != ' ') return false;
    while (key[i] == '-' || isdigit(key[i])) str += key[i++];
    b = std::stoll(str);
    return true;
}
char AffineCipher::EncryptChar(char chr, std::string key) const {
    char start;
    char end;
    GetBounds(chr, start, end);
    const char size = end - start + 1;
    ssize_t a;
    ssize_t b;
    if (!DecodeKey(key, a, b)) return INT8_MAX;
    char ret = (chr - start) * a + b;
    if (ret < 0) ret += size;
    return ret % size + start;
}
char AffineCipher::DecryptChar(char chr, std::string key) const {
    char start;
    char end;
    GetBounds(chr, start, end);
    const char size = end - start + 1;
    ssize_t a;
    ssize_t b;
    if (!DecodeKey(key, a, b)) return INT8_MAX;
    char ret = (chr - start - b) / a;
    if (ret < 0) ret += size;
    return ret % size + start;
}