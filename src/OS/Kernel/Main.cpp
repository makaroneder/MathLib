#include "VFS.hpp"
#include "Time.hpp"
#include "Disks.hpp"
#include "Memory.hpp"
#include "Arch/Arch.hpp"
#include "Jobs/Shell.hpp"
#include "DateKeeper.hpp"
#include "DebugTextUI.hpp"
#include "FileSystemInfo.hpp"
#include "KernelRenderer.hpp"
#include <FileSystem/ExtendedFileSystem/ExtendedFileSystem.hpp>
#include <FileSystem/MemoryFileSystem/MemoryFileSystem.hpp>
#include <FileSystem/ISO9660/ISO9660.hpp>
#include <FileSystem/FAT/FAT.hpp>
#include <MBR/MBR.hpp>
#include <Logger.hpp>
#include <JSON.hpp>

MathLib::Pair<MathLib::String, size_t> fsTable[] = {
    MathLib::Pair<MathLib::String, size_t>("iso9660fs", 0),
    MathLib::Pair<MathLib::String, size_t>("fatfs", 0),
    MathLib::Pair<MathLib::String, size_t>("extfs", 0),
};
MathLib::Expected<bool> AddFileSystem(MathLib::FileSystem* fs, MathLib::WritableSequence<FileSystemInfo>& fileSystems, const MathLib::String& fsName) {
    if (!fs) return MathLib::Expected<bool>();
    if (!fs->IsValid()) {
        delete fs;
        return false;
    }
    size_t fsIndex = SIZE_MAX;
    for (size_t j = 0; j < SizeOfArray(fsTable); j++) {
        if (fsTable[j].first != fsName) continue;
        fsIndex = j;
        break;
    }
    if (fsIndex == SIZE_MAX) return MathLib::Expected<bool>();
    return fileSystems.Add(FileSystemInfo(fs, fsName + MathLib::ToString(fsTable[fsIndex].second++)));
}
bool InitDisk(MathLib::ByteDevice* disk, const MathLib::Sequence<char>& diskName, MathLib::WritableSequence<FileSystemInfo>& fileSystems);
template <typename T>
MathLib::Expected<bool> AddPartitions(MathLib::ByteDevice* disk, const MathLib::Sequence<char>& diskName, MathLib::WritableSequence<FileSystemInfo>& fileSystems) {
    T partitionManager = T(*disk);
    const MathLib::Array<MathLib::SubByteDevice*> partitions = partitionManager.GetPartitions();
    if (partitions.IsEmpty()) return false;
    const size_t size = partitions.GetSize();
    LogString("Found "_M + MathLib::ToString(size) + " partitions on disk " + diskName + '\n');
    for (size_t i = 0; i < size; i++)
        if (!InitDisk(partitions.AtUnsafe(i), MathLib::CollectionToString(diskName) + '.' + MathLib::ToString(i), fileSystems)) return MathLib::Expected<bool>();
    return true;
}
bool InitDisk(MathLib::ByteDevice* disk, const MathLib::Sequence<char>& diskName, MathLib::WritableSequence<FileSystemInfo>& fileSystems) {
    MathLib::Expected<bool> tmp = AddFileSystem(new MathLib::ISO9660(*disk), fileSystems, "iso9660fs"_M);
    if (tmp.GetOr(true)) return tmp.HasValue();

    tmp = AddFileSystem(new MathLib::FAT(*disk), fileSystems, "fatfs"_M);
    if (tmp.GetOr(true)) return tmp.HasValue();

    tmp = AddFileSystem(new MathLib::ExtendedFileSystem(*disk), fileSystems, "extfs"_M);
    if (tmp.GetOr(true)) return tmp.HasValue();

    tmp = AddPartitions<MathLib::MBR>(disk, diskName, fileSystems);
    if (tmp.GetOr(true)) return tmp.HasValue();

    LogString("No file system found on disk "_M + diskName + '\n');
    return true;
}
void IdleTask(const MainData*) {
    while (true) {}
}
bool constructorsCalled = false;
[[gnu::constructor]] void TestConstructors(void) {
    constructorsCalled = true;
}
extern "C" void _init(void);
extern "C" [[noreturn]] void Main(uintptr_t signature, void* info) {
    _init();
    if (!constructorsCalled) MathLib::Panic("Failed to call global constructors");
    if (!InitArch(signature, info)) MathLib::Panic("Failed to initialize architecture");
    if (!renderer) {
        renderer = new KernelRenderer(0, 0);
        if (!renderer) MathLib::Panic("Failed to allocate renderer");
    }
    if (!textUI) {
        textUI = new DebugTextUI();
        if (!textUI) MathLib::Panic("Failed to allocate debug text UI");
    }
    else if (!textUI->Clear()) MathLib::Panic("Failed to clear text UI");
    if (!mainTimer) MathLib::Panic("Failed to initialize main timer");
    if (true) {
        MathLib::Array<FileSystemInfo> fileSystems = MathLib::MakeArray<FileSystemInfo>(
            FileSystemInfo(new MathLib::MemoryFileSystem(), "ramfs"_M)
        );
        for (size_t i = 0; i < disks.GetSize(); i++)
            if (!InitDisk(disks.AtUnsafe(i), MathLib::ToString(i), fileSystems)) MathLib::Panic("Failed to allocate file system");
        MathLib::Array<MathLib::JSON> configs;
        for (FileSystemInfo& fileSystem : fileSystems) {
            MathLib::JSON json;
            if (json.LoadFromPath(fileSystem, "config/vfs.json"_M) && !configs.Add(json)) MathLib::Panic("Failed to register VFS configuration option");
        }
        if (configs.IsEmpty()) MathLib::Panic("No VFS configuration provided");
        size_t config = 0;
        if (configs.GetSize() != 1) {
            // TODO:
        }
        LogString("Loading VFS configuartion '"_M + configs.AtUnsafe(config).GetChild("name"_M).Get("VFS configuration does not specify its name").GetValue() + "'\n");
        const MathLib::JSON mounts = configs.AtUnsafe(config).GetChild("mounts"_M).Get("VFS configuration does not specify mountpoints");
        for (const MathLib::JSON& mount : mounts) {
            const MathLib::String name = mount.GetChild("name"_M).Get("No file system specified in mountpoint").GetValue();
            const MathLib::String path = mount.GetChild("path"_M).Get("No path specified in mountpoint").GetValue();
            bool found = false;
            for (FileSystemInfo& info : fileSystems) {
                if (info.name != name) continue;
                if (!vfs.AddFileSystem(MathLib::VFSEntry(info.ReleaseFileSystem(), path))) MathLib::Panic("Failed to mount file system");
                found = true;
                break;
            }
            if (!found) LogString("File system '"_M + name + "' not found\n");
        }
    }
    LogString("All disks initialized\n");
    LogString("VFS: {\n");
    if (!vfs.ListFiles(MathLib::logger, ""_M, SIZE_MAX, "\t"_M)) MathLib::Panic("Failed to print VFS");
    LogString("}\n");
    LogString(ReportFreeMemory());
    LogString("Boot time: "_M + MathLib::ToString(MathLib::GetTime()) + "s\n");
    renderer->Fill(renderer->MapColor(0));
    if (!renderer->Update()) MathLib::Panic("Failed to update renderer");
    ArchSetInterrupts(false);
    // LogString("Idle task ID: "_M + MathLib::ToString(ArchAddTask((uintptr_t)IdleTask, nullptr), 10) + '\n');
    LogString("Shell task ID: "_M + MathLib::ToString(ArchAddTask((uintptr_t)ShellTask, nullptr), 10) + '\n');
    ArchSetInterrupts(true);
    while (true) {}
}