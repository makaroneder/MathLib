#ifndef Freestanding
#include "HostFileSystem.hpp"
#include <experimental/filesystem>
#include <filesystem>

namespace fs = std::filesystem;

size_t HostFileSystem::OpenInternal(const String& path, const OpenMode& mode) {
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
bool HostFileSystem::Close(const size_t& file) {
    FILE* raw = GetFile(file);
    if (!raw || fflush(raw) || fclose(raw)) return false;
    files[file] = nullptr;
    return true;
}
size_t HostFileSystem::Read(const size_t& file, void* buffer, const size_t& size, const size_t& position) {
    FILE* raw = GetFile(file);
    if (!raw || fseek(raw, position, SEEK_SET)) return 0;
    return fread(buffer, sizeof(uint8_t), size, raw);
}
size_t HostFileSystem::Write(const size_t& file, const void* buffer, const size_t& size, const size_t& position) {
    FILE* raw = GetFile(file);
    if (!raw || fseek(raw, position, SEEK_SET)) return 0;
    return fwrite(buffer, sizeof(uint8_t), size, raw);
}
size_t HostFileSystem::GetSize(const size_t& file) {
    FILE* raw = GetFile(file);
    if (!raw) return false;
    const size_t pos = ftell(raw);
    if (fseek(raw, 0, SEEK_END)) return 0;
    const size_t ret = ftell(raw);
    if (fseek(raw, pos, SEEK_SET)) return 0;
    return ret;
}
Array<FileInfo> HostFileSystem::ReadDirectory(const String& path) {
    if (!fs::exists(path.GetValue()) || !fs::is_directory(path.GetValue())) return Array<FileInfo>();
    Array<FileInfo> ret;
    for (const auto& entry : fs::directory_iterator(path.GetValue())) {
        FileInfo::Type type = FileInfo::Type::Unknown;
        if (fs::is_directory(entry.status())) type = FileInfo::Type::Directory;
        else if (fs::is_regular_file(entry.status())) type = FileInfo::Type::File;
        if (!ret.Add(FileInfo(type, entry.path().string()))) return Array<FileInfo>();
    }
    return ret;
}
FILE* HostFileSystem::GetFile(const size_t& file) {
    return file < files.GetSize() ? files[file] : nullptr;
}

#endif