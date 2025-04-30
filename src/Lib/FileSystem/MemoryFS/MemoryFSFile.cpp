#include "MemoryFSFile.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    MemoryFSFile::MemoryFSFile(size_t resource, bool write) : resource(resource), write(write), free(false) {
        EmptyBenchmark
    }
    bool MemoryFSFile::operator==(const MemoryFSFile& other) const {
        StartAndReturnFromBenchmark(resource == other.resource && write == other.write && free == other.free);
    }
    bool MemoryFSFile::operator!=(const MemoryFSFile& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}