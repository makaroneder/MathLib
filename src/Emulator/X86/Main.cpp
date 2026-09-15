#include <FileSystem/FileSystem.hpp>
#include <Emulator/X86/X86.hpp>
#include <ExternArray.hpp>
#include <Logger.hpp>

void Main(int argc, char** argv, MathLib::FileSystem& fs) {
    if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <input file>");
    MathLib::File file = fs.Open(MathLib::String(argv[1]), MathLib::OpenMode::Read);
    const size_t size = file.GetSize();
    uint8_t memory[size];
    if (!file.ReadBuffer(memory, size)) MathLib::Panic("Failed to read input file");
    MathLib::X86 x86 = MathLib::X86(MathLib::ExternArray<uint8_t>(memory, size));
    if (!x86.Run()) MathLib::Panic("Failed to update emulator");
    LogString("State: ");
    LogString(x86.ToString());
    LogChar('\n');
}