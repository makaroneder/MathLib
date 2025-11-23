#include "RunLengthEncoding.hpp"

namespace MathLib {
    Array<uint8_t> RunLengthEncoding::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        const size_t size = data.GetSize();
        if (key.GetSize() != 1 || !size) return Array<uint8_t>();
        const bool sizeFirst = key.AtUnsafe(0);
        Array<uint8_t> ret;
        uint8_t prev = data.AtUnsafe(0);
        uint8_t count = 0;
        for (size_t i = 1; i < size; i++) {
            const uint8_t tmp = data.AtUnsafe(i);
            if (prev != tmp || count == UINT8_MAX) {
                if (sizeFirst) Swap<uint8_t>(count, prev);
                if (!ret.Add(prev) || !ret.Add(count)) return Array<uint8_t>();
                count = 0;
                prev = tmp;
            }
            else count++;
        }
        if (sizeFirst) Swap<uint8_t>(count, prev);
        return ret.Add(prev) && ret.Add(count) ? ret : Array<uint8_t>();
    }
    Array<uint8_t> RunLengthEncoding::DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        const size_t size = data.GetSize();
        if (key.GetSize() != 1 || !size) return Array<uint8_t>();
        const bool sizeFirst = key.AtUnsafe(0);
        Array<uint8_t> ret;
        size_t pos = 0;
        for (size_t i = 0; i < size; i += 2) {
            const uint8_t count = data.AtUnsafe(i + !sizeFirst);
            const uint8_t x = data.AtUnsafe(i + sizeFirst);
            bool end = false;
            for (uint16_t j = 0; j <= count; j++) {
                if (range.Contains(pos++) && !ret.Add(x)) return Array<uint8_t>();
                if (pos >= range.GetMax()) {
                    end = true;
                    break;
                }
            }
            if (end) break;
        }
        return ret;
    }
}