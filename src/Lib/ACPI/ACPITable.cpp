#include "ACPITable.hpp"

namespace MathLib {
    bool ACPITable::CheckSignature(const char* expectedSignature) const {
        for (uint8_t i = 0; i < 4; i++)
            if (signature[i] != expectedSignature[i]) return false;
        return true;
    }
    bool ACPITable::IsValid(void) const {
        if (length < sizeof(ACPITable)) return false;
        uint8_t sum = 0;
        const uint8_t* self8 = (const uint8_t*)this;
        for (uint32_t i = 0; i < length; i++) sum += self8[i];
        return !sum;
    }
    uint32_t ACPITable::GetSize(void) const {
        return length - sizeof(ACPITable);
    }
}