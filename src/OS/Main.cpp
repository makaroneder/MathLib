#include "Time.hpp"
#include "Task.hpp"
#include "Disks.hpp"
#include "Memory.hpp"
#include "Arch/Arch.hpp"
#include "DateKeeper.hpp"
#include "DebugTextUI.hpp"
#include "KernelRenderer.hpp"
#include <MathLib.hpp>

MathLib::VFS vfs;
bool constructorsCalled = false;
[[gnu::constructor]] void TestConstructors(void) {
    constructorsCalled = true;
}
void MainTask(const void*) {
    MathLib::String command = "";
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
                    else if (args.At(0) == "ls") output = vfs.ListFiles(args.GetSize() > 1 ? args.At(1) : "", args.GetSize() > 2 ? MathLib::StringToNumber(args.At(2)) : 0);
                    else if (args.At(0) == "read") output = args.GetSize() < 2 ? "Usage: read <path>\n" : vfs.Open(args.At(1), MathLib::OpenMode::Read).ReadUntil('\0') + '\n';
                    else if (args.At(0) == "date") output = dateKeeper->GetDate().ToString() + '\n';
                    else if (args.At(0) == "memory") output = "Free memory: "_M + MathLib::ToString(allocator.GetFreeMemory()) + '\n';
                    else if (args.At(0) == "info") output = "Creation date: "_M + creationData.ToString() + '\n';
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
                    else output = "Unknown command: '"_M + args.At(0) + "'\n";
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
MathLib::Pair<MathLib::String, size_t> fsTable[] = {
    MathLib::Pair<MathLib::String, size_t>("iso9660fs", 0),
    MathLib::Pair<MathLib::String, size_t>("fatfs", 0),
};
template <typename T>
MathLib::Expected<bool> AddFileSystem(size_t i, const MathLib::String& fsName) {
    T* fs = new T(*disks.At(i));
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
        LogString("Created "_M + name + " on disk " + MathLib::ToString(i) + '\n');
        if (!vfs.AddFileSystem(MathLib::VFSEntry(fs, name))) return MathLib::Expected<bool>();
    }
    else {
        delete fs;
        return MathLib::Expected<bool>(false);
    }
    return MathLib::Expected<bool>(true);
}
uint8_t  stack[4096] __attribute__((aligned(16)));
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
    for (size_t i = 0; i < disks.GetSize(); i++)
        if (!AddFileSystem<MathLib::ISO9660>(i, "iso9660fs").Get("Failed to allocate file system"))
            if (!AddFileSystem<MathLib::FAT>(i, "fatfs").Get("Failed to allocate file system"))
                LogString("No file system found "_M + " on disk " + MathLib::ToString(i) + '\n');
    LogString("VFS: {\n"_M + vfs.ListFiles("", SIZE_MAX, "\t") + "}\n");
    LogString("Boot time: "_M + MathLib::Second<MathLib::num_t>(MathLib::GetTime()).ToString() + '\n');
    renderer->Fill(0);
    if (!renderer->Update()) MathLib::Panic("Failed to update renderer");
    if (!AddTask(Task(MathLib::FunctionPointer<void>(nullptr, &MainTask), stack, SizeOfArray(stack)))) MathLib::Panic("Failed to create kernel task");
    while (true) Schedule();
}