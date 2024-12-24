#include "MathFSFile.hpp"

namespace MathLib {
    MathFSFile::MathFSFile(const MathFSFileHeader& entry, bool write) : entry(entry), write(write), free(false) {
        EmptyBenchmark
    }
}