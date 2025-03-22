#include "CRC.hpp"
#include "../../Bitmap.hpp"

namespace MathLib {
    Array<uint8_t> CRC::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        // TODO: Support for bits < 8
        // TODO: Allow for reversing output
        StartBenchmark
        if (key.GetSize() != 4) ReturnFromBenchmark(Array<uint8_t>());
        const uint8_t bits = key.At(0);
        Bitmap byteShift = Bitmap(bits - 8);
        byteShift.Fill(false);
        const uint64_t polynomial = key.At(1);
        const Bitmap polynomialBitmap = Bitmap(Array<uint8_t>((const uint8_t*)&polynomial, sizeof(uint64_t)));
        Bitmap mask = Bitmap(bits);
        mask.Fill(true);
        const bool reverseInput = key.At(2);
        const uint64_t xorValue = key.At(3);
        Bitmap ret;
        const size_t size = data.GetSize();
        for (size_t i = 0; i < size; i++) {
            Bitmap tmp = byteShift;
            const uint8_t x = data.At(i);
            if (!tmp.Add(Bitmap(MakeArray<uint8_t>(reverseInput ? BitReverse<uint8_t>(x) : x)))) ReturnFromBenchmark(Array<uint8_t>());
            ret ^= tmp & mask;
            for (uint8_t j = 0; j < 8; j++) {
                Bitmap tmpBitmap = Bitmap(1);
                tmpBitmap.Fill(false);
                if (!tmpBitmap.Add(ret)) ReturnFromBenchmark(Array<uint8_t>());
                ret = tmpBitmap;
                if (ret.At(bits)) ret = (ret ^ polynomialBitmap) & mask;
            }
        }
        ReturnFromBenchmark((ret ^ Bitmap(Array<uint8_t>((const uint8_t*)&xorValue, sizeof(uint64_t)))).GetArray());
    }
}