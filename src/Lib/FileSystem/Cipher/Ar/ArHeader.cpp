#include "ArHeader.hpp"
#include "../../../String.hpp"
#include "../../../ExternArray.hpp"

namespace MathLib {
    ArHeader::ArHeader(void) : signature(0) {}
    ArHeader::ArHeader(const Sequence<char>& path, size_t dataSize) : signature(expectedSignature) {
        const uint8_t pathSize = Min<size_t>(path.GetSize(), SizeOfArray(name) - 1);
        for (uint8_t i = 0; i < pathSize; i++) name[i] = path.At(i);
        name[pathSize] = '/';
        for (uint8_t i = pathSize + 1; i < SizeOfArray(name); i++) name[i] = ' ';
        const String sizeStr = ToString(dataSize, 10, SizeOfArray(size));
        if (sizeStr.GetSize() != SizeOfArray(size)) Panic("Ar file too large");
        for (uint8_t i = 0; i < SizeOfArray(size); i++) size[i] = sizeStr.At(i);
        modificationTimestamp[0] = ownerID[0] = groupID[0] = mode[0] = '0';
        for (uint8_t i = 1; i < SizeOfArray(ownerID); i++)
            modificationTimestamp[i] = ownerID[i] = groupID[i] = mode[i] = ' ';
        for (uint8_t i = SizeOfArray(ownerID); i < SizeOfArray(mode); i++)
            modificationTimestamp[i] = mode[i] = ' ';
        for (uint8_t i = SizeOfArray(mode); i < SizeOfArray(modificationTimestamp); i++)
            modificationTimestamp[i] = ' ';
        for (uint8_t i = 0; i < 3; i++) mode[SizeOfArray(mode) - i - 1] = '6';
    }
    bool ArHeader::IsValid(void) const {
        if (signature != expectedSignature) return false;
        for (uint8_t i = 0; i < SizeOfArray(ownerID); i++) {
            if (!(IsBetween(ownerID[i], '0', '9') || (i && ownerID[i] == ' '))) return false;
            if (!(IsBetween(groupID[i], '0', '9') || (i && groupID[i] == ' '))) return false;
            if (!(IsBetween(mode[i], '0', '7') || (i && mode[i] == ' '))) return false;
            if (!(IsBetween(size[i], '0', '9') || (i && size[i] == ' '))) return false;
            if (!(IsBetween(modificationTimestamp[i], '0', '9') || (i && modificationTimestamp[i] == ' '))) return false;
        }
        for (uint8_t i = SizeOfArray(ownerID); i < SizeOfArray(mode); i++) {
            if (!(IsBetween(mode[i], '0', '7') || mode[i] == ' ')) return false;
            if (!(IsBetween(size[i], '0', '9') || size[i] == ' ')) return false;
            if (!(IsBetween(modificationTimestamp[i], '0', '9') || modificationTimestamp[i] == ' ')) return false;
        }
        for (uint8_t i = SizeOfArray(mode); i < SizeOfArray(size); i++) {
            if (!(IsBetween(size[i], '0', '9') || size[i] == ' ')) return false;
            if (!(IsBetween(modificationTimestamp[i], '0', '9') || modificationTimestamp[i] == ' ')) return false;
        }
        for (uint8_t i = SizeOfArray(size); i < SizeOfArray(modificationTimestamp); i++)
            if (!(IsBetween(modificationTimestamp[i], '0', '9') || modificationTimestamp[i] == ' ')) return false;
        return true;
    }
    String ArHeader::GetName(void) const {
        String ret;
        for (uint8_t i = 0; i < SizeOfArray(name); i++) {
            if (name[i] == '/') break;
            ret += name[i];
        }
        return ret;
    }
    size_t ArHeader::GetSize(void) const {
        return (size_t)StringToNumber(ExternArray<char>((char*)size, SizeOfArray(size)), 10);
    }
}