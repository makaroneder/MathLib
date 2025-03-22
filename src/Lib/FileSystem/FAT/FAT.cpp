#include "FAT.hpp"
#include "FATFSInfo.hpp"
#include "../../String.hpp"

namespace MathLib {
    FAT::FAT(ByteDevice& disk) : PhysicalFileSystem(disk), type(Type::None) {
        StartBenchmark
        if (!disk.ReadPositioned<MathLib::FATBootSector>(bootSector, 0)) MathLib::Panic("Failed to read FAT boot sector");
        if (!bootSector.IsValid()) type = Type::None;
        else if (!bootSector.sectorsPerFAT) {
            if (!bootSector.ebr32.IsValid()) type = Type::None;
            else {
                if (bootSector.ebr32.fsInfoCluster && bootSector.ebr32.fsInfoCluster != UINT16_MAX) {
                    const Expected<FATFSInfo> fsInfo = disk.ReadPositioned<FATFSInfo>(ClusterToSector(bootSector.ebr32.fsInfoCluster) * bootSector.bytesPerSector);
                    if (!fsInfo.Get("Failed to read FS info").IsValid()) type = Type::None;
                }
                if (type != Type::None) {
                    sectorsPerFAT = bootSector.ebr32.sectorsPerFAT;
                    type = Type::FAT32;
                    dataSection = bootSector.reservedSectors + sectorsPerFAT * bootSector.fatCount;
                    root = ClusterToSector(bootSector.ebr32.rootCluster);
                }
            }
        }
        else {
            if (!bootSector.ebr.IsValid()) type = Type::None;
            else {
                sectorsPerFAT = bootSector.sectorsPerFAT;
                size_t rootSectors = sizeof(FATDirectoryEntry) * bootSector.rootDirectoryEntries;
                if (rootSectors % bootSector.bytesPerSector) rootSectors += bootSector.bytesPerSector - rootSectors % bootSector.bytesPerSector;
                rootSectors /= bootSector.bytesPerSector;
                root = bootSector.reservedSectors + sectorsPerFAT * bootSector.fatCount;
                dataSection = root + rootSectors;
                const uint32_t clusters = (bootSector.GetSectorCount() - dataSection) / bootSector.sectorsPerCluster;
                type = clusters < 0xff5 ? Type::FAT12 : Type::FAT16;
            }
        }
        EndBenchmark
    }
    bool FAT::IsValid(void) const {
        StartAndReturnFromBenchmark(type != Type::None);
    }
    size_t FAT::ClusterToSector(size_t cluster) const {
        StartAndReturnFromBenchmark((cluster - 2) * bootSector.sectorsPerCluster + dataSection);
    }
    uint32_t FAT::GetNextCluster(size_t cluster, size_t fat) const {
        StartBenchmark
        if (fat >= bootSector.fatCount) return UINT32_MAX;
        uint32_t index;
        switch (type) {
            case Type::FAT12: {
                index = cluster * 3 / 2;
                break;
            }
            case Type::FAT16: {
                index = cluster * 2;
                break;
            }
            case Type::FAT32: {
                index = cluster * 4;
                break;
            }
            default: ReturnFromBenchmark(UINT32_MAX);
        }
        uint32_t ret;
        if (!disk.ReadPositioned<uint32_t>(ret, bootSector.reservedSectors + fat * bootSector.sectorsPerFAT + index)) ReturnFromBenchmark(UINT32_MAX);
        switch (type) {
            case Type::FAT12: {
                ret = *(uint16_t*)&ret;
                ret = cluster % 2 ? ret >> 4 : ret & 0xfff;
                ReturnFromBenchmark(ret < 0xff7 ? ret : UINT32_MAX);
            }
            case Type::FAT16: {
                ret = *(uint16_t*)&ret;
                ReturnFromBenchmark(ret < 0xfff7 ? ret : UINT32_MAX);
            }
            case Type::FAT32: {
                ret &= 0xfffffff;
                ReturnFromBenchmark(ret < 0xffffff7 ? ret : UINT32_MAX);
            }
            default: ReturnFromBenchmark(UINT32_MAX);
        }
    }
    bool FAT::Create(void) {
        // TODO:
        StartBenchmark
        ReturnFromBenchmark(false);
    }
    Expected<FATDirectoryEntry> FAT::GetDirectoryEntry(const Sequence<char>& path) {
        StartBenchmark
        size_t sector = root;
        FATDirectoryEntry prev = FATDirectoryEntry();
        prev.name[0] = '\0';
        if (type == Type::FAT32) {
            prev.SetCluster(bootSector.ebr32.rootCluster);
            prev.name[0] = '/';
        }
        const Array<String> split = Split(path, '/'_M, false);
        uint32_t cluster;
        if (split.IsEmpty()) ReturnFromBenchmark(prev);
        for (const Sequence<char>& name : split) {
            while (true) {
                size_t size = sizeof(FATDirectoryEntry) * bootSector.rootDirectoryEntries;
                if (prev.name[0]) {
                    size = bootSector.bytesPerSector * bootSector.sectorsPerCluster;
                    if (cluster == UINT32_MAX) ReturnFromBenchmark(Expected<FATDirectoryEntry>());
                    sector = ClusterToSector(cluster);
                    cluster = GetNextCluster(cluster, 0);
                }
                uint8_t buffer[size];
                if (!disk.ReadPositionedBuffer(buffer, size, sector * bootSector.bytesPerSector)) ReturnFromBenchmark(Expected<FATDirectoryEntry>());
                bool found = false;
                for (size_t i = 0; i < size; i += sizeof(FATDirectoryEntry)) {
                    const FATDirectoryEntry* entry = (const FATDirectoryEntry*)&buffer[i];
                    if (!entry->name[0]) break;
                    else if (entry->name[0] == FATDirectoryEntry::unusedEntry) continue;
                    if (!entry->IsLongFileName()) {
                        if (name == entry->GetName()) {
                            prev = *entry;
                            cluster = prev.GetCluster();
                            found = true;
                            break;
                        }
                    }
                }
                if (found) break;
                else if (!prev.name[0]) ReturnFromBenchmark(Expected<FATDirectoryEntry>());
            }
        }
        ReturnFromBenchmark(Expected<FATDirectoryEntry>(prev));
    }
    size_t FAT::OpenInternal(const Sequence<char>& path, OpenMode mode) {
        StartBenchmark
        const Expected<FATDirectoryEntry> entry = GetDirectoryEntry(path);
        if (!entry.HasValue()) ReturnFromBenchmark(SIZE_MAX);
        const FATFile ret = FATFile(entry.Get().GetCluster(), entry.Get().size, mode != OpenMode::Read);
        for (size_t i = 0; i < files.GetSize(); i++) {
            if (files.At(i).free) {
                files.At(i) = ret;
                ReturnFromBenchmark(i);
            }
        }
        ReturnFromBenchmark(files.Add(ret) ? files.GetSize() - 1 : SIZE_MAX);
    }
    bool FAT::Close(size_t file) {
        StartBenchmark
        if (file >= files.GetSize() || files.At(file).free) ReturnFromBenchmark(false);
        files.At(file).free = true;
        ReturnFromBenchmark(true);
    }
    size_t FAT::Read(size_t file, void* buffer, size_t size, size_t position) {
        StartAndReturnFromBenchmark((file < files.GetSize() && !files.At(file).free) ? disk.ReadPositionedSizedBuffer(buffer, files.At(file).size < size + position ? files.At(file).size - position : size, ClusterToSector(files.At(file).cluster) * bootSector.bytesPerSector + position) : 0);
    }
    size_t FAT::Write(size_t file, const void* buffer, size_t size, size_t position) {
        // TODO: Resize file
        StartAndReturnFromBenchmark((file < files.GetSize() && !files.At(file).free) ? disk.WritePositionedSizedBuffer(buffer, files.At(file).size < size + position ? files.At(file).size - position : size, ClusterToSector(files.At(file).cluster) * bootSector.bytesPerSector + position) : 0);
    }
    size_t FAT::GetSize(size_t file) {
        StartAndReturnFromBenchmark((file < files.GetSize() && !files.At(file).free) ? files.At(file).size : 0);
    }
    Array<FileInfo> FAT::ReadDirectory(const Sequence<char>& path) {
        StartBenchmark
        Array<FileInfo> ret;
        if (path.IsEmpty() && type != Type::FAT32) {
            size_t size = sizeof(FATDirectoryEntry) * bootSector.rootDirectoryEntries;
            uint8_t buffer[size];
            if (!disk.ReadPositionedBuffer(buffer, size, root * bootSector.bytesPerSector)) ReturnFromBenchmark(Array<FileInfo>());
            for (size_t i = 0; i < size; i += sizeof(FATDirectoryEntry)) {
                const FATDirectoryEntry* entry = (const FATDirectoryEntry*)&buffer[i];
                if (!entry->name[0]) break;
                else if (entry->name[0] == FATDirectoryEntry::unusedEntry) continue;
                if (!entry->IsLongFileName() && entry->name[0] != '.' && !entry->volumeID && !ret.Add(FileInfo(entry->directory ? FileInfo::Type::Directory : FileInfo::Type::File, entry->GetName()))) ReturnFromBenchmark(Array<FileInfo>());
            }
        }
        else {
            const Expected<FATDirectoryEntry> entry = GetDirectoryEntry(path);
            if (!entry.HasValue()) ReturnFromBenchmark(Array<FileInfo>());
            size_t size = bootSector.bytesPerSector * bootSector.sectorsPerCluster;
            uint8_t buffer[size];
            uint32_t cluster = entry.Get().GetCluster();
            while (cluster != UINT32_MAX) {
                const size_t sector = ClusterToSector(cluster);
                cluster = GetNextCluster(cluster, 0);
                if (!disk.ReadPositionedBuffer(buffer, size, sector * bootSector.bytesPerSector)) ReturnFromBenchmark(Array<FileInfo>());
                for (size_t i = 0; i < size; i += sizeof(FATDirectoryEntry)) {
                    const FATDirectoryEntry* entry = (const FATDirectoryEntry*)&buffer[i];
                    if (!entry->name[0]) ReturnFromBenchmark(ret)
                    else if (entry->name[0] == FATDirectoryEntry::unusedEntry) continue;
                    if (!entry->IsLongFileName() && entry->name[0] != '.' && !entry->volumeID && !ret.Add(FileInfo(entry->directory ? FileInfo::Type::Directory : FileInfo::Type::File, entry->GetName()))) ReturnFromBenchmark(Array<FileInfo>());
                }
            }
        }
        ReturnFromBenchmark(ret);
    }
}