#include "AffineCipher.hpp"
#include "../Host.hpp"

bool AffineCipher::DecodeKey(const String& key, ssize_t& a, ssize_t& b) const {
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
char AffineCipher::EncryptChar(const char& chr, const String& key) const {
    Interval<char> interval = GetBounds(chr);
    const char size = interval.GetSize() + 1;
    ssize_t a;
    ssize_t b;
    if (!DecodeKey(key, a, b)) return INT8_MAX;
    char ret = (chr - interval.GetMin()) * a + b;
    if (ret < 0) ret += size;
    return ret % size + interval.GetMin();
}
char AffineCipher::DecryptChar(const char& chr, const String& key) const {
    Interval<char> interval = GetBounds(chr);
    const char size = interval.GetSize() + 1;
    ssize_t a;
    ssize_t b;
    if (!DecodeKey(key, a, b)) return INT8_MAX;
    char ret = (chr - interval.GetMin() - b) / a;
    if (ret < 0) ret += size;
    return ret % size + interval.GetMin();
}