#include "OneWayCipher.hpp"
#include "../Host.hpp"

namespace MathLib {
    Array<uint64_t> OneWayCipher::StringToKey(const Sequence<char>& key) {
        Array<uint64_t> ret;
        const size_t size = key.GetSize();
        String tmp = "";
        for (size_t i = 0; i < size; i++) {
            const char chr = key.At(i);
            if (IsDigit(chr) || chr == '-') tmp += chr;
            else if (IsWhiteSpace(chr)) {
                if (!ret.Add(StringToNumber(tmp))) return Array<uint64_t>();
                tmp = "";
            }
            else return Array<uint64_t>();
        }
        return tmp.IsEmpty() || ret.Add(StringToNumber(tmp)) ? ret : Array<uint64_t>();
    }
}