#include <FileSystem/FileSystem.hpp>
#include <Executable/ELF/ELF.hpp>
#include <CommandLine.hpp>
#include <Logger.hpp>

void Main(int argc, char** argv, MathLib::FileSystem& fs) {
    const MathLib::CommandLine cmdLine = MathLib::CommandLine(argc, (const char**)argv);
    MathLib::File file = fs.Open(cmdLine.GetEntry("program"_M).Get("No program provided"), MathLib::OpenMode::Read);
    MathLib::ELF elf;
    if (!elf.Load(file)) MathLib::Panic("Failed to load ELF file");
    LogString("ELF: ");
    LogString(elf.ToString());
    LogChar('\n');
}