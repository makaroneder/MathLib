#include "ELFProgramHeader.hpp"

namespace MathLib {
    bool ELFProgramHeader::operator==(const ELFProgramHeader& other) const {
        return type == other.type && executable == other.executable && writable == other.writable && readable == other.readable && flags == other.flags && offset == other.offset && virtualAddress == other.virtualAddress && physicalAddress == other.physicalAddress && fileSize == other.fileSize && memorySize == other.memorySize && alignment == other.alignment;
    }
    bool ELFProgramHeader::operator!=(const ELFProgramHeader& other) const {
        return !(*this == other);
    }
}