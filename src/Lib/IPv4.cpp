#include "IPv4.hpp"
#include "String.hpp"
#include "Memory.hpp"

namespace MathLib {
    IPv4::IPv4(void) : data(0) {
        EmptyBenchmark
    }
    IPv4::IPv4(const Sequence<char>& str) {
        const Array<String> split = Split(str, '.'_M, false);
        const size_t size = split.GetSize();
        if (!size || size > sizeof(uint32_t)) Panic("Invalid IPv4 provided");
        uint8_t* buff = (uint8_t*)&data;
        for (uint8_t i = 0; i < size - 1; i++) buff[i] = StringToNumber(split.At(i));
        const size_t final = StringToNumber(split.At(size - 1));
        MemoryCopy(&final, &buff[size - 1], sizeof(uint32_t) - size + 1);
        uint8_t end = sizeof(uint32_t);
        for (uint8_t i = size - 1; i < end; i++)
            Swap<uint8_t>(buff[i], buff[--end]);
    }
    bool IPv4::Save(Writable& file) const {
        StartAndReturnFromBenchmark(file.Write<uint32_t>(data));
    }
    bool IPv4::Load(Readable& file) {
        StartAndReturnFromBenchmark(file.Read<uint32_t>(data));
    }
    String IPv4::ToString(const Sequence<char>& padding) const {
        StartBenchmark
        String ret = CollectionToString(padding);
        const uint8_t* buff8 = (const uint8_t*)&data;
        for (uint8_t i = 0; i < sizeof(uint32_t); i++) {
            if (i) ret += '.';
            ret += MathLib::ToString(buff8[i], 10);
        }
        ReturnFromBenchmark(ret);
    }
}