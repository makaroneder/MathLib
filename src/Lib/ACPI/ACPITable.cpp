#include "ACPITable.hpp"
#include "../Typedefs.hpp"

namespace MathLib {
    bool ACPITable::IsValid(void) const {
        StartBenchmark
        uint8_t sum = 0;
        const uint8_t* self8 = (const uint8_t*)this;
        for (uint32_t i = 0; i < length; i++) sum += self8[i];
        ReturnFromBenchmark(!sum);
    }
}