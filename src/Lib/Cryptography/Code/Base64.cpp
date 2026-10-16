#include "Base64.hpp"

namespace MathLib {
    Array<uint8_t> Base64::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        const size_t size = data.GetSize();
        Array<uint8_t> ret;
        for (size_t i = 0; i < size; i += 3) {
            if (!ret.Add(symbols[(data.AtUnsafe(i + 0) & 0b11111100) >> 2])) return Array<uint8_t>();
            const bool no1 = i + 1 == size;
            if (!ret.Add(symbols[((data.AtUnsafe(i + 0) & 0b00000011) << 4) | ((no1 ? 0 : data.AtUnsafe(i + 1) & 0b11110000) >> 4)])) return Array<uint8_t>();
            if (no1) {
                if (!ret.Add(paddingSymbol)) return Array<uint8_t>();
                if (!ret.Add(paddingSymbol)) return Array<uint8_t>();
                break;
            }
            const bool no2 = i + 2 == size;
            if (!ret.Add(symbols[((data.AtUnsafe(i + 1) & 0b00001111) << 2) | ((no2 ? 0 : data.AtUnsafe(i + 2) & 0b11000000) >> 6)])) return Array<uint8_t>();
            if (no2) {
                if (!ret.Add(paddingSymbol)) return Array<uint8_t>();
                break;
            }
            if (!ret.Add(symbols[data.AtUnsafe(i + 2) & 0b00111111])) return Array<uint8_t>();
        }
        return ret;
    }
    Array<uint8_t> Base64::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        const size_t size = data.GetSize();
        Array<uint8_t> ret;
        for (size_t i = 0; i < size;) {
            const uint8_t sym0 = GetSymbol(data, i);
            if (sym0 == UINT8_MAX || sym0 == sizeof(symbols)) return Array<uint8_t>();
            const uint8_t sym1 = GetSymbol(data, i);
            if (sym1 == UINT8_MAX || sym1 == sizeof(symbols)) return Array<uint8_t>();
            if (!ret.Add((sym0 << 2) | ((sym1 & 0b110000) >> 4))) return Array<uint8_t>();
            const uint8_t sym2 = GetSymbol(data, i);
            if (sym2 == sizeof(symbols)) break;
            if (sym2 == UINT8_MAX) return Array<uint8_t>();
            if (!ret.Add(((sym1 & 0b001111) << 4) | ((sym2 & 0b111100) >> 2))) return Array<uint8_t>();
            const uint8_t sym3 = GetSymbol(data, i);
            if (sym3 == sizeof(symbols)) break;
            if (sym3 == UINT8_MAX) return Array<uint8_t>();
            if (!ret.Add(((sym2 & 0b000011) << 6) | (sym3 & 0b111111))) return Array<uint8_t>();
        }
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(ret.GetSize(), range.GetMax());
        if (start >= end) return Array<uint8_t>();
        Array<uint8_t> sub = Array<uint8_t>(end - start);
        for (size_t i = start; i < end; i++) sub.AtUnsafe(i - start) = ret.AtUnsafe(i);
        return sub;
    }
    uint8_t Base64::ReverseSymbol(uint8_t symbol) const {
        if (symbol == paddingSymbol) return sizeof(symbols);
        for (uint8_t i = 0; i < sizeof(symbols); i++)
            if (symbols[i] == symbol) return i;
        return UINT8_MAX;
    }
    uint8_t Base64::GetSymbol(const Sequence<uint8_t>& data, size_t& i) const {
        const size_t size = data.GetSize();
        while (true) {
            if (i >= size) return sizeof(symbols);
            if (data.AtUnsafe(i++) == lineBreakSymbol) continue;
            return ReverseSymbol(data.AtUnsafe(i - 1));
        }
    }
}