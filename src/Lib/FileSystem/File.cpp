#include "File.hpp"
#include "../Host.hpp"

namespace MathLib {
    File::File(FileSystem& fileSystem, size_t index) : fileSystem(fileSystem), index(index) {
        EmptyBenchmark
    }
    File::~File(void) {
        StartBenchmark
        if (index != SIZE_MAX && !fileSystem.Close(index)) Panic("Failed to close file");
        EndBenchmark
    }
    size_t File::ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) {
        StartBenchmark
        ReturnFromBenchmark(index == SIZE_MAX ? 0 : fileSystem.Read(index, buffer, size, position));
    }
    size_t File::WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) {
        StartBenchmark
        ReturnFromBenchmark(index == SIZE_MAX ? 0 : fileSystem.Write(index, buffer, size, position));
    }
    size_t File::GetSize(void) const {
        StartBenchmark
        ReturnFromBenchmark(index == SIZE_MAX ? 0 : fileSystem.GetSize(index));
    }
}