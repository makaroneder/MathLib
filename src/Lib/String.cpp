#include "FunctionT.hpp"
#include "String.hpp"
#include "Host.hpp"

namespace MathLib {
    String Erase(const Sequence<char>& str, size_t pos, size_t len) {
        String ret = "";
        for (size_t i = 0; i < pos; i++) ret += str.At(i);
        for (size_t i = pos + len; i < str.GetSize(); i++) ret += str.At(i);
        return ret;
    }
    String SubString(const Sequence<char>& str, size_t pos, size_t len) {
        String ret;
        for (size_t i = 0; i < len; i++) ret += str.At(pos + i);
        return ret;
    }
    Array<String> Split(const Sequence<char>& str_, const Sequence<char>& delim, bool preserveDelim) {
        Array<String> ret;
        size_t pos = 0;
        String token;
        String str = CollectionToString(str_);
        while ((pos = str.Find(delim)) != SIZE_MAX) {
            token = SubString(str, 0, pos + delim.GetSize() * preserveDelim);
            if (!ret.Add(token)) return Array<String>();
            str = Erase(str, 0, pos + delim.GetSize());
        }
        return ret.Add(str) ? ret : Array<String>();
    }
    String BoolToString(bool x) {
        return x ? "true" : "false";
    }
    String ToString(size_t x, size_t base, size_t size) {
        String buff;
        if (!x) buff = "0";
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
        return ret;
    }
    size_t StringToNumber(const Sequence<char>& str, size_t base) {
        size_t ret = 0;
        const size_t size = str.GetSize();
        for (size_t i = 0; i < size; i++) {
            size_t tmp = str.At(i);
            if (IsDigit(tmp)) tmp -= '0';
            else if (IsHexDigit(tmp)) tmp = ToUpper(tmp) - 'A' + 10;
            if (tmp >= base) return SIZE_MAX;
            ret = ret * base + tmp;
        }
        return ret;
    }
    String DumpMemory(uintptr_t addr, size_t size, size_t lineSize) {
        String ret;
        const uint8_t* buff = (const uint8_t*)addr;
        for (size_t y = 0; y < size; y += lineSize) {
            for (size_t x = 0; x < lineSize; x++)
                ret += (y + x < size ? ToString(buff[y + x], 16, 2) : "??") + ' ';
            ret += '\t';
            for (size_t x = 0; x < lineSize; x++) {
                ret += y + x < size ? (IsAlphaDigit(buff[y + x]) ? (char)buff[y + x] : '.') : '?';
                ret += ' ';
            }
            ret.At(ret.GetSize() - 1) = '\n';
        }
        return ret;
    }
    [[nodiscard]] bool MatchRepeatable(const Sequence<char>& pattern, size_t& i, size_t& j, const Function<bool, size_t&>& function) {
        Interval<size_t> repeat = Interval<size_t>(1, 1);
        if (j < pattern.GetSize()) {
            switch (pattern.At(j)) {
                case '*': {
                    repeat = Interval<size_t>(0, SIZE_MAX);
                    j++;
                    break;
                }
                case '?': {
                    repeat = Interval<size_t>(0, 1);
                    j++;
                    break;
                }
                case '+': {
                    repeat = Interval<size_t>(1, SIZE_MAX);
                    j++;
                    break;
                }
                case '{': {
                    j++;
                    const size_t start = j;
                    const size_t end1 = pattern.Find(',', j);
                    if (end1 == SIZE_MAX) {
                        const size_t end = pattern.Find('}', j);
                        if (end == SIZE_MAX) return false;
                        const size_t tmp = StringToNumber(SubString(pattern, start, end - start));
                        repeat = Interval<size_t>(tmp, tmp);
                        j = end + 1;
                        break;
                    }
                    j = end1 + 1;
                    const size_t end2 = pattern.Find('}', j);
                    const String min = SubString(pattern, start, end1 - start);
                    if (end2 == SIZE_MAX) return false;
                    const String max = SubString(pattern, end1 + 1, end2 - end1 - 1);
                    repeat = Interval<size_t>(min.IsEmpty() ? 0 : StringToNumber(min), max.IsEmpty() ? SIZE_MAX : StringToNumber(max));
                    j = end2 + 1;
                    break;
                }
                default: break;
            }
        }
        size_t found = 0;
        while (true) {
            const size_t save = i;
            if (function(i)) {
                i = save;
                break;
            }
            found++;
        }
        return repeat.Contains(found);
    }
    [[nodiscard]] bool MatchInternal(const Sequence<char>& str, const Sequence<char>& pattern, size_t& i, const Interval<size_t>& patternInterval) {
        size_t j = patternInterval.GetMin();
        const size_t size1 = str.GetSize();
        const size_t size2 = patternInterval.GetMax();
        while (j < size2) {
            const char chr = pattern.At(j++);
            switch (chr) {
                case '^': {
                    if (i) return false;
                    break;
                }
                case '$': {
                    if (i != size1) return false;
                    break;
                }
                case '[': {
                    bool negate = false;
                    if (pattern.At(j) == '^') {
                        negate = true;
                        j++;
                    }
                    const size_t end = pattern.Find(']', j + 1);
                    if (end == SIZE_MAX) return false;
                    String tmp = SubString(pattern, j, end - j);
                    j = end + 1;
                    String patt;
                    for (size_t i = 0; i < tmp.GetSize(); i++) {
                        if (!i || i + 1 == tmp.GetSize() || tmp.At(i) != '-') patt += tmp.At(i);
                        else for (char chr = tmp.At(i - 1) + 1; chr < tmp.At(i + 1); chr++) patt += chr;
                    }
                    if (!MatchRepeatable(pattern, i, j, MakeFunctionT<bool, size_t&>([size1, negate, patt, &str](size_t& i) -> bool {
                        return i >= size1 || negate == patt.Contains(str.At(i++));
                    }))) return false;
                    break;
                }
                case '(': {
                    const size_t end = pattern.Find(')', j);
                    if (end == SIZE_MAX) return false;
                    const size_t start = j;
                    j = end + 1;
                    if (!MatchRepeatable(pattern, i, j, MakeFunctionT<bool, size_t&>([&str, &pattern, start, end](size_t& i) -> bool {
                        return !MatchInternal(str, pattern, i, Interval<size_t>(start, end));
                    }))) return false;
                    break;
                }
                case '.': {
                    if (!MatchRepeatable(pattern, i, j, MakeFunctionT<bool, size_t&>([size1](size_t& i) -> bool {
                        return i++ >= size1;
                    }))) return false;
                    break;
                }
                case '\\': {
                    if (i >= size1 || str.At(i++) != pattern.At(j++)) return false;
                    break;
                }
                default: {
                    if (!MatchRepeatable(pattern, i, j, MakeFunctionT<bool, size_t&>([size1, &str, chr](size_t& i) -> bool {
                        return i >= size1 || str.At(i++) != chr;
                    }))) return false;
                    break;
                }
            }
        }
        return j >= size2;
    }
    bool Match(const Sequence<char>& str, const Sequence<char>& pattern) {
        size_t i = 0;
        return MatchInternal(str, pattern, i, Interval<size_t>(0, pattern.GetSize())) && i >= str.GetSize();
    }
}