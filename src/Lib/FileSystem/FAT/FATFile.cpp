#include "FATFile.hpp"
#include "../../Benchmark.hpp"

namespace MathLib {
    FATFile::FATFile(void) {
        EmptyBenchmark
    }
    FATFile::FATFile(uint32_t cluster, uint32_t size, bool write) : cluster(cluster), size(size), write(write), free(false) {
        EmptyBenchmark
    }
    bool FATFile::operator==(const FATFile& other) const {
        StartAndReturnFromBenchmark(cluster == other.cluster && size == other.size && write == other.write && free == other.free);
    }
    bool FATFile::operator!=(const FATFile& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}