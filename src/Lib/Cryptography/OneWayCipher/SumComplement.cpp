#include "SumComplement.hpp"
#include "../../Bitmap.hpp"

namespace MathLib {
    Array<uint8_t> SumComplement::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        if (key.GetSize() != 1) return Array<uint8_t>();
        const uint64_t bits = key.At(0);
        Bitmap ret = Bitmap(bits);
        ret.Fill(false);
        Bitmap mask = ret;
        mask.Fill(true);
        const Bitmap input = Bitmap(data);
        const size_t size = input.GetSize();
        for (size_t i = 0; i < size; i += bits) {
            bool overflow = false;
            for (size_t j = 0; j < bits; j++) {
                const bool tmp = ret.At(j);
                if (tmp == input.At(i + j)) {
                    if (!ret.Set(j, overflow)) return Array<uint8_t>();
                    overflow = tmp;
                }
                else if (!ret.Set(j, !overflow)) return Array<uint8_t>();
            }
            ret &= mask;
        }
        return (~ret).GetArray();
    }
}