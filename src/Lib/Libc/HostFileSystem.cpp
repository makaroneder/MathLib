#ifndef Freestanding
#include "HostFileSystem.hpp"
#ifndef __MINGW32__
#if __cplusplus >= 201703
    #include <filesystem>
    namespace fs = std::filesystem;
#else
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#endif
#endif

namespace MathLib {
    size_t HostFileSystem::OpenInternal(const String& path, OpenMode mode) {
        StartBenchmark
        const char* modeStr = "";
        switch (mode) {
            case OpenMode::Read: {
                modeStr = "rb";
                break;
            }
            case OpenMode::Write: {
                modeStr = "wb";
                break;
            }
            case OpenMode::ReadWrite: {
                modeStr = "rb+";
                break;
            }
            default: ReturnFromBenchmark(SIZE_MAX);
        }
        FILE* file = fopen(path.GetValue(), modeStr);
        if (!file) ReturnFromBenchmark(SIZE_MAX);
        for (size_t i = 0; i < files.GetSize(); i++) {
            if (!files[i]) {
                files[i] = file;
                ReturnFromBenchmark(i);
            }
        }
        ReturnFromBenchmark(files.Add(file) ? files.GetSize() - 1 : SIZE_MAX);
    }
    bool HostFileSystem::Close(size_t file) {
        StartBenchmark
        FILE* raw = GetFile(file);
        if (!raw || fflush(raw) || fclose(raw)) ReturnFromBenchmark(false);
        files[file] = nullptr;
        ReturnFromBenchmark(true);
    }
    size_t HostFileSystem::Read(size_t file, void* buffer, size_t size, size_t position) {
        StartBenchmark
        FILE* raw = GetFile(file);
        if (!raw || fseek(raw, position, SEEK_SET)) ReturnFromBenchmark(0);
        ReturnFromBenchmark(fread(buffer, sizeof(uint8_t), size, raw));
    }
    size_t HostFileSystem::Write(size_t file, const void* buffer, size_t size, size_t position) {
        StartBenchmark
        FILE* raw = GetFile(file);
        if (!raw || fseek(raw, position, SEEK_SET)) ReturnFromBenchmark(0);
        ReturnFromBenchmark(fwrite(buffer, sizeof(uint8_t), size, raw));
    }
    size_t HostFileSystem::GetSize(size_t file) {
        StartBenchmark
        FILE* raw = GetFile(file);
        if (!raw) ReturnFromBenchmark(0);
        const size_t pos = ftell(raw);
        if (fseek(raw, 0, SEEK_END)) ReturnFromBenchmark(0);
        const size_t ret = ftell(raw);
        if (fseek(raw, pos, SEEK_SET)) ReturnFromBenchmark(0);
        ReturnFromBenchmark(ret);
    }
    Array<FileInfo> HostFileSystem::ReadDirectory(const String& path_) {
        StartBenchmark
        #ifndef __MINGW32__
        const String path = String(fs::current_path().c_str()) + '/' + path_;
        if (!fs::exists(path.GetValue()) || !fs::is_directory(path.GetValue())) ReturnFromBenchmark(Array<FileInfo>());
        Array<FileInfo> ret;
        for (const fs::directory_entry& entry : fs::directory_iterator(path.GetValue())) {
            FileInfo::Type type = FileInfo::Type::Unknown;
            if (fs::is_directory(entry.status())) type = FileInfo::Type::Directory;
            else if (fs::is_regular_file(entry.status())) type = FileInfo::Type::File;
            if (!ret.Add(FileInfo(type, entry.path().filename().string()))) ReturnFromBenchmark(Array<FileInfo>());
        }
        ReturnFromBenchmark(ret);
        #else
        (void)path_;
        ReturnFromBenchmark(Array<FileInfo>());
        #endif
    }
    FILE* HostFileSystem::GetFile(size_t file) {
        StartAndReturnFromBenchmark(file < files.GetSize() ? files[file] : nullptr);
    }
}

#endif