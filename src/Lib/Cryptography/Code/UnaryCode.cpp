#include "UnaryCode.hpp"
#include "../../Bitmap.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    Array<uint8_t> UnaryCode::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        const size_t size = data.GetSize();
        if (key.GetSize() != 1 || !size) return Array<uint8_t>();
        const bool symbol = key.AtUnsafe(0);
        Bitmap ret;
        for (size_t i = 0; i < size; i++) {
            const uint8_t x = data.AtUnsafe(i);
            for (uint16_t j = 0; j < x; j++)
                if (!ret.Add(symbol)) return Array<uint8_t>();
            if (!ret.Add(!symbol)) return Array<uint8_t>();
        }
        Array<uint8_t> tmp = ret.GetArray();
        return tmp.Add(ret.GetLastByteSize()) ? tmp : Array<uint8_t>();
    }
    Array<uint8_t> UnaryCode::DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        const size_t size = data.GetSize();
        if (key.GetSize() != 1 || !size) return Array<uint8_t>();
        const bool symbol = key.AtUnsafe(0);
        Bitmap bitmap = Bitmap(SubSequence<uint8_t>(data, Interval<size_t>(0, size - 1)), data.AtUnsafe(size - 1));
        const size_t bitmapSize = bitmap.GetSize();
        Array<uint8_t> ret;
        size_t pos = 0;
        size_t count = 0;
        for (size_t i = 0; i < bitmapSize; i++) {
            if (bitmap.AtUnsafe(i) == symbol) {
                count++;
                continue;
            }
            if (range.Contains(pos++) && !ret.Add(count)) return Array<uint8_t>();
            if (pos >= range.GetMax()) break;
            count = 0;
        }
        return ret;
    }
}