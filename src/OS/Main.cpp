#include "VFS.hpp"
#include "Time.hpp"
#include "Disks.hpp"
#include "Memory.hpp"
#include "Arch/Arch.hpp"
#include "Jobs/Idle.hpp"
#include "Jobs/Shell.hpp"
#include "DateKeeper.hpp"
#include "DebugTextUI.hpp"
#include "KernelRenderer.hpp"
#include "Jobs/JobManager.hpp"
#include <FileSystem/MemoryFS/MemoryFS.hpp>
#include <FileSystem/ISO9660/ISO9660.hpp>
#include <FileSystem/FAT/FAT.hpp>
#include <MBR/MBR.hpp>
#include <Logger.hpp>

MathLib::Pair<MathLib::String, size_t> fsTable[] = {
    MathLib::Pair<MathLib::String, size_t>("iso9660fs", 0),
    MathLib::Pair<MathLib::String, size_t>("fatfs", 0),
};
template <typename T>
MathLib::Expected<bool> AddFileSystem(MathLib::ByteDevice* disk, const MathLib::Sequence<char>& diskName, const MathLib::String& fsName) {
    T* fs = new T(*disk);
    if (!fs) return MathLib::Expected<bool>();
    else if (fs->IsValid()) {
        size_t fsIndex = SIZE_MAX;
        for (size_t j = 0; j < SizeOfArray(fsTable); j++) {
            if (fsTable[j].first == fsName) {
                fsIndex = j;
                break;
            }
        }
        if (fsIndex == SIZE_MAX) return MathLib::Expected<bool>();
        const MathLib::String name = fsName + MathLib::ToString(fsTable[fsIndex].second++);
        LogString("Created "_M + name + " on disk " + diskName + '\n');
        if (!vfs.AddFileSystem(MathLib::VFSEntry(fs, name))) return MathLib::Expected<bool>();
    }
    else {
        delete fs;
        return false;
    }
    return true;
}
bool InitDisk(MathLib::ByteDevice* disk, const MathLib::Sequence<char>& diskName);
template <typename T>
MathLib::Expected<bool> AddPartitions(MathLib::ByteDevice* disk, const MathLib::Sequence<char>& diskName) {
    T partitionManager = T(*disk);
    const MathLib::Array<MathLib::SubByteDevice*> partitions = partitionManager.GetPartitions();
    if (partitions.IsEmpty()) return false;
    const size_t size = partitions.GetSize();
    LogString("Found "_M + MathLib::ToString(size) + " partitions on disk " + diskName + '\n');
    for (size_t i = 0; i < size; i++)
        if (!InitDisk(partitions.At(i), MathLib::CollectionToString(diskName) + '.' + MathLib::ToString(i))) return MathLib::Expected<bool>();
    return true;
}
bool InitDisk(MathLib::ByteDevice* disk, const MathLib::Sequence<char>& diskName) {
    MathLib::Expected<bool> tmp = AddFileSystem<MathLib::ISO9660>(disk, diskName, "iso9660fs");
    if (!tmp.HasValue()) return false;
    if (!tmp.Get()) {
        tmp = AddFileSystem<MathLib::FAT>(disk, diskName, "fatfs");
        if (!tmp.HasValue()) return false;
        if (!tmp.Get()) {
            tmp = AddPartitions<MathLib::MBR>(disk, diskName);
            if (!tmp.HasValue()) return false;
            if (!tmp.Get()) LogString("No file system found on disk "_M + diskName + '\n');
        }
    }
    return true;
}
bool constructorsCalled = false;
[[gnu::constructor]] void TestConstructors(void) {
    constructorsCalled = true;
}
uint8_t  stack[4096] __attribute__((aligned(16)));
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
    if (!vfs.AddFileSystem(MathLib::VFSEntry(new MathLib::MemoryFS(), "ramfs"_M))) MathLib::Panic("Failed to allocate memory file system");
    for (size_t i = 0; i < disks.GetSize(); i++)
        if (!InitDisk(disks.At(i), MathLib::ToString(i))) MathLib::Panic("Failed to allocate file system");
    LogString("VFS: {\n"_M + vfs.ListFiles(""_M, SIZE_MAX, "\t"_M) + "}\n");
    LogString("Boot time: "_M + MathLib::ToString(MathLib::GetTime()) + "s\n");
    renderer->Fill(0);
    if (!renderer->Update()) MathLib::Panic("Failed to update renderer");
    if (!InitJobManger()) MathLib::Panic("Failed to initialize job manager");
    if (!AddJob(new Shell())) MathLib::Panic("Failed to allocate shell");
    if (!AddJob(new Idle())) MathLib::Panic("Failed to allocate idle job");
    while (true)
        if (!RunJobs()) MathLib::Panic("Failed to run job queue");
}