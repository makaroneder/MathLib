#include "ExtendedFileSystemInode.hpp"

namespace MathLib {
    uint64_t ExtendedFileSystemInode::GetSize(bool extendedFileSize) const {
        return extendedFileSize && type == (uint8_t)Type::File ? ((uint64_t)upperSize << 32) | lowerSize : lowerSize;
    }
}