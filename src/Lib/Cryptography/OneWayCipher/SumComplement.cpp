#include "SumComplement.hpp"
#include "../../Bitmap.hpp"

namespace MathLib {
    Array<uint8_t> SumComplement::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        StartBenchmark
        if (key.GetSize() != 1) ReturnFromBenchmark(MathLib::Array<uint8_t>());
        const uint64_t bits = key.At(0);
        MathLib::Bitmap ret = MathLib::Bitmap(bits);
        ret.Fill(false);
        MathLib::Bitmap mask = ret;
        mask.Fill(true);
        const MathLib::Bitmap input = MathLib::Bitmap(data);
        const size_t size = input.GetSize();
        for (size_t i = 0; i < size; i += bits) {
            bool overflow = false;
            for (size_t j = 0; j < bits; j++) {
                const bool tmp = ret.At(j);
                if (tmp == input.At(i + j)) {
                    if (!ret.Set(j, overflow)) ReturnFromBenchmark(MathLib::Array<uint8_t>());
                    overflow = tmp;
                }
                else if (!ret.Set(j, !overflow)) ReturnFromBenchmark(MathLib::Array<uint8_t>());
            }
            ret &= mask;
        }
        ReturnFromBenchmark((~ret).GetArray());
    }
}