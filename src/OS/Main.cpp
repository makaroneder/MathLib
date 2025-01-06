#include "Time.hpp"
#include "Disks.hpp"
#include "Memory.hpp"
#include "Arch/Arch.hpp"
#include "Scheduler.hpp"
#include "DateKeeper.hpp"
#include "DebugTextUI.hpp"
#include "KernelRenderer.hpp"
#include <MathLib.hpp>

MathLib::VFS vfs;
bool constructorsCalled = false;
[[gnu::constructor]] void TestConstructors(void) {
    constructorsCalled = true;
}
MathLib::Expected<char> WaitForKey(void) {
    MathLib::Array<MathLib::Event> events;
    while (true) {
        const MathLib::Event event = renderer->GetEvent();
        if (event.type == MathLib::Event::Type::KeyPressed && event.pressed) {
            for (const MathLib::Event& event : events)
                if (!renderer->AddEvent(event)) return MathLib::Expected<char>();
            return MathLib::Expected<char>(event.key);
        }
        else if (event.type != MathLib::Event::Type::None && !events.Add(event)) return MathLib::Expected<char>();
    }
}
void MainTask(const void*, size_t task) {
    MathLib::String command = "";
    LogString(MathLib::String("Running in task ") + MathLib::ToString(task) + '\n');
    if (!textUI->Puts("> ")) MathLib::Panic("Failed to print data to text UI");
    // TODO: Split output of commands so we can read it if it's too long
    // TODO: Add option to chain commands
    while (true) {
        const MathLib::Event event = renderer->GetEvent();
        if (event.type == MathLib::Event::Type::KeyPressed && event.pressed) {
            if (event.key == '\b') {
                if (!command.IsEmpty()) {
                    if (!textUI->Write<char>('\b')) MathLib::Panic("Failed to print data to text UI");
                    command = MathLib::SubString(command, 0, command.GetSize() - 1);
                }
            }
            else if (event.key == '\n') {
                if (!textUI->Write<char>('\n')) MathLib::Panic("Failed to print data to text UI");
                const MathLib::Array<MathLib::String> args = Split(command, " ", false);
                if (args.GetSize() && !command.IsEmpty()) {
                    MathLib::String output;
                    if (args.At(0) == "echo") {
                        for (size_t i = 1; i < args.GetSize(); i++)
                            output += args.At(i) + ' ';
                        output += '\n';
                    }
                    else if (args.At(0) == "clear") {
                        if (!textUI->Clear()) MathLib::Panic("Failed to clear text UI");
                    }
                    else if (args.At(0) == "exit") ShutdownArch();
                    else if (args.At(0) == "ls") output = vfs.ListFiles(args.GetSize() > 1 ? args.At(1) : "");
                    else if (args.At(0) == "read") output = args.GetSize() < 2 ? "Usage: read <path>\n" : vfs.Open(args.At(1), MathLib::OpenMode::Read).ReadUntil('\0') + '\n';
                    else if (args.At(0) == "date") output = dateKeeper->GetDate().ToString() + '\n';
                    else if (args.At(0) == "memory") output = MathLib::String("Free memory: ") + MathLib::ToString(allocator.GetFreeMemory()) + '\n';
                    else if (args.At(0) == "info") output = MathLib::String("Creation date: ") + __TIMESTAMP__ + '\n';
                    else if (args.At(0) == "dumpMemory") {
                        if (args.GetSize() < 3) output = "Usage: dumpMemory <address> <size> [line size]\n";
                        else {
                            const uintptr_t addr = MathLib::StringToNumber(args.At(1));
                            const size_t size = MathLib::StringToNumber(args.At(2));
                            const size_t lineSize = args.GetSize() < 4 ? 8 : MathLib::StringToNumber(args.At(3));
                            output = MathLib::DumpMemory(addr, size, lineSize);
                        }
                    }
                    else if (args.At(0) == "dumpFile") {
                        if (args.GetSize() < 2) output = "Usage: dumpFile <path> [line size]\n";
                        else {
                            MathLib::File file = vfs.Open(args.At(1), MathLib::OpenMode::Read);
                            const size_t size = file.GetSize();
                            uint8_t buff[size];
                            if (!file.ReadBuffer(buff, size)) output = "Failed to read file\n";
                            const size_t lineSize = args.GetSize() < 3 ? 8 : MathLib::StringToNumber(args.At(2));
                            output = MathLib::DumpMemory((uintptr_t)buff, size, lineSize);
                        }
                    }
                    else output = MathLib::String("Unknown command: '") + args.At(0) + "'\n";
                    if (!textUI->Puts(output)) MathLib::Panic("Failed to print data to text UI");
                }
                command = "";
                if (!textUI->Puts("> ")) MathLib::Panic("Failed to print data to text UI");
            }
            else {
                if (!textUI->Write<char>(event.key)) MathLib::Panic("Failed to print data to text UI");
                command += event.key;
            }
        }
    }
}
extern "C" [[noreturn]] void Main(uintptr_t signature, void* info) {
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