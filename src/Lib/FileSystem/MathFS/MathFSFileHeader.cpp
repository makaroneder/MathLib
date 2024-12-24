#include "MathFSFileHeader.hpp"
#include "../../Memory.hpp"
#include "../../Host.hpp"

namespace MathLib {
    MathFSFileHeader::MathFSFileHeader(void) : type(Type::LastEntry), lba(0), size(0) {
        EmptyBenchmark
    }
    MathFSFileHeader::MathFSFileHeader(Type type, uint64_t lba, uint64_t size, const String& path) : type(type), lba(lba), size(size) {
        StartBenchmark
        MemoryCopy(path.GetValue(), name, Min(path.GetSize(), SizeOfArray(name)));
        name[SizeOfArray(name) - 1] = '\0';
        EndBenchmark
    }
}