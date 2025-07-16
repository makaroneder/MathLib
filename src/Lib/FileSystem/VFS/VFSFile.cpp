#include "VFSFile.hpp"
#include "../../Benchmark.hpp"

namespace MathLib {
    VFSFile::VFSFile(size_t fs, size_t index) : fs(fs), index(index), free(false) {
        EmptyBenchmark
    }
    bool VFSFile::operator==(const VFSFile& other) const {
        StartAndReturnFromBenchmark(fs == other.fs && index == other.index && free == other.free);
    }
    bool VFSFile::operator!=(const VFSFile& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}