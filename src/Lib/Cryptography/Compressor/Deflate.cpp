#include "Deflate.hpp"

namespace MathLib {
    Deflate::Deflate(void) {
        StartBenchmark
        for (uint8_t i = 0; i < 7; i++) fixedDataTree.lengthTable[i] = 0;
        fixedDataTree.lengthTable[7] = 24;
        fixedDataTree.lengthTable[8] = 152;
        fixedDataTree.lengthTable[9] = 112;
        for (uint8_t i = 0; i < 24; i++) fixedDataTree.translationTable[i] = i + 256;
        for (uint8_t i = 0; i < 144; i++) fixedDataTree.translationTable[i + 24] = i;
        for (uint8_t i = 0; i < 8; i++) fixedDataTree.translationTable[i + 24 + 144] = i + 280;
        for (uint8_t i = 0; i < 112; i++) fixedDataTree.translationTable[i + 24 + 144 + 8] = i + 144;
        for (uint8_t i = 0; i < 5; i++) fixedDistTree.lengthTable[i] = 0;
        fixedDistTree.lengthTable[5] = 32;
        for (uint8_t i = 0; i < 32; i++) fixedDistTree.translationTable[i] = i;
        EndBenchmark
    }
    void Deflate::GenerateTree(DeflateTree& tree, const uint8_t* lengths, uint16_t size) const {
        StartBenchmark
        for (uint8_t i = 0; i < 16; i++) tree.lengthTable[i] = 0;
        for (uint16_t i = 0; i < size; i++) tree.lengthTable[lengths[i]]++;
        tree.lengthTable[0] = 0;
        uint16_t offsets[16];
        uint16_t sum = 0;
        for (uint8_t i = 0; i < 16; i++) {
            offsets[i] = sum;
            sum += tree.lengthTable[i];
        }
        for (uint16_t i = 0; i < size; i++)
        if (lengths[i]) tree.translationTable[offsets[lengths[i]]++] = i;
        EndBenchmark
    }
    uint16_t Deflate::Decode(const DeflateTree& tree, const Bitmap& data, size_t& i) const {
        StartBenchmark
        uint32_t sum = 0;
        int32_t curr = 0;
        size_t len = 0;
        while (curr >= 0) {
            curr = curr * 2 + data.At(i++);
            sum += tree.lengthTable[++len];
            curr -= tree.lengthTable[len];
            
        };
        sum += curr;
        ReturnFromBenchmark(tree.translationTable[sum]);
    }
    Array<uint8_t> Deflate::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const {
        if (!key.IsEmpty()) ReturnFromBenchmark(Array<uint8_t>());
        Array<uint8_t> ret;
        if (encrypt) {
            size_t size = data.GetSize();
            size_t i = 0;
            while (true) {
                uint16_t writeSize = UINT16_MAX;
                bool last = false;
                if (size <= UINT16_MAX) {
                    last = true;
                    writeSize = size;
                }
                size -= writeSize;
                if (!ret.Add(last)) ReturnFromBenchmark(Array<uint8_t>());
                const uint8_t* tmp = (const uint8_t*)&writeSize;
                if (!ret.Add(tmp[0]) || !ret.Add(tmp[1])) ReturnFromBenchmark(Array<uint8_t>());
                const uint16_t invWriteSize = ~writeSize;
                tmp = (const uint8_t*)&invWriteSize;
                if (!ret.Add(tmp[0]) || !ret.Add(tmp[1])) ReturnFromBenchmark(Array<uint8_t>());
                for (uint32_t j = 0; j < writeSize; j++)
                    if (!ret.Add(data.At(i++))) ReturnFromBenchmark(Array<uint8_t>());
                if (last) break;
            }
            ReturnFromBenchmark(ret);
        }
        const Bitmap bitmap = Bitmap(data);
        size_t i = 0;
        while (true) {
            const bool last = bitmap.At(i++);
            const uint8_t type = bitmap.Read(i, 2);
            i += 2;
            switch (type) {
                case 0b00: {
                    const uint8_t tmp = i % 8;
                    if (tmp) i += 8 - tmp;
                    const uint16_t length = bitmap.Read(i, 16);
                    i += 16;
                    if (length != (uint16_t)~bitmap.Read(i, 16)) ReturnFromBenchmark(Array<uint8_t>());
                    i += 16;
                    for (uint32_t j = 0; j < length; j++) {
                        if (!ret.Add(bitmap.Read(i, 8))) ReturnFromBenchmark(Array<uint8_t>());
                        i += 8;
                    }
                    break;
                }
                case 0b01:
                case 0b10: {
                    DeflateTree dataTree;
                    DeflateTree distTree;
                    if (type == 0b10) {
                        const uint16_t literalCodes = bitmap.Read(i, 5) + 257;
                        i += 5;
                        const uint8_t distanceCodes = bitmap.Read(i, 5) + 1;
                        i += 5;
                        const uint8_t codeLengthCodes = bitmap.Read(i, 4) + 4;
                        i += 4;
                        uint8_t lengths[literalCodes + distanceCodes] = { 0, };
                        const uint8_t codeLengths[] = {
                            16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15,
                        };
                        for (uint8_t j = 0; j < codeLengthCodes; j++) {
                            lengths[codeLengths[j]] = bitmap.Read(i, 3);
                            i += 3;
                        }
                        GenerateTree(dataTree, lengths, SizeOfArray(codeLengths));
                        for (size_t j = 0; j < SizeOfArray(lengths);) {
                            const uint16_t code = Decode(dataTree, bitmap, i);
                            uint8_t length = 3;
                            uint8_t extra = 0;
                            uint8_t value = 0;
                            switch (code) {
                                case 16: {
                                    value = lengths[j - 1];
                                    extra = 2;
                                    break;
                                }
                                case 17: {
                                    extra = 3;
                                    break;
                                }
                                case 18: {
                                    length = 11;
                                    extra = 7;
                                    break;
                                }
                                case 0 ... 15: {
                                    lengths[j++] = code;
                                    continue;
                                }
                                default: ReturnFromBenchmark(Array<uint8_t>());
                            }
                            length += bitmap.Read(i, extra);
                            i += extra;
                            for (uint8_t k = 0; k < length; k++) lengths[j++] = value;
                        }
                        GenerateTree(dataTree, lengths, literalCodes);
                        GenerateTree(distTree, lengths + literalCodes, distanceCodes);
                    }
                    else {
                        dataTree = fixedDataTree;
                        distTree = fixedDistTree;
                    }
                    while (true) {
                        const uint16_t value = Decode(dataTree, bitmap, i);
                        if (value <= UINT8_MAX) {
                            if (!ret.Add(value)) ReturnFromBenchmark(Array<uint8_t>());
                        }
                        else if (value == 256) break;
                        else {
                            uint16_t length = 0;
                            uint8_t extra = 0;
                            switch (value) {
                                case 257 ... 264: {
                                    length = value - 254;
                                    break;
                                }
                                case 265 ... 268: {
                                    extra = 1;
                                    length = (value - 265) * 2 + 11;
                                    break;
                                }
                                case 269 ... 272: {
                                    extra = 2;
                                    length = (value - 269) * 4 + 19;
                                    break;
                                }
                                case 273 ... 276: {
                                    extra = 3;
                                    length = (value - 273) * 8 + 35;
                                    break;
                                }
                                case 277 ... 280: {
                                    extra = 4;
                                    length = (value - 277) * 16 + 67;
                                    break;
                                }
                                case 281 ... 284: {
                                    extra = 5;
                                    length = (value - 281) * 32 + 131;
                                    break;
                                }
                                case 285: {
                                    length = 258;
                                    break;
                                }
                                default: ReturnFromBenchmark(Array<uint8_t>());
                            }
                            length += bitmap.Read(i, extra);
                            i += extra;
                            const uint16_t distanceCode = Decode(distTree, bitmap, i);
                            extra = 0;
                            uint16_t distance = 0;
                            switch (distanceCode) {
                                case 0 ... 3: {
                                    distance = distanceCode + 1;
                                    break;
                                }
                                case 4 ... 5: {
                                    extra = 1;
                                    distance = (distanceCode - 4) * 2 + 5;
                                    break;
                                }
                                case 6 ... 7: {
                                    extra = 2;
                                    distance = (distanceCode - 6) * 4 + 9;
                                    break;
                                }
                                case 8 ... 9: {
                                    extra = 3;
                                    distance = (distanceCode - 8) * 8 + 17;
                                    break;
                                }
                                case 10 ... 11: {
                                    extra = 4;
                                    distance = (distanceCode - 10) * 16 + 33;
                                    break;
                                }
                                case 12 ... 13: {
                                    extra = 5;
                                    distance = (distanceCode - 12) * 32 + 65;
                                    break;
                                }
                                case 14 ... 15: {
                                    extra = 6;
                                    distance = (distanceCode - 14) * 64 + 129;
                                    break;
                                }
                                case 16 ... 17: {
                                    extra = 7;
                                    distance = (distanceCode - 16) * 128 + 257;
                                    break;
                                }
                                case 18 ... 19: {
                                    extra = 8;
                                    distance = (distanceCode - 18) * 256 + 513;
                                    break;
                                }
                                case 20 ... 21: {
                                    extra = 9;
                                    distance = (distanceCode - 20) * 512 + 1025;
                                    break;
                                }
                                case 22 ... 23: {
                                    extra = 10;
                                    distance = (distanceCode - 22) * 1024 + 2049;
                                    break;
                                }
                                case 24 ... 25: {
                                    extra = 11;
                                    distance = (distanceCode - 24) * 2048 + 4097;
                                    break;
                                }
                                case 26 ... 27: {
                                    extra = 12;
                                    distance = (distanceCode - 26) * 4096 + 8193;
                                    break;
                                }
                                case 28 ... 29: {
                                    extra = 13;
                                    distance = (distanceCode - 28) * 8192 + 16385;
                                    break;
                                }
                            }
                            distance += bitmap.Read(i, extra);
                            i += extra;
                            const size_t start = ret.GetSize() - distance;
                            for (uint16_t j = 0; j < length; j++)
                            if (!ret.Add(ret.At(j + start))) ReturnFromBenchmark(Array<uint8_t>());
                        }
                    }
                    break;
                }
                default: ReturnFromBenchmark(Array<uint8_t>());
            }
            if (last) break;
        }
        ReturnFromBenchmark(ret);
    }
}