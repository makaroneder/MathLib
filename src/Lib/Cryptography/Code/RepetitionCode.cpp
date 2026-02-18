#include "RepetitionCode.hpp"
#include "../../Bitmap.hpp"

namespace MathLib {
    Array<uint8_t> RepetitionCode::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (key.type != CipherKey::Type::Normal || key.data.GetSize() != sizeof(size_t)) return Array<uint8_t>();
        const size_t n = key.data.AsT<size_t>().Get();
        const Bitmap tmp = Bitmap(data);
        const size_t size = tmp.GetSize();
        Bitmap ret = Bitmap(size * n);
        for (size_t i = 0; i < size; i++) {
            const Expected<bool> val = tmp.Get(i);
            if (!val.HasValue()) return Array<uint8_t>();
            for (size_t j = 0; j < n; j++)
                if (!ret.SetUnsafe(n * i + j, val.Get())) return Array<uint8_t>();
        }
        return ret.GetArray();
    }
    Array<uint8_t> RepetitionCode::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (key.type != CipherKey::Type::Normal || key.data.GetSize() != sizeof(size_t)) return Array<uint8_t>();
        const size_t n = key.data.AsT<size_t>().Get();
        const Bitmap tmp = Bitmap(data);
        const size_t start = range.GetMin() * 8;
        const size_t end = Min<size_t>(tmp.GetSize() / n, range.GetMax() * 8);
        if (start >= end) return Array<uint8_t>();
        Bitmap ret = Bitmap(end - start);
        for (size_t i = start; i < end; i++) {
            size_t ones = 0;
            for (size_t j = 0; j < n; j++) {
                const Expected<bool> val = tmp.Get(n * i + j);
                if (!val.HasValue()) return Array<uint8_t>();
                ones += val.Get();
            }
            if (!ret.SetUnsafe(i - start, ones * 2 >= n)) return Array<uint8_t>();
        }
        return ret.GetArray();
    }
}