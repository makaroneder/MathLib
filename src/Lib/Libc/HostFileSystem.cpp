#ifndef Freestanding
#include "HostFileSystem.hpp"
#if __cplusplus >= 201703
    #include <filesystem>
    namespace fs = std::filesystem;
#else
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#endif

namespace MathLib {
    size_t HostFileSystem::OpenInternal(const String& path, OpenMode mode) {
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
            default: return SIZE_MAX;
        }
        FILE* file = fopen(path.GetValue(), modeStr);
        if (!file) return SIZE_MAX;
        for (size_t i = 0; i < files.GetSize(); i++) {
            if (!files[i]) {
                files[i] = file;
                return i;
            }
        }
        return files.Add(file) ? files.GetSize() - 1 : SIZE_MAX;
    }
    bool HostFileSystem::Close(size_t file) {
        FILE* raw = GetFile(file);
        if (!raw || fflush(raw) || fclose(raw)) return false;
        files[file] = nullptr;
        return true;
    }
    size_t HostFileSystem::Read(size_t file, void* buffer, size_t size, size_t position) {
        FILE* raw = GetFile(file);
        if (!raw || fseek(raw, position, SEEK_SET)) return 0;
        return fread(buffer, sizeof(uint8_t), size, raw);
    }
    size_t HostFileSystem::Write(size_t file, const void* buffer, size_t size, size_t position) {
        FILE* raw = GetFile(file);
        if (!raw || fseek(raw, position, SEEK_SET)) return 0;
        return fwrite(buffer, sizeof(uint8_t), size, raw);
    }
    size_t HostFileSystem::GetSize(size_t file) {
        FILE* raw = GetFile(file);
        if (!raw) return false;
        const size_t pos = ftell(raw);
        if (fseek(raw, 0, SEEK_END)) return 0;
        const size_t ret = ftell(raw);
        if (fseek(raw, pos, SEEK_SET)) return 0;
        return ret;
    }
    Array<FileInfo> HostFileSystem::ReadDirectory(const String& path_) {
        const String path = String(fs::current_path().c_str()) + '/' + path_;
        if (!fs::exists(path.GetValue()) || !fs::is_directory(path.GetValue())) return Array<FileInfo>();
        Array<FileInfo> ret;
        for (const fs::directory_entry& entry : fs::directory_iterator(path.GetValue())) {
            FileInfo::Type type = FileInfo::Type::Unknown;
            if (fs::is_directory(entry.status())) type = FileInfo::Type::Directory;
            else if (fs::is_regular_file(entry.status())) type = FileInfo::Type::File;
            if (!ret.Add(FileInfo(type, entry.path().filename().string()))) return Array<FileInfo>();
        }
        return ret;
    }
    FILE* HostFileSystem::GetFile(size_t file) {
        return file < files.GetSize() ? files[file] : nullptr;
    }
}

#endif