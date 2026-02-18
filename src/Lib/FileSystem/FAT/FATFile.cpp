#include "FATFile.hpp"

namespace MathLib {
    FATFile::FATFile(void) {}
    FATFile::FATFile(uint32_t cluster, uint32_t size, bool write) : cluster(cluster), size(size), write(write), free(false) {}
    bool FATFile::operator==(const FATFile& other) const {
        return cluster == other.cluster && size == other.size && write == other.write && free == other.free;
    }
    bool FATFile::operator!=(const FATFile& other) const {
        return !(*this == other);
    }
}