#include "XZBlockFlags.hpp"

namespace MathLib {
    XZBlockFlags::XZBlockFlags(void) : filters(0), reserved(0), hasCompressedSize(false), hasUncompressedSize(false) {}
    XZBlockFlags::XZBlockFlags(uint8_t filters, bool hasCompressedSize, bool hasUncompressedSize) : filters(filters - 1), reserved(0), hasCompressedSize(hasCompressedSize), hasUncompressedSize(hasUncompressedSize) {}
}