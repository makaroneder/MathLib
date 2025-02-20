#include "AffineCipher.hpp"
#include "../Host.hpp"

namespace MathLib {
    bool AffineCipher::DecodeKey(const String& key, ssize_t& a, ssize_t& b) const {
        StartBenchmark
        String str = "";
        size_t i = 0;
        while (i < key.GetSize() && (key[i] == '-' || IsDigit(key[i]))) str += key[i++];
        a = StringToNumber(str);
        str = "";
        if (key[i++] != ' ') ReturnFromBenchmark(false);
        while (i < key.GetSize() && (key[i] == '-' || IsDigit(key[i]))) str += key[i++];
        b = StringToNumber(str);
        ReturnFromBenchmark(true);
    }
    Array<uint8_t> AffineCipher::Encrypt(const Collection<uint8_t>& data, const String& key) const {
        StartBenchmark
        ssize_t a;
        ssize_t b;
        if (!DecodeKey(key, a, b)) ReturnFromBenchmark(Array<uint8_t>());
        Array<uint8_t> ret = CollectionToArray<uint8_t>(data);
        for (uint8_t& chr : ret) chr = chr * a + b;
        ReturnFromBenchmark(ret);
    }
    Array<uint8_t> AffineCipher::Decrypt(const Collection<uint8_t>& data, const String& key) const {
        StartBenchmark
        ssize_t a;
        ssize_t b;
        if (!DecodeKey(key, a, b)) ReturnFromBenchmark(Array<uint8_t>());
        Array<uint8_t> ret = CollectionToArray<uint8_t>(data);
        for (uint8_t& chr : ret) chr = (chr - b) / a;
        ReturnFromBenchmark(ret);
    }
}