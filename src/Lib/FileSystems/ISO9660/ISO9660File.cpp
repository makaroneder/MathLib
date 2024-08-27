#include "ISO9660File.hpp"

ISO9660File::ISO9660File(DirectoryEntry entry, OpenMode mode) : entry(entry), mode(mode), free(false) {}