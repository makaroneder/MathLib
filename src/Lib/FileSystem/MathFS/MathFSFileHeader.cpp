#include "MathFSFileHeader.hpp"
#include "../../MinMax.hpp"
#include "../../Memory.hpp"
#include "../../Host.hpp"

namespace MathLib {
    MathFSFileHeader::MathFSFileHeader(void) : type(Type::LastEntry), lba(0), size(0) {
        EmptyBenchmark
    }
    MathFSFileHeader::MathFSFileHeader(Type type, uint64_t lba, uint64_t size, const Sequence<char>& path) : type(type), lba(lba), size(size) {
        StartBenchmark
        const size_t tmp = Min<size_t>(path.GetSize(), SizeOfArray(name));
        for (size_t i = 0; i < tmp; i++) name[i] = path.At(i);
        name[SizeOfArray(name) - 1] = '\0';
        EndBenchmark
    }
}