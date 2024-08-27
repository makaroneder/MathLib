#ifndef ISO9660File_H
#define ISO9660File_H
#include "DirectoryEntry.hpp"
#include "../OpenMode.hpp"

struct ISO9660File {
    DirectoryEntry entry;
    OpenMode mode;
    bool free;

    ISO9660File(DirectoryEntry entry = DirectoryEntry(), OpenMode mode = OpenMode::Read);
};

#endif