#include "BIP8.hpp"
#include "../../Bitmap.hpp"

namespace MathLib {
    Array<uint8_t> BIP8::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        StartBenchmark
        if (key.GetSize() != 1) ReturnFromBenchmark(Array<uint8_t>());
        const bool ret = Bitmap(data).GetCountOf(true) % 2;
        ReturnFromBenchmark(MakeArray<uint8_t>(key.At(0) ? ret : !ret));
    }
}