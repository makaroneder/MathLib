#include "FATFSInfo.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    bool FATFSInfo::IsValid(void) const {
        return *(const uint32_t*)&signature1 == *(const uint32_t*)&expectedSignature1 && *(const uint32_t*)&signature2 == *(const uint32_t*)&expectedSignature2 && signature3 == expectedSignature3;
    }
    bool FATFSInfo::operator==(const FATFSInfo& other) const {
        if (*(const uint32_t*)signature1 != *(const uint32_t*)other.signature1 || *(const uint32_t*)signature2 != *(const uint32_t*)other.signature2 || lastFreeCluster != other.lastFreeCluster || firstAvailableCluster != other.firstAvailableCluster || signature3 != other.signature3) return false;
        for (uint8_t i = 0; i < SizeOfArray(reserved1); i++)
            if (reserved1[i] != other.reserved1[i]) return false;
        for (uint8_t i = 0; i < SizeOfArray(reserved2); i++)
            if (reserved2[i] != other.reserved2[i]) return false;
        return true;
    }
    bool FATFSInfo::operator!=(const FATFSInfo& other) const {
        return !(*this == other);
    }
}