#include "Time.hpp"
#include "Disks.hpp"
#include "Arch/Arch.hpp"
#include "KernelRenderer.hpp"
#include <MathLib.hpp>

// TODO:
extern bool panicOnHost;
bool constructorsCalled = false;
[[gnu::constructor]] void TestConstructors(void) {
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
    if (!vfs.AddFileSystem(VFSEntry(new MemoryFS(), "ramfs"))) Panic("Failed to allocate memory file system");
    size_t iso9660fs = 0;
    for (size_t i = 0; i < disks.GetSize(); i++) {
        ISO9660* iso9660 = new ISO9660(*disks.At(i));
        if (!iso9660) Panic("Failed to allocate file system");
        else if (iso9660->IsValid()) {
            LogString(String("Found ISO9660 file system on disk ") + ToString(i) + '\n');
            if (!vfs.AddFileSystem(VFSEntry(iso9660, String("iso9660fs") + ToString(iso9660fs++)))) Panic("Failed to add file system to VFS");
        }
        else delete iso9660;
    }
    LogString(String("Boot time: ") + Second<num_t>(GetTime()).ToString() + '\n');
    mainTimer->Sleep(eps);
    renderer->Fill(UINT32_MAX);
    #if 0
    panicOnHost = true;
    renderer->Puts<num_t>("Hello, world!", &_binary_src_Lib_zap_light16_psf_start, CreateVector<num_t>(0, 0, 0), CreateVector<num_t>(0, 0, 0), CreateVector<size_t>(1, 1, 1), 0xff0000ff);
    #endif
    if (!renderer->Update()) Panic("Failed to update renderer");
    String command = "";
    LogString("> ");
    while (true) {
        const Event event = renderer->GetEvent();
        if (event.type == Event::Type::KeyPressed) {
            LogChar(event.key);
            if (event.key == '\n') {
                const Array<String> args = Split(command, " ", false);
                if (args.GetSize()) {
                    if (args.At(0) == "echo") {
                        for (size_t i = 1; i < args.GetSize(); i++) LogString(args.At(i) + ' ');
                        LogChar('\n');
                    }
                    else if (args.At(0) == "ls") LogString(vfs.ListFiles(args.GetSize() > 1 ? args.At(1) : ""));
                    else if (args.At(0) == "read") LogString(args.GetSize() < 2 ? "Usage: read <path>\n" : vfs.Open(args.At(1), OpenMode::Read).ReadUntil('\0') + '\n');
                    else if (args.At(0) == "exit") ShutdownArch();
                    else LogString(String("Unknown command: '") + args.At(0) + "'\n");
                }
                command = "";
                LogString("> ");
            }
            else command += event.key;
        }
    }
}