#include "MemoryFS.hpp"

namespace MathLib {
    MemoryFS::MemoryFS(void) : resources(Array<MemoryFSResource>()), files(Array<MemoryFSFile>()) {
        EmptyBenchmark
    }
    size_t MemoryFS::OpenInternal(const String& path, OpenMode mode) {
        StartBenchmark
        size_t resource = SIZE_MAX;
        for (size_t i = 0; i < resources.GetSize() && resource == SIZE_MAX; i++)
            if (resources.At(i).name == path) resource = i;
        if (mode == OpenMode::Write) {
            if (resource == SIZE_MAX) {
                if (!resources.Add(MemoryFSResource(path))) ReturnFromBenchmark(SIZE_MAX);
                resource = resources.GetSize() - 1;
            }
            else resources.At(resource).buffer = Array<uint8_t>();
        }
        if (resource == SIZE_MAX) ReturnFromBenchmark(SIZE_MAX);
        const MemoryFSFile ret = MemoryFSFile(resource, mode != OpenMode::Read);
        for (size_t i = 0; i < files.GetSize(); i++) {
            if (files.At(i).free) {
                files.At(i) = ret;
                ReturnFromBenchmark(i);
            }
        }
        ReturnFromBenchmark(files.Add(ret) ? files.GetSize() - 1 : SIZE_MAX);
    }
    bool MemoryFS::Close(size_t file) {
        StartBenchmark
        if (files.GetSize() <= file || resources.GetSize() <= files.At(file).resource) ReturnFromBenchmark(false);
        files.At(file).free = true;
        ReturnFromBenchmark(true);
    }
    size_t MemoryFS::Read(size_t file, void* buffer, size_t size, size_t position) {
        StartBenchmark
        if (files.GetSize() <= file || resources.GetSize() <= files.At(file).resource) ReturnFromBenchmark(0);
        const size_t s = resources.At(files.At(file).resource).buffer.GetSize() < size + position ? resources.At(files.At(file).resource).buffer.GetSize() - position : size;
        uint8_t* buff8 = (uint8_t*)buffer;
        for (size_t i = 0; i < s; i++) buff8[i] = resources.At(files.At(file).resource).buffer.At(i + position);
        ReturnFromBenchmark(s);
    }
    size_t MemoryFS::Write(size_t file, const void* buffer, size_t size, size_t position) {
        StartBenchmark
        if (files.GetSize() <= file || resources.GetSize() <= files.At(file).resource || !files.At(file).write) ReturnFromBenchmark(0);
        const size_t s = resources.At(files.At(file).resource).buffer.GetSize() < size + position ? resources.At(files.At(file).resource).buffer.GetSize() - position : size;
        const uint8_t* buff8 = (const uint8_t*)buffer;
        for (size_t i = 0; i < s; i++) resources.At(files.At(file).resource).buffer.At(i + position) = buff8[i];
        for (size_t i = s; i < size; i++)
            if (!resources.At(files.At(file).resource).buffer.Add(buff8[i])) ReturnFromBenchmark(s + i - 1);
        ReturnFromBenchmark(size);
    }
    size_t MemoryFS::GetSize(size_t file) {
        StartBenchmark
        if (files.GetSize() <= file || resources.GetSize() <= files.At(file).resource) ReturnFromBenchmark(0);
        ReturnFromBenchmark(resources.At(files.At(file).resource).buffer.GetSize());
    }
    Array<FileInfo> MemoryFS::ReadDirectory(const String& path) {
        StartBenchmark
        if (path.IsEmpty() || path == "/") {
            Array<FileInfo> ret = Array<FileInfo>(resources.GetSize());
            for (size_t i = 0; i < ret.GetSize(); i++) ret.At(i) = FileInfo(FileInfo::Type::File, resources.At(i).name);
            ReturnFromBenchmark(ret);
        }
        else ReturnFromBenchmark(Array<FileInfo>());
    }
}