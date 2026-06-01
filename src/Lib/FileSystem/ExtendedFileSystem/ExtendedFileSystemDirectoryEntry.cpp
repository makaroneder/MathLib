#include "ExtendedFileSystemDirectoryEntry.hpp"

namespace MathLib {
    uint16_t ExtendedFileSystemDirectoryEntry::GetNameLength(bool directoriesWithTypes) const {
        return directoriesWithTypes ? nameLength8 : nameLength16;
    }
}