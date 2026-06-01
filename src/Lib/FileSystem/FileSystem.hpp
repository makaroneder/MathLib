#ifndef MathLib_FileSystem_H
#define MathLib_FileSystem_H
#include "SeekMode.hpp"
#include "OpenMode.hpp"
#include "FileInfo.hpp"
#include "File.hpp"

namespace MathLib {
    struct FileSystem : Allocatable {
        [[nodiscard]] File Open(const Sequence<char>& path, OpenMode mode);
        [[nodiscard]] virtual bool IsValid(void) const = 0;
        [[nodiscard]] virtual size_t OpenInternal(const Sequence<char>& path, OpenMode mode) = 0;
        [[nodiscard]] virtual bool Close(size_t file) = 0;
        [[nodiscard]] virtual size_t Read(size_t file, void* buffer, size_t size, size_t position) = 0;
        [[nodiscard]] virtual size_t Write(size_t file, const void* buffer, size_t size, size_t position) = 0;
        [[nodiscard]] virtual size_t GetSize(size_t file) = 0;
        [[nodiscard]] virtual Array<FileInfo> ReadDirectory(const Sequence<char>& path) = 0;
        [[nodiscard]] virtual bool CreateDirectory(const Sequence<char>& path, bool overwrite) = 0;
        [[nodiscard]] String ListFiles(const Sequence<char>& path, size_t maxDepth, const Sequence<char>& padding = ""_M);
        [[nodiscard]] bool ListFiles(Writable& logger, const Sequence<char>& path, size_t maxDepth, const Sequence<char>& padding = ""_M);
        [[nodiscard]] bool ListFiles(Writable* logger, const Sequence<char>& path, size_t maxDepth, const Sequence<char>& padding = ""_M);
        [[nodiscard]] Array<FileInfo> GetContentsOfDirectory(const Sequence<char>& path);
    };
}

#endif