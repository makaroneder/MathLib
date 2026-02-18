#include "XZRecord.hpp"

namespace MathLib {
    XZRecord::XZRecord(void) : unpaddedSize(), uncompressedSize() {}
    XZRecord::XZRecord(size_t unpaddedSize, size_t uncompressedSize) : unpaddedSize(unpaddedSize), uncompressedSize(uncompressedSize) {}
    bool XZRecord::Equals(const XZRecord& other) const {
        return unpaddedSize == other.unpaddedSize && uncompressedSize == other.uncompressedSize;
    }
}