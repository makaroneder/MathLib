#include "Cipher.hpp"
#include "../Host.hpp"

namespace MathLib {
    String Cipher::EncryptString(const Sequence<char>& str, const Sequence<uint64_t>& key, bool encrypt) const {
        StartBenchmark
        Array<uint8_t> tmp = Array<uint8_t>(str.GetSize());
        for (size_t i = 0; i < tmp.GetSize(); i++) {
            if (IsDigit(str.At(i))) tmp.At(i) = str.At(i) - '0';
            else if (IsAlpha(str.At(i))) tmp.At(i) = ToUpper(str.At(i)) - 'A';
            else tmp.At(i) = UINT8_MAX;
        }
        tmp = encrypt ? Encrypt(tmp, key) : Decrypt(tmp, key);
        String ret;
        for (size_t i = 0; i < str.GetSize(); i++) {
            char base;
            uint8_t bounds;
            if (IsDigit(str.At(i))) {
                base = '0';
                bounds = '9' - '0' + 1;
            }
            else if (IsAlpha(str.At(i))) {
                if (IsUpper(str.At(i))) {
                    base = 'A';
                    bounds = 'Z' - 'A' + 1;
                }
                else {
                    base = 'a';
                    bounds = 'z' - 'a' + 1;
                }
            }
            else bounds = 0;
            char chr = tmp.At(i);
            if (chr < '\0') chr += bounds;
            ret += bounds ? base + chr % bounds : str.At(i);
        }
        ReturnFromBenchmark(ret);
    }
    Array<uint8_t> Cipher::Decrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        StartAndReturnFromBenchmark(DecryptPartial(data, key, Interval<size_t>(0, SIZE_MAX)));
    }
}