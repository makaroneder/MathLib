#include "UnaryCode.hpp"
#include "../../Bitmap.hpp"

namespace MathLib {
    Array<uint8_t> UnaryCode::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const {
        StartBenchmark
        const size_t size = data.GetSize();
        if (key.GetSize() != 1 || !size) ReturnFromBenchmark(Array<uint8_t>());
        const bool symbol = key.At(0);
        if (encrypt) {
            Bitmap ret;
            for (size_t i = 0; i < size; i++) {
                const uint8_t x = data.At(i);
                for (uint16_t j = 0; j < x; j++)
                    if (!ret.Add(symbol)) ReturnFromBenchmark(Array<uint8_t>());
                if (!ret.Add(!symbol)) ReturnFromBenchmark(Array<uint8_t>());
            }
            Array<uint8_t> tmp = ret.GetArray();
            ReturnFromBenchmark(tmp.Add(ret.GetLastByteSize()) ? tmp : Array<uint8_t>());
        }
        Bitmap bitmap = Bitmap(data, data.At(size - 1));
        const size_t bitmapSize = bitmap.GetSize();
        Array<uint8_t> ret;
        size_t count = 0;
        for (size_t i = 0; i < bitmapSize; i++) {
            if (bitmap.At(i) == symbol) count++;
            else {
                if (!ret.Add(count)) ReturnFromBenchmark(Array<uint8_t>());
                count = 0;
            }
        }
        ReturnFromBenchmark(ret);
    }
}