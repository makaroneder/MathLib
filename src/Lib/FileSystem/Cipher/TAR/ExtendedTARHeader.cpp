#include "ExtendedTARHeader.hpp"

namespace MathLib {
    bool ExtendedTARHeader::IsValid(const char* expectedSignature, const char* expectedVersion) const {
        if (!TARHeader::IsValid()) return false;
        for (uint8_t i = 0; i < SizeOfArray(signature); i++)
            if (signature[i] != expectedSignature[i]) return false;
        for (uint8_t i = 0; i < SizeOfArray(version); i++)
            if (version[i] != expectedVersion[i]) return false;
        return true;
    }
    bool ExtendedTARHeader::Create(const Sequence<char>& path_, size_t dataSize, const char* expectedSignature, const char* expectedVersion) {
        for (uint8_t i = 0; i < SizeOfArray(signature); i++) signature[i] = expectedSignature[i];
        for (uint8_t i = 0; i < SizeOfArray(version); i++) version[i] = expectedVersion[i];
        const String name = "MathLib";
        const size_t nameSize = name.GetSize();
        for (uint8_t i = 0; i < SizeOfArray(ownerName); i++)
            ownerName[i] = groupName[i] = i < nameSize ? name.At(i) : '\0';
        for (uint8_t i = 0; i < SizeOfArray(deviceMajor); i++) deviceMajor[i] = deviceMinor[i] = '\0';
        return TARHeader::Create(path_, dataSize);
    }
}