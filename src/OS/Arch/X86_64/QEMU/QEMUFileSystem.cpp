#ifdef __x86_64__
#include "QEMUFileSystem.hpp"
#include "../IO.hpp"
#include <FunctionT.hpp>
#include <FileSystem/Path.hpp>

void QEMUFileSystem::SetSelector(QEMUSelector selector) const {
    WritePort<uint16_t>((uint16_t)IOOffset::Selector, (uint16_t)selector);
}
void QEMUFileSystem::Read(void* buffer, size_t size) const {
    uint8_t* buff = (uint8_t*)buffer;
    for (size_t i = 0; i < size; i++) buff[i] = ReadPort<uint8_t>((uint16_t)IOOffset::Data);
}
bool QEMUFileSystem::IsValid(void) const {
    char buff[4];
    SetSelector(QEMUSelector::Signature);
    Read(buff, SizeOfArray(buff));
    for (uint8_t i = 0; i < SizeOfArray(buff); i++)
        if (buff[i] != expectedSignature[i]) return false;
    return true;
}
bool QEMUFileSystem::Foreach(const MathLib::Function<bool, QEMUFileEntry>& function) {
    uint32_t entries = 0;
    SetSelector(QEMUSelector::Directory);
    Read(&entries, sizeof(uint32_t));
    entries = MathLib::SwapEndian32(entries);
    for (uint64_t i = 0; i < entries; i++) {
        QEMUFileEntry entry;
        Read(&entry, sizeof(QEMUFileEntry));
        if (function(entry)) return true;
    }
    return false;
}
size_t QEMUFileSystem::OpenInternal(const MathLib::Sequence<char>& path, MathLib::OpenMode mode) {
    if (mode == MathLib::OpenMode::Write) return SIZE_MAX;
    QEMUSelector selector = QEMUSelector::Signature;
    uint32_t size = 0;
    if (!Foreach(MathLib::MakeFunctionT<bool, QEMUFileEntry>([&path, &selector, &size](QEMUFileEntry entry) -> bool {
        if (path != MathLib::String(entry.name)) return false;
        selector = (QEMUSelector)MathLib::SwapEndian16((uint16_t)entry.selector);
        size = MathLib::SwapEndian32(entry.size);
        return true;
    }))) return SIZE_MAX;
    const QEMUFile ret = QEMUFile(selector, size);
    for (size_t i = 0; i < files.GetSize(); i++) {
        if (files.At(i).free) {
            files.At(i) = ret;
            return i;
        }
    }
    return files.Add(ret) ? files.GetSize() - 1 : SIZE_MAX;
}
bool QEMUFileSystem::Close(size_t file) {
    if (file >= files.GetSize() || files.At(file).free) return false;
    files.At(file).free = true;
    return true;
}
size_t QEMUFileSystem::Read(size_t file, void* buffer, size_t size, size_t position) {
    if (file >= files.GetSize() || files.At(file).free) return 0;
    const QEMUFile raw = files.At(file);
    SetSelector(raw.selector);
    uint8_t tmp;
    for (uint64_t i = 0; i < position; i++) Read(&tmp, sizeof(uint8_t));
    const uint32_t ret = MathLib::Min<uint32_t>(raw.size - position, size);
    Read(buffer, ret);
    return ret;
}
size_t QEMUFileSystem::Write(size_t, const void*, size_t, size_t) {
    return 0;
}
size_t QEMUFileSystem::GetSize(size_t file) {
    return file >= files.GetSize() || files.At(file).free ? 0 : files.At(file).size;
}
MathLib::Array<MathLib::FileInfo> QEMUFileSystem::ReadDirectory(const MathLib::Sequence<char>& path) {
    MathLib::Array<MathLib::FileInfo> ret;
    return Foreach(MathLib::MakeFunctionT<bool, QEMUFileEntry>([&ret, &path](QEMUFileEntry entry) -> bool {
        const MathLib::SingleTypePair<MathLib::String> name = MathLib::RemoveBasePathAndPopFirstPathElement(path, MathLib::String(entry.name));
        if (name.first.IsEmpty()) return false;
        const MathLib::FileInfo info = MathLib::FileInfo(name.second.IsEmpty() ? MathLib::FileInfo::Type::File : MathLib::FileInfo::Type::Directory, name.first);
        return !(ret.Contains(info) || ret.Add(info));
    })) ? MathLib::Array<MathLib::FileInfo>() : ret;
}

#endif