#ifndef MathLib_FileSystem_ISO9660_File_H
#define MathLib_FileSystem_ISO9660_File_H
#include "ISO9660DirectoryEntry.hpp"
#include "../OpenMode.hpp"

namespace MathLib {
    struct ISO9660File {
        ISO9660DirectoryEntry entry;
        OpenMode mode;
        bool free;

        ISO9660File(ISO9660DirectoryEntry entry = ISO9660DirectoryEntry(), OpenMode mode = OpenMode::Read);
        [[nodiscard]] bool operator==(const ISO9660File& other) const;
        [[nodiscard]] bool operator!=(const ISO9660File& other) const;
    };
}

#endif