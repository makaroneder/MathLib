#include "ExecutableSegment.hpp"

namespace MathLib {
    ExecutableSegment::ExecutableSegment(void) : data(), address(0) {}
    ExecutableSegment::ExecutableSegment(uint64_t address, const Array<uint8_t>& data) : data(data), address(address) {}
    ExecutableSegment::ExecutableSegment(uint64_t start, uint64_t end) : data(end - start), address(start) {}
    String ExecutableSegment::ToString(const Sequence<char>& padding) const {
        const String padd = CollectionToString(padding);
        const String padd2 = padd + '\t';
        return "{\n"_M +
            padd2 + "Address: 0x" + MathLib::ToString(address, 16, 16) + '\n' +
            padd2 + "Data: {\n" +
                DumpMemory<uint8_t>(data, 16, padd2 + '\t') +
            padd2 + "}\n" +
        padd + '}';
    }
}
