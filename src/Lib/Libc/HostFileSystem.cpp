#ifndef Freestanding
#include "HostFileSystem.hpp"
#include <stdarg.h>

size_t HostFileSystem::Open(String path, OpenMode mode) {
    const char* modeStr = "";
    switch (mode) {
        case OpenMode::Read: {
            modeStr = "r";
            break;
        }
        case OpenMode::Write: {
            modeStr = "w";
            break;
        }
        case OpenMode::ReadByte: {
            modeStr = "rb";
            break;
        }
        case OpenMode::WriteByte: {
            modeStr = "wb";
            break;
        }
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
bool HostFileSystem::Close(size_t file) {
    FILE* raw = GetFile(file);
    if (!raw || fclose(raw)) return false;
    files[file] = nullptr;
    return true;
}
bool HostFileSystem::Read(size_t file, void* buffer, size_t size) {
    FILE* raw = GetFile(file);
    return raw && fread(buffer, sizeof(uint8_t), size, raw) == size;
}
bool HostFileSystem::Write(size_t file, const void* buffer, size_t size) {
    FILE* raw = GetFile(file);
    return raw && fwrite(buffer, sizeof(uint8_t), size, raw) == size;
}
bool HostFileSystem::Flush(size_t file) {
    FILE* raw = GetFile(file);
    return raw && !fflush(raw);
}
bool HostFileSystem::Seek(size_t file, ssize_t offset, SeekMode mode) {
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
size_t HostFileSystem::Tell(size_t file) {
    FILE* raw = GetFile(file);
    return raw ? ftell(raw) : SIZE_MAX;
}
FILE* HostFileSystem::GetFile(size_t file) {
    return file < files.GetSize() ? files[file] : nullptr;
}

#endif