#ifndef FileSystems_ISO9660_File_H
#define FileSystems_ISO9660_File_H
#include "ISO9660DirectoryEntry.hpp"
#include "../OpenMode.hpp"

struct ISO9660File {
    ISO9660DirectoryEntry entry;
    OpenMode mode;
    bool free;

    ISO9660File(ISO9660DirectoryEntry entry = ISO9660DirectoryEntry(), OpenMode mode = OpenMode::Read);
};

#endif