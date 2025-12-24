#include "SHA1.hpp"
#include "../../Interfaces/Sequence/SequenceUnion.hpp"
#include "../../Interfaces/Sequence/ExtendedSequence.hpp"
#include "../../Interfaces/Sequence/FixedSizeCollection.hpp"

namespace MathLib {
    Array<uint8_t> SHA1::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        const size_t keySize = key.GetSize();
        if (keySize > 1) return Array<uint8_t>();
        const bool sha0 = keySize && key.AtUnsafe(0);
        const ExtendedSequence<uint8_t> tmp1 = ExtendedSequence<uint8_t>(data, 0x80, 1);
        const size_t padLen = (56 - (tmp1.GetSize() % 64) + 64) % 64;
        const ExtendedSequence<uint8_t> tmp2 = ExtendedSequence<uint8_t>(tmp1, 0x00, padLen);
        const uint64_t originalSize = data.GetSize() * 8;
        FixedSizeCollection<uint8_t, sizeof(uint64_t)> originalSizeSequence;
        for (uint8_t i = 0; i < sizeof(uint64_t); i++)
            originalSizeSequence.AtUnsafe(i) = originalSize >> (56 - 8 * i);
        const SequenceUnion<uint8_t> input = SequenceUnion<uint8_t>(tmp2, originalSizeSequence);
        const size_t size = input.GetSize();
        uint32_t h[] = {
            0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0,
        };
        for (size_t i = 0; i < size; i += 64) {
            FixedSizeCollection<uint32_t, 80> w;
            w.FillBytes32(0);
            for (uint8_t j = 0; j < 16; j++) {
                w.AtUnsafe(j) = input.AtUnsafe(i + 4 * j + 0) << 24;
                w.AtUnsafe(j) |= input.AtUnsafe(i + 4 * j + 1) << 16;
                w.AtUnsafe(j) |= input.AtUnsafe(i + 4 * j + 2) << 8;
                w.AtUnsafe(j) |= input.AtUnsafe(i + 4 * j + 3);
            }
            if (sha0) for (uint8_t j = 16; j < 80; j++)
                w.AtUnsafe(j) = w.AtUnsafe(j - 3) ^ w.AtUnsafe(j - 8) ^ w.AtUnsafe(j - 14) ^ w.AtUnsafe(j - 16);
            else for (uint8_t j = 16; j < 80; j++)
                w.AtUnsafe(j) = CircularLeftShift<uint32_t>(w.AtUnsafe(j - 3) ^ w.AtUnsafe(j - 8) ^ w.AtUnsafe(j - 14) ^ w.AtUnsafe(j - 16), 1);
            uint32_t a = h[0];
            uint32_t b = h[1];
            uint32_t c = h[2];
            uint32_t d = h[3];
            uint32_t e = h[4];
            for (uint8_t j = 0; j < 80; j++) {
                uint32_t k;
                uint32_t f;
                if (j < 20) {
                    f = (b & c) | (~b & d);
                    k = 0x5a827999;
                }
                else if (j < 40) {
                    f = b ^ c ^ d;
                    k = 0x6ed9eba1;
                }
                else if (j < 60) {
                    f = (b & c) | (b & d) | (c & d);
                    k = 0x8f1bbcdc;
                }
                else {
                    f = b ^ c ^ d;
                    k = 0xca62c1d6;
                }
                const uint32_t tmp = CircularLeftShift<uint32_t>(a, 5) + f + e + k + w.AtUnsafe(j);
                e = d;
                d = c;
                c = CircularLeftShift<uint32_t>(b, 30);
                b = a;
                a = tmp;
            }
            h[0] += a;
            h[1] += b;
            h[2] += c;
            h[3] += d;
            h[4] += e;
        }
        Array<uint8_t> ret = Array<uint8_t>(SizeOfArray(h) * sizeof(uint32_t));
        for (uint8_t i = 0; i < sizeof(uint32_t); i++) {
            for (uint8_t j = 0; j < SizeOfArray(h); j++)
                ret[i + j * sizeof(uint32_t)] = h[j] >> (24 - 8 * i);
        }
        return ret;
    }
}