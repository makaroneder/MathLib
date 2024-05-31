#include "AffineCipher.hpp"
#include "../Host.hpp"

bool AffineCipher::DecodeKey(String key, ssize_t& a, ssize_t& b) const {
    String str = "";
    size_t i = 0;
    while (i < key.GetSize() && (key[i] == '-' || IsDigit(key[i]))) str += key[i++];
    a = StringToNumber(str);
    str = "";
    if (key[i++] != ' ') return false;
    while (i < key.GetSize() && (key[i] == '-' || IsDigit(key[i]))) str += key[i++];
    b = StringToNumber(str);
    return true;
}
char AffineCipher::EncryptChar(char chr, String key) const {
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
char AffineCipher::DecryptChar(char chr, String key) const {
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