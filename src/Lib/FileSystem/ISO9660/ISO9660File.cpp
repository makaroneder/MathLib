#include "ISO9660File.hpp"

namespace MathLib {
    ISO9660File::ISO9660File(ISO9660DirectoryEntry entry, OpenMode mode) : entry(entry), mode(mode), free(false) {}
    bool ISO9660File::operator==(const ISO9660File& other) const {
        return entry == other.entry && mode == other.mode && free == other.free;
    }
    bool ISO9660File::operator!=(const ISO9660File& other) const {
        return !(*this == other);
    }
}