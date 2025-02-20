#include "File.hpp"
#include "../Host.hpp"
#include "FileSystem.hpp"

namespace MathLib {
    File::File(FileSystem& fileSystem, size_t index) : fileSystem(fileSystem), index(index) {
        EmptyBenchmark
    }
    File::~File(void) {
        StartBenchmark
        if (index != SIZE_MAX && !fileSystem.Close(index)) Panic("Failed to close file");
        EndBenchmark
    }
    bool File::Exists(void) const {
        return index != SIZE_MAX;
    }
    size_t File::ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) {
        StartAndReturnFromBenchmark(index == SIZE_MAX ? 0 : fileSystem.Read(index, buffer, size, position));
    }
    size_t File::WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) {
        StartAndReturnFromBenchmark(index == SIZE_MAX ? 0 : fileSystem.Write(index, buffer, size, position));
    }
    size_t File::GetSize(void) const {
        StartAndReturnFromBenchmark(index == SIZE_MAX ? 0 : fileSystem.GetSize(index));
    }
}