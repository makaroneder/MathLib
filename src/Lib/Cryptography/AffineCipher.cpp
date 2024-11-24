#include "AffineCipher.hpp"
#include "../Host.hpp"

namespace MathLib {
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
    Array<uint8_t> AffineCipher::Encrypt(const Array<uint8_t>& data, const String& key) const {
        ssize_t a;
        ssize_t b;
        if (!DecodeKey(key, a, b)) return Array<uint8_t>();
        Array<uint8_t> ret = data;
        for (uint8_t& chr : ret) chr = chr * a + b;
        return ret;
    }
    Array<uint8_t> AffineCipher::Decrypt(const Array<uint8_t>& data, const String& key) const {
        ssize_t a;
        ssize_t b;
        if (!DecodeKey(key, a, b)) return Array<uint8_t>();
        Array<uint8_t> ret = data;
        for (uint8_t& chr : ret) chr = (chr - b) / a;
        return ret;
    }
}