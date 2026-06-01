#include "ExecutableData.hpp"

namespace MathLib {
    ExecutableData::ExecutableData(void) : ExecutableSegment(), entry(0) {}
    ExecutableData::ExecutableData(uint64_t address, const Array<uint8_t>& data, uint64_t entry) : ExecutableSegment(address, data), entry(entry) {}
    ExecutableData::ExecutableData(uint64_t start, uint64_t end, uint64_t entry) : ExecutableSegment(start, end), entry(entry) {}
    bool ExecutableData::LoadSegment(const ExecutableSegment& segment) {
        const uint64_t start = segment.address;
        const uint64_t end = start + segment.data.GetSize();
        if (start == end) return true;
        if (!(IsBetween(start, address, address + data.GetSize() - 1) && IsBetween(end, address, address + data.GetSize()))) return false;
        MemoryCopy(segment.data.GetValue(), data.GetValue() + start - address, segment.data.GetSize());
        return true;
    }
    String ExecutableData::ToString(const Sequence<char>& padding) const {
        const String padd = CollectionToString(padding);
        const String padd2 = padd + '\t';
        return "{\n"_M +
            padd2 + "Address: 0x" + MathLib::ToString(address, 16, 16) + '\n' +
            padd2 + "Entry: 0x" + MathLib::ToString(entry, 16, 16) + '\n' +
            padd2 + "Data: {\n" +
                DumpMemory<uint8_t>(data, 16, padd2 + '\t') +
            padd2 + "}\n" +
        padd + '}';
    }
}
