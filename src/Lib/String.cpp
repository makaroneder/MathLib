#include "String.hpp"

String Erase(String str, size_t pos, size_t len) {
    String ret = "";
    for (size_t i = 0; i < pos; i++) ret += str.At(i);
    for (size_t i = pos + len; i < str.GetSize(); i++) ret += str.At(i);
    return ret;
}
size_t Find(String str, String delim) {
    for (size_t i = 0; i < str.GetSize() - delim.GetSize(); i++) {
        bool found = true;
        for (size_t j = 0; j < delim.GetSize() && found; j++)
            if (str.At(i + j) != delim.At(j)) found = false;
        if (found) return i;
    }
    return SIZE_MAX;
}
String SubString(String str, size_t pos, size_t len) {
    String ret;
    for (size_t i = 0; i < len; i++) ret += str.At(pos + i);
    return ret;
}
Array<String> Split(String str, String delim) {
    Array<String> ret;
    size_t pos = 0;
    String token;
    while ((pos = Find(str, delim)) != SIZE_MAX) {
        token = SubString(str, 0, pos + delim.GetSize());
        ret.Add(token);
        str = Erase(str, 0, pos + delim.GetSize());
    }
    ret.Add(str);
    return ret;
}
String ToString(size_t x, size_t base) {
    if (!x) return "0";
    String buff;
    while (x) {
        const size_t n = x % base;
        x /= base;
        if (n < 10) buff += n + '0';
        else if (n < 16) buff += n - 10 + 'a';
    }
    String ret;
    for (size_t j = buff.GetSize() - 1; j > 0; j--) ret += buff.At(j);
    ret += buff[0];
    return ret;
}