#include "ISO9660.hpp"
#include "../../String.hpp"
#include "../../FunctionT.hpp"

namespace MathLib {
    ISO9660::ISO9660(ByteDevice& disk) : PhysicalFileSystem(disk), pvd(ISO9660PrimaryVolumeDescriptor()), files() {
        if (!disk.Seek(16 * 2048, SeekMode::Set)) Panic("Failed to set disk position");
        uint8_t buff[2048];
        while (disk.Tell() < disk.GetSize()) {
            ISO9660VolumeDescriptor* descriptor = (ISO9660VolumeDescriptor*)buff;
            if (!disk.ReadBuffer(buff, 2048)) Panic("Failed to read primary volume descriptor");
            else if (!descriptor->IsValid() || descriptor->type == ISO9660VolumeDescriptor::Type::VolumeDescriptorSetTerminator || descriptor->type == ISO9660VolumeDescriptor::Type::PrimaryVolumeDescriptor) {
                pvd = *(ISO9660PrimaryVolumeDescriptor*)descriptor;
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
    size_t ISO9660::OpenInternal(const Sequence<char>& path, OpenMode mode) {
        // TODO: Create file and overwrite its data
        if (mode == OpenMode::Write) return SIZE_MAX;
        const Expected<ISO9660DirectoryEntry> prev = GetDirectoryEntry(path);
        if (!prev.HasValue()) return SIZE_MAX;
        const ISO9660File ret = ISO9660File(prev.Get(), mode);
        const size_t size = files.GetSize();
        for (size_t i = 0; i < size; i++) {
            if (files.AtUnsafe(i).free) {
                files.AtUnsafe(i) = ret;
                return i;
            }
        }
        return files.Add(ret) ? size : SIZE_MAX;
    }
    bool ISO9660::Close(size_t file) {
        if (file >= files.GetSize() || files.AtUnsafe(file).free) return false;
        files.AtUnsafe(file).free = true;
        return true;
    }
    size_t ISO9660::Read(size_t file, void* buffer, size_t size, size_t position) {
        if (file >= files.GetSize()) return 0;
        const ISO9660DirectoryEntry& raw = files.AtUnsafe(file).entry;
        if (!disk.Seek(raw.extent.little * 2048 + position, SeekMode::Set)) return 0;
        return disk.ReadSizedBuffer(buffer, raw.bytesPerExtent.little < size + position ? raw.bytesPerExtent.little - position : size);
    }
    size_t ISO9660::Write(size_t file, const void* buffer, size_t size, size_t position) {
        if (file >= files.GetSize()) return 0;
        const ISO9660File& raw = files.AtUnsafe(file);
        if (raw.mode != OpenMode::Write || raw.mode != OpenMode::ReadWrite) return 0;
        // TODO: Resize file
        if (raw.entry.bytesPerExtent.little < size + position || !disk.Seek(raw.entry.extent.little * 2048 + position, SeekMode::Set)) return 0;
        return disk.WriteSizedBuffer(buffer, size);
    }
    size_t ISO9660::GetSize(size_t file) {
        return file < files.GetSize() ? files.AtUnsafe(file).entry.bytesPerExtent.little : 0;
    }
    Array<FileInfo> ISO9660::ReadDirectory(const Sequence<char>& path) {
        Expected<ISO9660DirectoryEntry> entry = GetDirectoryEntry(path);
        if (!entry.HasValue() || !entry.Get().directory) return Array<FileInfo>();
        Array<FileInfo> ret;
        return ReadDirectoryEntry(entry.Get(), MakeFunctionT<bool, const ISO9660DirectoryEntry*>([&ret](const ISO9660DirectoryEntry* tmp) -> bool {
            return ret.Add(FileInfo(tmp->directory ? FileInfo::Type::Directory : FileInfo::Type::File, tmp->GetName()));
        })) ? ret : Array<FileInfo>();
    }
    bool ISO9660::ReadDirectoryEntry(const ISO9660DirectoryEntry& parent, const Function<bool, const ISO9660DirectoryEntry*>& function) {
        uint8_t buff[parent.bytesPerExtent.little];
        if (!disk.ReadPositionedBuffer(buff, parent.bytesPerExtent.little, parent.extent.little * 2048)) return false;
        size_t off = 0;
        while (off != parent.bytesPerExtent.little) {
            ISO9660DirectoryEntry* entry = (ISO9660DirectoryEntry*)&buff[off];
            if (!entry->IsValid()) break;
            if (entry->nameLength && (IsAlphaDigit(entry->name[0]) || IsWhiteSpace(entry->name[0]) || entry->name[0] == '_')) {
                uint8_t tmp[entry->length];
                MemoryCopy(buff + off, tmp, entry->length);
                if (!function((const ISO9660DirectoryEntry*)tmp)) return false;
            }
            off += entry->length;
        }
        return true;
    }
    Expected<ISO9660DirectoryEntry> ISO9660::GetDirectoryEntry(const Sequence<char>& path) {
        ISO9660DirectoryEntry prev = pvd.root;
        const Array<String> split = Split(path, '/'_M, false);
        for (const Sequence<char>& name : split) {
            if (name.IsEmpty()) continue;
            bool found = false;
            if (!ReadDirectoryEntry(prev, MakeFunctionT<bool, const ISO9660DirectoryEntry*>([&name, &prev, &found](const ISO9660DirectoryEntry* entry) -> bool {
                if (name == entry->GetName()) {
                    if (found) return false;
                    found = true;
                    prev = *entry;
                }
                return true;
            })) || !found) return Expected<ISO9660DirectoryEntry>();
        }
        return prev;
    }
    bool ISO9660::CreateDirectory(const Sequence<char>& path, bool overwrite) {
        // TODO:
        (void)path;
        (void)overwrite;
        return false;
    }
}