#include "MemoryFileSystemEntry.hpp"
#include "../Path.hpp"

namespace MathLib {
    MemoryFileSystemEntry::MemoryFileSystemEntry(void) : name(), data(), children(), type(Type::None) {}
    MemoryFileSystemEntry::MemoryFileSystemEntry(const String& name, Type type) : name(name), data(), children(), type(type) {}
    MemoryFileSystemEntry::MemoryFileSystemEntry(const String& name, const Array<uint8_t>& data) : name(name), data(data), children(), type(Type::File) {}
    MemoryFileSystemEntry::MemoryFileSystemEntry(const String& name, const Array<MemoryFileSystemEntry>& children) : name(name), data(), children(children), type(Type::Directory) {}
    size_t MemoryFileSystemEntry::GetSize(void) const {
        return type == Type::File ? data.GetSize() : 0;
    }
    size_t MemoryFileSystemEntry::ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) {
        return type == Type::File ? ReadPositionedSizedBuffer(buffer, size, position) : 0;
    }
    size_t MemoryFileSystemEntry::WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) {
        return type == Type::File ? WritePositionedSizedBuffer(buffer, size, position) : 0;
    }
    FileInfo MemoryFileSystemEntry::ToFileInfo(void) const {
        FileInfo::Type tmp = FileInfo::Type::Unknown;
        switch (type) {
            case Type::File: {
                tmp = FileInfo::Type::File;
                break;
            }
            case Type::Directory: {
                tmp = FileInfo::Type::Directory;
                break;
            }
            default: break;
        }
        return FileInfo(tmp, name);
    }
    MemoryFileSystemEntry* MemoryFileSystemEntry::Find(const Sequence<char>& path) {
        if (path.IsEmpty()) return this;
        if (type != Type::Directory) return nullptr;
        const SingleTypePair<String> tmp = PopFirstPathElement(path);
        for (MemoryFileSystemEntry& entry : children) {
            if (entry.name != tmp.first) continue;
            return entry.Find(tmp.second);
        }
        return nullptr;
    }
    MemoryFileSystemEntry* MemoryFileSystemEntry::Create(const Sequence<char>& path, Type retType, bool overwrite) {
        if (type != Type::Directory) return nullptr;
        const String base = RemoveLastPathElement(path);
        const String last = RemoveBasePath(base, path);
        MemoryFileSystemEntry* const dir = Find(base);
        if (!dir || dir->type != Type::Directory) return nullptr;
        MemoryFileSystemEntry* const ret = dir->Find(last);
        if (!ret) return dir->children.Add(MemoryFileSystemEntry(last, retType)) ? &dir->children.AtUnsafe(dir->children.GetSize() - 1) : nullptr;
        if (ret->type != retType) return nullptr;
        if (overwrite) {
            ret->data = Array<uint8_t>();
            ret->children = Array<MemoryFileSystemEntry>();
        }
        return ret;
    }
    Array<FileInfo> MemoryFileSystemEntry::ReadDirectory(const Sequence<char>& path) {
        const MemoryFileSystemEntry* const dir = Find(path);
        if (dir->type != Type::Directory) return Array<FileInfo>();
        const size_t size = dir->children.GetSize();
        Array<FileInfo> ret = size;
        for (size_t i = 0; i < size; i++) ret.AtUnsafe(i) = dir->children.AtUnsafe(i).ToFileInfo();
        return ret;
    }
    bool MemoryFileSystemEntry::Equals(const MemoryFileSystemEntry& other) const {
        return name == other.name;
    }
}