#include "FATFile.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    FATFile::FATFile(void) {
        EmptyBenchmark
    }
    FATFile::FATFile(uint32_t cluster, uint32_t size, bool write) : cluster(cluster), size(size), write(write), free(false) {
        EmptyBenchmark
    }
}