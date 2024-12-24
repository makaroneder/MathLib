#include "String.hpp"

namespace MathLib {
    String Erase(String str, size_t pos, size_t len) {
        StartBenchmark
        String ret = "";
        for (size_t i = 0; i < pos; i++) ret += str.At(i);
        for (size_t i = pos + len; i < str.GetSize(); i++) ret += str.At(i);
        ReturnFromBenchmark(ret);
    }
    String SubString(String str, size_t pos, size_t len) {
        StartBenchmark
        String ret;
        for (size_t i = 0; i < len; i++) ret += str.At(pos + i);
        ReturnFromBenchmark(ret);
    }
    Array<String> Split(String str, String delim, bool preserveDelim) {
        StartBenchmark
        Array<String> ret;
        size_t pos = 0;
        String token;
        while ((pos = str.Find(delim)) != SIZE_MAX) {
            token = SubString(str, 0, pos + delim.GetSize() * preserveDelim);
            ret.Add(token);
            str = Erase(str, 0, pos + delim.GetSize());
        }
        ret.Add(str);
        ReturnFromBenchmark(ret);
    }
    String BoolToString(bool x) {
        StartBenchmark
        ReturnFromBenchmark(x ? "true" : "false");
    }
    String ToString(size_t x, size_t base, size_t size) {
        StartBenchmark
        if (!x) ReturnFromBenchmark("0");
        String buff;
        while (x) {
            const size_t n = x % base;
            x /= base;
            if (n < 10) buff += n + '0';
            else if (n < 16) buff += n - 10 + 'a';
        }
        String ret;
        for (size_t i = buff.GetSize(); i < size; i++) ret += '0';
        for (size_t i = buff.GetSize() - 1; i > 0; i--) ret += buff.At(i);
        ret += buff[0];
        ReturnFromBenchmark(ret);
    }
}