#include "GNUTARHeader.hpp"

namespace MathLib {
    bool GNUTARHeader::IsValid(void) const {
        return ExtendedTARHeader::IsValid(expectedSignature, expectedVersion);
    }
    bool GNUTARHeader::Create(const Sequence<char>& path_, size_t dataSize) {
        extended = false;
        reserved1 = 0;
        for (uint8_t i = 0; i < SizeOfArray(lastAccessTime); i++)
            lastAccessTime[i] = lastChangeTime[i] = offset[i] = gnuSize[i] = '\0';
        for (uint8_t i = 0; i < SizeOfArray(longNames); i++) longNames[i] = '\0';
        for (uint8_t i = 0; i < SizeOfArray(sparse); i++) {
            for (uint8_t j = 0; j < SizeOfArray(sparse[i].offset); j++) {
                sparse[i].offset[j] = '\0';
                sparse[i].size[j] = '\0';
            }
        }
        for (uint8_t i = 0; i < SizeOfArray(reserved2); i++) reserved2[i] = 0;
        return ExtendedTARHeader::Create(path_, dataSize, expectedSignature, expectedVersion);
    }
}