#include "Time.hpp"
#include "Disks.hpp"
#include "Arch/Arch.hpp"
#include "KernelRenderer.hpp"
#include <MathLib.hpp>

bool constructorsCalled = false;
__attribute__((constructor)) void TestConstructors(void) {
    constructorsCalled = true;
}
extern "C" [[noreturn]] void Main(uintptr_t signature, void* info) {
    if (!constructorsCalled) Panic("Failed to call global constructors");
    if (!InitArch(signature, info)) Panic("Failed to initialize architecture");
    if (!renderer) {
        renderer = new KernelRenderer(800, 800);
        if (!renderer) Panic("Failed to allocate renderer");
    }
    if (!mainTimer) Panic("Failed to initialize main timer");
    VFS vfs;
    for (ByteDevice*& disk : disks) {
        ISO9660* iso9660 = new ISO9660(*disk);
        if (!iso9660) Panic("Failed to allocate file system");
        if (iso9660->IsValid()) {
            LogString("Found ISO9660 file system\n");
            if (!vfs.AddFileSystem(VFSEntry(iso9660, String("fs") + ToString(vfs.GetFileSystems())))) Panic("Failed to add file system to VFS");
        }
        else delete iso9660;
    }
    LogString(String("VFS:\n") + vfs.ListFiles(""));
    LogString(String("Boot time: ") + Second<num_t>(GetTime()).ToString() + '\n');
    mainTimer->Sleep(eps);
    renderer->Fill(UINT32_MAX);
    if (!renderer->Update()) Panic("Failed to update renderer");
    while (true) {
        const Event event = renderer->GetEvent();
        if (event.type != Event::Type::None) LogString(String("Event: ") + event.ToString() + '\n');
    }
}