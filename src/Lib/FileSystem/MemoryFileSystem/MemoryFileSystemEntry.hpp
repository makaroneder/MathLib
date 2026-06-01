#ifndef MathLib_FileSystem_MemoryEntry_H
#define MathLib_FileSystem_MemoryEntry_H
#include "../../Interfaces/Sequence/ByteArray.hpp"
#include "../FileInfo.hpp"

namespace MathLib {
    struct MemoryFileSystemEntry;
    struct MemoryFileSystemEntry : ByteDevice, Comparable<MemoryFileSystemEntry> {
        enum class Type : uint8_t {
            None,
            File,
            Directory,
        };
        String name;
        ByteArray data;
        Array<MemoryFileSystemEntry> children;
        Type type;

        MemoryFileSystemEntry(void);
        MemoryFileSystemEntry(const String& name, Type type);
        MemoryFileSystemEntry(const String& name, const Array<uint8_t>& data);
        MemoryFileSystemEntry(const String& name, const Array<MemoryFileSystemEntry>& children);
        [[nodiscard]] virtual size_t GetSize(void) const override;
        [[nodiscard]] virtual size_t ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) override;
        [[nodiscard]] FileInfo ToFileInfo(void) const;
        [[nodiscard]] MemoryFileSystemEntry* Find(const Sequence<char>& path);
        [[nodiscard]] MemoryFileSystemEntry* Create(const Sequence<char>& path, Type retType, bool overwrite);
        [[nodiscard]] Array<FileInfo> ReadDirectory(const Sequence<char>& path);

        protected:
        [[nodiscard]] virtual bool Equals(const MemoryFileSystemEntry& other) const override;
    };
}

#endif