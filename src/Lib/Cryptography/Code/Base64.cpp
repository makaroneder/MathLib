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
        for (size_t i = 0; i < size; i += 4) {
            const uint8_t sym0 = ReverseSymbol(data.AtUnsafe(i + 0));
            if (sym0 == UINT8_MAX) return Array<uint8_t>();
            const uint8_t sym1 = ReverseSymbol(data.AtUnsafe(i + 1));
            if (sym1 == UINT8_MAX) return Array<uint8_t>();
            if (!ret.Add((sym0 << 2) | ((sym1 & 0b110000) >> 4))) return Array<uint8_t>();
            if (i + 2 == size || data.AtUnsafe(i + 2) == paddingSymbol) break;
            const uint8_t sym2 = ReverseSymbol(data.AtUnsafe(i + 2));
            if (sym2 == UINT8_MAX) return Array<uint8_t>();
            if (!ret.Add(((sym1 & 0b001111) << 4) | ((sym2 & 0b111100) >> 2))) return Array<uint8_t>();
            if (i + 3 == size || data.AtUnsafe(i + 3) == paddingSymbol) break;
            const uint8_t sym3 = ReverseSymbol(data.AtUnsafe(i + 3));
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
        for (uint8_t i = 0; i < sizeof(symbols); i++)
            if (symbols[i] == symbol) return i;
        return UINT8_MAX;
    }
}