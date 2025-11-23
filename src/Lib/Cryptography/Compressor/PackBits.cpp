#include "PackBits.hpp"

namespace MathLib {
    Array<uint8_t> PackBits::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        const size_t size = data.GetSize();
        Array<uint8_t> ret;
        for (size_t i = 0; i < size;) {
            const uint8_t symbol = data.At(i);
            uint8_t repeats = 0;
            bool repeat = true;
            uint8_t prev = symbol;
            for (size_t j = i + 1; j < size && repeats < 127; j++) {
                const uint8_t tmp = data.At(j);
                if (prev == tmp) {
                    if (!repeat) {
                        repeats--;
                        break;
                    }
                    repeats++;
                }
                else {
                    if (repeat && repeats) break;
                    repeat = false;
                    repeats++;
                    prev = tmp;
                }
            }
            if (!repeat) {
                if (!ret.Add(repeats)) return Array<uint8_t>();
                for (uint8_t j = 0; j <= repeats; j++)
                    if (!ret.Add(data.At(i + j))) return Array<uint8_t>();
            }
            else if (!ret.Add(-repeats) || !ret.Add(symbol)) return Array<uint8_t>();
            i += repeats + 1;
        }
        return ret;
    }
    Array<uint8_t> PackBits::DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        const size_t size = data.GetSize();
        Array<uint8_t> ret;
        size_t pos = 0;
        for (size_t i = 0; i < size;) {
            const int8_t tmp = data.At(i++);
            bool end = false;
            switch (tmp) {
                case 0 ... 127: {
                    const uint8_t count = tmp + 1;
                    for (uint8_t j = 0; j < count; j++) {
                        if (range.Contains(pos++) && !ret.Add(data.At(i++))) return Array<uint8_t>();
                        if (pos >= range.GetMax()) {
                            end = true;
                            break;
                        }
                    }
                    break;
                }
                case -127 ... -1: {
                    const uint8_t count = 1 - tmp;
                    const uint8_t value = data.At(i++);
                    for (uint8_t j = 0; j < count; j++) {
                        if (range.Contains(pos++) && !ret.Add(value)) return Array<uint8_t>();
                        if (pos >= range.GetMax()) {
                            end = true;
                            break;
                        }
                    }
                    break;
                }
                case -128: break;
                default: return Array<uint8_t>();
            }
            if (end) break;
        }
        return ret;
    }
}