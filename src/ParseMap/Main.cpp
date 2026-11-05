#include <Interfaces/Sequence/ConstantSequence.hpp>
#include <Interfaces/Sequence/SubSequence.hpp>
#include <FileSystem/FileSystem.hpp>
#include <Typedefs.hpp>
#include <Defer.hpp>
#include <iostream>
#include <cxxabi.h>

void Main(int argc, char** argv, MathLib::FileSystem& fs) {
    if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <map file>");
    MathLib::File map = fs.Open(MathLib::String(argv[1]), MathLib::OpenMode::Read);
    size_t num = 0;
    while (map.GetSizeLeft()) {
        num++;
        const MathLib::String line = map.ReadUntil('\n');
        if (!line.StartsWith(MathLib::ConstantSequence<char>(' ', 16))) continue;
        if (!line.StartsWith("0x"_M, 16)) continue;
        bool skip = false;
        for (uint8_t i = 0; i < 16; i++) {
            if (MathLib::IsHexDigit(line.At(16 + 2 + i))) continue;
            skip = true;
            break;
        }
        if (skip) continue;
        if (!line.StartsWith(MathLib::ConstantSequence<char>(' ', 16), 16 + 2 + 16)) continue;
        int status = -1;
        char* const symbol = abi::__cxa_demangle(line.GetValue() + 16 + 2 + 16 + 16, nullptr, 0, &status);
        defer(free(symbol));
        if (!status) std::cout << symbol << std::endl;
        else if (status != -2) MathLib::Panic("Failed to demangle symbol");
    }
}