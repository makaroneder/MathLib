#include "ISO9660.hpp"
#include "../../String.hpp"

ISO9660::ISO9660(ByteDevice& disk) : PhysicalFileSystem(disk), files(Array<ISO9660File>()) {
    if (!disk.Seek(16 * 2048, SeekMode::Set)) Panic("Failed to set disk position");
    uint8_t buff[2048];
    while (true) {
        VolumeDescriptor* descriptor = (VolumeDescriptor*)buff;
        if (!disk.ReadBuffer(buff, 2048)) Panic("Failed to read primary volume descriptor");
        else if (descriptor->type == VolumeDescriptor::Type::PrimaryVolumeDescriptor) {
            pvd = *(PrimaryVolumeDescriptor*)descriptor;
            break;
        }
        else if (descriptor->type == VolumeDescriptor::Type::VolumeDescriptorSetTerminator) {
            pvd.type = VolumeDescriptor::Type::VolumeDescriptorSetTerminator;
            break;
        }
    }
}
bool ISO9660::IsValid(void) const {
    return pvd.IsValid();
}
bool ISO9660::Create(void) {
    // TODO: Create file system
    return false;
}
size_t ISO9660::OpenInternal(const String& path, const OpenMode& mode) {
    // TODO: Create file and overwrite its data
    if (mode == OpenMode::Write) return SIZE_MAX;
    const Expected<DirectoryEntry> prev = GetDirectoryEntry(path);
    if (!prev.HasValue()) return SIZE_MAX;
    const ISO9660File ret = ISO9660File(prev.Get(), mode);
    for (size_t i = 0; i < files.GetSize(); i++) {
        if (files.At(i).free) {
            files.At(i) = ret;
            return i;
        }
    }
    return files.Add(ret) ? files.GetSize() - 1 : SIZE_MAX;
}
bool ISO9660::Close(const size_t& file) {
    if (file >= files.GetSize()) return false;
    files.At(file).free = true;
    return true;
}
size_t ISO9660::Read(const size_t& file, void* buffer, const size_t& size, const size_t& position) {
    if (file >= files.GetSize()) return 0;
    const DirectoryEntry& raw = files.At(file).entry;
    if (!disk.Seek(raw.extent.little * 2048 + position, SeekMode::Set)) return 0;
    return disk.ReadSizedBuffer(buffer, raw.bytesPerExtent.little < size + position ? raw.bytesPerExtent.little - position : size);
}
size_t ISO9660::Write(const size_t& file, const void* buffer, const size_t& size, const size_t& position) {
    if (file >= files.GetSize()) return 0;
    const ISO9660File& raw = files.At(file);
    if (raw.mode != OpenMode::Write || raw.mode != OpenMode::ReadWrite) return 0;
    // TODO: Resize file
    if (raw.entry.bytesPerExtent.little < size + position) return 0;
    if (!disk.Seek(raw.entry.extent.little * 2048 + position, SeekMode::Set)) return 0;
    return disk.WriteSizedBuffer(buffer, size);
}
size_t ISO9660::GetSize(const size_t& file) {
    if (file >= files.GetSize()) return 0;
    return files.At(file).entry.bytesPerExtent.little;
}
Array<FileInfo> ISO9660::ReadDirectory(const String& path) {
    Expected<DirectoryEntry> entry = GetDirectoryEntry(path);
    if (!entry.HasValue() || !entry.Get().directory) return Array<FileInfo>();
    Array<DirectoryEntry*> rawRet = ReadDirectoryEntry(entry.Get());
    Array<FileInfo> ret;
    for (DirectoryEntry*& tmp : rawRet) {
        ret.Add(FileInfo(tmp->directory ? FileInfo::Type::Directory : FileInfo::Type::File, tmp->GetName()));
        delete tmp;
    }
    return ret;
}
Array<DirectoryEntry*> ISO9660::ReadDirectoryEntry(const DirectoryEntry& parent) {
    uint8_t buff[parent.bytesPerExtent.little] = { 0, };
    if (!disk.Seek(parent.extent.little * 2048, SeekMode::Set)) return Array<DirectoryEntry*>();
    if (!disk.ReadBuffer(buff, parent.bytesPerExtent.little)) return Array<DirectoryEntry*>();
    size_t off = 0;
    Array<DirectoryEntry*> ret;
    while (off != parent.bytesPerExtent.little) {
        DirectoryEntry* entry = (DirectoryEntry*)&buff[off];
        if (!entry->IsValid()) break;
        if (entry->nameLength && (IsAlphaDigit(entry->name[0]) || IsWhiteSpace(entry->name[0]) || entry->name[0] == '_')) {
            uint8_t* tmp = new uint8_t[entry->length];
            if (!tmp) {
                for (DirectoryEntry*& entry : ret) delete entry;
                return Array<DirectoryEntry*>();
            }
            for (size_t j = 0; j < entry->length; j++) tmp[j] = buff[j + off];
            if (!ret.Add((DirectoryEntry*)tmp)) {
                for (DirectoryEntry*& entry : ret) delete entry;
                return Array<DirectoryEntry*>();
            }
        }
        off += entry->length;
    }
    return ret;
}
Expected<DirectoryEntry> ISO9660::GetDirectoryEntry(const String& path) {
    DirectoryEntry prev = pvd.root;
    const Array<String> split = Split(path, "/", false);
    for (const String& name : split) {
        if (name.IsEmpty()) continue;
        Array<DirectoryEntry*> subData = ReadDirectoryEntry(prev);
        bool found = false;
        for (DirectoryEntry*& entry : subData) {
            if (name == entry->GetName()) {
                found = true;
                prev = *entry;
                delete entry;
                break;
            }
            else delete entry;
        }
        if (!found) return Expected<DirectoryEntry>();
    }
    return Expected<DirectoryEntry>(prev);
}