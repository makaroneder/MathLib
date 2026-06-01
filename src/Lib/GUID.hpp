#ifndef MathLib_GUID_H
#define MathLib_GUID_H
#include "Interfaces/Printable.hpp"

namespace MathLib {
    struct GUID {
        uint32_t data1;
        uint16_t data2[3];
        uint16_t data3[3];
    } __attribute__((packed));
    MakeFormatter(GUID, self, padding, {
        String ret = CollectionToString(padding) + MathLib::ToString(self.data1, 16, sizeof(uint32_t) * 2) + '-';
        for (size_t i = 0; i < SizeOfArray(self.data2); i++)
            ret += MathLib::ToString(self.data2[i], 16, sizeof(uint16_t) * 2) + '-';
        for (size_t i = 0; i < SizeOfArray(self.data3); i++)
            ret += MathLib::ToString(self.data3[i], 16, sizeof(uint16_t) * 2);
        return ret;
    });
}

#endif