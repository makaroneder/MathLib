#include "CRC.hpp"
#include "../../Bitmap.hpp"

namespace MathLib {
    Array<uint8_t> CRC::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        // TODO: Support for bits < 8
        StartBenchmark
        if (key.GetSize() != 6) ReturnFromBenchmark(Array<uint8_t>());
        const size_t bits = key.At(0);
        Bitmap byteShift = Bitmap(bits - 8);
        byteShift.Fill(false);
        const uint64_t polynomial = key.At(1);
        const Bitmap polynomialBitmap = Bitmap(Array<uint8_t>((const uint8_t*)&polynomial, sizeof(uint64_t)));
        Bitmap mask = Bitmap(bits);
        mask.Fill(true);
        const bool reverseInput = key.At(2);
        const bool reverseOutput = key.At(3);
        const uint64_t xorValue = key.At(4);
        Bitmap ret = Bitmap(bits);
        ret.Fill(key.At(5));
        const size_t size = data.GetSize();
        for (size_t i = 0; i < size; i++) {
            const uint8_t x = data.At(i);
            Bitmap tmp = byteShift;
            if (!tmp.AddSequence(Bitmap(MakeArray<uint8_t>(reverseInput ? BitReverse<uint8_t>(x) : x)))) ReturnFromBenchmark(Array<uint8_t>());
            ret ^= tmp & mask;
            for (uint8_t j = 0; j < 8; j++) {
                Bitmap tmpBitmap = Bitmap(1);
                tmpBitmap.Fill(false);
                if (!tmpBitmap.AddSequence(ret)) ReturnFromBenchmark(Array<uint8_t>());
                ret = tmpBitmap;
                if (ret.At(bits)) ret = (ret ^ polynomialBitmap) & mask;
            }
        }
        const Bitmap xorBitmap = Bitmap(Array<uint8_t>((const uint8_t*)&xorValue, sizeof(uint64_t)));
        Bitmap tmp = Bitmap(bits);
        for (size_t i = 0; i < bits; i++)
            if (!tmp.Set(i, ret.At(reverseOutput ? (bits - 1 - i) : i) ^ xorBitmap.At(i))) ReturnFromBenchmark(Array<uint8_t>());
        ReturnFromBenchmark(tmp.GetArray());
    }
}