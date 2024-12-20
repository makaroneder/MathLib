#include "GUID.hpp"
#include "String.hpp"

namespace MathLib {
    GUID::GUID(RawGUID guid) {
        data1 = guid.data1;
        for (size_t i = 0; i < SizeOfArray(data2); i++) data2[i] = guid.data2[i];
        data3 = guid.data3;
    }
    String GUID::ToString(const String& padding) const {
        String ret = padding + MathLib::ToString(data1, 16, sizeof(uint32_t) * 2) + '-';
        for (size_t i = 0; i < SizeOfArray(data2); i++)
            ret += MathLib::ToString(data2[i], 16, sizeof(uint16_t) * 2) + '-';
        return ret + MathLib::ToString(data3, 16, 6 * 2);
    }
}