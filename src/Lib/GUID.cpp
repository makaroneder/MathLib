#include "GUID.hpp"
#include "String.hpp"

namespace MathLib {
    GUID::GUID(RawGUID guid) {
        StartBenchmark
        data1 = guid.data1;
        for (size_t i = 0; i < SizeOfArray(data2); i++) data2[i] = guid.data2[i];
        data3 = guid.data3;
        EndBenchmark
    }
    String GUID::ToString(const Sequence<char>& padding) const {
        StartBenchmark
        String ret = CollectionToString(padding) + MathLib::ToString(data1, 16, sizeof(uint32_t) * 2) + '-';
        for (size_t i = 0; i < SizeOfArray(data2); i++)
            ret += MathLib::ToString(data2[i], 16, sizeof(uint16_t) * 2) + '-';
        ReturnFromBenchmark(ret + MathLib::ToString(data3, 16, 6 * 2));
    }
}