#include "Time.hpp"
#include "Disks.hpp"
#include "Arch/Arch.hpp"
#include "Scheduler.hpp"
#include "DateKeeper.hpp"
#include "KernelRenderer.hpp"
#include <MathLib.hpp>

MathLib::VFS vfs;
bool constructorsCalled = false;
[[gnu::constructor]] void TestConstructors(void) {
    constructorsCalled = true;
}
void MainTask(const void*, size_t task) {
    MathLib::String command = "";
    LogString(MathLib::String("Running in task ") + MathLib::ToString(task) + '\n');
    LogString("> ");
    while (true) {
        const MathLib::Event event = renderer->GetEvent();
        if (event.type == MathLib::Event::Type::KeyPressed && event.pressed) {
            LogChar(event.key);
            if (event.key == '\n') {
                const MathLib::Array<MathLib::String> args = Split(command, " ", false);
                if (args.GetSize()) {
                    if (args.At(0) == "echo") {
                        for (size_t i = 1; i < args.GetSize(); i++) LogString(args.At(i) + ' ');
                        LogChar('\n');
                    }
                    else if (args.At(0) == "ls") LogString(vfs.ListFiles(args.GetSize() > 1 ? args.At(1) : ""));
                    else if (args.At(0) == "read") LogString(args.GetSize() < 2 ? "Usage: read <path>\n" : vfs.Open(args.At(1), MathLib::OpenMode::Read).ReadUntil('\0') + '\n');
                    else if (args.At(0) == "date") LogString(dateKeeper->GetDate().ToString() + '\n');
                    else if (args.At(0) == "exit") ShutdownArch();
                    else LogString(MathLib::String("Unknown command: '") + args.At(0) + "'\n");
                }
                command = "";
                LogString("> ");
            }
            else if (event.key == '\b') {
                command = MathLib::SubString(command, 0, command.GetSize() - 1);
                LogString(MathLib::String("\n> ") + command);
            }
            else command += event.key;
        }
    }
}
extern "C" [[noreturn]] void Main(uintptr_t signature, void* info) {
    if (!constructorsCalled) MathLib::Panic("Failed to call global constructors");
    if (!InitArch(signature, info)) MathLib::Panic("Failed to initialize architecture");
    if (!renderer) {
        renderer = new KernelRenderer(800, 800);
        if (!renderer) MathLib::Panic("Failed to allocate renderer");
    }
    if (!mainTimer) MathLib::Panic("Failed to initialize main timer");
    if (!vfs.AddFileSystem(MathLib::VFSEntry(new MathLib::MemoryFS(), "ramfs"))) MathLib::Panic("Failed to allocate memory file system");
    size_t iso9660fs = 0;
    for (size_t i = 0; i < disks.GetSize(); i++) {
        MathLib::ISO9660* iso9660 = new MathLib::ISO9660(*disks.At(i));
        if (!iso9660) MathLib::Panic("Failed to allocate file system");
        else if (iso9660->IsValid()) {
            LogString(MathLib::String("Found ISO9660 file system on disk ") + MathLib::ToString(i) + '\n');
            if (!vfs.AddFileSystem(MathLib::VFSEntry(iso9660, MathLib::String("iso9660fs") + MathLib::ToString(iso9660fs++)))) MathLib::Panic("Failed to add file system to VFS");
        }
        else delete iso9660;
    }
    LogString(MathLib::String("Boot time: ") + MathLib::Second<MathLib::num_t>(MathLib::GetTime()).ToString() + '\n');
    renderer->Fill(0);
    if (!renderer->Update()) MathLib::Panic("Failed to update renderer");
    if (!StartScheduler(MathLib::FunctionPointer<void, size_t>(nullptr, &MainTask))) MathLib::Panic("Failed to start scheduler");
    mainTimer->Sleep(MathLib::eps);
    MathLib::Panic("Failed to run main task");
}