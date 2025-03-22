#include "ISO9660.hpp"
#include "../../String.hpp"

namespace MathLib {
    ISO9660::ISO9660(ByteDevice& disk) : PhysicalFileSystem(disk), pvd(ISO9660PrimaryVolumeDescriptor()), files() {
        StartBenchmark
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
        EndBenchmark
    }
    bool ISO9660::IsValid(void) const {
        StartAndReturnFromBenchmark(pvd.IsValid());
    }
    bool ISO9660::Create(void) {
        // TODO: Create file system
        StartBenchmark
        ReturnFromBenchmark(false);
    }
    size_t ISO9660::OpenInternal(const Sequence<char>& path, OpenMode mode) {
        // TODO: Create file and overwrite its data
        StartBenchmark
        if (mode == OpenMode::Write) ReturnFromBenchmark(SIZE_MAX);
        const Expected<ISO9660DirectoryEntry> prev = GetDirectoryEntry(path);
        if (!prev.HasValue()) ReturnFromBenchmark(SIZE_MAX);
        const ISO9660File ret = ISO9660File(prev.Get(), mode);
        for (size_t i = 0; i < files.GetSize(); i++) {
            if (files.At(i).free) {
                files.At(i) = ret;
                ReturnFromBenchmark(i);
            }
        }
        ReturnFromBenchmark(files.Add(ret) ? files.GetSize() - 1 : SIZE_MAX);
    }
    bool ISO9660::Close(size_t file) {
        StartBenchmark
        if (file >= files.GetSize() || files.At(file).free) ReturnFromBenchmark(false);
        files.At(file).free = true;
        ReturnFromBenchmark(true);
    }
    size_t ISO9660::Read(size_t file, void* buffer, size_t size, size_t position) {
        StartBenchmark
        if (file >= files.GetSize()) ReturnFromBenchmark(0);
        const ISO9660DirectoryEntry& raw = files.At(file).entry;
        if (!disk.Seek(raw.extent.little * 2048 + position, SeekMode::Set)) ReturnFromBenchmark(0);
        ReturnFromBenchmark(disk.ReadSizedBuffer(buffer, raw.bytesPerExtent.little < size + position ? raw.bytesPerExtent.little - position : size));
    }
    size_t ISO9660::Write(size_t file, const void* buffer, size_t size, size_t position) {
        StartBenchmark
        if (file >= files.GetSize()) ReturnFromBenchmark(0);
        const ISO9660File& raw = files.At(file);
        if (raw.mode != OpenMode::Write || raw.mode != OpenMode::ReadWrite) ReturnFromBenchmark(0);
        // TODO: Resize file
        if (raw.entry.bytesPerExtent.little < size + position || !disk.Seek(raw.entry.extent.little * 2048 + position, SeekMode::Set)) ReturnFromBenchmark(0);
        ReturnFromBenchmark(disk.WriteSizedBuffer(buffer, size));
    }
    size_t ISO9660::GetSize(size_t file) {
        StartAndReturnFromBenchmark(file < files.GetSize() ? files.At(file).entry.bytesPerExtent.little : 0);
    }
    Array<FileInfo> ISO9660::ReadDirectory(const Sequence<char>& path) {
        StartBenchmark
        Expected<ISO9660DirectoryEntry> entry = GetDirectoryEntry(path);
        if (!entry.HasValue() || !entry.Get().directory) ReturnFromBenchmark(Array<FileInfo>());
        Array<ISO9660DirectoryEntry*> rawRet = ReadDirectoryEntry(entry.Get());
        Array<FileInfo> ret;
        for (ISO9660DirectoryEntry*& tmp : rawRet) {
            if (!ret.Add(FileInfo(tmp->directory ? FileInfo::Type::Directory : FileInfo::Type::File, tmp->GetName()))) {
                delete tmp;
                ReturnFromBenchmark(Array<FileInfo>());
            }
            delete tmp;
        }
        ReturnFromBenchmark(ret);
    }
    Array<ISO9660DirectoryEntry*> ISO9660::ReadDirectoryEntry(const ISO9660DirectoryEntry& parent) {
        StartBenchmark
        uint8_t buff[parent.bytesPerExtent.little];
        if (!disk.Seek(parent.extent.little * 2048, SeekMode::Set) || !disk.ReadBuffer(buff, parent.bytesPerExtent.little)) ReturnFromBenchmark(Array<ISO9660DirectoryEntry*>());
        size_t off = 0;
        Array<ISO9660DirectoryEntry*> ret;
        while (off != parent.bytesPerExtent.little) {
            ISO9660DirectoryEntry* entry = (ISO9660DirectoryEntry*)&buff[off];
            if (!entry->IsValid()) break;
            if (entry->nameLength && (IsAlphaDigit(entry->name[0]) || IsWhiteSpace(entry->name[0]) || entry->name[0] == '_')) {
                uint8_t* tmp = new uint8_t[entry->length];
                if (!tmp) {
                    for (ISO9660DirectoryEntry*& entry : ret) delete entry;
                    ReturnFromBenchmark(Array<ISO9660DirectoryEntry*>());
                }
                for (size_t j = 0; j < entry->length; j++) tmp[j] = buff[j + off];
                if (!ret.Add((ISO9660DirectoryEntry*)tmp)) {
                    for (ISO9660DirectoryEntry*& entry : ret) delete entry;
                    ReturnFromBenchmark(Array<ISO9660DirectoryEntry*>());
                }
            }
            off += entry->length;
        }
        ReturnFromBenchmark(ret);
    }
    Expected<ISO9660DirectoryEntry> ISO9660::GetDirectoryEntry(const Sequence<char>& path) {
        StartBenchmark
        ISO9660DirectoryEntry prev = pvd.root;
        const Array<String> split = Split(path, '/'_M, false);
        for (const Sequence<char>& name : split) {
            if (name.IsEmpty()) continue;
            Array<ISO9660DirectoryEntry*> subData = ReadDirectoryEntry(prev);
            bool found = false;
            for (ISO9660DirectoryEntry*& entry : subData) {
                if (name == entry->GetName()) {
                    found = true;
                    prev = *entry;
                    delete entry;
                    break;
                }
                else delete entry;
            }
            if (!found) ReturnFromBenchmark(Expected<ISO9660DirectoryEntry>());
        }
        ReturnFromBenchmark(Expected<ISO9660DirectoryEntry>(prev));
    }
}