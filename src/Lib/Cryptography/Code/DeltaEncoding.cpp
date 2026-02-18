#include "DeltaEncoding.hpp"

namespace MathLib {
    Array<uint8_t> DeltaEncoding::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        const size_t size = data.GetSize();
        uint8_t prev = 0;
        Array<uint8_t> ret = Array<uint8_t>(size);
        for (size_t i = 0; i < size; i++) {
            const uint8_t tmp = data.AtUnsafe(i);
            ret.AtUnsafe(i) = tmp - prev;
            prev = tmp;
        }
        return ret;
    }
    Array<uint8_t> DeltaEncoding::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        const size_t size = data.GetSize();
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(range.GetMax(), size);
        uint8_t prev = 0;
        Array<uint8_t> ret = Array<uint8_t>(end - start);
        for (size_t i = 0; i < end; i++) {
            prev += data.AtUnsafe(i);
            if (i >= start) ret.AtUnsafe(i - start) = prev;
        }
        return ret;
    }
}