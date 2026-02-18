#include "CRC.hpp"
#include "../../Bitmap.hpp"

namespace MathLib {
    Array<uint8_t> CRC::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        // TODO: Support for bits < 8
        if (key.type != CipherKey::Type::MultiKey || key.children.GetSize() != 6) return Array<uint8_t>();
        const CipherKey key1 = key.children.AtUnsafe(0);
        if (key1.type != CipherKey::Type::Normal || key1.data.GetSize() != sizeof(size_t)) return Array<uint8_t>();
        const CipherKey key2 = key.children.AtUnsafe(1);
        if (key2.type != CipherKey::Type::Normal || key2.data.GetSize() != sizeof(Polynomial)) return Array<uint8_t>();
        const CipherKey key3 = key.children.AtUnsafe(2);
        if (key3.type != CipherKey::Type::Normal || key3.data.GetSize() != sizeof(bool)) return Array<uint8_t>();
        const CipherKey key4 = key.children.AtUnsafe(3);
        if (key4.type != CipherKey::Type::Normal || key4.data.GetSize() != sizeof(bool)) return Array<uint8_t>();
        const CipherKey key5 = key.children.AtUnsafe(4);
        if (key5.type != CipherKey::Type::Normal || key5.data.GetSize() != sizeof(uint64_t)) return Array<uint8_t>();
        const CipherKey key6 = key.children.AtUnsafe(5);
        if (key6.type != CipherKey::Type::Normal || key6.data.GetSize() != sizeof(bool)) return Array<uint8_t>();
        const size_t bits = key1.data.AsT<size_t>().Get();
        Bitmap byteShift = Bitmap(bits - 8);
        byteShift.Fill(false);
        const Polynomial polynomial = key2.data.AsT<Polynomial>().Get();
        const Bitmap polynomialBitmap = Bitmap(Array<uint8_t>((const uint8_t*)&polynomial, sizeof(uint64_t)));
        Bitmap mask = Bitmap(bits);
        mask.Fill(true);
        const bool reverseInput = key3.data.AsT<bool>().Get();
        const bool reverseOutput = key4.data.AsT<bool>().Get();
        const uint64_t xorValue = key5.data.AsT<uint64_t>().Get();
        Bitmap ret = Bitmap(bits);
        ret.Fill(key6.data.AsT<bool>().Get());
        const size_t size = data.GetSize();
        for (size_t i = 0; i < size; i++) {
            const uint8_t x = data.At(i);
            Bitmap tmp = byteShift;
            if (!tmp.AddSequence(Bitmap(MakeArray<uint8_t>(reverseInput ? BitReverse<uint8_t>(x) : x)))) return Array<uint8_t>();
            ret ^= tmp & mask;
            for (uint8_t j = 0; j < 8; j++) {
                Bitmap tmpBitmap = Bitmap(1);
                tmpBitmap.Fill(false);
                if (!tmpBitmap.AddSequence(ret)) return Array<uint8_t>();
                ret = tmpBitmap;
                if (ret.At(bits)) ret = (ret ^ polynomialBitmap) & mask;
            }
        }
        const Bitmap xorBitmap = Bitmap(Array<uint8_t>((const uint8_t*)&xorValue, sizeof(uint64_t)));
        Bitmap tmp = Bitmap(bits);
        for (size_t i = 0; i < bits; i++)
            if (!tmp.Set(i, ret.At(reverseOutput ? (bits - 1 - i) : i) ^ xorBitmap.At(i))) return Array<uint8_t>();
        return tmp.GetArray();
    }
}