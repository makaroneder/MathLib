#include "Shell.hpp"
#include "../VFS.hpp"
#include "../TextUI.hpp"
#include "../Memory.hpp"
#include "../Arch/Arch.hpp"
#include "../DateKeeper.hpp"
#include "../KernelRenderer.hpp"

Shell::Shell(void) : command(), init(true) {}
MathLib::JobState Shell::RunInternal(void) {
    if (init) {
        command = "";
        if (!textUI->Clear() || !textUI->Puts("> ")) return MathLib::JobState::Failed;
        init = false;
    }
    // TODO: Split output of commands so we can read it if it's too long
    // TODO: Add option to chain commands
    const MathLib::Event event = renderer->GetEvent();
    if (event.type == MathLib::Event::Type::KeyPressed && event.pressed) {
        if (event.key == '\b') {
            if (!command.IsEmpty()) {
                if (!textUI->Write<char>('\b')) return MathLib::JobState::Failed;
                command = MathLib::SubString(command, 0, command.GetSize() - 1);
            }
        }
        else if (event.key == '\n') {
            if (!textUI->Write<char>('\n')) return MathLib::JobState::Failed;
            const MathLib::Array<MathLib::String> args = Split(command, " "_M, false);
            if (!args.IsEmpty() && !command.IsEmpty()) {
                MathLib::String output;
                if (args.At(0) == "echo") {
                    for (size_t i = 1; i < args.GetSize(); i++) output += args.At(i) + ' ';
                    output += '\n';
                }
                else if (args.At(0) == "clear") {
                    if (!textUI->Clear()) return MathLib::JobState::Failed;
                }
                else if (args.At(0) == "exit") ShutdownArch();
                else if (args.At(0) == "ls") output = vfs.ListFiles(args.GetSize() > 1 ? args.At(1) : "", args.GetSize() > 2 ? MathLib::StringToNumber(args.At(2)) : 0);
                else if (args.At(0) == "read") output = args.GetSize() < 2 ? "Usage: read <path>\n" : vfs.Open(args.At(1), MathLib::OpenMode::Read).ReadUntil('\0') + '\n';
                else if (args.At(0) == "date") output = dateKeeper->GetDate().ToString() + '\n';
                else if (args.At(0) == "info") output = "Creation year: "_M + MathLib::ToString(Date::currentYear, 10) + '\n';
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
                if (!textUI->Puts(output)) return MathLib::JobState::Failed;
            }
            command = "";
            if (!textUI->Puts("> ")) return MathLib::JobState::Failed;
        }
        else {
            if (!textUI->Write<char>(event.key)) return MathLib::JobState::Failed;
            command += event.key;
        }
    }
    return MathLib::JobState::Running;
}