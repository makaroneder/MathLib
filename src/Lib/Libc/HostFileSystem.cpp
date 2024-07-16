#ifndef Freestanding
#include "HostFileSystem.hpp"
#include <stdarg.h>

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
    files.Add(file);
    return files.GetSize() - 1;
}
bool HostFileSystem::Close(const size_t& file) {
    FILE* raw = GetFile(file);
    if (!raw || fflush(raw) || fclose(raw)) return false;
    files[file] = nullptr;
    return true;
}
bool HostFileSystem::Read(const size_t& file, void* buffer, const size_t& size) {
    FILE* raw = GetFile(file);
    return raw && fread(buffer, sizeof(uint8_t), size, raw) == size;
}
bool HostFileSystem::Write(const size_t& file, const void* buffer, const size_t& size) {
    FILE* raw = GetFile(file);
    return raw && fwrite(buffer, sizeof(uint8_t), size, raw) == size;
}
bool HostFileSystem::Seek(const size_t& file, const ssize_t& offset, const SeekMode& mode) {
    FILE* raw = GetFile(file);
    int seek;
    switch (mode) {
        case SeekMode::Set: {
            seek = SEEK_SET;
            break;
        }
        case SeekMode::Current: {
            seek = SEEK_CUR;
            break;
        }
        case SeekMode::End: {
            seek = SEEK_END;
            break;
        }
    }
    return raw && !fseek(raw, offset, seek);
}
size_t HostFileSystem::Tell(const size_t& file) {
    FILE* raw = GetFile(file);
    return raw ? ftell(raw) : SIZE_MAX;
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
FILE* HostFileSystem::GetFile(const size_t& file) {
    return file < files.GetSize() ? files[file] : nullptr;
}

#endif