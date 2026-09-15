#include <FileSystem/FileSystem.hpp>
#include <Emulator/AML/AML.hpp>
#include <Logger.hpp>

void Main(int argc, char** argv, MathLib::FileSystem& fs) {
    if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <input file>");
    MathLib::File file = fs.Open(MathLib::String(argv[1]), MathLib::OpenMode::Read);
    const size_t size = file.GetSize();
    uint8_t memory[size];
    if (!file.ReadBuffer(memory, size)) MathLib::Panic("Failed to read input file");
    MathLib::AML aml = MathLib::AML((const MathLib::DSDT*)memory);
    if (!aml.Run()) MathLib::Panic("Failed to update emulator");
    LogString(aml.GetRoot().ToString());
    LogChar('\n');
}